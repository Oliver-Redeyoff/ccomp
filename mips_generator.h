#ifndef _MIPS_H_ 
#define _MIPS_H_


#include "nodes.h"
#include "tac_generator.h"


enum LOCAl_TYPE {
    INT_LOCAL_TYPE = 1,
    CLOSURE_LOCAL_TYPE = 2
};

enum MIPS_DATA_TYPE {
    TEXT_DATA_TYPE = 1
};


typedef struct local {
    int type;
    TOKEN* name;
    TOKEN* closure_label;
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

typedef struct mips_data {
    char label[500];
    int type;
    char text[500];
    struct mips_data* next;
} MIPS_DATA;

typedef struct mips_program {
    char* name;
    MIPS_INSTR* instructions;
    MIPS_DATA* data;
} MIPS_PROGRAM;


MIPS_PROGRAM* generate_MIPS(BASIC_BLOCK* root_BB);
void MIPS_loop(BASIC_BLOCK* root_BB);
MIPS_INSTR* map_to_MIPS(TAC* current_TAC);
AR* generate_AR(TAC* block_start_TAC);

MIPS_INSTR* block_start_MIPS_template(TAC* block_start_TAC);
MIPS_INSTR* block_end_MIPS_template(TAC* block_end_TAC);
MIPS_INSTR* function_call_MIPS_template(TAC* function_call_TAC);
MIPS_INSTR* builtin_call_MIPS_template(TAC* builtin_call_TAC);
MIPS_INSTR* return_MIPS_template(TAC* return_TAC);
MIPS_INSTR* operation_MIPS_template(TAC* operation_TAC);

AR* get_AR(TOKEN* search_token);
int get_AR_size(AR* activation_record);
AR* get_containing_AR(TAC* search_TAC, int block_type);
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
void append_instr_to_program(MIPS_INSTR* new_instr);
void append_data(MIPS_DATA* new_data, MIPS_DATA* data_chain);
char* append_data_to_program(MIPS_DATA* new_data);


#endif