#   Augusto Cesar Monteiro Silva - 8941234
#   Lucas Helfstein Rocha        - 8802426

CC=g++
CFLAGS=-std=c++11
DEPS = Paginas.hpp Processos.hpp Simulador.hpp
OBJ = ep3.o Simulador.o Processos.o Paginas.o

%.o: %.cpp $(DEPS)
	$(CC) -g -c -o  $@ $< $(CFLAGS)

ep3: $(OBJ)
	$(CC) -o ep3 $^ $(CFLAGS)

clean:
	rm -f ep3 *.o
