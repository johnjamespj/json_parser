#include "./avl.h"

/**
 * @brief List struct
 */
typedef struct {
    int length;
    int _space;
    void ** array;
} List;

/**
 * @brief Create a List object
 * 
 * @return List* new list
 */
List* createList();

/**
 * @brief Creates a list
 * 
 * @param list to be added to
 * @param data pointer to be added
 */
void addToList(List* list, void * data);

/**
 * @brief Frees a list and the underlying array.
 * 
 * @param list to be freed
 */
void freeList(List* list);

/**
 * @brief Hashmap struct
 * 
 */
typedef struct {
    int length;
    Node * _root;
} Hashmap;

/**
 * @brief Create a Hash Map object
 * 
 * @return Hashmap* 
 */
Hashmap* createHashMap();

/**
 * @brief Added an item the hash map
 * 
 * @param map 
 * @param key 
 * @param data 
 */
void addToHashMap(Hashmap* map, char* key, void * data);

/**
 * @brief Frees a hashmap
 * 
 * @param map 
 */
void freeHashMap(Hashmap* map);

/**
 * @brief Checks if a key is in the hashmap
 * 
 * @param map 
 * @param key 
 * @return int 
 */
int containsKeyHashMap(Hashmap* map, char* key);
