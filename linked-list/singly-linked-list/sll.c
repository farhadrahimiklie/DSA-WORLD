#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node{
    int data;
    struct Node *next;
}Node;

Node *head = NULL;

Node *Create_Node(int data){
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory Allocation is Failed.\n");
        exit(1);
    }

    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void insert_at_begining(Node **head, int data){
    Node *newNode = Create_Node(data);
    if (newNode == NULL) {
        *head = newNode;
        return;
    }

    newNode->next = *head;
    *head = newNode;
    return;
}

void insert_at_end(Node **head, int data){
    Node *newNode = Create_Node(data);
    if (newNode == NULL) {
        *head = newNode;
        return;
    }

    Node *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
    return;
}

void insert_at_index(Node **head, int position, int data){
    if (position == 0) {
        insert_at_begining(head, data);
        return;
    }

    Node* newNode  = Create_Node(data);
    Node *temp = *head;
    for (int i = 0; i < position-1; i++) {
        temp = temp->next;
    }

    newNode->next = temp->next;
    temp->next = newNode;
    return;
}

void delete_at_begining(Node **head){
    if (*head == NULL) {
        printf("Linked List is Empty nothing to remove from here.\n");
        return;
    }
    Node *temp = *head;
    *head = (*head)->next;
    temp->next = NULL;
    free(temp);
    return;
}

void delete_at_end(Node **head){
    if (*head == NULL) {
        printf("Linked List is Empty nothing to remove from here.\n");
        return;
    }
    Node *temp = (*head)->next;
    Node *prev = *head;
    while (temp->next != NULL) {
        temp = temp->next;
        prev = prev->next;
    }
    prev->next = NULL;
    free(temp);
    return;
}

void delete_at_index(Node **head, int position){
    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }
    if (position == 0) {
        delete_at_begining(head);
        return;
    }

    Node *temp = (*head)->next;
    Node* q = *head;
    for (int i = 0; i < position-1; i++) {
        temp = temp->next;
        q = q->next;
    }
    q->next = temp->next;
    temp->next = NULL;
    free(temp);
}

void update_at_index(Node **head, int index, int new_element){
    if ((*head)->next == NULL) {
        (*head)->data = new_element;
        return;
    }
    Node *temp = *head;
    for (int i = 0; i < index; i++) {
        temp = temp->next;
    }
    temp->data = new_element;
    return;
}

int Search(Node **head, int key){
    Node *temp = *head;
    int index = 0;
    while (temp != NULL) {
        if (temp->data == key) {
            return index;
        }
        temp = temp->next;
        index++;
    }

    return -1;
}

void display(Node *head){
    Node *temp = head;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf(">> NULL \n");
}

int main(){
    insert_at_begining(&head, 10);
    insert_at_begining(&head, 20);
    insert_at_begining(&head, 30);
    insert_at_begining(&head, 40);
    insert_at_begining(&head, 50);
    //insert_at_end(&head, 600);
    //insert_at_index(&head, 2, 700);

    //delete_at_begining(&head);
    //delete_at_end(&head);
    //delete_at_index(&head, 4);

    //int result = Search(&head, 23);
    //if (result != -1) {
        //printf("Key is Found at index [%d]\n", result);
    //}else {
        //printf("Key is not Found at index [%d]\n", result);
    //}

   // update_at_index(&head,4, 100);

    display(head);
    return 0;
}
