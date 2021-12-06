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

    // create new mips program
    MIPS_PROGRAM* program = (MIPS_PROGRAM*)malloc(sizeof(MIPS_PROGRAM));

    MIPS_loop(root_BB, program);

    return program;

}

// Loop over TAC and map to MIPS
void MIPS_loop(BASIC_BLOCK* root_BB, MIPS_PROGRAM* program) {

    TAC* current_TAC = get_next_TAC(NULL);

    while (current_TAC != NULL) {

        MIPS_INSTR* new_instr = map_to_MIPS(current_TAC);
        append_instr_to_program(new_instr, program);

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
            sprintf(new_instr->instr_str, "  j %s\n", goto_tac->name->lexeme);
            break;
        }

        case FUNCTION_CALL_TAC_TYPE:
            new_instr = function_call_MIPS_template(current_TAC);
            break;

        case RETURN_TAC_TYPE:
            new_instr = return_MIPS_template(current_TAC);
            break;

        case OPERATION_TAC_TYPE: 
            new_instr = operation_template(current_TAC);
            break;
        
        default:
            break;

    }

    return new_instr;

}

// Map start of block to MIPS
MIPS_INSTR* block_start_template(TAC* block_start_TAC) {

    TAC_BLOCK_DELIMITER* block_delimiter = &block_start_TAC->v.tac_block_delimiter;
    
    AR* block_AR = generate_AR(block_start_TAC);
    int AR_size = get_AR_size(block_AR);

    MIPS_INSTR* initial_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));

    // add initial label
    if (block_delimiter->block_type == FUNCTION_BLOCK_TYPE) {
        MIPS_INSTR* label_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(label_instr->instr_str, "%s:", block_start_TAC->v.tac_block_delimiter.name->lexeme);
        append_instr(label_instr, initial_instr);
    }

    // first if function block need to store lexical parent pointer before we overwrite it
    if (block_delimiter->block_type == FUNCTION_BLOCK_TYPE) {
        MIPS_INSTR* caller_SL_temp_store_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(caller_SL_temp_store_instr->instr_str, "  move $t0, $a0");
        append_instr(caller_SL_temp_store_instr, initial_instr);
    }

    // load AR size as argument for syscall 
    MIPS_INSTR* sbreak_argument_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(sbreak_argument_instr->instr_str, "  li $a0, %d", AR_size);
    append_instr(sbreak_argument_instr, initial_instr);

    // sbreak syscall 
    MIPS_INSTR* sbreak_syscall_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(sbreak_syscall_instr->instr_str, "  syscall sbrk");
    append_instr(sbreak_syscall_instr, initial_instr);

    // put caller's AR in new AR
    MIPS_INSTR* caller_AR_store_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(caller_AR_store_instr->instr_str, "  lw $fp, 0($v0)");
    append_instr(caller_AR_store_instr, initial_instr);

    // put caller's return PC and lexical parent scope in new AR if function
    if (block_delimiter->block_type == FUNCTION_BLOCK_TYPE) {
        MIPS_INSTR* caller_PC_store_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(caller_PC_store_instr->instr_str, "  lw $ra, 4($v0)");
        append_instr(caller_PC_store_instr, initial_instr);

        MIPS_INSTR* caller_SL_store_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(caller_SL_store_instr->instr_str, "  lw $t0, 8($v0)");
        append_instr(caller_SL_store_instr, initial_instr);
    }

    // put lexical parent scope in new AR if if statement or while statement
    if (block_delimiter->block_type == IF_BLOCK_TYPE || block_delimiter->block_type == WHILE_BLOCK_TYPE) {
        MIPS_INSTR* caller_SL_store_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(caller_SL_store_instr->instr_str, "  lw $fp, 8($v0)");
        append_instr(caller_SL_store_instr, initial_instr);
    }

    // get pointer to AR
    MIPS_INSTR* get_FP_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(get_FP_instr->instr_str, "  move $fp, $v0\n");
    append_instr(get_FP_instr, initial_instr);

    // instantiate locals
    LOCAL* current_local = block_AR->locals;
    int current_index = 12;
    while (current_local != NULL) {

        if (current_local->type == INT_LOCAL_TYPE) {
            MIPS_INSTR* instantiate_int_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
            sprintf(instantiate_int_instr->instr_str, "  store $zero, %d($fp)  #instantiate local int", current_index);
            append_instr(instantiate_int_instr, initial_instr);
            current_index += 4;
        }
        else if (current_local->type == CLOSURE_LOCAL_TYPE) {
            // first part of closure is lexical parent frame, which is just the current frame pointer
            MIPS_INSTR* instantiate_closure_frame_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
            sprintf(instantiate_closure_frame_instr->instr_str, "  store $fp, %d($fp)  #instantiate local closure", current_index);
            append_instr(instantiate_closure_frame_instr, initial_instr);
            current_index += 4;
            
            // second part of closure is adress of label
            // first put address to label in register
            MIPS_INSTR* instantiate_closure_label_instr_1 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
            sprintf(instantiate_closure_label_instr_1->instr_str, "  la $t0, %s", current_local->v.local_closure->name->lexeme);
            append_instr(instantiate_closure_label_instr_1, initial_instr);
            // then store value of that register in memory
            MIPS_INSTR* instantiate_closure_label_instr_2 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
            sprintf(instantiate_closure_label_instr_2->instr_str, "  store $t0, %d($fp)", current_index);
            append_instr(instantiate_closure_label_instr_2, initial_instr);
            current_index += 4;
        }

        current_local = current_local->next;

    }


    return initial_instr;

}

// Map function call to MIPS
MIPS_INSTR* function_call_MIPS_template(TAC* function_call_TAC) {

    TAC_FUNCTION_CALL* function_call_tac = &function_call_TAC->v.tac_function_call;

    MIPS_INSTR* initial_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));

    // get address in memory of the closure stored in the activation record
    printf("Searching for address of closure local %s\n", function_call_tac->name->lexeme);
    AR* containing_block_AR = get_containing_AR(function_call_TAC);
    MIPS_INSTR* get_addr_instr = get_local_address(function_call_tac->name, containing_block_AR);
    append_instr(get_addr_instr, initial_instr);

    // pass lexical parent AR as argument
    MIPS_INSTR* lexical_parent_arg_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(lexical_parent_arg_instr->instr_str, "  lw $a0, 0($t0)");
    append_instr(lexical_parent_arg_instr, initial_instr);

    // add jump and link
    MIPS_INSTR* jump_link_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(jump_link_instr->instr_str, "  jal %s\n", function_call_tac->name->lexeme);
    append_instr(jump_link_instr, initial_instr);

    return initial_instr;

}

// Map return to MIPS
MIPS_INSTR* return_MIPS_template(TAC* return_TAC) {

    MIPS_INSTR* initial_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));

    // get containing block's AR
    AR* containing_AR = get_containing_AR(return_TAC);
    // get containing function block's AR
    AR* containing_function_AR = get_containing_function_AR(return_TAC);
    // get path from containing_AR (current fp) to containing function AR so that we can use it's caller's PC and FP
    MIPS_INSTR* get_addr_intr = get_AR_address(containing_function_AR, containing_AR);
    append_instr(get_addr_intr, initial_instr);


    return initial_instr;

}

// Map operation tac to mips
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
                sprintf(new_instr->instr_str, "  li %s, %d", get_register_name(dest), src1->value);
                break;
            }
            if ((dest->type == TEMPORARY_REG_IDENTIFIER || dest->type == ARGUMENT_REG_IDENTIFIER || dest->type == RETURN_REG_IDENTIFIER)
                && (src1->type == TEMPORARY_REG_IDENTIFIER || src1->type == ARGUMENT_REG_IDENTIFIER || src1->type == RETURN_REG_IDENTIFIER)) {
                sprintf(new_instr->instr_str, "  move %s, %s", get_register_name(dest), get_register_name(src1));
            }
            break;
        
        default:
            break;

    }

    return new_instr;

}


// Generate blank activation records
AR* generate_AR(TAC* block_start_TAC) {

    TOKEN* AR_block_name = block_start_TAC->v.tac_block_delimiter.name;

    // create new AR
    AR* new_AR = (AR*)malloc(sizeof(AR));
    printf("Creating new AR for %s\n", AR_block_name->lexeme);
    new_AR->block_name = AR_block_name;
    if (block_start_TAC->v.tac_block_delimiter.parent_block_name != NULL) {
        printf("With lexical parent : %s\n", block_start_TAC->v.tac_block_delimiter.parent_block_name->lexeme);
        new_AR->lexical_parent_AR = get_AR(block_start_TAC->v.tac_block_delimiter.parent_block_name);
    } else {
        printf("With no lexical parent\n");
    }
    append_AR(new_AR);

    TAC* current_TAC = get_next_TAC(block_start_TAC);
    // create int locals
    while (1) {

        // stop when we reach the end of the block
        if (current_TAC->type == BLOCK_END_TAC_TYPE && current_TAC->v.tac_block_delimiter.name == AR_block_name) {
            break;
        }

        // for any int declaration, add that to the current_AR as a local
        if (current_TAC->type == OPERATION_TAC_TYPE) {

            TAC_OPERATION* operation = &current_TAC->v.tac_operation;

            if (operation->is_declaration == 1) {
                // create int local
                LOCAL_INT* new_int_local = (LOCAL_INT*)malloc(sizeof(LOCAL_INT));
                new_int_local->name = operation->dest;
                // create new local
                LOCAL* new_local = (LOCAL*)malloc(sizeof(LOCAL));
                new_local->type = INT_LOCAL_TYPE;
                new_local->v.local_int = new_int_local;
                add_local(new_local, new_AR);
            }

        }

        current_TAC = get_next_TAC(current_TAC);

    }

    current_TAC = get_next_TAC(block_start_TAC);
    // now should add closures for functions that were defined in current block
    while (1) {

        if (current_TAC == NULL) {
            break;
        }

        if (current_TAC->type == BLOCK_START_TAC_TYPE && current_TAC->v.tac_block_delimiter.block_type == FUNCTION_BLOCK_TYPE) {
            
            if (current_TAC->v.tac_block_delimiter.parent_block_name == AR_block_name) {
                // create int local
                LOCAL_CLOSURE* new_closure_local = (LOCAL_CLOSURE*)malloc(sizeof(LOCAL_CLOSURE));
                new_closure_local->name = current_TAC->v.tac_block_delimiter.name;
                // create new local
                LOCAL* new_local = (LOCAL*)malloc(sizeof(LOCAL));
                new_local->type = CLOSURE_LOCAL_TYPE;
                new_local->v.local_closure = new_closure_local;
                add_local(new_local, new_AR);
            }

        }

        current_TAC = get_next_TAC(current_TAC);

    }

    return new_AR;

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

// Get size of AR in number of bytes to use for MIPS
int get_AR_size(AR* activation_record) {

    // initial value of 12 to account for the FP, PC, and SL
    int size = 12;

    LOCAL* current_local = activation_record->locals;
    while (1) {

        if (current_local == NULL) {
            break;
        }

        if (current_local->type == INT_LOCAL_TYPE) {
            size += 4;
        }
        else if (current_local->type == CLOSURE_LOCAL_TYPE) {
            size += 8;
        }

        current_local = current_local->next;

    }

    return size;


}

// Search for TAC's containing block's AR
AR* get_containing_AR(TAC* search_TAC) {

    TAC* current_TAC = get_next_TAC(NULL);
    AR* current_AR = NULL;

    while (current_TAC != NULL) {

        if (current_TAC == NULL) {
            break;
        }

        if (current_TAC->type == BLOCK_START_TAC_TYPE) {
            current_AR = get_AR(current_TAC->v.tac_block_delimiter.name);
        }
        else if (current_TAC->type == BLOCK_END_TAC_TYPE) {
            current_AR = current_AR->lexical_parent_AR;
        }

        if (current_TAC == search_TAC) {
            //printf("found current AR: %s\n", current_AR->block_name->lexeme);
            return current_AR;
        }

        current_TAC = get_next_TAC(current_TAC);

    }

    return NULL;

}

// Search for TAC's containing functional block's AR
AR* get_containing_function_AR(TAC* search_TAC) {

    TAC* current_TAC = get_next_TAC(NULL);
    AR* current_AR = NULL;

    while (current_TAC != NULL) {

        if (current_TAC == NULL) {
            break;
        }

        if (current_TAC == search_TAC) {
            return current_AR;
        }

        if (current_TAC->type == BLOCK_START_TAC_TYPE && current_TAC->v.tac_block_delimiter.block_type == FUNCTION_BLOCK_TYPE) {
            current_AR = get_AR(current_TAC->v.tac_block_delimiter.name);
        }

        current_TAC = get_next_TAC(current_TAC);

    }

    return NULL;

}

// Get address of AR from initial_AR by looking at callers' ARs
MIPS_INSTR* get_AR_address(AR* search_AR, AR* initial_AR) {

    MIPS_INSTR* initial_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(initial_instr->instr_str, "  move $t0, $fp");

    AR* current_AR = initial_AR;

    while (1) {

        if (current_AR == search_AR) {
            break;
        }

        if (current_AR->lexical_parent_AR == NULL) {
            break;
        } else {
            MIPS_INSTR* in_parent_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
            sprintf(in_parent_instr->instr_str, "  lw $t0, 8($t0)");
            append_instr(in_parent_instr, initial_instr);

            current_AR = current_AR->lexical_parent_AR;
        }

    }

    return initial_instr;

}

// Get address int memory to local by looking through Activation Record and it's lexical parents
// the result will be stored in $t0
MIPS_INSTR* get_local_address(TOKEN* search_token, AR* initial_AR) {

    MIPS_INSTR* initial_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(initial_instr->instr_str, "  move $t0, $fp");

    AR* current_AR = initial_AR;
    LOCAL* current_local = NULL;
    printf("\n");

    while (1) {

        current_local = current_AR->locals;
        int current_index = 12;

        printf("Looking at AR %s\n", current_AR->block_name->lexeme);

        while (1) {

            if (current_local->type == INT_LOCAL_TYPE) {
                printf("Looking at int local %s\n", current_local->v.local_int->name->lexeme);
                if (current_local->v.local_int->name == search_token) {
                    MIPS_INSTR* get_from_memory_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
                    printf("Found local\n\n");
                    sprintf(get_from_memory_instr->instr_str, "  la $t0, %d($t0)", current_index);
                    append_instr(get_from_memory_instr, initial_instr);
                    return initial_instr;
                }
                current_index += 4;
            }
            else if (current_local->type == CLOSURE_LOCAL_TYPE) {
                printf("Looking at closure local %s\n", current_local->v.local_closure->name->lexeme);
                if (current_local->v.local_closure->name == search_token) {
                    printf("Found local\n\n");
                    MIPS_INSTR* get_from_memory_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
                    sprintf(get_from_memory_instr->instr_str, "  la $t0, %d($t0)", current_index);
                    append_instr(get_from_memory_instr, initial_instr);
                    return initial_instr;
                }
                current_index += 8;
            }

            if (current_local->next == NULL) {
                break;
            } else {
                current_local = current_local->next;
            }

        }

        if (current_AR->lexical_parent_AR == NULL) {
            break;
        } else {
            MIPS_INSTR* in_parent_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
            sprintf(in_parent_instr->instr_str, "  lw $t0, 8($t0)");
            append_instr(in_parent_instr, initial_instr);

            current_AR = current_AR->lexical_parent_AR;
        }

    }

    printf("Not found local\n\n");
    return initial_instr;

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
        register_name[1] = 'v';
        register_name[2] = register_token->value+'0';
    }

    return strdup(register_name);

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

// Add instr to chain of instrs
void append_instr(MIPS_INSTR* new_instr, MIPS_INSTR* instr_chain) {

    MIPS_INSTR* current_instr = instr_chain;

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

// Add new MIPS instruction to linked list
void append_instr_to_program(MIPS_INSTR* new_instr, MIPS_PROGRAM* program) {

    if (program->instructions == NULL) {
        program->instructions = new_instr;
        return;
    }

    append_instr(new_instr, program->instructions);
    
    return;

}