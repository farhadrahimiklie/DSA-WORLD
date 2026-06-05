#include <stdio.h>
#include "circular-queue.h"

#define SIZE 5

int queue[SIZE];
int front = -1;
int rear = -1;


int isEmpty()
{
    return front == -1;
}

int isFull()
{
    return (rear + 1) % SIZE == front;
}

void enqueue(int value)
{
    if (isFull())
    {
        printf("\nQueue is Full!\n");
        return;
    }

    if (isEmpty())
    {
        front = 0;
        rear = 0;
    }
    else
    {
        rear = (rear + 1) % SIZE;
    }

    queue[rear] = value;

    printf("\n%d inserted successfully.\n", value);
}

void dequeue()
{
    if (isEmpty())
    {
        printf("\nQueue is Empty!\n");
        return;
    }

    int removedValue = queue[front];

    if (front == rear)
    {
        front = -1;
        rear = -1;
    }
    else
    {
        front = (front + 1) % SIZE;
    }

    printf("\nRemoved: %d\n", removedValue);
}

void peek()
{
    if (isEmpty())
    {
        printf("\nQueue is Empty!\n");
        return;
    }

    printf("\nFront Element: %d\n", queue[front]);
}

void display()
{
    if (isEmpty())
    {
        printf("\nQueue is Empty!\n");
        return;
    }

    printf("\nQueue Elements:\n");

    int i = front;

    while (1)
    {
        printf("%d ", queue[i]);

        if (i == rear)
            break;

        i = (i + 1) % SIZE;
    }

    printf("\n");

    printf("Front Index = %d\n", front);
    printf("Rear Index  = %d\n", rear);
}

int main()
{
    enqueue(10);
    enqueue(20);
    enqueue(30);
    enqueue(40);
    enqueue(50);

    display();

    dequeue();
    dequeue();
    dequeue();

    display();

    enqueue(60);
    enqueue(70);
    enqueue(80);

    display();

    dequeue();

    display();

    return 0;
}
