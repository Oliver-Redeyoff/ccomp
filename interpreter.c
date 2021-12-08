#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "C.tab.h"
#include "interpreter.h"

VALUE* global_interpret_result = NULL;
char global_error[100] = "";
int global_interupt = 0;
char* built_in_functions[3] = {"print_string", "print_int", "read_int"};


// Creates a new closure in given frame
void declare_function(NODE* function_node, FRAME* current_frame) {

  VALUE* clos_value = (VALUE*)malloc(sizeof(VALUE));
  CLOSURE* clos = (CLOSURE*)malloc(sizeof(CLOSURE));
  if (clos_value==NULL || clos==NULL) {
    strcpy(global_error, "Cannot define function");
    global_interupt = ERROR_INTERUPT;
    return;
  }

  clos_value->type = CLOSURE_TYPE;

  clos->declaration = function_node;
  clos->frame = current_frame;

  clos_value->v.closure = clos;

  NODE *function_name_node = function_node->left->right->left->left;
  TOKEN *function_name_token = (TOKEN *)function_name_node;

  // check the name doesn't conflict with an in-built function
  if (is_inbuilt_function(function_name_token) == 1) {
    strcpy(global_error, "Function name can't be the same as inbuilt function name");
    global_interupt = ERROR_INTERUPT;
    return;
  }

  add_binding(current_frame, function_name_token, clos_value);

  return;

}

// check if function name is the same as an existing inbuilt one
int is_inbuilt_function(TOKEN* function_name_token) {

  int len = sizeof(built_in_functions) / sizeof(built_in_functions[0]);

  for (int i=0 ; i<len ; i++) {
    if (strcmp(function_name_token->lexeme, built_in_functions[i]) == 0) {
      return 1;
    } 
  }

  return 0;

}

// Creates new frame for function, initialises arguments and then interpretes the body
VALUE* call_function(TOKEN* function_name_token, NODE* argument_values_node, FRAME* current_frame) {

  // if is inbuilt function, run custom code
  if (is_inbuilt_function(function_name_token) == 1) {

    if (strcmp(function_name_token->lexeme, built_in_functions[0]) == 0) {
      print_string_builtin(argument_values_node);
      return NULL;
    }
    else if (strcmp(function_name_token->lexeme, built_in_functions[1]) == 0) {
      print_int_builtin(argument_values_node, current_frame);
      return NULL;
    }
    else if (strcmp(function_name_token->lexeme, built_in_functions[2]) == 0) {
      return input_int_builtin();
    }

  }

  printf("Setting up function %s\n", function_name_token->lexeme);

  // find the closure for the function
  VALUE* clos_value = get_value(function_name_token, CLOSURE_TYPE, current_frame);
  CLOSURE* clos = clos_value->v.closure;

  // first need to extend current frame with function declaration's frame
  FRAME* new_frame = extend_frame(clos->frame);

  if (argument_values_node != NULL) {
    // now populate it with function arguments
    NODE* current_arg_declaration_node = clos->declaration->left->right->right;
    NODE* current_arg_value_node = argument_values_node;
    
    declare_function_arguments_rec(current_arg_declaration_node, current_arg_value_node, new_frame, current_frame);
  }

  // then interprete body of function
  printf("Interpreting function %s\n", function_name_token->lexeme);
  interpret_rec(clos->declaration->right, new_frame);

  // get returned value and reset global value and interupt
  VALUE* func_return_value = (VALUE*)malloc(sizeof(VALUE));
  if (global_interupt == RETURN_INTERUPT) {
    memcpy(func_return_value, global_interpret_result, sizeof(VALUE));
    global_interpret_result = NULL;
    global_interupt = NO_INTERUPT;
  }

  if (func_return_value == NULL) {
    printf("Function %s returning null\n", function_name_token->lexeme);
  }
  else if (func_return_value->type == INTEGER_TYPE) {
    printf("Function %s returning %d\n", function_name_token->lexeme, func_return_value->v.integer);
  }
  else if (func_return_value->type == CLOSURE_TYPE) {
    printf("Function %s returning function\n", function_name_token->lexeme);
  }
  else {
    printf("Function %s returning something elseof type %d\n", function_name_token->lexeme, func_return_value->type);
  }

  return func_return_value;

}

// Builtin print string function
void print_string_builtin(NODE* argument_values_node) {

  if (argument_values_node->type == LEAF) {
    TOKEN* string_token = (TOKEN*)argument_values_node->left;
    if (string_token->type == STRING_LITERAL) {
      printf("%s", string_token->lexeme);
    }
    else if (string_token->type == IDENTIFIER) {
      strcpy(global_error, "Print string should have a string as argument");
      global_interupt = ERROR_INTERUPT;
      return;
    }
  } else {
    strcpy(global_error, "Print string only takes one argument");
    global_interupt = ERROR_INTERUPT;
    return;
  }

  return;

}

// Builtin print int function
void print_int_builtin(NODE* argument_values_node, FRAME* current_frame) {
  if (argument_values_node->type == LEAF) {
    TOKEN* string_token = (TOKEN*)argument_values_node->left;
    if (string_token->type == STRING_LITERAL) {
      strcpy(global_error, "Print int should have a identifier as argument");
      global_interupt = ERROR_INTERUPT;
      return;
    }
    else if (string_token->type == IDENTIFIER) {
      VALUE* var_value = get_value(string_token, INTEGER_TYPE, current_frame);
      printf("%d", var_value->v.integer);
    }
  } else {
    strcpy(global_error, "Prints only takes one argument");
    global_interupt = ERROR_INTERUPT;
    return;
  }

  return;
}

// Builtin input int function
VALUE* input_int_builtin() {

  VALUE* input = (VALUE*)malloc(sizeof(VALUE));
  if (input == NULL) {
    strcpy(global_error, "Could not allocate space for new value");
    global_interupt = ERROR_INTERUPT;
    return NULL;
  }

  input->type = INTEGER_TYPE;

  // get input
  int input_val = 0;
  
  // clear stdin
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { }

  printf("User input : ");
  scanf("%d", &input_val);
  //fgets(buf, 12, stdin);
  printf("Input of user was %d\n", input_val);

  input->v.integer = input_val;

  return input;

}


// Evaluates if expression, then creates new frame for block which is to be executed if there is one
void if_statement(NODE* if_node, FRAME* current_frame) {

  VALUE* if_condition = evaluate_expression(if_node->left, current_frame);
  
  NODE* block_to_interpret = NULL;

  if (if_condition->v.integer == 1) {
    if (if_node->right->type == ELSE) {
      block_to_interpret = if_node->right->left;
    } else {
      block_to_interpret = if_node->right;
    }
  } else {
    if (if_node->right->type == ELSE) {
      block_to_interpret = if_node->right->right;
    } else {
      return;
    }
  }

  // extend current frame for the block to use
  FRAME* new_frame = extend_frame(current_frame);

  interpret_rec(block_to_interpret, new_frame);

  return;

}

// While loop
void while_statement(NODE* while_node, FRAME* current_frame) {

  printf("Entering loop\n");

  while (1) {

    printf("Looping\n");
    // if loop condition is false exit loop
    VALUE* loop_condition = evaluate_expression(while_node->left, current_frame);
    if (loop_condition->v.integer == 0) {
      break;
    }

    // if break interupt, resolve interupt and exit loop
    if (global_interupt == BREAK_INTERUPT) {
      global_interupt = NO_INTERUPT;
      break;
    }

    // if return interupt, exit loop
    if (global_interupt == RETURN_INTERUPT) {
      break;
    }

    // if continue interupt, go to next loop
    if (global_interupt == CONTINUE_INTERUPT) {
      continue;
    }

    FRAME* new_frame = extend_frame(current_frame);

    interpret_rec(while_node->right, new_frame);

  }

  return;

}


// Initialises function arguments recursively when it is called
void declare_function_arguments_rec(NODE* current_arg_declaration_node, NODE* current_arg_value_node, FRAME* destination_frame, FRAME* lookup_frame) {

  if (current_arg_declaration_node->type == ',') {
    declare_function_arguments_rec(current_arg_declaration_node->left, current_arg_value_node->left, destination_frame, lookup_frame);
    declare_function_arguments_rec(current_arg_declaration_node->right, current_arg_value_node->right, destination_frame, lookup_frame);
  } 
  else if (current_arg_declaration_node->type == '~') {
    declare_variable((TOKEN*)current_arg_declaration_node->left->left, (TOKEN*)current_arg_declaration_node->right->left, current_arg_value_node, destination_frame, lookup_frame);
  } 
  else {
    strcpy(global_error, "Arguments do not match up with function declaration");
    global_interupt = ERROR_INTERUPT;
    return;
  }

  return;

}

// Declare a list of variables recusively
void declare_variables(NODE* initial_node, NODE* current_node, TOKEN* type_token, FRAME* current_frame) {

  if (current_node->type == ',') {
    declare_variables(initial_node, current_node->left, type_token, current_frame);
    declare_variables(initial_node, current_node->right, type_token, current_frame);
  } 
  else if (current_node->type == '=') {
    TOKEN* name_token = (TOKEN*)current_node->left->left;
    declare_variable(type_token, name_token, current_node->right, current_frame, current_frame);
  }
  else if (current_node->type == LEAF) {
    // if this was a list of declarations, need to check initial_node to assignement value
    // else declare with default assignement value
    if (initial_node->right->type == ',' && initial_node->right->right->type == '=') {
      TOKEN* name_token = (TOKEN*)current_node->left;
      declare_variable(type_token, name_token, initial_node->right->right->right, current_frame, current_frame);
    } else {
      TOKEN* name_token = (TOKEN*)current_node->left;
      declare_variable(type_token, name_token, NULL, current_frame, current_frame);
    }
  }

  return;

}

// Backend for declaring variable
void declare_variable(TOKEN* type_token, TOKEN* name_token, NODE* value_node, FRAME* destination_frame, FRAME* lookup_frame) {

  // create an empty value struct where we will store the value
  VALUE* variable_value = (VALUE*)malloc(sizeof(VALUE));

  // get type of variable
  int variable_type = ANY_TYPE;
  if (type_token->type == INT) {
    variable_type = INTEGER_TYPE;
  }
  else if (type_token->type == FUNCTION) {
    variable_type = CLOSURE_TYPE;
  }
  else {
    strcpy(global_error, "Value type not recognised");
    global_interupt = ERROR_INTERUPT;
    return;
  }
  variable_value->type = variable_type;

  // look at right to see if it is just declaration or also assignement
  if (value_node == NULL) {
    if (variable_value->type == INTEGER_TYPE) {
      variable_value->v.integer = 0;
    }
    else if (variable_value->type == CLOSURE_TYPE) {
      strcpy(global_error, "Can't define null function");
      global_interupt = ERROR_INTERUPT;
      return;
    }
  } else {
    VALUE* evaluated_value = evaluate_expression(value_node, lookup_frame);
    if (evaluated_value->type == variable_value->type) {
      variable_value = evaluated_value;
    } else {
      strcpy(global_error, "Evaluated value type doesn't match variable type");
      global_interupt = ERROR_INTERUPT;
      return;
    }
  }

  if (variable_value->type == INTEGER_TYPE) {
    printf("Declaring int variable %s with value %d\n", name_token->lexeme, variable_value->v.integer);
  }
  else if (variable_value->type == CLOSURE_TYPE) {
    printf("Declaring function variable %s\n", name_token->lexeme);
  }
  
  BINDING* test = add_binding(destination_frame, name_token, variable_value);

  return;

} 

// Variable assignements frontend
void assign_variables(NODE* initial_node, NODE* current_node, FRAME* current_frame) {
  
  if (current_node->type == ',') {
    assign_variables(initial_node, current_node->left, current_frame);
    assign_variables(initial_node, current_node->right, current_frame);
  } 
  else if (current_node->type == '=') {
    TOKEN* name_token = (TOKEN*)current_node->left->left;
    assign_variable(name_token, current_node->right, current_frame);
  }
  else if (current_node->type == LEAF) {
    // if this was a list of declarations, need to check initial_node to assignement value
    // else declare with default assignement value
    if (initial_node->type == ',' && initial_node->right->type == '=') {
      TOKEN* name_token = (TOKEN*)current_node->left;
      assign_variable(name_token, initial_node->right->right->right, current_frame);
    } else {
      strcpy(global_error, "Error with assignement of variables");
      global_interupt = ERROR_INTERUPT;
      return;
    }
  }

  return;

}

// Variable assignement backend
void assign_variable(TOKEN* name_token, NODE* value_node, FRAME* current_frame) {

  BINDING* binding = find_binding(name_token, current_frame);
  VALUE* new_value = evaluate_expression(value_node, current_frame);
  binding->val = new_value;

  printf("Assigning variable %s with value %d\n", name_token->lexeme, new_value->v.integer);

  return;

}


// Recursively interpret starting at a given node
void interpret_rec(NODE* current_node, FRAME* current_frame) {

  if (global_interupt != NO_INTERUPT) {
    return;
  }

  int node_type = current_node->type;

  switch (node_type) {

    case ';':
      interpret_rec(current_node->left, current_frame);
      interpret_rec(current_node->right, current_frame);
      break;

    case 'D':
      declare_function(current_node, current_frame);
      break;

    case '~':
      if (current_node->left->type == LEAF) {
        declare_variables(current_node, current_node->right, (TOKEN*)current_node->left->left, current_frame);
      } else {
        interpret_rec(current_node->left, current_frame);
        interpret_rec(current_node->right, current_frame);
      }
      break;

    case '=':
      assign_variables(current_node, current_node, current_frame);
      break;

    case ',':
      assign_variables(current_node, current_node, current_frame);
      break;

    case APPLY:
      call_function((TOKEN*)current_node->left->left, current_node->right, current_frame);
      break;

    case RETURN: {
      VALUE* return_value = evaluate_expression(current_node->left, current_frame);
      global_interpret_result = return_value;
      global_interupt = RETURN_INTERUPT;
      break;

    case IF:
      if_statement(current_node, current_frame);
      break;

    case WHILE:
      while_statement(current_node, current_frame);
      break;

    case BREAK:
      global_interupt = BREAK_INTERUPT;
      break;

    case CONTINUE:
      global_interupt = CONTINUE_INTERUPT;
      break;
      
    }

    default:
      strcpy(global_error, "Token type is not recognised by interpreter");
      global_interupt = ERROR_INTERUPT;
      return;
  }

  return;

}

// Evaluates an expression and returns the value, can probably merge this into interpret though
VALUE* evaluate_expression(NODE* current_node, FRAME* current_frame) {

  if (current_node == NULL) {
    return NULL;
  }

  int node_type = current_node->type;
  VALUE* res = (VALUE*)malloc(sizeof(VALUE));

  switch (node_type) {

    case LEAF: {
      TOKEN* value_token = (TOKEN*)current_node->left;
      if (value_token->type == CONSTANT) {
        res->type = INTEGER_TYPE;
        res->v.integer = value_token->value;
        return res;
      }
      else if (value_token->type == IDENTIFIER) {
        return get_value(value_token, ANY_TYPE, current_frame);
      }
      else {
      }
    }

    case '+': {
      VALUE* left_res = evaluate_expression(current_node->left, current_frame);
      VALUE* right_res = evaluate_expression(current_node->right, current_frame);
      res->type = INTEGER_TYPE;
      res->v.integer = left_res->v.integer + right_res->v.integer;
      break;
    }

    case '-': {
      VALUE* left_res = evaluate_expression(current_node->left, current_frame);
      VALUE* right_res = evaluate_expression(current_node->right, current_frame);
      res->type = INTEGER_TYPE;
      res->v.integer = left_res->v.integer - right_res->v.integer;
      break;
    }

    case '*': {
      VALUE* left_res = evaluate_expression(current_node->left, current_frame);
      VALUE* right_res = evaluate_expression(current_node->right, current_frame);
      res->type = INTEGER_TYPE;
      res->v.integer = left_res->v.integer * right_res->v.integer;
      break;
    }

    case '/': {
      VALUE* left_res = evaluate_expression(current_node->left, current_frame);
      VALUE* right_res = evaluate_expression(current_node->right, current_frame);
      res->type = INTEGER_TYPE;
      res->v.integer = left_res->v.integer / right_res->v.integer;
      break;
    }

    case '%': {
      VALUE* left_res = evaluate_expression(current_node->left, current_frame);
      VALUE* right_res = evaluate_expression(current_node->right, current_frame);
      res->type = INTEGER_TYPE;
      res->v.integer = left_res->v.integer % right_res->v.integer;
      break;
    }

    case EQ_OP: {
      VALUE* left_res = evaluate_expression(current_node->left, current_frame);
      VALUE* right_res = evaluate_expression(current_node->right, current_frame);
      res->type = INTEGER_TYPE;
      res->v.integer = left_res->v.integer == right_res->v.integer;
      break;
    }

    case NE_OP: {
      VALUE* left_res = evaluate_expression(current_node->left, current_frame);
      VALUE* right_res = evaluate_expression(current_node->right, current_frame);
      res->type = INTEGER_TYPE;
      res->v.integer = left_res->v.integer != right_res->v.integer;
      break;
    }

    case '>': {
      VALUE* left_res = evaluate_expression(current_node->left, current_frame);
      VALUE* right_res = evaluate_expression(current_node->right, current_frame);
      res->type = INTEGER_TYPE;
      res->v.integer = left_res->v.integer > right_res->v.integer;
      break;
    }

    case GE_OP: {
      VALUE* left_res = evaluate_expression(current_node->left, current_frame);
      VALUE* right_res = evaluate_expression(current_node->right, current_frame);
      res->type = INTEGER_TYPE;
      res->v.integer = left_res->v.integer >= right_res->v.integer;
      break;
    }

    case '<': {
      VALUE* left_res = evaluate_expression(current_node->left, current_frame);
      VALUE* right_res = evaluate_expression(current_node->right, current_frame);
      res->type = INTEGER_TYPE;
      res->v.integer = left_res->v.integer < right_res->v.integer;
      break;
    }

    case LE_OP: {
      VALUE* left_res = evaluate_expression(current_node->left, current_frame);
      VALUE* right_res = evaluate_expression(current_node->right, current_frame);
      res->type = INTEGER_TYPE;
      res->v.integer = left_res->v.integer <= right_res->v.integer;
      break;
    }
    
    case APPLY:
      return call_function((TOKEN*)current_node->left->left, current_node->right, current_frame);

    default:
      printf("erroring here %c\n", node_type);
      strcpy(global_error, "Operator is not recognised");
      global_interupt = ERROR_INTERUPT;
      return NULL;

  }

  return res;

}

// Finds main method and calls it
void call_main(FRAME* root_frame) {

  BINDING* current_binding = root_frame->bindings;

  while (TRUE) {
    if (strcmp(current_binding->name_token->lexeme, "main") == 0){
      call_function(current_binding->name_token, NULL, root_frame);
      break;
    } else {
      if (current_binding->next == NULL) {
        strcpy(global_error, "No main function is defined in root scope");
        global_interupt = ERROR_INTERUPT;
        return;
      } else {
        current_binding = current_binding->next;
      }
    }
  }

  return;

}

// Entry point for interpreter
VALUE* interpret(NODE* tree) {

  // Create root frame
  FRAME* root_frame = (FRAME*)malloc(sizeof(FRAME));

  // Populate root frame with global bindings
  interpret_rec(tree, root_frame);

  // Add global bindings for builtin functions
  int len = sizeof(built_in_functions) / sizeof(built_in_functions[0]);
  for (int i=0 ; i<len ; i++) {
    TOKEN* builtin_token = lookup_token(built_in_functions[i]);
    VALUE* builtin_closure_value = (VALUE*)malloc(sizeof(VALUE));
    builtin_closure_value->type = CLOSURE_TYPE;
    add_binding(root_frame, builtin_token, builtin_closure_value);
  }

  // Call main method
  call_main(root_frame);

  if (global_interupt == ERROR_INTERUPT) {
    printf("ERROR: %s\n", global_error);
  }

  return NULL;

}



// Add a new frame which extends a existing frame
FRAME* extend_frame(FRAME* frame) {

  FRAME* new_frame = (FRAME*)malloc(sizeof(FRAME));
  if (new_frame==NULL) {
    strcpy(global_error, "Cannot create new frame frame");
    global_interupt = ERROR_INTERUPT;
    return NULL;
  }

  new_frame->bindings = NULL;
  new_frame->next = frame;

  return new_frame;

}

// Add a new binding to a frame
BINDING* add_binding(FRAME* frame, TOKEN* name_token, VALUE* val) {

  BINDING* b = (BINDING*)malloc(sizeof(BINDING));
  if (b==NULL){
    strcpy(global_error, "Cannot create binding");
    global_interupt = ERROR_INTERUPT;
    return NULL;
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

// Find a binding for a token starting at a given frame
BINDING* find_binding(TOKEN* search_token, FRAME* frame) {

  FRAME* current_frame = frame;

  while (TRUE) {
    BINDING* current_binding = current_frame->bindings;

    // search through bindings for frame
    while(TRUE) {

      if (current_binding == NULL) {
        break;
      }
      //printf("looking at token %s for search token %s\n", current_binding->name_token->lexeme, search_token->lexeme);
      if (current_binding->name_token == search_token) {
        return current_binding;
      } else {
        current_binding = current_binding->next;
      }
    }

    // if not found look at parent frame
    if (current_frame->next == NULL) {
      strcpy(global_error, "Could not find binding");
      global_interupt = ERROR_INTERUPT;
      return NULL;
    } else {
      current_frame = current_frame->next;
    }
  }

  return NULL;

}

// Get value associated with a token, starting at a given frame and working back to the root node until we find the binding
VALUE* get_value(TOKEN* search_token, int search_type, FRAME* frame) {

  BINDING* binding = find_binding(search_token, frame);

  if (binding == NULL) {
    return NULL;
  }

  if (search_type == ANY_TYPE || binding->val->type == search_type) {
    return binding->val;
  }

  return NULL;

}