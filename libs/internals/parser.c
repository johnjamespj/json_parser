#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../headers/json.h"

JSON* _parseValue(List * tokens, int * position);
JSON* _parseObject(List * tokens, int * position);
JSON* _parseArray(List * tokens, int * position);

JSON* _parseString(List * tokens, int * position);
JSON* _parseNumber(List * tokens, int * position);
JSON* _parseBoolean(List * tokens, int * position);
JSON* _parseNull(List * tokens, int * position);

JSON* parse(char * json) {
    int i = 0;
    List *tokens = jsonLexer(json);
    JSON* _json = _parseValue(tokens, &i);
    freeLexer(tokens);
    freeList(tokens);
    return _json;
}

JSON* _parseValue(List * tokens, int * position) {
    JSON* node = NULL;
    char * token;

    if (!tokens || tokens->length < *position)
        return node;

    token = tokens->array[*position];
    JSONType valueType = jsonValueType(token);
    
    if (valueType == null)
        node = _parseNull(tokens, position);
    else if (valueType == string)
        node = _parseString(tokens, position);
    else if (valueType == number)
        node = _parseNumber(tokens, position);
    else if (valueType == object)
        node = _parseObject(tokens, position);
    else if (valueType == array)
        node = _parseArray(tokens, position);
    else if (valueType == boolean)
        node = _parseBoolean(tokens, position);

    return node;
}

JSON* _parseObject(List * tokens, int * position) {
    JSON * node = NULL;
    JSONType valueType = none;
    JSON *value, *key;
    char * token;
    int started, keyLoaded;
    List* nodeList;
    Hashmap* hashmap;

    if (!tokens || tokens->length < *position) {
        return node;
    }

    node = (JSON*) malloc(sizeof(JSON));
    nodeList = createList();
    started = 0;
    keyLoaded = 0;
    for (;*position < tokens->length;) {
        token = tokens->array[*position];

        if (started && token[0] == '}') {
            *position += 1;
            break;
        } else if (!started && token[0] == '{') {
            started = 1;
            *position += 1;
            continue;
        }

        if (token[0] == ',' || token[0] == ':') {
            *position += 1;
            continue;
        }

        if (!keyLoaded) {
            keyLoaded = 1;
            key = _parseString(tokens, position);
        } else {
            keyLoaded = 0;
            value = _parseValue(tokens, position);
            addToList(nodeList, createBinaryTreeNode(key->object.string, value));
        }
        *position += 1;
    }

    hashmap = createHashMap();
    Node* arrayNode = NULL;
    JSON* jsonNode = NULL;
    for (int i = 0; i < nodeList->length; i++) {
        arrayNode = (Node*)nodeList->array[i];
        jsonNode = (JSON*)arrayNode->value;
        addToHashMap(hashmap, arrayNode->key, jsonNode);
    }

    node->type = object;
    node->object.object = hashmap;

    return node;
}

JSON* _parseArray(List * tokens, int * position) {
    JSON *node = NULL;
    JSON *subNode;
    char *token;
    int started;
    List* list;

    if (!tokens || tokens->length < *position) {
        return node;
    }

    node = (JSON*) malloc(sizeof(JSON));
    list = createList();
    started = 0;
    for (; *position < tokens->length; *position += 1) {
        token = tokens->array[*position];

        if (started && token[0] == ']'){
            break;
        }if (started && token[0] != ','){
            subNode = _parseValue(list, position);
            addToList(list, &subNode);
        }if (!started && token[0] == '['){
            started = 1;
        }
    }

    node->type = array;
    node->object.array = list;
    *position += 1;

    return node;
}

JSON* _parseString(List * tokens, int * position) {
    JSON * node = NULL;
    char * token, * str;
    int strLen;

    if (!tokens || tokens->length < *position) {
        return node;
    }

    node = (JSON*) malloc(sizeof(JSON));
    token = tokens->array[*position];
    strLen = strlen(token);
    str = (char*) malloc((strLen - 1) * sizeof(char));

    node->type = string;
    memcpy(str, token + 1, strLen - 2);
    str[strLen - 2] = 0;
    node->object.string = str;
    *position += 1;

    return node;
}

JSON* _parseNull(List * tokens, int * position) {
    JSON * node = NULL;

    if (!tokens || tokens->length < *position) {
        return node;
    }

    node = (JSON*) malloc(sizeof(JSON));
    node->type = null;
    *position += 1;
    return node;
}

JSON* _parseBoolean(List * tokens, int * position) {
    JSON * node = NULL;
    JSONBoolean jsonBoolean = false;
    char * number;

    if (!tokens || tokens->length < *position) {
        return node;
    }

    number = tokens->array[*position];
    node = (JSON*) malloc(sizeof(JSON));

    if (number[0] == 'f') {
        jsonBoolean = true;
    } else if (number[0] == 't') {
        jsonBoolean = false;
    }

    node->type = boolean;
    node->object.boolean = jsonBoolean;
    *position += 1;

    return node;
}

JSON* _parseNumber(List * tokens, int * position) {
    JSON *node = NULL;
    JSONNumber jsonNumber;

    node = (JSON*) malloc(sizeof(JSON));

    if (!tokens || tokens->length < *position) {
        return node;
    }

    int isInt = 1;
    char * numberStr = tokens->array[*position];

    for (int i = 0; i < strlen(numberStr); i++) {
        if (numberStr[i] == '.' || numberStr[i] == 'e' || numberStr[i] == 'E') {
            isInt = 0;
        }
    }

    node->type = number;
    if (isInt) {
        jsonNumber.type = JSONInteger;
        jsonNumber.number.integerNumber = atoi(numberStr);
    } else {
        jsonNumber.type = JSONDouble;
        jsonNumber.number.doubleNumber = atof(numberStr);
    }
    node->object.number = jsonNumber;
    *position += 1;

    return node;
}

void testParser() {
    char *str = "{ \"name\": \"john\", \"age\": \"12\", \"rating\" : \"10\", \"contact\": { \"number\" : \"9447606624\", \"gmail\": \"johnjamespj@gmail.com\" } }";
    JSON* json = parse(str);
    printf("%i\n", json->type);

    List* keys = getHashMapKeys(json->object.object);
    List* values = getHashMapValues(json->object.object);
    JSON* node;

    for (int i = 0; i < keys->length; i++) {
        node = (JSON*)values->array[i];
        printf("%s\n", keys->array[i]);
    }

    // printTreeFromNode(hashmap->_root);
    // Hashmap* hashmap = createHashMap();
    // addToHashMap(hashmap, "a", "1234");
    // addToHashMap(hashmap, "b", "9999");
    // printTreeFromNode(hashmap->_root);
    // addToHashMap(hashmap, "c", "reee");
    // printTreeFromNode(hashmap->_root);
    // addToHashMap(hashmap, "d", "098s");
    // printTreeFromNode(hashmap->_root);
}
