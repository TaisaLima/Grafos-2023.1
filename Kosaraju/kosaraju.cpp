#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <stack>

using namespace std;

typedef vector<int> Graph;
typedef vector<vector<int>> SCC_list;

bool equal (const char *char_ptr, string str_to_cmp) {
    string str(char_ptr);
    return not str.compare(str_to_cmp);
}

void read_graph (ifstream & in_file, Graph g[], Graph t[], int V, int E) {
    for (int i = 0; i < E; i++) {
        int a, b;
        in_file >> a >> b;
        
        if (a < 1 || b < 1 || a > V || b > V) {
            cerr << "-- O vertice " << (a > V ? a : b) << " nao existe no grafo " 
                <<"logo nao ha como adicionar adjacencia, linha: " << (i + 2) << endl;

            in_file.close();
            exit(1);
        }

        g[a - 1].push_back(b - 1);
        t[b - 1].push_back(a - 1);
    }
}

void DFS (Graph g[], vector<int> & visited, stack<int> & acess_stack, int u) {
    visited[u] = true;

    for (int i = 0; i < g[u].size(); i++) {
        int v = g[u].at(i);

        if (visited[v] == false)
            DFS(g, visited, acess_stack, v);
    }

    acess_stack.push(u);
}

void DFS_scc (Graph t[], vector<int> & visited, vector<int> & scc, int u) {
    visited[u] = true;
    scc.push_back(u + 1); // cout << u + 1 << ' ';

    for (int i = 0; i < t[u].size(); i++) {
        int v = t[u].at(i);

        if (visited[v] == false)
            DFS_scc(t, visited, scc, v);
    }
}

SCC_list kosaraju (Graph g[], Graph t[], int V, int E) {
    SCC_list scc_table;

    stack<int> acess_stack;
    vector<int> visited(V, false);

    for (int u = 0; u < V; u++)
        if (visited[u] == false)
            DFS(g, visited, acess_stack, u);

    // altera todos os valores de visitado para false
    fill(visited.begin(), visited.end(), false);

    vector<int> scc;
    while (not acess_stack.empty()) {
        int u = acess_stack.top();
        acess_stack.pop();

        if (visited[u] == false) {
            DFS_scc(t, visited, scc, u);
            scc_table.push_back(scc); // cout << endl;
            scc.clear();
        }
    }

    return scc_table;
}

int main (int argc, char *argv[]) {
    string in = "";
    string out = "";

    /*Inicializa em 1 pois argv[0] contem o caminho do programa*/
    for (int i = 1; i < argc; i++) {
        if (equal(argv[i], "-h")) {
            cout    << "\t* Help *\n"
                    << "\t-h: exibe o help\n"
                    << "\t-o <arquivo>: redireciona a saida para o \"arquivo\"\n"
                    << "\t-f <arquivo>: indica o \"arquivo\" que contem o grafo de entrada\n"
                    << "\n"
                    << "\tEste programa tem como objetivo encontrar as componentes conexas de um grafo\n"
                    << "\tnao direcionado por meio do algoritmo de Kruskal.\n";

            return 0;
        } else if (equal(argv[i], "-o") && i < argc - 1) {
            out = argv[++i];
        } else if (equal(argv[i], "-f") && i < argc - 1) {
            in = argv[++i];
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

    read_graph(in_file, g, t, V, E);
    in_file.close();

    ofstream out_file;
    if (not equal("", out)) {
        out_file.open(out);
        if (not out_file.is_open()) {
            cerr << "-- Nao foi possivel abrir o arquivo de saida" << endl;
            return 1;
        }
    }
    
    SCC_list scc_table = kosaraju(g, t, V, E);

    for (int i = 0; i < scc_table.size(); i++) {
        vector<int> scc = scc_table.at(i);

        for (int j = 0; j < scc.size(); j++) {
            if (out_file.is_open())
                out_file << scc.at(j) << " ";
            else
                cout << scc.at(j) << " ";
        }

        if (out_file.is_open())
            out_file << endl;
        else
            cout << endl;
    }

    if (out_file.is_open())
        out_file.close();

    return 0;
}