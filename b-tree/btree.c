#include <stdio.h>
#include <stdlib.h>

#define T 2
#define MAX_KEYS (2 * T - 1)
#define CHILDREN (2 * T)

int level = 0;

typedef struct Node{
    int keys[MAX_KEYS];
    struct Node *children[CHILDREN];
    int used_keys;
    int leaf;
}Node;

// ---------- Create Node ----------
Node *createNode(int leaf){
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory Allocation is Failed.\n");
        exit(EXIT_FAILURE);
    }
    newNode->leaf = leaf;
    newNode->used_keys = 0;
 
    for(int j = 0; j < MAX_KEYS; j++){
        newNode->keys[j] = 0;
    }

    for(int i = 0; i < CHILDREN; i++){
        newNode->children[i] = NULL;
    }

    return newNode;
}

// --------- Split Child ----------
void SplitChild(Node *parent, int level, Node *child){
    Node *new_child = createNode(child->leaf);

    new_child->used_keys = T -1;

    // Move last keys to new node
    for (int j = 0; j < T - 1; j++){
        new_child->keys[j] = child->keys[j + T];
    }

    // Move children if not leaf
    if (!child->leaf) {
        for (int j = 0; j < T; j++){
            new_child->children[j] = child->children[j + T];
        }
    }

    child->used_keys = T - 1;

    // Shift parent's children
    for (int j = parent->used_keys; j >= level + 1; j--)
        parent->children[j + 1] = parent->children[j];

    parent->children[level + 1] = new_child;

    // Shift parent's keys
    for (int j = parent->used_keys - 1; j >= level; j--)
        parent->keys[j + 1] = parent->keys[j];

    // Move middle key up
    parent->keys[level] = child->keys[T - 1];
    parent->used_keys++;
}

void insertNonFull(Node *root, int key) {
    int index = root->used_keys - 1;
    if (root->leaf) {
        while (index >= 0 && key < root->keys[index]) {
            root->keys[index + 1] = root->keys[index];
            index--;
        }
        root->keys[index + 1] = key;
        root->used_keys++;
    } else {
        while (index >= 0 && key < root->keys[index])
            index--;

        index++;

        if (root->children[index]->used_keys == MAX_KEYS) {
            SplitChild(root, level, root->children[index]);

            if (key > root->keys[index])
                index++;
        }

        insertNonFull(root->children[index], key);
    }
}

Node* insert(Node* root, int key) {
    if (root->used_keys == MAX_KEYS) {
        Node* newRoot = createNode(0);
        newRoot->children[0] = root;

        SplitChild(newRoot, level, root);

        insertNonFull(newRoot, key);

        return newRoot;
    } else {
        insertNonFull(root, key);
        return root;
    }
}

Node* search(Node* root, int key) {
    int index = 0;

    while (index < root->used_keys && key > root->keys[index])
        index++;

    if (index < root->used_keys && key == root->keys[index])
        return root;

    if (root->leaf)
        return NULL;

    return search(root->children[index], key);
}


int findKey(Node* root, int key) {
    int index = 0;
    while (index < root->used_keys && root->keys[index] < key)
        index++;
    return index;
}


void removeFromLeaf(Node* root, int index) {
    for (int i = index + 1; i < root->used_keys; i++)
        root->keys[i - 1] = root->keys[i];
    root->used_keys--;
}


int getPredecessor(Node* root, int index) {
    Node* current = root->children[index];
    while (!current->leaf)
        current = current->children[current->used_keys];
    return current->keys[current->used_keys - 1];
}


int getSuccessor(Node* root, int index) {
    Node* current = root->children[index + 1];
    while (!current->leaf)
        current = current->children[0];
    return current->keys[0];
}


void merge(Node* root, int index) {
    Node* child = root->children[index];
    Node* sibling = root->children[index + 1];

    child->keys[T - 1] = root->keys[index];

    for (int i = 0; i < sibling->used_keys; i++)
        child->keys[i + T] = sibling->keys[i];

    if (!child->leaf) {
        for (int i = 0; i <= sibling->used_keys; i++)
            child->children[i + T] = sibling->children[i];
    }

    for (int i = index + 1; i < root->used_keys; i++)
        root->keys[i - 1] = root->keys[i];

    for (int i = index + 2; i <= root->used_keys; i++)
        root->children[i - 1] = root->children[i];

    child->used_keys += sibling->used_keys + 1;
    root->used_keys--;

    free(sibling);
}


void borrowFromPrev(Node* root, int index) {
    Node* child = root->children[index];
    Node* sibling = root->children[index - 1];

    for (int i = child->used_keys - 1; i >= 0; i--)
        child->keys[i + 1] = child->keys[i];

    child->keys[0] = root->keys[index - 1];
    root->keys[index - 1] = sibling->keys[sibling->used_keys - 1];

    child->used_keys++;
    sibling->used_keys--;
}


void borrowFromNext(Node* root, int index) {
    Node* child = root->children[index];
    Node* sibling = root->children[index + 1];

    child->keys[child->used_keys] = root->keys[index];
    root->keys[index] = sibling->keys[0];

    for (int i = 1; i < sibling->used_keys; i++)
        sibling->keys[i - 1] = sibling->keys[i];

    child->used_keys++;
    sibling->used_keys--;
}

void fill(Node* root, int index) {
    if (index != 0 && root->children[index - 1]->used_keys >= T)
        borrowFromPrev(root, index);
    else if (index != root->used_keys && root->children[index + 1]->used_keys >= T)
        borrowFromNext(root, index);
    else {
        if (index != root->used_keys)
            merge(root, index);
        else
            merge(root, index - 1);
    }
}


void removeFromNonLeaf(Node* root, int index);

void removeKey(Node* root, int key) {
    int index = findKey(root, key);

    if (index < root->used_keys && root->keys[index] == key) {
        if (root->leaf)
            removeFromLeaf(root, index);
        else
            removeFromNonLeaf(root, index);
    } else {
        if (root->leaf) {
            printf("Key not found\n");
            return;
        }

        int flag = (index == root->used_keys);

        if (root->children[index]->used_keys < T)
            fill(root, index);

        if (flag && index > root->used_keys)
            removeKey(root->children[index - 1], key);
        else
            removeKey(root->children[index], key);
    }
}


void removeFromNonLeaf(Node* root, int index) {
    int key = root->keys[index];

    if (root->children[index]->used_keys >= T) {
        int pred = getPredecessor(root, index);
        root->keys[index] = pred;
        removeKey(root->children[index], pred);
    } else if (root->children[index + 1]->used_keys >= T) {
        int succ = getSuccessor(root, index);
        root->keys[index] = succ;
        removeKey(root->children[index + 1], succ);
    } else {
        merge(root, index);
        removeKey(root->children[index], key);
    }
}


void printTree(Node* root, int level) {
    if (root != NULL) {
        printf("Level %d: ", level);

        for (int i = 0; i < root->used_keys; i++)
            printf("%d ", root->keys[i]);

        printf("\n");

        for (int i = 0; i <= root->used_keys; i++)
            printTree(root->children[i], level + 1);
    }
}

int main() {
    Node* root = createNode(1);

    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    //int keys[] = {1,2,3,4,5,6,7};
    int n = sizeof(keys)/sizeof(keys[0]);

    for (int i = 0; i < n; i++)
        root = insert(root, keys[i]);

    printf("B-Tree:\n");
    printTree(root, level);

    int key = 6;
    if (search(root, key))
        printf("Found %d\n", key);
    else
        printf("Not Found %d\n", key);


     printf("\nDeleting 6...\n");
     removeKey(root, 6);
     printTree(root, 0);
    return 0;
}
