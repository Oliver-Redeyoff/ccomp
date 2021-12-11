#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "C.tab.h"
#include "mips_generator.h"

BASIC_BLOCK* root_BB;
MIPS_PROGRAM* program;
AR* AR_list;

// Entry point for MIPS generation
MIPS_PROGRAM* generate_MIPS(BASIC_BLOCK* root) {

    root_BB = root;

    // first generate ARs
    AR* global_AR = (AR*)malloc(sizeof(AR));
    AR_list = global_AR;

    // create new mips program
    program = (MIPS_PROGRAM*)malloc(sizeof(MIPS_PROGRAM));

    MIPS_loop(root_BB);

    return program;

}

// Loop over TAC and map to MIPS
void MIPS_loop(BASIC_BLOCK* root_BB) {

    TAC* current_TAC = get_next_TAC(NULL);

    while (current_TAC != NULL) {

        MIPS_INSTR* new_instr = map_to_MIPS(current_TAC);
        append_instr_to_program(new_instr);

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
            new_instr = block_start_MIPS_template(current_TAC);
            break;
        }

        case BLOCK_END_TAC_TYPE: {
            new_instr = block_end_MIPS_template(current_TAC);
            break;
        }

        case LABEL_TAC_TYPE: {
            TAC_LABEL* label_tac = &current_TAC->v.tac_label;
            sprintf(new_instr->instr_str, "%s:", label_tac->name->lexeme);
            break;
        }

        case GOTO_TAC_TYPE: {
            TAC_GOTO* goto_tac = &current_TAC->v.tac_goto;
            sprintf(new_instr->instr_str, "  j %s", goto_tac->name->lexeme);
            break;
        }

        case IF_TAC_TYPE: {
            TAC_IF* if_tac = &current_TAC->v.tac_if;
            sprintf(new_instr->instr_str, "  beq %s, 1, %s", get_register_name(if_tac->condition_result), if_tac->jump_label->lexeme);
            break;
        }

        case FUNCTION_CALL_TAC_TYPE:
            new_instr = function_call_MIPS_template(current_TAC);
            break;

        case BUILTIN_CALL_TAC_TYPE:
            new_instr = builtin_call_MIPS_template(current_TAC);
            break;

        case RETURN_TAC_TYPE:
            new_instr = return_MIPS_template(current_TAC);
            break;

        case OPERATION_TAC_TYPE: 
            new_instr = operation_MIPS_template(current_TAC);
            break;

        case EXIT_PROGRAM_TAC_TYPE: {
            sprintf(new_instr->instr_str, "  li $v0, 10");
            MIPS_INSTR* exit_syscall_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
            sprintf(exit_syscall_instr->instr_str, "  syscall");
            append_instr(exit_syscall_instr, new_instr);
            break;
        }
        
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
    new_AR->block_name = AR_block_name;
    if (block_start_TAC->v.tac_block_delimiter.parent_block_name != NULL) {
        new_AR->lexical_parent_AR = get_AR(block_start_TAC->v.tac_block_delimiter.parent_block_name);
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

            if (operation->assignement_type == INT_DECLARATION_ASSIGNEMENT_TYPE) {
                // create new local
                LOCAL* new_local = (LOCAL*)malloc(sizeof(LOCAL));
                new_local->type = INT_LOCAL_TYPE;
                new_local->name = operation->dest;
                add_local(new_local, new_AR);
            }
            else if (operation->assignement_type == CLOSURE_DECLARATION_ASSIGNEMENT_TYPE) {
                // create new local
                LOCAL* new_local = (LOCAL*)malloc(sizeof(LOCAL));
                new_local->type = CLOSURE_LOCAL_TYPE;
                new_local->name = operation->dest;
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
                // create new local
                LOCAL* new_local = (LOCAL*)malloc(sizeof(LOCAL));
                new_local->type = CLOSURE_LOCAL_TYPE;
                new_local->name = current_TAC->v.tac_block_delimiter.name;
                new_local->closure_label = current_TAC->v.tac_block_delimiter.name;
                add_local(new_local, new_AR);
            }

        }

        current_TAC = get_next_TAC(current_TAC);

    }

    return new_AR;

}


// Map start of block to MIPS
MIPS_INSTR* block_start_MIPS_template(TAC* block_start_TAC) {

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
    MIPS_INSTR* load_sbreak_code_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(load_sbreak_code_instr->instr_str, "  li $v0, 9");
    append_instr(load_sbreak_code_instr, initial_instr);

    // sbreak syscall 
    MIPS_INSTR* sbreak_syscall_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(sbreak_syscall_instr->instr_str, "  syscall");
    append_instr(sbreak_syscall_instr, initial_instr);

    // put caller's AR in new AR
    MIPS_INSTR* caller_AR_store_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(caller_AR_store_instr->instr_str, "  sw $fp, 0($v0)");
    append_instr(caller_AR_store_instr, initial_instr);
    
    // put caller's return PC and lexical parent scope in new AR if function
    if (block_delimiter->block_type == FUNCTION_BLOCK_TYPE) {
        MIPS_INSTR* caller_PC_store_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(caller_PC_store_instr->instr_str, "  sw $ra, 4($v0)");
        append_instr(caller_PC_store_instr, initial_instr);

        MIPS_INSTR* caller_SL_store_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(caller_SL_store_instr->instr_str, "  sw $t0, 8($v0)");
        append_instr(caller_SL_store_instr, initial_instr);
    }

    // put lexical parent scope in new AR if if statement or while statement
    if (block_delimiter->block_type == IF_BLOCK_TYPE || block_delimiter->block_type == WHILE_BLOCK_TYPE) {
        MIPS_INSTR* caller_SL_store_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(caller_SL_store_instr->instr_str, "  sw $fp, 8($v0)");
        append_instr(caller_SL_store_instr, initial_instr);
    }

    // get pointer to AR
    MIPS_INSTR* get_FP_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(get_FP_instr->instr_str, "  move $fp, $v0");
    append_instr(get_FP_instr, initial_instr);

    // instantiate locals
    LOCAL* current_local = block_AR->locals;
    int current_index = 12;
    while (current_local != NULL) {

        if (current_local->type == INT_LOCAL_TYPE) {
            MIPS_INSTR* instantiate_int_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
            sprintf(instantiate_int_instr->instr_str, "  sw $zero, %d($fp)  #instantiate local int", current_index);
            append_instr(instantiate_int_instr, initial_instr);
            current_index += 4;
        }
        else if (current_local->type == CLOSURE_LOCAL_TYPE) {
            // first part of closure is lexical parent frame, which is just the current frame pointer
            MIPS_INSTR* instantiate_closure_frame_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
            sprintf(instantiate_closure_frame_instr->instr_str, "  sw $fp, %d($fp)  #instantiate local closure", current_index);
            append_instr(instantiate_closure_frame_instr, initial_instr);
            current_index += 4;
            
            // second part of closure is adress of label
            // first put address to label in register
            if (current_local->closure_label != NULL) {
                MIPS_INSTR* instantiate_closure_label_instr_1 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
                sprintf(instantiate_closure_label_instr_1->instr_str, "  la $t0, %s", current_local->closure_label->lexeme);
                append_instr(instantiate_closure_label_instr_1, initial_instr);
                // then store value of that register in memory
                MIPS_INSTR* instantiate_closure_label_instr_2 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
                sprintf(instantiate_closure_label_instr_2->instr_str, "  sw $t0, %d($fp)", current_index);
                append_instr(instantiate_closure_label_instr_2, initial_instr);
            }
            current_index += 4;
        }

        current_local = current_local->next;

    }


    return initial_instr;

}

// Map end of block to MIPS
MIPS_INSTR* block_end_MIPS_template(TAC* block_end_TAC) {

    TAC_BLOCK_DELIMITER* block_end = &block_end_TAC->v.tac_block_delimiter;

    MIPS_INSTR* initial_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    
    // get containing block's AR
    AR* containing_AR = get_containing_AR(block_end_TAC, ANY_BLOCK_TYPE);
    // get containing function block's AR
    AR* end_block_AR = get_AR(block_end->name);
    // get path from containing AR (current fp) to containing function AR so that we can use it's caller's PC and FP
    MIPS_INSTR* get_addr_intr = get_AR_address(end_block_AR, containing_AR);
    append_instr(get_addr_intr, initial_instr);

    if (block_end->block_type == FUNCTION_BLOCK_TYPE) {

        // restore frame pointer to the caller's fp
        MIPS_INSTR* restore_fp_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(restore_fp_instr->instr_str, "  lw $fp, 0($t0)");
        append_instr(restore_fp_instr, initial_instr);

        // load return address into t0
        MIPS_INSTR* load_return_addr_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(load_return_addr_instr->instr_str, "  lw $t0 4($t0)");
        append_instr(load_return_addr_instr, initial_instr);

        // jump to the return address
        MIPS_INSTR* jump_return_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(jump_return_instr->instr_str, "  jr $t0");
        append_instr(jump_return_instr, initial_instr);
    } else {
        // restore frame pointer to the parent's frame pointer
        MIPS_INSTR* restore_fp_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(restore_fp_instr->instr_str, "  lw $fp, 0($t0)");
        append_instr(restore_fp_instr, initial_instr);
    }

    return initial_instr;    

}

// Map function call to MIPS
MIPS_INSTR* function_call_MIPS_template(TAC* function_call_TAC) {

    TAC_FUNCTION_CALL* function_call_tac = &function_call_TAC->v.tac_function_call;

    MIPS_INSTR* initial_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(initial_instr->instr_str, "  # calling function, loading it's lexical scope as first argument");

    // get address in memory of the closure stored in the activation record
    AR* containing_block_AR = get_containing_AR(function_call_TAC, ANY_BLOCK_TYPE);
    MIPS_INSTR* get_addr_instr = get_local_address(function_call_tac->name, containing_block_AR);
    append_instr(get_addr_instr, initial_instr);

    // pass lexical parent AR as argument
    MIPS_INSTR* lexical_parent_arg_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(lexical_parent_arg_instr->instr_str, "  lw $a0, 0($t0)");
    append_instr(lexical_parent_arg_instr, initial_instr);

    // load address of closure to jump to into register t0
    MIPS_INSTR* load_jump_address_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(load_jump_address_instr->instr_str, "  lw $t0, 4($t0)");
    append_instr(load_jump_address_instr, initial_instr);

    // first get value of PC so that we can set RA
    char* getPC_label = new_getPC();
    MIPS_INSTR* set_RA_instr_1 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(set_RA_instr_1->instr_str, "  jal %s", getPC_label);
    append_instr(set_RA_instr_1, initial_instr);
    MIPS_INSTR* set_RA_instr_2 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(set_RA_instr_2->instr_str, "  %s:", getPC_label);
    append_instr(set_RA_instr_2, initial_instr);
    MIPS_INSTR* set_RA_instr_3 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(set_RA_instr_3->instr_str, "  la $ra, 8($ra)");
    append_instr(set_RA_instr_3, initial_instr);

    // add jump and link to address store in register t0
    MIPS_INSTR* jump_link_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(jump_link_instr->instr_str, "  jr $t0");
    append_instr(jump_link_instr, initial_instr);

    return initial_instr;

}

// Map builtin call to MIPS
MIPS_INSTR* builtin_call_MIPS_template(TAC* builtin_call_TAC) {

    TAC_BUILTIN_CALL* builtin_call = &builtin_call_TAC->v.tac_builtin_call;

    MIPS_INSTR* initial_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(initial_instr->instr_str, "  # calling builtin function");

    if (builtin_call->type == PRINT_STR_BUILTIN_FUNCTION_TYPE) {
        // first add data to program and get it's label
        MIPS_DATA* text_data = (MIPS_DATA*)malloc(sizeof(MIPS_DATA));
        text_data->type = TEXT_DATA_TYPE;
        sprintf(text_data->text, "%s", builtin_call->argument->lexeme);
        char* data_label = append_data_to_program(text_data);
        // now do the syscall
        MIPS_INSTR* print_str_syscall_1 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(print_str_syscall_1->instr_str, "  la $a0, %s", data_label);
        append_instr(print_str_syscall_1, initial_instr);
        MIPS_INSTR* print_str_syscall_2 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(print_str_syscall_2->instr_str, "  li $v0 4");
        append_instr(print_str_syscall_2, initial_instr);
        MIPS_INSTR* print_str_syscall_3 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(print_str_syscall_3->instr_str, "  syscall");
        append_instr(print_str_syscall_3, initial_instr);
    }
    else if (builtin_call->type == PRINT_INT_BUILTIN_FUNCTION_TYPE) {
        // first load int address
        AR* containing_block_AR = get_containing_AR(builtin_call_TAC, ANY_BLOCK_TYPE);
        MIPS_INSTR* get_addr_instr = get_local_address(builtin_call->argument, containing_block_AR);
        append_instr(get_addr_instr, initial_instr);
        // now do the syscall
        MIPS_INSTR* print_str_syscall_1 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(print_str_syscall_1->instr_str, "  lw $a0, 0($t0)");
        append_instr(print_str_syscall_1, initial_instr);
        MIPS_INSTR* print_str_syscall_2 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(print_str_syscall_2->instr_str, "  li $v0 1");
        append_instr(print_str_syscall_2, initial_instr);
        MIPS_INSTR* print_str_syscall_3 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(print_str_syscall_3->instr_str, "  syscall");
        append_instr(print_str_syscall_3, initial_instr);
    }
    else if (builtin_call->type == INPUT_INT_BUILTIN_FUNCTION_TYPE) {
        // do the syscall
        MIPS_INSTR* print_str_syscall_1 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(print_str_syscall_1->instr_str, "  li $v0 5");
        append_instr(print_str_syscall_1, initial_instr);
        MIPS_INSTR* print_str_syscall_2 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
        sprintf(print_str_syscall_2->instr_str, "  syscall");
        append_instr(print_str_syscall_2, initial_instr);
    }

    return initial_instr;

}

// Map return to MIPS
MIPS_INSTR* return_MIPS_template(TAC* return_TAC) {

    MIPS_INSTR* initial_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));

    // get containing block's AR
    AR* containing_AR = get_containing_AR(return_TAC, ANY_BLOCK_TYPE);
    // get containing function block's AR
    AR* containing_function_AR = get_containing_AR(return_TAC, FUNCTION_BLOCK_TYPE);
    // get path from containing AR (current fp) to containing function AR so that we can use it's caller's PC and FP
    MIPS_INSTR* get_addr_intr = get_AR_address(containing_function_AR, containing_AR);
    append_instr(get_addr_intr, initial_instr);

    // restore frame pointer to the caller's fp
    MIPS_INSTR* restore_fp_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(restore_fp_instr->instr_str, "  lw $fp, 0($t0)");
    append_instr(restore_fp_instr, initial_instr);

    // load return address into t0
    MIPS_INSTR* load_return_addr_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(load_return_addr_instr->instr_str, "  lw $t0 4($t0)");
    append_instr(load_return_addr_instr, initial_instr);

    // jump to the return address
    MIPS_INSTR* jump_return_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
    sprintf(jump_return_instr->instr_str, "  jr $t0");
    append_instr(jump_return_instr, initial_instr);

    return initial_instr;

}

// Map operation tac to MIPS
MIPS_INSTR* operation_MIPS_template(TAC* operation_TAC) {

    MIPS_INSTR* new_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));

    TAC_OPERATION* operation = &operation_TAC->v.tac_operation;
    TOKEN* dest = operation->dest;
    TOKEN* src1 = operation->src1;
    TOKEN* src2 = operation->src2;

    switch (operation->op) {

        case NONE_OPERATION:
            // load immediate value into register
            if (is_register(dest) == 1 && src1->type == CONSTANT) {
                sprintf(new_instr->instr_str, "  li %s, %d", get_register_name(dest), src1->value);
                break;
            }
            // copy register value into other register
            if (is_register(dest) == 1 && is_register(src1) == 1) {
                sprintf(new_instr->instr_str, "  move %s, %s", get_register_name(dest), get_register_name(src1));
            }
            // load local into register
            if (is_register(dest) == 1 && src1->type == IDENTIFIER) {
                sprintf(new_instr->instr_str, "  # load memory address into register");
                // get AR to start looking in
                AR* current_AR = get_containing_AR(operation_TAC, ANY_BLOCK_TYPE);
                // load address of local in $t0
                MIPS_INSTR* load_local_addr_instr = get_local_address(src1, current_AR);
                append_instr(load_local_addr_instr, new_instr);
                // load value at address into destination register
                int local_type = get_local_type(src1, current_AR);
                if (local_type == INT_LOCAL_TYPE) {
                    MIPS_INSTR* load_int_local_value_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
                    sprintf(load_int_local_value_instr->instr_str, "  lw %s, 0($t0)", get_register_name(dest));
                    append_instr(load_int_local_value_instr, new_instr);
                }
                else if (local_type == CLOSURE_LOCAL_TYPE) {
                    MIPS_INSTR* load_closure_local_value_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
                    sprintf(load_closure_local_value_instr->instr_str, "  la %s, 0($t0)", get_register_name(dest));
                    append_instr(load_closure_local_value_instr, new_instr);
                }
            }
            // store register value into memory address
            if (dest->type == IDENTIFIER && is_register(src1)) {
                sprintf(new_instr->instr_str, "  # store register value into memory address");
                // get AR to start looking in
                AR* current_AR = get_containing_AR(operation_TAC, ANY_BLOCK_TYPE);
                // load address of local in $t0
                MIPS_INSTR* store_local_addr_instr = get_local_address(dest, current_AR);
                append_instr(store_local_addr_instr, new_instr);
                // store value of register into memory address
                int local_type = get_local_type(dest, current_AR);
                if (local_type == INT_LOCAL_TYPE) {
                    MIPS_INSTR* store_int_local_value_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
                    sprintf(store_int_local_value_instr->instr_str, "  sw %s, 0($t0)", get_register_name(src1));
                    append_instr(store_int_local_value_instr, new_instr);
                }
                else if (local_type == CLOSURE_LOCAL_TYPE) {
                    // register stores a pointer to the closure, so need to load that closure first
                    // load first word
                    MIPS_INSTR* load_closure_local_value_instr_1 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
                    sprintf(load_closure_local_value_instr_1->instr_str, "  lw $v0, 0(%s)", get_register_name(src1));
                    append_instr(load_closure_local_value_instr_1, new_instr);
                    // store first word
                    MIPS_INSTR* store_closure_local_value_instr_1 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
                    sprintf(store_closure_local_value_instr_1->instr_str, "  sw $v0, 0($t0)");
                    append_instr(store_closure_local_value_instr_1, new_instr);
                    // load second word
                    MIPS_INSTR* load_closure_local_value_instr_2 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
                    sprintf(load_closure_local_value_instr_2->instr_str, "  lw $v0, 4(%s)", get_register_name(src1));
                    append_instr(load_closure_local_value_instr_2, new_instr);
                    // store second word
                    MIPS_INSTR* store_closure_local_value_instr_2 = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
                    sprintf(store_closure_local_value_instr_2->instr_str, "  sw $v0, 4($t0)");
                    append_instr(store_closure_local_value_instr_2, new_instr);
                }
                else {
                    printf("EROOR : NO TYPE\n");
                }
            }
            break;

        case ADD_OPERATION:
            if (is_register(dest) == 1 && is_register(src1) == 1 && is_register(src2)) {
                sprintf(new_instr->instr_str, "  add %s, %s, %s", get_register_name(dest), get_register_name(src1), get_register_name(src2));
            }
            break;
        
        case SUBTRACT_OPERATION:
            if (is_register(dest) == 1 && is_register(src1) == 1 && is_register(src2)) {
                sprintf(new_instr->instr_str, "  sub %s, %s, %s", get_register_name(dest), get_register_name(src1), get_register_name(src2));
            }
            break;

        case MULTIPLY_OPERATION:
            if (is_register(dest) == 1 && is_register(src1) == 1 && is_register(src2)) {
                sprintf(new_instr->instr_str, "  mul %s, %s, %s", get_register_name(dest), get_register_name(src1), get_register_name(src2));
            }
            break;

        case DIVIDE_OPERATION:
            if (is_register(dest) == 1 && is_register(src1) == 1 && is_register(src2)) {
                sprintf(new_instr->instr_str, "  div %s, %s", get_register_name(src1), get_register_name(src2));
                // copy quotient into destination register
                MIPS_INSTR* copy_quotient_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
                sprintf(copy_quotient_instr->instr_str, "  mflo %s", get_register_name(dest));
                append_instr(copy_quotient_instr, new_instr);
            }
            break;

        case MODULO_OPERATION:
            if (is_register(dest) == 1 && is_register(src1) == 1 && is_register(src2)) {
                sprintf(new_instr->instr_str, "  div %s, %s", get_register_name(src1), get_register_name(src2));
                // copy remainder into destination register
                MIPS_INSTR* copy_remainder_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
                sprintf(copy_remainder_instr->instr_str, "  mfhi %s", get_register_name(dest));
                append_instr(copy_remainder_instr, new_instr);
            }
            break;

        case EQUAL_OPERATION:
            if (is_register(dest) == 1 && is_register(src1) == 1 && is_register(src2)) {
                sprintf(new_instr->instr_str, "  seq %s, %s, %s", get_register_name(dest), get_register_name(src1), get_register_name(src2));
            }
            if (is_register(dest) == 1 && is_register(src1) == 1 && src2->type == CONSTANT) {
                sprintf(new_instr->instr_str, "  seq %s, %s, %d", get_register_name(dest), get_register_name(src1), src2->value);
            }
            break;

        case NOT_EQUAL_OPERATION:
            if (is_register(dest) == 1 && is_register(src1) == 1 && is_register(src2)) {
                sprintf(new_instr->instr_str, "  sne %s, %s, %s", get_register_name(dest), get_register_name(src1), get_register_name(src2));
            }
            break;

        case GREATER_OPERATION:
            if (is_register(dest) == 1 && is_register(src1) == 1 && is_register(src2)) {
                sprintf(new_instr->instr_str, "  sgt %s, %s, %s", get_register_name(dest), get_register_name(src1), get_register_name(src2));
            }
            break;

        case GREATER_EQUAL_OPERATION:
            if (is_register(dest) == 1 && is_register(src1) == 1 && is_register(src2)) {
                sprintf(new_instr->instr_str, "  sge %s, %s, %s", get_register_name(dest), get_register_name(src1), get_register_name(src2));
            }
            break;

        case LESS_OPERATION:
            if (is_register(dest) == 1 && is_register(src1) == 1 && is_register(src2)) {
                sprintf(new_instr->instr_str, "  slt %s, %s, %s", get_register_name(dest), get_register_name(src1), get_register_name(src2));
            }
            break;

        case LESS_EQUAL_OPERATION:
            if (is_register(dest) == 1 && is_register(src1) == 1 && is_register(src2)) {
                sprintf(new_instr->instr_str, "  sle %s, %s, %s", get_register_name(dest), get_register_name(src1), get_register_name(src2));
            }
            break;
        
        default:
            break;

    }

    return new_instr;

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
AR* get_containing_AR(TAC* search_TAC, int block_type) {

    TAC* current_TAC = get_next_TAC(NULL);
    AR* current_AR = NULL;

    while (current_TAC != NULL) {

        if (current_TAC == NULL) {
            break;
        }

        if (current_TAC == search_TAC) {
            //printf("found current AR: %s\n", current_AR->block_name->lexeme);
            return current_AR;
        }

        if (current_TAC->type == BLOCK_START_TAC_TYPE) {
            TAC_BLOCK_DELIMITER* delimiter = &current_TAC->v.tac_block_delimiter;
            if (delimiter->block_type == block_type || block_type == ANY_BLOCK_TYPE) {\
                current_AR = get_AR(current_TAC->v.tac_block_delimiter.name);
            }
        }
        else if (current_TAC->type == BLOCK_END_TAC_TYPE) {
            AR* block_end_AR = get_AR(current_TAC->v.tac_block_delimiter.name);
            if (block_end_AR == current_AR) {
                TAC_BLOCK_DELIMITER* delimiter = &current_TAC->v.tac_block_delimiter;
                if (delimiter->block_type == block_type || block_type == ANY_BLOCK_TYPE) {
                    current_AR = current_AR->lexical_parent_AR;
                }
            }
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

    while (1) {

        current_local = current_AR->locals;
        int current_index = 12;

        while (1) {

            if (current_local == NULL) {
                break;
            }

            if (current_local->name == search_token) {
                MIPS_INSTR* get_from_memory_instr = (MIPS_INSTR*)malloc(sizeof(MIPS_INSTR));
                sprintf(get_from_memory_instr->instr_str, "  la $t0, %d($t0)", current_index);
                append_instr(get_from_memory_instr, initial_instr);
                return initial_instr;
            }

            if (current_local->type == CLOSURE_LOCAL_TYPE) {
                current_index += 8;
            } else {
                current_index += 4;
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

    return initial_instr;

}

// Get type of local
int get_local_type(TOKEN* search_token, AR* initial_AR) {

    AR* current_AR = initial_AR;
    LOCAL* current_local = NULL;

    while (1) {

        current_local = current_AR->locals;

        while (1) {

            if (current_local == NULL) {
                break;
            }

            if (current_local->name == search_token) {
                return current_local->type;
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
            current_AR = current_AR->lexical_parent_AR;
        }

    }

    return 0;

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

// Checks if token is a type of register identifier
int is_register(TOKEN* token) {
    if (token->type == TEMPORARY_REG_IDENTIFIER || token->type == ARGUMENT_REG_IDENTIFIER || token->type == RETURN_REG_IDENTIFIER) {
        return 1;
    } else {
        return 0;
    }
}

// gets string for name of a register in MIPS
char* get_register_name(TOKEN* register_token) {
    
    char register_name[5];

    if (register_token->type == TEMPORARY_REG_IDENTIFIER) {
        if (register_token->value < 10) {
            register_name[0] = '$';
            register_name[1] = 't';
            register_name[2] = register_token->value+'0';
        }
        else if (register_token->value >= 10 && register_token->value < 20) {
            register_name[0] = '$';
            register_name[1] = 's';
            register_name[2] = (register_token->value-10)+'0';
        }
        else {
            printf("ERROR : ran out of registers\n");
            exit(0);
        }
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

// Generate new temporary register
int next_getPC = 1;
char* new_getPC() {

    char* buf = (char*)malloc(12*sizeof(char));
    buf[0] = 'g';
    buf[1] = 'e';
    buf[2] = 't';
    buf[3] = 'P';
    buf[4] = 'C';
    buf[5] = next_getPC+'0';

    next_getPC += 1;

    return buf;

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

// Add new MIPS instruction to program
void append_instr_to_program(MIPS_INSTR* new_instr) {

    if (program->instructions == NULL) {
        program->instructions = new_instr;
        return;
    }

    append_instr(new_instr, program->instructions);
    
    return;

}

// Add data to chain of data
void append_data(MIPS_DATA* new_data, MIPS_DATA* data_chain) {

    MIPS_DATA* current_data = data_chain;

    while (1) {

        if (current_data->next == NULL) {
            current_data->next = new_data;
            break;
        } else {
            current_data = current_data->next;
        }

    }

    return;

}

// Add new MIPS data to program, returns label of new data
int data_count = 1;
char* append_data_to_program(MIPS_DATA* new_data) {

    char* data_label = (char*)malloc(6*sizeof(char));
    data_label[0] = 'd';
    data_label[1] = 'a';
    data_label[2] = 't';
    data_label[3] = 'a';
    data_label[4] = data_count + '0';

    sprintf(new_data->label, "%s", data_label);

    if (program->data == NULL) {
        program->data = new_data;
    } else {
        append_data(new_data, program->data);
    }

    data_count += 1;
    
    return data_label;

}