#include "nodes.h"

enum valueType {
  INTEGER_TYPE = 1,
  CLOSURE_TYPE = 2
};

typedef struct value {
  int          type;
  union {
    int integer;
    struct closure* closure;
  } v;
} VALUE;

typedef struct binding {
	TOKEN* name_token;
	VALUE* val;
	struct binding* next;
} BINDING;

typedef struct frame {
	BINDING* bindings;
	struct frame* next;
} FRAME;

typedef struct closure {
  FRAME* frame;
  NODE* declaration;
} CLOSURE;

FRAME* extend_frame(FRAME* frame);
BINDING* add_binding(FRAME* frame, TOKEN* name_token, VALUE* val);

VALUE* get_value(TOKEN* search_name_token, int search_type, FRAME* frame);
