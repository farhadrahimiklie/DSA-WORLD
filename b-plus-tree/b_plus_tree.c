#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ORDER 5
#define MAX_KEYS (ORDER - 1)
#define MIN_KEYS ((MAX_KEYS + 1) / 2)

typedef struct Node {
    int keys[MAX_KEYS];
    int values[MAX_KEYS];       
    struct Node* children[ORDER];  
    struct Node* next;             

    int n_elements_used;
    bool is_leaf;
} Node;



Node* CreateNode(bool leaf) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    node->is_leaf = leaf;
    node->n_elements_used = 0;
    node->next = NULL;

    for (int i = 0; i < MAX_KEYS; i++) {
        node->keys[i] = 0;
        node->values[i] = 0;
    }

    for (int i = 0; i < ORDER; i++) {
        node->children[i] = NULL;
    }

    return node;
}



Node* find_leaf(Node* root, int key) {
    if (!root) return NULL;

    Node* cur = root;

    while (!cur->is_leaf) {
        int i = 0;
        while (i < cur->n_elements_used && key >= cur->keys[i]) {
            i++;
        }
        cur = cur->children[i];
    }

    return cur;
}



bool Search(Node* root, int key, int* value) {
    Node* leaf = find_leaf(root, key);
    if (!leaf) return false;

    for (int i = 0; i < leaf->n_elements_used; i++) {
        if (leaf->keys[i] == key) {
            *value = leaf->values[i];
            return true;
        }
    }

    return false;
}



void insert_leaf(Node* leaf, int key, int value) {
    int i;


    for (i = 0; i < leaf->n_elements_used; i++) {
        if (leaf->keys[i] == key) {
            leaf->values[i] = value;
            return;
        }
    }


    i = leaf->n_elements_used - 1;
    while (i >= 0 && leaf->keys[i] > key) {
        leaf->keys[i + 1] = leaf->keys[i];
        leaf->values[i + 1] = leaf->values[i];
        i--;
    }

    leaf->keys[i + 1] = key;
    leaf->values[i + 1] = value;
    leaf->n_elements_used++;
}



Node* split_leaf(Node* leaf, int* up_key) {
    Node* new_leaf = CreateNode(true);

    int mid = (leaf->n_elements_used + 1) / 2;

    new_leaf->n_elements_used = leaf->n_elements_used - mid;
    leaf->n_elements_used = mid;

    for (int i = 0; i < new_leaf->n_elements_used; i++) {
        new_leaf->keys[i] = leaf->keys[mid + i];
        new_leaf->values[i] = leaf->values[mid + i];
    }

    new_leaf->next = leaf->next;
    leaf->next = new_leaf;

    *up_key = new_leaf->keys[0];
    return new_leaf;
}

Node* split_internal(Node* node, int* up_key) {
    Node* new_node = CreateNode(false);

    int mid = node->n_elements_used / 2;

    *up_key = node->keys[mid];

    int j = 0;


    for (int i = mid + 1; i < node->n_elements_used; i++) {
        new_node->keys[j++] = node->keys[i];
    }

    new_node->n_elements_used = j;


    j = 0;
    for (int i = mid + 1; i <= node->n_elements_used; i++) {
        new_node->children[j++] = node->children[i];
    }

    node->n_elements_used = mid;

    return new_node;
}


Node* insert_recursive(Node* node, int key, int value, int* up_key) {
    if (node->is_leaf) {
        insert_leaf(node, key, value);

        if (node->n_elements_used <= MAX_KEYS)
            return NULL;

        return split_leaf(node, up_key);
    }

    int i = 0;
    while (i < node->n_elements_used && key >= node->keys[i]) {
        i++;
    }

    int child_up_key;
    Node* new_child = insert_recursive(node->children[i], key, value, &child_up_key);

    if (!new_child) return NULL;


    for (int j = node->n_elements_used; j > i; j--) {
        node->keys[j] = node->keys[j - 1];
    }

    for (int j = node->n_elements_used + 1; j > i + 1; j--) {
        node->children[j] = node->children[j - 1];
    }

    node->keys[i] = child_up_key;
    node->children[i + 1] = new_child;
    node->n_elements_used++;

    if (node->n_elements_used <= MAX_KEYS)
        return NULL;

    return split_internal(node, up_key);
}



Node* insert(Node* root, int key, int value) {
    if (!root) {
        root = CreateNode(true);
        root->keys[0] = key;
        root->values[0] = value;
        root->n_elements_used = 1;
        return root;
    }

    int up_key;
    Node* new_child = insert_recursive(root, key, value, &up_key);

    if (!new_child) return root;

    Node* new_root = CreateNode(false);
    new_root->keys[0] = up_key;
    new_root->children[0] = root;
    new_root->children[1] = new_child;
    new_root->n_elements_used = 1;

    return new_root;
}


void borrow_from_left(Node* parent, int idx) {
    Node* node = parent->children[idx];
    Node* left = parent->children[idx - 1];

    for (int i = node->n_elements_used; i > 0; i--) {
        node->keys[i] = node->keys[i - 1];
        if (node->is_leaf)
            node->values[i] = node->values[i - 1];
    }

    if (node->is_leaf) {
        node->keys[0] = left->keys[left->n_elements_used - 1];
        node->values[0] = left->values[left->n_elements_used - 1];
        parent->keys[idx - 1] = node->keys[0];
    } else {
        node->keys[0] = parent->keys[idx - 1];
        parent->keys[idx - 1] = left->keys[left->n_elements_used - 1];

        for (int i = node->n_elements_used + 1; i > 0; i--)
            node->children[i] = node->children[i - 1];

        node->children[0] = left->children[left->n_elements_used];
    }

    left->n_elements_used--;
    node->n_elements_used++;
}

void borrow_from_right(Node* parent, int idx) {
    Node* node = parent->children[idx];
    Node* right = parent->children[idx + 1];

    if (node->is_leaf) {
        node->keys[node->n_elements_used] = right->keys[0];
        node->values[node->n_elements_used] = right->values[0];

        for (int i = 1; i < right->n_elements_used; i++) {
            right->keys[i - 1] = right->keys[i];
            right->values[i - 1] = right->values[i];
        }

        parent->keys[idx] = right->keys[0];
    } else {
        node->keys[node->n_elements_used] = parent->keys[idx];
        parent->keys[idx] = right->keys[0];

        node->children[node->n_elements_used + 1] = right->children[0];

        for (int i = 1; i <= right->n_elements_used; i++)
            right->children[i - 1] = right->children[i];

        for (int i = 1; i < right->n_elements_used; i++)
            right->keys[i - 1] = right->keys[i];
    }

    right->n_elements_used--;
    node->n_elements_used++;
}



void merge(Node* parent, int idx) {
    Node* left = parent->children[idx];
    Node* right = parent->children[idx + 1];

    if (!left->is_leaf) {
        left->keys[left->n_elements_used++] = parent->keys[idx];
    }

    for (int i = 0; i < right->n_elements_used; i++) {
        left->keys[left->n_elements_used + i] = right->keys[i];
        if (left->is_leaf)
            left->values[left->n_elements_used + i] = right->values[i];
    }

    if (!left->is_leaf) {
        for (int i = 0; i <= right->n_elements_used; i++)
            left->children[left->n_elements_used + i] = right->children[i];
    } else {
        left->next = right->next;
    }

    left->n_elements_used += right->n_elements_used;

    for (int i = idx + 1; i < parent->n_elements_used; i++)
        parent->keys[i - 1] = parent->keys[i];

    for (int i = idx + 2; i <= parent->n_elements_used; i++)
        parent->children[i - 1] = parent->children[i];

    parent->n_elements_used--;

    free(right);
}


void delete_recursive(Node* root, int key) {
    int idx = 0;

    while (idx < root->n_elements_used && key > root->keys[idx]) {
        idx++;
    }

    if (root->is_leaf) {
        if (idx == root->n_elements_used || root->keys[idx] != key)
            return;

        for (int i = idx + 1; i < root->n_elements_used; i++) {
            root->keys[i - 1] = root->keys[i];
            root->values[i - 1] = root->values[i];
        }

        root->n_elements_used--;
        return;
    }

    Node* child = root->children[idx];

    if (child->n_elements_used < MIN_KEYS) {
        if (idx > 0 && root->children[idx - 1]->n_elements_used > MIN_KEYS)
            borrow_from_left(root, idx);
        else if (idx < root->n_elements_used && root->children[idx + 1]->n_elements_used > MIN_KEYS)
            borrow_from_right(root, idx);
        else {
            if (idx < root->n_elements_used)
                merge(root, idx);
            else {
                merge(root, idx - 1);
                idx--;
            }
        }
    }

    delete_recursive(root->children[idx], key);
}

Node* delete(Node* root, int key) {
    if (!root) return NULL;

    delete_recursive(root, key);

    if (!root->is_leaf && root->n_elements_used == 0) {
        Node* tmp = root;
        root = root->children[0];
        free(tmp);
    }

    if (root && root->is_leaf && root->n_elements_used == 0) {
        free(root);
        return NULL;
    }

    return root;
}



void Traversal(Node* root) {
    if (!root) return;

    Node* cur = root;
    while (!cur->is_leaf)
        cur = cur->children[0];

    while (cur) {
        for (int i = 0; i < cur->n_elements_used; i++) {
            printf("(%d:%d) ", cur->keys[i], cur->values[i]);
        }
        cur = cur->next;
    }
}



int main() {
    Node* root = NULL;

    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int size = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < size; i++) {
        root = insert(root, keys[i], keys[i] * 10);
    }

    printf("B+ Tree:\n");
    Traversal(root);
    printf("\n");

    int val;
    if (Search(root, 20, &val))
        printf("Found 20 -> %d\n", val);
    else
        printf("Not found\n");

    int del[] = {6, 7, 5, 17, 20};
    for (int i = 0; i < 5; i++) {
        root = delete(root, del[i]);
    }

    printf("After deletion:\n");
    Traversal(root);
    printf("\n");

    return 0;
}
