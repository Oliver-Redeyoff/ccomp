#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "C.tab.h"
#include "tac_generator.h"



// Entry point of TAC generation
BASIC_BLOCK* generate_TAC(NODE* tree) {

    BASIC_BLOCK* root_Basic_Block = (BASIC_BLOCK*)malloc(sizeof(BASIC_BLOCK));
    if (root_Basic_Block==NULL) {
        printf("ERROR: could not allocate memory for root basic block\n");
        exit(1);
    }

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
                variable_assignement_template(current_node, current_BB);
            } else {
                map_to_TAC(current_node->left, current_BB);
                map_to_TAC(current_node->right, current_BB);
            }
            break;

        case '=': {
            TOKEN* result_temporary = expression_template(current_node->right, current_BB);
            variable_assignement_rec(current_node, result_temporary, current_BB);
            break;
        }

        case ',':
            variable_assignement_template(current_node, current_BB);
            break;

        case 'D':
            function_declaration_template(current_node, current_BB);
            break;

        case RETURN:
            return_template(current_node, current_BB);
            break;

        default:
            printf("Node type %d not recognised\n", node_type);
            exit(1);
            break;
    }

    return;

}



// Templates
void variable_assignement_template(NODE* assignement_node, BASIC_BLOCK* current_BB) {

    TOKEN* result_temporary = NULL;

    // map the expression to TAC and use that to assign values to variables
    if (assignement_node->right->type == '=') {
        result_temporary = expression_template(assignement_node->right->right, current_BB);
    }
    else if (assignement_node->right->type == ',') {
        result_temporary = expression_template(assignement_node->right->right->right, current_BB);
    }

    variable_assignement_rec(assignement_node->right, result_temporary, current_BB);

    return;

}

void variable_assignement_rec(NODE* current_node, TOKEN* result_temporary, BASIC_BLOCK* current_BB) {

    TOKEN* variable_token = NULL;

    if (current_node->type == '=') {
        variable_token = (TOKEN*)current_node->left->left;
    }
    else if (current_node->type == ',') {
        variable_assignement_rec(current_node->left, result_temporary, current_BB);
        variable_assignement_rec(current_node->right, result_temporary, current_BB);
    }
    else if (current_node->type == LEAF) {
        variable_token = (TOKEN*)current_node->left;
    }

    // if we have a variable name to assign the value to, do so
    if (variable_token != NULL) {
        TAC* new_tac = (TAC*)malloc(sizeof(TAC));
        new_tac->type = OPERATION_TAC_TYPE;

        TAC_OPERATION* operation = malloc(sizeof(TAC_OPERATION));
        operation->op = NONE_OPERATION;
        operation->dest = variable_token;
        operation->src1 = result_temporary;

        new_tac->v.tac_operation = *operation;

        add_TAC(new_tac, current_BB);
    }

    return;

}

// Maps an expression to TAC and returns the temporary token which contains the value of the expression
TOKEN* expression_template(NODE* current_node, BASIC_BLOCK* current_BB) {

    printf("In expression template\n");

    int node_type = current_node->type;

    TOKEN* left_token = NULL;
    TOKEN* right_token = NULL;

    int operation;

    switch (node_type) {

        case LEAF: {
            printf("Leaf case\n");
            TOKEN* token = (TOKEN*)current_node->left;
            return token;
        }

        case '+': {
            printf("Add case\n");
            left_token = expression_template(current_node->left, current_BB);
            right_token = expression_template(current_node->right, current_BB);
            operation = ADD_OPERATION;
            break;
        }

        case '-': {
            printf("Minus case\n");
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

        default:
            break;

    }

    if (left_token != NULL && right_token != NULL) {

        TOKEN* dest_token = new_temporary();
        //printf("type of dest : %d\n", dest_token->type);

        TAC* new_tac = (TAC*)malloc(sizeof(TAC));
        new_tac->type = OPERATION_TAC_TYPE;

        TAC_OPERATION* tac_operation = (TAC_OPERATION*)malloc(sizeof(TAC_OPERATION));
        tac_operation->op = operation;
        tac_operation->dest = dest_token;
        tac_operation->src1 = left_token;
        tac_operation->src2 = right_token;

        new_tac->v.tac_operation = *tac_operation;

        add_TAC(new_tac, current_BB);

        return dest_token;

    }

    return NULL;

}

void function_declaration_template(NODE* D_node, BASIC_BLOCK* current_BB) {

    // create new basic block
    BASIC_BLOCK* new_BB = create_Basic_Block(current_BB);

    // create new tac for label to go at start of new block
    TAC* label_tac = (TAC*)malloc(sizeof(TAC));
    label_tac->type = LABEL_TAC_TYPE;

    // get function name token and put it in label name token
    TOKEN* function_name_token = (TOKEN*)D_node->left->right->left->left;
    TAC_LABEL* function_label = (TAC_LABEL*)malloc(sizeof(TAC_LABEL));
    function_label->name = function_name_token;
    label_tac->v.tac_label = *function_label;

    // add label tac to new block and map function definition into block
    add_TAC(label_tac, new_BB);
    map_to_TAC(D_node->right, new_BB);

    return;

}

void return_template(NODE* return_node, BASIC_BLOCK* current_BB) {

    TAC* new_tac = (TAC*)malloc(sizeof(TAC));
    new_tac->type = RETURN_TAC_TYPE;

    TAC_RETURN* tac_return = (TAC_RETURN*)malloc(sizeof(TAC_RETURN));
    TOKEN* return_result = expression_template(return_node->left, current_BB);
    tac_return->name = return_result;

    new_tac->v.tac_return = *tac_return;

    add_TAC(new_tac, current_BB);

    return;

}


// Add a new Basic Block to the linked list of Basic Blocks
BASIC_BLOCK* create_Basic_Block(BASIC_BLOCK* current_BB) {

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

// Add a TAC to the end of the list of instructions of a Basic Block
void add_TAC(TAC* new_tac, BASIC_BLOCK* current_BB) {

    printf("Adding new tac\n");

    TAC* current_TAC = current_BB->leader;

    if (current_TAC == NULL) {
        printf("putting tac as leader\n");
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



// Generate new temporary
int temporary_counter = 0;
TOKEN* new_temporary() {
    
    TOKEN* temporary_token = (TOKEN*)malloc(sizeof(TOKEN));
    if (temporary_token==NULL) {
        printf("ERROR: could not allocate memory for new temporary token\n");
        exit(1);
    }

    temporary_token->type = TEMPORARY_IDENTIFIER;
    temporary_token->value = temporary_counter;
    temporary_counter += 1;

    return temporary_token;

}