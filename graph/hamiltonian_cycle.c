#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
bool isValidNextVertex(Graph* graph, int v, int path[], int pos);
bool hamiltonianCycleUtil(Graph* graph, int path[], int pos);
bool findHamiltonianCycle(Graph* graph);



int main() {
	
    int V = 6; // Number of vertices
    Graph* graph = createGraph(V);

    addEdge(graph, 0, 1, 0, false);
	addEdge(graph, 0, 2, 0, false);
	addEdge(graph, 1, 2, 0, false);
	addEdge(graph, 2, 3, 0, false); // Add an edge to complete the cycle
	addEdge(graph, 3, 4, 0, false); // Add edges to complete the cycle
	addEdge(graph, 4, 5, 0, false); // Add an edge to complete the cycle
	addEdge(graph, 5, 0, 0, false); // Add an edge to complete the cycle

    
    // Print the original graph
    printGraph(graph);
	
	findHamiltonianCycle(graph);
	
	
	
	
	
	
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

bool isValidNextVertex(Graph* graph, int v, int path[], int pos) {
    if (!graph->visited[v]) {
        AdjNode* current = graph->array[path[pos - 1]].head;
        while (current != NULL) {
            if (current->vertex == v) {
                return true;
            }
            current = current->next;
        }
    }
    return false;
}

bool hamiltonianCycleUtil(Graph* graph, int path[], int pos) {
	
    if (pos == graph->V) {
        int startVertex = path[0];
        AdjNode* lastVertex = graph->array[path[pos - 1]].head;
        while (lastVertex != NULL) {
            if (lastVertex->vertex == startVertex) {
                return true;  // Found a Hamiltonian Cycle
            }
            lastVertex = lastVertex->next;
        }
        return false;  // No cycle found
    }

    for (int v = 1; v < graph->V; v++) {
        if (isValidNextVertex(graph, v, path, pos)) {
            path[pos] = v;
            graph->visited[v] = 1;
            
            printf("Selected vertex: %d\n", v); // Display v
            
            if (hamiltonianCycleUtil(graph, path, pos + 1)) {
                return true;
            }
            
            path[pos] = -1;  // Backtrack
            graph->visited[v] = 0;
        }
    }

    return false;
}

bool findHamiltonianCycle(Graph* graph) {
	
    int path[graph->V];
    for (int i = 0; i < graph->V; i++) {
        path[i] = -1;
        graph->visited[i] = 0;
    }

    // Start from vertex 0
    path[0] = 0;
    graph->visited[0] = 1;

    if (hamiltonianCycleUtil(graph, path, 1)) {
        printf("Hamiltonian Cycle found: ");
        for (int i = 0; i < graph->V; i++) {
            printf("%d ", path[i]);
        }
        printf("%d\n", path[0]);  // Return to starting vertex
        return true;
    } else {
        printf("No Hamiltonian Cycle found.\n");
        return false;
    }
}

