#include<iostream>
#include<list>

class Processo {

    unsigned int PID;
    
public:

    std::list<int> p;
    std::list<int> t;
    int base;
    std::string nome;
    int limite;
    
    Processo(int limite, int PID, std::list<int> p, std::list<int> t, std::string nome);
    void definir_base(int base);
    int pega_endereco();
    std::string getPID();
    int proximo_tempo() const;
    bool p_empty();
    bool operator<(Processo b) const;
};

