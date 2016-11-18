#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>
#include<cstring>
#include<sstream>
#include<string>
#include<math.h>
#include "Processos.hpp"
#include "Paginas.hpp"

using namespace std;

// Etc
void imprimeBitmap(vector<bool> bitmap){
    for (int i = 0; i < bitmap.size(); i++)
        cout << bitmap[i];
    cout << "\n";
}

void imprimeEstadoMemoria(vector<bool> bitmap, string arquivo){
    vector<bool>::iterator it;
    cout << "Bitmap: ";
    for (it = bitmap.begin(); it != bitmap.end(); it++)
        cout << *it << " ";
    cout << "\n";
            
    string linha;
    ifstream arq;
    cout << "Arquivo " << arquivo << ": ";
    
    arq.open(arquivo.c_str(), ios::in | ios::binary);
    while(!arq.eof()) {
        int32_t n;
        arq.read((char *)&n, sizeof(int32_t));
        cout << n << " ";
    }
    cout << "\n";
    // while(getline(arq, linha))
    //     cout << linha << "\n";
    arq.close();
    
    return;
}

void zeraR(vector<Pagina> *tabela){
    vector<Pagina>::iterator it, end;
    end = tabela->end();
    
    for (it = tabela->begin(); it != end; it++)
        it->R = false;
}

void criaArquivoMem(ofstream &arquivo_mem, streamsize total) {
    // Cria o arquivo /tmp/ep3.mem e preenche com -1
    arquivo_mem.open("/tmp/ep3.mem", ios::binary | ios::out);

    int32_t um = -1;
    
    for (int i = 0; i < total; i++) {
        arquivo_mem.write((char *)&um, sizeof(int32_t));
        // arquivo_mem.write("-", 1);
        // if ((i+1) != total)
        //     arquivo_mem.write("1", 1);
    }
    arquivo_mem.flush();

}

void criaArquivoVir(ofstream &arquivo_vir, streamsize virtual_m) {
    // Cria o arquivo /tmp/ep3.mem e preenche com -1
    arquivo_vir.open("/tmp/ep3.vir", ios::binary | ios::out);
    int32_t um = -1;
    
    for (int i = 0; i < virtual_m; i++) {
        arquivo_vir.write((char *)&um, sizeof(int32_t));
    //     arquivo_vir.write("-", 1);
    //     if ((i+1) != virtual_m)
    //         arquivo_vir.write("1", 1);
    }
    arquivo_vir.flush();
}

void fechaArquivos(ofstream &file1, ofstream &file2) {
    file1.close();
    file2.close();
}

void escreveArquivoVir(ofstream &arquivo_mem, Processo *p, vector<bool> *bitmap){

    int base = p->pega_endereco() + 1;
    int limite = p->limite;
    string pid = p->getPID();
    uint32_t pidn = stoi(pid);
    const char * pidchar = pid.c_str();
    int len = pid.size();
    int tamanho_b = (*bitmap).size();
    
    // arquivo
    arquivo_mem.seekp(4 * base);
    for (int i = 0; i < limite; i++) {
        arquivo_mem.write((char *)&pidn, sizeof(uint32_t));
    //     if ((i+len) > limite)
    //         arquivo_mem.write(pidchar, limite - i);
    //     else 
    //         arquivo_mem.write(pidchar, len); 
    }
    arquivo_mem.flush();
    
    // bitmap
    for (int i = 0; i < limite; i++)
        (*bitmap)[(i + base) % tamanho_b] = 1;
}

void escreveArquivoMem(ofstream &arquivo_mem, int indice, Processo p, int pag){
    
    int endereco_ini = indice * pag;
    string pid = p.getPID();
    uint32_t pidn = stoi(pid);
    arquivo_mem.seekp(4 * endereco_ini);

    // arquivo
    for (int i = 0; i < pag; i++) {
        arquivo_mem.write((char *)&pidn, sizeof(uint32_t));
        // if ((i+len) > pag)
        //     arquivo_mem.write(pidchar, pag - i);
        // else
        //     arquivo_mem.write(pidchar, len);
    }
    arquivo_mem.flush();
}

vector<Pagina> criaTabela(int quant_paginas){
    // Cria a tabela de paginas
    vector<Pagina> tabela(quant_paginas);
    for (int i = 0; i < tabela.size(); i++) {
        Pagina pag = Pagina();
        pag.numero_tabela = i;
        tabela[i] = pag;
    }
    return tabela;
}

void trocaPaginaTabela(vector<Pagina> *tabela, int ind_pagina_anterior, int ind_pagina_nova, int endereco) {
    (*tabela)[ind_pagina_anterior].present = false;
    (*tabela)[ind_pagina_nova].present = true;
    (*tabela)[ind_pagina_nova].R = true;
    (*tabela)[ind_pagina_nova].numero_fis = endereco;   
}

void clockLRU(vector<Pagina> *tabela, vector<int> *counter, int tam_counter, vector<int> ant_tabela) {
    for (int i = 0; i < counter->size(); i++) {
        (*counter)[i] = (*counter)[i] / 2;
        if ((*tabela)[ant_tabela[i]].R)
            (*counter)[i] += pow(2, tam_counter);
    }
}

void checaIntervalo(int intervalo, int t_atual, vector<Pagina> *tab, vector<int> *count, vector<bool> mem, vector<bool> vir, bool done, vector<int> ant_tabela) {
    static int tempo_anterior = 0, tempo_R = 0;
    const int intervalo_R = 2, tam_counter = 8;

    if (done)
        tempo_anterior = tempo_R = 0;
    
    // Se passou <intervalo> unidades de tempo
    if (t_atual - tempo_anterior >= intervalo) {
        imprimeEstadoMemoria(vir, "/tmp/ep3.vir");
        imprimeEstadoMemoria(mem, "/tmp/ep3.mem");
        cout << "\n";
        tempo_anterior = t_atual;
    }
    // Se passou o tempo necessario para zerar os R's
    if (t_atual - tempo_R >= intervalo_R) {
        clockLRU(tab, count, tam_counter, ant_tabela);
        zeraR(tab);
        tempo_R = t_atual;
    }
}

// Gerencia de espaco livre
int FirstFit(int tamanho_p, vector<bool> bitmap){ 
    // Coloca no primeiro lugar que couber o processo
    // Representado com o numero 1
    int tamanho_b = bitmap.size();
    for (int i = 0; i < tamanho_b; i++)
        if (!bitmap[i]) {
            int j;
            for (j = 0; j < tamanho_p; j++, i++) {
                if (bitmap[i])
                    break;
            }
            if (j == tamanho_p)
                return (i - tamanho_p);
        }
    return (-1);
}

int NextFit(int tamanho_p, vector<bool> bitmap){ 
    // Coloca no primeiro lugar que couber o processo, partindo de onde parou na ultima vez
    // Representado com o numero 2
    int tamanho_b = bitmap.size();
    static int last_pos = 0;
    
    for (int i = 0; i < tamanho_b; i++)
        if (!bitmap[(i + last_pos) % tamanho_b]) {
            int j;
            for (j = 0; j < tamanho_p; j++, i++) {
                if (bitmap[(i + last_pos) % tamanho_b])
                    break;
            }
            if (j == tamanho_p) {
                last_pos = (i + last_pos) % tamanho_b;
                if ((last_pos - tamanho_p) < 0)
                    return tamanho_b + (last_pos - tamanho_p);
                return (last_pos - tamanho_p);
            }
        }
    
    return (-1);
}

int BestFit(int tamanho_p, vector<bool> bitmap){ 
    // Encontra o menor buraco na memoria onde o processo pode ser encaixado
    // Representado com o numero 3
    
    int tamanho = bitmap.size();
    int menor, base_menor, j, aux_base;

    // Menor tamanho inicialmente é o maior possivel
    menor = tamanho + 1;

    // Procurando buracos
    for (int i = 0; i < tamanho; i++) 
        
        // Encontra um buraco
        if (!bitmap[i]) {

            // Guarda o começo do buraco
            aux_base = i;
            
            // Obtem o tamanho do buraco
            for (j = 0; i < tamanho; j++, i++)
                if (bitmap[i])
                    break;
            
            // Se processo cabe no buraco
            if (j >= tamanho_p) {
                
                // Caso especial (talvez precise de algo)
                // if (j == tamanho) {
                //     base_menor = aux_base;
                //     return (base_menor);
                // }
                
                // Se o tamanho do buraco for o menor ate agora, atualiza 
                if (j < menor) {
                    menor = j;
                    base_menor = aux_base;
                }

            }
        }

    return (base_menor);
}

int WorstFit(int tamanho_p, vector<bool> bitmap){ 
    // Encontra o maior buraco na memoria onde o processo pode ser encaixado
    // Representado com o numero 4
    
    int tamanho = bitmap.size();
    int maior, base_maior, j, aux_base;

    // Maior tamanho inicialmente é o menor possivel
    maior = -1;

    // Procurando buracos
    for (int i = 0; i < tamanho; i++) 
        
        // Encontra um buraco
        if (!bitmap[i]) {

            // Guarda o começo do buraco
            aux_base = i;
            
            // Obtem o tamanho do buraco
            for (j = 0; j < tamanho; j++, i++)
                if (bitmap[i])
                    break;
            
            // Se processo cabe no buraco
            if (j >= tamanho_p) {
                
                // Caso especial (talvez precise de algo)
                // if (j == tamanho) {
                //     base_maior = aux_base;
                //     return (base_maior);
                // }
                
                // Se o tamanho do buraco for o maior ate agora, atualiza 
                if (j > maior) {
                    maior = j;
                    base_maior = aux_base;
                }

                // Senão, va em busca de outro buraco
            }
        }

    return (base_maior);
}

// Paginacao
void Optimal(vector<int> *tempo_futuro, vector<Pagina> *tabela, int p, ofstream &arquivo, Processo proc, int pag, vector<int> ant_tabela){
    vector<int>::iterator maximo = max_element((*tempo_futuro).begin(), (*tempo_futuro).end());
    int indice = distance((*tempo_futuro).begin(), maximo);

    // cout << "Quadro de pagina " << indice << " foi trocada. Processo " << proc.getPID() << " utilizou a pagina " << p << "\n";
    
    // Tira a pagina que demorara mais para executar, e coloca a que esta executando
    trocaPaginaTabela(tabela, ant_tabela[indice], p, indice);
    
    (*tempo_futuro)[indice] = 999999; // Inf? Se a pagina nao for mais utilizada, o tempo futuro eh infinito
    list<int>::const_iterator iteradorT, iteradorP;
    for (iteradorT = proc.t.begin(), iteradorP = proc.p.begin(); iteradorT != proc.t.end(); ++iteradorT, ++iteradorP)
        if (pag * p <= *iteradorP + proc.base && *iteradorP + proc.base < pag * (p + 1)) {
            (*tempo_futuro)[indice] = *iteradorT;
            break;
        }
    
    escreveArquivoMem(arquivo, indice, proc, pag);
    
    return;
}

void SecondChance(list<Pagina> *fila, vector<Pagina> *tabela, int p, ofstream &arquivo, Processo proc, int pag){

    while (fila->front().R) {
        fila->front().R = false;
        fila->push_back(fila->front());
        fila->pop_front();
    }

    // cout << "Quadro de pagina " << fila->front().numero_fis << " foi trocada. Processo " << proc.getPID() << " utilizou a pagina " << p << "\n";
    
    int endereco_fisico = fila->front().numero_fis;
    int pagina_anterior = fila->front().numero_tabela;

    trocaPaginaTabela(tabela, pagina_anterior, p, endereco_fisico);

    escreveArquivoMem(arquivo, endereco_fisico, proc, pag);
    
    fila->pop_front();
    fila->push_back((*tabela)[p]);
    return;
}

void Clock(vector<Pagina> *relogio, vector<Pagina> *tabela, int p, ofstream &arquivo, Processo proc, int pag){
    static int i = 0;
    int tam = relogio->size();
    
    while((*relogio)[i].R) {
        (*relogio)[i].R = false;
        i = (i + 1) % tam;
    }

    // cout << "Quadro de pagina " << (*relogio)[i].numero_fis << " foi trocada. Processo " << proc.getPID() << " utilizou a pagina " << p << "\n";

    int endereco_fisico = (*relogio)[i].numero_fis;
    int pagina_anterior = (*relogio)[i].numero_tabela;

    trocaPaginaTabela(tabela, pagina_anterior, p, endereco_fisico);
    escreveArquivoMem(arquivo, endereco_fisico, proc, pag);

    (*relogio)[i] = (*tabela)[p];
    
    return;
}

void LRU(vector<int> *counter, vector<Pagina> *tabela, int p, ofstream &arquivo, Processo proc, int pag, vector<int> ant_tabela) {

    vector<int>::iterator min = min_element(begin(*counter), end(*counter));
    int quadro = distance(begin(*counter), min);
    
    // cout << "Quadro de pagina " << quadro << " foi trocada. Processo " << proc.getPID() << " utilizou a pagina " << p << "\n";

    trocaPaginaTabela(tabela, ant_tabela[quadro], p, quadro);
    escreveArquivoMem(arquivo, quadro, proc, pag);

    (*counter)[quadro] = 0;
    
    return;
}

void deletaProcessoArquivo(ofstream &arquivo, Processo p, int base, vector<bool> *bitmap) {

    int limite = p.limite;
    arquivo.seekp(4 * base);
    int32_t um = -1;
    
    for (int i = 0; i < limite; i++) {
        arquivo.write((char *)&um, sizeof(int32_t));
        // arquivo.write("-", 1);
        // if ((i+1) != limite)
        //     arquivo.write("1", 1);
    }
    arquivo.flush();

    for (int i = 0; i < limite; i++)
        (*bitmap)[i + base] = 0;
}

Processo criaProcesso(string linha, int32_t PID, int gerenciadorMemoria, vector<bool> bitmap) {
    
    istringstream linhastream(linha);
    string token;
    
    getline(linhastream, token, ' ');
    int t0 = atoi(token.c_str());
    getline(linhastream, token, ' ');
    string nome = token.c_str();
    getline(linhastream, token, ' ');
    int tf = atoi(token.c_str());
    getline(linhastream, token, ' ');
    int b = atoi(token.c_str());
    
    list<int> p, t;
    // p_i = -1 significa que o processo nao esta usando a memoria no tempo t_i
    p.push_back(-1);
    t.push_back(t0);

    while(getline(linhastream, token, ' ')) {
        p.push_back(atoi(token.c_str()));
        getline(linhastream, token, ' ');
        t.push_back(atoi(token.c_str()));
    }
    p.push_back(-1);
    t.push_back(tf);
    
    Processo proc = Processo(b, PID, p, t, nome);
    int base = 0;
    if (gerenciadorMemoria == 1) base = FirstFit(proc.limite, bitmap); 
    else if (gerenciadorMemoria == 2) base = NextFit(proc.limite, bitmap); 
    else if (gerenciadorMemoria == 3) base = BestFit(proc.limite, bitmap);
    else if (gerenciadorMemoria == 4) base = WorstFit(proc.limite, bitmap);
    proc.definir_base(base);
    
    return proc;
}

void simulador(ifstream *arq, int gerenciadorMemoria, int paginacao, int intervalo){

    bool done = false;
    uint32_t PID = 0;
    string linha;
    getline(*arq, linha);
    istringstream linhastream(linha);
    string token;

    // Primeira linha do arquivo
    getline(linhastream, token, ' ');
    int total = atoi(token.c_str());
    getline(linhastream, token, ' ');
    int virtual_m = atoi(token.c_str());
    getline(linhastream, token, ' ');
    int s = atoi(token.c_str());
    getline(linhastream, token, ' ');
    int pag = atoi(token.c_str());
 
    ofstream arquivo_fis, arquivo_vir;
    criaArquivoMem(arquivo_fis, total);
    criaArquivoVir(arquivo_vir, virtual_m);
    int quant_maxima_vir = virtual_m/pag; // Quantidade de paginas que cabem na memoria virtual
    int quant_maxima_fis = total/pag;     // Quantidade de paginas que cabem na memoria fisica
    vector<bool> bitmap_mem(quant_maxima_fis, false);
    vector<bool> bitmap_vir(virtual_m, false);
    list<Processo> lista;       // lista de processos que estao executando
    vector<Pagina> tabela = criaTabela(quant_maxima_vir);
    vector<int> ant_tabela(quant_maxima_fis);
    int t0 = 0, tempo_atual, clock_counter = 0;

    // Estruturas de dados para os algoritmos de paginacao
    vector<int> tempo_futuro(quant_maxima_fis); // Optimal
    list<Pagina> fila;                          // Second-chance
    vector<Pagina> relogio(quant_maxima_fis);   // Clock
    vector<int> counter(quant_maxima_fis);      // LRU
    
    // Enquanto 
    while(!done) {

        // Se chegou no final do arquivo, repete o segundo while ate a lista ficar vazia
        if (arq->eof()){    
            t0 = 9999999;       // inf
            done = true;
        }
        
        getline(*arq, linha);
            
        // Lista de processos vazia, coloca o proximo processo na lista    
        if (lista.empty()) {
                        
            Processo p = criaProcesso(linha, PID, gerenciadorMemoria, bitmap_vir);
            PID++;
            escreveArquivoVir(arquivo_vir, &p, &bitmap_vir);
	        lista.push_back(p);

            istringstream linhastream(linha);
            string token;
            getline(linhastream, token, ' ');

            // Verifica o intervalo entre um evento e outro
            tempo_atual = atoi(token.c_str());
            checaIntervalo(intervalo, tempo_atual, &tabela, &counter, bitmap_mem, bitmap_vir, done, ant_tabela);
        }
        
        // Lista contem processos
        else {

            istringstream linhastream(linha);
            string token;
            getline(linhastream, token, ' ');
            t0 = atoi(token.c_str());
            
            // Enquanto houver processos e o primeiro da lista ...... que?
            while (!lista.empty() && t0 > lista.front().proximo_tempo()){

                // Verifica o intervalo entre um evento e outro
                tempo_atual = lista.front().proximo_tempo();
                checaIntervalo(intervalo, tempo_atual, &tabela, &counter, bitmap_mem, bitmap_vir, done, ant_tabela);

                // Pega o p minimo dos processos que estao em execucao
                int pi = lista.front().pega_endereco();
                int pagina = floor(pi/pag);

                // Se o processo acabou neste tempo
                if (lista.front().p_empty()) {
                    
                    deletaProcessoArquivo(arquivo_vir, lista.front(), pi + 1, &bitmap_vir);
                                    
                    for (int i = lista.front().base; i < lista.front().base + lista.front().limite; i++)
                        if (tabela[pagina].present) {
                            int numero_fisico = tabela[pagina].numero_fis;
                            bitmap_mem[numero_fisico] = false;
                            tempo_futuro[numero_fisico] = 999999;                                // Optimal
                            fila.remove(tabela[pagina]);                                         // Second-chance
                            clock_counter = distance(relogio.begin(), find(relogio.begin(), relogio.end(), tabela[pagina])); // Clock
                            clock_counter = (clock_counter + 1) % relogio.size();
                            counter[numero_fisico] = 0;                                          // LRU
                        }
                    
                    lista.pop_front();
                    lista.sort();
                }

                // Senao, acesso de memoria
                else {
                    
                    // Se a pagina nao esta na tabela
                    if (!tabela[pagina].present) {
                        
                        auto livre = find(bitmap_mem.begin(), bitmap_mem.end(), false);
                        
                        // Se tem algum espaco livre na memoria fisica
                        if (livre != bitmap_mem.end()) {
                                                        
                            int ind = distance(bitmap_mem.begin(), livre);
                            bitmap_mem[ind] = true;
                            ant_tabela[ind] = pagina;
                            tabela[pagina].numero_fis = ind;
                            tabela[pagina].present = true;
                            tabela[pagina].R = true;

                            // Optimal
                            if (paginacao == 1) {
                                
                                // Adiciona proximo tempo ao vetor tempo_futuro
                                list<int>::const_iterator iteradorT, iteradorP;
                                tempo_futuro[ind] = 999999; // inf
                                for (iteradorT = lista.front().t.begin(), iteradorP = lista.front().p.begin(); iteradorT != lista.front().t.end(); ++iteradorT, ++iteradorP)
                                    if (pag * pagina <= *iteradorP + lista.front().base && *iteradorP + lista.front().base < pag * (pagina + 1)) {
                                        tempo_futuro[ind] = *iteradorT;
                                        break;
                                    }
                            }
                            
                            // Second-chance
                            else if (paginacao == 2) {
                                fila.push_back(tabela[pagina]);
                            }

                            // Clock
                            else if (paginacao == 3) {
                                relogio[clock_counter] = tabela[pagina];
                                clock_counter = (clock_counter + 1) % relogio.size();
                            }
                            
                            // cout << "Processo " << lista.front().getPID() << " utilizou a memoria " << pi << ".";
                            // cout << "Pagina " << floor(pi/pag) << " foi colocada na memoria fisica.\n";
                            escreveArquivoMem(arquivo_fis, ind, lista.front(), pag);
                        }

                        // Senao, aplica paginacao
                        else {
                            
                            if (paginacao == 1)      Optimal(&tempo_futuro, &tabela, pagina, arquivo_fis, lista.front(), pag, ant_tabela);
                            else if (paginacao == 2) SecondChance(&fila, &tabela, pagina, arquivo_fis, lista.front(), pag);
                            else if (paginacao == 3) Clock(&relogio, &tabela, pagina, arquivo_fis, lista.front(), pag);
                            else if (paginacao == 4) LRU(&counter, &tabela, pagina, arquivo_fis, lista.front(), pag, ant_tabela);
                        }
                    }

                    // Pagina esta na tabela
                    else {
                        tabela[pagina].R = true;
                        
                        // Para o optimal
                        list<int>::const_iterator iteradorT, iteradorP;
                        tempo_futuro[tabela[pagina].numero_fis] = 999999; // inf
                        
                        for (iteradorT = lista.front().t.begin(), iteradorP = lista.front().p.begin(); iteradorT != lista.front().t.end(); ++iteradorT, ++iteradorP)
                            if (pag * pagina <= *iteradorP + lista.front().base && *iteradorP + lista.front().base < pag * (pagina + 1)) {
                                tempo_futuro[tabela[pagina].numero_fis] = *iteradorT;
                                break;
                            }
                    }
                }
                
                lista.sort();
            }

            if (!done) {
                // Adiciona o processo da proxima linha na lista
                Processo p = criaProcesso(linha, PID, gerenciadorMemoria, bitmap_vir);
                PID++;
                escreveArquivoVir(arquivo_vir, &p, &bitmap_vir);
                lista.push_back(p);
                lista.sort();
            }
            
            // Verifica o intervalo entre um evento e outro
            tempo_atual = t0;
            checaIntervalo(intervalo, tempo_atual, &tabela, &counter, bitmap_mem, bitmap_vir, done, ant_tabela);
        }
    }

    fechaArquivos(arquivo_fis,arquivo_vir);    
}
