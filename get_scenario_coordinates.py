from lib2to3.pgen2.token import RPAR
import matplotlib.image as mpimg
import matplotlib.pyplot as plt
import csv

# inicializa o Arquivo em branco
with open('IEEE123-coordinates.csv', 'w') as file:
    file.write("")

# recupera as coordenados quando o click do botao for acionado
def mouse_event(event):
    datax = round(event.xdata, 2)
    datay = round(event.ydata, 2)
    print(datax,' \t', datay)
    # abre o arquivo para escrever no modo append linha a linha
    with open('IEEE123-coordinates.csv', 'a') as file:
        file.write(f'{datax},{datay}\n')

# ler a imagem do cenario passado
img = mpimg.imread('C://Users//Miguel//Documents//ufsc 2022.2//Disciplina IoT//IEEE-123-node-test-feeder.png') 

# usa o figure para recuperar os dados da figura gerada pelo matplot
fig = plt.figure()
cid = fig.canvas.mpl_connect('button_press_event', mouse_event)

# mostra a imagem na escala correta do cenario
plt.imshow(img,extent=[0,300,0,215])
print(img.shape) # mostra o formato da imagem
plt.grid()
plt.show()
