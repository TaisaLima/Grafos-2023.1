#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

typedef vector<int> Graph;
typedef struct {
    int u, v, cost;
} Edge;

bool comparator (const Edge & a, const Edge & b) {
    return a.cost > b.cost; // irá servir para ordenar as aretas em ordem decrescente
}

bool equal (const char *char_ptr, string str_to_cmp) {
    string str(char_ptr);
    return not str.compare(str_to_cmp);
}

void read_graph (ifstream & in_file, Graph g[], vector<Edge> & e, int V, int E) {
    for (int i = 0; i < E; i++) {
        int a, b, cost;
        in_file >> a >> b >> cost;
        
        if (a < 1 || b < 1 || a > V || b > V) {
            cerr << "-- O vertice " << (a > V ? a : b) << " nao existe no grafo " 
                <<"logo nao ha como adicionar adjacencia, linha: " << (i + 2) << endl;

            in_file.close();
            exit(1);
        }

        g[a - 1].push_back(b - 1);
        g[b - 1].push_back(a - 1);

        e.push_back({a - 1, b - 1, cost});
    }
}

void remove_edge(Graph g[], int u, int v) {
    for (int i = 0; i < g[u].size(); i++) {
        if (g[u].at(i) == v) {
            g[u].erase(g[u].begin() + i);
            return;
        }
    }
}

void DFS (Graph g[], vector<int> & visited, int u) {
    visited[u] = true;

    for (int i = 0; i < g[u].size(); i++) {
        int v = g[u].at(i);

        if (visited[v] == false)
            DFS(g, visited, v);
    }
}

bool verify_conectivity (Graph g[], int u, int V) {
    vector<int> visited(V, false);
    DFS(g, visited, u);
    for (int i = 0; i < V; i++)
        if (visited[i] == false)
                return false;
    return true;
}

void reverse_delete (Graph g[], vector<Edge> & e, int V, int E) {
    sort(e.begin(), e.end(), comparator);
    
    int i = 0;
    while (i < e.size()) {
        Edge edge = e.at(i);
        e.erase(e.begin() + i);

        remove_edge(g, edge.u, edge.v);
        remove_edge(g, edge.v, edge.u);

        int conex = verify_conectivity(g, edge.u, V);

        if (not conex) {
            g[edge.u].push_back(edge.v);
            g[edge.v].push_back(edge.u);

            e.insert(e.begin() + i, edge);
            i++;
        }
    }
}

int main (int argc, char *argv[]) {
    string in = "";
    string out = "";
    bool show = false;

    /*Inicializa em 1 pois argv[0] contem o caminho do programa*/
    for (int i = 1; i < argc; i++) {
        if (equal(argv[i], "-h")) {
            cout    << "\n\t* Help *\n"
                    << "\n"
                    << "\t-h: exibe o help\n"
                    << "\t-o <arquivo>: redireciona a saida para o \"arquivo\"\n"
                    << "\t-f <arquivo>: indica o \"arquivo\" que contem o grafo de entrada\n"
                    << "\t-s: apresenta na saida as arestas que formam a AGM\n"
                    << "\n"
                    << "\tEste programa tem como objetivo calcular o custo de uma AGM\n"
                    << "\t(arvore geradora minima) a partir de um grafo conexo nao direcionado\n"
                    << "\tpor meio do algoritmo ReverseDelete.\n"
                    << "\tAssim como apresentar as arestas que compoem a AGM ao utilizar o -s\n"
                    << "\n"
                    << "\tAo executar: \'make run\', o programa sera executado utilizando os casos\n"
                    << "\ttestes de \'entradas_alg5\' escrevendo os custos minimos de cada AGM em \'saidas_alg5\'.\n"
                    << "\tAo executar: \'make run_s\', o programa sera executado igualmente ao \'make run\', porem,\n"
                    << "\ta saida tera as arestas que formam cada AGM.\n\n";

            return 0;
        } else if (equal(argv[i], "-o") && i < argc - 1) {
            out = argv[++i];
        } else if (equal(argv[i], "-f") && i < argc - 1) {
            in = argv[++i];
        } else if (equal(argv[i], "-s")) {
            show = true;
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

    Graph g[V], t[V];
    vector<Edge> e;

    read_graph(in_file, g, e, V, E);
    in_file.close();

    ofstream out_file;
    if (not equal("", out)) {
        out_file.open(out);
        if (not out_file.is_open()) {
            cerr << "-- Nao foi possivel abrir o arquivo de saida" << endl;
            return 1;
        }
    }
    
    reverse_delete(g, e, V, E);

    int sum = 0;
    for (int i = 0; i < e.size(); i++)
        sum += e.at(i).cost;       

    if (show) {
        for (int i = 0; i < e.size(); i++) {
            if (out_file.is_open())
                out_file << "(" << e.at(i).u + 1 << ", " << e.at(i).v + 1 << ") ";
            else
                cout << "(" << e.at(i).u + 1 << ", " << e.at(i).v + 1 << ") ";
        }
    } else {
        if (out_file.is_open())
            out_file << sum << endl;
        else
            cout << sum << endl;
    }

    if (out_file.is_open())
        out_file.close();

    return 0;
}