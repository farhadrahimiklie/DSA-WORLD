#include <stdio.h>
#include <stdlib.h>
#include "stack_using_array.h"

void init_stack(Stack *stack){
    stack->size = 5;
    stack->top = -1;
    stack->storage = (int*)malloc(stack->size * sizeof(int));
    if (stack->storage == NULL) {
        printf("Memory Allocation Failed.\n");
        exit(EXIT_FAILURE);
    }
    return;
}

int isEmpty(Stack *stack){
    if (stack->top == -1) {
        return 1;
    }else {
        return 0;
    }
}

int isFull(Stack *stack){
    if (stack->top == stack->size-1) {
        return 1;
    }else {
        return 0;
    }
}

void push_to_stack(Stack *stack, int data){
    if (isFull(stack)) {
        int newSize = stack->size * 2;
        int *temp = realloc(stack->storage, newSize * sizeof(int));
        if (temp == NULL) {
            printf("Reallocation is Failed.\n");
            return;
        }

        stack->storage = temp;
        stack->size = newSize;
    }

    stack->top++;
    stack->storage[stack->top] = data;
    return;
}

int pop_from_stack(Stack *stack){
    if (isEmpty(stack)) {
        printf("Stack is Empty...\n");
        return -1;
    }

    int data = stack->storage[stack->top];
    stack->top--;

    return data;
}

int Top(Stack *stack){
    if (isEmpty(stack)) {
        printf("stack is Empty\n");
        return -1;
    }
    return stack->storage[stack->top];
}

void display(Stack *stack){
    if (isEmpty(stack)) {
        printf("Stack is Empty...\n");
        return;
    }

    printf("listing data...\n");
    for (int i = stack->top; i >= 0; i--) {
        printf("%d ", stack->storage[i]);
    }
    printf("\n");
}

void free_stack(Stack *stack){
    if (stack == NULL) {
        return;
    }

    free(stack->storage);
    stack->storage = NULL;

    free(stack);
    return;
}

int main(){
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        printf("Memory Allocation Failed\n");
        return 1;
    }
    init_stack(stack);
    push_to_stack(stack, 10);
    push_to_stack(stack, 20);
    push_to_stack(stack, 30);
    push_to_stack(stack, 40);
    push_to_stack(stack, 50);

    int x = pop_from_stack(stack);
    printf("removed element = %d\n", x);

    //int top = Top(stack);
    //printf("top of stack is %d \n", top);


    display(stack);
    free_stack(stack);
    return 0;
}
