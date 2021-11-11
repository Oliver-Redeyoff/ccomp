#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "C.tab.h"
#include "environment.h"
#include <string.h>

extern int yydebug;
extern NODE* yyparse(void);
extern NODE* ans;
extern void init_symbtable(void);

void declare_function(NODE* function_node, FRAME* current_frame);
void call_function(TOKEN* function_name_token, NODE* argument_values_node, FRAME* current_frame);

void declare_function_arguments_rec(NODE* current_arg_declaration_node, NODE* current_arg_value_node, FRAME* current_frame);
void declare_variables(NODE* current_node, FRAME* current_frame);
void declare_variables_inner(NODE* current_node, TOKEN* type_token, FRAME* current_frame);
void declare_variable(TOKEN* type_token, TOKEN* name_token, NODE* value_node, FRAME* current_frame);

VALUE* evaluate_expression(NODE* current_node, FRAME* current_frame);
VALUE* interpret(NODE* current_node, FRAME* current_frame);


/////////////////////////
// Visualisation stuff //
/////////////////////////
char *named(int t) {
    static char b[100];
    if (isgraph(t) || t==' ') {
      sprintf(b, "%c", t);
      return b;
    }
    switch (t) {
      default: return "???";
    case IDENTIFIER:
      return "id";
    case CONSTANT:
      return "constant";
    case STRING_LITERAL:
      return "string";
    case LE_OP:
      return "<=";
    case GE_OP:
      return ">=";
    case EQ_OP:
      return "==";
    case NE_OP:
      return "!=";
    case EXTERN:
      return "extern";
    case AUTO:
      return "auto";
    case INT:
      return "int";
    case VOID:
      return "void";
    case APPLY:
      return "apply";
    case LEAF:
      return "leaf";
    case IF:
      return "if";
    case ELSE:
      return "else";
    case WHILE:
      return "while";
    case CONTINUE:
      return "continue";
    case BREAK:
      return "break";
    case RETURN:
      return "return";
    }
}

void print_leaf(NODE* tree, int level) {
    TOKEN* t = (TOKEN*)tree;
    int i;
    for (i=0; i<level; i++) putchar(' ');
    if (t->type == CONSTANT) printf("leaf: %d\n", t->value);
    else if (t->type == STRING_LITERAL) printf("leaf: \"%s\"\n", t->lexeme);
    else if (t) puts(t->lexeme);
}

void print_tree0(NODE* tree, int level) {
    int i;
    if (tree==NULL) return;
    if (tree->type==LEAF) {
      print_leaf(tree->left, level);
    }
    else {
      for(i=0; i<level; i++) putchar(' ');
      printf("%s\n", named(tree->type));
/*       if (tree->type=='~') { */
/*         for(i=0; i<level+2; i++) putchar(' '); */
/*         printf("%p\n", tree->left); */
/*       } */
/*       else */
        print_tree0(tree->left, level+2);
      print_tree0(tree->right, level+2);
    }
}

void print_tree(NODE* tree) {
    print_tree0(tree, 0);
}


////////////////////////
// Interpreting logic //
////////////////////////
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
    variable_value = evaluate_expression(value_node, current_frame);
  }

  printf("Declaring variable %s with value %d\n", name_token->lexeme, variable_value->v.integer);

  BINDING* test = add_binding(current_frame, name_token, variable_value);
} 



VALUE* evaluate_expression(NODE* current_node, FRAME* current_frame) {
  VALUE* variable_value = (VALUE*)malloc(sizeof(VALUE));
  variable_value->v.integer = 0;

  return variable_value;
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
      return NULL;

    default:
      printf("Token type %d is not recognised by interpreter\n", node_type);
      break;
  }

  return NULL;
}


//////////
// Main //
//////////
int main(int argc, char** argv) {
    
    NODE* tree;
    FRAME* root_frame = (FRAME*)malloc(sizeof(FRAME));

    if (argc>1 && strcmp(argv[1],"-d")==0) yydebug = 1;
    init_symbtable();
    printf("--C COMPILER\n");
    
    // Parse inputed program into AST
    yyparse();
    tree = ans;
    printf("parse finished with %p\n", tree);
    print_tree(tree);
    
    // Interprete result of the program from AST
    interpret(tree, root_frame);

    // Look through all bindings in the root frame, and call main function
    BINDING* current_binding = root_frame->bindings;
    while (TRUE) {

      printf("lexeme: %s\n", current_binding->name_token->lexeme);

      if (strcmp(current_binding->name_token->lexeme, "main") == 0){
        call_function(current_binding->name_token, NULL, root_frame);
        break;
      } else {
        if (current_binding->next == NULL) {
          printf("no main function is defined\n");
          break;
        } else {
          current_binding = current_binding->next;
        }
      }

    }
    
    return 0;
}
