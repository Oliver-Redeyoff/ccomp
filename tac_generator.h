#ifndef _TAC_H_ 
#define _TAC_H_


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
    BLOCK_START_TAC_TYPE = 4,
    BLOCK_END_TAC_TYPE = 5,
    FUNCTION_CALL_TAC_TYPE = 6,
    BUILTIN_CALL_TAC_TYPE = 7,
    IF_TAC_TYPE = 8,
    RETURN_TAC_TYPE = 9,
    EXIT_PROGRAM_TAC_TYPE = 10
};

enum BUILTIN_FUNCTION_TYPE {
    PRINT_STR_BUILTIN_FUNCTION_TYPE = 1,
    PRINT_INT_BUILTIN_FUNCTION_TYPE = 2,
    INPUT_INT_BUILTIN_FUNCTION_TYPE = 3
};

enum OP_TYPE {
    NONE_OPERATION = 0,
    ADD_OPERATION = 1,
    SUBTRACT_OPERATION = 2,
    MULTIPLY_OPERATION = 3,
    DIVIDE_OPERATION = 4,
    MODULO_OPERATION = 5,
    EQUAL_OPERATION = 6,
    NOT_EQUAL_OPERATION = 7,
    GREATER_OPERATION = 8,
    GREATER_EQUAL_OPERATION = 9,
    LESS_OPERATION = 10,
    LESS_EQUAL_OPERATION = 11
};

enum BLOCK_TYPE {
    ANY_BLOCK_TYPE = 0,
    FUNCTION_BLOCK_TYPE = 1,
    IF_BLOCK_TYPE = 2,
    WHILE_BLOCK_TYPE = 3
};

enum ASSIGNEMENT_TYPE {
    NOT_DECLARATION_ASSIGNEMENT_TYPE = 0,
    INT_DECLARATION_ASSIGNEMENT_TYPE = 1,
    CLOSURE_DECLARATION_ASSIGNEMENT_TYPE =2
};


typedef struct tac_label {
    TOKEN* name;
} TAC_LABEL;

typedef struct tac_goto {
    TOKEN* name;
} TAC_GOTO;

typedef struct tac_operation {
    int assignement_type;
    int op;
    TOKEN* src1;
    TOKEN* src2;
    TOKEN* dest;
} TAC_OPERATION;

typedef struct tac_block_delimiter {
    int block_type;
    TOKEN* name;
    int arity;
    TOKEN* parent_block_name;
} TAC_BLOCK_DELIMITER;

typedef struct tac_function_call {
    TOKEN* name;
    int arity;
} TAC_FUNCTION_CALL;

typedef struct tac_builtin_call {
    int type;
    TOKEN* argument;
} TAC_BUILTIN_CALL;

typedef struct tac_if {
    TOKEN* condition_result;
    TOKEN* jump_label;
} TAC_IF;

typedef struct tac {
    int type;
    union {
        TAC_LABEL tac_label;
        TAC_GOTO tac_goto;
        TAC_OPERATION tac_operation;
        TAC_BLOCK_DELIMITER tac_block_delimiter;
        TAC_FUNCTION_CALL tac_function_call;
        TAC_BUILTIN_CALL tac_builtin_call;
        TAC_IF tac_if;
    } v;
    struct tac* next;
} TAC;

typedef struct basic_block {
    TAC* leader;
    struct basic_block* next;
} BASIC_BLOCK;


BASIC_BLOCK* generate_TAC(NODE* tree);
TOKEN* find_main_token(BASIC_BLOCK* root_BB);
void map_to_TAC(NODE* current_node, BASIC_BLOCK* current_BB);
void subdivide_basic_blocks(BASIC_BLOCK* root_BB);
void split_BB(TAC* current_TAC, BASIC_BLOCK* current_BB);
void rename_block(BASIC_BLOCK* root_BB, char* search_name, char* new_name);

void variable_declaration_template(NODE* assignement_node, BASIC_BLOCK* current_BB);
void variable_assignement_rec(NODE* current_node, TOKEN* result_temporary, BASIC_BLOCK* current_BB, int is_declaration);
TOKEN* expression_template(NODE* current_node, BASIC_BLOCK* current_BB);
TOKEN* generate_false_check(TOKEN* value_token, BASIC_BLOCK* current_BB);

void function_declaration_template(NODE* current_node, BASIC_BLOCK* current_BB);
void function_declaration_argument_retrival_rec(NODE* current_node, BASIC_BLOCK* current_BB, int arg_count);

void function_call_template(NODE* apply_node, BASIC_BLOCK* current_BB);
void function_call_argument_buffer_rec(NODE* current_node, BASIC_BLOCK* current_BB, int arg_count);
void return_template(NODE* return_node, BASIC_BLOCK* current_BB);

void if_template(NODE* if_node, BASIC_BLOCK* current_BB);
void while_template(NODE* while_node, BASIC_BLOCK* current_BB);
void continue_template(NODE* continue_node, BASIC_BLOCK* current_BB);
void break_template(NODE* break_node, BASIC_BLOCK* current_BB);

BASIC_BLOCK* append_Basic_Block(BASIC_BLOCK* current_block);
BASIC_BLOCK* insert_Basic_Block(BASIC_BLOCK* current_block);
void add_TAC(TAC* new_tac, BASIC_BLOCK* current_block);

TOKEN* new_temporary_reg();
TOKEN* new_argument_reg(int id);
TOKEN* new_if();
TOKEN* new_else();
TOKEN* new_next();
TOKEN* new_loop();

TAC* generate_label(TOKEN* name);
TAC* generate_goto(TOKEN* name);


#endif