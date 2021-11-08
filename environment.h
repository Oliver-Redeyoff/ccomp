#include "nodes.h"

enum valueType {
  INTEGER_TYPE = 1,
  FUNCTION_TYPE = 2
};

typedef struct value {
  int          type;
  union {
    int integer;
    struct func *function;
  } v;
} VALUE;

typedef struct binding {
	TOKEN* name_token;
	VALUE* val;
	struct binding *next;
} BINDING;

typedef struct frame {
	BINDING* bindings;
	struct frame *next;
} FRAME;

typedef struct closure {
  FRAME* frame;
  NODE* code;
} CLOSURE;

typedef struct func {
  int returnType;
  TOKEN* token;
  VALUE* params;
  CLOSURE* closure;
} FUNC;

FRAME* make_frame(FRAME* frame);
BINDING* add_binding(FRAME* frame, TOKEN* name_token, VALUE* val);

VALUE* get_value(FRAME* frame, TOKEN* token);
