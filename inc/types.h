#ifndef LITHP__TYPES_H
#define LITHP__TYPES_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <utils.h>

typedef enum {
    TYPE_INT,
    TYPE_DEC,
    TYPE_SYM,
    TYPE_ERR,
    TYPE_SEXPR,
    TYPE_QUOTE
} TYPE;

typedef enum { ADD, SUB, MUL, DIV, POW, MOD, MIN, MAX } OP;
typedef enum { TAG_INT, TAG_DEC, TAG_SYM, TAG_SEXPR, TAG_QUOTE } SYMBOL;

typedef struct Result {
    TYPE type;
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
Result* valResult(const long);

Result* decResult(const long double);

Result* symResult(const char* const);

Result* sexprResult();

Result* errResult(const char* const);

Result* quoteResult();

// Destructor
void resultFree(Result*);

// Utility functions
void resultListAppend(Result* const list, Result* const res);

#endif
