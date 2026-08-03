#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_LEVEL 16
#define P 0.5

/*==========================
  Node Structure
  ==========================*/
typedef struct Node
{
    int key;
    struct Node **forward;
    int level;
} Node;

/*==========================
  Skip List Structure
  ==========================*/
typedef struct SkipList
{
    int level;
    Node *header;
} SkipList;

/*==========================
  Create Node
  ==========================*/
Node *create_node(int key, int level)
{
    Node *node = (Node *)malloc(sizeof(Node));

    if (node == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    node->key = key;
    node->level = level;

    node->forward = (Node **)calloc(level + 1, sizeof(Node *));

    if (node->forward == NULL)
    {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    return node;
}

/*==========================
  Create Skip List
  ==========================*/
SkipList *create_skip_list(void)
{
    SkipList *list = (SkipList *)malloc(sizeof(SkipList));

    if (list == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    list->level = 0;

    list->header = create_node(-1, MAX_LEVEL);

    return list;
}

/*==========================
  Random Level
  ==========================*/
int random_level(void)
{
    int level = 0;

    while (((double)rand() / RAND_MAX) < P &&
            level < MAX_LEVEL)
    {
        level++;
    }

    return level;
}

/*==========================
  Search
  ==========================*/
Node *search(SkipList *list, int key)
{
    Node *current = list->header;

    for (int i = list->level; i >= 0; i--)
    {
        while (current->forward[i] &&
                current->forward[i]->key < key)
        {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if (current && current->key == key)
        return current;

    return NULL;
}

/*==========================
  Insert
  ==========================*/
void insert(SkipList *list, int key)
{
    Node *update[MAX_LEVEL + 1];
    Node *current = list->header;

    for (int i = list->level; i >= 0; i--)
    {
        while (current->forward[i] &&
                current->forward[i]->key < key)
        {
            current = current->forward[i];
        }

        update[i] = current;
    }

    current = current->forward[0];

    if (current && current->key == key)
    {
        printf("%d already exists.\n", key);
        return;
    }

    int level = random_level();

    if (level > list->level)
    {
        for (int i = list->level + 1; i <= level; i++)
            update[i] = list->header;

        list->level = level;
    }

    Node *new_node = create_node(key, level);

    for (int i = 0; i <= level; i++)
    {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }
}

/*==========================
  Delete
  ==========================*/
bool delete_key(SkipList *list, int key)
{
    Node *update[MAX_LEVEL + 1];
    Node *current = list->header;

    for (int i = list->level; i >= 0; i--)
    {
        while (current->forward[i] &&
                current->forward[i]->key < key)
        {
            current = current->forward[i];
        }

        update[i] = current;
    }

    current = current->forward[0];

    if (current == NULL || current->key != key)
        return false;

    for (int i = 0; i <= list->level; i++)
    {
        if (update[i]->forward[i] != current)
            break;

        update[i]->forward[i] = current->forward[i];
    }

    free(current->forward);
    free(current);

    while (list->level > 0 &&
            list->header->forward[list->level] == NULL)
    {
        list->level--;
    }

    return true;
}

/*==========================
  Print
  ==========================*/
void print_skip_list(SkipList *list)
{
    printf("\n");

    for (int i = list->level; i >= 0; i--)
    {
        printf("Level %2d : ", i);

        Node *current = list->header->forward[i];

        while (current)
        {
            printf("%d ", current->key);
            current = current->forward[i];
        }

        printf("\n");
    }

    printf("\n");
}

/*==========================
  Free Skip List
  ==========================*/
void free_skip_list(SkipList *list)
{
    Node *current = list->header->forward[0];

    while (current)
    {
        Node *next = current->forward[0];

        free(current->forward);
        free(current);

        current = next;
    }

    free(list->header->forward);
    free(list->header);

    free(list);
}

/*==========================
  Main
  ==========================*/
int main(void)
{
    srand((unsigned)time(NULL));

    SkipList *list = create_skip_list();

    insert(list, 10);
    insert(list, 20);
    insert(list, 15);
    insert(list, 30);
    insert(list, 5);
    insert(list, 8);
    insert(list, 18);
    insert(list, 25);
    insert(list, 35);
    insert(list, 40);
    insert(list, 50);

    print_skip_list(list);

    Node *result = search(list, 25);

    if (result)
        printf("Found: %d\n", result->key);
    else
        printf("Not Found\n");

    printf("\nDeleting 25...\n");

    if (delete_key(list, 25))
        printf("Deleted successfully.\n");
    else
        printf("Key not found.\n");

    print_skip_list(list);

    free_skip_list(list);

    return 0;
}
