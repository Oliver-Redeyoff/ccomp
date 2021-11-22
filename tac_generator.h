#include "nodes.h"

enum TAC_TYPE {
    LABEL_TAC_TYPE = 1,
    INSTRUCTION_TAC_TYPE = 2,
    CALL_TAC_TYPE = 3
};

enum OP_TYPE {
    ADD = 1,
    SUBTRACT = 2,
    MULTIPLY = 3,
    DIVIDE = 4
};

typedef struct tac_operation {
    int op;
    TOKEN* src1;
    TOKEN* src2;
    TOKEN* dest;
} TAC_OPERATION;

typedef struct tac_label {
    TOKEN* name;
} TAC_LABEL;

typedef struct tac_call {
    TOKEN* name;
    int arity;
} TAC_CALL;

typedef struct tac_return {
    TOKEN* name;
} TAC_RETURN;

typedef struct tac {
    int type;
    union {
        TAC_LABEL tac_label;
        TAC_OPERATION tac_operation;
        TAC_CALL tac_call;
        TAC_RETURN tac_return;
    } v;
    struct tac* next;
} TAC;

typedef struct basic_block {
    TAC* leader;
    struct basic_block* next;
} BASIC_BLOCK;


typedef struct temporary {
    int id;
    TOKEN* token;
} TEMPORARY;


BASIC_BLOCK* generate_TAC(NODE* tree);
void map_to_TAC(NODE* current_node, BASIC_BLOCK* current_BB);

void variable_assignement_template(NODE* current_node, BASIC_BLOCK* current_BB);
void expression_template(NODE* current_node, BASIC_BLOCK* current_BB);
void function_declaration_template(NODE* current_node, BASIC_BLOCK* current_BB);

BASIC_BLOCK* create_Basic_Block(BASIC_BLOCK* current_block);
void add_TAC(TAC* new_tac, BASIC_BLOCK* current_block);

TEMPORARY* new_temporary(TOKEN* token);
