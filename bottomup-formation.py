import numpy as np
import pandas as pd
import random
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
        self.hopToPAN = 0
        self.energy = 0
        self.isCH = False
        self.child = []
        self.childCH = []
        self.hasParent = False
        self.parent = None
        self.neighbors = []
        self.achieveBlue = 0
        self.blues = []
        self.nearRed = []
        self.nearBlue = []

    def countBlue(self): # conta azuis proximos
        self.achieveBlue = len(self.nearBlue)
        return self.achieveBlue

    def as_dict(self): # retorna tabela para tomada de decisao
        return {'ID': self.nodeID, 'Depth': self.hopToPAN, 'nearBlue': self.countBlue(), 'Energy': self.energy}

    def checkAdjNodes(self):
        rangeRF = np.pi*(self.radius)**2
        return rangeRF

    def forcePosition(self, x, y): # forca a posicao do nodo
        self.posX = x
        self.posY = y
    
    def forceDataRate(self,drate): # forca o datarate
        self.datarate = drate

    def associateNodes(self, nodes_list, datarate): # busca nodos com base na taxa de aquisicao de dados
        d=0
        if datarate == 'r':
            d=0.2
        elif datarate == 'b':
            d=0.05

        for n in self.nodesAchievable(nodes_list):
            if n.datarate == d and n.assoc == False and n.nodeID != self.nodeID and n.isCH == False:
                n.assoc = True
                n.parent = self
                n.hasParent = True
                self.isCH = True
                self.child.append(n)
        return self.child            
      
    def farawayCH(self): # retorna no mais longe para ser CH
        lista_dist = []
        for i in range(len(self.child)):
            res = (self.posX - self.child[i].posX)**2 + (self.posY - self.child[i].posY)**2
            dist = np.sqrt(res)
            lista_dist.append(dist)
        if len(lista_dist) > 0:
            m = max(lista_dist)
            p = lista_dist.index(m)
            return self.child[p]
    
    def selectChildCH(self,n): # seleciona CH filho
        for n in self.child:
            if n.isCH == False and n == self.farawayCH():
                n.isCH == True
                self.childCH.append(n)
                self.child.remove(n)

    def nodesAchievable(self,nodes): # procura os nodos dentro do alcance de RF
        lista = []
        for n in nodes:
            d = np.sqrt((self.posX - n.posX)**2 + (self.posY - n.posY)**2)
            if d <= self.radius and d != 0:
                lista.append(n)
        return lista
        # deveria ser um atributo de cada nodo

    def blueAchievable(self,nodes): # procura os nodos azuis para ponte 
        for n in nodes:
            d = np.sqrt((self.posX - n.posX)**2 + (self.posY - n.posY)**2)
            if d <= self.radius and d != 0 and n.datarate == 0.05:
                self.blues.append(n)
        return self.blues

    def nearReds(self,nodes): # procura os nodos azuis para ponte 
        for n in nodes:
            d = np.sqrt((self.posX - n.posX)**2 + (self.posY - n.posY)**2)
            if d <= envirX and d != 0 and n.datarate == 0.2:
                self.nearRed.append([n,d])
        return self.nearRed

    def nearBlues(self,nodes): # procura os nodos azuis para ponte 
        for n in nodes:
            d = np.sqrt((self.posX - n.posX)**2 + (self.posY - n.posY)**2)
            if d <= envirX and d != 0 and n.datarate == 0.05:
                self.nearBlue.append([n,d])
        return self.nearBlue
    
    def moreDistant(self,datarate):
        l = []
        if datarate == 'r':
          l = sorted(self.nearRed, key=lambda x: x[1])
          return l[0]
        elif datarate == 'b':
          l = sorted(self.nearBlue, key=lambda x: x[1])
          return l[0]

    def getNodeID(self):
        return self.nodeID

    def getModel(self):
        return self.model

    def getAssoc(self):
        return self.assoc

# leitura da disposicao dos nodes no cenario Intel Lab Data
arq = open("C:\\Users\\Miguel\\Documents\\Github\\ClusterTreeTopologyDataBased\\topology_IntelLabData.txt")
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

# Escolha de geracao de cenario aleatorio ou Intel Lab Data
op = int(input('1. Intel Lab Data | 2. Random Nodes |\tEscolha: '))

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

# definicao do Nodo 2 como Coordenador na posicao central
nodes[3].forcePosition(envirX/2,envirY/2)
xPANC = nodes[3].posX
yPANC = nodes[3].posY
# print_nodes()
print(nodes[3].checkAdjNodes())

# Cenario DbCTF - - nao executar esse em caso de aleatorio
for n in nodes:
  if n.nodeID == 26 or n.nodeID == 31  or n.nodeID == 33 or n.nodeID == 32 or n.nodeID == 30 or n.nodeID == 28 or n.nodeID == 29 or n.nodeID == 27 or n.nodeID == 23 or n.nodeID == 25 or n.nodeID == 24 or n.nodeID == 22 or n.nodeID == 21 or n.nodeID == 19: 
    n.forceDataRate(0.2)
  else:
    n.forceDataRate(0.05)
for n in nodes:
  if n.nodeID == 33:
    n.hopToPAN = 1
    n.energy = 200
  if n.nodeID ==32 or n.nodeID ==29:
    n.hopToPAN = 2
    n.energy = random.randrange(100,199)
  if n.nodeID == 28 or n.nodeID == 25 or n.nodeID == 23 or n.nodeID == 27:
    n.hopToPAN = 3
    n.energy = random.randrange(60,180)
  if n.nodeID ==24 or n.nodeID ==22 or n.nodeID ==21: 
    n.hopToPAN = 4
    n.energy = random.randrange(60,180)
  if n.nodeID ==19:
    n.hopToPAN = 5
    n.energy = random.randrange(50,100)
# Contar nao associados
nodesNoPan = []

for n in nodes:
  if n.nodeID != 4:
    nodesNoPan.append(n)
print(len(nodesNoPan))

def countNoAssoc(lista):
  count = 0
  for i in lista:
    if i.assoc == False:
      # return True
      count+=1
    # else:
      # return False
  return count

def findNoAssoc(lista):
  for i in lista:
    if i.assoc == False:
      return True
    else:
      return False

# Node 3 fixado no centro como Coordenador PAN
nodes[3].forcePosition(envirX/2,envirY/2)
xPANC = nodes[3].posX
yPANC = nodes[3].posY

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

posxList = np.array(posxList)
posyList = np.array(posyList)
ratesList = np.array(ratesList)

def printAchieves(node,lista):
    print('nodes dentro do RF:')
    nachieve = node.nodesAchievable(lista)
    nassoc = node.child
    for n in nachieve:
      if n != None:
        print(n.nodeID)
    print('associados:')
    for no in nassoc:
      if no != None:
        print(no.nodeID)

count = 0

# Plotagem do cenario
def plotCenario(lista,node,datarate):
  if datarate == 'b':
    ncol = 'b'
  elif datarate == 'r':
    ncol = 'r'
  else:
    ncol = 'k'
  col = np.where(ratesList==0.05,'b',np.where(ratesList==0.2,'r','k'))
  ax = plt.gca()
  ra = plt.Circle((xPANC,yPANC), node.radius, color='yellowgreen', alpha=0.5,fill=False)
  ax.add_patch(ra)
#   ax.invert_xaxis()
#   ax.invert_yaxis()
  plt.scatter(posxList,posyList,c=col)
  # zip joins x and y coordinates in pairs
  for x,y,l in zip(posxList,posyList,nodeIDList):

      label = l

      plt.annotate(label, # this is the text
                  (x,y), # this is the point to label
                  textcoords="offset points", # how to position the text
                  xytext=(0,4), # distance from text to points (x,y)
                  ha='center') # horizontal alignment can be left, right or center

  plt.scatter(xPANC,yPANC,c='k',s=45)
  plt.plot(x, y,c='k')

  # Plot connections
  point1 = [node.posX, node.posY]
  if len(node.child) != 0:
    point2 = [node.child[0].posX, node.child[0].posY]
  else:
    point2 = [node.posX, node.posY]
  pointsy = []
  for j in range(len(node.child)):
    pointsy.append([node.child[j].posX,node.child[j].posY])
  
  x_values = [point1[0], point2[0]]
  y_values = [point1[1], point2[1]]
  plotFormCluster(node,datarate)
  plt.plot(x_values, y_values,c=ncol)

# Plotagem dos novos clusters 
def plotFormCluster(node,datarate):
  if datarate == 'b':
    col='b'
  elif datarate == 'r':
    col='r'
  basept = [node.posX, node.posY]
  listX0 = []
  listX1 = []
  points = []
  for j in range(len(node.child)):
    points.append([node.child[j].posX, node.child[j].posY])
  for j in range(len(node.child)):
    listX0.append([basept[0],points[j][0]])
    listX1.append([basept[1],points[j][1]])
  for j in range(len(node.child)):
    plt.plot(listX0[j], listX1[j], c=col,alpha=0.2)

# Formacao do cluster
def formClusterBU(node,lista,datarate):
  count = 0
  volta = 0
  for n in lista:
    if n.nodeID == 4 and len(n.childCH) != 0:
          volta+=1
  if len(node.child) == 0 and findNoAssoc(nodesNoPan) == True and volta > 1:
        print(f'volta para procurar novo CH filho, node = {node.nodeID}')
        node = node.parent      
  if node != None:
    print(f'nodes dentro do RF de {node.nodeID}:')
    nachieve = node.nodesAchievable(lista)
    for n in nachieve:
      if len(nachieve)!=0 and n != None:
        print(n.nodeID)
      else:
        print('#### lista de alcancaveis vazia')
    node.associateNodes(lista,datarate)
    print(f'nodos associados por {node.nodeID}')
    for n in node.child:
      if n.assoc == True:
        print(n.nodeID)
    print(f'total = {len(node.child)}')
    x = countNoAssoc(nodesNoPan)
    print(f'- - - - - - x = {x}')
    plotCenario(lista,node,datarate)
    if len(node.child) != 0:
      plotFormCluster(node,datarate)
    else:
      ach = node.nodesAchievable(lista)
      aux = []
      for n in ach:
        if n.assoc==False:
          aux.append(n)
      while node.nodeID != 4 and len(node.child)==0:
        node = node.parent
        if node != None:
          print(f'voltou pro node pai {node.nodeID}')
          plotFormCluster(node,datarate)
        else:
            return
    idFar = node.farawayCH()
    if idFar != None or len(node.child) != 0:
      if count == 0:
          plotCenario(lista,node,datarate)
      count+=1
      node.selectChildCH(idFar)
      print(f'child mais longe = {idFar.nodeID}')
      print(f'pai de {idFar.nodeID} = {idFar.parent.nodeID} com drate = {idFar.datarate}')
      plotFormCluster(idFar,datarate)
      formClusterBU(idFar,lista,datarate)
  else:
    print('#### node invalido')
  return count,idFar,datarate

# Formacao do cluster
# def formCluster(node,lista,datarate):
#   count = 0
#   volta = 0
#   for n in lista:
#     if n.nodeID == 3 and len(n.childCH) != 0:
#           volta+=1
#   if len(node.child) == 0 and findNoAssoc(nodesNoPan) == True and volta > 1:
#         print(f'volta para procurar novo CH filho, node = {node.nodeID}')
#         node = node.parent      
#   if node != None:
#     print(f'nodes dentro do RF de {node.nodeID}:')
#     nachieve = node.nodesAchievable(lista)
#     for n in nachieve:
#       if len(nachieve)!=0 and n != None:
#         print(n.nodeID)
#       else:
#         print('#### lista de alcancaveis vazia')
#     node.associateNodes(lista,datarate)
#     print(f'nodos associados por {node.nodeID}')
#     for n in node.child:
#       if n.assoc == True:
#         print(n.nodeID)
#     print(f'total = {len(node.child)}')
#     x = countNoAssoc(nodesNoPan)
#     print(f'- - - - - - x = {x}')
#     plotCenario(lista,node,datarate)
#     if len(node.child) != 0:
#       plotFormCluster(node,datarate)
#     else:
#       ach = node.nodesAchievable(lista)
#       aux = []
#       for n in ach:
#         if n.assoc==False:
#           aux.append(n)
#       while node.nodeID != 4 and len(node.child)==0:
#         node = node.parent
#         if node != None:
#           print(f'voltou pro node pai {node.nodeID}')
#           plotFormCluster(node,datarate)
#         else:
#             return
#     idFar = node.farawayCH()
#     if idFar != None or len(node.child) != 0:
#       if count == 0:
#           plotCenario(lista,node,datarate)
#       count+=1
#       node.selectChildCH(idFar)
#       print(f'child mais longe = {idFar.nodeID}')
#       print(f'pai de {idFar.nodeID} = {idFar.parent.nodeID} com drate = {idFar.datarate}')
#       plotFormCluster(idFar,datarate)
#       formCluster(idFar,lista,datarate)
#   else:
#     print('#### node invalido')
#   return count,idFar,datarate

# plot Cenario
plotCenario(nodes,nodes[3],None)

redNodes = []
blueNodes = []
for n in nodes:
    if n.nodeID != 4 and n.assoc == False and n.datarate == 0.2:
        redNodes.append(n)
    elif n.nodeID != 4 and n.assoc == False and n.datarate == 0.05:
        blueNodes.append(n)

print(f'{len(redNodes)} nodos nao associados')
while len(redNodes) >8:
    nrand = np.random.choice(redNodes)
    print(f'nodo CH escolhido: {nrand.nodeID}')
    formClusterBU(nrand,nodes,'r')
    redNodes.remove(nrand)
while len(blueNodes) > 5:
    nrand = np.random.choice(blueNodes)
    print(f'nodo CH escolhido: {nrand.nodeID}')
    formClusterBU(nrand,nodes,'b')
    blueNodes.remove(nrand)
ax = plt.gca()
ax.invert_xaxis()
ax.invert_yaxis()
plt.grid()
plt.show()

# Formacao Bottom-up

dreds = pd.DataFrame([x.as_dict() for x in redNodes])

print(dreds)