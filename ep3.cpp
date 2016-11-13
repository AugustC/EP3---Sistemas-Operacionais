#include<iostream>
#include<cstdlib>
#include<list>
#include<fstream>
#include "Simulador.hpp"

#include<sstream>
#include<string>

using namespace std;

// Prompt
int main(int argc, char* argv[]) {

    int gerenciador;
    int paginacao = 1;
    int intervalo = 1;

    string token, linha, entrada;
	ifstream f;

    while (1) {
		
		// prompt
		cout << "(ep3): ";
		getline(cin, entrada);

        // comando
		istringstream linhastream(entrada);        
		getline(linhastream, token, ' ');

		// casos do enunciado
		if (token == "carrega") {
			
			getline(linhastream, token, ' ');
			// carrega arquivo
		    string arquivo(token);
	    	f.open(arquivo.c_str(), ios::in);	
		}	    
	    
	    else if (token == "espaço"){
	    	
	    	getline(linhastream, token, ' ');
	    	gerenciador = atoi(token.c_str());
	    }
	    
		else if (token == "substitui") {
	    	
	    	getline(linhastream, token, ' ');
	    	paginacao = atoi(token.c_str());
	    }

	    else if (token == "executa") {
	    	
	    	getline(linhastream, token, ' ');
	    	intervalo = atoi(token.c_str());
		    simulador(&f, gerenciador, paginacao, intervalo);
	    }

	    else if (token == "sai")
	    	return (0);
		
		else if (token == "\n")
			cout << endl;
		else 
			cout << "Comando desconhecido"<< endl;
		}

    return 0;
}
