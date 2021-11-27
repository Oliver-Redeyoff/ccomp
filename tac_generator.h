#include "nodes.h"

enum EXTRA_TOKEN_TYPE {
    TEMPORARY_IDENTIFIER = 257,
};

enum TAC_TYPE {
    LABEL_TAC_TYPE = 1,
    OPERATION_TAC_TYPE = 2,
    CALL_TAC_TYPE = 3,
    RETURN_TAC_TYPE = 4
};

enum OP_TYPE {
    NONE_OPERATION = 0,
    ADD_OPERATION = 1,
    SUBTRACT_OPERATION = 2,
    MULTIPLY_OPERATION = 3,
    DIVIDE_OPERATION = 4
};

typedef struct tac_label {
    TOKEN* name;
} TAC_LABEL;

typedef struct tac_operation {
    int op;
    TOKEN* src1;
    TOKEN* src2;
    TOKEN* dest;
} TAC_OPERATION;

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


BASIC_BLOCK* generate_TAC(NODE* tree);
void map_to_TAC(NODE* current_node, BASIC_BLOCK* current_BB);

void variable_assignement_template(NODE* current_node, BASIC_BLOCK* current_BB);
void variable_assignement_rec(NODE* current_node, TOKEN* result_temporary, BASIC_BLOCK* current_BB);
TOKEN* expression_template(NODE* current_node, BASIC_BLOCK* current_BB);
void function_declaration_template(NODE* current_node, BASIC_BLOCK* current_BB);
void return_template(NODE* return_node, BASIC_BLOCK* current_BB);

BASIC_BLOCK* create_Basic_Block(BASIC_BLOCK* current_block);
void add_TAC(TAC* new_tac, BASIC_BLOCK* current_block);

TOKEN* new_temporary();
