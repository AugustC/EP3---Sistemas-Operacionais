#include "Processos.hpp"
#include "Paginas.hpp"
#include<iostream>
#include<fstream>
#include<boost/dynamic_bitset.hpp>
#include<cstring>
#include<sstream>
#include<string>
using namespace std;

// Gerencia de espaco livre
int FirstFit(int program_size, boost::dynamic_bitset<> bitmap){
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
}


int NextFit();
int BestFit();
int WorstFit();

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

Processo criaProcesso(string linha, int PID) {
    
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
    
    Processo proc = Processo(b, PID, p, t);
    return proc;
}


void simulador(ifstream arq, int gerenciadorMemoria, int paginacao, int intervalo){

    int PID = 0;
    std::string linha;
    std::getline(arq, linha);
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
    boost::dynamic_bitset<> bitmap_mem(total);
    boost::dynamic_bitset<> bitmap_vir(virtual_m);
    std::list<Processo> lista;
    
    while(std::getline(linhastream, token, ' ')) {
        if (lista.empty()) {
            Processo p = criaProcesso(linha, PID);
            PID++;
        }
        else {
	    std::istringstream linhastream(linha);
	    std::string token;
	    std::getline(linhastream, token, ' ');
            int t0 = atoi(token.c_str());
            while (!lista.empty() && t0 > lista.front().proximo_tempo()){
                // Pega minimo, mexe na memoria
            };
	}
    }	
    // Exemplo
    // bitmap_mem[0] = bitmap_mem[1] = bitmap_mem[5] = bitmap_mem[6] = bitmap_mem[10] = bitmap_mem[13] = 1;
    
    fechaArquivos(file,file2);
    
}
