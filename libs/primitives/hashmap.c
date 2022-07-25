#include <stdlib.h>
#include <string.h>

#include "../headers/primitives.h"

Hashmap* createHashMap() {
    Hashmap* hashmap = (Hashmap*) malloc(sizeof(Hashmap));
    hashmap->length = 0;
    return hashmap;
}

void addToHashMap(Hashmap* map, char* key, void * data) {
    if (!map->_root) {
        map->_root = createBinaryTreeNode(key, data);
    } else {
        map->_root = addToAVLTree(map->_root, createBinaryTreeNode(key, data));
    }
}

void freeHashMap(Hashmap* map) {
    if (map->_root) freeBinaryTree(map->_root);
    free(map);
}

int containsKeyHashMap(Hashmap* map, char* key) {
    Node* focus = findNode(map->_root, key);
    return focus && strcmp(focus->key, key) == 0;
}

void* getItemAtHashMap(Hashmap* map, char* key) {
    Node* focus = findNode(map->_root, key);

    if (focus && strcmp(focus->key, key) == 0) {
        return focus->value;
    }

    return NULL;
}

void inOrderRead(Node *node, List * list) {
    if (node == NULL) {
        return;
    }

    inOrderRead(node->left, list);
    if (node->left) {
        addToList(list, node->left);
    }

    inOrderRead(node->right, list);
    if (node->right) {
        addToList(list, node->right);
    }
}

List* getHashMapKeys(Hashmap* map) {
    List* list = createList();
    inOrderRead(map->_root, list);
    return list;
}
