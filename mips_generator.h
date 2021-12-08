#ifndef _MIPS_H_ 
#define _MIPS_H_

#include "nodes.h"
#include "tac_generator.h"

enum LOCAl_TYPE {
    INT_LOCAL_TYPE = 1,
    CLOSURE_LOCAL_TYPE = 2
};

typedef struct local {
    int type;
    TOKEN* name;
    struct local* next;
} LOCAL;

typedef struct ar {
    TOKEN* block_name;
    struct ar* lexical_parent_AR;  // pointer to lexical parent's Activation Record
    LOCAL* locals;  // pointer to first local of scope
    struct ar* next;
} AR;

typedef struct mips_instr {
    char instr_str[500];
    struct mips_instr* next;
} MIPS_INSTR;

typedef struct mips_program {
    char* name;
    MIPS_INSTR* instructions;
} MIPS_PROGRAM;

MIPS_PROGRAM* generate_MIPS(BASIC_BLOCK* root_BB);
void MIPS_loop(BASIC_BLOCK* root_BB, MIPS_PROGRAM* program);
MIPS_INSTR* map_to_MIPS(TAC* current_TAC);

MIPS_INSTR* block_start_template(TAC* block_start_TAC);
MIPS_INSTR* function_call_MIPS_template(TAC* function_call_TAC);
MIPS_INSTR* return_MIPS_template(TAC* return_TAC);
MIPS_INSTR* operation_template(TAC* operation_TAC);

AR* generate_AR(TAC* block_start_TAC);
AR* get_AR(TOKEN* search_token);
int get_AR_size(AR* activation_record);
AR* get_containing_AR(TAC* search_TAC);
AR* get_containing_function_AR(TAC* search_TAC);
MIPS_INSTR* get_AR_address(AR* search_AR, AR* initial_AR);
MIPS_INSTR* get_local_address(TOKEN* search_token, AR* initial_AR);
int get_local_type(TOKEN* search_token, AR* initial_AR);
TAC* get_next_TAC(TAC* search_TAC);
int is_register(TOKEN* token);
char* get_register_name(TOKEN* register_token);
char* new_getPC();

void append_AR(AR* new_AR);
void add_local(LOCAL* new_local, AR* current_AR);
void append_instr(MIPS_INSTR* new_instr, MIPS_INSTR* instr_chain);
void append_instr_to_program(MIPS_INSTR* new_instr, MIPS_PROGRAM* program);

#endif