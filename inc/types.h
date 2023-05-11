#ifndef LITHP__TYPES_H
#define LITHP__TYPES_H

#include <stddef.h>

#include <utils.h>

typedef enum { TYPE_INT, VAL_DEC, VAL_SYM, VAL_SEXPR, VAL_ERR } VAL_TYPE;
typedef enum { TAG_INT, TAG_DEC, TAG_SYM, TAG_SEXPR, TAG_QUOTE } SYMBOL;

typedef struct Result {
    VAL_TYPE type;
    union {
        long int integer;
        long double decimal;
        char* error;
        char* symbol;
        // An s-expression with size information
        struct {
            size count;
            struct Result** cell;
        } list;
    } result;
} Result;

// Constructors
Result* valResult(long);

Result* decResult(long double);

Result* symResult(char*);

Result* sexprResult();

Result* errResult(char*);

// Destructor
void resultFree(Result*);

// Utility functions
void resultListAppend(Result* list, Result* res);

#endif
