#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "C.tab.h"
#include "mips_generator.h"

BASIC_BLOCK* root_BB;
BLANK_AR* blank_ARs;

// Entry point for MIPS generation
MIPS_PROGRAM* generate_MIPS(BASIC_BLOCK* root) {

    root_BB = root;

    printf("starting generating MIPS\n");

    // first generate blank ARs
    AR* global_AR = (AR*)malloc(sizeof(AR));
    blank_ARs_loop(global_AR);

    // create new mips program
    MIPS_PROGRAM* program = (MIPS_PROGRAM*)malloc(sizeof(MIPS_PROGRAM));

    MIPS_loop(root_BB, program);

    return program;

}


// Generate blank activation records
void blank_ARs_loop(AR* global_AR) {

    TAC* current_TAC = get_next_TAC(NULL);
    AR* current_AR = global_AR;

    while (current_TAC != NULL) {

        if (current_TAC->type == BLOCK_START_TAC_TYPE) {

            // create new AR and put current AR as the lexical parent AR
            // set current_AR to this new one
            printf("Generating new blank AR\n");
            AR* new_AR = (AR*)malloc(sizeof(AR));
            new_AR->block_name = current_TAC->v.tac_block_delimiter.name;
            new_AR->sl = get_blank_AR(current_TAC->v.tac_block_delimiter.parent_block_name);
            append_blank_AR(new_AR);
            current_AR = new_AR;

        }
        else if (current_TAC->type == BLOCK_END_TAC_TYPE) {

            // set current_AR to lexical parent one
            printf("End of block, back to parent block\n");
            current_AR = current_AR->sl;

        }

        // for any declaration, add that to the current_AR as a local


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

        case LABEL_TAC_TYPE: {
            TAC_LABEL* label_tac = &current_TAC->v.tac_label;
            sprintf(new_instr->instr_str, "%s:", label_tac->name->lexeme);
            break;
        }

        case GOTO_TAC_TYPE: {
            TAC_GOTO* goto_tac = &current_TAC->v.tac_goto;
            sprintf(new_instr->instr_str, "    j %s\n", goto_tac->name->lexeme);
            break;
        }

        case OPERATION_TAC_TYPE: 
            operation_template(current_TAC);
        
        default:
            break;

    }

    return new_instr;

}

// map operation tac to mips
MIPS_INSTR* operation_template(TAC* operation_TAC) {

    MIPS_INSTR* new_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));

    return new_instr;

}


// Get next tac
TAC* get_next_TAC(TAC* search_TAC) {

    printf("Getting next tac\n");

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

// Get blank_AR with token
AR* get_blank_AR(TOKEN* search_token) {

    BLANK_AR* current_blank_AR = blank_ARs;

    while (1) {
        
        if (current_blank_AR == NULL) {
            break;
        } 
        else if (current_blank_AR->activation_record->block_name == search_token) {
            return current_blank_AR->activation_record;
        }
        else {
            current_blank_AR = current_blank_AR->next;
        }

    }

    return NULL;

}

// Add a new blank AR to list of blank ARs
void append_blank_AR(AR* new_AR) {

    BLANK_AR* new_blank_AR = (BLANK_AR*)malloc(sizeof(BLANK_AR));
    new_blank_AR->activation_record = new_AR;

    BLANK_AR* current_blank_AR = blank_ARs;

    while (1) {

        if (current_blank_AR == NULL) {
            current_blank_AR = new_blank_AR;
            break;
        } else {
            current_blank_AR = current_blank_AR->next;
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