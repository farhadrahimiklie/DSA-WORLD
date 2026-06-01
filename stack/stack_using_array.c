#include <stdio.h>
#include <stdlib.h>
#include "stack_using_array.h"

void init_stack(Stack *stack){
    stack->size = 5;
    stack->top = -1;
    stack->storage = (int*)malloc(stack->size * sizeof(int));
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
        printf("Stack is Full...\n");
        return;
    }

    stack->top++;
    stack->storage[stack->top] = data;
    return;
}

void pop_from_stack(Stack *stack){
    if (isEmpty(stack)) {
        printf("Stack is Empty...\n");
        return;
    }

    //int x = stack->storage[stack->top];
    stack->top--;
    return;
}

int Top(Stack *stack){
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

int main(){
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    init_stack(stack);
    push_to_stack(stack, 10);
    push_to_stack(stack, 20);
    push_to_stack(stack, 30);
    push_to_stack(stack, 40);
    push_to_stack(stack, 50);

    //pop_from_stack(stack);

    //int top = Top(stack);
    //printf("top of stack is %d \n", top);


    display(stack);






    //printf("removed element is %d \n", x);
    return 0;
}
