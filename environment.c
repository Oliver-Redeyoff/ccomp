#include <stdio.h>
#include <stdlib.h>
#include "environment.h"

FRAME* extend_frame(FRAME* frame) {
    FRAME *new_frame = (FRAME*)malloc(sizeof(FRAME));
    if (new_frame==NULL) {
      perror("Cannot make frame");
      exit(1);
    }

    new_frame->bindings = NULL;
    new_frame->next = frame;

    return new_frame;
}

BINDING* add_binding(FRAME* frame, TOKEN* name_token, VALUE* val) {
    BINDING *b = (BINDING*)malloc(sizeof(BINDING));
    if (b==NULL){
        perror("Cannot make binding");
        exit(1);
    }
    b->name_token = name_token;
    b->val = val;
    b->next = NULL;

    if(frame->bindings == NULL) {
        frame->bindings = b;
    } else {
        BINDING *current_binding = frame->bindings;
        while (TRUE){
            if(current_binding->next == NULL) {
                current_binding->next = b;
                break;
            } else {
                current_binding = current_binding->next;
            }
        }   
    }

    return b;
}