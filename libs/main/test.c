#include <stdio.h>
#include <stdlib.h>

#include "../headers/primitives.h"

int main(int argc, char *argv[]) {
    Hashmap*  hashmap = createHashMap();

    addToHashMap(hashmap, "1", "a");
    addToHashMap(hashmap, "2", "b");
    addToHashMap(hashmap, "3", "c");
    addToHashMap(hashmap, "4", "de");
    addToHashMap(hashmap, "5", "dej");
    addToHashMap(hashmap, "6", "doj");

    printf("1:%i\n", containsKeyHashMap(hashmap, "1"));         // 1-true
    printf("112:%i\n\n", containsKeyHashMap(hashmap, "112"));   // 0-false

    printf("1:%s\n", getItemAtHashMap(hashmap, "1"));           // a
    printf("6:%s\n", getItemAtHashMap(hashmap, "6"));           // doj
    printf("122:%s\n\n", getItemAtHashMap(hashmap, "122"));     // (NULL)

    freeHashMap(hashmap);
    return 0;
}