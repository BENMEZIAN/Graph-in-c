#include <stdio.h>
#include <stdlib.h>

// Node
typedef struct Node{
	int data;
	int priority; // Lower values indicate higher priority
	struct Node* next;

}Node;

// Function to Create A New Node
Node* newNode(int d, int p){
	Node* temp = (Node*)malloc(sizeof(Node));
	temp->data = d;
	temp->priority = p;
	temp->next = NULL;

	return temp;
}

typedef struct PriorityQueue{
    Node* front;
    Node* rear;
}PriorityQueue;


void init(PriorityQueue* pq){
	pq->front = NULL;
    pq->rear = NULL;
}

void enqueue(PriorityQueue* pq, int d, int p){
    
	Node* temp = newNode(d, p);

    if (pq->front == NULL || pq->front->priority > p) {
        temp->next = pq->front;
        pq->front = temp;
    } else {
        Node* start = pq->front;
        while (start->next != NULL && start->next->priority < p) {
            start = start->next;
        }
        temp->next = start->next;
        start->next = temp;
    }

    if (pq->rear == NULL || pq->rear->priority < p) {
        pq->rear = temp;
    }
}

int isEmpty(PriorityQueue* pq) {
    return pq->front == NULL;
}

int peek(PriorityQueue* pq) {
    return pq->front->data;
}

void dequeue(PriorityQueue* pq){
	
    if (pq->front == NULL) {
        return;
    }
    Node* temp = pq->front;
    pq->front = pq->front->next;
    if (pq->front == NULL) {
        pq->rear = NULL;
    }
    free(temp);
}

void printQueue(PriorityQueue* pq) {
    
	if (isEmpty(pq)) {
        printf("Queue is empty\n");
        return;
    }

    printf("Priority queue: ");
    Node* curr = pq->front;
    while (curr != NULL) {
        printf("%d->", curr->data);
        curr = curr->next;
    }
    printf("\n");
}
