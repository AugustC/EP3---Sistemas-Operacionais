import random
import numpy as np

# Aqui esta hardcoded, mas esse 10000 eh a qtd de processos
x = np.zeros ((10001, 14));
y = np.zeros (5);

for i in range (1, 10001): # tamanho hardcoded
        x[i][0] = random.randint (0, 45) #tempo de entrada
x.sort (axis=0)

for i in range (1, 10001): #aqui tb
        x[i][1] = i
        x[i][2] = random.randint (x[i][0], x[i][0]+15) #tempo de saida
        x[i][3] = random.randint (5, 30) #tamanho
        for j in range (0, 5):
                y[j] = random.randint (x[i][0], x[i][2])
        y.sort();
        for j in range (0, 5):
                x[i][2*j+4] = random.randint (0, x[i][3])
                x[i][2*j+5] = y[j]

x[0][0] = 300                   # total
x[0][1] = 300000                # virtual
x[0][2] = 5                     # s
x[0][3] = 10                    # p

np.savetxt ('trace_inicial', x, delimiter = ' ', fmt='%d')

cont = 0
with open('trace_inicial', 'r') as f:

        lines = f.readlines()
        linesf = []

        lines = [line[:1] + ' P' + line[1:] for line in lines]

        for line in lines:
                linha = line[:3] + str(cont) + line[3:]
                linesf.append(linha)
                cont = cont + 1

with open('trace_final', 'w') as f:
        f.writelines(linesf)
