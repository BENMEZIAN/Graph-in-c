#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

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


typedef enum Color {
    RED, GREEN, BLUE, YELLOW, ORANGE, PINK, BLACK, BROWN, WHITE, PURPLE,
    VIOLET, CYAN, MAGENTA, GRAY, GOLD, SILVER, // Add more colors here
    NUM_COLORS
} Color;

// Function to set text color
void setConsoleColor(Color c) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, c + 1); // Colors start from 1 in SetConsoleTextAttribute
}

bool isSafe(Graph* graph, int vertex, Color c, int result[]) {
    AdjNode* i = graph->array[vertex].head;
    while (i != NULL) {
        if (result[i->vertex] == c)
            return false;
        i = i->next;
    }
    return true;
}

void colorGraph(Graph* graph) {
    int result[graph->V];

    // Initialize all vertices with no color assigned (-1)
    for (int u = 0; u < graph->V; u++)
        result[u] = -1;

    for (int u = 0; u < graph->V; u++) {
        for (Color c = RED; c < NUM_COLORS; c++) {
            if (isSafe(graph, u, c, result)) {
                result[u] = c;
                break;
            }
        }
    }

    // Array of color names for printing
    const char* colorNames[NUM_COLORS] = {
        "RED", "GREEN", "BLUE", "YELLOW", "ORANGE", "PINK", "BLACK", "BROWN", "WHITE", "PURPLE",
        "VIOLET", "CYAN", "MAGENTA", "GRAY", "GOLD", "SILVER"
    };

    // Print the coloring result
    for (int u = 0; u < graph->V; u++) {
        printf("Vertex %d ---> Color (%d) - %s\n", u,result[u],colorNames[result[u]]);
    }
}


int main() {
	
    int V = 6; // Number of vertices
    Graph* graph = createGraph(V);

    addEdge(graph, 0, 1, 0, false);
    addEdge(graph, 0, 2, 0, false);
    addEdge(graph, 1, 2, 0, false);
    addEdge(graph, 3, 0, 0, false);
    addEdge(graph, 3, 1, 0, false);
    addEdge(graph, 4, 0, 0, false);
    addEdge(graph, 4, 1, 0, false);    
    addEdge(graph, 5, 0, 0, false);
    addEdge(graph, 5, 1, 0, false);

    
    // Print the original graph
    printGraph(graph);

    colorGraph(graph);
    /*
	Vertex 0 ---> Color (0) - RED
	Vertex 1 ---> Color (1) - GREEN
	Vertex 2 ---> Color (2) - BLUE
	Vertex 3 ---> Color (2) - BLUE
	Vertex 4 ---> Color (2) - BLUE
	Vertex 5 ---> Color (2) - BLUE
	*/
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

