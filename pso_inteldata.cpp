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
#include <algorithm>
#include <bits/stdc++.h>

#define numNODES 54
#define MAX_VIZ 20

const double EULER = 2.71828182845904523536;
double melhor_global = 47;
double melhor_local = 47;

using namespace std;

vector<string> solucoes;

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

////////////////////////////////
/* Funcao para leitura dos dados do arquivo CSV tabela completa */
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

////////////////////
/* Funcao que identifica diferentes hotspots */
vector<int> identificaHotspots(vector<Neighb> neig, map<int,pair<int, int>> mapa){ // 
    vector<pair<int,int*>> chs     = {};
    vector<pair<int,int>> marcados = {};
    vector<pair<int,int>> n_marcados = {};
    vector<int> hotspot1;
    vector<int> orfaos;
    int minD = 4;

    for(vector<Neighb>::iterator nb = neig.begin(); nb !=neig.end(); nb++)
        if (nb->cluster == "vermelho")
            chs.push_back(make_pair(nb->nodeID, nb->vizinhos));
    for (auto i : chs)
        n_marcados.push_back(make_pair(i.first,0));
    
    // percorre estruturas de vermelhos e vizinhos de cada um deles
    for(vector<pair<int,int*>>::iterator nodo = chs.begin(); nodo!=chs.end(); nodo++){
        for(vector<Neighb>::iterator nb = neig.begin(); nb != neig.end(); nb++){
            if (nodo->first == nb->nodeID && nb->red > 0){
                if(find(hotspot1.begin(), hotspot1.end(), nb->nodeID) == hotspot1.end()){
                    hotspot1.push_back(nb->nodeID);
                }    
                for(vector<pair<int, int>>::iterator n = n_marcados.begin(); n != n_marcados.end(); n++){
                    for(auto v : nb->vizinhos){
                        if(v == n->first){
                            if(find(hotspot1.begin(), hotspot1.end(), v) == hotspot1.end()){
                                hotspot1.push_back(n->first);
                                marcados.push_back(make_pair(n->first,1));
                            }
                        }
                    }
                }
            }else{
                orfaos.push_back(nb->nodeID);
            }
        }
    }
    int count=0;
    vector<int> hotspot_pos;

    cout << "tamanho marcado " << marcados.size() << endl;
    cout << "n_tamanho marcado " << n_marcados.size() << endl;

    for(int i=0;i<marcados.size();i++){
        for(int j=0;j<n_marcados.size();j++){
            if(marcados[i].first == n_marcados[j].first)
                n_marcados[j] = make_pair(n_marcados[j].first,1);
        }
    }

    for (auto n : n_marcados){
        if(n.second == 0){
            if (find(hotspot_pos.begin(),hotspot_pos.end(),n.first) == hotspot_pos.end())
                hotspot_pos.push_back(n.first); count++;
        }
    }
    vector<int> pos;
    for(auto hp : hotspot_pos){
        for(map<int,pair<int, int>>::iterator m = mapa.begin(); m != mapa.end(); m++){
            // cout << m->first << endl;
            if (m->first == hp)
                if (find(pos.begin(),pos.end(),m->second.second) == pos.end())
                    pos.push_back(m->second.second);
        }
    }

    cout << "Total de " << count << " hotspots" << endl;
    cout << "Delimitados pelas posicoes ";
    for (auto p : pos)
        cout << p << " ";
    cout << " da particula" << endl;
    
    // cout << "n_marcados" <<endl;
    // for (auto n : hotspot_pos)
    //     cout << n << endl;
    // cout << "marcados" <<endl;
    // for (auto m : marcados)
    //     cout << m.first << " " << m.second << endl;

    return hotspot_pos;
}

////////////////////
/* Funcao que retorna o somatorio de profundidade dos CHs selecionados*/
int somatorio(string particula, map<int,pair<int, int>> mapa){
    int soma = 0;
    for(map<int,pair<int, int>>::iterator mp2 = mapa.begin(); mp2!=mapa.end(); mp2++){
        for (int i=0;i<particula.length();i++){
            if (particula[i] == '1' && i == mp2->second.second){
                soma += mp2->second.first;
                if (i == 12 || i == 15)
                    soma += 3;
                if (i == 14)
                    soma += 6;
                if (i == 13)
                    soma += 9;
            }
        }              
    }
    // lembrar de definir que se i > 12 (i.e., os nodos 40, 41, 42 e 43) cada um deles resulta em +3, +6
return soma;
}

////////////////////
/* Funcao que executa o algoritmo PSO */
double sigmoide(double somaInd){
    return 1 / (1 + pow(EULER,(-somaInd)));
}

////////////////////
/* Funcao que executa o algoritmo PSO */
double executa_PSO(double atual){
    double pb = melhor_local;
    double gb = melhor_global;
    double w = 0.9, fi1 = 0.3, fi2 = 0.5, c1 = 2, c2 = 2;

    return atual + (c1 * fi1 * (pb - atual)) + (c2 * fi2 * (gb - atual));
}

////////////////////
/* Funcao que verifica se todos os membros possuem CHs vizinhos */
char verificaLeafs(string particula, map<int,pair<int, int>> folhas, vector<Neighb> neig){
    char ret = 'S';
    vector<int> chs = {};
    for(map<int,pair<int, int>>::iterator mp2 = folhas.begin(); mp2!=folhas.end(); mp2++){
        for (int i=0;i<particula.length();i++){
            if (particula[i] == '1' && i == mp2->second.second){
                chs.push_back(mp2->first);
            }
        }
    }
    int count=0;
    for (int i=0;i<particula.length();i++){
        for(map<int,pair<int, int>>::iterator mp2 = folhas.begin(); mp2!=folhas.end(); mp2++){
            if (particula[i] == '0' && i == mp2->second.second){
                for(vector<Neighb>::iterator ng = neig.begin(); ng!=neig.end(); ng++){
                    if (mp2->first == ng->nodeID){
                        // cout << mp2->first << " membro" << endl; // sinaliza a cada novo membro para procurar CHs
                        for(int j=0; j < ng->nViz; j++){ // cout << ng->vizinhos[j] << " avaliado" << endl;
                            for(int k=0; k < chs.size(); k++){
                                if (ng->vizinhos[j] == chs[k]){
                                    // cout << " CH encontrado " << chs[k] << endl;
                                    count++;
                                }
                            }
                            
                        }
                    }
                }
                // cout << count << endl;
                if (count == 0) return 'N';
                else count = 0;
            }
        }
    }

    return ret;
}

////////////////////
/* Funcao que verifica se todos os CHs encontram pelo menos um CH */
char verificaCHs(string particula, map<int,pair<int, int>> folhas, vector<Neighb> neig){
    char ret = 'S';
    vector<int> chs = {};
    for(map<int,pair<int, int>>::iterator mp2 = folhas.begin(); mp2!=folhas.end(); mp2++){
        for (int i=0;i<particula.length();i++){
            if (particula[i] == '1' && i == mp2->second.second){
                chs.push_back(mp2->first);
            }
        }
    }
    int countCh = 0;
    for (int i=0;i<particula.length();i++){ // percorre a particula e identifica os 1s
        for(map<int,pair<int, int>>::iterator mp2 = folhas.begin(); mp2!=folhas.end(); mp2++){
            if (particula[i] == '1' && i == mp2->second.second){ // se 1 ta na posicao do map2
                for(vector<Neighb>::iterator ng = neig.begin(); ng!=neig.end(); ng++){
                    if (mp2->first == ng->nodeID){ // se o da posicao correspondente tem id = ng
                        // cout << " ch " << mp2->first << " verificado" << endl;
                        for(int j=0; j < ng->nViz; j++){ // cout << ng->vizinhos[j] << " avaliado" << endl;
                            for(int k=0; k < chs.size(); k++){
                                if (ng->vizinhos[j] == chs[k]){
                                    // cout << "ngVizinhos = " << ng->vizinhos[j] << endl;
                                    // cout << ng->nodeID << " tem vizinho_ch " << chs[k] << " //";
                                    countCh++;
                                }
                            }
                        }
                    }
                }
                if (countCh == 0 && i < 12) ret = 'N';
                else countCh = 0;
            }
        }
    }
    return ret;
}

////////////////////
/* Funcao que define se a configuracao de CHs escolhidos atende aos requisitos de conectividade */
char factivel(char membros, char ch){
    if (membros == 'S' && ch == 'S')
        return 'S';
    else
        return 'N';
}

////////////////////
/* Funcao de velocidade da particula com deslocamento a esquerda */
string shift_left(string particula, int vez, map<int,pair<int, int>> mapa, vector<Neighb> neig){
        int _v = 0;
        int otimo_old = 100, otimo_new = 0;
        while (_v <= vez){
            otimo_old = somatorio(particula,mapa);
            string aux = "";
            char ini = particula[0];
            for(int i=1; i<particula.length(); i++)
                aux += particula[i];
            particula = "";
            particula = aux + ini;
            cout << particula;
            otimo_new = somatorio(particula,mapa);
            cout << " = " << otimo_new << " \tPSO = " << executa_PSO(somatorio(particula,mapa)) << "  \tsigmoide(part3) = " << sigmoide(executa_PSO(somatorio(particula,mapa))) << " factivel = " << factivel(verificaLeafs(particula,mapa,neig), verificaCHs(particula,mapa,neig)) << endl;
            _v++;
            if(factivel(verificaLeafs(particula,mapa,neig), verificaCHs(particula,mapa,neig)) == 'S'){
                if(find(solucoes.begin(), solucoes.end(), particula) == solucoes.end())
                    solucoes.push_back(particula);
                if (somatorio(particula,mapa) < melhor_local)
                    melhor_local = somatorio(particula,mapa);
                if (melhor_local < melhor_global)
                    melhor_global = melhor_local;                    
            }      
        }
        return particula;
}

////////////////////
/* Funcao de velocidade da particula com deslocamento a direita */
string shift_right(string particula, int vez, map<int,pair<int, int>> mapa,vector<Neighb> neig){
        int _v = 0;
        int otimo_old = 100, otimo_new = 0;
        string aux;
        while (_v <= vez){
            aux = "";
            char fim = particula[particula.length()-1];
            aux += fim;
            for(int i=0; i<particula.length() - 1; i++)
                aux += particula[i];
            particula = "";
            particula = aux;
            cout << particula;
            otimo_new = somatorio(particula,mapa);
            cout << " = " << otimo_new << " \tPSO = " << executa_PSO(somatorio(particula,mapa)) << "  \tsigmoide(part3) = " << sigmoide(executa_PSO(somatorio(particula,mapa))) << " \tfactivel = " << factivel(verificaLeafs(particula,mapa,neig), verificaCHs(particula,mapa,neig)) << endl;
            _v++;
            if(factivel(verificaLeafs(particula,mapa,neig), verificaCHs(particula,mapa,neig)) == 'S'){
                if(find(solucoes.begin(), solucoes.end(), particula) == solucoes.end())
                    solucoes.push_back(particula);
                if (somatorio(particula,mapa) < melhor_local)
                    melhor_local = somatorio(particula,mapa);
                if (melhor_local < melhor_global)
                    melhor_global = melhor_local;                    
            }
        }
        return particula;
}

////////////////////
/* Funcao MAIN */
int main(int argc, char const *argv[]){

    vector<Neighb> __neighb = read__csv(); 

    // cout << endl << "DADOS DE TODOS" << endl;
    // for(vector<Neighb>::iterator it = __neighb.begin();it!=__neighb.end();it++){
    //     cout <<  it->nodeID << " " << it->depth << " " << it->datarate << " " << it->viz << " " << it->blue << " " << it->red << " " << it->cluster << endl;
    // }
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
    map<int,int> mapa1;                        // estrutura com nodos e quantidade total de vizinhos
    map<int, pair<int, int>> mapa2, mapa3;     // estruturas para percorrer posicao da particula [0 a 16]
    string particula1, particula2, particula3; // representacao das particulas em string binaria do 0 a 16
    map<int,int> lista;

    // Procura vizinhos somente de vermelhos
    for(vector<Neighb>::iterator it = __neighb.begin(); it!=__neighb.end(); it++){  // percorre a lista de nodos
        if (it->cluster == "vermelho"){                 // seleciona somente vermelhos
            cnt++;
            particula2 += '0';
            particula3 += '0';
            for ( a = 0; a < it->nViz; a++){
                for(vector<Neighb>::iterator it_ = __neighb.begin(); it_ != __neighb.end(); it_++){
                    if (it->vizinhos[a] == it_->nodeID){
                        it->setHet(it->cluster, it_->cluster);
                        if (it->cluster != it_->cluster){
                            it->somaDepthViz(it_->depth);
                            somaD += it_->depth;
                            somaH += it->nHet;
                            lista[it->nodeID] = it->somaViz;
                        }else{
                            it->somaDepthVizHom(it_->depth);
                        }
                    }
                }
            }
            mapa1[it->nodeID] = it->somaViz;
            mapa3[it->nodeID] = make_pair(it->somaVizHom,posi);
            mapa2[it->nodeID] = make_pair(it->depth,posi);
            posi++;
            relacaoViz = it->somaViz / it->somaVizHom; // abs(it->somaViz - it->somaVizHom);
            medD = somaD / cnt;
            medH = somaH / cnt;
            somaR += relacaoViz;
            medR = somaR / cnt;
            cout <<  it->nodeID << " \t" << it->depth << " " << "\t" << it->blue << " \t" << it->red << // "\tdifer = " << it->nHet << 
                " \tsomaD_HET = " << it->somaViz << " \tsomaD_HOM = " << it->somaVizHom << " \tHET/D: " << (double) (it->nHet / it->depth) << // " \tpropDif: " << propDif << " \tpropDepth: " << propDep << 
                " \trelacaoViz: " <<  relacaoViz << endl;
        }
    }

    identificaHotspots(__neighb, mapa2);

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
    d=4;
    // Particula 3 define como CH os nodos representante de cada profundidade
    for(int w = particula3.length(); w > 0; w--){
        for(map<int, pair<int, int>>::iterator mp = mapa2.end(); mp != mapa2.begin(); mp--){
            // cout << mp->first << " " << mp->second.first << " " << mp->second.second << " " << d << endl;
            if ( d == mp->second.first ){
                // cout << "d = " << d << " it->d = " << mp->second << " w = " << w << endl;
                particula3[mp->second.second] = '1';
                break;
            }
        }
        d--;
    }
    string particula4 = "0101000010011000";
    string particula5 = "1000010000011000";
    string particula6 = "1000010010110000";
    int somaP1 = somatorio(particula1,mapa2);
    int somaP2 = somatorio(particula2,mapa2);
    int somaP3 = somatorio(particula3,mapa2);
    int somaP4 = somatorio(particula4,mapa2);
    int somaP5 = somatorio(particula5,mapa2);
    int somaP6 = somatorio(particula6,mapa2);
    if (somaP1 < melhor_global){
        melhor_global = somaP1;
    }
    if (somaP2 < melhor_global){
        melhor_global = somaP2;
    }
    if (somaP3 < melhor_global){
        melhor_global = somaP3;
    }
    if (somaP4 < melhor_global){
        melhor_global = somaP4;
    }
    if (somaP5 < melhor_global){
        melhor_global = somaP5;
    }
    if (somaP6 < melhor_global){
        melhor_global = somaP6;
    }
    cout << "a = " << a << " cnt = " << cnt << " media de vizinhos heterogeneos = " << medH << " media de profundiades = " << medD << " relacao = " << medR << endl << endl;
    cout << "melhor_global = " << melhor_global << endl;
    cout << "particula1 = " << particula1 << " = " << somaP1 << " PSO = " << executa_PSO(somatorio(particula1,mapa2)) << " \tsigmoide(part1) = " << sigmoide(executa_PSO(somatorio(particula1,mapa2))) << " \tfactivel = " << verificaLeafs(particula1,mapa2,__neighb) << " \tfacCH = " << verificaCHs(particula1,mapa2,__neighb) <<endl;
    cout << "particula2 = " << particula2 << " = " << somaP2 << " PSO = " << executa_PSO(somatorio(particula2,mapa2)) << " \tsigmoide(part2) = " << sigmoide(executa_PSO(somatorio(particula2,mapa2))) << " \tfactivel = " << verificaLeafs(particula2,mapa2,__neighb) << " \tfacCH = " << verificaCHs(particula2,mapa2,__neighb) << endl;
    cout << "particula3 = " << particula3 << " = " << somaP3 << " PSO = " << executa_PSO(somatorio(particula3,mapa2)) << " \tsigmoide(part3) = " << sigmoide(executa_PSO(somatorio(particula3,mapa2))) << " \tfactivel = " << verificaLeafs(particula3,mapa2,__neighb) << " \tfacCH = " << verificaCHs(particula3,mapa2,__neighb) << endl;
    cout << "particula4 = " << particula4 << " = " << somaP4 << " PSO = " << executa_PSO(somatorio(particula4,mapa2)) << " \tsigmoide(part4) = " << sigmoide(executa_PSO(somatorio(particula4,mapa2))) << " \tfactivel = " << verificaLeafs(particula4,mapa2,__neighb) << " \tfacCH = " << verificaCHs(particula4,mapa2,__neighb) << endl;
    cout << "particula5 = " << particula5 << " = " << somaP5 << " PSO = " << executa_PSO(somatorio(particula5,mapa2)) << " \tsigmoide(part3) = " << sigmoide(executa_PSO(somatorio(particula5,mapa2))) << " \tfactivel = " << verificaLeafs(particula5,mapa2,__neighb) << " \tfacCH = " << verificaCHs(particula5,mapa2,__neighb) << endl;
    cout << "particula6 = " << particula6 << " = " << somaP6 << " PSO = " << executa_PSO(somatorio(particula6,mapa2)) << " \tsigmoide(part3) = " << sigmoide(executa_PSO(somatorio(particula6,mapa2))) << " \tfactivel = " << verificaLeafs(particula6,mapa2,__neighb) << " \tfacCH = " << verificaCHs(particula6,mapa2,__neighb) << endl;
 
    // cout << "sigmoide(part2) = " << sigmoide(13.4) << " PSO = " << executa_PSO(,22) << endl; 
    cout << endl;

    cout << "SHIFT_LEFT " << "P1 = " << endl;
    cout << particula1 << endl;
    shift_left(particula1,8,mapa2,__neighb);
    cout << endl << "SHIFT_RIGHT P1 = " << endl;
    cout << particula1 << endl;
    shift_right(particula1,8, mapa2,__neighb);

    cout << endl << "SHIFT_LEFT " << "P2 = " << endl;
    cout << particula2 << endl;
    shift_left(particula2,8,mapa2,__neighb);
    cout << endl << "SHIFT_RIGHT P2 = " << endl;
    cout << particula2 << endl;
    shift_right(particula2,8,mapa2,__neighb);
    cout << endl;
    
    cout << endl;

    cout << "SHIFT_LEFT " << "P3 = " << endl;
    cout << particula3 << endl;
    shift_left(particula3,8,mapa2,__neighb);
    cout << endl << "SHIFT_RIGHT P3 = " << endl;
    cout << particula3 << endl;
    shift_right(particula3,8, mapa2,__neighb);
    cout << endl;

    cout << "SHIFT_LEFT " << "P4 = " << endl;
    cout << particula4 << endl;
    shift_left(particula4,8,mapa2,__neighb);
    cout << endl << "SHIFT_RIGHT P4 = " << endl;
    cout << particula4 << endl;
    shift_right(particula4,8, mapa2,__neighb);
    cout << endl;

    cout << "SHIFT_LEFT " << "P5 = " << endl;
    cout << particula5 << endl;
    shift_left(particula5,8,mapa2,__neighb);
    cout << endl << "SHIFT_RIGHT P5 = " << endl;
    cout << particula5 << endl;
    shift_right(particula5,8, mapa2,__neighb);
    cout << endl;

    cout << "SHIFT_LEFT " << "P6 = " << endl;
    cout << particula6 << endl;
    shift_left(particula6,8,mapa2,__neighb);
    cout << endl << "SHIFT_RIGHT P6 = " << endl;
    cout << particula6 << endl;
    shift_right(particula6,8, mapa2,__neighb);
    cout << endl;
    cout << endl << "Solucoes factiveis = " << endl;

    for (vector<string>::iterator its = solucoes.begin(); its!=solucoes.end(); its++)
        cout << *its << " = " << somatorio(*its,mapa2) << endl;

    return 0;
}