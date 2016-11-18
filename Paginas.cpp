#include<iostream>
#include "Paginas.hpp"

Pagina::Pagina(){
    R = false;
    present = false;
}

bool Pagina::operator == (const Pagina &p) {
    return (p.numero_tabela == numero_tabela);
}
