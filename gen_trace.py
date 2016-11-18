import random
import numpy as np

x = np.zeros ((100, 13));
y = np.zeros (5);

for i in range (0, 100):
	x[i][0] = random.randint (0, 45) #tempo de entrada
x.sort (axis=0)

for i in range (0, 100):
	x[i][1] = random.randint (x[i][0], x[i][0]+15) #tempo de saida
	x[i][2] = random.randint (5, 30) #tamanho
	for j in range (0, 5):
		y[j] = random.randint (x[i][0], x[i][1])
	y.sort();
	for j in range (0, 5):
		x[i][2*j+3] = y[j]
		x[i][2*j+4] = random.randint (0, x[i][2])


np.savetxt ('trace_test.txt', x, delimiter = ' ', fmt='%d')