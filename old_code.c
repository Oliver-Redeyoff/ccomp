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
    if (t->type == CONSTANT) printf("leaf_value: %d\n", t->value);
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

void print_tac(BASIC_BLOCK* current_BB) {

  char* operation_characters[12] = {"p", "+", "-", "*", "/", "%", "==", "!=", ">", ">=", "<", "<="};

  while (1) {
    TAC* current_TAC = current_BB->leader;

    if (current_TAC == NULL) {
      printf("ERROR: basic block is empty\n");
      return;
    }

    printf("\n\n--\n");

    while (1) {
      switch (current_TAC->type) {

        case LABEL_TAC_TYPE: 
          printf("label \033[0;35m%s\033[0m\n", current_TAC->v.tac_label.name->lexeme);
          break;

        case GOTO_TAC_TYPE: 
          printf("goto \033[0;35m%s\033[0m\n", current_TAC->v.tac_label.name->lexeme);
          break;

        case BLOCK_START_TAC_TYPE:
          printf("block %d start \033[0;35m%s\033[0m %d", current_TAC->v.tac_block_delimiter.block_type, current_TAC->v.tac_block_delimiter.name->lexeme, current_TAC->v.tac_block_delimiter.arity);
          if (current_TAC->v.tac_block_delimiter.parent_block_name != NULL) {
            printf(" (\033[0;35m%s\033[0m)", current_TAC->v.tac_block_delimiter.parent_block_name->lexeme);
          }
          printf("\n");
          break;

        case BLOCK_END_TAC_TYPE:
          printf("block %d end \033[0;35m%s\033[0m %d", current_TAC->v.tac_block_delimiter.block_type, current_TAC->v.tac_block_delimiter.name->lexeme, current_TAC->v.tac_block_delimiter.arity);
          if (current_TAC->v.tac_block_delimiter.parent_block_name != NULL) {
            printf(" (\033[0;35m%s\033[0m)", current_TAC->v.tac_block_delimiter.parent_block_name->lexeme);
          }
          printf("\n");
          break;

        case FUNCTION_CALL_TAC_TYPE:
          printf("call %s\n", current_TAC->v.tac_function_call.name->lexeme);
          break;

        case BUILTIN_CALL_TAC_TYPE:
          printf("call builtin %d", current_TAC->v.tac_builtin_call.type);
          if (current_TAC->v.tac_builtin_call.argument != NULL) {
            printf(", %s\n", current_TAC->v.tac_builtin_call.argument->lexeme);
          } else {
            printf("\n");
          }
          break;

        case IF_TAC_TYPE: {
          TAC_IF tac_if = current_TAC->v.tac_if;
          printf("if ");
          if (tac_if.condition_result->type == IDENTIFIER) { printf("%s ", tac_if.condition_result->lexeme); }
          else if (tac_if.condition_result->type == TEMPORARY_REG_IDENTIFIER) { printf("t%d ", tac_if.condition_result->value); }
          else if (tac_if.condition_result->type == ARGUMENT_REG_IDENTIFIER) { printf("a%d ", tac_if.condition_result->value); }
          else if (tac_if.condition_result->type == RETURN_REG_IDENTIFIER) { printf("v%d ", tac_if.condition_result->value); }
          printf("\033[0;35m%s\033[0m\n", tac_if.jump_label->lexeme);
          break;
        }

        case RETURN_TAC_TYPE:
          printf("return\n");
          break;

        case EXIT_PROGRAM_TAC_TYPE:
          printf("exit\n");
          break;

        case OPERATION_TAC_TYPE: {
          TAC_OPERATION operation = current_TAC->v.tac_operation;
          char* destination;
          char* operation_char = operation_characters[operation.op];
          char* source1;
          char* source2;
          
          // print destination
          if (operation.dest->type == IDENTIFIER) { printf("%s = ", operation.dest->lexeme); }
          else if (operation.dest->type == TEMPORARY_REG_IDENTIFIER) { printf("t%d = ", operation.dest->value); }
          else if (operation.dest->type == ARGUMENT_REG_IDENTIFIER) { printf("a%d = ", operation.dest->value); }
          else if (operation.dest->type == RETURN_REG_IDENTIFIER) { printf("v%d = ", operation.dest->value); }

          // print source 1
          if (operation.src1->type == IDENTIFIER) { printf("%s", operation.src1->lexeme); }
          else if (operation.src1->type == TEMPORARY_REG_IDENTIFIER) { printf("t%d", operation.src1->value); }
          else if (operation.src1->type == ARGUMENT_REG_IDENTIFIER) { printf("a%d", operation.src1->value); }
          else if (operation.src1->type == RETURN_REG_IDENTIFIER) { printf("v%d", operation.src1->value); }
          else if (operation.src1->type == CONSTANT) { printf("%d", operation.src1->value); }

          // print source 2
          if (operation.src2 != NULL) {
            printf(" %s ", operation_char);
            if (operation.src2->type == IDENTIFIER) { printf("%s", operation.src2->lexeme); }
            else if (operation.src2->type == TEMPORARY_REG_IDENTIFIER) { printf("t%d", operation.src2->value); }
            else if (operation.src2->type == RETURN_REG_IDENTIFIER) { printf("v%d", operation.src2->value); }
            else if (operation.src2->type == CONSTANT) { printf("%d", operation.src2->value); }
          }

          if (operation.assignement_type == INT_DECLARATION_ASSIGNEMENT_TYPE) {
            printf(" (int variable declaration) ");
          }
          else if (operation.assignement_type == CLOSURE_DECLARATION_ASSIGNEMENT_TYPE) {
            printf(" (closure variable declaration) ");
          }

          printf("\n");

          break;
        }
        
        default:
          printf("UNKNOWN TAC\n");
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


void print_mips(MIPS_PROGRAM* program) {

  printf("\n");
  printf(".text\n");

  MIPS_INSTR* current_instr = program->instructions;
  while (1) {

    if (current_instr == NULL) {
      break;
    } else {
      printf("%s\n", current_instr->instr_str);
    }

    current_instr = current_instr->next;

  }

  printf("\n");
  printf(".data\n");

  MIPS_DATA* current_data = program->data;
  while (1) {

    if (current_data == NULL) {
      break;
    } else {
      if (current_data->type == TEXT_DATA_TYPE) {
        printf("%s:  .asciiz \"%s\"\n", current_data->label, current_data->text);
      }
    }

    current_data = current_data->next;

  }

  return;

}


cat << EndOfMessage
____________________________________________________________________
|                                                 _ _              |
|               ___      ___ ___  _ __ ___  _ __ (_) | ___ _ __    |
|  _____ _____ / __|    / __/ _ \| '_   _ \| '_ \| | |/ _ \ '__|   |
| |_____|_____| (__    | (_| (_) | | | | | | |_) | | |  __/ |      |
|              \___|    \___\___/|_| |_| |_| .__/|_|_|\___|_|      |
|                                          |_|                     |
|  by Oliver Redeyoff                                              |
|__________________________________________________________________|

EndOfMessage