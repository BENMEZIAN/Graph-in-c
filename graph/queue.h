#include <stdio.h>
#include <stdlib.h>

// Structure for a node in the linked list
typedef struct Node {
    int data;
    struct Node* next;
}Node;

// Structure for the queue
typedef struct queue{
    struct Node* front;
    struct Node* rear;
}queue;

void init(queue* q) {
    q->front = NULL;
    q->rear = NULL;
}

int isEmpty(queue* q) {
    return q->front == NULL;
}

void enqueue(queue* q, int value) {
	
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;

    if (isEmpty(q)) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

int dequeue(queue* q) {
	
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return -1; // Return a sentinel value indicating failure
    }

    Node* temp = q->front;
    int dequeuedValue = temp->data;

    q->front = q->front->next;
    free(temp);

    if (q->front == NULL) {
        q->rear = NULL;
    }

    return dequeuedValue;
}

void printQueue(queue* q) {
	
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }

    Node* curr = q->front;
    printf("Queue: ");
    while (curr != NULL) {
        printf("%d -> ", curr->data);
        curr = curr->next;
    }
    printf("NULL\n");
}

void clearQueue(queue* q) {
	
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }

    Node* curr = q->front;
    printf("Queue: ");
    while (curr != NULL) {
        printf("%d -> ", curr->data);
        curr = curr->next;
    }
    printf("NULL\n");
}
