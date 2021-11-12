#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "C.tab.h"
#include "interpreter.h"



void declare_function(NODE* function_node, FRAME* current_frame) {

  printf("declaring function\n");

  VALUE* clos_value = (VALUE*)malloc(sizeof(VALUE));
  CLOSURE* clos = (CLOSURE*)malloc(sizeof(CLOSURE));
  if (clos_value==NULL || clos==NULL) {
    perror("Cannot define function");
    exit(1);
  }

  clos_value->type = CLOSURE_TYPE;

  clos->declaration = function_node;
  clos->frame = current_frame;

  clos_value->v.closure = clos;

  NODE *function_name_node = function_node->left->right->left->left;
  TOKEN *function_name_token = (TOKEN *)function_name_node;

  add_binding(current_frame, function_name_token, clos_value);

}

void call_function(TOKEN* function_name_token, NODE* argument_values_node, FRAME* current_frame) {

  printf("Setting up function %s\n", function_name_token->lexeme);
  // find the closure for the function
  VALUE* clos_value = get_value(function_name_token, CLOSURE_TYPE, current_frame);
  CLOSURE* clo = clos_value->v.closure;

  // first need to extend current frame with function's frame
  current_frame = extend_frame(current_frame);

  if (argument_values_node != NULL) {
    // now populate it with function arguments
    NODE* current_arg_declaration_node = clo->declaration->left->right->right;
    NODE* current_arg_value_node = argument_values_node;
    
    declare_function_arguments_rec(current_arg_declaration_node, current_arg_value_node, current_frame);
  }

  // then interprete body of function
  printf("Interpreting function %s\n", function_name_token->lexeme);
  interpret(clo->declaration->right, current_frame);
}



void declare_function_arguments_rec(NODE* current_arg_declaration_node, NODE* current_arg_value_node, FRAME* current_frame) {

  if (current_arg_declaration_node->type == ',') {
    declare_function_arguments_rec(current_arg_declaration_node->left, current_arg_value_node->left, current_frame);
    declare_function_arguments_rec(current_arg_declaration_node->right, current_arg_value_node->right, current_frame);
  } else {
    if (current_arg_declaration_node->type == '~') {
      declare_variable((TOKEN*)current_arg_declaration_node->left, (TOKEN*)current_arg_declaration_node->right, current_arg_value_node, current_frame);
    } else {
      printf("ERROR: arguments do not match up with function declaration\n");
      exit(1);
    }
  }

}

void declare_variables(NODE* current_node, FRAME* current_frame) {
  // need to somehow use the type and inforce it
  TOKEN* type_token = (TOKEN*)current_node->left;

  declare_variables_inner(current_node->right, type_token, current_frame);
}

void declare_variables_inner(NODE* current_node, TOKEN* type_token, FRAME* current_frame) {

  if (current_node->type == ',') {
    declare_variables_inner(current_node->left, type_token, current_frame);
    declare_variables_inner(current_node->right, type_token, current_frame);
  } 
  else if (current_node->type == '=') {
    TOKEN* name_token = (TOKEN*)current_node->left->left;
    declare_variable(type_token, name_token, current_node->right, current_frame);
  }
  else if(current_node->type == LEAF) {
    TOKEN* name_token = (TOKEN*)current_node->left;
    declare_variable(type_token, name_token, NULL, current_frame);
  }

}

void declare_variable(TOKEN* type_token, TOKEN* name_token, NODE* value_node, FRAME* current_frame) {

  // create an empty value struct where we will store the value
  VALUE* variable_value = (VALUE*)malloc(sizeof(VALUE));

  // look at right to see if it is just declaration or also assignement
  if (value_node == NULL) {
    variable_value->v.integer = 0;
  } else {
    variable_value = evaluate_expression(value_node, INTEGER_TYPE, current_frame);
  }

  printf("Declaring variable %s with value %d\n", name_token->lexeme, variable_value->v.integer);

  BINDING* test = add_binding(current_frame, name_token, variable_value);
} 



VALUE* evaluate_expression(NODE* current_node, int result_type, FRAME* current_frame) {
  int node_type = current_node->type;
  VALUE* res = (VALUE*)malloc(sizeof(VALUE));
  res->type = result_type;

  if (node_type == LEAF) {
    TOKEN* value_token = (TOKEN*)current_node->left;
    res->type = result_type;
    res->v.integer = value_token->value;
    return res;
  }

  VALUE* left_res = evaluate_expression(current_node->left, result_type, current_frame);
  VALUE* right_res = evaluate_expression(current_node->right, result_type, current_frame);

  switch (node_type) {
    case '+':
      res->v.integer = left_res->v.integer + right_res->v.integer;
      break;

    case '-':
      res->v.integer = left_res->v.integer - right_res->v.integer;
      break;

    case '*':
      res->v.integer = left_res->v.integer * right_res->v.integer;
      break;

    case '/':
      res->v.integer = left_res->v.integer / right_res->v.integer;
      break;

    case '%':
      res->v.integer = left_res->v.integer % right_res->v.integer;
      break;

    case EQ_OP:
      res->v.integer = left_res->v.integer == right_res->v.integer;
      break;

    case NE_OP:
      res->v.integer = left_res->v.integer != right_res->v.integer;
      break;

    case '>':
      res->v.integer = left_res->v.integer > right_res->v.integer;
      break;

    case GE_OP:
      res->v.integer = left_res->v.integer >= right_res->v.integer;
      break;

    case '<':
      res->v.integer = left_res->v.integer < right_res->v.integer;
      break;

    case LE_OP:
      res->v.integer = left_res->v.integer <= right_res->v.integer;
      break;
    
    default:
      printf("ERROR: operator is not recognised\n");
      exit(0);
      break;
  }

  return res;
}

VALUE* interpret(NODE* current_node, FRAME* current_frame) {
  int node_type = current_node->type;

  switch (node_type) {
    case '~':
      if (current_node->left->type == LEAF) {
        declare_variables(current_node, current_frame);
      } else {
        interpret(current_node->left, current_frame);
        interpret(current_node->right, current_frame);
      }
      break;

    case 'D':
      declare_function(current_node, current_frame);
      break;

    case ';':
      interpret(current_node->left, current_frame);
      interpret(current_node->right, current_frame);
      break;

    case APPLY:
      call_function((TOKEN*)current_node->left->left, current_node->right, current_frame);
      
    case RETURN:
      return evaluate_expression(current_node->left, INTEGER_TYPE, current_frame);

    default:
      printf("Token type %d is not recognised by interpreter\n", node_type);
      break;
  }

  return NULL;
}



// Add a new frame which extends a existing frame
FRAME* extend_frame(FRAME* frame) {
    FRAME* new_frame = (FRAME*)malloc(sizeof(FRAME));
    if (new_frame==NULL) {
      printf("ERROR: cannot create new frame frame\n");
      exit(1);
    }

    new_frame->bindings = NULL;
    new_frame->next = frame;

    return new_frame;
}

// Add a new binding to a frame
BINDING* add_binding(FRAME* frame, TOKEN* name_token, VALUE* val) {
    BINDING* b = (BINDING*)malloc(sizeof(BINDING));
    if (b==NULL){
        printf("ERROR: cannot create binding for %s\n", name_token->lexeme);
        exit(1);
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

// Get value associated with a token, starting at a given frame and working back to the root node until we find the binding
VALUE* get_value(TOKEN* search_name_token, int search_type, FRAME* frame) {
    FRAME* current_frame = frame;
    int found_binding = FALSE;

    while (TRUE) {
        BINDING* current_binding = current_frame->bindings;

        // search through bindings for frame
        while(TRUE) {
            if (current_binding->name_token == search_name_token && current_binding->val->type == search_type) {
                return current_binding->val;
            } else {
                if (current_binding->next == NULL) {
                    break;
                } else {
                    current_binding = current_binding->next;
                }
            }
        }

        // if not found look at parent frame
        if (current_frame->next == NULL) {
            printf("ERROR: could not find binding %s\n", search_name_token->lexeme);
            exit(0);
        } else {
            current_frame = current_frame->next;
        }
    }

}