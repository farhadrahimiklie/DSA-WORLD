#ifndef SLL_H
#define SLL_H

// create a node structure
typedef struct Node{
    int data;
    struct Node *next;
}Node;

Node *CreateNode(int data);
void insert_at_start(Node **head, int data);
void insert_at_end(Node **head, int data);
void insert_at_index(Node **head, int index, int data);
void delete_at_start(Node **head);
void delete_at_end(Node **head);
void delete_at_index(Node **head, int index);
int Search(Node **head, int key);
void update_node(Node **head, int index, int new_data);
void display(Node *head);

#endif
