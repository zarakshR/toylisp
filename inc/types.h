#ifndef TOYLISP__TYPES_H
#define TOYLISP__TYPES_H

#include "util.h"

// Represents one of the specific internal datatypes
typedef enum { TYPE_INT, TYPE_SYM, TYPE_ERR, TYPE_SEXPR, TYPE_QUOTE } TYPE;

// Represents one of the specific internal operators
typedef enum { ADD, SUB, MUL, DIV, POW, MOD, MIN, MAX } OP;

// Represents a tag in the parse tree
typedef enum { TAG_INT, TAG_SYM, TAG_SEXPR, TAG_QUOTE } TAG;

// Core data type. All elements we interact with should be a Result type.
// The type field stores the type of the element as a TYPE. Depending on the
// type, any one of the types in the union stores the actual data we are
// interested in. list is a special type that represents a list of Results. list
// is to be used for accessing the elements of both s-expressions and quotes.

// It is an error to try and use an element of a type not indicated by the
// enclosing Result.
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

// Constructors - Return pointer to a new instance of the respective datatype
Result* valResult(const long);
Result* symResult(const char* const);
Result* sexprResult();
Result* errResult(const char* const);
Result* quoteResult();

// Destructor - Delete an element. lists have their elements recursively deleted
void resultFree(Result*);

// Utility functions
void resultListAppend(Result* const list, Result* const res);
void printResult(const Result* const);
OP parseSym(const char*);

// Get the n'th element of sexpr. Has type Result*
#define ELEM_AT(SEXPRESSION, INDEX) SEXPRESSION->result.list.cell[INDEX]

// Get the number of elements in sexpr. Has type size
#define SIZE_OF(SEXPRESSION) SEXPRESSION->result.list.count

#endif
