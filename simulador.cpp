#include "Processos.hpp"
#include<iostream>

// Gerencia de espaco livre
int FirstFit();
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
fstream criaArquivoMem(total){
    // Cria o arquivo /tmp/ep3.mem e preenche com -1
    fstream arquivo_mem;
    int aux = -1;
    arquivo_mem.open("/tmp/ep3.mem", ios::binary | ios::out | ios::in);
    arquivo_mem.write(&aux, total); // Se total for em bytes
    return arquivo_mem;
}

fstream criaArquivoVir(virtual_m){
    // Cria o arquivo /tmp/ep3.mem e preenche com -1
    fstream arquivo_vir;
    int aux = -1;
    arquivo_vir.open("/tmp/ep3.vir", ios::binary | ios::out | ios::in);
    arquivo_vir.write(&aux, virtual_m);
    return arquivo_vir;
}

void simulador(FILE * arquivo, int gerenciadorMemoria, int paginacao, int intervalo);
