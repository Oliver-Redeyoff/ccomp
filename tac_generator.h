#include "nodes.h"

enum TAC_TYPE {
    CALL_TAC = 1,
    LABEL_TAC = 2,
    INSTRUCTION_TAC = 3
};

enum OP_TYPE {
    ADD = 1,
    SUBTRACT = 2,
    MULTIPLY = 3,
    DIVIDE = 4
};

typedef struct operation {
    int op;
    TOKEN* src1;
    TOKEN* src2;
    TOKEN* dest;
} OPERATION;

typedef struct label {
    TOKEN* name;
} LABEL;

typedef struct call {
    TOKEN* name;
    int arity;
} CALL;

typedef struct tac {
    int type;
    union {
        OPERATION operation;
        CALL call;
    } v;
    struct tac* next;
} TAC;

typedef struct basic_block {
    LABEL* label;
    TAC* leader;
    struct basic_block* next;
} BASIC_BLOCK;


typedef struct temporary {
    int id;
    TOKEN* token;
} TEMPORARY;


BASIC_BLOCK* generate_TAC(NODE* tree);
void map_to_TAC(NODE* current_node, BASIC_BLOCK* current_BB);

TAC* variable_assignement_template(NODE* current_node);
TAC* expression_template(NODE* current_node);

BASIC_BLOCK* create_Basic_Block(BASIC_BLOCK* current_block);
void add_TAC(TAC* new_tac, BASIC_BLOCK* current_block);

TEMPORARY* new_temporary(TOKEN* token);
