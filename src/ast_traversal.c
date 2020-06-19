/*
 * ast_traversal.c
 *
 *  Created on: 18/06/2020
 *      Author: pirx
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "ast_traversal.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

struct PrgmPoint {
  FPCODE *code;
  struct PrgmPoint *next;
};
typedef struct PrgmPoint PrgmPoint;

struct PrgmCode {
  PrgmPoint *head;
  PrgmPoint *queu;
  size_t size;
};

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

PrgmCode *PC_New(void) {
  PrgmCode *pc = calloc(1, sizeof(struct PrgmCode));
  // head = NULL, queu = NULL, size = 0
  assert(pc);
  return pc;
}

void PC_AddEnd(PrgmCode *pc, FPCODE *code) {
  PrgmPoint *pp = malloc(sizeof(struct PrgmPoint));
  assert(pp);
  pp->code = code;
  pp->next = NULL;
  pc->size += 1;

  if (pc->head == NULL) { // 0
    pc->head = pp;
    pc->queu = pp;
  } else {
    pc->queu->next = pp;
    pc->queu = pp;
  }
  return;
}

void PC_AddBeg(PrgmCode *pc, FPCODE *code) {
  PrgmPoint *pp = malloc(sizeof(struct PrgmPoint));
  assert(pp);
  pp->code = code;
  pp->next = pc->head;
  pc->size += 1;

  if (pc->head == NULL) { // 0
    pc->head = pp;
    pc->queu = pp;
  } else {
    pc->head = pp;
  }
  return;
}

void PC_FusionEnd(PrgmCode *dst, PrgmCode *src) {
  if (src->size == 0) { // src NULL
    // Do Nothing
    return;
  }
  if (dst->size == 0) { // dst NULL
    dst->head = src->head;
    dst->queu = src->queu;
    dst->size = src->size;
    return;
  }
  // Head to Queu
  dst->queu->next = src->head;
  dst->queu = src->queu;
  // Ugly
  for (PrgmPoint *cur = src->head; cur != NULL; cur = cur->next) {
    if (cur->code->type == CONDITIONAL_JUMP || cur->code->type == JUMP)
      cur->code->arg += dst->size; // Offset jump
  }
  dst->size += src->size;
}

void PrgmCodePrint(PrgmCode *pc) {
  size_t cpt = 0;
  for (PrgmPoint *cur = pc->head; cur != NULL; cur = cur->next, cpt++) {
    printf("[%ld] ", cpt);
    FPC_Print(cur->code);
    printf("\n");
  }
}

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Public function
 ******************************************************************************/

PrgmCode *AST_ToCodeRec(AST_NODE *node) {
  assert(node);
  printf("Ast To rec (%s): \n", AST_NODE_TYPE_STR[*(int *)node]);
  PrgmCode *pc = PC_New();
  // processing
  switch (*(int *)node) {
  case AST_NODE_TYPE_PCODE:
    if (AST_CAST_PCODE(node)->arg1) {
      printf("ARG1:\n");
      PC_FusionEnd(pc, AST_ToCodeRec(AST_CAST_PCODE(node)->arg1));
    }
    if (AST_CAST_PCODE(node)->arg2) {
      printf("ARG2:\n");
      PC_FusionEnd(pc, AST_ToCodeRec(AST_CAST_PCODE(node)->arg2));
    }
    PC_AddEnd(pc, AST_CAST_PCODE(node)->code);
    break;
  case AST_NODE_TYPE_IF: {
    PrgmCode *test, *ift, *iff;

    test = AST_ToCodeRec(AST_CAST_IF(node)->test);
    ift = AST_ToCodeRec(AST_CAST_IF(node)->if_true);
    if (AST_CAST_IF(node)->if_false) { // if "else"
      iff = AST_ToCodeRec(AST_CAST_IF(node)->if_false);
    }
    // Todo optimise ELSE
    if (!AST_CAST_IF(node)->if_false) { // only "if"
      PC_FusionEnd(pc, test);           // test
      PC_AddEnd(
          pc, FPC_Create(CONDITIONAL_JUMP, (void *)test->size + 1 + ift->size));
      PC_FusionEnd(pc, ift); // if true
    } else {
      PC_FusionEnd(pc, test); // test
      PC_AddEnd(pc, FPC_Create(CONDITIONAL_JUMP,
                               (void *)test->size + 1 + ift->size + 1));
      PC_FusionEnd(pc, ift); // if true
      PC_AddEnd(pc, FPC_Create(JUMP, (void *)test->size + 1 + ift->size + 1 +
                                         iff->size));
      PC_FusionEnd(pc, iff); // if false
    }
    break;
  }
  case AST_NODE_TYPE_WHILE:
    // TODO
    break;
  case AST_NODE_TYPE_STAT:
    for (AST_NODE_STAT *cur = node; cur != NULL; cur = cur->next) {
      PC_FusionEnd(pc, AST_ToCodeRec(AST_CAST_STAT(cur)->ptr));
    }
    break;
  }
  // printf("CODE : =>\n");
  // PrgmCodePrint(pc);
  return pc;
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/
