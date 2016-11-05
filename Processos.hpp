#include<iostream>

class Processo {
	    int base;
	    int limite;
	    unsigned int PID;
	    int* p;
	    int* t;
	    int indice;
	public:
		int pega_endereco() return p[indice] + base;
}


// Implementar comparacao entre Processos, i. e.,
// Processo A > B, <=> A.t[ind] > B.t[ind]

bool compara (const Processo & a, const Processo & b) 
{
	return ( a.indice < b.indice);
}  

// #include <list>
// std::list<Processo> lista;
// lista.sort(compara);  // aproximadamente N logN