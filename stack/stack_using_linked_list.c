#include <stdio.h>
#include <stdlib.h>
#include "stack_using_linked_list.h"


Node *CreateNode(int data){
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory Allocation is Failed.\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

int isEmpty(Node *top){
    if (top == NULL) {
        return 1;
    }else {
        return 0;
    }
}

int isFull(void){
    Node *newFullNode = (Node*)malloc(sizeof(Node));
    if (newFullNode == NULL) {
        return 1;
    }

    free(newFullNode);

    return 0;
}

void push_at_stack(Node **top, int data){
    if (isFull()) {
        printf("Stack is Full.\n");
        return;
    }

    Node *newNode = CreateNode(data);
    newNode->next = *top;
    *top = newNode;
    return;
}

int pop_from_stack(Node **top){
    if (isEmpty(*top)) {
        printf("Stack is Empty\n");
        return -1;
    }
    Node *temp = *top;
    *top = (*top)->next;
    int data = temp->data;
    free(temp);
    return data;
}

int Peek(Node *top){
    if (isEmpty(top)) {
        printf("stack is Empty\n");
        return -1;
    }

    return top->data;
}

void display(Node *top){
    if (isEmpty(top)) {
        printf("stack is Empty\n");
        return;
    }

    Node *temp = top;
    printf("top --> bottom \n");
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void free_stack_linked_list(Node **top){
    Node *current = *top;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }

    *top = NULL;
}

int main(){
    Node *top = NULL;
    push_at_stack(&top, 10);
    push_at_stack(&top, 20);
    push_at_stack(&top, 30);
    push_at_stack(&top, 40);
    push_at_stack(&top, 50);

    //int x = pop_from_stack(&top);
    //printf("remove element is %d \n", x);

    display(top);

    free_stack_linked_list(&top);
}
