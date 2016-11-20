#!/bin/bash

for pagina in 1 2 3 4; do

    for memoria in 1 2 3 4; do

        echo "Memora $memoria e Paginacao $pagina"

        for i in {0..30}; do

            echo "Iteracao $i (Memoria $memoria , Pagina $pagina)"
            ./ep3 trace_final $memoria $pagina 1 > out
            mv output_paginacao.txt output_testes/paginacao_$pagina\_$i.txt
            mv output_memoria.txt output_testes/memoria_$memoria\_$i.txt
        done
    done
done
