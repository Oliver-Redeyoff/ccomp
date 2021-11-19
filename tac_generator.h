#include "nodes.h"

enum TAC_TYPE {
    CALL = 1,
    LABEL = 2,
    INSTRUCTION = 3
};

typedef struct call {
    TOKEN* name; 
    int arity;
} CALL;

typedef struct label {
    TOKEN* name;
} LABEL;

typedef struct instruction {
    int op;
    TOKEN* src1;
    TOKEN* src2;
    TOKEN* dest;
} INSTRUCTION;

typedef struct tac {
    int type;
    union {
        CALL call;
        LABEL label;
        INSTRUCTION instruction;
    } v;
    struct tac* next;
} TAC;

typedef struct basic_block
{
    TAC* leader;
    struct basic_block* next;
} BASIC_BLOCK;

BASIC_BLOCK* generate_TAC(NODE* tree);
