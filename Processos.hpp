#include<iostream>

class Processo {
    int base;
    unsigned int PID;
    std::list<int> p;
    std::list<int> t;

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

    int proximo_tempo() {
        return t.front;
    }

    void elimina_primeiros() {
        p.erase(p.front);
        t.erase(t.front);
    }
};

// Processo A > B, <=> A.t[ind] > B.t[ind]

bool compara (const Processo & a, const Processo & b) {
    return (a.proximo_tempo() < b.proximo_tempo());
}  
