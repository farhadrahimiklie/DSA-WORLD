#include <stdio.h>
#include <stdlib.h>
#include "dll.h"


Node *CreateNode(int data){
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Dynamic Memory Allocation is Failed.\n");
        exit(1);
    }

    newNode->data = data;
    newNode->next = newNode->prev = NULL;
    return newNode;
}

void insert_at_start(Node **head, int data){
    Node *newNode = CreateNode(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    newNode->next = *head;
    (*head)->prev = newNode;
    *head = newNode;
    return;
}

void insert_at_end(Node **head, int data){
    Node *newNode = CreateNode(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    Node *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
    newNode->prev = temp;
    return;


}

void insert_at_index(Node **head, int index, int data){
    if (index == 0) {
        insert_at_start(head, data);
        return;
    }

    if (index < 0) {
        printf("Invalid Index\n");
        return;
    }

    Node *newNode = CreateNode(data);

    if (*head == NULL) {
        printf("Out of Range\n");
        free(newNode);
        return;
    }


    Node *temp = *head;
    for (int i = 0; i < index -1 && temp != NULL; i++) {
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Out of Range\n");
        free(newNode);
        return;
    }

    newNode->next = temp->next;
    if (temp->next != NULL) {
        temp->next->prev = newNode;
    }
    newNode->prev = temp;
    temp->next = newNode;
    return;
}

void delete_at_start(Node **head){
    if (*head == NULL) {
        printf("linked is empty\n");
        return;
    }

    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }


    Node *temp = *head;
    *head = temp->next;
    (*head)->prev = NULL;
    temp->next = NULL;
    free(temp);
    return;
}

void delete_at_end(Node **head){
    if (*head == NULL) {
        printf("linked list is empty\n");
        return;
    }

    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }


    Node *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    temp->prev->next = NULL;
    temp->prev = NULL;
    free(temp);
    return;
}

void delete_at_index(Node **head, int index){
    if (*head == NULL) {
        printf("linked list is empty\n");
        return;
    }

    if ((*head)->next == NULL) {
        if (index == 0) {
            free(*head);   
            *head = NULL;
        }else{
            printf("Out of Range\n");
        }
        return;
    }


    if (index < 0) {
        printf("Invalid index\n");
        return;
    }

    if (index == 0) {
        delete_at_start(head);
        return;
    }

    Node *temp = *head;
    for (int i = 0; i < index && temp != NULL; i++) {
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Out of Range\n");
        return;
    }

    if (temp->next != NULL) {
        temp->next->prev = temp->prev;
    }
    if (temp->prev != NULL) {
        temp->prev->next = temp->next;

    }

    free(temp);
    return;
}

int Search(Node *head, int key){
    if (head == NULL) {
        printf("list is empty\n");
        return -1;
    }
    Node *temp = head;
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

// goes forward and print all nodes
void display_forward(Node *head){
    if (head == NULL) {
        printf("linked list is empty\n");
        return;
    }
    Node *temp = head;
    printf("NULL <--> ");
    while (temp != NULL) {
        printf("%d <--> ",temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

void display_backward(Node *head){
    if (head == NULL) {
        printf("linked list is empty\n");
        return;
    }

    Node *temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    printf("NULL <--> ");
    while (temp != NULL) {
        printf("%d <--> ", temp->data);
        temp = temp->prev;
    }

    printf("NULL\n");
}

void free_doubly_linked_list(Node **head){
    Node *temp;
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }

    *head = NULL;
}

int main(){
    Node *head = NULL;

    insert_at_start(&head, 10);
    insert_at_start(&head, 20);
    insert_at_start(&head, 30);
    insert_at_start(&head, 40);
    insert_at_start(&head, 50);
    //insert_at_end(&head, 100);
    //insert_at_index(&head, 0, 100);


    //delete_at_start(&head);
    // delete_at_end(&head);
    //delete_at_index(&head, 5);


    //int result = Search(head, 0);
    //if (result != -1) {
    //printf("Node Found [%d]\n", result);
    //}else {
    //printf("Node Not Found\n");
    //}



    //display_forward(head);
    display_backward(head);
    free_doubly_linked_list(&head);
    return 0;
}
