#include<iostream>
#include<list>
#include "Processos.hpp"
    
Processo::Processo(int lim, int pid_in, std::list<int> p_in, std::list<int> t_in, std::string nome_in){
    limite = lim;
    PID = pid_in;
    p = p_in;
    t = t_in;
    nome = nome_in;
}

void Processo::definir_base(int base) {
    this->base = base;
}

int Processo::pega_endereco() {
    int x = p.front();
    p.pop_front();
    t.pop_front();
    return (x + base);
}

int Processo::proximo_tempo() const {
    return t.front();
}

bool Processo::operator<(Processo b) const {
    return proximo_tempo() > b.proximo_tempo();
}  
