#ifndef TOYLISP_UTIL_H
#define TOYLISP_UTIL_H

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <readline/history.h>
#include <readline/readline.h>

#include <utils.h>

// Common error strings. Maybe remove later?
#define DIV_ZERO  "DIVISION BY ZERO"
#define ARG_COUNT "WRONG NO. OF ARGUMENTS"
#define INT_FLOW  "INTEGER OVER/UNDERFLOW"

// For unrecoverable errors
#define PANIC(msg) assert(0 && msg)

#endif
