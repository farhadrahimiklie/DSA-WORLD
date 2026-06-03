#ifndef QUEUE_H
#define QUEUE_H

typedef struct {
    int *array;
    int size;
    int front;
    int rear;
}Queue;

void init_queue(Queue *queue);
int isEmpty(Queue *ptr);
int isFull(Queue *ptr);
void enqueue(Queue **ptr, int data);
int dequeue(Queue **ptr);
int Peek(Queue *ptr);
void free_queue(Queue *queue);

#endif
