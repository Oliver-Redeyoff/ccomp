#ifndef _INTERPRETER_H_ 
#define _INTERPRETER_H_


#include <stdlib.h>
#include "nodes.h"


enum blockType {
  GLOBAL_BLOCK = 0,
  FUNCTION_BLOCK = 1,
  IF_BLOCK = 2,
};

enum interuptType {
  NO_INTERUPT = 0,
  RETURN_INTERUPT = 1,
  BREAK_INTERUPT = 2,
  CONTINUE_INTERUPT = 3,
  ERROR_INTERUPT = 4
};

enum valueType {
  ANY_TYPE = 0,
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


extern TOKEN* lookup_token(char*);

VALUE* interpret(NODE* tree);
void interpret_rec(NODE* current_node, FRAME* current_frame);
VALUE* evaluate_expression(NODE* current_node, FRAME* current_frame);
void call_main(FRAME* root_frame);

void declare_function(NODE* function_node, FRAME* current_frame);
VALUE* call_function(TOKEN* function_name_token, NODE* argument_values_node, FRAME* current_frame);
int is_inbuilt_function(TOKEN* function_name_token);
void print_string_builtin(NODE* argument_values_node);
void print_int_builtin(NODE* argument_values_node, FRAME* current_frame);
VALUE* input_int_builtin();

void if_statement(NODE* if_node, FRAME* current_frame);
void while_statement(NODE* while_node, FRAME* current_frame);

void declare_function_arguments_rec(NODE* current_arg_declaration_node, NODE* current_arg_value_node, FRAME* destination_frame, FRAME* lookup_frame);
void declare_variables(NODE* initital_node, NODE* current_node, TOKEN* type_token, FRAME* current_frame);
void declare_variable(TOKEN* type_token, TOKEN* name_token, NODE* value_node, FRAME* destination_frame, FRAME* lookup_frame);
void assign_variables(NODE* initial_node, NODE* current_node, FRAME* current_frame);
void assign_variable(TOKEN* name_token, NODE* value_node, FRAME* current_frame);


FRAME* extend_frame(FRAME* frame);
BINDING* add_binding(FRAME* frame, TOKEN* name_token, VALUE* val);

BINDING* find_binding(TOKEN* search_token, FRAME* frame);
VALUE* get_value(TOKEN* search_token, int search_type, FRAME* frame);


#endif