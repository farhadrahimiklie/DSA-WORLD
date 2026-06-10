#ifndef SECOND_HASH_H
#define SECOND_HASH_H

// ========== type alias
typedef unsigned int uint;
typedef unsigned long ulong;

// ========== CONSTANTS
#define INITIAL_CAPACITY 8
#define LOAD_FACTOR_BALANCE_THRESHOLD 0.7
#define KEY_CAPACITY 64

// ========== ENUMS
typedef enum { LINEAR, QUADRATIC, DOUBLE_HASH } ProbingTypes;
typedef enum { OPEN_ADDRESSING, SEPARATE_CHAINING } CollisionMode;
typedef enum { EMPTY, OCCUPIED, DELETED } Status;

// ========== Entry (OPEN_ADDRESSING)
typedef struct {
    char key[KEY_CAPACITY];
    int value;
    Status status;
}Entry;

// ========== Node (SEPARATE_CHAINING)
typedef struct Node{
    char key[KEY_CAPACITY];
    int value;
    struct Node *next;
}Node;

// ========== Statistics
typedef struct {
    long inserts, searches, deletes, updates;
    long collisions, probing;
}Statistics;

// ========== Hash Table Structure
typedef struct {
    Entry *bucket; // OPEN_ADDRESSING
    Node **chainbucket; // SEPARATE_CHAINING

    int capacity;
    int count;

    ProbingTypes probing;
    CollisionMode mode;

    Statistics statistics;
}HashTable;


// ========== first Hash
uint first_hash(char *key, int capacity);
uint second_hash(char *key, int capacity);
uint which_type_of_probe_should_return(HashTable *map, char *key, int i);
void rehash_open_addressing(HashTable *map);
void rehash_separate_chain(HashTable *map);
void insert_open_raw(HashTable *map, char *key, int value);
void insert_chain_raw(HashTable *map, char *key, int value);
void rehash(HashTable *map);
HashTable *create_hash_table(int capacity, ProbingTypes probing, CollisionMode mode);
void insert_to_open_addressing(HashTable *map, char *key, int value);
int search_from_open_addressing(HashTable *map, char *key);
int delete_from_open_addressing(HashTable *map, char *key);
int update_from_open_addressing(HashTable *map, char *key, int value);
void insert_to_separate_chaining(HashTable *map, char *key, int value);
int search_from_separate_chaining(HashTable *map, char *key);
int delete_from_separate_chaining(HashTable *map, char *key);
int update_from_separate_chaining(HashTable *map, char *key, int value);
void insert(HashTable *map, char *key, int value);
int search(HashTable *map, char *key);
int delete(HashTable *map, char *key);
int update(HashTable *map, char *key, int value);
int exists(HashTable *map, char *key);
int size(HashTable *map);
int isEmpty(HashTable *map);
void clear(HashTable *map);
void destroy(HashTable *map);
void print_Statistics(HashTable *map);
#endif
