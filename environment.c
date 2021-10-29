#include <stdio.h>
#include <stdlib.h>
#include "environment.h"

FRAME* make_frame(FRAME* frames) {
    FRAME *new_frame = (FRAME*)malloc(sizeof(FRAME));
    if (new_frame==NULL) {
      perror("Cannot make frame");
      exit(1);
    }
    new_frame->bindings = NULL;
    new_frame->next = NULL;

    if(frames->next == NULL) {
        frames->next = new_frame;
    } else {
        FRAME *current_frame = frames;
        while (TRUE) {
            if(current_frame->next == NULL) {
                current_frame->next = new_frame;
                break;
            } else {
                current_frame = current_frame->next;
            }
        }
        
    }

    return new_frame;
}

BINDING* make_binding(FRAME* frames, char* name, VALUE* val) {
    BINDING *b = (BINDING*)malloc(sizeof(BINDING));
    if (b==NULL){
        perror("Cannot make binding");
        exit(1);
    }
    b->name = name;
    b->val = val;
    b->next = NULL;

    if(frames->bindings == NULL) {
        frames->bindings = b;
    } else {
        BINDING *current_binding = frames->bindings;
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