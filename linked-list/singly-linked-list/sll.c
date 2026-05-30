#include <stdio.h>
#include <stdlib.h>
#include "sll.h"

// create a head pointer to point to the first node of linked list
Node *head = NULL;

// create node is a function to make a node with value and next null pointer.
Node *CreateNode(int data){
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Dynamic Memory Allocation is Failed.\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// insert at the start of the linked list
void insert_at_start(Node **head, int data){
    Node *newNode = CreateNode(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    newNode->next = *head;
    *head = newNode;
    return;
}

// insert at the end of linked list
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
    return;
}

// insert at the index of the linked list (where we want)
void insert_at_index(Node **head, int index, int data){
    Node *newNode = CreateNode(data);

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    if (index < 0) {
        printf("Invalid Index\n");
        return;
    }

    if (index == 0) {
        insert_at_start(head, data);
        return;
    }

    Node *temp = *head;

    for (int i = 0; i < index-1 && temp != NULL; i++) {
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Out of range\n");
        return;
    }
    newNode->next = temp->next;
    temp->next = newNode;
    return;

}

// delete from start of the linked list
void delete_at_start(Node **head){
    if (*head == NULL) {
        printf("linked list is empty nothing to delete.\n");
        return;
    }
    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }

    Node *temp = *head;
    *head = temp->next;
    temp->next = NULL;
    free(temp);
}

// delete from end fo the linked list
void delete_at_end(Node **head){
    if (*head == NULL) {
        printf("linked list is empty nothing to delete.\n");
        return;
    }
    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
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
}

// delete node using index (where we want can delete the node from linked list)
void delete_at_index(Node **head, int index){
    if (*head == NULL) {
        printf("list is empty\n");
        return;
    }

    if (index < 0) {
        printf("Invalid index\n");
        return;
    }

    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }

    if (index == 0) {
        delete_at_start(head);
        return;
    }

    Node *temp = (*head)->next;
    Node *prev = *head;
    for (int i = 0; i < index -1 && temp != NULL; i++) {
        temp = temp->next;
        prev = prev->next;
    }

    if (temp == NULL) {
        printf("Out of Range\n");
        return;
    }
    prev->next = temp->next;
    temp->next = NULL;
    free(temp);
    return;

}

// Search a node we want from anywhere of the list
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

// update the existing node with new node inside linked list
void update_node(Node **head, int index, int new_data){
    if (index < 0) {
        printf("Invalid index\n");
        return;
    }

    if ((*head)->next == NULL) {
        (*head)->data = new_data;
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

    temp->data = new_data;
return;
}

// display the created linked list nodes one by one's
void display(Node *head){
    if (head == NULL) {
        printf("linked list is empty.\n");
        return;
    }

    Node *temp = head;

    printf("[");
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("]");
    printf("\n");
}

int main(){
    insert_at_start(&head, 10);
    insert_at_start(&head, 20);
    insert_at_start(&head, 30);
    insert_at_start(&head, 40);
    insert_at_start(&head, 50);
    //insert_at_end(&head, 60);
    //insert_at_index(&head, 2, 100);


    //delete_at_start(&head);
    //delete_at_end(&head);
    //delete_at_index(&head, 5);

    //int result = Search(&head, 70);
    //if (result != -1) {
        //printf("Node Found index[%d]\n", result);
    //}else {
        //printf("Node not Found\n");
    //}

    //update_node(&head, -1, 100);

    display(head);
    return 0;
}
