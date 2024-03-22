#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>


using namespace std;

int total_cost(vector<int> costs){
    int total = 0;
    for(int i=0; i<costs.size(); i++){
        total += costs[i];
    }

    return total;
}

typedef vector<pair<int,int>> Graph;

void read_graph (ifstream & in_file, Graph g[], int V, int E) {
    for (int i = 0; i < E; i++) {
        int a, b, cost;
        in_file >> a >> b >> cost;
        
        if (a < 1 || b < 1 || a > V || b > V) {
            cerr << "-- O vertice " << (a > V ? a : b) << " nao existe no grafo " 
                <<"logo nao ha como adicionar adjacencia, linha: " << (i + 2) << endl;

            in_file.close();
            exit(1);
        }

        if (cost < 0) {
            cerr << "-- O algoritmo de Dijkstra nao aceita arestas de custo negativo " 
                <<"logo nao ha como adicionar adjacencia, linha: " << (i + 2) << endl;

            in_file.close();
            exit(1);
        }

        g[a - 1].push_back(make_pair(b - 1, cost));
        g[b - 1].push_back(make_pair(a - 1, cost));
    }
}

int size() return V;
vector<node*> get_edges() {return adjlist;}


pair < vector<int>, int> prim(Graph g, int source){
    int v = g.size();
    vector<node*> edges = g.edges(); 
    vector<int> costs;
    vector<int> prev;
    vector<bool> visited;

    for(int i=0; i < v; i++){
        costs.push_back(INT_MAX);
        prev.push_back(-1);  
        visited.push_back(false);
    }

    priority_queue< pair<int,int>, vector <pair<int,int>> , greater<pair<int,int>> > min_heap;
    

    costs[source] = 0;
    min_heap.push(make_pair(costs[source], source));

    while(min_heap.size() > 0){
        int u = min_heap.top().second; 
        min_heap.pop();
        visited[u] = true;  
        node *current = edges[u];
        while(current != NULL){
            int weight = current->weight;
            int dest = current->dest;

            if(!visited[dest] && costs[dest] > weight){
                costs[dest] = weight;
                prev[dest] = u; 
                min_heap.push(make_pair(costs[dest], dest));
            }
            current = current->next;
        }
    }
    
    return make_pair(prev, total_cost(costs)); 

}

void help(){
    printf("Alg de Prim para uma AGM:\n"
    "Para calcular o custo da árvore geradora mínima, use: \n./prim -f entrada.in -i x\n"
    "Para mostrar a árvore geradora completa, use: \n./prim -f entrada.in -i x -s\n");
}

int main(int argc, char* argv[]){
    int f = 0, o = 0, s = 0, in = 0, l = 0;
    int source = 0, dest;
    char* input_name;
    char* output_name;
    // Recebendo e tratando os argumentos
    for (int i = 0; i < argc; i++){
        if(!strcmp(argv[i], "-h")) help();
        if(!strcmp(argv[i], "-o")){o++; output_name = argv[i+1]; }
        if(!strcmp(argv[i], "-f")){f++; input_name = argv[i+1]; } 
        if(!strcmp(argv[i], "-i")){ in++; source = stoi(argv[i+1]); }
        if(!strcmp(argv[i], "-l")){ l++; dest = stoi(argv[i+1]); }
        if(!strcmp(argv[i], "-s")) s++;
    }  
    ofstream output_file;
    output_file.open(output_name);
    
    if(f){
        Graph g = read_graph(input_name);
        
        if (!s){
            pair < vector<int>, int>  cost = prim(g, source);
            if (!o) printf("custo da árvore geradora mínima: %d\n", cost.second);
            else{
                output_file << "custo da árvore geradora mínima: " << cost.second << endl; 
            }
        }
        else{
            pair < vector<int>, int>  par = prim(g, source);
            vector<int> costs = par.first;
            if (!o) printf("árvore geradora mínima: "); 
            else{
                output_file << "árvore geradora mínima: " << endl;
            }
            for (int i = 0; i < costs.size(); i++){  
                if (costs[i] == -1) continue;
                if (!o) printf("(%d,%d)  ", costs[i], i); 
                else{
                    output_file  << "(" << costs[i] << "," << i << ")  ";
                }
            }
            printf("\n");
            output_file << endl;
        }
    }
    output_file.close(); 
    return 0;
}
