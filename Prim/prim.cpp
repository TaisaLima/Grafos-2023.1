#include <stdlib.h>
#include <bits/stdc++.h>
#include <stdio.h>
#include <limits.h>
#define MAX_LINE 305

using namespace std;
typedef struct{
  int n, m;
  int **arestas;
}GRAFO;

void freeGRAFO(GRAFO *g){
  int i = -1;
  while(++i < g->m){
    free(g->arestas[i]);
  }
  free(g->arestas);
  free(g);
}

typedef struct{
  int *pai;
  int *rank;
} SET;

void freeSET(SET *s){
  free(s->pai);
  free(s->rank);
  free(s);
}

void swap(int *a, int *b){
  int temp = *a;
  *a = *b;
  *b = temp;
}

SET *makeSet(int n){
  int i = -1;
  SET *s = (SET*) malloc(sizeof(SET));
  s->pai = (int*) malloc(n * sizeof(int));
  s->rank = (int*) malloc(n * sizeof(int));
  while(++i < n){
    s->pai[i] = i;
    s->rank[i] = 0;
  }
  return s;
}

int find(SET *s, int x){
  if(x != s->pai[x])
    s->pai[x] = find(s, s->pai[x]);
  return s->pai[x];
}

void unionSet(SET *s, int x, int y){
  int temp;
  if(s->rank[x] < s->rank[y]){
    temp = x;
    x = y;
    y = temp;
  }
  s->pai[y] = x;
  if(s->rank[x] == s->rank[y])
    s->rank[x]++;
}

void merge(int **v, int comeco, int meio, int fim, int **arrAux, int peso){
  int i = comeco, j = meio + 1, k = comeco;
  while(i <= meio && j <= fim){
    if((peso && v[i][2] < v[j][2]) || (!peso && (v[i][0] < v[j][0] || (v[i][0] == v[j][0] && v[i][1] < v[j][1])))){
      arrAux[k++] = v[i++];
    }else{
      arrAux[k++] = v[j++];
    }
  }
  while(i <= meio)
    arrAux[k++] = v[i++];
  while(j <= fim)
    arrAux[k++] = v[j++];
  for(i = comeco; i <= fim; i++)
    v[i] = arrAux[i];
}

void mergeSort(int **v, int comeco, int fim, int **arrAux, int peso){
  if(comeco < fim){
    int meio = (fim+comeco)/2;
    mergeSort(v, comeco, meio, arrAux, peso);
    mergeSort(v, meio+1, fim, arrAux, peso);
    merge(v, comeco, meio, fim, arrAux, peso);
  }
}

GRAFO* readGRAFO(FILE *inFile){
  GRAFO *g = (GRAFO*) malloc(sizeof(GRAFO));
  char line[MAX_LINE];
  int i = -1, temp;
  if(inFile != NULL)
    fscanf(inFile, "%d %d", &(g->n), &(g->m));
  else
    scanf("%d %d", &(g->n), &(g->m));
  g->arestas = (int**) malloc(g->m * sizeof(int*));
  i = -1;
  while(++i < g->m){
    g->arestas[i] = (int*) malloc(3 * sizeof(int));
    if(inFile != NULL)
      fscanf(inFile, "\n%[^\n]", line);
    else
      scanf("\n%[^\n]", line);
    if(sscanf(line, "%d %d %d", &g->arestas[i][0], &g->arestas[i][1], &g->arestas[i][2]) == 2)
      g->arestas[i][2] = 1;
    if(--g->arestas[i][0] > --g->arestas[i][1]){
      temp = g->arestas[i][0];
      g->arestas[i][0] = g->arestas[i][1];
      g->arestas[i][1] = temp;
    }
  }
  if(inFile != NULL)
		fclose(inFile);
  return g;
}

int kruskal(GRAFO *g, int **arestas, int **arrAux){
  int u, v, i = -1, tot = 0, count = 0;
  SET *s = makeSet(g->n);
  mergeSort(g->arestas, 0, g->m - 1, arrAux, 1);
  
  while(++i < g->m){
    u = find(s, g->arestas[i][0]);
    v = find(s, g->arestas[i][1]);
    if(u != v){
      arestas[count++] = g->arestas[i];
      tot += g->arestas[i][2];
      if(count == g->n - 1)
        return tot;
      unionSet(s, u, v);
    }
  }
  return tot;
}

int prim(GRAFO* g, int** arestas) {
    int i = -1, tot = 0, count = 0;
    int* key = (int*)malloc(g->n * sizeof(int));
    int* parent = (int*)malloc(g->n * sizeof(int));
    int* inMST = (int*)malloc(g->n * sizeof(int));
    priority_queue< pair<int,int>, vector <pair<int,int>> , greater<pair<int,int>> > min_heap;
    
    
    for (i = 0; i < g->n; i++) {
        key[i] = INT_MAX;
        parent[i] = -1;
        inMST[i] = 0;
    }

    key[1] = 0;
    min_heap.push(make_pair(0, 1));
    
    while (!min_heap.empty() && count < g->n - 1) {
        int u = min_heap.top().second;
        min_heap.pop();
        
        if (inMST[u] == 1)
          continue;
        
        inMST[u] = 1;

        for (i = 0; i < g->m; i++) {
            int current_u = g->arestas[i][0];
            int v = g->arestas[i][1];
            int weight = g->arestas[i][2];
            if (current_u == u) {
              if (!inMST[v] && weight < key[v]) {
                  parent[v] = u;
                  min_heap.push(make_pair(key[v], v));
                  key[v] = weight;
              }
            }
        }
        count++;
    }


    free(key);
    free(parent);
    free(inMST);
    
    return tot;
}

int readArgs(int argc, char *argv[], FILE **inFile, FILE **outFile, int *show){
  int i = 0;
  *show = 0;
  *outFile = NULL;
  *inFile = NULL;
  while(++i < argc){
    if(argv[i][0] != '-'){
      printf("Argumento não reconhecido, use o comando -h para ajudar.\n");
      return 0;
    }
    switch(argv[i][1]){
      case 'h':
        printf("Este programa tem o objetivo de calcular a \
arvore geradora mínima de um grafo \
não direcionado com vertices começando de 1 até n.\n\
Não é definido vértices iniciais nem finais. Se o argumento s for \
passado, ele mostrará também todas as arestas da arvore em ordem.\n\n\
Argumentos do programa:\n\
-h : mostra o help (Quando h é selecionado, invalida outros argumentos.)\n\
-o <arquivo> : redireciona a saida para o \"arquivo\"\n\
-f <arquivo> : indica o \"arquivo\" que contém o grafo de entrada\n\
-s : mostra arestas da arvore (ordenadas)\n\
-i : vértice inicial (não hablitado)\n\
-l : vértice final (não hablitado)\n");
        return 0;
      case 'o':
        *outFile = fopen(argv[++i], "w");
        break;
      case 'f':
        *inFile = fopen(argv[++i], "r");
        if(*inFile == NULL){
          printf("arquivo de entrada não existe.\n");
          return 0;
        }
        break;
      case 's':
        *show = 1;
        break;
      default:
        printf("Comando não reconhecido, use o comando -h para ajudar.\n");
        return 0;
    }
  }
  return 1;
}

int main(int argc, char *argv[]) {
  int **arestas, **arrAux, tot, j, i = -1, show;
  FILE *outFile, *inFile;
  if(!readArgs(argc, argv, &inFile, &outFile, &show)){
    return 0;
  }
  GRAFO *g = readGRAFO(inFile);
  arrAux = (int**) malloc(g->m * sizeof(int*));
  arestas = (int**) malloc((g->n - 1) * sizeof(int*));
  tot = kruskal(g, arestas, arrAux);
  if(show){
    mergeSort(arestas, 0, g->n - 2, arrAux, 0);
    i = -1;
    if(outFile != NULL){
      while(++i < g->n - 1){
        fprintf(outFile, "(%d,%d) ", arestas[i][0] + 1, arestas[i][1] + 1);
      }
      fclose(outFile);
    }else{
      while(++i < g->n - 1){
        printf("(%d,%d) ", arestas[i][0] + 1, arestas[i][1] + 1);
      }
    }
  }else{
    if(outFile != NULL){
      fprintf(outFile, "%d\n", tot);
      fclose(outFile);
    }else{
      printf("%d\n", tot);
    }
  }
  freeGRAFO(g);
  free(arestas);
  free(arrAux);
  return 0;
}
