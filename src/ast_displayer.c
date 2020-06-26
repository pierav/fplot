/*
 * ast_displayer.c
 *
 *  Created on: 16/06/2020
 *      Author: pirx
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "ast_displayer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

void fprintRecuDot(AST_NODE *node, FILE *pf);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Public function
 ******************************************************************************/

void AST_DISPLAY_Text(AST_NODE *cur, int space) {
  if (cur == NULL)
    return;
  for (int i = 0; i < space; i++)
    printf(" | ");
  printf("%s", AST_NODE_TYPE_STR[*(int *)cur]);
  switch (AST_NODE_GET_TYPE(cur)) {
  case AST_NODE_TYPE_PCODE: {
    PC_Print(((AST_NODE_PCODE *)cur)->code);
    printf("\n");
    if (((AST_NODE_PCODE *)cur)->arg1) {
      for (int i = 0; i < space; i++)
        printf(" | ");
      printf("arg1:\n");
      AST_DISPLAY_Text(((AST_NODE_PCODE *)cur)->arg1, space + 1);
    }
    if (((AST_NODE_PCODE *)cur)->arg2) {
      for (int i = 0; i < space; i++)
        printf(" | ");
      printf("arg2:\n");
      AST_DISPLAY_Text(((AST_NODE_PCODE *)cur)->arg2, space + 1);
    }
    break;
  }
  case AST_NODE_TYPE_IF: {
    printf("\n");
    if (((AST_NODE_IF *)cur)->if_true) {
      for (int i = 0; i < space; i++)
        printf(" | ");
      printf("if true:\n");
      AST_DISPLAY_Text(((AST_NODE_IF *)cur)->if_true, space + 1);
    }
    if (((AST_NODE_IF *)cur)->if_false) {
      for (int i = 0; i < space; i++)
        printf(" | ");
      printf("if false:\n");
      AST_DISPLAY_Text(((AST_NODE_IF *)cur)->if_false, space + 1);
    }
    break;
  }
  case AST_NODE_TYPE_WHILE: {
    printf("\n");
    for (int i = 0; i < space; i++)
      printf(" | ");
    printf("test:\n");
    AST_DISPLAY_Text(((AST_NODE_WHILE *)cur)->test, space + 1);
    for (int i = 0; i < space; i++)
      printf(" | ");
    printf("while true:\n");
    AST_DISPLAY_Text(((AST_NODE_WHILE *)cur)->while_true, space + 1);
    break;
  }
  case AST_NODE_TYPE_STAT: {
    printf("\n");
    for (AST_NODE_STAT *c = cur; c != NULL; c = c->next) {
      AST_DISPLAY_Text(c->ptr, space + 1);
    }
    break;
  }
  default:
    break;
  }
}

void AST_DISPLAY_DotF(AST_NODE *root, char *namefile) {
  FILE *pf = fopen(namefile, "w");
  assert(pf);
  fprintf(pf, "digraph AST {\n");
  fprintf(pf, "node[shape = record];\n");
  fprintRecuDot(root, pf);
  fprintf(pf, "}\n");
  fclose(pf);
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/

void fprintRecuDot(AST_NODE *node, FILE *pf) {
  fprintf(pf, "n%p ", node);

  switch (AST_NODE_GET_TYPE(node)) {
  case AST_NODE_TYPE_PCODE: {
    fprintf(pf, "[fillcolor=lightblue, style=\"filled\", ");
    fprintf(pf, "label = \"");
    PC_FPrint(pf, AST_NODE_CAST_PCODE(node)->code);
    fprintf(pf, "\"]\n");

    if (AST_NODE_CAST_PCODE(node)->arg1) {
      fprintRecuDot(AST_NODE_CAST_PCODE(node)->arg1, pf);
      fprintf(pf, "n%p -> n%p [ label=\"a1\"];\n", node,
              AST_NODE_CAST_PCODE(node)->arg1);
    }
    if (AST_NODE_CAST_PCODE(node)->arg2) {
      fprintRecuDot(AST_NODE_CAST_PCODE(node)->arg2, pf);
      fprintf(pf, "n%p -> n%p [ label=\"a2\"]\n", node,
              AST_NODE_CAST_PCODE(node)->arg2);
    }
    break;
  }
  case AST_NODE_TYPE_IF: {
    fprintf(pf, "[fillcolor=lightgreen, style=\"filled\", shape=diamond, ");
    fprintf(pf, "label = \"IF\"]\n");

    fprintRecuDot(AST_NODE_CAST_IF(node)->test, pf);
    fprintf(pf, "n%p -> n%p [ label=\"test\"]\n", node,
            AST_NODE_CAST_IF(node)->test);
    if (AST_NODE_CAST_IF(node)->if_true) {
      fprintRecuDot(AST_NODE_CAST_IF(node)->if_true, pf);
      fprintf(pf, "n%p -> n%p [ label=\"T\"]\n", node,
              AST_NODE_CAST_IF(node)->if_true);
    }
    if (AST_NODE_CAST_IF(node)->if_false) {
      fprintRecuDot(AST_NODE_CAST_IF(node)->if_false, pf);
      fprintf(pf, "n%p -> n%p [ label=\"F\"]\n", node,
              AST_NODE_CAST_IF(node)->if_false);
    }
    break;
  }
  case AST_NODE_TYPE_WHILE: {
    fprintf(pf, "[fillcolor=gold, style=\"filled\", shape=trapezium, ");
    fprintf(pf, "label = \"WHILE\"]\n");

    fprintRecuDot(AST_NODE_CAST_WHILE(node)->test, pf);
    fprintf(pf, "n%p -> n%p [ label=\"test\"]\n", node,
            AST_NODE_CAST_WHILE(node)->test);

    fprintRecuDot(AST_NODE_CAST_WHILE(node)->while_true, pf);
    fprintf(pf, "n%p -> n%p [ label=\"T\"]\n", node,
            AST_NODE_CAST_WHILE(node)->while_true);
    break;
  }
  case AST_NODE_TYPE_STAT: {
    fprintf(pf, "[fillcolor=purple, style=\"filled\", shape=cylinder, ");
    fprintf(pf, "label = \"STATS\"]\n");
    size_t cpt = 1;
    for (AST_NODE_STAT *cur = node; cur != NULL; cur = cur->next, cpt++) {
      fprintRecuDot(cur->ptr, pf);
      fprintf(pf, "n%p -> n%p [ label=\"%ld\"]\n", node, cur->ptr, cpt);
    }
  }
  default:
    break;
  }
}
