#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    int *items;
    int size;
    int capacity;
}Vector;

void init_vector(Vector *vector);
void reallocate_vector(Vector *vector, int new_capacity);
void grow_capacity(Vector *vector);
void reserve(Vector *vector, int new_capacity);
void resize(Vector *vector, int new_size);
void push_back(Vector *vector, int data);
void pop_back(Vector *vector);
void set(Vector *vector, int index, int data);
int get(Vector *vector, int index);
int front(Vector *vector);
int back(Vector *vector);
int isEmpty(Vector *vector);
void delete_at_index(Vector *vector, int index);
void insert_at_index(Vector *vector, int index, int data);
void clear(Vector *vector);
void free_vector(Vector *vector);
void display(Vector *vector);
#endif
