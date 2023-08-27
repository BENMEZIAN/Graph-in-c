#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define INFINITY 10000

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
    int* visited;
}Graph;


Graph* createGraph(int vertices);
void addEdge(Graph* graph, int source, int destination, int weight, bool isDirected);
void removeEdge(Graph* G, int src, int dest, bool isDirected);
void addVertex(Graph* graph);
void removeVertex(Graph* graph, int vertexToRemove);
void printGraph(Graph* graph);
void Dijkstra(Graph* graph,int startVertex);
void DijkstraStartEnd(Graph* graph,int startVertex,int goalVertex);




int main() {
	
    int V = 6;
    Graph* graph = createGraph(V);

    addEdge(graph, 0, 1, 5, true);
    addEdge(graph, 0, 3, 3, true);
    addEdge(graph, 1, 2, 2, true);
    addEdge(graph, 3, 2, 8, true);
    addEdge(graph, 3, 4, 4, true);
    
    printGraph(graph);
	int sv = 0;
	int ev = 4;
	printf("The shortest paths from %d as start vertex are:",sv);
	Dijkstra(graph,sv);
	
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

void Dijkstra(Graph* graph, int startVertex) { // to print all the shortest paths from startVertex to all the other vertexes 
    // Initialization
    int V = graph->V;
    int distances[V];
    int previous[V];
    bool visited[V];

    for (int i = 0; i < V; ++i) {
        distances[i] = INFINITY;
        previous[i] = -1;
        visited[i] = false;
    }

    distances[startVertex] = 0;

    // Main algorithm loop
    for (int count = 0; count < V - 1; ++count) {
        int minDistance = INFINITY;
        int minIndex = -1;

        // Find the vertex with the minimum distance value
        for (int v = 0; v < V; ++v) {
            if (!visited[v] && distances[v] < minDistance) {
                minDistance = distances[v];
                minIndex = v;
            }
        }

        if (minIndex == -1) {
            break; // No more reachable vertices
        }

        visited[minIndex] = true;

        // Update distances for adjacent vertices
        AdjNode* current = graph->array[minIndex].head;
        while (current != NULL) {
            int neighbor = current->vertex;
            int weight = current->weight;
            if (!visited[neighbor] && distances[minIndex] != INFINITY && distances[minIndex] + weight < distances[neighbor]) {
                distances[neighbor] = distances[minIndex] + weight;
                previous[neighbor] = minIndex;
            }
            current = current->next;
        }
    }
    
    // Print shortest distances and paths
    printf("\n-----------------------------------------------\n");
    printf("Vertex\tDistance\tPath\n");
    for (int i = 0; i < V; ++i) {
        printf("%d\t%d\t\t", i, distances[i]);
        int pathStack[V]; // For storing the path in reverse order
        int stackSize = 0;

        // Build the path using the previous array
        int j = i;
        while (j != -1) {
            pathStack[stackSize++] = j;
            j = previous[j];
        }

        // Print the path
        for (int k = stackSize - 1; k >= 0; --k) {
            printf("%d", pathStack[k]);
            if (k > 0) {
                printf(" -> ");
            }
        }
        printf("\n");
    }
    printf("-----------------------------------------------\n");
}

void DijkstraStartEnd(Graph* graph, int startVertex, int goalVertex) { // to print all the shortest path from startVertex to goalvertex
    // Initialization
    int V = graph->V;
    int distances[V];
    int previous[V];
    bool visited[V];

    for (int i = 0; i < V; ++i) {
        distances[i] = INFINITY;
        previous[i] = -1;
        visited[i] = false;
    }

    distances[startVertex] = 0;

    // Main algorithm loop
    for (int count = 0; count < V - 1; ++count) {
        int minDistance = INFINITY;
        int minIndex = -1;

        // Find the vertex with the minimum distance value
        for (int v = 0; v < V; ++v) {
            if (!visited[v] && distances[v] < minDistance) {
                minDistance = distances[v];
                minIndex = v;
            }
        }

        if (minIndex == -1 || minIndex == goalVertex) {
            break; // No more reachable vertices or reached the goal vertex
        }

        visited[minIndex] = true;

        // Update distances for adjacent vertices
        AdjNode* current = graph->array[minIndex].head;
        while (current != NULL) {
            int neighbor = current->vertex;
            int weight = current->weight;
            if (!visited[neighbor] && distances[minIndex] != INFINITY && distances[minIndex] + weight < distances[neighbor]) {
                distances[neighbor] = distances[minIndex] + weight;
                previous[neighbor] = minIndex;
            }
            current = current->next;
        }
    }

	// Print the shortest distance and reversed path from the start vertex to the goal vertex
   // printf("Shortest path from %d to %d having %d as distance is: ", startVertex, goalVertex, distances[goalVertex]);
    int current = goalVertex;
    int reversedPath[V];
    int pathLength = 0;
    while (current != -1) {
        reversedPath[pathLength++] = current;
        current = previous[current];
    }
    
    for (int i = pathLength - 1; i >= 0; --i) {
        printf("%d", reversedPath[i]);
        if (i > 0) {
            printf(" -> ");
        }
    }
    printf("\n");    
}

