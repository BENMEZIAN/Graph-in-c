#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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
    int* visited;
}Graph;


Graph* createGraph(int vertices);
void addEdge(Graph* graph, int source, int destination, int weight, bool isDirected);
void removeEdge(Graph* G, int src, int dest, bool isDirected);
void addVertex(Graph* graph);
void removeVertex(Graph* graph, int vertexToRemove);
void printGraph(Graph* graph);
void tarjanDFS(int vertex, Graph* graph, int* discovery, int* lowLink, bool* onStack, stack* stack, int* index, int* sccCount);
void tarjan_SCC(Graph* graph);




int main() {
	
    int numNodes = 10;
    Graph *graph = createGraph(numNodes);

    addEdge(graph, 0, 1, 0, true);
    addEdge(graph, 1, 2, 0, true);
    addEdge(graph, 2, 0, 0, true);
    addEdge(graph, 2, 3, 0, true);
    addEdge(graph, 3, 4, 0, true);
    addEdge(graph, 4, 5, 0, true);
    addEdge(graph, 5, 3, 0, true);
    addEdge(graph, 6, 4, 0, true);
    addEdge(graph, 6, 7, 0, true);
    addEdge(graph, 7, 8, 0, true);
    addEdge(graph, 8, 9, 0, true);
    addEdge(graph, 9, 6, 0, true);
    
    printGraph(graph);
	
	tarjan_SCC(graph);
	
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

void tarjanDFS(int vertex, Graph* graph, int* discovery, int* lowLink, bool* onStack, stack* stack, int* index, int* sccCount){
	
    discovery[vertex] = *index;
    lowLink[vertex] = *index;
    (*index)++;
    push(stack, vertex);
    onStack[vertex] = true;

    AdjNode* node = graph->array[vertex].head;
    while(node != NULL){
        int neighbor = node->vertex;
        if(discovery[neighbor] == -1){
            tarjanDFS(neighbor, graph, discovery, lowLink, onStack, stack, index, sccCount);
            lowLink[vertex] = (lowLink[vertex] < lowLink[neighbor]) ? lowLink[vertex] : lowLink[neighbor];
        } else if(onStack[neighbor]){
            lowLink[vertex] = (lowLink[vertex] < discovery[neighbor]) ? lowLink[vertex] : discovery[neighbor];
        }
        node = node->next;
    }

    if(lowLink[vertex] == discovery[vertex]){
        printf("Strongly Connected Component %d:", (*sccCount) + 1);
        int v;
        do{
            v = pop(stack);
            onStack[v] = false;
            printf(" %d", v);
        } while(v != vertex);
        printf("\n");
        (*sccCount)++;
    }
}

void tarjan_SCC(Graph* graph){
	
    int discovery[graph->V];
    int lowLink[graph->V];
    bool onStack[graph->V];
    stack s;
    int index = 0;
    int sccCount = 0;

    for(int i = 0; i < graph->V; i++){
        discovery[i] = -1;
        lowLink[i] = -1;
        onStack[i] = false;
    }

    init(&s);

    for(int i = 0; i < graph->V; i++){
        if(discovery[i] == -1){
            tarjanDFS(i, graph, discovery, lowLink, onStack, &s, &index, &sccCount);
        }
    }
}

