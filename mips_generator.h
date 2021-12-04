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
    TOKEN* block_name;
    struct ar* fp;  // pointer to caller's Activation Record
    int pc;         // caller's Program Counter
    struct ar* sl;  // pointer to lexical parent's Activation Record
    LOCAL* locals;  // pointer to first local of scope
} AR;

typedef struct blank_ar {
    AR* activation_record;
    struct blank_ar* next;
} BLANK_AR;

typedef struct mips_instr {
    char instr_str[500];
    struct mips_instr* next;
} MIPS_INSTR;

typedef struct mips_program {
    char* name;
    MIPS_INSTR* instructions;
} MIPS_PROGRAM;

MIPS_PROGRAM* generate_MIPS(BASIC_BLOCK* root_BB);
void blank_ARs_loop(AR* global_AR);
void MIPS_loop(BASIC_BLOCK* root_BB, MIPS_PROGRAM* program);
MIPS_INSTR* map_to_MIPS(TAC* current_TAC);

MIPS_INSTR* operation_template(TAC* operation_TAC);

TAC* get_next_TAC(TAC* search_TAC);
AR* get_blank_AR(TOKEN* search_token);

void append_blank_AR(AR* new_AR);
void append_instr(MIPS_INSTR* new_instr, MIPS_PROGRAM* program);

#endif