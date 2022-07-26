#include "../headers/internals.h"

/** JSON Specs: https://www.json.org/json-en.html */
typedef enum {
    true,
    false
} JSONBoolean;

typedef enum {
    JSONDouble,
    JSONInteger
} JSONNumberType;

typedef union {
    long integerNumber;
    double doubleNumber;
} JSONUnionNumber;

typedef struct {
    JSONUnionNumber number;
    JSONNumberType type;
} JSONNumber;

typedef union JSONObjectUnion {
    JSONBoolean boolean;
    JSONNumber number;
    // free this string
    char * string;
    // free this list
    List * array;
    // free this hashmap
    Hashmap * object;
} JSONObject;

typedef struct {
    JSONType type;
    JSONObject object;
} JSON;

JSON* parse(char * json);

void testParser();
