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

void imprimeEstadoMemoria(vector<bool> bitmap, fstream &arquivo){
    return;
}

void zeraR(vector<Pagina> *tabela){
    vector<Pagina>::iterator it, end;
    end = tabela->end();
    
    for (it = tabela->begin(); it != end; it++)
        it->R = false;
}

void criaArquivoMem(fstream &arquivo_mem, streamsize total) {
    // Cria o arquivo /tmp/ep3.mem e preenche com -1
    arquivo_mem.open("/tmp/ep3.mem", ios::binary | ios::out);
    
    for (int i = 0; i < total; i+=2) {
        arquivo_mem.write("-", 1);
        if ((i+1) != total)
            arquivo_mem.write("1", 1);
    }

}

void criaArquivoVir(fstream &arquivo_vir, streamsize virtual_m) {
    // Cria o arquivo /tmp/ep3.mem e preenche com -1
    arquivo_vir.open("/tmp/ep3.vir", ios::binary | ios::out);
    
    for (int i = 0; i < virtual_m; i+=2) {
        arquivo_vir.write("-", 1);
        if ((i+1) != virtual_m)
            arquivo_vir.write("1", 1);
    }
}

void fechaArquivos(fstream &file1, fstream &file2) {
    file1.close();
    file2.close();
}

void escreveArquivoVir(fstream &arquivo_mem, Processo *p, vector<bool> *bitmap){

    int base = p->pega_endereco() + 1;
    int limite = p->limite;
    string pid = p->getPID();
    const char * pidchar = pid.c_str();
    int len = pid.size();
    int tamanho_b = (*bitmap).size();
    arquivo_mem.seekp(base);
    
    // arquivo
    arquivo_mem.seekp(base);
    for (int i = 0; i < limite; i+=len) {
        if ((i+len) > limite)
            arquivo_mem.write(pidchar, limite - i);
        else 
            arquivo_mem.write(pidchar, len); 
    }

    // bitmap
    for (int i = 0; i < limite; i++)
        (*bitmap)[(i + base) % tamanho_b] = 1;
}

void escreveArquivoMem(fstream &arquivo_mem, int indice, Processo p, int pag){
    
    int endereco_ini = indice * pag;
    string pid = p.getPID();
    const char * pidchar = pid.c_str();
    int len = pid.size();
    arquivo_mem.seekp(endereco_ini);

    // arquivo
    for (int i = 0; i < pag; i+=len) {
        if ((i+len) > pag)
            arquivo_mem.write(pidchar, pag - i);
        else
            arquivo_mem.write(pidchar, len);
    }
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
void Optimal(vector<int> *tempo_futuro, vector<Pagina> *tabela, int p, fstream &arquivo, Processo proc, int pag, vector<int> ant_tabela){
    vector<int>::iterator maximo = max_element((*tempo_futuro).begin(), (*tempo_futuro).end());
    int indice = distance((*tempo_futuro).begin(), maximo);

    cout << "Quadro de pagina " << indice << " foi trocada. Processo " << proc.getPID() << " utilizou a pagina " << p << "\n";
    
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

void SecondChance(list<Pagina> *fila, vector<Pagina> *tabela, int p, fstream &arquivo, Processo proc, int pag){

    while (fila->front().R) {
        fila->front().R = false;
        fila->push_back(fila->front());
        fila->pop_front();
    }

    cout << "Quadro de pagina " << fila->front().numero_fis << " foi trocada. Processo " << proc.getPID() << " utilizou a pagina " << p << "\n";
    
    int endereco_fisico = fila->front().numero_fis;
    int pagina_anterior = fila->front().numero_tabela;

    trocaPaginaTabela(tabela, pagina_anterior, p, endereco_fisico);

    escreveArquivoMem(arquivo, endereco_fisico, proc, pag);
    
    fila->pop_front();
    fila->push_back((*tabela)[p]);
    return;
}

void Clock(vector<Pagina> *relogio, vector<Pagina> *tabela, int p, fstream &arquivo, Processo proc, int pag){
    static int i = 0;
    int tam = relogio->size();
    
    while((*relogio)[i].R) {
        (*relogio)[i].R = false;
        i = (i + 1) % tam;
    }

    cout << "Quadro de pagina " << (*relogio)[i].numero_fis << " foi trocada. Processo " << proc.getPID() << " utilizou a pagina " << p << "\n";

    int endereco_fisico = (*relogio)[i].numero_fis;
    int pagina_anterior = (*relogio)[i].numero_tabela;

    trocaPaginaTabela(tabela, pagina_anterior, p, endereco_fisico);

    escreveArquivoMem(arquivo, endereco_fisico, proc, pag);

    (*relogio)[i] = (*tabela)[p];
    
    return;
}

void LRU(){
    return;
}


void deletaProcessoArquivo(fstream &arquivo, Processo p, int base, vector<bool> *bitmap) {

    int limite = p.limite;
    string pid = p.getPID();
    int len = pid.size();
    arquivo.seekp(base);

    for (int i = 0; i < limite; i += 2) {
        arquivo.write("-", 1);
        if ((i+1) != limite)
            arquivo.write("1", 1);
    }

    for (int i = 0; i < limite; i++)
        (*bitmap)[i + base] = 0;
}

Processo criaProcesso(string linha, int PID, int gerenciadorMemoria, vector<bool> bitmap) {
    
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

    int PID = 0;
    string linha;
    getline(*arq, linha);
    istringstream linhastream(linha);
    string token;
    
    getline(linhastream, token, ' ');
    int total = atoi(token.c_str());
    getline(linhastream, token, ' ');
    int virtual_m = atoi(token.c_str());
    getline(linhastream, token, ' ');
    int s = atoi(token.c_str());
    getline(linhastream, token, ' ');
    int pag = atoi(token.c_str());
 
    fstream arquivo_fis, arquivo_vir;
    criaArquivoMem(arquivo_fis, total);
    criaArquivoVir(arquivo_vir, virtual_m);
    int quant_maxima_vir = virtual_m/pag; // Quantidade de paginas que cabem na memoria virtual
    int quant_maxima_fis = total/pag;
    vector<bool> bitmap_mem(quant_maxima_fis);
    vector<bool> bitmap_vir(virtual_m);
    list<Processo> lista;
    vector<Pagina> tabela = criaTabela(quant_maxima_vir);
    vector<int> tempo_futuro(quant_maxima_fis);
    vector<int> ant_tabela(quant_maxima_fis);
    list<Pagina> fila;
    vector<Pagina> relogio;
    int tempo_anterior, tempo_R, intervalo_R;
    tempo_anterior = tempo_R = 0;
    
    while(getline(*arq, linha)) {
        
        if (lista.empty()) {
            Processo p = criaProcesso(linha, PID, gerenciadorMemoria, bitmap_vir);
            PID++;
            escreveArquivoVir(arquivo_vir, &p, &bitmap_vir);
	    lista.push_back(p);
        }
        
        else {

            istringstream linhastream(linha);
            string token;
            getline(linhastream, token, ' ');
            int t0 = atoi(token.c_str());

            if (t0 - tempo_anterior > intervalo) {
                imprimeEstadoMemoria(bitmap_vir, arquivo_vir);
                imprimeEstadoMemoria(bitmap_mem, arquivo_fis);
            }

            if (t0 - tempo_R > intervalo_R) zeraR(&tabela);
            
            while (!lista.empty() && t0 > lista.front().proximo_tempo()){
                // Pega o p minimo dos processos que estao em execucao
                
                int pi = lista.front().pega_endereco();

                // Se o processo acabou neste tempo
                if (lista.front().p_empty()) {
                    deletaProcessoArquivo(arquivo_vir, lista.front(), pi + 1, &bitmap_vir);
                    
                    for (int i = lista.front().base; i < lista.front().base + lista.front().limite; i++)
                        if (tabela[floor(i/ pag)].present) {
                            bitmap_mem[tabela[floor(i/pag)].numero_fis] = false;
                            tempo_futuro[tabela[floor(i/pag)].numero_fis] = 999999; // Optimal
                            fila.remove(tabela[floor(i/pag)]); // Second-chance
                            relogio.erase(find(relogio.begin(), relogio.end(), tabela[floor(i/pag)])); // Clock
                        }
                    
                    lista.pop_front();
                    lista.sort();
                }

                // Paginacao
                else {
                    int pagina = floor(pi / pag);
                    if (!tabela[pagina].present) {
                        auto livre = find(bitmap_mem.begin(), bitmap_mem.end(), false);
                        
                        if (livre != bitmap_mem.end()) {
                            // Tem algum espaco livre na memoria fisica
                            int ind = distance(bitmap_mem.begin(), livre);
                            bitmap_mem[ind] = true;
                            ant_tabela[ind] = pagina;
                            tabela[pagina].numero_fis = ind;
                            tabela[pagina].present = true;

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
                                relogio.insert(relogio.end(), tabela[pagina]);
                            }
                            
                            cout << "Processo " << lista.front().getPID() << " utilizou a memoria " << pi << ".";
                            cout << "Pagina " << floor(pi/pag) << " foi colocada na memoria fisica.\n";
                            escreveArquivoMem(arquivo_fis, ind, lista.front(), pag);
                        }
                        else {
                            // Paginacao
                            if (paginacao == 1)      Optimal(&tempo_futuro, &tabela, pagina, arquivo_fis, lista.front(), pag, ant_tabela);
                            else if (paginacao == 2) SecondChance(&fila, &tabela, pagina, arquivo_fis, lista.front(), pag);
                            else if (paginacao == 3) Clock(&relogio, &tabela, pagina, arquivo_fis, lista.front(), pag);
                            else if (paginacao == 4) LRU();
                        }
                    }
                    else {
                        tabela[pagina].R = true;
                        
                        list<int>::const_iterator iteradorT, iteradorP;
                        tempo_futuro[tabela[pagina].numero_fis] = 999999; // inf
                        for (iteradorT = lista.front().t.begin(), iteradorP = lista.front().p.begin(); iteradorT != lista.front().t.end(); ++iteradorT, ++iteradorP)
                            if (pag * pagina <= *iteradorP + lista.front().base && *iteradorP + lista.front().base < pag * (pagina + 1)) {
                                tempo_futuro[tabela[pagina].numero_fis] = *iteradorT;
                                break;
                            }
                        // Teste do Optimal
                        // for (int i = 0; i < tempo_futuro.size(); i++)
                        //     cout << tempo_futuro[i] << " ";
                        // cout << "\n";

                        
                    }
                    
                }
                lista.sort();
            }
            Processo p = criaProcesso(linha, PID, gerenciadorMemoria, bitmap_vir);
            escreveArquivoVir(arquivo_vir, &p, &bitmap_vir);
            PID++;
            lista.push_back(p);
            lista.sort();
        }
    }
    
    fechaArquivos(arquivo_fis,arquivo_vir);
    
}
