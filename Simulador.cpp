#include<iostream>
#include<fstream>
#include<vector>
#include<cstring>
#include<sstream>
#include<string>
#include "Processos.hpp"
#include "Paginas.hpp"

using namespace std;

// Gerencia de espaco livre
int FirstFit(int program_size, std::vector<bool> bitmap){ 
    // Coloca no primeiro lugar que couber o processo
    // Representado com o numero 1
    int size = bitmap.size();
    for (int i = 0; i < size; i++)
        if (!bitmap[i]) {
            int j;
            for (j = 0; j < program_size; j++, i++) {
                if (bitmap[i])
                    break;
            }
            if (j == program_size)
                return (i - program_size);
        }
    return (-1);
}

// Variavel externa para o 

int NextFit(int program_size, std::vector<bool> bitmap){ 
    // Coloca no primeiro lugar que couber o processo, partindo de onde parou na ultima vez
    // Representado com o numero 2
    static int last_pos = 0;
    int size = bitmap.size();
    for (int i = 0; i < size; i++)
        if (!bitmap[(i + last_pos) % size]) {
            int j;
            for (j = 0; j < program_size; j++, i++) {
                if (bitmap[(i + last_pos) % size])
                    break;
            }
            if (j == program_size) {
                last_pos = i;
                return (i - program_size);
            }
        }
    return (-1);
}

int BestFit(int tamanho_p, std::vector<bool> bitmap){ 
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
            for (j = i; j < tamanho; j++, i++)
                if (bitmap[j])
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

                // Senão, va em busca de outro buraco
            }
        }

    return (base_menor);
}

int WorstFit(int tamanho_p, std::vector<bool> bitmap){ 
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
            for (j = i; j < tamanho; j++, i++)
                if (bitmap[j])
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
void Optimal();
void SecondChance();
void Clock();
void LRU();

// Etc
void zeraR();
void copiaMem_Vir();
void copiaVir_Mem();
void criaArquivoMem(fstream &arquivo_mem, streamsize total) {
    // Cria o arquivo /tmp/ep3.mem e preenche com -1
    arquivo_mem.open("/tmp/ep3.mem", ios::binary | ios::out);
    for (int i = 0; i < total; i+=2)
        arquivo_mem.write("-1", 2);

}
void criaArquivoVir(fstream &arquivo_vir, streamsize virtual_m) {
    // Cria o arquivo /tmp/ep3.mem e preenche com -1
    arquivo_vir.open("/tmp/ep3.vir", ios::binary | ios::out);
    for (int i = 0; i < virtual_m; i+=2)
        arquivo_vir.write("-1", 2);
}
void fechaArquivos(fstream &file1, fstream &file2) {
    file1.close();
    file2.close();
}
void escreveArquivoVir(fstream &arquivo_mem, Processo p, std::vector<bool> *bitmap){

    int base = p.pega_endereco() + 1;
    int limite = p.limite;
    std::string pid = std::to_string(p.PID);
    const char * pidchar = pid.c_str();
    int len = pid.size();
    arquivo_mem.seekp(base);
    
    for (int i = 0; i < limite; i+=pid.size())
        arquivo_mem.write(pidchar, len);
    for (int i = base; i < limite; i++)
        (*bitmap)[i] = 1;
}

Processo criaProcesso(string linha, int PID, int gerenciadorMemoria, std::vector<bool> bitmap) {
    
    std::istringstream linhastream(linha);
    std::string token;
    
    std::getline(linhastream, token, ' ');
    int t0 = atoi(token.c_str());
    std::getline(linhastream, token, ' ');
    string nome = token.c_str();
    std::getline(linhastream, token, ' ');
    int tf = atoi(token.c_str());
    std::getline(linhastream, token, ' ');
    int b = atoi(token.c_str());
    
    std::list<int> p, t;
    // p_i = -1 significa que o processo nao esta usando a memoria no tempo t_i
    p.push_back(-1);
    t.push_back(t0);

    while(std::getline(linhastream, token, ' ')) {
        p.push_back(atoi(token.c_str()));
	std::getline(linhastream, token, ' ');
        t.push_back(atoi(token.c_str()));
    }
    p.push_back(-1);
    t.push_back(tf);
    
    Processo proc = Processo(b, PID, p, t, nome);
    int base = 0;
    if (gerenciadorMemoria == 1) base = FirstFit(proc.limite, bitmap); 
    if (gerenciadorMemoria == 2) base = NextFit(proc.limite, bitmap); 
    if (gerenciadorMemoria == 3) base = BestFit(proc.limite, bitmap); 
    proc.definir_base(base);
    
    return proc;
}


void simulador(ifstream *arq, int gerenciadorMemoria, int paginacao, int intervalo){

    int PID = 0;
    std::string linha;
    std::getline(*arq, linha);
    std::istringstream linhastream(linha);
    std::string token;
    
    std::getline(linhastream, token, ' ');
    int total = atoi(token.c_str());
    std::getline(linhastream, token, ' ');
    int virtual_m = atoi(token.c_str());
    std::getline(linhastream, token, ' ');
    int s = atoi(token.c_str());
    std::getline(linhastream, token, ' ');
    int pag = atoi(token.c_str());
 
    fstream file, file2;
    criaArquivoMem(file, total);
    criaArquivoVir(file2, virtual_m);
    std::vector<bool> bitmap_mem(total);
    std::vector<bool> bitmap_vir(virtual_m);
    std::list<Processo> lista;
    
    while(std::getline(*arq, linha)) {
        if (lista.empty()) {
            Processo p = criaProcesso(linha, PID, gerenciadorMemoria, bitmap_vir);
            PID++;
            escreveArquivoVir(file2, p, &bitmap_vir);
	    lista.push_back(p);
        }
        else {
	    std::istringstream linhastream(linha);
	    std::string token;
	    std::getline(linhastream, token, ' ');
            int t0 = atoi(token.c_str());
            while (!lista.empty() && t0 > lista.front().proximo_tempo()){
                // Pega minimo dos processos que estao em execucao, mexe na memoria
		
            }
	    Processo p = criaProcesso(linha, PID, gerenciadorMemoria, bitmap_vir);
        escreveArquivoVir(file2, p, &bitmap_vir);
	    PID++;
	    lista.push_back(p);
            
	    // std::cout << "Antes de ordenar\n";
	    // for (std::list<Processo>::iterator it=lista.begin(); it != lista.end(); ++it)
	    //     std::cout << it->proximo_tempo() << "\n";
	    // lista.sort();
	    // std::cout << "Depois de ordenar\n";
	    // for (std::list<Processo>::iterator it=lista.begin(); it != lista.end(); ++it)
	    //     std::cout << it->proximo_tempo() << "\n";
	}
    }
    
    fechaArquivos(file,file2);
    
}
