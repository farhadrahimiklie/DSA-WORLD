#ifndef HASH_H
#define HASH_H

#define INITIAL_SIZE 10
#define LOAD_FACTOR_THRESHOLD 0.7

#define EMPTY 0
#define OCCUPIED 1
#define DELETED 2

typedef unsigned int uint;
typedef unsigned long ulong;

typedef struct {
    char key[20];
    int value;
    int isOccupied;
}Entry;

typedef struct {
    Entry *bucket;
    int size;
    int count;
}HashTable;

uint hash(char *key, int size);
void rehash(HashTable *map);
HashTable *create_hash_table(int size);
void insert_entry_to_bucket(HashTable *map, char *key, int value);
int search_from_bucket(HashTable *map, char *key);
int delete_from_bucket(HashTable *map, char *key);
int update_from_bucket(HashTable *map, char *key, int newValue);
int is_entry_in_the_bucket_exit(HashTable *map, char *key);
int size(HashTable *map);
int isEmpty(HashTable *map);
void clear(HashTable *map);
void destroyTable(HashTable *map);
#endif
