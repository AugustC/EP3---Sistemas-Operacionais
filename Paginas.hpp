#include<iostream>

class Pagina {

public:
    bool R;
    bool M;
    bool present;
    int numero_tabela;
    int numero_fis;
    
    Pagina();
    bool operator == (const Pagina &p);
};
