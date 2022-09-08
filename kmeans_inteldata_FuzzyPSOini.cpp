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
/* Estrutura dos vizinhos nodos sensores */
struct Neighb{
    int nodeID, depth, blue, red, somaViz=0, somaVizHom=0;
    double  datarate;  
    string viz, cluster;
    int vizinhos[MAX_VIZ], nViz, nHet=0;
    int i=0;

    Neighb(int id, int d, double drate, string cl){  // Construtor comum
        cl = setCluster();
        nodeID = id;
        depth = d; 
        datarate = drate;
        cluster = cl;
    }    

    Neighb(int nodeID, int depth, double datarate, string viz, int blue, int red) :     // Construtor no modo de lista de inicializacao
    nodeID(nodeID),
    depth(depth), 
    datarate(datarate),
    viz(viz),
    blue(blue),
    red(red),
    cluster(setCluster()) {}

    string setCluster(){      // Funcao que atribui cluster conforme o datarate
        if (datarate == 0.05)
            return "azul";
        else if(datarate == 0.2)
            return "vermelho";
    }

    int *getArrayVizinhos(string viz){          // Funcao que percorre a lista de vizinhos e devolve array
        viz = viz.substr(1, viz.size() - 2);    // remove primeiro e ultimo caractere, podia usar viz.erase(0, 1); e viz.erase(viz.size() - 1);
        char *token = strtok((char *)viz.c_str(), ", ");    // recupera os valores da string separados por delimitador (convertendo string para char *)
        while (token != NULL){
            vizinhos[i] = stoi(token);          // cout << token << " ";
            i++;
            token = strtok(NULL, ", ");
        }
        nViz = i;                               // cout << "size " << i << endl;
        return vizinhos;
    }

    int somaVizinhos(int id, int nViz, int vizinhos[], string cluster){
        int v=0;
        for (int i=0; i < nViz; i++ )
            if ( id == vizinhos[i] )
                v++;
        nHet = v;
        return v;
    }

    bool buscaVizinho(int id, int nViz, int vizinhos[]){
        for (int i=0; i < nViz; i++){
            if (id == vizinhos[i])
                return true;
            else    
                return false;
        }
    }

    void setHet(string cl1, string cl2){
        if (cl1 != cl2)
            nHet++;
    }

    void somaDepthViz(int depth_viz){
        somaViz += depth_viz;
    }

    void somaDepthVizHom(int depth_viz){
        somaVizHom += depth_viz;
    }
};

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

////////////////////////////////
/* Funcao para leitura dos dados do arquivo CSV */
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
/* Funcao para leitura dos dados do arquivo CSV */
vector<Neighb> read__csv(){
    vector<Neighb> neighb;
    string line;
    ifstream file("lista_nodos__.csv");
    
    map<int, string> lista;    

    while (getline(file, line)) {
        stringstream lineStream(line);
        string ls, viz;
        int nodeID, depth, blue, red;
        double datarate;

        getline(lineStream, ls, ';');
        getline(lineStream, ls, ';');
        nodeID = stoi(ls); // id = ls; 
        getline(lineStream, ls, ';');
        depth = stoi(ls);  // depth = ls; 
        getline(lineStream, ls, ';');
        datarate = stod(ls); // drate = ls;
        getline(lineStream, ls, ';');
        viz = ls;
        getline(lineStream, ls, ';');
        blue = stoi(ls); // blue = ls;
        getline(lineStream, ls, '\n');
        red = stoi(ls);
        // getline(lineStream, ls, '\n');
        //red = stoi(ls);
        // lista[nodeID] = _neigh;
        // cout << nodeID << "\td: " << depth << "\tdr: " << datarate << "\tnb: " << viz << "\tb: " << blue << endl;
        neighb.push_back(Neighb(nodeID, depth, datarate, viz, blue, red));
    }

    // for (auto i : lista)
    //     cout << i.first << " vizinhos = " << i.second << endl;

    return neighb;
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

vector<Neighb> __neighb = read__csv(); 

cout << endl << "DADOS DE TODOS" << endl;
for(vector<Neighb>::iterator it = __neighb.begin();it!=__neighb.end();it++){
    cout <<  it->nodeID << " " << it->depth << " " << it->datarate << " " << it->viz << " " << it->blue << " " << it->red << " " << it->cluster << endl;
}
cout << endl << "VIZINHOS DE TODOS" << endl;

// Percorre a lista de vizinhos
for(vector<Neighb>::iterator it = __neighb.begin(); it!=__neighb.end(); it++){
    cout << it->nodeID << " vizinhos = ";
    it->getArrayVizinhos(it->viz);
    for (int j=0; j < it->nViz; j++)
        cout << it->vizinhos[j] << " ";
    cout << endl;
}
cout << endl << "ID |\tDEPTH |\tBLUE |\tRED  \t::  somente de vermelhos" << endl;

int posi=0, somaD=0, somaH=0, somaR=0, medD=0, medH=0, a=0, medR=0, cnt=0;
int relacaoViz=0;
map<int,int> mapa1;
map<int, pair<int, int>> mapa2;
string particula1, particula2;

// Procura vizinhos somente de vermelhos
for(vector<Neighb>::iterator it = __neighb.begin(); it!=__neighb.end(); it++){  // percorre a lista de nodos
    if (it->cluster == "vermelho"){                 // seleciona somente vermelhos
        cnt++;
        particula2 += '0';
        for ( a = 0; a < it->nViz; a++){
            for(vector<Neighb>::iterator it_ = __neighb.begin(); it_ != __neighb.end(); it_++){
                if (it->vizinhos[a] == it_->nodeID){
                    it->setHet(it->cluster, it_->cluster);
                    if (it->cluster != it_->cluster){
                        it->somaDepthViz(it_->depth);
                        somaD += it_->depth;
                        somaH += it->nHet;
                    }else{
                        it->somaDepthVizHom(it_->depth);
                    }
                }
            }
        }
        mapa1[it->nodeID] = it->somaViz;
        mapa2[it->nodeID] = make_pair(it->depth,posi);
        posi++;
        relacaoViz = abs(it->somaViz - it->somaVizHom);
        medD = somaD / cnt;
        medH = somaH / cnt;
        somaR += relacaoViz;
        medR = somaR / cnt;
        cout <<  it->nodeID << " \t" << it->depth << " " << "\t" << it->blue << " \t" << it->red << // "\tdifer = " << it->nHet << 
            " \tsomaD_HET = " << it->somaViz << " \tsomaD_HOM = " << it->somaVizHom << " \tHET/D: " << (double) (it->nHet / it->depth) << // " \tpropDif: " << propDif << " \tpropDepth: " << propDep << 
            " \trelacaoViz: " <<  relacaoViz << endl;
    }
}
// Particula 1 define como CH os nodos com somatorio de profundidade maior que a media
for (map<int,int>::iterator mp = mapa1.begin(); mp != mapa1.end(); mp++){
    if (mp->second > medD){
        particula1 += '1';         // cout << "entrou aqui no " << mp->first << endl;
    }
    else{
        particula1 += '0';
    }
}
int d = 1;
// Particula 2 define como CH os nodos representante de cada profundidade
for(int w = 0; w < particula2.length(); w++){
    for(map<int, pair<int, int>>::iterator mp = mapa2.begin(); mp != mapa2.end(); mp++){
        // cout << mp->first << " " << mp->second.first << " " << mp->second.second << " " << d << endl;
        if ( d == mp->second.first ){
            // cout << "d = " << d << " it->d = " << mp->second << " w = " << w << endl;
            particula2[mp->second.second] = '1';
            break;
        }
    }
    d++;
}

cout << "a = " << a << " cnt = " << cnt << " media de vizinhos heterogeneos = " << medH << " media de profundiades = " << medD << " relacao = " << medR << endl;
cout << "particula1 = " << particula1 << endl;
cout << "particula2 = " << particula2 << endl;
cout << endl;

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