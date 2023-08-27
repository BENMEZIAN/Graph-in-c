#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>


// Graph structure to store number of vertices and Adjacency matrix
typedef struct Graph{
    int V;
    int** Adj;
}Graph;


Graph* adjMatrix();
void initGraph(Graph *G);
void printGraph(Graph *G);
void addEdge(Graph *G,int x,int y,bool direction);
void removeEdge(Graph *G,int x,int y);



// Driver code
int main(int argc, char *argv[]){
	
	Graph *graph = adjMatrix();
	
	initGraph(graph);
	
	addEdge(graph, 0, 1, false);
    addEdge(graph, 0, 2, false);
    addEdge(graph, 0, 3, false);

	addEdge(graph, 1, 2, false);
	addEdge(graph, 1, 3, false);
	
    addEdge(graph, 2, 1, false);
    addEdge(graph, 2, 3, false);
    
    addEdge(graph, 3, 1, false);
    addEdge(graph, 3, 2, false);
	
	addEdge(graph, 0, 0, false);
	addEdge(graph, 1, 1, false);
    addEdge(graph, 2, 2, false);
	addEdge(graph, 3, 3, false);
	
	removeEdge(graph, 0, 0);
	removeEdge(graph, 1, 1);
    removeEdge(graph, 2, 2);
	removeEdge(graph, 3, 3);
	
	
	printGraph(graph);
	/*
	Adjacency matrix of this graph:
	  0 1 2 3
	0 0 1 1 1
	1 1 0 1 1
	2 1 1 0 1
	3 1 1 1 0
	*/
	
		
	return 0;  
}


// fonction qui crée la matrice d'adjacence d'un graphe
Graph* adjMatrix(){
    Graph* G = (Graph*)
        malloc(sizeof(Graph));
    if (!G) {
        printf("Memory Error\n");
        return NULL;
    }
    G->V = 4;
 
    G->Adj = (int**)malloc((G->V) * sizeof(int*));
    for (int k = 0; k < G->V; k++) {
        G->Adj[k] = (int*)malloc((G->V) * sizeof(int));
    }
    
    return G;
}

// fonction qui initialise un la matrice d'adjacence
void initGraph(Graph *G){
	for (int u = 0; u < G->V; u++) {
        for (int v = 0; v < G->V; v++) {
            G->Adj[u][v] = 0;
        }
    }
}

// fonction qui affiche la matrice d'adjacence du graphe
void printGraph(Graph *G){
	
	printf("Adjacency matrix of this graph:\n");
	printf("  ");
	for(int u = 0; u < G->V; u++){
		printf("%d ",u);
	}
	printf("\n");
    for (int u = 0; u < G->V; u++) {
		printf("%d ",u);
       	for (int v = 0; v < G->V;v++) {
		   printf("%d ",G->Adj[u][v]);
        }
		printf("\n");
    }
}

// fonction qui ajoute un arc entre deux sommets
void addEdge(Graph *G,int x,int y,bool direction){
	
	if(direction==true){
		G->Adj[x][y] = 1;
		G->Adj[y][x] = 0;
	}else if(direction==false){
	  	G->Adj[x][y] = 1;
		G->Adj[y][x] = 1;
	}
}

// fonction qui supprime un arc entre deux sommets
void removeEdge(Graph *G,int x,int y){
		G->Adj[x][y] = 0;
		G->Adj[y][x] = 0;	
}

