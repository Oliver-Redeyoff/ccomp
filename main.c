#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "C.tab.h"
#include "environment.h"
#include <string.h>

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

void print_leaf(NODE *tree, int level) {
    TOKEN *t = (TOKEN *)tree;
    int i;
    for (i=0; i<level; i++) putchar(' ');
    if (t->type == CONSTANT) printf("leaf: %d\n", t->value);
    else if (t->type == STRING_LITERAL) printf("leaf: \"%s\"\n", t->lexeme);
    else if (t) puts(t->lexeme);
}

void print_tree0(NODE *tree, int level) {
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

void print_tree(NODE *tree) {
    print_tree0(tree, 0);
}

void declare_function(NODE *function_node) {

  printf("declaring function\n");

  // get the definition from the left node, and the body of the function from the right node
  NODE *function_definition = function_node->left;
  NODE *function_body = function_node->right;

  // get return type, name and parameters
  NODE *function_return_type_node = function_definition->left->left;
  TOKEN *function_return_type_token = (TOKEN *)function_return_type_node;

  NODE *function_name_node = function_definition->right->left->left;
  TOKEN *function_name_token = (TOKEN *)function_name_node;

  VALUE* f = (VALUE*)malloc(sizeof(VALUE));
  if (f==NULL) {
    perror("Cannot define function");
    exit(1);
  }

  f->type = FUNCTION_TYPE;
  f->v.function->name = "testing";

}

void interprete(NODE *tree) {
  int node_type = tree->type;

  switch (node_type) {
    case '~':
      // this is for a variable or method declaration
      printf("%s\n", named(tree->left->type));
      break;

    case 'D':
      declare_function(tree);
      break;
      
    default:
      printf("Token is not recognised by interpreter");
      break;
  }

}

extern int yydebug;
extern NODE* yyparse(void);
extern NODE* ans;
extern void init_symbtable(void);

extern FRAME* environment;

int main(int argc, char** argv) {
    NODE* tree;
    if (argc>1 && strcmp(argv[1],"-d")==0) yydebug = 1;
    init_symbtable();
    printf("--C COMPILER\n");
    
    // Parse inputed program into AST
    yyparse();
    tree = ans;
    printf("parse finished with %p\n", tree);
    print_tree(tree);
    
    // Interprete result of the program from AST
    interprete(tree);
    
    return 0;
}
