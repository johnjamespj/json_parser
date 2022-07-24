#include "../headers/primitives.h"

typedef enum {
    null = 0,
    string = 1,
    number = 2,
    object = 3,
    array = 4,
    boolean = 5,
    none
} JSONType;

/** Parses json string into list of relevenet elements */
List* jsonLexer(char * json);

/** Checks if a character is whitespace. Source: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/JSON#full_json_grammar */
int isValidWhitespace(char c);

/** Trim whitespaces from a json value */
char * trimValueWhitespace(char * c);

/** Infer type of json value */
JSONType jsonValueType(char * c);

/** Frees lexer list */
void freeLexer(List *list);

void testLexer();
