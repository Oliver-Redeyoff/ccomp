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

typedef struct func {
  int returnType;
  char* name;
  VALUE* params;
} FUNC;

typedef struct binding {
	char* name;
	VALUE* val;
	struct binding *next;
} BINDING;

typedef struct frame {
	BINDING* bindings;
	struct frame *next;
} FRAME;

FRAME* make_frame(FRAME* frames);
BINDING* make_binding(FRAME* frames, char* name, VALUE* val);
VALUE* get_value(FRAME* frames, char* name);
