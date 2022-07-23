/**
 * @brief RBNode entry and node in the red black tree
 * 
 */
typedef struct NodeStruct {
    char * key;
    void * value;

    struct NodeStruct * left;
    struct NodeStruct * right;
} Node;

/**
 * @brief Truck of binary to be displayed
 * 
 */
typedef struct TrunkStruct
{
    struct TrunkStruct * prev;
    char * str;
} Trunk;

/**
 * @brief Print the tree from the node
 * 
 * @param root 
 */
void printTreeFromNode(Node* root);

/** Prints a binary tree from a node */
void printTreeFromNode(Node* root);

/** AVL Tree insertion */
Node* addToAVLTree(Node* root, Node* newNode);

/** Creates a tree node */
Node* createBinaryTreeNode(char * key, void * value);

/** Find the node for the give value */
Node* findNode(Node* root, char * key);

/** Free the binary tree */
void freeBinaryTree(Node* root);

/** Height of a binary tree */
int treeHeight(Node* root);

/** checks if a tree is balanced */
int isTreeBalanced(Node* root);

void testTree();
