#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

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


Graph* createGraph(int vertices);
void addEdge(Graph* graph, int source, int destination, int weight, bool isDirected);
void removeEdge(Graph* G, int src, int dest, bool isDirected);
void addVertex(Graph* graph);
void removeVertex(Graph* graph, int vertexToRemove);
void printGraph(Graph* graph);
void Dfs(Graph* graph, int startVertex); // Iterative DFS 
bool isConnected(Graph* graph);




int main() {
	
    int V = 6;
    Graph* graph = createGraph(V);

    addEdge(graph, 0, 1, 5, false);
    addEdge(graph, 0, 3, 3, false);
    addEdge(graph, 1, 2, 2, false);
    addEdge(graph, 3, 2, 8, false);
	addEdge(graph, 3, 4, 8, false);
	addEdge(graph, 5, 2, 8, false);
	
	    
    printGraph(graph);
    
    int res = isConnected(graph);
    
    if(res == 1){
	    printf("The graph is connected");	
	}else{
		printf("The graph is not connected");	
	}
	
	
	
	
	
	
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

void Dfs(Graph* graph, int startVertex){
	// Reset all vertices as not visited before starting DFS
    for (int i = 0; i < graph->V; ++i) {
        graph->visited[i] = 0;
    }
    
	stack s;
	
	init(&s);
	push(&s, startVertex);
    graph->visited[startVertex] = 1;

    while (!is_empty(&s)) {
        int currentVertex = pop(&s);
        
        AdjNode* temp = graph->array[currentVertex].head;
        while (temp != NULL) {
            int adjVertex = temp->vertex;
            if (graph->visited[adjVertex] == 0) {
                push(&s, adjVertex);
                graph->visited[adjVertex] = 1;
            }
            temp = temp->next;
        }
    }
}

bool isConnected(Graph* graph) {
    // Reset all vertices as not visited before starting DFS
    for (int i = 0; i < graph->V; ++i) {
        graph->visited[i] = 0;
    }
    
    // Perform DFS traversal from the first vertex
    Dfs(graph, 0);

    // Check if all vertices are visited
    for (int i = 0; i < graph->V; ++i) {
        if (graph->visited[i] == 0) {
            return false; // Graph is not connected
        }
    }
    
    return true; // Graph is connected
}

