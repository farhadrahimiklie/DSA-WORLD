#include <stdio.h>
#include "array.h"

void Traverse_Array_Elements(int array[], int size){
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void Insert_Element_in_Array(int array[], int* size, int capacity, int new_element, int index){
    if (*size >= capacity) {
        printf("Array Full\n");
        return;
    }

    if (index < 0 || index > *size) {
        printf("Invalid Index\n");
        return;
    }

    for (int i = *size -1; i >= index; i--) {
        array[i + 1] = array[i];
    }

    array[index] = new_element;
    (*size)++;
}

void Update_Element_inside_Array(int array[], int size, int new_element, int index){
    if (index < 0 || index >= size) {
        printf("Invalid Index\n");
        return;
    }
    array[index] = new_element;
}

int Search_Element_inside_Array(int array[], int size, int key){
    for (int i = 0; i < size; i++) {
        if (array[i] == key) {
            return i;
        }
    }
    return -1;
}

void Delete_Element_inside_Array(int array[], int *size, int index){
    if (index < 0 || index >= *size) {
        printf("Invalid Index\n");
        return;
    }

    for (int i = index; i < *size -1; i++) {
        array[i] = array[i + 1];
    }
    (*size)--;
}

void Reverse_Array_Element(int array[], int size){
    int start = 0, end = size -1;

    while (start < end) {
        int temp = array[start];
        array[start] = array[end];
        array[end] = temp;

        start++;
        end--;
    }
}

int main() {
    int array[100] = {1,2,3,4,5};
    int size = 5;
    int capacity = 100;
    Insert_Element_in_Array(array, &size, capacity, 10, 2);
    Update_Element_inside_Array(array, size, 99, 1);
    Delete_Element_inside_Array(array, &size, 3);
    Reverse_Array_Element(array, size);
    int result = Search_Element_inside_Array(array, size, 99);
    if (result != -1) {
        printf("Value Found at Index: %d\n", result);
    }else{
        printf("Value Not Found\n");
    }

    Traverse_Array_Elements(array, size);

    return 0;
}
