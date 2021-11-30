#ifndef _MIPS_H_ 
#define _MIPS_H_

#include "nodes.h"
#include "tac_generator.h"

enum LOCAl_TYPE {
    INT_LOCAL_TYPE = 1,
    CLOSURE_LOCAL_TYPE = 2
};

typedef struct local_int {
    TOKEN* name;
} LOCAL_INT;

typedef struct local_closure {
    TOKEN* name;
    struct ar* ar;
} LOCAL_CLOSURE;

typedef struct local {
    int type;
    union {
        LOCAL_INT* local_int;
        LOCAL_CLOSURE* local_closure;
    } v;
    struct local* next;
} LOCAL;

typedef struct ar {
    struct ar* fp;  // pointer to caller's Activation Record
    int pc;         // caller's Program Counter
    struct ar* sl;  // pointer to lexical parent's Activation Record
    LOCAL* locals;  // pointer to first local of scope
} AR;

void generate_MIPS(BASIC_BLOCK* root_BB);

#endif