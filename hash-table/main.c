#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;
 
#define INITIAL_SIZE 10
#define LOAD_FACTOR_THRESHOLD 0.7

typedef struct {
    char key[15];
    int value;
    int isOccupied; // 0 empty   1 occupied
}Entry;

typedef struct {
    Entry *table;
    int size; // current table size
    int count; // current number of elements
}HashTable;

uint hash(char *key, int size){
    unsigned long hash = 5381;
    int c;

    while (*key != '\0') {
        c = *key;
        hash = hash * 33 + c;
        key++;
    }

    return hash % size;
}

void rehash(HashTable *map){

}

HashTable *CreateMap(int size){
    HashTable *map = malloc(sizeof(HashTable));
    map->size = size;
    map->count = 0;
    map->table = calloc(size, sizeof(Entry));
    return map;
}

void insert(HashTable *map, char *key, int value){
    // check load factor
    if (map->count / map->size > LOAD_FACTOR_THRESHOLD) {
        rehash(map);
    }

    uint index = hash(key, map->size);

    while (map->table[index].isOccupied) {
        if (strcmp(map->table[index].key, key) == 0) {
            map->table[index].value = value;
            return;
        }

        index = (index + 1) % map->size;
    }

    strcpy(map->table[index].key, key);
    map->table[index].value = value;
    map->table[index].isOccupied = 1;
    map->count++;
}

int search(HashTable *map, char *key){
    uint index = hash(key, map->size);

    uint startIndex = index;
    while (map->table[index].isOccupied) {
        if (strcmp(map->table[index].key, key) ==0) {
            return map->table[index].value;
        }

        index = (index + 1) % map->size;
        if (index == startIndex) {
            break;
        }
    }
    return -1;
}

int main(){
    HashTable *map = CreateMap(INITIAL_SIZE); // INITIAL_SIZE is 10 by default.

    insert(map, "apple", 100);
    insert(map, "banana", 800);

   printf("apple: %d\n", search(map, "banana")); 
   free(map->table);
   free(map)
    return 0;
}
