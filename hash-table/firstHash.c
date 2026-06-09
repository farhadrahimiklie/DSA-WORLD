#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
uint hash(char *key, int size){
    ulong hash = 5381;
    int c;

    while ((c = *key++)) {
        hash = hash * 33 + c;
    }

    return hash % size;
}

void rehash(HashTable *map){
    int oldSize = map->size;
    int newSize = oldSize * 2;

    Entry *newTable = calloc(newSize, sizeof(Entry));
    if (newTable == NULL) {
        printf("Memory Allocation is Failed. \n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < oldSize; i++) {
        if (map->bucket[i].isOccupied == OCCUPIED) {
            uint index = hash(map->bucket[i].key, newSize);

            while (newTable[index].isOccupied == OCCUPIED) {
               index = (index + 1) % newSize; 
            }

            strcpy(newTable[index].key, map->bucket[i].key);
            newTable[index].value = map->bucket[i].value;
            newTable[index].isOccupied = OCCUPIED;
        }
    }

    free(map->bucket);
    map->bucket = newTable;
    map->size = newSize;

    printf("Rehashing done! New Size = %d\n", newSize);
}

HashTable *create_hash_table(int size){
    HashTable *map = (HashTable*)malloc(sizeof(HashTable));
    if (map == NULL) {
        printf("Memory Allocation is Failed. \n");
        exit(EXIT_FAILURE);
    }

    map->size = size;
    map->count = 0;
    map->bucket = (Entry*)calloc(size, sizeof(Entry));
    if (map->bucket == NULL) {
        printf("Memory Allocation is Failed. \n");
        exit(EXIT_FAILURE);
    }

    return map;
}

void insert_entry_to_bucket(HashTable *map, char *key, int value){
    if ((float)map->count / map->size > LOAD_FACTOR_THRESHOLD) {
        rehash(map);
    }

    uint index = hash(key, map->size);
    int firstDeleted = -1;

    while (map->bucket[index].isOccupied != EMPTY) {
        if (map->bucket[index].isOccupied == OCCUPIED && strcmp(map->bucket[index].key, key) == 0) {
            map->bucket[index].value = value;
            return;
        }

        if (map->bucket[index].isOccupied == DELETED && firstDeleted == -1) {
            firstDeleted = index;
        }
        index = (index + 1) % map->size;
    }

    if (firstDeleted != -1) {
        index = firstDeleted;
    }

    strcpy(map->bucket[index].key, key);
    map->bucket[index].value = value;
    map->bucket[index].isOccupied = OCCUPIED;
    map->count++;
}

int search_from_bucket(HashTable *map, char *key){
    uint index = hash(key, map->size);
    uint startIndex = index;
    
    while (map->bucket[index].isOccupied != EMPTY) {
        if (map->bucket[index].isOccupied == OCCUPIED && strcmp(map->bucket[index].key, key) == 0) {
            return map->bucket[index].value;
        }

        index = (index + 1) % map->size;
        if (index == startIndex) {
            break;
        }

    }
    return -1;
}

int delete_from_bucket(HashTable *map, char *key){
    uint index = hash(key, map->size);
    uint startIndex = index;

    while (map->bucket[index].isOccupied != EMPTY) {
        if (map->bucket[index].isOccupied == OCCUPIED && strcmp(map->bucket[index].key, key) == 0) {
            map->bucket[index].isOccupied = DELETED;
            map->count--;
            return 1;
        }
        index = (index + 1) % map->size;
        if (index == startIndex) {
            break;
        }
    }
    return 0;
}

int update_from_bucket(HashTable *map, char *key, int newValue){
    uint index = hash(key, map->size);

    uint startIndex = index;

    while (map->bucket[index].isOccupied != EMPTY) {
        if (map->bucket[index].isOccupied == OCCUPIED && strcmp(map->bucket[index].key, key) == 0) {
            map->bucket[index].value = newValue;
            return 1;
        }
        index = (index + 1) % map->size;
        if (index == startIndex) {
            break;
        }
    }
    return 0;
}

int is_entry_in_the_bucket_exit(HashTable *map, char *key){
    uint index = hash(key, map->size);
    uint startIndex = index;

    while (map->bucket[index].isOccupied != EMPTY) {
        if (map->bucket[index].isOccupied == OCCUPIED && strcmp(map->bucket[index].key, key) == 0) {
            return 1;
        }
        index = (index + 1) % map->size;
        if (index == startIndex) {
            break;
        }
    }
    return 0;
}

int size(HashTable *map){
    return map->count;
}

int isEmpty(HashTable *map){
    return map->count == 0;
}

void clear(HashTable *map){
    for (int i = 0; i < map->size; i++) {
        map->bucket[i].isOccupied = EMPTY;
    }
    map->count = 0;
}

void destroyTable(HashTable *map){
    if (map == NULL) {
        return;
    }

    free(map->bucket);
    free(map);
}

int main(){
    HashTable *map = create_hash_table(INITIAL_SIZE);
    insert_entry_to_bucket(map, "apple", 1000);
    insert_entry_to_bucket(map, "banana", 3000);
    insert_entry_to_bucket(map, "lemu", 6000);
    insert_entry_to_bucket(map, "orange", 9000);
    insert_entry_to_bucket(map, "cheery", 5000);

    delete_from_bucket(map, "orange");
    printf("apple: %d\n", search_from_bucket(map, "orange"));
    return 0;
}
