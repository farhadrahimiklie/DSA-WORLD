#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *storage;
    int top;
    int size;
}Stack;

void init_stack(Stack *stack){
    stack->size = 5;
    stack->top = -1;
    stack->storage = (int*)malloc(stack->size * sizeof(int));
    return;
}

int isEmpty(Stack *ptr){
    if (ptr->top == -1) {
        return 1;
    }else {
        return 0;
    }
}

int isFull(Stack *ptr){
    if (ptr->top == ptr->size-1) {
        return 1;
    }else {
        return 0;
    }
}

void Push_in_stack(Stack *ptr, int data){
    if (isFull(ptr)) {
        printf("Stack is Full\n");
        return;
    }

    ptr->top++;
    ptr->storage[ptr->top] = data;
    return;
}

int Pop_from_stack(Stack *ptr, int data){
    if (isEmpty(ptr)) {
        printf("Stack is Empty Now\n");
        return -1;
    }

    //int x = ptr->storage[ptr->top];
    //ptr->top--;
    //return x;
}

int Top(Stack *ptr){
    return ptr->storage[ptr->top];
}

void display(Stack *ptr){
    if (isEmpty(ptr)) {
        printf("Stack is emtpy\n");
        return;
    }

    for (int i = ptr->top; i >= 0; i--) {
        printf("%d ", ptr->storage[i]);
    }
    printf("\n");
    
}

int main(){
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    init_stack(stack);
    Push_in_stack(stack, 10);
    Push_in_stack(stack, 20);
    Push_in_stack(stack, 30);
    Push_in_stack(stack, 40);
    Push_in_stack(stack, 50);
    display(stack);
    return 0;
}
