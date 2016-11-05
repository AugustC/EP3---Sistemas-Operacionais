#include<iostream>
#include<cstdlib>
#include<boost/dynamic_bitset.hpp>
#include<list>
#include "Simulador.cpp"
#include<fstream>

// Prompt
int main(int argc, char* argv[]) {

    int gerenciador = 1;
    int paginacao = 1;
    int intervalo = 1;

    // Carrega arquivo
    char * arquivo = "teste";
    ifstream f;
    f.open(arquivo, ios::in);
    
    simulador(f, gerenciador, paginacao, intervalo);
    return 0;
}
