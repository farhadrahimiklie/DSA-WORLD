#ifndef STACK_USING_LINKED_LIST_H
#define STACK_USING_LINKED_LIST_H

typedef struct Node{
    int data;
    struct Node *next;
}Node;

Node *CreateNode(int data);
int isEmpty(Node *top);
int isFull(void);
void push_at_stack(Node **top, int data);
int pop_from_stack(Node **top);
int Peek(Node *top);
void display(Node *top);
void free_stack_linked_list(Node **top);
#endif
