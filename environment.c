#include <stdio.h>
#include <stdlib.h>
#include "environment.h"

FRAME* extend_frame(FRAME* frame) {
    FRAME* new_frame = (FRAME*)malloc(sizeof(FRAME));
    if (new_frame==NULL) {
      perror("Cannot make frame");
      exit(1);
    }

    new_frame->bindings = NULL;
    new_frame->next = frame;

    return new_frame;
}

BINDING* add_binding(FRAME* frame, TOKEN* name_token, VALUE* val) {
    BINDING* b = (BINDING*)malloc(sizeof(BINDING));
    if (b==NULL){
        perror("Cannot make binding");
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

VALUE* get_value(TOKEN* search_name_token, int search_type, FRAME* frame) {
    while (TRUE) {
        BINDING* current_binding = frame->bindings;
        if (current_binding->name_token == search_name_token && current_binding->val->type == search_type) {
            return current_binding->val;
        } else {
            if (current_binding->next == NULL) {
                printf("No such binding exists");
                return NULL;
            } else {
                current_binding = current_binding->next;
            }
        }
    }
}