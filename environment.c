#include <stdio.h>
#include <stdlib.h>
#include "environment.h"

// Add a new frame which extends a existing frame
FRAME* extend_frame(FRAME* frame) {
    FRAME* new_frame = (FRAME*)malloc(sizeof(FRAME));
    if (new_frame==NULL) {
      printf("ERROR: cannot create new frame frame\n");
      exit(1);
    }

    new_frame->bindings = NULL;
    new_frame->next = frame;

    return new_frame;
}

// Add a new binding to a frame
BINDING* add_binding(FRAME* frame, TOKEN* name_token, VALUE* val) {
    BINDING* b = (BINDING*)malloc(sizeof(BINDING));
    if (b==NULL){
        printf("ERROR: cannot create binding for %s\n", name_token->lexeme);
        exit(1);
    }
    b->name_token = name_token;
    b->val = val;
    b->next = NULL;

    if (frame->bindings == NULL) {
        frame->bindings = b;
    } else {
        BINDING* current_binding = frame->bindings;
        while (TRUE){
            if (current_binding->next == NULL) {
                current_binding->next = b;
                break;
            } else {
                current_binding = current_binding->next;
            }
        }   
    }

    return b;
}

// Get value associated with a token, starting at a given frame and working back to the root node until we find the binding
VALUE* get_value(TOKEN* search_name_token, int search_type, FRAME* frame) {
    FRAME* current_frame = frame;
    int found_binding = FALSE;

    while (TRUE) {
        BINDING* current_binding = current_frame->bindings;

        // search through bindings for frame
        while(TRUE) {
            if (current_binding->name_token == search_name_token && current_binding->val->type == search_type) {
                return current_binding->val;
            } else {
                if (current_binding->next == NULL) {
                    break;
                } else {
                    current_binding = current_binding->next;
                }
            }
        }

        // if not found look at parent frame
        if (current_frame->next == NULL) {
            printf("ERROR: could not find binding %s\n", search_name_token->lexeme);
            exit(0);
        } else {
            current_frame = current_frame->next;
        }
    }

}