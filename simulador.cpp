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
FILE * criaArquivoMem();
FILE * criaArquivoVir();

void simulador(FILE * arquivo, int gerenciadorMemoria, int paginacao, int intervalo);
