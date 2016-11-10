#   Augusto Cesar Monteiro Silva - 8941234
#   Lucas Helfstein Rocha        - 8802426

CC=g++
CFLAGS=-std=c++11

ep3: ep3.cpp
	$(CC) $(CFLAGS) -o ep3 ep3.cpp

clean:
	rm -f ep3
