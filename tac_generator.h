#include "nodes.h"

enum REGISTER_TOKEN_TYPE {
    RETURN_REG_IDENTIFIER = 255,
    ARGUMENT_REG_IDENTIFIER = 256,
    TEMPORARY_REG_IDENTIFIER = 257
};

enum TAC_TYPE {
    LABEL_TAC_TYPE = 1,
    GOTO_TAC_TYPE = 2,
    OPERATION_TAC_TYPE = 3,
    FUNCTION_START_TAC_TYPE = 4,
    FUNCTION_END_TAC_TYPE = 5,
    FUNCTION_CALL_TAC_TYPE = 6,
    IF_TAC_TYPE = 7
};

enum OP_TYPE {
    NONE_OPERATION = 0,
    ADD_OPERATION = 1,
    SUBTRACT_OPERATION = 2,
    MULTIPLY_OPERATION = 3,
    DIVIDE_OPERATION = 4,
    EQUAL_OPERATION = 5,
    NOT_EQUAL_OPERATION = 6,
    LESS_OPERATION = 7,
    LESS_EQUAL_OPERATION = 8
};

typedef struct tac_label {
    TOKEN* name;
} TAC_LABEL;

typedef struct tac_goto {
    TOKEN* name;
} TAC_GOTO;

typedef struct tac_operation {
    int is_declaration;
    int op;
    TOKEN* src1;
    TOKEN* src2;
    TOKEN* dest;
} TAC_OPERATION;

typedef struct tac_function_delimiter {
    TOKEN* name;
    int arity;
} TAC_FUNCTION_DELIMITER;

typedef struct tac_function_call {
    TOKEN* name;
    int arity;
} TAC_FUNCTION_CALL;

typedef struct tac_if {
    TOKEN* condition_result;
    TOKEN* else_label;
} TAC_IF;

typedef struct tac {
    int type;
    union {
        TAC_LABEL tac_label;
        TAC_GOTO tac_goto;
        TAC_OPERATION tac_operation;
        TAC_FUNCTION_DELIMITER tac_function_delimiter;
        TAC_FUNCTION_CALL tac_function_call;
        TAC_IF tac_if;
    } v;
    struct tac* next;
} TAC;

typedef struct basic_block {
    TAC* leader;
    struct basic_block* next;
    struct basic_block* lexical_parent;
} BASIC_BLOCK;


BASIC_BLOCK* generate_TAC(NODE* tree);
void map_to_TAC(NODE* current_node, BASIC_BLOCK* current_BB);

void variable_declaration_template(NODE* assignement_node, BASIC_BLOCK* current_BB);
void variable_assignement_rec(NODE* current_node, TOKEN* result_temporary, BASIC_BLOCK* current_BB, int is_declaration);
TOKEN* expression_template(NODE* current_node, BASIC_BLOCK* current_BB);

void function_declaration_template(NODE* current_node, BASIC_BLOCK* current_BB);
int function_declaration_argument_count_rec(NODE* current_node);
void function_declaration_argument_retrival_rec(NODE* current_node, BASIC_BLOCK* current_BB, int arg_count);

void function_call_template(NODE* apply_node, BASIC_BLOCK* current_BB);
int function_call_argument_count_rec(NODE* current_node);
void function_call_argument_buffer_rec(NODE* current_node, BASIC_BLOCK* current_BB, int arg_count);
void return_template(NODE* return_node, BASIC_BLOCK* current_BB);
TAC_FUNCTION_DELIMITER* get_basic_block_function_start(BASIC_BLOCK* current_BB);

void if_template(NODE* if_node, BASIC_BLOCK* current_BB);

BASIC_BLOCK* append_Basic_Block(BASIC_BLOCK* current_block);
BASIC_BLOCK* insert_Basic_Block(BASIC_BLOCK* current_block);
void add_TAC(TAC* new_tac, BASIC_BLOCK* current_block);

TOKEN* new_temporary_reg();
TOKEN* new_argument_reg(int id);
TOKEN* new_else();
TOKEN* new_next();

TAC* generate_label(TOKEN* name);
TAC* generate_goto(TOKEN* name);
