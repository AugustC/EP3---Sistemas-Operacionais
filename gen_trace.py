import re
import random
import numpy as np

# Numero de processos que serao gerados
qtd = 10000	

# Tamanho da memoria virtual
mem_vir = 10000

# Tamanho da memoria fisica
mem_fis = 1000

# Tempo total de execucao (tf maximo, um numero grande), por enquanto nao uso isso
total_exec = 170

# Unidade de alocacao
unidade_alocacao = 5

# Tamanho da pagina
tam_pagina = 10

# Temos etc
x = np.zeros ((qtd + 1, 13));
y = np.zeros (5);

# Tempo de entrada
for i in range (1, qtd + 1): 
	x[i][0] = random.randint (0, 45) 
x.sort (axis=0)

# memoria disponivel em ti
mem_ocup = np.zeros(total_exec)

# Tempos de para os traces e acessos
for i in range (1, qtd + 1): 

	# tempo de saida eh o maximo 
	x[i][1] = random.randint (x[i][0], total_exec) 

	# tamanho do processo
	x[i][2] = random.randint (5, 30) 

	# escolhe 5 tempos e ordena
	for j in range (0, 5):
		y[j] = random.randint(x[i][0], x[i][1])
	y.sort();

	# processo vai acessar em um tempo, se naquele momento tiver memoria disponivel
	for j in range (0, 5):
		
		tf = 0

		while y[j] < total_exec and not (mem_ocup[y[j]] + tam_pagina < mem_fis):
			y[j] = y[j] + 1

		# se foi no tempo permitido
		if y[j] < total_exec:

			# atualiza o tf, se necessario
			if y[j] > tf:
				tf = y[j]
				x[i][1] = tf

			# pi, ti
			x[i][2*j+3] = random.randint (0, x[i][2])
			x[i][2*j+4] = y[j]
			mem_ocup[y[j]] = mem_ocup[y[j]] + tam_pagina

		# se cai muito nesse caso, considere aumentar seus parametros...
		else:
			break

	# pode ordenar aqui de novo pq temos garantia de que cabe na memoria
	y.sort();

# header
x[0][0] = mem_fis                # total
x[0][1] = mem_vir                # virtual
x[0][2] = unidade_alocacao       # s
x[0][3] = tam_pagina             # p

np.savetxt ('trace_inicial', x, delimiter = ' ', fmt='%d')

# nomeia os processos, etc
cont = 0
with open('trace_inicial', 'r') as f:
	
	lines = f.readlines()
	linesf = []
	linesf.append(lines[0])
	trace = lines[1:]
    
	for line in trace:

		nums = re.findall('\d+', line)
		linha = nums[0] + ' P' + str(cont) + ' ' + ' '.join(nums[1:]) + '\n'
		linesf.append(linha)
		cont = cont + 1

with open('trace_final', 'w') as f:
    f.writelines(linesf)
