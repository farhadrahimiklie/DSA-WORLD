#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int data;
    struct Node *prev;
    struct Node *next;
}Node;

Node *head = NULL;

Node *CreateNode(int data){
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory Allocation is Failed.\n");
        exit(1);
    }
    newNode->data = data;
    newNode->prev = newNode->next = NULL;
    return newNode;
}

void Insert_at_begining(Node **head, int data){
    Node *newNode = CreateNode(data);
    if (*head == NULL) {
        *head = newNode;
        newNode = *head;
        return;
    }
    Node *temp = *head;
    newNode->next = *head;
    temp->prev = newNode;
    *head = newNode;
    return;
}

void display(Node *head){
    Node *temp = head;
    
    printf("NULL<<");
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf(">>NULL");
    printf("\n");
}

int main(){
    Insert_at_begining(&head, 10);
    Insert_at_begining(&head, 20);
    Insert_at_begining(&head, 30);
    Insert_at_begining(&head, 40);
    Insert_at_begining(&head, 50);
    display(head);
    return 0;
}
