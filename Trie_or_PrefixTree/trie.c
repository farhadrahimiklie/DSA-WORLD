#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define N 26

typedef struct Node{
    struct Node *children[N];

    bool is_end_of_word;
}Node;

Node *create_node(){
    Node *node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        printf("Memory Allocation is Failed.\n");
        exit(EXIT_FAILURE);
    }

    node->is_end_of_word = false;

    for (int i = 0; i < N; i++) {
        node->children[i] = NULL;
    }

    return node;
}

void insert(Node *root, const char *word){
    Node *current = root;
    while (*word) { // h e l l o
        int index = *word - 'a';
        if (current->children[index] == NULL) {
            current->children[index] = create_node();
        }

        current = current->children[index];

        word++;
    }

    current->is_end_of_word = true;
}

bool search(Node *root, const char *word){
    Node *current = root;

    while (*word) {
        int index = *word - 'a';

        if (current->children[index] == NULL) {
            return false;
        }

        current = current->children[index];

        word++;
    }

    return current->is_end_of_word;
}

bool starts_with(Node *root, const char *prefix){
    Node *current = root;

    while (*prefix) {
        int index = *prefix - 'a';

        if (current->children[index] == NULL) {
            return false;
        }

        current = current->children[index];

        prefix++;
    }

    return true;
}

bool has_children(Node *root){
    for (int i = 0; i < N; i++) {
        if (root->children[i] != NULL) {
            return true;
        }
    }

    return false;
}

bool delete_helper(Node *root, const char *word){
    // end of the word reached.
    if (*word == '\0') {
        // word doesn't exists.
        if (!root->is_end_of_word) {
            return false;
        }

        // remove end-of-the-word marker
        root->is_end_of_word = false;

        // Tell parent this node can be deleted. only if it has no children
        return !has_children(root);
    }

    int index = *word - 'a';

    Node *child = root->children[index];

    // word not found
    if (child == NULL) {
        return false;
    }

    bool should_delete_child = delete_helper(child, word + 1);

    if (should_delete_child) {
        free(child);
        root->children[index] = NULL;

        // don't delete root.
        return (!root->is_end_of_word && !has_children(root));
    }

    return false;
}

void delete_word(Node *root, const char *word){
    if (root == NULL) {
        return;
    }

    delete_helper(root, word);
}

int main(){
        Node *root = create_node();

        insert(root, "apple");
        insert(root, "app");
        insert(root, "ape");
        insert(root, "cat");
        insert(root, "car");
        insert(root, "care");

        printf("Initial Search\n");
        printf("-----------------\n");

        printf("apple : %d\n", search(root, "apple"));
        printf("app   : %d\n", search(root, "app"));
        printf("ape   : %d\n", search(root, "ape"));
        printf("cat   : %d\n", search(root, "cat"));
        printf("care  : %d\n", search(root, "care"));
        printf("dog   : %d\n", search(root, "dog"));

        printf("\n");

        printf("Deleting apple...\n");
        delete_word(root, "apple");

        printf("apple : %d\n", search(root, "apple"));
        printf("app   : %d\n", search(root, "app"));

        printf("\n");

        printf("Deleting app...\n");
        delete_word(root, "app");

        printf("app   : %d\n", search(root, "app"));
        printf("ape   : %d\n", search(root, "ape"));

        printf("\n");

        printf("Deleting care...\n");
        delete_word(root, "care");

        printf("care  : %d\n", search(root, "care"));
        printf("car   : %d\n", search(root, "car"));

        printf("\n");

        printf("Deleting car...\n");
        delete_word(root, "car");

        printf("car   : %d\n", search(root, "car"));
        printf("cat   : %d\n", search(root, "cat"));

        printf("\n");

        printf("Deleting cat...\n");
        delete_word(root, "cat");

        printf("cat   : %d\n", search(root, "cat"));

    return 0;
}
