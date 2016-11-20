#include<iostream>
#include<fstream>
#include<vector>
#include<cstring>
#include<sstream>
#include<string>

#include "Processos.hpp"
#include "Paginas.hpp"

using namespace std;


// Etc
void imprimeBitmap(vector<bool> bitmap);
void imprimeEstadoMemoria(vector<bool> bitmap, string arquivo);
void zeraR(vector<Pagina> *tabela);
void copiaMem_Vir();
void copiaVir_Mem();
void criaArquivoMem(ofstream &arquivo_mem, streamsize total);
void criaArquivoVir(ofstream &arquivo_vir, streamsize virtual_m);
void fechaArquivos(ofstream &file1, ofstream &file2);
void escreveArquivoVir(ofstream &arquivo_mem, Processo *p, vector<bool> *bitmap);
void escreveArquivoMem(ofstream &arquivo_mem, int indice, Processo p, int pag);

void escreveOutputPaginacao(int alg_pag, int pagefaults);
void escreveOutputMemoria(int alg_mem, clock_t tempo);

Processo criaProcesso(string linha, int PID, int gerenciadorMemoria, vector<bool> bitmap);
void deletaProcessoArquivo(ofstream &arquivo, Processo p, int base, vector<bool> *bitmap);
vector<Pagina> criaTabela(int quant_paginas);
void trocaPaginaTabela(vector<Pagina> *tabela, int ind_pagina_anterior, int ind_pagina_nova, int endereco);
void clockLRU(vector<Pagina> *tabela, vector<int> *counter, int tam_counter, vector<int> ant_tabela);
void checaIntervalo(int intervalo, int t_atual, vector<Pagina> *tab, vector<int> *count, vector<bool> mem, vector<bool> vir, bool done, vector<int> ant_tabela);
    
// Gerencia de espaco livre
int FirstFit(int tamanho_p, vector<bool> bitmap);
int NextFit(int tamanho_p, vector<bool> bitmap);
int BestFit(int tamanho_p, vector<bool> bitmap);
int WorstFit(int tamanho_p, vector<bool> bitmap);

// Paginacao
void Optimal(vector<int> *tempo_futuro, vector<Pagina> *tabela, int p, ofstream &arquivo, Processo proc, int pag, vector<int> ant_tabela);
void SecondChance(list<Pagina> *fila, vector<Pagina> *tabela, int p, ofstream &arquivo, Processo proc, int pag);
void Clock(vector<Pagina> *relogio, vector<Pagina> *tabela, int p, ofstream &arquivo, Processo proc, int pag);
void LRU(vector<int> *counter, vector<Pagina> *tabela, int p, ofstream &arquivo, Processo proc, int pag, vector<int> ant_tabela);

// Simulador
void simulador(ifstream *arq, int gerenciadorMemoria, int paginacao, int intervalo);
