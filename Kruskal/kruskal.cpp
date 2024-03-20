#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>

#define MAX __INT_MAX__ - 7

using namespace std;

class comparator {
public:
    bool operator ()(pair<int, pair<int,int>> & a, pair<int, pair<int,int>> & b) {
        if (a.first == b.first)
            return a.second.first > b.second.first;
        return a.first > b.first;
    }
};

typedef priority_queue<pair<int, pair<int,int>>, vector<pair<int, pair<int,int>>>, comparator> Edge_queue;
typedef vector<pair<int, pair<int,int>>> Edge_list;

bool equal (const char *char_ptr, string str_to_cmp) {
    string str(char_ptr);
    return not str.compare(str_to_cmp);
}

void read_graph (ifstream & in_file, Edge_queue & e, int V, int E) {
    for (int i = 0; i < E; i++) {
        int a, b, cost;
        in_file >> a >> b >> cost;
        
        if (a < 1 || b < 1 || a > V || b > V) {
            cerr << "-- O vertice " << (a > V ? a : b) << " nao existe no grafo " 
                <<"logo nao ha como adicionar adjacencia, linha: " << (i + 2) << endl;

            in_file.close();
            exit(1);
        }

        e.push({cost, {a - 1, b - 1}});
    }
}

int find_set(vector<int> & set, int u) {
    if (set[u] != u)
        set[u] = find_set(set, set[u]);
    return set[u];
}

void union_set(vector<int> & set, int u, int v) {
    int rootU = find_set(set, u);
    int rootV = find_set(set, v);

    if (rootU != rootV)
        set[rootU] = rootV;
}

Edge_list kruskal (Edge_queue & e, int V) {
    Edge_list solve;
    vector<int> set(V); // Make-Set

    for (int i = 0; i < set.size(); i++)
        set.at(i) = i;

    while (not e.empty()) {
        pair<int, pair<int,int>> edge = e.top();
        e.pop();

        int u = edge.second.first;
        int v = edge.second.second;

        if (find_set(set, u) != find_set(set, v)) {
            solve.push_back(edge);
            union_set(set, u, v);
        }
    }

    return solve;
}

int main (int argc, char *argv[]) {
    string in = "";
    string out = "";
    bool show = false;

    /*Inicializa em 1 pois argv[0] contem o caminho do programa*/
    for (int i = 1; i < argc; i++) {
        if (equal(argv[i], "-h")) {
            cout    << "\n\t* Help *\n"
                    << "\t-h: exibe o help\n"
                    << "\t-o <arquivo>: redireciona a saida para o \"arquivo\"\n"
                    << "\t-f <arquivo>: indica o \"arquivo\" que contem o grafo de entrada\n"
                    << "\t-s: apresenta na saida os vertices que formam a AGM\n"
                    << "\n"
                    << "\tEste programa tem como objetivo calcular o custo de uma AGM\n"
                    << "\t(arvore geradora minima) a partir de um grafo conexo nao direcionado\n"
                    << "\tpor meio do algoritmo de Kruskal.\n"
                    << "\tAssim como apresentar as arestas que compoem a AGM ao utilizar o -s\n\n";

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

    // Edge_queue faz com que a leitura das arestas ja faça a fila ordenada
    Edge_queue e;
    read_graph(in_file, e, V, E);
    in_file.close();
    
    Edge_list solve = kruskal(e, V);

    ofstream out_file;
    if (not equal("", out)) {
        out_file.open(out);
        if (not out_file.is_open()) {
            cerr << "-- Nao foi possivel abrir o arquivo de saida" << endl;
            return 1;
        }
    }
    
    int sum = 0;
    for (int i = 0; i < solve.size(); i++)
        sum += solve.at(i).first;

    if (show) {
        for (int i = 0; i < solve.size(); i++)
            if (out_file.is_open())
                out_file << "(" << solve.at(i).second.first + 1 << "," << solve.at(i).second.second + 1 << ") ";
            else 
                cout << "(" << solve.at(i).second.first + 1 << "," << solve.at(i).second.second + 1 << ") ";
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