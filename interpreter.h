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

void declare_function(NODE* function_node, FRAME* current_frame);
void call_function(TOKEN* function_name_token, NODE* argument_values_node, FRAME* current_frame);

void declare_function_arguments_rec(NODE* current_arg_declaration_node, NODE* current_arg_value_node, FRAME* current_frame);
void declare_variables(NODE* current_node, FRAME* current_frame);
void declare_variables_inner(NODE* current_node, TOKEN* type_token, FRAME* current_frame);
void declare_variable(TOKEN* type_token, TOKEN* name_token, NODE* value_node, FRAME* current_frame);

VALUE* evaluate_expression(NODE* current_node, int result_type, FRAME* current_frame);
VALUE* interpret(NODE* current_node, FRAME* current_frame);
void call_main(FRAME* root_frame);

FRAME* extend_frame(FRAME* frame);
BINDING* add_binding(FRAME* frame, TOKEN* name_token, VALUE* val);

VALUE* get_value(TOKEN* search_name_token, int search_type, FRAME* frame);
