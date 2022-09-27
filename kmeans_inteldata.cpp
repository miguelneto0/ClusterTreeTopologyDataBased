#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <math.h>
#include <iomanip>   // para usar setprecision
#include <time.h>    // para usar clock_t
#include <ctime>     // para usar no seed aleatorio 
#include <fstream>   // para ler arquivos
#include <sstream>   // para manipular arquivos
#include <vector>    // para guardar os pontos
#include <map>       // para guardar os vizinhos de cada nodo
#include <typeinfo>
#include <utility>

#define numNODES 54
#define MAX_VIZ 20

clock_t ini, fim;

using namespace std;

/////////////////////////////
/* Estrutura dos pontos de dados lidos dos sensores */
struct Point{
    int nodeID;
    double temp, umid, datarate;     // valores de temp e umid 
    int cluster;           // cluster padrao
    double minDist;        // distancia minima (padrao infinita)

    Point() :                         // Construtor no modo de lista de inicializacao
        nodeID(-1),
        temp(0.0), 
        umid(0.0),
        cluster(-1),
        minDist(__DBL_MAX__) {}
        
    Point(int nodeID, double temp, double umid) : // Construtor no modo de lista de inicializacao
        nodeID(nodeID),
        temp(temp),                   // um exemplo poderia ser temp 30
        umid(umid),
        cluster(-1),
        minDist(__DBL_MAX__) {}

    double distance(Point p) {        // Funcao que calcula distancia euclidiana
        return (p.temp - temp) * (p.temp - temp) + (p.umid - umid) * (p.umid - umid);
    }
};

//////////////////////////////
/* Classe que define os nodos sensores */
    /*class Node{
        private:
            int nodeID;
            float datarate;
        public:
            void setNodeID(int id){ this->nodeID = id;}
            void setDatarate(float dr){ this->datarate = dr;}
            int getNodeID(){ return nodeID;}
            float getDatarate(){ return datarate;}
            Node(){ setDatarate(0);setNodeID(0);}
            Node(int id, float dr);
    };
    Node::Node(int id, float dr){
        setNodeID(id);
        setDatarate(dr);
    }*/

////////////////////////////////
/* Funcao para leitura dos dados do arquivo CSV padrao */
vector<Point> readcsv(){
    vector<Point> points;
    string line;
    ifstream file("data10h_avg.csv");

    while (getline(file, line)) {
        stringstream lineStream(line);
        string bit;

        int nodeID;
        double x, y;
        getline(lineStream, bit, ';');
        nodeID = stoi(bit);
        getline(lineStream, bit, ';');
        x = stof(bit);
        getline(lineStream, bit, '\n');
        y = stof(bit);

        points.push_back(Point(nodeID, x, y));
    }
    return points;
}

////////////////////////////////
/* Funcao que executa o Kmeans */
void kMeansClustering(vector<Point>* points, int epochs, int k, bool fuzzy){
    vector<Point> centroids;
    *points = readcsv();
    srand(time(0));          // funcao com seed para funcao rand nao gerar os mesmos
                             // o time faz com que seja gerado sempre no tempo corrente
    
    for(int i = 0; i < k; ++i) {
        centroids.push_back(points->at(rand() % points->size()));
    }
    cout << "total de centroids: " << centroids.size() << endl;
    cout << centroids.front().temp << " " << centroids.front().umid << endl;
    cout << centroids.back().temp << " " << centroids.back().umid << endl;
    cout << "total de pontos: " << points->size() << endl;
    
    vector<Point>::iterator c;  // iterator para percorrer os centroids
    vector<Point>::iterator it; // iterator para percorrer os points

    for(c = begin(centroids);c != end(centroids); ++c){
    int clusterId = c - begin(centroids);   // identifica a quantidade de cluster 
                                            // pela comparacao dos valores com o inicial
    
        for (it = points->begin(); it != points->end(); ++it){        
            Point p = *it;  // instancia que representa cada ponto do conjunto    
            double dist = c->distance(p); // recupera distancia do ponto
            if (dist < p.minDist) {       // verifica se é menor do que a dos centroids
                p.minDist = dist;         // se for, atualiza distancia e atribui cluster
                p.cluster = clusterId;
            }
            *it = p;                      // atualiza a instacia com valores atribuidos
        }
    }

    vector<int> nPoints;
    vector<double> sumX, sumY;

    int e = 0;

    while (e < epochs){

        // Inicializa o vetor de somas
        for (int j = 0; j < k; ++j) {
            nPoints.push_back(0);
            sumX.push_back(0.0);
            sumY.push_back(0.0);
        }

        // novos centroids a cada iteracao

        // Itera em cada estrutura para centroids
        for (it = points->begin(); it != points->end(); ++it){
            int clusterId = it->cluster;    // recupera o cluster de cada nodo
            nPoints[clusterId] += 1;        // atualiza a quantidade de nodos percorridos em cada cluster
            sumX[clusterId] += it->temp;    // atualiza o somatorio de cada valor da variavel x eg. temp
            sumY[clusterId] += it->umid;    // atualiza o somatorio de cada valor da variavel y eg. umid

            it->minDist = __DBL_MAX__;  // reseta distancia
        }

        // cout << "sumX b = " << sumX.back() <<  " sumX f = " << sumX.front() << endl;
        // cout << "sumY b = " << sumY.back() <<  " sumY f = " << sumY.front() << endl;
        // cout << "nPoints b = " << nPoints.back() <<  " nPoints f = " << nPoints.front() << endl;
        
        // Calcula novos centroides
        for (c = begin(centroids); c != end(centroids); ++c) {
            int clusterId = c - begin(centroids);
            c->temp = sumX[clusterId] / nPoints[clusterId];
            c->umid = sumY[clusterId] / nPoints[clusterId];
        }
        e++;
    }
    // Inicio do Fuzzy-C-Means
    Point c0, c1;
    if (fuzzy){
        cout << "inicia processo de fuzzyficacao" << endl; // verifica grau de pertinecia dos dados rotulados com calcula da base menor e media
        for (int a=0; a < k; a++) {
            cout << "media temp [" << a << "] = " << (sumX[a] / nPoints[a] ) << endl; // imprime as medias temperatura de cada cluster
            cout << "media umid [" << a << "] = " << (sumY[a] / nPoints[a] ) << endl; // imprime as medias umidade de cada cluster
            // falta calcular o grau de pertinencia de cada ponto verificando a distancia para cada centroide
            if (a==0)
                c0 = Point(a,(sumX[a] / nPoints[a]), (sumY[a] / nPoints[a])); c0.cluster = a;
            if (a==1)
                c1 = Point(a,(sumX[a] / nPoints[a]), (sumY[a] / nPoints[a])); c1.cluster = a;
        }
        // MEDIR GRAU DE PERTINENCIA COM BASE NA ESCALA DE DISTANCIA EXISTENTE ENTRE OS CONJUNTOS, APROXIMANDO AO MAXIMO DE 1 QUANDO HA INCERTEZA
        for (it = points->begin(); it != points->end(); ++it)   // fase de inferencia
            if (abs(it->distance(c0) / it->distance(c1)) >= 0.5 && abs(it->distance(c0) / it->distance(c1)) <= 2.5){
                cout << it->cluster << " -> id" << it->nodeID << " temp= " << it->temp << " umid= " << it->umid << "\tdist = " << (it->distance(c0) / it->distance(c1)) << endl;
                if ((it->distance(c0) / it->distance(c1)) < 1) // fase de defuzzificacao
                    it->cluster = !(it->cluster); 
            }
    }
   
}

////////////////////////////////
/* Funcao que executa o DBSCAN */
void DBSCANClustering(vector<Point>* points, double epsilon){
    *points = readcsv();
    srand(time(0));          // funcao com seed para funcao rand nao gerar os mesmos
                             // o time() faz com que seja gerado rand() sempre no tempo corrente
    
    int n_clusters = 0;

    Point refer = points->at(rand() % points->size()); // escolhe um dado aleatorio
    cout << "refer: " << refer.nodeID << endl;
    for(vector<Point>::iterator i = points->begin(); i != points->end() ; ++i){
        if (i->distance(refer) <= epsilon && i->cluster != n_clusters){
            i->cluster = n_clusters;
            // cout << "node = " << i->nodeID << endl;
        }else{
            i->cluster = n_clusters + 1;
        }
        refer = *i;
    }
}

////////////////////
/* Funcao MAIN */
int main(int argc, char const *argv[]){

/* implementacao teste */    
    // list<Node> my_nodes(54);
    // list<Node>::iterator it;
    // my_nodes.clear();
    // Node n1;
    // for(int i=0;i<numNODES;i++){
    //     if( i>39 && i<44 ){
    //         Node novo(i, 0.2f); 
    //         my_nodes.push_back(novo);
    //     }else{
    //         Node novo(i, 0.05f); 
    //         my_nodes.push_back(novo);
    //     }   
    // }
    // for(it=my_nodes.begin();it!=my_nodes.end();it++){
    //     cout << it->getNodeID() << " " << it->getDatarate() << endl;
    // }
    // my_nodes.clear();

    
/* implementacao */
    vector<Point> points = readcsv(); 
    for(vector<Point>::iterator it = points.begin();it!=points.end();it++){
        cout <<  it->nodeID << " " << it->temp << " " << it->umid << endl;
    }

    cout << "\nAlgoritmo de Clustering a ser aplicado ao Intel Lab Data:\n";
    cout << "1. K-means\t2. DBSCAN\t3. Fuzzy-C" << endl;
    
    int op=0, k=0, epoch=0;
    double e=0;

    cin >> op;
    switch (op){
    case 1:
        cout << "Valor de k: ";
        cin >> k;
        cout << "Epocas: ";
        cin >> epoch;
        ini = clock();
        kMeansClustering(&points,epoch,k,false); // chamada funcao Kmeans passando vetor de pontos, numero de iteracoes e clusters
        fim = clock();
        cout << "tempo decorrido = " << fim - ini << " ms" << endl;
        break;
    case 2:
        cout << "Valor de epsilon: ";
        cin >> e;
        ini = clock();
        DBSCANClustering(&points, e);
        fim = clock();
        cout << "tempo decorrido = " << fim - ini << " ms" << endl;
        break;
    case 3:
        cout << "Valor de k: ";
        cin >> k;
        cout << "Epocas: ";
        cin >> epoch;
        ini = clock();
        kMeansClustering(&points, epoch,k, true);
        fim = clock();
        cout << "tempo decorrido = " << fim - ini << endl;
        break;
    default:
        break;
    }
        
    int red=0,blue=0; // contando os nodos em cada cluster
    
    for(vector<Point>::iterator it = points.begin(); it != points.end(); it++){
        if(it->cluster == 0)
            red++;
        else
            blue++;
    }
    cout << "total red: " << red << " blue: " << blue << endl; 

    // Criando novo arquivo com o resultado do Kmeans
    ofstream myfile;
    myfile.open("data10h_saida.csv");
    myfile << "id,temp,umid,cluster" << endl;

    for (vector<Point>::iterator it = begin(points); it != end(points); ++it) {
        myfile << it->nodeID << "," << it->temp << "," << it->umid << "," << it->cluster << endl;
    }
    myfile.close();
    
    return 0;
}