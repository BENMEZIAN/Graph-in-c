#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "queue.h"

// Node to store adjacency list
typedef struct AdjNode{
    int vertex;
    int weight;  // Weight of the edge
    struct AdjNode* next;
}AdjNode;

// Adjacency List representation
typedef struct AdjList{
    int value;         // Vertex value
    AdjNode* head;
}AdjList;

// Graph representation
typedef struct Graph{
    int V;     // Number of vertices
    AdjList* array; // Array of adjacency lists
    int *visited;
}Graph;

// Define the residual edge structure
typedef struct ResidualEdge {
    int capacity;
    int flow;
} ResidualEdge;

// Define the residual graph structure
typedef struct ResidualGraph {
    int V;
    ResidualEdge** edges; // 2D array for storing edges and capacities
} ResidualGraph;


Graph* createGraph(int vertices);
void addEdge(Graph* graph, int source, int destination, int weight, bool isDirected);
void removeEdge(Graph* G, int src, int dest, bool isDirected);
void addVertex(Graph* graph);
void removeVertex(Graph* graph, int vertexToRemove);
void printGraph(Graph* graph);
ResidualGraph* createResidualGraph(int vertices);
void addResidualEdge(ResidualGraph* rGraph, int source, int destination, int capacity,bool isDirected);
void printResidualGraph(ResidualGraph* rGraph);
void fordFulkersonBFS(Graph* graph, ResidualGraph* rGraph, int source, int target);



int main() {
	
    int V = 5;
    Graph* graph = createGraph(V);
    ResidualGraph* rGraph = createResidualGraph(V);
    
    // ... (adding edges to the original graph) ...
    addEdge(graph, 0, 1, 5, true);
    addEdge(graph, 0, 3, 3, true);
    addEdge(graph, 1, 2, 2, true);
    addEdge(graph, 2, 3, 8, true);
    addEdge(graph, 3, 4, 4, true);
    
    // Populate residual graph with capacities
    for (int i = 0; i < V; ++i) {
        AdjNode* temp = graph->array[i].head;
        while (temp != NULL) {
            addResidualEdge(rGraph, i, temp->vertex, temp->weight,true);
            temp = temp->next;
        }
    }
    
    printResidualGraph(rGraph);
    
    fordFulkersonBFS(graph, rGraph, 0, 4);
    
    
    return 0;
}


Graph* createGraph(int vertices){
	
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = vertices;
    graph->array = (AdjList*)malloc(vertices * sizeof(AdjList));
    graph->visited = (int*)malloc(vertices * sizeof(int));
    for(int i = 0; i < vertices; ++i){
        graph->array[i].value = i; // Set the vertex value
        graph->array[i].head = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

void addEdge(Graph* graph, int source, int destination, int weight, bool isDirected){
    // Create the new node
    AdjNode* newNode = (AdjNode*)malloc(sizeof(AdjNode));
    newNode->vertex = destination;
    newNode->weight = weight;
    newNode->next = NULL;

    // Add edge from source to destination
    if(graph->array[source].head == NULL){
        graph->array[source].head = newNode;
    }else{
        AdjNode* current = graph->array[source].head;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = newNode;
    }

    if(!isDirected){
        // Create the new node for the reverse edge
        newNode = (AdjNode*)malloc(sizeof(AdjNode));
        newNode->vertex = source;
        newNode->weight = weight;
        newNode->next = NULL;

        // Add edge from destination to source (undirected)
        if(graph->array[destination].head == NULL){
            graph->array[destination].head = newNode;
        }else{
            AdjNode* current = graph->array[destination].head;
            while(current->next != NULL){
                current = current->next;
            }
            current->next = newNode;
        }
    }
}

void removeEdge(Graph* G, int src, int dest, bool isDirected){
	
    if(src < 0 || src >= G->V || dest < 0 || dest >= G->V){
        printf("Invalid vertex indices.\n");
        return;
    }

    // Remove the edge from source to destination
    AdjNode* current = G->array[src].head;
    AdjNode* prev = NULL;
    while(current != NULL){
        if(current->vertex == dest){
            if(prev == NULL){
                G->array[src].head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }

    // If it's an undirected graph, also remove the reverse edge
    if(!isDirected){
        current = G->array[dest].head;
        prev = NULL;
        while(current != NULL){
            if(current->vertex == src){
                if(prev == NULL){
                    G->array[dest].head = current->next;
                }else{
                    prev->next = current->next;
                }
                free(current);
                break;
            }
            prev = current;
            current = current->next;
        }
    }
}

void addVertex(Graph* graph){
	
    int newVertex = graph->V;
    graph->V += 1;
    
    // Allocate memory for the new adjacency list array
    graph->array = (AdjList*)realloc(graph->array, graph->V * sizeof(AdjList));
    
    // Initialize the new vertex's adjacency list
    graph->array[newVertex].value = newVertex; // Set the vertex value
    graph->array[newVertex].head = NULL;
}

void removeVertex(Graph* graph, int vertexToRemove){
	
    if(vertexToRemove >= graph->V){
        return; // Invalid vertex index
    }

    // Remove all the edges connected to the vertex
    AdjNode* current = graph->array[vertexToRemove].head;
    while(current != NULL){
        AdjNode* temp = current;
        current = current->next;
        free(temp);
    }
    graph->array[vertexToRemove].head = NULL;

    // Shift the remaining vertices to fill the gap
    for(int v = vertexToRemove; v < graph->V - 1; v++){
        graph->array[v] = graph->array[v + 1];
        // Update destination of edges pointing to vertices greater than the removed vertex
        current = graph->array[v].head;
        while(current != NULL){
            if(current->vertex > vertexToRemove){
                current->vertex;
            }
            current = current->next;
        }
    }

    // Clear the last vertex's entry
    graph->array[graph->V - 1].head = NULL;

    // Update the number of vertices
    graph->V--;
}

void printGraph(Graph* graph){
	
    for(int i = 0; i < graph->V; ++i){
        printf("head %d: ", graph->array[i].value);
        AdjNode* temp = graph->array[i].head;
        while(temp != NULL){
            printf("-> %d ", temp->vertex);
            temp = temp->next;
        }
        printf("-> NULL\n");
    }
}

ResidualGraph* createResidualGraph(int vertices) {
    ResidualGraph* rGraph = (ResidualGraph*)malloc(sizeof(ResidualGraph));
    rGraph->V = vertices;
    rGraph->edges = (ResidualEdge**)malloc(vertices * sizeof(ResidualEdge*));
    for (int i = 0; i < vertices; ++i) {
        rGraph->edges[i] = (ResidualEdge*)malloc(vertices * sizeof(ResidualEdge));
        for (int j = 0; j < vertices; ++j) {
            rGraph->edges[i][j].capacity = 0;
            rGraph->edges[i][j].flow = 0;
        }
    }
    return rGraph;
}

void addResidualEdge(ResidualGraph* rGraph, int source, int destination, int capacity,bool isDirected) {
    rGraph->edges[source][destination].capacity += capacity;
    // Uncomment the line below if your graph is undirected
    if(!isDirected){
		rGraph->edges[destination][source].capacity += capacity;	
	}
}

void fordFulkersonBFS(Graph* graph, ResidualGraph* rGraph, int source, int target) {
	
    int* parent = (int*)malloc(graph->V * sizeof(int));
    int maxFlow = 0;

    while (1) {
        memset(parent, -1, graph->V * sizeof(int));

        // BFS to find augmenting path
        queue q;
        init(&q);
        enqueue(&q, source);
        parent[source] = source;
        while (!isEmpty(&q)) {
            int currentVertex = dequeue(&q);
            AdjNode* temp = graph->array[currentVertex].head;
            while (temp != NULL) {
                int adjVertex = temp->vertex;
                if (parent[adjVertex] == -1 && rGraph->edges[currentVertex][adjVertex].capacity > rGraph->edges[currentVertex][adjVertex].flow) {
                    enqueue(&q, adjVertex);
                    parent[adjVertex] = currentVertex;
                }
                temp = temp->next;
            }
        }

        // If no augmenting path found, break
        if (parent[target] == -1) {
            break;
        }

        // Find the minimum residual capacity on the augmenting path
        int pathFlow = INT_MAX;
        for (int v = target; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = fmin(pathFlow, rGraph->edges[u][v].capacity - rGraph->edges[u][v].flow);
        }
        
        // Update residual capacities and flows
        for (int v = target; v != source; v = parent[v]) {
            int u = parent[v];
            rGraph->edges[u][v].flow += pathFlow;
            rGraph->edges[v][u].flow -= pathFlow;
        }

        maxFlow += pathFlow;
    }
    
    printf("Max Flow: %d\n", maxFlow);
    free(parent);
}

void printResidualGraph(ResidualGraph* rGraph) {
    printf("Residual Graph:\n");
    for (int i = 0; i < rGraph->V; ++i) {
        for (int j = 0; j < rGraph->V; ++j) {
            printf("(%d -> %d): Capacity = %d, Flow = %d\n", i, j, rGraph->edges[i][j].capacity, rGraph->edges[i][j].flow);
        }
    }
}

