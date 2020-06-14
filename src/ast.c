/*
 * ast.c
 *
 *  Created on: 14/06/2020
 *      Author: pirx
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "ast.h"
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

/*******************************************************************************
 * Variables
 ******************************************************************************/
char *AST_NODE_TYPE_STR[] = {"PCODE", "IF", "WHILE", "STAT"};
/*******************************************************************************
 * Public function
 ******************************************************************************/

AST_NODE *AST_NODE_PCODE_Create(FPCODE *code, AST_NODE *arg1, AST_NODE *arg2) {
  // AST_NODE_DisplayRecu(fils);
  AST_NODE_PCODE *ret = malloc(sizeof(struct AST_NODE_PCODE));
  ret->type = AST_NODE_TYPE_PCODE;
  ret->arg1 = arg1;
  ret->arg2 = arg2;
  ret->code = code;
  return (AST_NODE *)ret;
}

AST_NODE *AST_NODE_IF_Create(AST_NODE *test, AST_NODE *if_true,
                             AST_NODE *if_false) {
  AST_NODE_IF *ret = malloc(sizeof(struct AST_NODE_IF));
  ret->type = AST_NODE_TYPE_IF;
  ret->test = test;
  ret->if_true = if_true;
  ret->if_false = if_false;
  return (AST_NODE *)ret;
}

AST_NODE *AST_NODE_STAT_Create(AST_NODE_STAT *next, AST_NODE *ptr) {
  AST_NODE_STAT *ret = malloc(sizeof(struct AST_NODE_STAT));
  ret->type = AST_NODE_TYPE_STAT;
  ret->next = next;
  ret->ptr = ptr;
  return (AST_NODE *)ret;
}

void AST_NODE_DisplayRecu(AST_NODE *cur, int space) {
  if (cur == NULL)
    return;
  for (int i = 0; i < space; i++)
    printf(" | ");
  printf("%s", AST_NODE_TYPE_STR[*(int *)cur]);
  switch (*(int *)cur) {
  case AST_NODE_TYPE_PCODE: {
    FPC_Print(((AST_NODE_PCODE *)cur)->code);
    printf("\n");
    if (((AST_NODE_PCODE *)cur)->arg1) {
      for (int i = 0; i < space; i++)
        printf(" | ");
      printf("arg1:\n");
      AST_NODE_DisplayRecu(((AST_NODE_PCODE *)cur)->arg1, space + 1);
    }
    if (((AST_NODE_PCODE *)cur)->arg2) {
      for (int i = 0; i < space; i++)
        printf(" | ");
      printf("arg2:\n");
      AST_NODE_DisplayRecu(((AST_NODE_PCODE *)cur)->arg2, space + 1);
    }
    break;
  }
  case AST_NODE_TYPE_IF: {
    printf("\n");
    if (((AST_NODE_IF *)cur)->if_true) {
      for (int i = 0; i < space; i++)
        printf(" | ");
      printf("if true:\n");
      AST_NODE_DisplayRecu(((AST_NODE_IF *)cur)->if_true, space + 1);
    }
    if (((AST_NODE_IF *)cur)->if_false) {
      for (int i = 0; i < space; i++)
        printf(" | ");
      printf("if false:\n");
      AST_NODE_DisplayRecu(((AST_NODE_IF *)cur)->if_false, space + 1);
    }
    break;
  }
  case AST_NODE_TYPE_STAT: {
    printf("\n");
    for (AST_NODE_STAT *c = cur; c != NULL; c = c->next) {
      for (int i = 0; i < space; i++)
        printf(" |>");
      AST_NODE_DisplayRecu(c->ptr, space + 1);
    }
    break;
  }
  default:
    break;
  }
}

void fprintRecuDot(AST_NODE *node, FILE *pf) {
  switch (*(int *)node) {
  case AST_NODE_TYPE_PCODE: {
    fprintf(pf, "n%p [label = \"PCODE : ", node);
    FPC_FPrint(pf, ((AST_NODE_PCODE *)node)->code);
    fprintf(pf, "\"]\n");
    if (((AST_NODE_PCODE *)node)->arg1) {
      fprintRecuDot(((AST_NODE_PCODE *)node)->arg1, pf);
      fprintf(pf, "n%p -> n%p [ label=\"a1\"];\n", node,
              ((AST_NODE_PCODE *)node)->arg1);
    }
    if (((AST_NODE_PCODE *)node)->arg2) {
      fprintRecuDot(((AST_NODE_PCODE *)node)->arg2, pf);
      fprintf(pf, "n%p -> n%p [ label=\"a2\"]\n", node,
              ((AST_NODE_PCODE *)node)->arg2);
    }
    break;
  }
  case AST_NODE_TYPE_IF: {
    fprintf(pf, "n%p [label = \"%s\"]\n", node, "IF");
    fprintRecuDot(((AST_NODE_IF *)node)->test, pf);
    fprintf(pf, "n%p -> n%p\n [ label=\"test\"]", node,
            ((AST_NODE_IF *)node)->test);
    if (((AST_NODE_IF *)node)->if_true) {
      fprintRecuDot(((AST_NODE_IF *)node)->if_true, pf);
      fprintf(pf, "n%p -> n%p [ label=\"T\"]\n", node,
              ((AST_NODE_IF *)node)->if_true);
    }
    if (((AST_NODE_IF *)node)->if_false) {
      fprintRecuDot(((AST_NODE_IF *)node)->if_false, pf);
      fprintf(pf, "n%p -> n%p [ label=\"F\"]\n", node,
              ((AST_NODE_IF *)node)->if_false);
    }
    break;
  }
  case AST_NODE_TYPE_STAT: {
    fprintf(pf, "n%p [label = \"%s\"]\n", node, "STATS");
    for (AST_NODE_STAT *cur = node; cur != NULL; cur = cur->next) {
      fprintRecuDot(cur->ptr, pf);
      fprintf(pf, "n%p -> n%p\n", node, cur->ptr);
    }
  }
  default:
    break;
  }
}

void AST_ToDot(AST_NODE *root) {
  FILE *pf = fopen("out.dot", "w");
  assert(pf);
  fprintf(pf, "digraph L {\n");
  fprintf(pf, "node[shape = record fontname = Arial];\n");
  fprintRecuDot(root, pf);
  fprintf(pf, "}\n");
  fclose(pf);
}
/*******************************************************************************
 * Internal function
 ******************************************************************************/
