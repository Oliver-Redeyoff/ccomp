#include "nodes.h"

enum valueType {
  INTEGER_TYPE = 1,
  BOOLEAN_TYPE = 2,
  STRING_TYPE = 3,
  FUNCTION_TYPE = 4
};

typedef struct value {
  int          type;
  union {
    int integer;
    int boolean; // will need this soon
    char* string;
    struct func *function;
  } v;
} VALUE;

typedef struct func {
  int returnType;
  char* name;
  VALUE* params;
} FUNC;

typedef struct binding {
	TOKEN* name;
	VALUE* val;
	struct binding *next;
} BINDING;

typedef struct frame {
	BINDING* bindings;
	struct frame *next;
} FRAME;
