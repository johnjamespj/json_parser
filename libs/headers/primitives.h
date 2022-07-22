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
