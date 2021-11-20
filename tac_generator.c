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

    root_Basic_Block->leader = NULL;
    root_Basic_Block->next = NULL;

    // Add any necessary info in the pre_main block here

    map_to_TAC(tree, root_Basic_Block);

    return root_Basic_Block;

}

// Recurse over AST and map each part to a TAC template
void map_to_TAC(NODE* current_node, BASIC_BLOCK* current_BB) {

    int node_type = current_node->type;

    switch (node_type) {
        case '~':
            if (current_node->left->type == LEAF) {
                variable_assignement_template(current_node);
            } else {
                map_to_TAC(current_node->left, current_BB);
                map_to_TAC(current_node->right, current_BB);
            }
            break;

        case '=':
            variable_assignement_template(current_node);
            break;

        case ',':
            variable_assignement_template(current_node);
            break;

        case 'D':
            
            break;
    }

    return;

}



// Templates
TAC* variable_assignement_template(NODE* current_node) {

    return NULL;

}

TAC* expression_template(NODE* current_node) {

    return NULL;

}



// Add a new Basic Block to the linked list of Basic Blocks
BASIC_BLOCK* create_Basic_Block(BASIC_BLOCK* current_block) {

    BASIC_BLOCK* new_Basic_Block = (BASIC_BLOCK*)malloc(sizeof(BASIC_BLOCK));
    if (new_Basic_Block==NULL) {
        printf("ERROR: could not allocate memory for new basic block\n");
        exit(1);
    }

    new_Basic_Block->leader = NULL;
    new_Basic_Block->next = NULL;

    current_block->next = new_Basic_Block;

    return new_Basic_Block;

}

// Add a TAC to the end of the list of instructions of a Basic Block
void add_TAC(TAC* new_tac, BASIC_BLOCK* current_block) {

    TAC* current_TAC = current_block->leader;

    while(1) {
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
TEMPORARY* new_temporary(TOKEN* token) {
    
    TEMPORARY* new_temporary = (TEMPORARY*)malloc(sizeof(TEMPORARY));
    if (new_temporary==NULL) {
        printf("ERROR: could not allocate memory for new temporary\n");
        exit(1);
    }

    return new_temporary;

}