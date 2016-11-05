#include "Processos.hpp"
#include "Paginas.hpp"
#include<iostream>
#include<fstream>
#include<boost/dynamic_bitset.hpp>
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
void criaArquivoMem(fstream &arquivo_mem, streamsize total){
    // Cria o arquivo /tmp/ep3.mem e preenche com -1
    arquivo_mem.open("/tmp/ep3.mem", ios::binary | ios::out);
    for (int i = 0; i < total; i+=2)
        arquivo_mem.write("-1", 2);

}
void criaArquivoVir(fstream &arquivo_vir, streamsize virtual_m){
    // Cria o arquivo /tmp/ep3.mem e preenche com -1
    arquivo_vir.open("/tmp/ep3.vir", ios::binary | ios::out);
    for (int i = 0; i < virtual_m; i+=2)
        arquivo_vir.write("-1", 2);
}
void fechaArquivos(fstream &file1, fstream &file2){
    file1.close();
    file2.close();
}

void simulador(ifstream arquivo, int gerenciadorMemoria, int paginacao, int intervalo){

    int p = 0;
    int t = 0;
    Processo proc = Processo(3, 1, &p, &t);

    int total = atoi(argv[1]);
    int virtual_m = atoi(argv[2]);
    
    fstream file, file2;
    criaArquivoMem(file, total);
    criaArquivoVir(file2, virtual_m);
    boost::dynamic_bitset<> bitmap_mem(total);
    boost::dynamic_bitset<> bitmap_vir(virtual_m);

    // Exemplo
    // bitmap_mem[0] = bitmap_mem[1] = bitmap_mem[5] = bitmap_mem[6] = bitmap_mem[10] = bitmap_mem[13] = 1;

    int endereco = FirstFit(proc.limite, bitmap_mem);
    // Criar funcao
    for (int i = endereco; i < endereco + proc.limite; i++)
        bitmap_mem[i] = 1;

    std::cout << FirstFit(proc.limite, bitmap_mem);    
    
    fechaArquivos(file,file2);
    
}
