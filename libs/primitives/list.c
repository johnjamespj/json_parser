#include <stdlib.h>

#include "../headers/primitives.h"

List* createList() {
    List* list;
    list = (List*) malloc(sizeof(List));
    list->length = 0;
    list->_space = 0;
    return list;
}

void freeList(List* list) {
    free(list->array);
    free(list);
}

void addToList(List* list, void * data) {
    // add first item
    if (list->length == 0) {
        list->array = (void**)malloc(2 * sizeof(void**));
        list->_space = 2;
    }

    // double if need more space
    if (list->length == list->_space) {
        list->_space *= 2;
        list->array = realloc(list->array, list->_space * sizeof(void**));
    }

    // add item
    list->array[list->length++] = data;
}
