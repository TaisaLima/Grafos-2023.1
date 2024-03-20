#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>

#define MAX __INT_MAX__ - 7

using namespace std;

class comparator {
public:
    bool operator ()(pair<int,int> & a, pair<int,int> b) {
        if (a.first == b.first)
            return a.second < b.second;
        return a.first < b.first;
    }
};

typedef vector<pair<int,int>> Graph;

bool equal (const char *char_ptr, string str_to_cmp) {
    string str(char_ptr);
    return not str.compare(str_to_cmp);
}

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

vector<int> dijkstra (Graph g[], int vertices, int v0) {
    vector<int> dist(vertices, MAX);
    dist[v0] = 0;

    priority_queue<pair<int,int>, vector<pair<int,int>>, comparator> queue;
    queue.push(make_pair(0, v0));

    while (not queue.empty()) {
        int  u = queue.top().second;
        queue.pop();

        for (int i = 0; i < g[u].size(); i++) {
            int v = g[u].at(i).first;
            int edge_cost = g[u].at(i).second;

            if (dist[v] > dist[u] + edge_cost) {
                dist[v] = dist[u] + edge_cost;
                queue.push(make_pair(dist[v], v));
            }
        }
    }

    return dist;
}

int main (int argc, char *argv[]) {
    string in = "";
    string out = "";
    int initial = 1;

    /*Inicializa em 1 pois argv[0] contem o caminho do programa*/
    for (int i = 1; i < argc; i++) {
        if (equal(argv[i], "-h")) {
            cout    << "\n\t* Help *\n"
                    << "\n"
                    << "\t-h: exibe o help\n"
                    << "\t-o <arquivo>: redireciona a saida para o \"arquivo\"\n"
                    << "\t-f <arquivo>: indica o \"arquivo\" que contem o grafo de entrada\n"
                    << "\t-i: indica o \"vertice\" inicial (deve ser um numero)\n"
                    << "\n"
                    << "\tEste programa tem como objetivo calcular a distancia minima para todos\n"
                    << "\tos vertices de um grafo nao direcionado a partir de um vertice inicial\n"
                    << "\tpor meio do algoritmo de Dijkstra. (Vertice inicial padrao = 1)\n"
                    << "\tO algoritmo de Dijkstra aceita apenas arestas de peso nao negativo\n\n";

            return 0;
        } else if (equal(argv[i], "-o") && i < argc - 1) {
            out = argv[++i];
        } else if (equal(argv[i], "-f") && i < argc - 1) {
            in = argv[++i];
        } else if (equal(argv[i], "-i") && i < argc - 1) {
            initial = atoi(argv[++i]);
        }
    }

    // verifica a existencia da especificação de um arquivo de entrada
    if (in == "") {
        cerr << "-- Nenhum arquivo de entrada especificado. Use o parametro -f." << endl;
        return 1;
    }

    ifstream in_file;
    in_file.open(in);

    // verifica se foi possivel abrir o arquivo de entrada
    if (not in_file.is_open()) {
        cerr << "-- Nao foi possivel abrir o arquivo de entrada" << endl;
        return 1;
    }

    int V, E;
    in_file >> V >> E;

    // verifica se o vertice inicial informado pertence ao grafo
    if (initial > V) {
        cerr << "-- O vertice inicial informado nao pertence ao grafo" << endl;
        return 1;
    }

    Graph g[V];
    read_graph(in_file, g, V, E);
    in_file.close();
    
    vector<int> dist = dijkstra(g, V, initial - 1);

    ofstream out_file;
    if (not equal("", out)) {
        out_file.open(out);
        if (not out_file.is_open()) {
            cerr << "-- Nao foi possivel abrir o arquivo de saida" << endl;
            return 1;
        }
    }

    for (int i = 0; i < V; i++)
        if (out_file.is_open())
            out_file << i + 1 << ':' << (dist[i] == MAX ? -1 : dist[i]) << ' ';
        else
            cout << i + 1 << ':' << (dist[i] == MAX ? -1 : dist[i]);
    
    if (out_file.is_open())
        out_file.close();

    return 0;
}