#include <stdio.h>
#include <stdlib.h>
#include "environment.h"

FRAME* make_frame(BINDING* initial) {
    FRAME *f = (FRAME*)malloc(sizeof(FRAME));
    if (f==NULL) {
      perror("Cannot make frame");
      exit(1);
    }
    f->bindings = NULL;
    f->next = NULL;
}