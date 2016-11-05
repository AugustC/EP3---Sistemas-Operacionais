#include "Processos.hpp"
#include "Paginas.hpp"
#include<iostream>
#include<fstream>
#include<boost/dynamic_bitset.hpp>
#include<cstring>
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
    int t0 = atoi(std::strtok(linha, " ")); 
    string nome = std::strtok(NULL, " ");
    int tf = atoi(std::strtok(NULL, " "));
    int b = atoi(std::strtok(NULL, " "));
    std::list<int> p, t;

    // p_i = -1 significa que o processo nao esta usando a memoria no tempo t_i
    p.push_back(-1);
    t.push_back(t0);
    linha = std::strtok(NULL, " ");
    while(linha != NULL) {
        p.push_back(atoi(linha));
        linha = std::strtok(NULL, " ");
        t.push_back(atoi(linha));
        linha = std::strtok(NULL, " ");
    }
    p.push_back(-1);
    t.push_back(tf);
    
    Processo proc = Processo(b, PID, p, t);
    return proc;
}


void simulador(ifstream arq, int gerenciadorMemoria, int paginacao, int intervalo){

    int PID = 0;
    string linha;
    std::getline(arq, linha);
    int total = atoi(std::strtok(linha, " "));
    int virtual_m = atoi(std::strtok(NULL, " "));
    int s = atoi(std::strtok(NULL, " "));
    int pag = atoi(std::strtok(NULL, " "));
    std::list<Processo> lista;
    
    fstream file, file2;
    criaArquivoMem(file, total);
    criaArquivoVir(file2, virtual_m);
    boost::dynamic_bitset<> bitmap_mem(total);
    boost::dynamic_bitset<> bitmap_vir(virtual_m);

    while(linha != NULL) {
        if (lista.empty()) {
            Processo p = criaProcesso(linha, PID);
            PID++;
        }
        else {
            int t0 = atoi(std::strtok(linha, " "));
            while (!lista.empty && t0 > lista.front.proximo_tempo()){
                // Pega minimo, mexe na memoria
            };
    }





    

    // Exemplo
    // bitmap_mem[0] = bitmap_mem[1] = bitmap_mem[5] = bitmap_mem[6] = bitmap_mem[10] = bitmap_mem[13] = 1;

    int endereco = FirstFit(proc.limite, bitmap_mem);
    // Criar funcao
    for (int i = endereco; i < endereco + proc.limite; i++)
        bitmap_mem[i] = 1;

    std::cout << FirstFit(proc.limite, bitmap_mem);    
    
    fechaArquivos(file,file2);
    
}
