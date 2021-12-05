#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "C.tab.h"
#include "mips_generator.h"

BASIC_BLOCK* root_BB;
AR* AR_list;

// Entry point for MIPS generation
MIPS_PROGRAM* generate_MIPS(BASIC_BLOCK* root) {

    root_BB = root;

    printf("starting generating MIPS\n");

    // first generate ARs
    AR* global_AR = (AR*)malloc(sizeof(AR));
    AR_list = global_AR;
    generate_ARs(global_AR);

    // create new mips program
    MIPS_PROGRAM* program = (MIPS_PROGRAM*)malloc(sizeof(MIPS_PROGRAM));

    MIPS_loop(root_BB, program);

    return program;

}


// Generate blank activation records
void generate_ARs(AR* global_AR) {

    TAC* current_TAC = get_next_TAC(NULL);
    AR* current_AR = global_AR;

    while (current_TAC != NULL) {

        if (current_TAC->type == BLOCK_START_TAC_TYPE) {

            // create new AR and put current AR as the lexical parent AR
            // set current_AR to this new one
            printf("New activation record\n");
            AR* new_AR = (AR*)malloc(sizeof(AR));
            new_AR->block_name = current_TAC->v.tac_block_delimiter.name;
            new_AR->sl = NULL;
            append_AR(new_AR);
            current_AR = new_AR;

        }
        else if (current_TAC->type == BLOCK_END_TAC_TYPE) {

            // set current_AR to lexical parent one
            current_AR = current_AR->sl;

        }

        // for any declaration, add that to the current_AR as a local
        if (current_TAC->type == OPERATION_TAC_TYPE) {

            TAC_OPERATION* operation = &current_TAC->v.tac_operation;

            if (operation->is_declaration == 1) {
                printf("New local\n");
                // create int local
                LOCAL_INT* new_int_local = (LOCAL_INT*)malloc(sizeof(LOCAL_INT));
                new_int_local->name = operation->dest;
                // create new local
                LOCAL* new_local = (LOCAL*)malloc(sizeof(LOCAL));
                new_local->type = INT_LOCAL_TYPE;
                new_local->v.local_int = new_int_local;
                add_local(new_local, current_AR);
            }

        }


        current_TAC = get_next_TAC(current_TAC);

    }

    return;

}


// Loop over TAC and map to MIPS
void MIPS_loop(BASIC_BLOCK* root_BB, MIPS_PROGRAM* program) {

    TAC* current_TAC = get_next_TAC(NULL);

    while (current_TAC != NULL) {

        MIPS_INSTR* new_instr = map_to_MIPS(current_TAC);
        append_instr(new_instr, program);

        current_TAC = get_next_TAC(current_TAC);

    }

    return;

}

// map a specific TAC to MIPS
MIPS_INSTR* map_to_MIPS(TAC* current_TAC) {

    int tac_type = current_TAC->type;
    MIPS_INSTR* new_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));

    switch (tac_type) {

        case BLOCK_START_TAC_TYPE: {
            new_instr = block_start_template(current_TAC);
            break;
        }

        case LABEL_TAC_TYPE: {
            TAC_LABEL* label_tac = &current_TAC->v.tac_label;
            sprintf(new_instr->instr_str, "%s:", label_tac->name->lexeme);
            break;
        }

        case GOTO_TAC_TYPE: {
            TAC_GOTO* goto_tac = &current_TAC->v.tac_goto;
            sprintf(new_instr->instr_str, " j %s\n", goto_tac->name->lexeme);
            break;
        }

        case FUNCTION_CALL_TAC_TYPE: {
            TAC_FUNCTION_CALL* function_call_tac = &current_TAC->v.tac_function_call;
            sprintf(new_instr->instr_str, " jal %s\n", function_call_tac->name->lexeme);
            break;
        }

        case OPERATION_TAC_TYPE: 
            new_instr = operation_template(current_TAC);
            break;
        
        default:
            break;

    }

    return new_instr;

}

// map start of block to mips
MIPS_INSTR* block_start_template(TAC* block_start_TAC) {

    MIPS_INSTR* label_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));

    // get block's AR
    AR* block_AR = get_AR(block_start_TAC->v.tac_block_delimiter.name);

    sprintf(label_instr->instr_str, "%s:", block_start_TAC->v.tac_block_delimiter.name->lexeme);

    return label_instr;

}

// map operation tac to mips
MIPS_INSTR* operation_template(TAC* operation_TAC) {

    MIPS_INSTR* new_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));

    TAC_OPERATION* operation = &operation_TAC->v.tac_operation;
    TOKEN* dest = operation->dest;
    TOKEN* src1 = operation->src1;
    TOKEN* src2 = operation->src2;

    switch (operation->op) {

        case NONE_OPERATION:
            if ((dest->type == TEMPORARY_REG_IDENTIFIER || dest->type == ARGUMENT_REG_IDENTIFIER || dest->type == RETURN_REG_IDENTIFIER)
                && src1->type == CONSTANT) {
                sprintf(new_instr->instr_str, " li %s, %d", get_register_name(dest), src1->value);
                break;
            }
            if ((dest->type == TEMPORARY_REG_IDENTIFIER || dest->type == ARGUMENT_REG_IDENTIFIER || dest->type == RETURN_REG_IDENTIFIER)
                && (src1->type == TEMPORARY_REG_IDENTIFIER || src1->type == ARGUMENT_REG_IDENTIFIER || src1->type == RETURN_REG_IDENTIFIER)) {
                sprintf(new_instr->instr_str, " move %s, %s", get_register_name(dest), get_register_name(src1));
            }
            break;
        
        default:
            break;

    }

    return new_instr;

}


// Get next tac
TAC* get_next_TAC(TAC* search_TAC) {

    BASIC_BLOCK* current_BB = root_BB;
    TAC* current_TAC = current_BB->leader;

    int return_next = 0;

    while (1) {

        current_TAC = current_BB->leader;
        if (search_TAC == NULL) {
            return current_TAC;
        }

        while (1) {

            if (return_next == 1) {
                return current_TAC;
            }

            if (current_TAC == search_TAC) {
                return_next = 1;
            }

            if (current_TAC->next == NULL) {
                break;
            } else {
                current_TAC = current_TAC->next;
            }

        }

        if (current_BB->next == NULL) {
            break;
        } else {
            current_BB = current_BB->next;
        }

    }

    return NULL;

}

// gets string for name of a register in MIPS
char* get_register_name(TOKEN* register_token) {
    
    char register_name[5];

    if (register_token->type == TEMPORARY_REG_IDENTIFIER) {
        register_name[0] = '$';
        register_name[1] = 't';
        register_name[2] = register_token->value+'0';
    }
    else if (register_token->type == ARGUMENT_REG_IDENTIFIER) {
        register_name[0] = '$';
        register_name[1] = 'a';
        register_name[2] = register_token->value+'0';
    }
    else if (register_token->type == RETURN_REG_IDENTIFIER) {
        register_name[0] = '$';
        register_name[1] = 's';
        register_name[2] = register_token->value+'0';
    }

    return strdup(register_name);

}

// Lookup ar using token
AR* get_AR(TOKEN* search_token) {

    AR* current_AR = AR_list;

    while (1) {

        if (current_AR == NULL) {
            return NULL;
        }

        if (current_AR->block_name == search_token) {
            return current_AR;
        } else {
            current_AR = current_AR->next;
        }

    }

    return NULL;

}

// Add a new blank AR to list of blank ARs
void append_AR(AR* new_AR) {

    if (AR_list == NULL) {
        AR_list = new_AR;
    }

    AR* current_AR = AR_list;

    while (1) {

        if (current_AR->next == NULL) {
            current_AR->next = new_AR;
            break;
        } else {
            current_AR = current_AR->next;
        }

    }

    return;

}

// Add local to activation record
void add_local(LOCAL* new_local, AR* current_AR) {

    LOCAL* current_local = current_AR->locals;

    if (current_local == NULL) {
        current_AR->locals = new_local;
        return;
    }

    while (1) {

        if (current_local->next == NULL) {
            current_local->next = new_local;
            break;
        } else {
            current_local = current_local->next;
        }

    }

    return;

}

// Add new MIPS instruction to linked list
void append_instr(MIPS_INSTR* new_instr, MIPS_PROGRAM* program) {

    MIPS_INSTR* current_instr = program->instructions;

    if (current_instr == NULL) {
        program->instructions = new_instr;
        return;
    }

    while (1) {

        if (current_instr->next == NULL) {
            current_instr->next = new_instr;
            break;
        } else {
            current_instr = current_instr->next;
        }

    }

    return;

}