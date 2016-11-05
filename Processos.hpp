#include<iostream>

class Processo {
    int base;
    unsigned int PID;
    int* p;
    int* t;

public:

    int indice;
    int limite;
    
    Processo(int limite, int PID, int * p, int * t){
        this->limite = limite;
        this->PID = PID;
        this->p = p;
        this->t = t;
        this->indice = 0;
    }
    
    int pega_endereco() {
        return (p[indice] + base);
    }

    void definir_base(int base) {
        this->base = base;
    }
};


// Implementar comparacao entre Processos, i. e.,
// Processo A > B, <=> A.t[ind] > B.t[ind]

bool compara (const Processo & a, const Processo & b) {
	return ( a.indice < b.indice);
}  
