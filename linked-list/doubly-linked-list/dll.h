#ifndef DLL_H
#define DLL_H

typedef struct Node{
    int data;
    struct Node *next;
    struct Node *prev;
}Node;

Node *CreateNode(int data);
void insert_at_start(Node **head, int data);
void insert_at_end(Node **head, int data);
void insert_at_index(Node **head, int index, int data);
void delete_at_start(Node **head);
void delete_at_end(Node **head);
void delete_at_index(Node **head, int index);
int Search(Node **head, int key);
void display_forward(Node *head);
void display_backward(Node *head);

#endif
