#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ========== type alias
typedef unsigned int uint;
typedef unsigned long ulong;

// ========== some macros
#define INITIAL_CAPACITY 8
#define LOAD_FACTOR_BALANCE_THRESHOLD 0.7
#define KEY_CAPACITY 64

// ========== Enums
typedef enum { LINEAR, QUADRATIC, DOUBLE_HASH } ProbingTypes;
typedef enum { OPEN_ADDRESSING, SEPARATE_CHAINING } CollisionMode;
typedef enum { EMPTY, OCCUPIED, DELETED } State;

// ========== Entry (OPEN_ADDRESSING)
typedef struct {
    char key[KEY_CAPACITY];
    int value;
    State state;
}Entry;

// ========== Node (SEPARATE_CHAINING)
typedef struct Node{
    char key[KEY_CAPACITY];
    int value;
    struct Node *next;
}Node;

// ========== Analytics structure
typedef struct {
    long inserts, updates, deletes, searches;
    long collisions, probing;
}Analytics;

// ========== Hash Table Structure
typedef struct {
    Entry *table;
    Node **chainTable;

    int capacity;
    int count;

    ProbingTypes probing;
    CollisionMode mode;

    Analytics analyse;
}HashTable;

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

HashTable *create_hash_table(int capacity, ProbingTypes probing, CollisionMode mode){
    HashTable *init_hash = (HashTable*)malloc(sizeof(HashTable));
    if (!init_hash) {
        printf("Memory Allocation is Failed.\n");
        exit(EXIT_FAILURE);
    }

    init_hash->capacity = capacity;
    init_hash->count = 0;
    init_hash->probing = probing;
    init_hash->mode = mode;

    memset(&init_hash->analyse, 0, sizeof(Analytics));

    if (mode == OPEN_ADDRESSING) {
        init_hash->table = (Entry*)calloc(capacity, sizeof(Entry));
        if (init_hash->table == NULL) {
            printf("Memory Allocation is Failed\n");
            exit(EXIT_FAILURE);
        }
        init_hash->chainTable = NULL;
    }else {
        init_hash->table = NULL;
        init_hash->chainTable = calloc(capacity, sizeof(Node));
        if (init_hash->chainTable == NULL) {
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
        map->analyse.probing++;

        if (map->table[index].state == OCCUPIED && strcmp(map->table[index].key, key) == 0) {
            map->table[index].value = value;
            map->analyse.updates++;
            return;
        }

        if (map->table[index].state != OCCUPIED) {
            strncpy(map->table[index].key, key, KEY_CAPACITY -1);
            map->table[index].value = value;
            map->table[index].state = OCCUPIED;

            map->count++;
            map->analyse.inserts++;
            return;
        }
    }
}

int search_from_open_addressing(HashTable *map, char *key){
    for (int i = 0; i < map->capacity; i++) {
        uint index = which_type_of_probe_should_return(map, key, i);
        map->analyse.probing++;

        if (map->table[index].state == EMPTY) {
            return -1;
        }
        if (map->table[index].state == OCCUPIED && strcmp(map->table[index].key, key) == 0) {
            map->analyse.searches++;
            return map->table[index].value;
        }
    }
    return -1;
}

int delete_from_open_addressing(HashTable *map, char *key){
    for (int i = 0; i < map->capacity; i++) {
        uint index = which_type_of_probe_should_return(map, key, i);
        map->analyse.probing++;

        if (map->table[index].state == EMPTY) {
            return 0;
        }

        if (map->table[index].state == OCCUPIED && strcmp(map->table[index].key, key) == 0) {
            map->table[index].state = DELETED;
            map->analyse.deletes++;
            return 1;
        }
    }
    return 0;
}

int update_from_open_addressing(HashTable *map, char *key, int value){
    for (int i = 0; i < map->capacity; i++) {
        uint index = which_type_of_probe_should_return(map, key, i);
        map->analyse.probing++;

        if (map->table[index].state == EMPTY) {
            return 0;
        }

        if (map->table[index].state == OCCUPIED && strcmp(map->table[index].key, key) == 0) {
            map->table[index].value = value;
            map->analyse.updates++;
            return 1;
        }
    }
    return 0;
}

// ========== SEPARATE_CHAINING (insert, search, delete, update)
void insert_to_separate_chaining(HashTable *map, char *key, int value){
    uint index = first_hash(key, map->capacity);
    Node *head = map->chainTable[index]; // first time is EMPTY

    while (head) { // if head exist means one node or more exist then go inside the while loop
        map->analyse.probing++;

        if (strcmp(head->key, key) == 0) {
            head->value = value;
            map->analyse.updates++;
            return;
        }

        map->analyse.collisions++;
        head = head->next;
    }

    Node *newNode = malloc(sizeof(Node));
    strcpy(newNode->key, key);
    newNode->value = value;
    newNode->next = map->chainTable[index];

    map->chainTable[index] = newNode;

    map->analyse.inserts++;
    map->count++;
}

int search_from_separate_chaining(HashTable *map, char *key){
    uint index = first_hash(key, map->capacity);

    Node *head = map->chainTable[index];

    while (head) {
        map->analyse.probing++;

        if (strcmp(head->key, key) == 0) {
            map->analyse.searches++;
            return head->value;
        }
        head = head->next;
    }
    return -1;
}

int delete_from_separate_chaining(HashTable *map, char *key){
    uint index = first_hash(key, map->capacity);
    Node *head = map->chainTable[index];
    Node *prev = NULL;

    while (head) {
        map->analyse.probing++;
        if (strcmp(head->key, key) == 0) {
            if (prev) {
                prev->next = head->next;
            }else {
                map->chainTable[index] = head->next;
            }
            free(head);
            map->analyse.deletes++;
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
    Node *head = map->chainTable[index];

    while (head) {
        map->analyse.probing++;

        if (strcmp(head->key, key) == 0) {
            head->value = value;
            map->analyse.updates++;
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

// ========== return us the capacity of the HashTable
int size(HashTable *map){
    return map->count;
}

// ========== check the Table has empty or not
int isEmpty(HashTable *map){
    return map->count ==0;
}

// ========== clear the Memory
void clear(HashTable *map){
    map->count = 0;

    if (map->mode == OPEN_ADDRESSING) {
        for (int i = 0; i < map->capacity; i++) {
            map->table[i].state = EMPTY;
        }
    }else {
        for (int i = 0; i < map->capacity; i++) {
            Node *head = map->chainTable[i];

            while (head) {
            Node *temp = head;
            head = head->next;
            free(temp);
            }
            map->chainTable[i] = NULL;
        }
    }
}

// ========== Destroy the full memory of the HashTable
void destroy(HashTable *map){
    clear(map);

    if (map->mode == OPEN_ADDRESSING) {
        free(map->table);
    }else{
        free(map->chainTable);
    }
    free(map);
}

// ========== print all Analytics data
void print_analytics(HashTable *map){
    printf("\n --- ANALYTICS ---\n");
    printf("Inserts : %ld\n", map->analyse.inserts);
    printf("Searches : %ld\n", map->analyse.searches);
    printf("Updates : %ld\n", map->analyse.updates);
    printf("Deletes : %ld\n", map->analyse.deletes);
    printf("Collisions : %ld\n", map->analyse.collisions);
    printf("Probing : %ld\n", map->analyse.probing);
}

int main(){
    printf(" ===== OPEN_ADDRESSING ===== \n");
    HashTable *map1 = create_hash_table(INITIAL_CAPACITY, DOUBLE_HASH, OPEN_ADDRESSING);
    insert(map1, "apple", 100);
    insert(map1, "banana", 200);
    insert(map1, "orange", 300);

    printf("banana = %d\n", search(map1, "banana"));

    update(map1, "banana", 500);
    printf("banana updated = %d\n", search(map1, "banana"));

    delete(map1, "banana");
    printf("banana after delete = %d\n", search(map1, "banana"));

    print_analytics(map1);
    destroy(map1);

    printf("\n ===== SEPARATE_CHAINING =====\n");

    HashTable *map2 = create_hash_table(INITIAL_CAPACITY, LINEAR, SEPARATE_CHAINING);
    insert(map2, "A", 1);
    insert(map2, "B", 2);
    insert(map2, "C", 3);

    printf("B = %d\n", search(map2, "B"));

    update(map2, "B", 777);
    printf("B updated = %d\n", search(map2, "B"));

    delete(map2, "B");
    printf("B after deleted = %d\n", search(map2, "B"));

    print_analytics(map2);
    destroy(map2);
    return 0;
}
