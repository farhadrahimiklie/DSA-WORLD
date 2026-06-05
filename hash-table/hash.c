#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 10
#define LOAD_FACTOR_THRESHOLD 0.7

// HashMap Entry
typedef struct {
    char key[50];
    int value;
    int isOccupied; // 0 = empty, 1 = occupied
} Entry;

// HashMap structure
typedef struct {
    Entry* table;  // dynamic array of entries
    int size;      // current table size
    int count;     // current number of elements
} HashMap;

// ---------------- HASH FUNCTION (djb2) ----------------
unsigned int hash(char *key, int size) {
    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
        hash = hash * 33 + c;

    return hash % size;
}

// ---------------- REHASH FUNCTION ----------------
void rehash(HashMap* map) {
    int oldSize = map->size;
    int newSize = oldSize * 2;

    Entry* newTable = calloc(newSize, sizeof(Entry));

    for (int i = 0; i < oldSize; i++) {
        if (map->table[i].isOccupied) {
            unsigned int index = hash(map->table[i].key, newSize);

            // Linear probing in new table
            while (newTable[index].isOccupied) {
                index = (index + 1) % newSize;
            }

            // Move entry
            strcpy(newTable[index].key, map->table[i].key);
            newTable[index].value = map->table[i].value;
            newTable[index].isOccupied = 1;
        }
    }

    free(map->table);
    map->table = newTable;
    map->size = newSize;

    printf("Rehashing done! New table size: %d\n", newSize);
}

// ---------------- CREATE HASHMAP ----------------
HashMap* createMap(int size) {
    HashMap* map = malloc(sizeof(HashMap));
    map->size = size;
    map->count = 0;
    map->table = calloc(size, sizeof(Entry));
    return map;
}

// ---------------- INSERT ----------------
void insert(HashMap* map, char* key, int value) {

    // Check load factor
    if ((float)map->count / map->size > LOAD_FACTOR_THRESHOLD) {
        rehash(map);
    }

    unsigned int index = hash(key, map->size);

    // Linear probing
    while (map->table[index].isOccupied) {
        // If key exists, update value
        if (strcmp(map->table[index].key, key) == 0) {
            map->table[index].value = value;
            return;
        }
        index = (index + 1) % map->size;
    }

    // Insert new entry
    strcpy(map->table[index].key, key);
    map->table[index].value = value;
    map->table[index].isOccupied = 1;
    map->count++;
}

// ---------------- SEARCH ----------------
int search(HashMap* map, char* key) {
    unsigned int index = hash(key, map->size);

    int startIndex = index; // prevent infinite loop

    while (map->table[index].isOccupied) {
        if (strcmp(map->table[index].key, key) == 0) {
            return map->table[index].value;
        }
        index = (index + 1) % map->size;

        if (index == startIndex) break; // full loop
    }

    return -1; // not found
}

// ---------------- MAIN ----------------
int main() {
    HashMap* map = createMap(INITIAL_SIZE);

    insert(map, "apple", 10);
    insert(map, "banana", 20);
    insert(map, "grape", 30);
    insert(map, "orange", 40);
    insert(map, "pear", 50);
    insert(map, "kiwi", 60);
    insert(map, "melon", 70); // should trigger rehash
    insert(map, "juwari", 80); // should trigger rehash
    insert(map, "lemu", 90); // should trigger rehash

    printf("apple: %d\n", search(map, "apple"));
    printf("banana: %d\n", search(map, "banana"));
    printf("grape: %d\n", search(map, "grape"));
    printf("pear: %d\n", search(map, "pear"));
    printf("kiwi: %d\n", search(map, "kiwi"));
    printf("melon: %d\n", search(map, "melon"));
    printf("juwari: %d\n", search(map, "juwari")); // not exists
    printf("lemu: %d\n", search(map, "lemu")); // not exists

    free(map->table);
    free(map);

    return 0;
}

