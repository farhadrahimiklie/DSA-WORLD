#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void init_queue(Queue *queue){
    queue->size = 6;
    queue->front = queue->rear = -1;
    queue->array = (int*)malloc(queue->size * sizeof(int));
    if (queue->array == NULL) {
        printf("Memory Allocation is Failed\n");
        exit(EXIT_FAILURE);
    }
    return;
}

int isEmpty(Queue *ptr){
    if (ptr->front == ptr->rear) {
        return 1;
    }else {
        return 0;
    }
}

int isFull(Queue *ptr){
    if (ptr->rear == ptr->size-1) {
        return 1;
    }else {
        return 0;
    }
}

void enqueue(Queue **ptr, int data){
    if (isFull(*ptr)) {
        printf("queue is full.\n");
        return;
    }

    (*ptr)->rear++;
    (*ptr)->array[(*ptr)->rear] = data;
    return;
}

int dequeue(Queue **ptr){
    if (isEmpty(*ptr)) {
        printf("Queue is empty\n");
        return -1;
    }
    (*ptr)->front++;
    int x = (*ptr)->array[(*ptr)->front];
    return x;
}

int Peek(Queue *ptr){
    if (isEmpty(ptr)) {
        printf("Queue is Empty\n");
        return -1;
    }

    return ptr->array[ptr->front];
}

void free_queue(Queue *queue){
    if(queue == NULL){
        return;
    }

    free(queue->array);
    queue->array = NULL;

    free(queue);
}

int main(){
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) {
        printf("Memory Allocation is Failed\n");
        exit(EXIT_FAILURE);
    }

    init_queue(queue);
}
