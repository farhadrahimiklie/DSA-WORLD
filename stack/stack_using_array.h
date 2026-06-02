#ifndef STACK_USING_ARRAY_H
#define STACK_USING_ARRAY_H

typedef struct {
    int *storage;
    int size;
    int top;
}Stack;

void init_stack(Stack *stack);
int isEmpty(Stack *stack);
int isFull(Stack *stack);
void push_to_stack(Stack *stack, int data);
int pop_from_stack(Stack *stack);
int Top(Stack *stack);
void display(Stack *stack);
void free_stack(Stack *stack);
#endif
