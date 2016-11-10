#include<iostream>
#include<list>

class Processo {

    int base;
    std::list<int> p;
    std::list<int> t;

public:

    unsigned int PID;
    std::string nome;
    int limite;
    
    Processo(int limite, int PID, std::list<int> p, std::list<int> t, std::string nome);
    void definir_base(int base);
    int pega_endereco();
    int proximo_tempo() const;
    bool operator<(Processo b) const;
};

