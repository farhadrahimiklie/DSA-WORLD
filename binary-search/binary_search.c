#include <stdio.h>
#include "binary_search.h"

int Binary_Search_Iterative_Method(int array[], int size, int target){
    int start = 0, end = size -1;
    while (start <= end) {
        int middle = start + (end - start) / 2;

        if (array[middle] == target) {
            return middle;
        }else if (array[middle] < target) {
            start = middle + 1;
        }else {
            end = middle - 1;
        }
   }
    return -1;
}

int Binary_Search_Recursive_Method(int array[], int start, int end, int target){
    if (start > end) {
        return -1;
    }

    int middle = start + (end - start) / 2;
    if (array[middle] == target) {
        return middle;
    }else if (array[middle] < target) {
        return Binary_Search_Recursive_Method(array, middle +1, end, target);
    }else {
        return Binary_Search_Recursive_Method(array, start, middle -1, target);
    }
}
int main(){
    int array[] = {1, 2, 3, 4, 6, 7, 35, 91, 453, 1597};
    int size = sizeof(array) / sizeof(array[0]);
    int target = 1;
    //int result = Binary_Search_Iterative_Method(array, size, target);
    int result = Binary_Search_Recursive_Method(array, 0, size-1, target);
    if(result != -1){
        printf("Data Found in Index: %d \n", result);
    }else{
        printf("Data not Found ! \n");
    }
    return 0;
}
