#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

/* =========================
   Initialize Vector
========================= */
void init_vector(Vector *vector) {

    vector->size = 0;
    vector->capacity = 4;

    vector->items = (int *)malloc(
        vector->capacity * sizeof(int)
    );

    if (vector->items == NULL) {
        printf("Memory Allocation Failed\n");
        exit(1);
    }
}

/* =========================
   Internal Reallocation
========================= */
void reallocate(Vector *vector, int new_capacity) {

    int *temp = realloc(vector->items, new_capacity * sizeof(int));

    if (temp == NULL) {
        printf("Reallocation Failed\n");
        exit(1);
    }

    vector->items = temp;
    vector->capacity = new_capacity;
}

/* =========================
   Auto Grow Capacity
========================= */
void grow_capacity(Vector *vector) {

    int new_capacity = vector->capacity * 2;

    reallocate(vector, new_capacity);
}

/* =========================
   Reserve Capacity
========================= */
void reserve(Vector *vector, int new_capacity) {

    /*
       reserve never changes size
       only capacity
    */

    if (new_capacity <= vector->capacity) {
        return;
    }

    reallocate(vector, new_capacity);
}

/* =========================
   Resize Vector
========================= */
void resize(Vector *vector, int new_size) {

    /*
       resize changes actual size

       if new size bigger:
       add 0 as default value

       if new size smaller:
       remove elements logically
    */

    if (new_size < 0) {
        printf("Invalid Size\n");
        return;
    }

    /* increase capacity if needed */
    if (new_size > vector->capacity) {

        int new_capacity = vector->capacity;

        while (new_capacity < new_size) {
            new_capacity *= 2;
        }

        reallocate(vector, new_capacity);
    }

    /* initialize new elements with 0 */
    if (new_size > vector->size) {

        for (int i = vector->size;
             i < new_size;
             i++) {

            vector->items[i] = 0;
        }
    }

    vector->size = new_size;
}

/* =========================
   Push Back
========================= */
void push_back(Vector *vector, int data) {

    if (vector->size >= vector->capacity) {
        grow_capacity(vector);
    }

    vector->items[vector->size] = data;
    vector->size++;
}

/* =========================
   Pop Back
========================= */
void pop_back(Vector *vector) {

    if (vector->size == 0) {
        printf("Vector is Empty\n");
        return;
    }

    vector->size--;
}

/* =========================
   Set
========================= */
void set(Vector *vector,
         int index,
         int value) {

    if (index < 0 ||
        index >= vector->size) {

        printf("Invalid Index\n");
        return;
    }

    vector->items[index] = value;
}

/* =========================
   Get
========================= */
int get(Vector *vector, int index) {

    if (index < 0 ||
        index >= vector->size) {

        printf("Invalid Index\n");
        return -1;
    }

    return vector->items[index];
}

/* =========================
   Front
========================= */
int front(Vector *vector) {

    if (vector->size == 0) {
        printf("Vector is Empty\n");
        return -1;
    }

    return vector->items[0];
}

/* =========================
   Back
========================= */
int back(Vector *vector) {

    if (vector->size == 0) {
        printf("Vector is Empty\n");
        return -1;
    }

    return vector->items[
        vector->size - 1
    ];
}

/* =========================
   Is Empty
========================= */
int isEmpty(Vector *vector) {
    return vector->size == 0;
}

/* =========================
   Delete At Index
========================= */
void delete_at_index(Vector *vector,
                     int index) {

    if (index < 0 ||
        index >= vector->size) {

        printf("Invalid Index\n");
        return;
    }

    for (int i = index;
         i < vector->size - 1;
         i++) {

        vector->items[i] =
        vector->items[i + 1];
    }

    vector->size--;
}

/* =========================
   Insert At Index
========================= */
void insert_at_index(Vector *vector,
                     int index,
                     int new_element) {

    if (index < 0 ||
        index > vector->size) {

        printf("Invalid Index\n");
        return;
    }

    if (vector->size >= vector->capacity) {
        grow_capacity(vector);
    }

    for (int i = vector->size - 1;
         i >= index;
         i--) {

        vector->items[i + 1] =
        vector->items[i];
    }

    vector->items[index] =
    new_element;

    vector->size++;
}

/* =========================
   Clear Vector
========================= */
void clear(Vector *vector) {

    /*
       remove all elements
       but keep capacity
    */

    vector->size = 0;
}

/* =========================
   Display
========================= */
void display(Vector *vector) {

    if (vector->size == 0) {
        printf("Vector is Empty\n");
        return;
    }

    printf("[ ");

    for (int i = 0;
         i < vector->size;
         i++) {

        printf("%d ",
               vector->items[i]);
    }

    printf("]\n");
}

/* =========================
   Free Memory
========================= */
void free_vector(Vector *vector) {

    free(vector->items);

    vector->items = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

/* =========================
   Main
========================= */
int main() {

    Vector vector;

    init_vector(&vector);

    push_back(&vector, 10);
    push_back(&vector, 20);
    push_back(&vector, 30);

    printf("Initial Vector:\n");
    display(&vector);

    printf("Size: %d\n", vector.size);
    printf("Capacity: %d\n\n",
           vector.capacity);

    /* reserve */
    reserve(&vector, 100);

    printf("After reserve(100):\n");

    display(&vector);

    printf("Size: %d\n", vector.size);
    printf("Capacity: %d\n\n",
           vector.capacity);

    /* resize bigger */
    resize(&vector, 6);

    printf("After resize(6):\n");

    display(&vector);

    printf("Size: %d\n", vector.size);
    printf("Capacity: %d\n\n",
           vector.capacity);

    /* resize smaller */
    resize(&vector, 2);

    printf("After resize(2):\n");

    display(&vector);

    printf("Size: %d\n", vector.size);
    printf("Capacity: %d\n\n",
           vector.capacity);

    clear(&vector);

    printf("After clear():\n");

    display(&vector);

    printf("Size: %d\n", vector.size);
    printf("Capacity: %d\n",
           vector.capacity);

    free_vector(&vector);

    return 0;
}
