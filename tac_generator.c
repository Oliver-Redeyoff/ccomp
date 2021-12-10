#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "C.tab.h"
#include "tac_generator.h"

BASIC_BLOCK* root_BB;

TOKEN* return_reg_token;
TOKEN* parent_block_token;
char* built_in_functions_tac[3] = {"print_string", "print_int", "read_int"};

TAC_BLOCK_DELIMITER* current_loop_block;


// Entry point of TAC generation
BASIC_BLOCK* generate_TAC(NODE* tree) {

    BASIC_BLOCK* root_Basic_Block = (BASIC_BLOCK*)malloc(sizeof(BASIC_BLOCK));
    if (root_Basic_Block==NULL) {
        printf("ERROR: could not allocate memory for root basic block\n");
        exit(1);
    }
    root_BB = root_Basic_Block;

    current_loop_block = NULL;

    // instantiate single return_reg
    return_reg_token = (TOKEN*)malloc(sizeof(TOKEN));
    return_reg_token->type = RETURN_REG_IDENTIFIER;
    return_reg_token->value = 0;

    // create new tac for start of global block
    TOKEN* premain_token = (TOKEN*)malloc(sizeof(TOKEN));
    premain_token->type = IDENTIFIER;
    premain_token->lexeme = "premain";

    TAC_BLOCK_DELIMITER* premain_declaration = (TAC_BLOCK_DELIMITER*)malloc(sizeof(TAC_BLOCK_DELIMITER));
    premain_declaration->block_type = FUNCTION_BLOCK_TYPE;
    premain_declaration->name = premain_token;
    premain_declaration->arity = 0;
    premain_declaration->parent_block_name = NULL;
    // create new TAC for function block start indicator
    TAC* premain_start_tac = (TAC*)malloc(sizeof(TAC));
    premain_start_tac->type = BLOCK_START_TAC_TYPE;
    premain_start_tac->v.tac_block_delimiter = *premain_declaration;
    add_TAC(premain_start_tac, root_Basic_Block);

    parent_block_token = premain_token;
    
    map_to_TAC(tree, root_Basic_Block);

    // get main function name token
    TOKEN* main_token = find_main_token(root_Basic_Block);

    // create new function call TAC
    TAC_FUNCTION_CALL* tac_function_call = (TAC_FUNCTION_CALL*)malloc(sizeof(TAC_FUNCTION_CALL));
    tac_function_call->name = main_token;
    tac_function_call->arity = 0;
    // create new TAC
    TAC* new_tac = (TAC*)malloc(sizeof(TAC));
    new_tac->type = FUNCTION_CALL_TAC_TYPE;
    new_tac->v.tac_function_call = *tac_function_call;
    add_TAC(new_tac, root_Basic_Block);

    // create new TAC for end of global block
    TAC* program_end_tac = (TAC*)malloc(sizeof(TAC));
    program_end_tac->type = EXIT_PROGRAM_TAC_TYPE;
    add_TAC(program_end_tac, root_Basic_Block);

    // create new TAC for end of global block
    TAC* premain_end_tac = (TAC*)malloc(sizeof(TAC));
    premain_end_tac->type = BLOCK_END_TAC_TYPE;
    premain_end_tac->v.tac_block_delimiter = *premain_declaration;
    add_TAC(premain_end_tac, root_Basic_Block);

    // divide basic blocks into more basic blocks if needed
    subdivide_basic_blocks(root_Basic_Block);

    // rename main to main1
    rename_block(root_Basic_Block, "main", "main1");

    // rename premain to main
    rename_block(root_Basic_Block, "premain", "main");

    return root_Basic_Block;

}

// Find token for main function name
TOKEN* find_main_token(BASIC_BLOCK* root_BB) {

    BASIC_BLOCK* current_BB = root_BB;
    TAC* current_TAC = NULL;

    while (1) {

        if (current_BB == NULL) {
            break;
        }

        current_TAC = current_BB->leader;

        while (current_TAC != NULL) {

            if (current_TAC->type == BLOCK_START_TAC_TYPE) {
                if (strcmp(current_TAC->v.tac_block_delimiter.name->lexeme, "main") == 0) {
                    return current_TAC->v.tac_block_delimiter.name;
                }
            }


            current_TAC = current_TAC->next;

        }

        if (current_BB->next == NULL) {
            break;
        } else {
            current_BB = current_BB->next;
        }

    }

    return NULL;

}

// Recurse over AST and map each part to a TAC template
void map_to_TAC(NODE* current_node, BASIC_BLOCK* current_BB) {

    int node_type = current_node->type;

    switch (node_type) {

        case ';':
            map_to_TAC(current_node->left, current_BB);
            map_to_TAC(current_node->right, current_BB);
            break;

        case 'D':
            function_declaration_template(current_node, current_BB);
            break;

        case '~':
            if (current_node->left->type == LEAF) {
                variable_declaration_template(current_node, current_BB);
            } else {
                map_to_TAC(current_node->left, current_BB);
                map_to_TAC(current_node->right, current_BB);
            }
            break;

        case '=': {
            TOKEN* result_temporary = expression_template(current_node->right, current_BB);
            variable_assignement_rec(current_node, result_temporary, current_BB, 0);
            break;
        }

        case ',':
            variable_declaration_template(current_node, current_BB);
            break;

        case APPLY:
            function_call_template(current_node, current_BB);
            break;

        case RETURN:
            return_template(current_node, current_BB);
            break;

        case IF:
            if_template(current_node, current_BB);
            break;

        case WHILE:
            while_template(current_node, current_BB);
            break;

        case BREAK: {
            break_template(current_node, current_BB);
            break;
        }

        case CONTINUE: {
            continue_template(current_node, current_BB);
            break;
        }

        default:
            printf("Node type %d not recognised\n", node_type);
            exit(1);
            break;
    }

    return;

}

// Go through generated tac and create any other necessary Basic Blocks
void subdivide_basic_blocks(BASIC_BLOCK* root_BB) {

    BASIC_BLOCK* current_BB = root_BB;
    TAC* current_TAC = current_BB->leader;

    while (1) {

        current_TAC = current_BB->leader;

        while (1) {
            
            if (current_TAC == NULL) {
                break;
            }

            if (current_TAC->type == IF_TAC_TYPE) {
                split_BB(current_TAC, current_BB);
            }
            else if (current_TAC->next != NULL && current_TAC->next->type == LABEL_TAC_TYPE) {
                split_BB(current_TAC, current_BB);
            }
            else if (current_TAC->type == FUNCTION_CALL_TAC_TYPE) {
                split_BB(current_TAC, current_BB);
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

    return;

}

// Split Basic Block into two after given TAC
void split_BB(TAC* current_TAC, BASIC_BLOCK* current_BB) {

    BASIC_BLOCK* new_BB = insert_Basic_Block(current_BB);

    new_BB->leader = current_TAC->next;
    current_TAC->next = NULL;

    return;

}

// main needs to be renamed to main1 so that it is not confused with actuall entry point function main
void rename_block(BASIC_BLOCK* root_BB, char* search_name, char* new_name) {

    BASIC_BLOCK* current_BB = root_BB;
    TAC* current_TAC = current_BB->leader;

    while (1) {

        current_TAC = current_BB->leader;

        while (1) {
            
            if (current_TAC == NULL) {
                break;
            }

            if (current_TAC->type == BLOCK_START_TAC_TYPE) {
                if (strcmp(current_TAC->v.tac_block_delimiter.name->lexeme, search_name) == 0) {
                    current_TAC->v.tac_block_delimiter.name->lexeme = new_name;
                }
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

    return;

}


// Gets the result of the declaration expression and then call variable_assignement_rec
void variable_declaration_template(NODE* assignement_node, BASIC_BLOCK* current_BB) {

    int declaration_type;
    TOKEN* result_temporary = NULL;

    TOKEN* declarartion_type_token = (TOKEN*)assignement_node->left->left;
    // get the type of this declaration
    if (strcmp(declarartion_type_token->lexeme, "function") == 0) {
        declaration_type = CLOSURE_DECLARATION_ASSIGNEMENT_TYPE;
    }
    else if (strcmp(declarartion_type_token->lexeme, "int") == 0) {
        declaration_type = INT_DECLARATION_ASSIGNEMENT_TYPE;
    }

    // map the expression to TAC and use that to assign values to variables
    if (assignement_node->right->type == '=') {
        result_temporary = expression_template(assignement_node->right->right, current_BB);
    }
    else if (assignement_node->right->type == ',') {
        result_temporary = expression_template(assignement_node->right->right->right, current_BB);
    }

    variable_assignement_rec(assignement_node->right, result_temporary, current_BB, declaration_type);

    return;

}

// Recurses through tree to get identifiers to assign value to, and does so
void variable_assignement_rec(NODE* current_node, TOKEN* result_temporary, BASIC_BLOCK* current_BB, int assignement_type) {

    TOKEN* variable_token = NULL;

    if (current_node->type == '=') {
        variable_token = (TOKEN*)current_node->left->left;
    }
    else if (current_node->type == ',') {
        variable_assignement_rec(current_node->left, result_temporary, current_BB, assignement_type);
        variable_assignement_rec(current_node->right, result_temporary, current_BB, assignement_type);
    }
    else if (current_node->type == LEAF) {
        variable_token = (TOKEN*)current_node->left;
    }

    // if we have a variable name to assign the value to, do so
    if (variable_token != NULL) {
        // create new operation tac
        TAC_OPERATION* tac_operation = malloc(sizeof(TAC_OPERATION));
        tac_operation->assignement_type = assignement_type;
        tac_operation->op = NONE_OPERATION;
        tac_operation->dest = variable_token;
        tac_operation->src1 = result_temporary;
        // create new tac
        TAC* new_tac = (TAC*)malloc(sizeof(TAC));
        new_tac->type = OPERATION_TAC_TYPE;
        new_tac->v.tac_operation = *tac_operation;
        add_TAC(new_tac, current_BB);
    }

    return;

}

// Maps an expression to a series of TACs and returns the token which contains the value of the expression
TOKEN* expression_template(NODE* current_node, BASIC_BLOCK* current_BB) {

    int node_type = current_node->type;

    TOKEN* left_token = NULL;
    TOKEN* right_token = NULL;

    int operation;

    switch (node_type) {

        case LEAF: {
            TOKEN* identifier_token = (TOKEN*)current_node->left;
            TOKEN* dest_token = new_temporary_reg();
            // new operation TAC
            TAC_OPERATION* tac_operation = (TAC_OPERATION*)malloc(sizeof(TAC_OPERATION));
            tac_operation->assignement_type = NOT_DECLARATION_ASSIGNEMENT_TYPE;
            tac_operation->op = NONE_OPERATION;
            tac_operation->dest = dest_token;
            tac_operation->src1 = identifier_token;
            tac_operation->src2 = NULL;
            // new TAC
            TAC* new_tac = (TAC*)malloc(sizeof(TAC));
            new_tac->type = OPERATION_TAC_TYPE;
            new_tac->v.tac_operation = *tac_operation;
            add_TAC(new_tac, current_BB);

            return dest_token;
        }

        case '+': {
            left_token = expression_template(current_node->left, current_BB);
            right_token = expression_template(current_node->right, current_BB);
            operation = ADD_OPERATION;
            break;
        }

        case '-': {
            left_token = expression_template(current_node->left, current_BB);
            right_token = expression_template(current_node->right, current_BB);
            operation = SUBTRACT_OPERATION;
            break;
        }

        case '*': {
            left_token = expression_template(current_node->left, current_BB);
            right_token = expression_template(current_node->right, current_BB);
            operation = MULTIPLY_OPERATION;
            break;
        }

        case '/': {
            left_token = expression_template(current_node->left, current_BB);
            right_token = expression_template(current_node->right, current_BB);
            operation = DIVIDE_OPERATION;
            break;
        }

        case '%': {
            left_token = expression_template(current_node->left, current_BB);
            right_token = expression_template(current_node->right, current_BB);
            operation = MODULO_OPERATION;
            break;
        }

        case EQ_OP: {
            left_token = expression_template(current_node->left, current_BB);
            right_token = expression_template(current_node->right, current_BB);
            operation = EQUAL_OPERATION;
            break;
        }

        case NE_OP: {
            left_token = expression_template(current_node->left, current_BB);
            right_token = expression_template(current_node->right, current_BB);
            operation = NOT_EQUAL_OPERATION;
            break;
        }

        case '<': {
            left_token = expression_template(current_node->left, current_BB);
            right_token = expression_template(current_node->right, current_BB);
            operation = LESS_OPERATION;
            break;
        }

        case LE_OP: {
            left_token = expression_template(current_node->left, current_BB);
            right_token = expression_template(current_node->right, current_BB);
            operation = LESS_EQUAL_OPERATION;
            break;
        }

        case '>': {
            // swap left and right around so that we can use less
            left_token = expression_template(current_node->left, current_BB);
            right_token = expression_template(current_node->right, current_BB);
            operation = GREATER_OPERATION;
            break;
        }

        case GE_OP: {
            // swap left and right around so that we can use less equal
            left_token = expression_template(current_node->left, current_BB);
            right_token = expression_template(current_node->right, current_BB);
            operation = GREATER_EQUAL_OPERATION;
            break;
        }


        case APPLY: {
            function_call_template(current_node, current_BB);

            TOKEN* dest_token = new_temporary_reg();
            // new operation TAC
            TAC_OPERATION* tac_operation = (TAC_OPERATION*)malloc(sizeof(TAC_OPERATION));
            tac_operation->assignement_type = NOT_DECLARATION_ASSIGNEMENT_TYPE;
            tac_operation->op = NONE_OPERATION;
            tac_operation->dest = dest_token;
            tac_operation->src1 = return_reg_token;
            tac_operation->src2 = NULL;
            // new TAC
            TAC* new_tac = (TAC*)malloc(sizeof(TAC));
            new_tac->type = OPERATION_TAC_TYPE;
            new_tac->v.tac_operation = *tac_operation;
            add_TAC(new_tac, current_BB);

            return dest_token;
        }

        default:
            break;

    }

    if (left_token != NULL && right_token != NULL) {

        TOKEN* dest_token = new_temporary_reg();

        // new operation TAC
        TAC_OPERATION* tac_operation = (TAC_OPERATION*)malloc(sizeof(TAC_OPERATION));
        tac_operation->assignement_type = NOT_DECLARATION_ASSIGNEMENT_TYPE;
        tac_operation->op = operation;
        tac_operation->dest = dest_token;
        tac_operation->src1 = left_token;
        tac_operation->src2 = right_token;
        // new TAC
        TAC* new_tac = (TAC*)malloc(sizeof(TAC));
        new_tac->type = OPERATION_TAC_TYPE;
        new_tac->v.tac_operation = *tac_operation;
        add_TAC(new_tac, current_BB);

        return dest_token;

    }

    return NULL;

}

// Returns token that contains value of if passed in token is false
TOKEN* generate_false_check(TOKEN* value_token, BASIC_BLOCK* current_BB) {

    // create token to represent false value 
    TOKEN* false_token = (TOKEN*)malloc(sizeof(TOKEN));
    false_token->type = CONSTANT;
    false_token->value = 0;

    // create new temporary
    TOKEN* result_token = new_temporary_reg();

    // new operation TAC
    TAC_OPERATION* tac_operation = (TAC_OPERATION*)malloc(sizeof(TAC_OPERATION));
    tac_operation->assignement_type = NOT_DECLARATION_ASSIGNEMENT_TYPE;
    tac_operation->op = EQUAL_OPERATION;
    tac_operation->dest = result_token;
    tac_operation->src1 = value_token;
    tac_operation->src2 = false_token;
    // new TAC
    TAC* new_tac = (TAC*)malloc(sizeof(TAC));
    new_tac->type = OPERATION_TAC_TYPE;
    new_tac->v.tac_operation = *tac_operation;
    add_TAC(new_tac, current_BB);
    
    return result_token;

}


// Creates new block, adds label TAC, function delimiter TAC as well as the body of the function mapped to TAC
void function_declaration_template(NODE* D_node, BASIC_BLOCK* current_BB) {

    // create new basic block
    BASIC_BLOCK* new_BB = append_Basic_Block(current_BB);

    TOKEN* function_name_token = (TOKEN*)D_node->left->right->left->left;

    // put function name token and arity in function start TAC
    int function_arity = function_declaration_argument_count_rec(D_node->left->right->right);
    TAC_BLOCK_DELIMITER* tac_function_declaration = (TAC_BLOCK_DELIMITER*)malloc(sizeof(TAC_BLOCK_DELIMITER));
    tac_function_declaration->block_type = FUNCTION_BLOCK_TYPE;
    tac_function_declaration->name = function_name_token;
    tac_function_declaration->arity = function_arity;
    tac_function_declaration->parent_block_name = parent_block_token;
    // create new TAC for function block start indicator
    TAC* function_start_tac = (TAC*)malloc(sizeof(TAC));
    function_start_tac->type = BLOCK_START_TAC_TYPE;
    function_start_tac->v.tac_block_delimiter = *tac_function_declaration;
    add_TAC(function_start_tac, new_BB);


    // update current parent block token
    parent_block_token = function_name_token;

    // get arguments from registers and assign their value to argument variables
    if (D_node->left->right->right != NULL) {
        function_declaration_argument_retrival_rec(D_node->left->right->right, new_BB, 1);
    }

    // map body of function into the basic block
    map_to_TAC(D_node->right, new_BB);

    // create new TAC for function end indicator
    TAC* function_end_tac = (TAC*)malloc(sizeof(TAC));
    function_end_tac->type = BLOCK_END_TAC_TYPE;
    function_end_tac->v.tac_block_delimiter = *tac_function_declaration;
    add_TAC(function_end_tac, new_BB);

    // revert parent block name
    parent_block_token = tac_function_declaration->parent_block_name;

    return;

}

// Recurse through function declaration and get argument count
int function_declaration_argument_count_rec(NODE* current_node) {

    if (current_node == NULL) {
        return 0;
    }

    if (current_node->type == ',') {
        // recurse over left and right
        int arity_left = function_declaration_argument_count_rec(current_node->left);
        int arity_right = function_declaration_argument_count_rec(current_node->right);

        return 2 + arity_left + arity_right;
    } 
    else if (current_node->type == '~') {
        return 1;
    }

    return 0;

}

// Recurse through function declaration and get arguments to be assigned values to
void function_declaration_argument_retrival_rec(NODE* current_node, BASIC_BLOCK* current_BB, int arg_count) {

    if (current_node->type == ',') {
        // recurse over left and right
        function_declaration_argument_retrival_rec(current_node->left, current_BB, arg_count+1);
        function_declaration_argument_retrival_rec(current_node->right, current_BB, arg_count+2);
    } 
    else if (current_node->type == '~') {
        // get argument register token which should contain passed argument result
        TOKEN* argument_reg_token = new_argument_reg(arg_count);
        // get argument variable token
        TOKEN* argument_type_token = (TOKEN*)current_node->left->left;
        TOKEN* argument_variable_token = (TOKEN*)current_node->right->left;

        // new operation TAC
        TAC_OPERATION* tac_operation = (TAC_OPERATION*)malloc(sizeof(TAC_OPERATION));
        if (strcmp(argument_type_token->lexeme, "int") == 0) {
            tac_operation->assignement_type = INT_DECLARATION_ASSIGNEMENT_TYPE;
        }
        else if (strcmp(argument_type_token->lexeme, "function") == 0) {
            tac_operation->assignement_type = CLOSURE_DECLARATION_ASSIGNEMENT_TYPE;
        }
        tac_operation->op = NONE_OPERATION;
        tac_operation->dest = argument_variable_token;
        tac_operation->src1 = argument_reg_token;
        tac_operation->src2 = NULL;
        // new TAC
        TAC* new_tac = (TAC*)malloc(sizeof(TAC));
        new_tac->type = OPERATION_TAC_TYPE;
        new_tac->v.tac_operation = *tac_operation;
        add_TAC(new_tac, current_BB);
    }

    return;

}


// Adds function call TAC to current basic block
void function_call_template(NODE* apply_node, BASIC_BLOCK* current_BB) {

    TOKEN* function_name_token = (TOKEN*)apply_node->left->left;

    if (strcmp(function_name_token->lexeme, built_in_functions_tac[0]) == 0) {
        // create new builtin call TAC
        TAC_BUILTIN_CALL* builtin_call = (TAC_BUILTIN_CALL*)malloc(sizeof(TAC_BUILTIN_CALL));
        builtin_call->type = PRINT_STR_BUILTIN_FUNCTION_TYPE;
        builtin_call->argument = (TOKEN*)apply_node->right->left;
        // create new TAC
        TAC* new_tac = (TAC*)malloc(sizeof(TAC));
        new_tac->type = BUILTIN_CALL_TAC_TYPE;
        new_tac->v.tac_builtin_call = *builtin_call;
        add_TAC(new_tac, current_BB);
        return;
    }
    else if (strcmp(function_name_token->lexeme, built_in_functions_tac[1]) == 0) {
        // create new builtin call TAC
        TAC_BUILTIN_CALL* builtin_call = (TAC_BUILTIN_CALL*)malloc(sizeof(TAC_BUILTIN_CALL));
        builtin_call->type = PRINT_INT_BUILTIN_FUNCTION_TYPE;
        builtin_call->argument = (TOKEN*)apply_node->right->left;
        // create new TAC
        TAC* new_tac = (TAC*)malloc(sizeof(TAC));
        new_tac->type = BUILTIN_CALL_TAC_TYPE;
        new_tac->v.tac_builtin_call = *builtin_call;
        add_TAC(new_tac, current_BB);
        return;
    }
    else if (strcmp(function_name_token->lexeme, built_in_functions_tac[2]) == 0) {
        // create new builtin call TAC
        TAC_BUILTIN_CALL* builtin_call = (TAC_BUILTIN_CALL*)malloc(sizeof(TAC_BUILTIN_CALL));
        builtin_call->type = INPUT_INT_BUILTIN_FUNCTION_TYPE;
        builtin_call->argument = NULL;
        // create new TAC
        TAC* new_tac = (TAC*)malloc(sizeof(TAC));
        new_tac->type = BUILTIN_CALL_TAC_TYPE;
        new_tac->v.tac_builtin_call = *builtin_call;
        add_TAC(new_tac, current_BB);
        return;
    }

    // first put arguments in argument registers
    if (apply_node->right != NULL) {
        function_call_argument_buffer_rec(apply_node->right, current_BB, 1);
    }

    // create new function call TAC
    TAC_FUNCTION_CALL* tac_function_call = (TAC_FUNCTION_CALL*)malloc(sizeof(TAC_FUNCTION_CALL));
    tac_function_call->name = function_name_token;
    tac_function_call->arity = function_call_argument_count_rec(apply_node->right);
    // create new TAC
    TAC* new_tac = (TAC*)malloc(sizeof(TAC));
    new_tac->type = FUNCTION_CALL_TAC_TYPE;
    new_tac->v.tac_function_call = *tac_function_call;
    add_TAC(new_tac, current_BB);

    return;

}

// Recurse through function call to get count of arguments being passed
int function_call_argument_count_rec(NODE* current_node) {

    if (current_node == NULL) {
        return 0;
    }

    if (current_node->type == ',') {
        // recurse over left and right
        int arity_left = function_call_argument_count_rec(current_node->left);
        int arity_right = function_call_argument_count_rec(current_node->right);

        return 2 + arity_left + arity_right;
    } else {
        return 1;
    }

    return 0;

}

// Recurse through tree to get values to pass as arguments through argument registers
void function_call_argument_buffer_rec(NODE* current_node, BASIC_BLOCK* current_BB, int arg_count) {

    if (current_node->type == ',') {
        // recurse over left and right
        function_call_argument_buffer_rec(current_node->left, current_BB, arg_count+1);
        function_call_argument_buffer_rec(current_node->right, current_BB, arg_count+2);
    } else {
        // get token containing expression result
        TOKEN* expression_result_token = expression_template(current_node, current_BB);
        // get token for argument register
        TOKEN* argument_reg_token = new_argument_reg(arg_count);

        // new operation TAC
        TAC_OPERATION* tac_operation = (TAC_OPERATION*)malloc(sizeof(TAC_OPERATION));
        tac_operation->assignement_type = NOT_DECLARATION_ASSIGNEMENT_TYPE;
        tac_operation->op = NONE_OPERATION;
        tac_operation->dest = argument_reg_token;
        tac_operation->src1 = expression_result_token;
        tac_operation->src2 = NULL;
        // new TAC
        TAC* new_tac = (TAC*)malloc(sizeof(TAC));
        new_tac->type = OPERATION_TAC_TYPE;
        new_tac->v.tac_operation = *tac_operation;
        add_TAC(new_tac, current_BB);
    }

    return;

}

// Adds return TAC to current basic block
void return_template(NODE* return_node, BASIC_BLOCK* current_BB) {

    // get token containing return value
    TOKEN* return_result = expression_template(return_node->left, current_BB);

    // new operation TAC
    TAC_OPERATION* tac_operation = (TAC_OPERATION*)malloc(sizeof(TAC_OPERATION));
    tac_operation->assignement_type = NOT_DECLARATION_ASSIGNEMENT_TYPE;
    tac_operation->op = NONE_OPERATION;
    tac_operation->dest = return_reg_token;
    tac_operation->src1 = return_result;
    tac_operation->src2 = NULL;
    // new TAC
    TAC* new_tac = (TAC*)malloc(sizeof(TAC));
    new_tac->type = OPERATION_TAC_TYPE;
    new_tac->v.tac_operation = *tac_operation;
    add_TAC(new_tac, current_BB);

    // add return TAC
    TAC* return_tac = (TAC*)malloc(sizeof(TAC));
    return_tac->type = RETURN_TAC_TYPE;
    add_TAC(return_tac, current_BB);

    return;

}


// If statement
void if_template(NODE* if_node, BASIC_BLOCK* current_BB) {

    // create tokens for else and next label
    TOKEN* if_label_token = new_if();
    TOKEN* else_label_token = new_else();
    TOKEN* next_label_token = new_next();

    // create if condition TAC
    // get token containing result of if condition
    TOKEN* if_condition_result = expression_template(if_node->left, current_BB);
    TOKEN* inverse_if_condition_result = generate_false_check(if_condition_result, current_BB);

    // new if statement TAC
    TAC_IF* tac_if = (TAC_IF*)malloc(sizeof(TAC_IF));
    tac_if->condition_result = inverse_if_condition_result;
    tac_if->jump_label = else_label_token;
    // new TAC
    TAC* new_tac = (TAC*)malloc(sizeof(TAC));
    new_tac->type = IF_TAC_TYPE;
    new_tac->v.tac_if = *tac_if;
    add_TAC(new_tac, current_BB);

    // add if label
    TAC* if_label_tac = generate_label(if_label_token);
    add_TAC(if_label_tac, current_BB);

    // indicate start of new if block
    TAC_BLOCK_DELIMITER* tac_if_block_delimiter = (TAC_BLOCK_DELIMITER*)malloc(sizeof(TAC_BLOCK_DELIMITER));
    tac_if_block_delimiter->block_type = IF_BLOCK_TYPE;
    tac_if_block_delimiter->name = if_label_token;
    tac_if_block_delimiter->arity = 0;
    tac_if_block_delimiter->parent_block_name = parent_block_token;
    // create new TAC for block start indicator
    TAC* if_block_start_tac = (TAC*)malloc(sizeof(TAC));
    if_block_start_tac->type = BLOCK_START_TAC_TYPE;
    if_block_start_tac->v.tac_block_delimiter = *tac_if_block_delimiter;
    add_TAC(if_block_start_tac, current_BB);

    // update current parent block token
    parent_block_token = if_label_token;

    // put if part bellow if statement
    if (if_node->right->type == ELSE) {
        // map body of if into Basic Block
        map_to_TAC(if_node->right->left, current_BB);
    } else {
        // map body of if into Basic Block
        map_to_TAC(if_node->right, current_BB);
    }

    // create new TAC for if block end indicator
    TAC* if_block_end_tac = (TAC*)malloc(sizeof(TAC));
    if_block_end_tac->type = BLOCK_END_TAC_TYPE;
    if_block_end_tac->v.tac_block_delimiter = *tac_if_block_delimiter;
    add_TAC(if_block_end_tac, current_BB);

    TAC* goto_next_tac_from_if = generate_goto(next_label_token);
    add_TAC(goto_next_tac_from_if, current_BB);

    // revert parent block name
    parent_block_token = tac_if_block_delimiter->parent_block_name;


    // put else label bellow if body
    TAC* else_label_tac = generate_label(else_label_token);
    add_TAC(else_label_tac, current_BB);

    // indicate start of new else block
    TAC_BLOCK_DELIMITER* tac_else_block_delimiter = (TAC_BLOCK_DELIMITER*)malloc(sizeof(TAC_BLOCK_DELIMITER));
    tac_else_block_delimiter->block_type = IF_BLOCK_TYPE;
    tac_else_block_delimiter->name = else_label_token;
    tac_else_block_delimiter->arity = 0;
    tac_else_block_delimiter->parent_block_name = parent_block_token;
    // create new TAC for block start indicator
    TAC* else_block_start_tac = (TAC*)malloc(sizeof(TAC));
    else_block_start_tac->type = BLOCK_START_TAC_TYPE;
    else_block_start_tac->v.tac_block_delimiter = *tac_else_block_delimiter;
    add_TAC(else_block_start_tac, current_BB);

    // update current parent block token
    parent_block_token = else_label_token;

    if (if_node->right->type == ELSE) {
        // map body of else into Basic Block
        map_to_TAC(if_node->right->right, current_BB);
    }

    // create new TAC for else block end indicator
    TAC* else_block_end_tac = (TAC*)malloc(sizeof(TAC));
    else_block_end_tac->type = BLOCK_END_TAC_TYPE;
    else_block_end_tac->v.tac_block_delimiter = *tac_else_block_delimiter;
    add_TAC(else_block_end_tac, current_BB);

    // revert parent block name
    parent_block_token = tac_else_block_delimiter->parent_block_name;

    TAC* goto_next_tac_from_else = generate_goto(next_label_token);
    add_TAC(goto_next_tac_from_else, current_BB);


    // put next part bellow else body
    TAC* next_label_tac = generate_label(next_label_token);
    add_TAC(next_label_tac, current_BB);

    return;

}

// While statement
void while_template(NODE* while_node, BASIC_BLOCK* current_BB) {

    TOKEN* loop_token = new_loop();
    TOKEN* next_token = new_next();

    // add loop label tac
    TAC* loop_label = generate_label(loop_token);
    add_TAC(loop_label, current_BB);

    // add check for while condition
    TOKEN* while_condition_result = expression_template(while_node->left, current_BB);
    TOKEN* inverse_while_condition_result = generate_false_check(while_condition_result, current_BB);

    // new if statement TAC
    TAC_IF* tac_if = (TAC_IF*)malloc(sizeof(TAC_IF));
    tac_if->condition_result = inverse_while_condition_result;
    tac_if->jump_label = next_token;
    // new TAC
    TAC* new_tac = (TAC*)malloc(sizeof(TAC));
    new_tac->type = IF_TAC_TYPE;
    new_tac->v.tac_if = *tac_if;
    add_TAC(new_tac, current_BB);

    // indicate start of new block
    TAC_BLOCK_DELIMITER* tac_block_delimiter = (TAC_BLOCK_DELIMITER*)malloc(sizeof(TAC_BLOCK_DELIMITER));
    tac_block_delimiter->block_type = WHILE_BLOCK_TYPE;
    tac_block_delimiter->name = loop_token;
    tac_block_delimiter->arity = 0;
    tac_block_delimiter->parent_block_name = parent_block_token;
    // create new TAC for block start indicator
    TAC* block_start_tac = (TAC*)malloc(sizeof(TAC));
    block_start_tac->type = BLOCK_START_TAC_TYPE;
    block_start_tac->v.tac_block_delimiter = *tac_block_delimiter;
    add_TAC(block_start_tac, current_BB);

    // update current parent block token
    parent_block_token = loop_token;

    //  update current loop name
    TAC_BLOCK_DELIMITER* previous_loop_block = current_loop_block;
    current_loop_block = tac_block_delimiter;

    // add body of while block
    map_to_TAC(while_node->right, current_BB);

    // restore old loop name
    current_loop_block = previous_loop_block;

    // create new TAC for block end indicator
    TAC* block_end_tac = (TAC*)malloc(sizeof(TAC));
    block_end_tac->type = BLOCK_END_TAC_TYPE;
    block_end_tac->v.tac_block_delimiter = *tac_block_delimiter;
    add_TAC(block_end_tac, current_BB);

    // revert parent block name
    parent_block_token = tac_block_delimiter->parent_block_name;

    // add goto loop
    TAC* loop_goto = generate_goto(loop_token);
    add_TAC(loop_goto, current_BB);

    // add next label
    TAC* next_label = generate_label(next_token);
    add_TAC(next_label, current_BB);

    return;

}

// Adds TAC to continue to next iteration of loop
void continue_template(NODE* continue_node, BASIC_BLOCK* current_BB) {

    // create new TAC for block end indicator
    TAC* block_end_tac = (TAC*)malloc(sizeof(TAC));
    block_end_tac->type = BLOCK_END_TAC_TYPE;
    block_end_tac->v.tac_block_delimiter = *current_loop_block;
    add_TAC(block_end_tac, current_BB);

    // add goto loop
    TAC* loop_goto = generate_goto(current_loop_block->name);
    add_TAC(loop_goto, current_BB);

    return;

}

// Adds TAC to break out of loop
void break_template(NODE* break_node, BASIC_BLOCK* current_BB) {

    // Find the label to go to the code after the loop
    BASIC_BLOCK* current_search_BB = root_BB;
    TAC* current_search_TAC = current_BB->leader;
    TOKEN* after_loop_label;
    int found_label = 0;
    while (found_label == 0) {

        current_search_TAC = current_BB->leader;

        while (found_label == 0) {
            
            if (current_search_TAC == NULL) {
                break;
            }

            // the if before the loop block will contain the label to jump to
            if (current_search_TAC->type == IF_TAC_TYPE) {
                after_loop_label = current_search_TAC->v.tac_if.jump_label;
            }

            // once we reach the start of the loop, the correct label to jump to
            // should be stored in after_loop_label
            if (current_search_TAC->type == BLOCK_START_TAC_TYPE) {
                if (current_search_TAC->v.tac_block_delimiter.name == current_loop_block->name) {
                    found_label = 1;
                    break;
                }
            }

            if (current_search_TAC->next == NULL) {
                break;
            } else {
                current_search_TAC = current_search_TAC->next;
            }

        }

        if (current_search_BB->next == NULL) {
            break;
        } else {
            current_search_BB = current_search_BB->next;
        }

    }

    // create new TAC for block end indicator
    TAC* block_end_tac = (TAC*)malloc(sizeof(TAC));
    block_end_tac->type = BLOCK_END_TAC_TYPE;
    block_end_tac->v.tac_block_delimiter = *current_loop_block;
    add_TAC(block_end_tac, current_BB);

    // add goto loop
    TAC* loop_goto = generate_goto(after_loop_label);
    add_TAC(loop_goto, current_BB);
    
    return;

}



// Add a new Basic Block to the end of the linked list of Basic Blocks
BASIC_BLOCK* append_Basic_Block(BASIC_BLOCK* current_BB) {

    BASIC_BLOCK* new_Basic_Block = (BASIC_BLOCK*)malloc(sizeof(BASIC_BLOCK));
    if (new_Basic_Block==NULL) {
        printf("ERROR: could not allocate memory for new basic block\n");
        exit(1);
    }

    while (1) {
        // see if basic block can be added as next, else move to next basic block
        if (current_BB->next == NULL) {
            current_BB->next = new_Basic_Block;
            break;
        } else {
            current_BB = current_BB->next;
        }
    }

    return new_Basic_Block;

}

// Insert a new Basic Block after the passed Basic Block
BASIC_BLOCK* insert_Basic_Block(BASIC_BLOCK* current_BB) {

    BASIC_BLOCK* new_Basic_Block = (BASIC_BLOCK*)malloc(sizeof(BASIC_BLOCK));
    if (new_Basic_Block==NULL) {
        printf("ERROR: could not allocate memory for new basic block\n");
        exit(1);
    }

    if (current_BB->next == NULL) {
        current_BB->next = new_Basic_Block;
    } else {
        BASIC_BLOCK* old_next = current_BB->next;
        current_BB->next = new_Basic_Block;
        new_Basic_Block->next = old_next;
    }

    return new_Basic_Block;

}

// Add a TAC to the end of the list of instructions of a Basic Block
void add_TAC(TAC* new_tac, BASIC_BLOCK* current_BB) {

    TAC* current_TAC = current_BB->leader;

    if (current_TAC == NULL) {
        current_BB->leader = new_tac;
        return;
    }

    while(1) {
        // see if tac can be added as next, or move onto next tac
        if (current_TAC->next == NULL) {
            current_TAC->next = new_tac;
            break;
        } else {
            current_TAC = current_TAC->next;
        }
    }

    return;

}


// Generate new temporary register
int temporary_reg_counter = 1;
TOKEN* new_temporary_reg() {
    
    TOKEN* temporary_reg_token = (TOKEN*)malloc(sizeof(TOKEN));
    if (temporary_reg_token==NULL) {
        printf("ERROR: could not allocate memory for new temporary token\n");
        exit(1);
    }

    temporary_reg_token->type = TEMPORARY_REG_IDENTIFIER;
    temporary_reg_token->value = temporary_reg_counter;

    temporary_reg_counter += 1;

    return temporary_reg_token;

}

// Generate new argument register
TOKEN* new_argument_reg(int id) {
    
    TOKEN* argument_reg_token = (TOKEN*)malloc(sizeof(TOKEN));
    if (argument_reg_token==NULL) {
        printf("ERROR: could not allocate memory for new temporary token\n");
        exit(1);
    }

    argument_reg_token->type = ARGUMENT_REG_IDENTIFIER;
    argument_reg_token->value = id;

    return argument_reg_token;

}

// Generate new if label token
int if_counter = 1;
TOKEN* new_if() {

    TOKEN* if_token = (TOKEN*)malloc(sizeof(TOKEN));
    if (if_token==NULL) {
        printf("ERROR: could not allocate memory for new if token\n");
        exit(1);
    }

    if_token->type = IDENTIFIER;
    char* buf = (char*)malloc(12*sizeof(char));
    buf[0] = 'i';
    buf[1] = 'f';
    buf[2] = '_';
    buf[3] = if_counter+'0';
    if_token->lexeme = buf;

    if_counter += 1;

    return if_token;

}

// Generate new else label token
int else_counter = 1;
TOKEN* new_else() {

    TOKEN* else_token = (TOKEN*)malloc(sizeof(TOKEN));
    if (else_token==NULL) {
        printf("ERROR: could not allocate memory for new else token\n");
        exit(1);
    }

    else_token->type = IDENTIFIER;
    char* buf = (char*)malloc(12*sizeof(char));
    buf[0] = 'e';
    buf[1] = 'l';
    buf[2] = 's';
    buf[3] = 'e';
    buf[4] = '_';
    buf[5] = else_counter+'0';
    else_token->lexeme = buf;

    else_counter += 1;

    return else_token;

}

// Generate new else label token
int next_counter = 1;
TOKEN* new_next() {

    TOKEN* next_token = (TOKEN*)malloc(sizeof(TOKEN));
    if (next_token==NULL) {
        printf("ERROR: could not allocate memory for new next token\n");
        exit(1);
    }

    next_token->type = IDENTIFIER;
    char* buf = (char*)malloc(12*sizeof(char));
    buf[0] = 'n';
    buf[1] = 'e';
    buf[2] = 'x';
    buf[3] = 't';
    buf[4] = '_';
    buf[5] = next_counter+'0';
    next_token->lexeme = buf;

    next_counter += 1;

    return next_token;

}

// Generate new loop label token
int loop_counter = 1;
TOKEN* new_loop() {
   
    TOKEN* loop_token = (TOKEN*)malloc(sizeof(TOKEN));
    if (loop_token==NULL) {
        printf("ERROR: could not allocate memory for new else token\n");
        exit(1);
    }

    loop_token->type = IDENTIFIER;
    char* buf = (char*)malloc(12*sizeof(char));
    buf[0] = 'l';
    buf[1] = 'o';
    buf[2] = 'o';
    buf[3] = 'p';
    buf[4] = '_';
    buf[5] = loop_counter+'0';
    loop_token->lexeme = buf;

    loop_counter += 1;

    return loop_token;
 
}


// Generate new label tac
TAC* generate_label(TOKEN* name) {

    // new operation TAC
    TAC_LABEL* tac_label = (TAC_LABEL*)malloc(sizeof(TAC_LABEL));
    tac_label->name = name;
    // new TAC
    TAC* new_tac = (TAC*)malloc(sizeof(TAC));
    new_tac->type = LABEL_TAC_TYPE;
    new_tac->v.tac_label = *tac_label;
    
    return new_tac;

}

// Generate new goto tac
TAC* generate_goto(TOKEN* name) {

    // new operation TAC
    TAC_GOTO* tac_goto = (TAC_GOTO*)malloc(sizeof(TAC_GOTO));
    tac_goto->name = name;
    // new TAC
    TAC* new_tac = (TAC*)malloc(sizeof(TAC));
    new_tac->type = GOTO_TAC_TYPE;
    new_tac->v.tac_goto = *tac_goto;
    
    return new_tac;

}