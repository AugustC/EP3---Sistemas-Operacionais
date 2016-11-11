#include<iostream>
#include<fstream>
#include<vector>
#include<cstring>
#include<sstream>
#include<string>
#include "Processos.hpp"
#include "Paginas.hpp"

using namespace std;

// Variavel global
extern int last_pos;

// Gerencia de espaco livre
int FirstFit(int program_size, std::vector<bool> bitmap);
int NextFit(int program_size, std::vector<bool> bitmap);
int BestFit(int tamanho_p, std::vector<bool> bitmap);
int WorstFit(int tamanho_p, std::vector<bool> bitmap);

// Paginacao
void Optimal();
void SecondChance();
void Clock();
void LRU();

// Etc
void zeraR();
void copiaMem_Vir();
void copiaVir_Mem();
void criaArquivoMem(fstream &arquivo_mem, streamsize total);
void criaArquivoVir(fstream &arquivo_vir, streamsize virtual_m);
void fechaArquivos(fstream &file1, fstream &file2);
void escreveArquivoVir(fstream &arquivo_mem, Processo p, std::vector<bool> bitmap);

Processo criaProcesso(string linha, int PID, int gerenciadorMemoria, std::vector<bool> bitmap);

void simulador(ifstream *arq, int gerenciadorMemoria, int paginacao, int intervalo);
