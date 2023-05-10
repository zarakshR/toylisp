typedef enum { VALUE, ERROR } RES_TYPE;
typedef enum { DIV_ZERO, ARG_COUNT, INT_FLOW } ERR_CODE;

typedef struct {
    RES_TYPE type;
    union {
        long value;
        ERR_CODE error;
    } result;
} Result;

typedef enum { NUM, EXPR, ROOT } SYMBOL;

typedef enum { ADD, SUB, MUL, DIV, POW, MOD, MIN, MAX } OP;
