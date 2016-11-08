#include<iostream>
#include<list>

class Processo {
    int base;
    unsigned int PID;
    std::list<int> p;
    std::list<int> t;

public:

    int indice;
    int limite;
    
    Processo(int limite, int PID, std::list<int> p, std::list<int> t){
        this->limite = limite;
        this->PID = PID;
        this->p = p;
        this->t = t;
        this->indice = 0;
    }
    
    int pega_endereco() {
        int x = p.front();
        p.pop_front();
        t.pop_front();
        return (x + base);
    }

    void definir_base(int base) {
        this->base = base;
    }

    int proximo_tempo() {
        return t.front();
    }

};

// Processo A > B, <=> A.t[ind] > B.t[ind]

bool compara (const Processo & a, const Processo & b) {
    return (a.proximo_tempo() < b.proximo_tempo());
}  
