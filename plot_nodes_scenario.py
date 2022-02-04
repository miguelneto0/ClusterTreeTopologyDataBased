import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# dimensoes do ambiente
envirX = 40 # eixo X do ambiente monitorado
envirY = 35  # eixo Y do ambiente monitorado

# classe Node
class Node():
    def __init__(self):
        models = ["SkyMote","MicazMote","TelosB"]
        drates = [0.05,0.2]
        self.nodeID = np.random.randint(0,101)
        self.model = np.random.choice(models)
        self.assoc = False
        self.posX = np.random.randint(0,envirX)
        self.posY = np.random.randint(0,envirY)
        self.datarate = np.random.choice(drates)
        self.radius = 10
        self.isCH = False
        self.child = []
        self.childCH = []
        self.hasParent = False
        self.parent = None

    def checkAdjNodes(self):
        # if <= sqrt(dX^2 + dY^2)
        rangeRF = np.pi*(self.radius)**2
        return rangeRF

    def forcePosition(self, x, y):
        self.posX = x
        self.posY = y
    
    def findNodes(self, nodes_list, CHnode,datarate):
        listNodesAchiev = CHnode.nodesAchievable(nodes_list)
        listNodesFound = []
        d=0
        if datarate == 'r':
            d=0.2
        elif datarate == 'b':
            d=0.05

        for n in listNodesAchiev:
            if n.datarate == d and n.assoc == False and n.nodeID != CHnode.nodeID:
                n.associate(n,CHnode)
                listNodesFound.append(n)
        return listNodesFound

    def associate(self,n,CH):
        if(n.assoc == False):
            n.assoc = True
            CH.isCH = True
            CH.child.append(n)
            n.parent = CH
            n.hasParent = True
      
    def farawayCH(self):
        lista_dist = []
        for i in range(len(self.child)):
            res = (self.posX - self.child[i].posX)**2 + (self.posY - self.child[i].posY)**2
            dist = np.sqrt(res)
            lista_dist.append(dist)
        if len(lista_dist) > 0:
            m = max(lista_dist)
            p = lista_dist.index(m)
            return self.child[p]
    
    def selectChildCH(self):
        for n in self.child:
            if n.isCH == False and n == self.farawayCH():
                n.isCH == True
                self.childCH.append(n)
                # self.child.remove(n)

    def nodesAchievable(self,nodes):
        lista = []
        for n in nodes:
            d = np.sqrt((self.posX - n.posX)**2 + (self.posY - n.posY)**2)
            if d <= self.radius and d != 0:
                lista.append(n)
        return lista


    def getNodeID(self):
        return self.nodeID

    def getModel(self):
        return self.model

    def getAssoc(self):
        return self.assoc

# leitura da disposicao dos nodes no cenario Intel Lab Data
arq = open("C:\\Users\\Miguel\\Documents\\Visual Studio 2019\\C++\\gl_projects\\topology_IntelLabData.txt")
lines = arq.readlines()

lab_ids = []
lab_xpos = []
lab_ypos = []
count_lines = 0
for l in lines:
    count_lines+=1
    a = l.split()
    lab_ids.append(int(a[0]))
    lab_xpos.append(float(a[1]))
    lab_ypos.append(float(a[2]))
# print(list_lin)

# ler dados coletados pelos sensores <data, hora, epoca, moteID, temp, humid, light, voltage>
collect = open('C:\\Users\\Miguel\\Documents\\Visual Studio 2019\\C++\\gl_projects\\data.txt')
alldata = collect.readlines()

# Escolha de geracao de cenario aleatorio ou Intel Lab Data
op = int(input('1. Intel Lab Data or 2. Random Nodes. Choose: '))

nodes = []
if op == 2:
    qt = int(input('Quantos nodos serao inseridos? '))
    print(f'{qt} nodos a serem inseridos.')

    for i in range(qt):
        ns = Node()
        nodes.append(ns)
elif op == 1:
    for i in range(count_lines):
        ns = Node()
        ns.nodeID = lab_ids[i]
        ns.forcePosition(lab_xpos[i],lab_ypos[i])
        nodes.append(ns)
else:
    exit(0)

# funcao de impressao dos nodos
def print_nodes():
    for i in range(len(nodes)):
        print(f'\nNODE {i+1} :: ID = {nodes[i].nodeID} | {nodes[i].model} | assoc = {nodes[i].assoc} | position = ({nodes[i].posX},{nodes[i].posY})\n')

# Node 3 fixado com Coordenador PAN
nodes[2].forcePosition(envirX/2,envirY/2)
xPANC = nodes[2].posX
yPANC = nodes[2].posY

# print_nodes()
print(nodes[2].checkAdjNodes())

# Cria estruturas para guardar os dados de cada nodo
posxList = [] # dados de posicao X
posyList = [] # dados de posicao Y
nodeIDList = [] # lista de IDs
ratesList = []  # lista de datarates

for n in nodes:
    # pos_X, pos_Y = n.posX, n.posY
    nodeIDList.append(n.nodeID)
    ratesList.append(n.datarate)
    posxList.append(n.posX)
    posyList.append(n.posY)

def printAchieves(node,lista):
    print('nodes dentro do RF:')
    nachieve = node.nodesAchievable(lista)
    nassoc = node.child
    for n in nachieve:
    	print(n.nodeID)
    print('associados:')
    for no in nassoc:
        print(no.nodeID)

def formCluster(node,lista,datarate):
    aux = node.findNodes(lista,node,datarate)
    print(f'nodos associados por {node.nodeID}')
    for n in aux:
        print(n.nodeID)
    print(f'total = {len(node.child)}')
    further = node.farawayCH()
    print(f'child mais longe: {further.nodeID}')	
    printAchieves(node,lista)
    # return further

def formClusterRec(node):
	point = [node.posX, node.posY]
	lista0 = []
	lista1 = []
	points = []
	for i in range(len(node.child)):
		points.append([node.child[i].posX, node.child[i].posY])
	for i in range(len(node.child)):
		lista0.append([point[0],points[i][0]])
		lista1.append([point[1],points[i][1]])
	for i in range(len(node.child)):
		plt.plot(lista0[i], lista1[i],c='k',alpha=0.2)

# printAchieves(nodes[2], nodes)
# a = formCluster(nodes[2], nodes, 'r')
# formClusterRec(a)
# (nodes[2],nodes,'r')

# Formacao do primeiro cluster a partir do Coordenador PAN
nodes[2].findNodes(nodes,nodes[2],'r')
print(f'nodos associados por {nodes[2].nodeID}')
for n in nodes:
    if n.assoc == True:
        print(n.nodeID)
print(f'nodos child de {nodes[2].nodeID}')
for n in nodes[2].child:
    print(n.nodeID)
print(f'nodos childCH de {nodes[2].nodeID}')
for n in nodes[2].childCH:
    print(n.nodeID)
print(f'total = {len(nodes[2].child)}')
idFar = nodes[2].farawayCH()
nodes[2].selectChildCH()
print(f'child mais longe = {idFar.nodeID}')
# print('nodes dentro do RF:')
# nachieve = nodes[2].nodesAchievable(nodes)
# for n in nachieve:
#     print(n.nodeID)

# Formacao do segundo cluster a partir do Coordenador PAN
idFar.findNodes(nodes,idFar,'r')
print(f'nodos associados por {idFar.nodeID}')
for n in idFar.child:
    if n.assoc == True:
        print(n.nodeID)
print(f'total = {len(idFar.child)}')
print(f'nodos child de {idFar.nodeID}')
for n in idFar.child:
    print(n.nodeID)
print(f'nodos childCH de {idFar.nodeID}')
for n in idFar.childCH:
    print(n.nodeID)
idFar2 = idFar.farawayCH()
print(f'child mais longe: {idFar2.nodeID}')
idFar.selectChildCH()

# Formacao do terceiro cluster a partir do Coordenador PAN
idFar2.findNodes(nodes,idFar2,'r')
print(f'nodos associados por {idFar2.nodeID}')
for n in idFar2.child:
    if n.assoc == True:
        print(n.nodeID)
print(f'total = {len(idFar2.child)}')
print(f'nodos child de {idFar2.nodeID}')
for n in idFar2.child:
    print(n.nodeID)
print(f'nodos childCH de {idFar2.nodeID}')
for n in idFar2.childCH:
    print(n.nodeID)
idFar3 = idFar2.farawayCH()
print(f'child mais longe: {idFar3.nodeID}')
idFar2.selectChildCH()

# Formacao do quarto cluster a partir do Coordenador PAN
idFar3.findNodes(nodes,idFar2,'r')
print(f'nodos associados por {idFar3.nodeID}')
for n in idFar3.child:
    if n.assoc == True:
        print(n.nodeID)
print(f'total = {len(idFar3.child)}')
idFar4 = idFar3.farawayCH()
if idFar4 != None:
    print(f'child mais longe: {idFar4.nodeID}')
idFar3.selectChildCH()

# AQUI DEVE SER REALIZADA A ESCOLHA DOS CHS FILHOS E RECURSIVA CRIACAO DE CLUSTER

# Plot cenario
# configuracao das listas de nodos para amostragem no gráfico
posxList = np.array(posxList)
posyList = np.array(posyList)
ratesList = np.array(ratesList)
# col = np.where(posxList<50,'b',np.where(posyList<50,'r','k'))
col = np.where(ratesList==0.05,'b',np.where(ratesList==0.2,'r','k'))
ax = plt.gca()
ra = plt.Circle((xPANC,yPANC), nodes[2].radius, color='yellowgreen', alpha=0.5,fill=False)
ax.add_patch(ra)
plt.scatter(posxList,posyList,c=col)
# zip joins x and y coordinates in pairs
for x,y,l in zip(posxList,posyList,nodeIDList):

    label = l

    plt.annotate(label, # this is the text
                 (x,y), # this is the point to label
                 textcoords="offset points", # how to position the text
                 xytext=(0,4), # distance from text to points (x,y)
                 ha='center') # horizontal alignment can be left, right or center
# theta = np.arange(0, y, 0.01)
# plt.plot(x * np.cos(theta), y * np.sin(theta),c='k')

plt.scatter(xPANC,yPANC,c='k',s=45)
plt.plot(x, y,c='k')

# Plot connections
point1 = [nodes[2].posX, nodes[2].posY]
point2 = [nodes[2].child[0].posX, nodes[2].child[0].posY]
pointsy = []
for j in range(len(nodes[2].child)):
    pointsy.append([nodes[2].child[j].posX,nodes[2].child[j].posY])

x_values = [point1[0], point2[0]]
y_values = [point1[1], point2[1]]
plt.plot(x_values, y_values,c='k')
print(pointsy)

# Criacao das listas com a posicao de cada membro para Plotagem do cluster associado
list_XY0_values = []
list_XY1_values = []
for j in range(len(nodes[2].child)):
    list_XY0_values.append([point1[0],pointsy[j][0]])
    list_XY1_values.append([point1[1],pointsy[j][1]])
# for i in range(len(nodes[2].child)):
#     print(f'list0[{i}] = {list_XY0_values}')
#     print(f'list1[{i}] = {list_XY1_values}')
#     print('- - - - - - - - - ')
for j in range(len(nodes[2].child)):
    plt.plot(list_XY0_values[j], list_XY1_values[j],c='k')

# Criacao das listas com a posicao de cada membro para Plotagem do cluster associado
point3 = [idFar.posX, idFar.posY]
list_XY0_ = []
list_XY1_ = []
pointsy2 = []
for j in range(len(idFar.child)):
    pointsy2.append([idFar.child[j].posX,idFar.child[j].posY])

for j in range(len(idFar.child)):
    list_XY0_.append([point3[0],pointsy2[j][0]])
    list_XY1_.append([point3[1],pointsy2[j][1]])
for j in range(len(idFar.child)):
    plt.plot(list_XY0_[j], list_XY1_[j],c='k',alpha=0.2)

# Criacao das listas com a posicao de cada membro para Plotagem do cluster associado
point4 = [idFar2.posX, idFar2.posY]
list_XY0__ = []
list_XY1__ = []
pointsy3 = []
for j in range(len(idFar2.child)):
    pointsy3.append([idFar2.child[j].posX,idFar2.child[j].posY])

for j in range(len(idFar2.child)):
    list_XY0__.append([point4[0],pointsy3[j][0]])
    list_XY1__.append([point4[1],pointsy3[j][1]])
for j in range(len(idFar2.child)):
    plt.plot(list_XY0__[j], list_XY1__[j],c='k',alpha=0.2)

# Criacao das listas com a posicao de cada membro para Plotagem do cluster associado
point5 = [idFar3.posX, idFar3.posY]
list_XY0___ = []
list_XY1___ = []
pointsy4 = []
for j in range(len(idFar3.child)):
    pointsy4.append([idFar3.child[j].posX,idFar3.child[j].posY])

for j in range(len(idFar3.child)):
    list_XY0___.append([point5[0],pointsy4[j][0]])
    list_XY1___.append([point5[1],pointsy4[j][1]])
for j in range(len(idFar3.child)):
    plt.plot(list_XY0___[j], list_XY1___[j],c='k',alpha=0.2)

# for j in range()
# plt.plot(x_values, y_points[1])
# plt.hist(posxList,orientation=u'vertical')
plt.grid()
# fig, ax = plt.subplots()
# ax.add_patch(radiorange)
plt.show()