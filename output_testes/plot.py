#!/usr/bin/python
# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt
import numpy as np

def autolabel(rects, ax):
    # ctrl c ctrl v da API
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x() + rect.get_width()/2., 1.05*height,
                '%.2f' % (height),
                ha='center', va='bottom')

def barplot (col1, col2, col3, col4, nome, legenda, std1=0, std2=0, std3=0, std4=0):
    width = 0.20

    fig, ax = plt.subplots()
    r1 = ax.bar(0, col1, width, color='r', yerr=std1)
    r2 = ax.bar(width, col2, width, color='b', yerr=std2)
    r3 = ax.bar(2 * width, col3, width, color='g', yerr=std3)
    r4 = ax.bar(3 * width, col4, width, color='y', yerr=std4)
    
    ax.set_title(nome)
    ax.legend((r1[0], r2[0], r3[0], r4[0]),legenda)

    autolabel(r1, ax)
    autolabel(r2, ax)
    autolabel(r3, ax)
    autolabel(r4, ax)
    
    plt.show()

FirstFit = []
NextFit = []
BestFit = []
WorstFit = []
for i in range(31):
    first = 'memoria_1_' + str(i) + '.txt'
    FF = np.loadtxt(first)
    FirstFit.append(FF.T[1].mean())
    
    nextf = 'memoria_2_' + str(i) + '.txt'
    NF = np.loadtxt(nextf)
    NextFit.append(NF.T[1].mean())

    bestf = 'memoria_3_' + str(i) + '.txt'
    BF = np.loadtxt(bestf)
    BestFit.append(BF.T[1].mean())

    worstf = 'memoria_4_' + str(i) + '.txt'
    WF = np.loadtxt(worstf)
    WorstFit.append(WF.T[1].mean())

FirstFit = np.asarray(FirstFit)
NextFit  = np.asarray(NextFit)
BestFit  = np.asarray(BestFit)
WorstFit = np.asarray(WorstFit)  
    
barplot(FirstFit.mean(), NextFit.mean(), BestFit.mean(), WorstFit.mean(), 'Tempo necesssário para achar o espaço livre', ('FirstFit', 'NextFit', 'BestFit', 'WorstFit'), FirstFit.std(), NextFit.std(), BestFit.std(), WorstFit.std())

Optimal = np.loadtxt('paginacao_1_0.txt')[1]
Second_Chance = np.loadtxt('paginacao_2_0.txt')[1]
Clock = np.loadtxt('paginacao_3_0.txt')[1]
LRU = np.loadtxt('paginacao_4_0.txt')[1]

barplot(Optimal, Second_Chance, Clock, LRU, 'Pagefaults', ('Optimal', 'Second Chance', 'Clock', 'LRU'))
