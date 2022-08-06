#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iomanip>
#include <ctime>     // para seed aleatorio 
#include <fstream>   // para ler arquivos
#include <sstream>   // para manipular arquivos
#include <vector>    // para guardar os pontos
#include <typeinfo>

#define numNODES 54

using namespace std;

/* Estrutura dos pontos de dados lidos dos sensores */
/////////////////////////////
struct Point{
    int nodeID;
    double temp, umid;     // valores de temp e umid 
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

/* Classe que define os nodos sensores */
    //////////////////////////////
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

/* Funcao para leitura dos dados do arquivo CSV */
////////////////////////////////
vector<Point> readcsv(){
    vector<Point> points;
    string line;
    ifstream file("data10h_avg.csv");
    int count=0;

    while (getline(file, line)) {
        stringstream lineStream(line);
        string bit;

        count++;
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

/* Funcao de execucao do Kmeans */
////////////////////////////////
void kMeansClustering(vector<Point>* points, int epochs, int k){
    vector<Point> centroids;
    *points = readcsv();
    srand(time(0));          // funcao com seed para funcao rand nao gerar os mesmos
                             // o time faz com que seja gerado sempre no tempo corrente
    cout << "\ncentroids:" << endl;
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
        int clusterId = c - begin(centroids); // identifica a quantidade de cluster 
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

        // vector<int> nPoints;
        // vector<double> sumX, sumY;

        // // Inicializa o vetor de somas
        // for (int j = 0; j < k; ++j) {
        //     nPoints.push_back(0);
        //     sumX.push_back(0.0);
        //     sumY.push_back(0.0);
        // }

        // novos centroids a cada iteracao

        // // Iterate over points to append data to centroids
        // for (vector<Point>::iterator it = points->begin(); it != points->end(); ++it){
        //     int clusterId = it->cluster; // recupera o cluster de cada nodo
        //     nPoints[clusterId] += 1;
        //     sumX[clusterId] += it->x;
        //     sumY[clusterId] += it->y;

        //     it->minDist = __DBL_MAX__;  // reset distance
        // }

        // // Compute the new centroids
        // for (vector<Point>::iterator c = begin(centroids); 
        //     c != end(centroids); ++c) {
        //     int clusterId = c - begin(centroids);
        //     c->x = sumX[clusterId] / nPoints[clusterId];
        //     c->y = sumY[clusterId] / nPoints[clusterId];
        // }
    }
}

////////////////////

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

    kMeansClustering(&points,80,2); // pass address of points to function
    
    int red=0,blue=0; // contando os nodos em cada cluster
    
    for(vector<Point>::iterator it = points.begin(); it != points.end(); it++){
        if(it->cluster == 0){
            red++;
        }else{
            blue++;
        }
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