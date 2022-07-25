#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../headers/internals.h"

void _JsonValueLexer(List* list, char * json, int * position);
void _JsonObjectLexer(List* list, char * json, int * position);
void _JsonArrayLexer(List* list, char * json, int * position);
void _JsonStringLexer(List* list, char * json, int * position);
void _JsonNumberLexer(List* list, char * json, int * position);
void _JsonBooleanLexer(List* list, char * json, int * position);
void _JsonNullLexer(List* list, char * json, int * position);

size_t trimLeft(char *out, size_t len, const char *str);
size_t trim(char *out, size_t len, const char *str);

List* jsonLexer(char * json) {
    List* list = createList();
    int i = 0;
    _JsonValueLexer(list, json, &i);
    return list;
}

void _JsonValueLexer(List* list, char * json, int * position) {
    int whitespace, jsonLen;
    char *processed;
    JSONType valueType;

    jsonLen = strlen(json);
    processed = (char*) malloc(jsonLen * sizeof(char));
    whitespace = trimLeft(processed, jsonLen, json);
    valueType = jsonValueType(processed);
    *position += whitespace;


    if (valueType == null)
        _JsonNullLexer(list, processed, position);
    else if (valueType == string)
        _JsonStringLexer(list, processed, position);
    else if (valueType == number)
        _JsonNumberLexer(list, processed, position);
    else if (valueType == object)
        _JsonObjectLexer(list, processed, position);
    else if (valueType == array)
        _JsonArrayLexer(list, processed, position);
    else if (valueType == boolean)
        _JsonBooleanLexer(list, processed, position);
}

void _JsonObjectLexer(List* list, char * json, int * position) {
    int i, strLen, started, numItems, commaBlock, valueBlock;
    char * token, * processed;

    strLen = strlen(json);
    processed = (char*) malloc(strLen * sizeof(char));
    token = (char*) malloc(2 * sizeof(char));
    token[0] = 0;
    strcat(token, "{");
    addToList(list, token);
    *position += 1;

    started = 0;
    numItems = 0;
    commaBlock = 0;
    valueBlock = 0;
    for (i = 0; i < strLen; i++) {
        if (started && !commaBlock && !valueBlock) {
            if (numItems) {
                token = (char*) malloc(2 * sizeof(char));
                token[0] = 0;
                strcat(token, ",");
                addToList(list, token);
            }

            _JsonValueLexer(list, json + i, &i);
            token = (char*) malloc(2 * sizeof(char));
            token[0] = 0;
            strcat(token, ":");
            addToList(list, token);
            valueBlock = 1;
            commaBlock = 1;
        }

        if (started && !commaBlock && valueBlock) {
            _JsonValueLexer(list, json + i, &i);
            valueBlock = 0;
            commaBlock = 1;
            numItems += 1;
        }

        if (commaBlock && (json[i] == ',' || json[i] == ':')) {
            commaBlock = 0;
        }

        if (!started && json[i] == '{') {
            started = 1;
        } else if (started && json[i] == '}') {
            break;
        }
    }


    token = (char*) malloc(2 * sizeof(char));
    token[0] = 0;
    strcat(token, "}");
    addToList(list, token);
    free(processed);
    *position += i;
}

void _JsonArrayLexer(List* list, char * json, int * position) {
    int i, strLen, started, numItems, commaBlock;
    char * token, * processed;

    strLen = strlen(json);
    processed = (char*) malloc(strLen * sizeof(char));
    token = (char*) malloc(2 * sizeof(char));
    token[0] = 0;
    strcat(token, "[");
    addToList(list, token);
    *position += 1;

    started = 0;
    numItems = 0;
    commaBlock = 0;
    for (i = 0; i < strLen; i++) {
        if (started && !commaBlock) {
            if (numItems) {
                token = (char*) malloc(2 * sizeof(char));
                token[0] = 0;
                strcat(token, ",");
                addToList(list, token);
            }

            _JsonValueLexer(list, json + i, &i);
            commaBlock = 1;
            numItems += 1;
        }

        if (commaBlock && json[i] == ',') {
            commaBlock = 0;
        }

        if (!started && json[i] == '[') {
            started = 1;
        } else if (started && json[i] == ']') {
            break;
        }
    }

    token = (char*) malloc(2 * sizeof(char));
    token[0] = 0;
    strcat(token, "]");
    addToList(list, token);
    free(processed);
    *position += i;
}

void _JsonStringLexer(List* list, char * json, int * position) {
    int block, i;
    char *processed, *token;

    block = 0;
    i = 0;
    for (i = 0; i < strlen(json); i++) {
        if (i && !block && json[i] == '"')
            break;

        if (block)
            block = 0;
        if (json[i] == '\\')
            block = 1;
    }

    processed = (char*) malloc((i) * sizeof(char));
    memcpy(processed, json + 1, i - 1);
    processed[i - 1] = '\0';

    token = (char*) malloc((i + 2) * sizeof(char));
    token[0] = 0;
    strcat(token, "\"");
    strcat(token, processed);
    free(processed);
    strcat(token, "\"");

    addToList(list, token);
    *position += 2 + strlen(processed);
}

void _JsonNumberLexer(List* list, char * json, int * position) {
    int i;
    char * processed;

    i = 0;
    for (i = 0; i < strlen(json); i++) {
        if (!(json[i] == '.'    ||
            json[i] == 'E'      ||
            json[i] == 'e'      ||
            json[i] == '-'      ||
            json[i] == '+') && 
            !(json[i] >= '0' && json[i] <= '9')) {
            break;
        }
    }

    processed = (char*) malloc((i + 1) * sizeof(char));
    memcpy(processed, json, i);
    processed[i] = '\0';
    addToList(list, processed);
    *position += i;
}

void _JsonBooleanLexer(List* list, char * json, int * position) {
    char *boolean;

    if (json[0] == 'f') {
        boolean = (char*) malloc(6 * sizeof(char));
        boolean[0] = 0;
        strcat(boolean, "false");
    } else if (json[0] == 't') {
        boolean = (char*) malloc(6 * sizeof(char));
        boolean[0] = 0;
        strcat(boolean, "true");
    } else {
        return;
    }

    addToList(list, boolean);
    *position += strlen(boolean);
}

void _JsonNullLexer(List* list, char * json, int * position) {
    char *nullString;

    if (json[0] == 'n') {
        nullString = (char*) malloc(5 * sizeof(char));
        nullString[0] = 0;
        strcat(nullString, "null");
        addToList(list, nullString);
    }

    *position += strlen(nullString);
}

int isValidWhitespace(char c) {
    return  c == ' '    ||      // space
            c == '\t'   ||      // tab space
            c == '\r'   ||      // carriage return
            c == '\n'   ||      // carriage return
            c == '\f';          // form feed
}

JSONType jsonValueType(char * c) {
    switch(c[0]) {
      case '{' :
         return object;
      case '[' :
         return array;
      case '"' :
         return string;
      case 't' :
      case 'f' :
         return boolean;
      case 'n' :
         return null;
   }

   if ((c[0] >= '0' && c[0] <= '9') || c[0] == '-') return number;
   return none;
}

size_t trimLeft(char *out, size_t len, const char *str) {
  if(len == 0)
    return 0;

  const char *start = str;
  const char *end;
  size_t out_size;

  // Trim leading space
  while(isValidWhitespace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
  {
    *out = 0;
    return 1;
  }

  // Trim trailing space
  end = str + strlen(str) + 1;

  // Set output size to minimum of trimmed string length and buffer size minus 1
  out_size = (end - str) < len - 1 ? (end - str) : len - 1;

  // Copy trimmed string and add null terminator
  memcpy(out, str, out_size);
  out[out_size] = 0;


  return str - start;
}

void freeLexer(List *tokens) {
    for (int i = 0; i < tokens->length; i++)
        free(tokens->array[i]);
}

void testLexer() {
    int i = 0;
    char * text = "{ \"abcd\" : 12 , \"b\" : 13, \"z\": [1, 2, 3 ] }";
    // char * text = "\"a\" : 12 , \"b\" : 13 }";
    List* tokens = createList();
    _JsonObjectLexer(tokens, text, &i);
    printf("%i <-> %lu\n", i, strlen(text));

    for (int i = 0; i < tokens->length; i++) {
        printf("'%s'    ", (char*)tokens->array[i]);
    }
    printf("\n");

    freeLexer(tokens);
    freeList(tokens);
}

