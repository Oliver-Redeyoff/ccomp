#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include "token.h"
#include "nodes.h"
#include "C.tab.h"
#include "interpreter.h"
#include "tac_generator.h"
#include "mips_generator.h"

extern int yydebug;
extern NODE* yyparse(void);
extern NODE* ans;
extern void init_symbtable(void);

char program_name[50];

// Prints intermediate code into file
void generate_tac_file(BASIC_BLOCK* current_BB) {

  char tac_file_name[80];
  strcpy(tac_file_name, "./testing/tac_results/");
  strcat(tac_file_name, program_name);
  strcat(tac_file_name, ".txt");

  FILE* fp = fopen(tac_file_name, "w+");

  char* operation_characters[12] = {"p", "+", "-", "*", "/", "%", "==", "!=", ">", ">=", "<", "<="};

  while (1) {
    TAC* current_TAC = current_BB->leader;

    while (1) {
      switch (current_TAC->type) {

        case LABEL_TAC_TYPE: 
          fprintf(fp, "label %s", current_TAC->v.tac_label.name->lexeme);
          break;

        case GOTO_TAC_TYPE: 
          fprintf(fp, "goto %s", current_TAC->v.tac_label.name->lexeme);
          break;

        case BLOCK_START_TAC_TYPE:
          fprintf(fp, "block %d start %s", current_TAC->v.tac_block_delimiter.block_type, current_TAC->v.tac_block_delimiter.name->lexeme, current_TAC->v.tac_block_delimiter.arity);
          if (current_TAC->v.tac_block_delimiter.parent_block_name != NULL) {
            fprintf(fp, " (%s)", current_TAC->v.tac_block_delimiter.parent_block_name->lexeme);
          }
          fprintf(fp, "\n");
          break;

        case BLOCK_END_TAC_TYPE:
          fprintf(fp, "block %d end %s %d", current_TAC->v.tac_block_delimiter.block_type, current_TAC->v.tac_block_delimiter.name->lexeme, current_TAC->v.tac_block_delimiter.arity);
          if (current_TAC->v.tac_block_delimiter.parent_block_name != NULL) {
            fprintf(fp, " (%s)", current_TAC->v.tac_block_delimiter.parent_block_name->lexeme);
          }
          fprintf(fp, "\n");
          break;

        case FUNCTION_CALL_TAC_TYPE:
          fprintf(fp, "call %s\n", current_TAC->v.tac_function_call.name->lexeme);
          break;

        case BUILTIN_CALL_TAC_TYPE:
          fprintf(fp, "call builtin %d", current_TAC->v.tac_builtin_call.type);
          if (current_TAC->v.tac_builtin_call.argument != NULL) {
            fprintf(fp, ", %s\n", current_TAC->v.tac_builtin_call.argument->lexeme);
          } else {
            fprintf(fp, "\n");
          }
          break;

        case IF_TAC_TYPE: {
          TAC_IF tac_if = current_TAC->v.tac_if;
          fprintf(fp, "if ");
          if (tac_if.condition_result->type == IDENTIFIER) { fprintf(fp, "%s ", tac_if.condition_result->lexeme); }
          else if (tac_if.condition_result->type == TEMPORARY_REG_IDENTIFIER) { fprintf(fp, "t%d ", tac_if.condition_result->value); }
          else if (tac_if.condition_result->type == ARGUMENT_REG_IDENTIFIER) { fprintf(fp, "a%d ", tac_if.condition_result->value); }
          else if (tac_if.condition_result->type == RETURN_REG_IDENTIFIER) { fprintf(fp, "v%d ", tac_if.condition_result->value); }
          fprintf(fp, "%s\n", tac_if.jump_label->lexeme);
          break;
        }

        case RETURN_TAC_TYPE:
          fprintf(fp, "return\n");
          break;

        case EXIT_PROGRAM_TAC_TYPE:
          fprintf(fp, "exit\n");
          break;

        case OPERATION_TAC_TYPE: {
          TAC_OPERATION operation = current_TAC->v.tac_operation;
          char* destination;
          char* operation_char = operation_characters[operation.op];
          char* source1;
          char* source2;
          
          // print destination
          if (operation.dest->type == IDENTIFIER) { fprintf(fp, "%s = ", operation.dest->lexeme); }
          else if (operation.dest->type == TEMPORARY_REG_IDENTIFIER) { fprintf(fp, "t%d = ", operation.dest->value); }
          else if (operation.dest->type == ARGUMENT_REG_IDENTIFIER) { fprintf(fp, "a%d = ", operation.dest->value); }
          else if (operation.dest->type == RETURN_REG_IDENTIFIER) { fprintf(fp, "v%d = ", operation.dest->value); }

          // print source 1
          if (operation.src1->type == IDENTIFIER) { fprintf(fp, "%s", operation.src1->lexeme); }
          else if (operation.src1->type == TEMPORARY_REG_IDENTIFIER) { fprintf(fp, "t%d", operation.src1->value); }
          else if (operation.src1->type == ARGUMENT_REG_IDENTIFIER) { fprintf(fp, "a%d", operation.src1->value); }
          else if (operation.src1->type == RETURN_REG_IDENTIFIER) { fprintf(fp, "v%d", operation.src1->value); }
          else if (operation.src1->type == CONSTANT) { fprintf(fp, "%d", operation.src1->value); }

          // print source 2
          if (operation.src2 != NULL) {
            fprintf(fp, " %s ", operation_char);
            if (operation.src2->type == IDENTIFIER) { fprintf(fp, "%s", operation.src2->lexeme); }
            else if (operation.src2->type == TEMPORARY_REG_IDENTIFIER) { fprintf(fp, "t%d", operation.src2->value); }
            else if (operation.src2->type == RETURN_REG_IDENTIFIER) { fprintf(fp, "v%d", operation.src2->value); }
            else if (operation.src2->type == CONSTANT) { fprintf(fp, "%d", operation.src2->value); }
          }

          if (operation.assignement_type == INT_DECLARATION_ASSIGNEMENT_TYPE) {
            fprintf(fp, " (int variable declaration) ");
          }
          else if (operation.assignement_type == CLOSURE_DECLARATION_ASSIGNEMENT_TYPE) {
            fprintf(fp, " (closure variable declaration) ");
          }

          fprintf(fp, "\n");

          break;
        }
        
        default:
          fprintf(fp, "UNKNOWN TAC\n");
          break;

      }

      // move onto next tac in basic block
      if (current_TAC->next == NULL) {
        break;
      } else {
        current_TAC = current_TAC->next;
      }

    }

    // move onto next basic block
    if (current_BB->next == NULL) {
      break;
    } else {
      current_BB = current_BB->next;
    }
  }

  return;

}

// Prints MIPS machine code into file
void generate_asm(MIPS_PROGRAM* program) {

  char asm_file_name[80];
  strcpy(asm_file_name, "./testing/mips_results/");
  strcat(asm_file_name, program_name);
  strcat(asm_file_name, ".asm");

  FILE* fp = fopen(asm_file_name, "w+");

  fprintf(fp, "\n");
  fprintf(fp, ".text\n");

  MIPS_INSTR* current_instr = program->instructions;
  while (1) {

    if (current_instr == NULL) {
      break;
    } else {
      fprintf(fp, "%s\n", current_instr->instr_str);
    }

    current_instr = current_instr->next;

  }

  fprintf(fp, "\n");
  fprintf(fp, ".data\n");

  MIPS_DATA* current_data = program->data;
  while (1) {

    if (current_data == NULL) {
      break;
    } else {
      if (current_data->type == TEXT_DATA_TYPE) {
        fprintf(fp, "%s:  .asciiz \"%s\"\n", current_data->label, current_data->text);
      }
    }

    current_data = current_data->next;

  }

  fclose(fp);

  return;

}


int main(int argc, char** argv) {

  NODE* tree;

  if (argc == 2) {
    sprintf(program_name, "%s", argv[1]);
  } else {
    sprintf(program_name, "noname");
  }

  init_symbtable();
  
  // Parse inputed program into AST
  yyparse();
  tree = ans;

  // Interprete abstract syntax tree
  VALUE* result = interpret(tree);
  
  // Generate intermediate code
  BASIC_BLOCK* root_BB = generate_TAC(tree);
  // Print intermediate code into file
  generate_tac_file(root_BB);
  
  // Generate MIPS machine code
  MIPS_PROGRAM* program = generate_MIPS(root_BB);
  // Print MIPS machine code into file
  generate_asm(program);


  return 0;

}
