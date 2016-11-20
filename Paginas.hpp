/***********************************************************/
/*   Augusto Cesar Monteiro Silva - 8941234                */
/*   Lucas Helfstein Rocha        - 8802426                */
/***********************************************************/

#include<iostream>

class Pagina {

public:
    bool R;
    bool present;
    int numero_tabela;
    int numero_fis;
    
    Pagina();
    bool operator == (const Pagina &p);
};
