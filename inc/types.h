#ifndef TOYLISP__TYPES_H
#define TOYLISP__TYPES_H

#include "util.h"

typedef enum { TYPE_INT, TYPE_SYM, TYPE_ERR, TYPE_SEXPR, TYPE_QUOTE } TYPE;
typedef enum { ADD, SUB, MUL, DIV, POW, MOD, MIN, MAX } OP;
typedef enum { TAG_INT, TAG_SYM, TAG_SEXPR, TAG_QUOTE } TAG;

typedef struct Result {
    TYPE type;
    union {
        long int integer;
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
Result* symResult(const char* const);
Result* sexprResult();
Result* errResult(const char* const);
Result* quoteResult();

// Destructor
void resultFree(Result*);

// Utility functions
void resultListAppend(Result* const list, Result* const res);
void printResult(const Result* const);
OP parseSym(const char*);

#endif
