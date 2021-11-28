#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "C.tab.h"
#include "tac_generator.h"

TOKEN* return_reg_token;


// Entry point of TAC generation
BASIC_BLOCK* generate_TAC(NODE* tree) {

    BASIC_BLOCK* root_Basic_Block = (BASIC_BLOCK*)malloc(sizeof(BASIC_BLOCK));
    if (root_Basic_Block==NULL) {
        printf("ERROR: could not allocate memory for root basic block\n");
        exit(1);
    }

    // instantiate single return_reg
    return_reg_token = (TOKEN*)malloc(sizeof(TOKEN));
    return_reg_token->type = RETURN_REG_IDENTIFIER;
    return_reg_token->value = 0;

    // create new tac for label to go at start of root block
    TOKEN* label_token = (TOKEN*)malloc(sizeof(TOKEN));
    label_token->type = IDENTIFIER;
    label_token->lexeme = "premain";

    TAC_LABEL* premain_label = (TAC_LABEL*)malloc(sizeof(TAC_LABEL));
    premain_label->name = label_token;

    TAC* label_tac = (TAC*)malloc(sizeof(TAC));
    label_tac->type = LABEL_TAC_TYPE;

    label_tac->v.tac_label = *premain_label;

    add_TAC(label_tac, root_Basic_Block);

    TAC* current_TAC = root_Basic_Block->leader;
    if (current_TAC == NULL) {
        printf("Is null here as well 1\n");
    }

    // Add any necessary info in the pre_main block here
    
    printf("Starting translating tac\n");
    map_to_TAC(tree, root_Basic_Block);

    return root_Basic_Block;

}

// Recurse over AST and map each part to a TAC template
void map_to_TAC(NODE* current_node, BASIC_BLOCK* current_BB) {

    int node_type = current_node->type;

    switch (node_type) {

        case ';':
            map_to_TAC(current_node->left, current_BB);
            map_to_TAC(current_node->right, current_BB);
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

        case 'D':
            function_declaration_template(current_node, current_BB);
            break;

        case RETURN:
            return_template(current_node, current_BB);
            break;

        case APPLY:
            function_call_template(current_node, current_BB);
            break;

        default:
            printf("Node type %d not recognised\n", node_type);
            exit(1);
            break;
    }

    return;

}


// Gets the result of the declaration expression and then call variable_assignement_rec
void variable_declaration_template(NODE* assignement_node, BASIC_BLOCK* current_BB) {

    TOKEN* result_temporary = NULL;

    // map the expression to TAC and use that to assign values to variables
    if (assignement_node->right->type == '=') {
        result_temporary = expression_template(assignement_node->right->right, current_BB);
    }
    else if (assignement_node->right->type == ',') {
        result_temporary = expression_template(assignement_node->right->right->right, current_BB);
    }

    variable_assignement_rec(assignement_node->right, result_temporary, current_BB, 1);

    return;

}

// Recurses through tree to get identifiers to assign value to, and does so
void variable_assignement_rec(NODE* current_node, TOKEN* result_temporary, BASIC_BLOCK* current_BB, int is_declaration) {

    TOKEN* variable_token = NULL;

    if (current_node->type == '=') {
        variable_token = (TOKEN*)current_node->left->left;
    }
    else if (current_node->type == ',') {
        variable_assignement_rec(current_node->left, result_temporary, current_BB, is_declaration);
        variable_assignement_rec(current_node->right, result_temporary, current_BB, is_declaration);
    }
    else if (current_node->type == LEAF) {
        variable_token = (TOKEN*)current_node->left;
    }

    // if we have a variable name to assign the value to, do so
    if (variable_token != NULL) {
        // create new operation tac
        TAC_OPERATION* tac_operation = malloc(sizeof(TAC_OPERATION));
        tac_operation->is_declaration = is_declaration;
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
            TOKEN* token = (TOKEN*)current_node->left;
            return token;
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

        case APPLY: {
            function_call_template(current_node, current_BB);

            TOKEN* dest_token = new_temporary_reg();
            // new operation TAC
            TAC_OPERATION* tac_operation = (TAC_OPERATION*)malloc(sizeof(TAC_OPERATION));
            tac_operation->is_declaration = 0;
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
        tac_operation->is_declaration = 0;
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


// Creates new block, adds label TAC, function delimiter TAC as well as the body of the function mapped to TAC
void function_declaration_template(NODE* D_node, BASIC_BLOCK* current_BB) {

    // create new basic block
    BASIC_BLOCK* new_BB = create_Basic_Block(current_BB);

    // get function name token and put it in label name token
    TOKEN* function_name_token = (TOKEN*)D_node->left->right->left->left;
    TAC_LABEL* function_label = (TAC_LABEL*)malloc(sizeof(TAC_LABEL));
    function_label->name = function_name_token;
    // create new TAC for label to go at start of new block
    TAC* label_tac = (TAC*)malloc(sizeof(TAC));
    label_tac->type = LABEL_TAC_TYPE;
    label_tac->v.tac_label = *function_label;
    add_TAC(label_tac, new_BB);

    // put function name token and arity in function start TAC
    int function_arity = function_declaration_argument_count_rec(D_node->left->right->right);
    TAC_FUNCTION_DELIMITER* tac_function_declaration = (TAC_FUNCTION_DELIMITER*)malloc(sizeof(TAC_FUNCTION_DELIMITER));
    tac_function_declaration->name = function_name_token;
    tac_function_declaration->arity = function_arity;
    // create new TAC for function start indicator
    TAC* function_start_tac = (TAC*)malloc(sizeof(TAC));
    function_start_tac->type = FUNCTION_START_TAC_TYPE;
    function_start_tac->v.tac_function_delimiter = *tac_function_declaration;
    add_TAC(function_start_tac, new_BB);


    // get arguments from registers and assign their value to argument variables
    if (D_node->left->right->right != NULL) {
        function_declaration_argument_retrival_rec(D_node->left->right->right, new_BB, 0);
    }

    // map body of function into the basic block
    map_to_TAC(D_node->right, new_BB);

    // create new TAC for function end indicator
    TAC* function_end_tac = (TAC*)malloc(sizeof(TAC));
    function_end_tac->type = FUNCTION_END_TAC_TYPE;
    function_end_tac->v.tac_function_delimiter = *tac_function_declaration;
    add_TAC(function_end_tac, new_BB);

    return;

}

// Recurse through function declaration and get argument count
int function_declaration_argument_count_rec(NODE* current_node) {

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
        TOKEN* argument_variable_token = (TOKEN*)current_node->right->left;

        // new operation TAC
        TAC_OPERATION* tac_operation = (TAC_OPERATION*)malloc(sizeof(TAC_OPERATION));
        tac_operation->is_declaration = 1;
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

    // first put arguments in argument registers
    if (apply_node->right != NULL) {
        function_call_argument_buffer_rec(apply_node->right, current_BB, 0);
    }

    // create new function call TAC
    TAC_FUNCTION_CALL* tac_function_call = (TAC_FUNCTION_CALL*)malloc(sizeof(TAC_FUNCTION_CALL));
    tac_function_call->name = (TOKEN*)apply_node->left->left;
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
        tac_operation->is_declaration = 0;
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

// Adds return TAC to current basaic block
void return_template(NODE* return_node, BASIC_BLOCK* current_BB) {

    // get token containing return value
    TOKEN* return_result = expression_template(return_node->left, current_BB);

    // new operation TAC
    TAC_OPERATION* tac_operation = (TAC_OPERATION*)malloc(sizeof(TAC_OPERATION));
    tac_operation->is_declaration = 0;
    tac_operation->op = NONE_OPERATION;
    tac_operation->dest = return_reg_token;
    tac_operation->src1 = return_result;
    tac_operation->src2 = NULL;
    // new TAC
    TAC* new_tac = (TAC*)malloc(sizeof(TAC));
    new_tac->type = OPERATION_TAC_TYPE;
    new_tac->v.tac_operation = *tac_operation;
    add_TAC(new_tac, current_BB);

    return;

}


// If statement
void if_template(NODE* if_node, BASIC_BLOCK* current_BB) {

    return;

}


// Add a new Basic Block to the linked list of Basic Blocks
BASIC_BLOCK* create_Basic_Block(BASIC_BLOCK* current_BB) {

    BASIC_BLOCK* new_Basic_Block = (BASIC_BLOCK*)malloc(sizeof(BASIC_BLOCK));
    if (new_Basic_Block==NULL) {
        printf("ERROR: could not allocate memory for new basic block\n");
        exit(1);
    }
    new_Basic_Block->lexical_parent = current_BB;

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
int temporary_reg_counter = 0;
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