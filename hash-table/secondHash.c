#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "secondHash.h"

// ========== first Hash
uint first_hash(char *key, int capacity){
    ulong hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = hash * 33 + c;
    }

    return hash % capacity;
}

uint second_hash(char *key, int capacity){
    ulong hash = 0;
    int c;
    while ((c = *key++)) {
        hash = hash * 31 + c;
    }

    return (hash % (capacity -1)) + 1;
}

uint which_type_of_probe_should_return(HashTable *map, char *key, int i){
    uint index = first_hash(key, map->capacity);

    // LINEAR Probing
    if (map->probing == LINEAR) {
        return (index + i) % map->capacity;
    }

    // QUADRATIC Probing
    if (map->probing == QUADRATIC) {
        return (index + i * i) % map->capacity;
    }

    // DOUBLE_HASH Probing
    return (index + i * second_hash(key, map->capacity)) % map->capacity;
}

HashTable *create_hash_bucket(int capacity, ProbingTypes probing, CollisionMode mode){
    HashTable *init_hash = (HashTable*)malloc(sizeof(HashTable));
    if (!init_hash) {
        printf("Memory Allocation is Failed.\n");
        exit(EXIT_FAILURE);
    }

    init_hash->capacity = capacity;
    init_hash->count = 0;
    init_hash->probing = probing;
    init_hash->mode = mode;

    memset(&init_hash->statistics, 0, sizeof(Statistics));

    if (mode == OPEN_ADDRESSING) {
        init_hash->bucket = (Entry*)calloc(capacity, sizeof(Entry));
        if (init_hash->bucket == NULL) {
            printf("Memory Allocation is Failed\n");
            exit(EXIT_FAILURE);
        }
        init_hash->chainbucket = NULL;
    }else {
        init_hash->bucket = NULL;
        init_hash->chainbucket = calloc(capacity, sizeof(Node));
        if (init_hash->chainbucket == NULL) {
            printf("Memory Allocation is Failed\n");
            exit(EXIT_FAILURE);
        }
    }

    return init_hash;
}

// ========== OPEN_ADDRESSING (insert, search, delete, update)
void insert_to_open_addressing(HashTable *map, char *key, int value){
    for (int i = 0; i < map->capacity; i++) {
        uint index = which_type_of_probe_should_return(map, key, i);
        map->statistics.probing++;

        if (map->bucket[index].status == OCCUPIED && strcmp(map->bucket[index].key, key) == 0) {
            map->bucket[index].value = value;
            map->statistics.updates++;
            return;
        }

        if (map->bucket[index].status != OCCUPIED) {
            strncpy(map->bucket[index].key, key, KEY_CAPACITY -1);
            map->bucket[index].value = value;
            map->bucket[index].status = OCCUPIED;

            map->count++;
            map->statistics.inserts++;
            return;
        }
    }
}

int search_from_open_addressing(HashTable *map, char *key){
    for (int i = 0; i < map->capacity; i++) {
        uint index = which_type_of_probe_should_return(map, key, i);
        map->statistics.probing++;

        if (map->bucket[index].status == EMPTY) {
            return -1;
        }
        if (map->bucket[index].status == OCCUPIED && strcmp(map->bucket[index].key, key) == 0) {
            map->statistics.searches++;
            return map->bucket[index].value;
        }
    }
    return -1;
}

int delete_from_open_addressing(HashTable *map, char *key){
    for (int i = 0; i < map->capacity; i++) {
        uint index = which_type_of_probe_should_return(map, key, i);
        map->statistics.probing++;

        if (map->bucket[index].status == EMPTY) {
            return 0;
        }

        if (map->bucket[index].status == OCCUPIED && strcmp(map->bucket[index].key, key) == 0) {
            map->bucket[index].status = DELETED;
            map->statistics.deletes++;
            return 1;
        }
    }
    return 0;
}

int update_from_open_addressing(HashTable *map, char *key, int value){
    for (int i = 0; i < map->capacity; i++) {
        uint index = which_type_of_probe_should_return(map, key, i);
        map->statistics.probing++;

        if (map->bucket[index].status == EMPTY) {
            return 0;
        }

        if (map->bucket[index].status == OCCUPIED && strcmp(map->bucket[index].key, key) == 0) {
            map->bucket[index].value = value;
            map->statistics.updates++;
            return 1;
        }
    }
    return 0;
}

// ========== SEPARATE_CHAINING (insert, search, delete, update)
void insert_to_separate_chaining(HashTable *map, char *key, int value){
    uint index = first_hash(key, map->capacity);
    Node *head = map->chainbucket[index]; // first time is EMPTY

    while (head) { // if head exist means one node or more exist then go inside the while loop
        map->statistics.probing++;

        if (strcmp(head->key, key) == 0) {
            head->value = value;
            map->statistics.updates++;
            return;
        }

        map->statistics.collisions++;
        head = head->next;
    }

    Node *newNode = malloc(sizeof(Node));
    strcpy(newNode->key, key);
    newNode->value = value;
    newNode->next = map->chainbucket[index];

    map->chainbucket[index] = newNode;

    map->statistics.inserts++;
    map->count++;
}

int search_from_separate_chaining(HashTable *map, char *key){
    uint index = first_hash(key, map->capacity);

    Node *head = map->chainbucket[index];

    while (head) {
        map->statistics.probing++;

        if (strcmp(head->key, key) == 0) {
            map->statistics.searches++;
            return head->value;
        }
        head = head->next;
    }
    return -1;
}

int delete_from_separate_chaining(HashTable *map, char *key){
    uint index = first_hash(key, map->capacity);
    Node *head = map->chainbucket[index];
    Node *prev = NULL;

    while (head) {
        map->statistics.probing++;
        if (strcmp(head->key, key) == 0) {
            if (prev) {
                prev->next = head->next;
            }else {
                map->chainbucket[index] = head->next;
            }
            free(head);
            map->statistics.deletes++;
            map->count--;
            return 1;
        }
        prev = head;
        head = head->next;
    }
    return 0;
}

int update_from_separate_chaining(HashTable *map, char *key, int value){
    uint index = first_hash(key, map->capacity);
    Node *head = map->chainbucket[index];

    while (head) {
        map->statistics.probing++;

        if (strcmp(head->key, key) == 0) {
            head->value = value;
            map->statistics.updates++;
            return 1;
        }
        head = head->next;
    }
    return 0;
}

// ========== DISPATCHER
void insert(HashTable *map, char *key, int value){
    (map->mode == OPEN_ADDRESSING) ? insert_to_open_addressing(map, key, value) : insert_to_separate_chaining(map, key, value);
}

int search(HashTable *map, char *key){
    return (map->mode == OPEN_ADDRESSING) ? search_from_open_addressing(map, key) : search_from_separate_chaining(map, key);
}

int delete(HashTable *map, char *key){
    return (map->mode == OPEN_ADDRESSING) ? delete_from_open_addressing(map, key) : delete_from_separate_chaining(map, key);
}

int update(HashTable *map, char *key, int value){
    return (map->mode == OPEN_ADDRESSING) ? update_from_open_addressing(map, key, value) : update_from_separate_chaining(map, key, value);
}

// ========== if any key exist or not this functio check it.
int exists(HashTable *map, char *key){
    return search(map, key) != -1;
}

// ========== return us the capacity of the Hashbucket
int size(HashTable *map){
    return map->count;
}

// ========== check the bucket has empty or not
int isEmpty(HashTable *map){
    return map->count ==0;
}

// ========== clear the Memory
void clear(HashTable *map){
    map->count = 0;

    if (map->mode == OPEN_ADDRESSING) {
        for (int i = 0; i < map->capacity; i++) {
            map->bucket[i].status = EMPTY;
        }
    }else {
        for (int i = 0; i < map->capacity; i++) {
            Node *head = map->chainbucket[i];

            while (head) {
            Node *temp = head;
            head = head->next;
            free(temp);
            }
            map->chainbucket[i] = NULL;
        }
    }
}

// ========== Destroy the full memory of the Hashbucket
void destroy(HashTable *map){
    clear(map);

    if (map->mode == OPEN_ADDRESSING) {
        free(map->bucket);
    }else{
        free(map->chainbucket);
    }
    free(map);
}

// ========== print all Statistics data
void print_Statistics(HashTable *map){
    printf("\n --- ANALYTICS ---\n");
    printf("Inserts : %ld\n", map->statistics.inserts);
    printf("Searches : %ld\n", map->statistics.searches);
    printf("Updates : %ld\n", map->statistics.updates);
    printf("Deletes : %ld\n", map->statistics.deletes);
    printf("Collisions : %ld\n", map->statistics.collisions);
    printf("Probing : %ld\n", map->statistics.probing);
}

int main(){
    printf(" ===== OPEN_ADDRESSING ===== \n");
    HashTable *map1 = create_hash_bucket(INITIAL_CAPACITY, DOUBLE_HASH, OPEN_ADDRESSING);
    insert(map1, "apple", 100);
    insert(map1, "banana", 200);
    insert(map1, "orange", 300);

    printf("banana = %d\n", search(map1, "banana"));

    update(map1, "banana", 500);
    printf("banana updated = %d\n", search(map1, "banana"));

    delete(map1, "banana");
    printf("banana after delete = %d\n", search(map1, "banana"));

    print_Statistics(map1);
    destroy(map1);

    printf("\n ===== SEPARATE_CHAINING =====\n");

    HashTable *map2 = create_hash_bucket(INITIAL_CAPACITY, LINEAR, SEPARATE_CHAINING);
    insert(map2, "A", 1);
    insert(map2, "B", 2);
    insert(map2, "C", 3);

    printf("B = %d\n", search(map2, "B"));

    update(map2, "B", 777);
    printf("B updated = %d\n", search(map2, "B"));

    delete(map2, "B");
    printf("B after deleted = %d\n", search(map2, "B"));

    print_Statistics(map2);
    destroy(map2);
    return 0;
}
