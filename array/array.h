#ifndef ARRAY_H
#define ARRAY_H

void Traverse_Array_Elements(int array[], int size);
void Insert_Element_in_Array(int array[], int* size, int capacity, int new_element, int index);
void Update_Element_inside_Array(int array[], int size, int new_element, int index);
int Search_Element_inside_Array(int array[], int size, int key);
void Delete_Element_inside_Array(int array[], int* size, int index);
void Reverse_Array_Element(int array[], int size);

#endif
