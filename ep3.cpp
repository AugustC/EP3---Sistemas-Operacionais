#include<iostream>
#include<cstdlib>
#include<list>
#include<fstream>
#include "Simulador.hpp"

// Prompt
int main(int argc, char* argv[]) {

    int gerenciador = 1;
    int paginacao = 1;
    int intervalo = 1;

    // Carrega arquivo
    string arquivo("teste");
    ifstream f;
    f.open(arquivo.c_str(), ios::in);
    
    simulador(&f, gerenciador, paginacao, intervalo);
    return 0;
}
