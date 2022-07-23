#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/avl.h"

int max(int a, int b) {
    return a > b ? a : b;
}

Node* createBinaryTreeNode(char * key, void * value) {
    Node *node = (Node*) malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    return node;
}

Node* findNode(Node* root, char * key) {
    int strComp;

    if (root == NULL)
        return NULL;
    else if (key == NULL)
        return NULL;

    strComp = strcmp(root->key, key);

    if (strComp < 0 && root->left)
        return findNode(root->left, key);
    else if (strComp > 0 && root->right)
        return findNode(root->right, key);
    return root;
}

void freeBinaryTree(Node* root) {
    if (root->left)
        freeBinaryTree(root->left);
    if (root->right)
        freeBinaryTree(root->right);
    free(root);
}

Node* rotateLL(Node* root) {
    Node* y = root->left;
    Node* right = root->right;

    root->left = y->right;
    y->right = root;

    return y;
}

Node* rotateLR(Node* root) {
    Node* y = root->left;
    Node* x = y->right;
    Node* T2 = x->right;

    x->left = y;
    root->left = x;
    y->right = T2;
    return rotateLL(root);
}

Node* rotateRR(Node* root) {
    Node* left = root->left;
    Node* y = root->right;

    root->right = y->left;
    y->left = root;

    return y;
}

Node* rotateRL(Node* root) {
    Node* y = root->right;
    Node* x = y->left;
    Node* T3 = x->right;

    x->right = y;
    root->right = x;
    y->left = T3;
    return rotateRR(root);
}


int isLeftChildOf(Node* root, Node* child) {
    return root->left == child;
}

int isTreeBalanced(Node* root) {
    int df = abs(treeHeight(root->left) - treeHeight(root->right));
    return df <= 1;
}

int treeHeight(Node* root) {
    int left, right;

    if (!root)
        return 0;
    
    left = treeHeight(root->left) + 1;
    right = treeHeight(root->right) + 1;

    return left > right ? left : right;
}

Node* addToAVLTree(Node* root, Node* newNode){
    int strComp, childIsLeft, gchildIsLeft;
    Node* temp = root;
    Node *child, *gchild;

    if (root == NULL)
        return NULL;

    strComp = strcmp(root->key, newNode->key);

    // recursively find the node
    if (strComp < 0 && root->left)
        temp->left = addToAVLTree(root->left, newNode);
    else if (strComp > 0 && root->right)
        temp->right = addToAVLTree(root->right, newNode);

    // add a new node
    if (strComp < 0 && !root->left)
        root->left = newNode;
    else if (strComp > 0 && !root->right)
        root->right = newNode;

    if (!isTreeBalanced(root)) {
        childIsLeft = strcmp(root->key, newNode->key) < 0;
        gchildIsLeft = strcmp(childIsLeft ? root->left->key : root->right->key, newNode->key) < 0;
        child =  childIsLeft ? root->left : root->right;
        gchild = gchildIsLeft ? child->left : child->right;

        if (childIsLeft && gchildIsLeft) {
            temp = rotateLL(root);
        } else if (!childIsLeft && !gchildIsLeft) {
            temp = rotateRR(root);
        } else if (childIsLeft && !gchildIsLeft) {
            temp = rotateLR(root);
        } else if (!childIsLeft && gchildIsLeft) {
            temp = rotateRL(root);
        }
    }

    return temp;
}

void testTree(){
    Node* root = createBinaryTreeNode("991", "qwasd");
    printTreeFromNode(root);
    root = addToAVLTree(root, createBinaryTreeNode("abc", "qwasd"));
    printTreeFromNode(root);
    root = addToAVLTree(root, createBinaryTreeNode("jkl", "qwasd"));
    printTreeFromNode(root);
    root = addToAVLTree(root, createBinaryTreeNode("hgf", "qwasd"));
    printTreeFromNode(root);
    root = addToAVLTree(root, createBinaryTreeNode("a", "qwasd"));
    printTreeFromNode(root);
    root = addToAVLTree(root, createBinaryTreeNode("b", "qwasd"));
    printTreeFromNode(root);
    root = addToAVLTree(root, createBinaryTreeNode("2", "qwasd"));
    printTreeFromNode(root);
    root = addToAVLTree(root, createBinaryTreeNode("112", "qwasd"));
    printTreeFromNode(root);
    root = addToAVLTree(root, createBinaryTreeNode("dd", "qwasd"));
    printTreeFromNode(root);
    freeBinaryTree(root);
}

void showTrunks(Trunk *p){
    if (p == NULL) {
        return;
    }
 
    showTrunks(p->prev);
    printf("%s", p->str);
}

void printTree(Node* root, Trunk *prev, int isLeft){
    if (root == NULL) {
        return;
    }
 
    char * prev_str = "    ";
    Trunk *trunk = (Trunk*) malloc(sizeof(Trunk));
    trunk->str = prev_str;
    trunk->prev = prev;
 
    printTree(root->right, trunk, 1);
 
    if (!prev) {
        trunk->str = "———";
    }
    else if (isLeft)
    {
        trunk->str = ".———";
        prev_str = "   |";
    }
    else {
        trunk->str = "`———";
        prev->str = prev_str;
    }
 
    showTrunks(trunk);
    printf(" %s\n", root->key);
 
    if (prev) {
        prev->str = prev_str;
    }
    trunk->str = "   |";
 
    printTree(root->left, trunk, 0);

    // free trunk
    free(trunk);
}

void printTreeFromNode(Node* root) {
    printf("\n");
    printTree(root, NULL, 0);
    printf("\n");
}
