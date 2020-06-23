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
  PCODE *code;
  struct PrgmPoint *next;
};
typedef struct PrgmPoint PrgmPoint;

struct PrgmCode {
  PrgmPoint *head;
  PrgmPoint *queu;
  size_t size;
};
typedef struct PrgmCode PrgmCode;

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

PrgmCode AST_ToCodeRec(AST_NODE *node);

void PC_AddEnd(PrgmCode *pc, PCODE *code);
void PC_AddBeg(PrgmCode *pc, PCODE *code);
void PC_FusionEnd(PrgmCode *dst, PrgmCode *src);
void PrgmCodePrint(PrgmCode *pc);
void PrgmFreeOnlyPointsRec(PrgmPoint *point);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Public function
 ******************************************************************************/

PCODE **AST_ComputePrgm(AST_NODE *root, size_t *size) {
  PrgmCode pc = AST_ToCodeRec(root);

  /*
  printf("PRGM(size = %ld):\n", pc.size);
  PrgmCodePrint(&pc);
  printf("EOP\n");
  */

  PCODE **prgm = malloc(sizeof(struct PCODE *) * pc.size);
  *size = 0;
  for (PrgmPoint *cur = pc.head; cur != NULL; cur = cur->next, *size += 1) {
    prgm[*size] = cur->code;
  }
  PrgmFreeOnlyPointsRec(pc.head);
  return prgm;
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/

PrgmCode AST_ToCodeRec(AST_NODE *node) {
  PrgmCode pc = {0, 0, 0};
  if (node) { // processing
    switch (AST_NODE_GET_TYPE(node)) {
    case AST_NODE_TYPE_PCODE: {
      PrgmCode arg1 = AST_ToCodeRec(AST_NODE_CAST_PCODE(node)->arg1);
      PrgmCode arg2 = AST_ToCodeRec(AST_NODE_CAST_PCODE(node)->arg2);
      PC_FusionEnd(&pc, &arg1);
      PC_FusionEnd(&pc, &arg2);
      PC_AddEnd(&pc, AST_NODE_CAST_PCODE(node)->code);
      break;
    }
    case AST_NODE_TYPE_IF: {
      PrgmCode test = AST_ToCodeRec(AST_NODE_CAST_IF(node)->test);
      PrgmCode ift = AST_ToCodeRec(AST_NODE_CAST_IF(node)->if_true);
      PrgmCode iff = AST_ToCodeRec(AST_NODE_CAST_IF(node)->if_false);
      if (!AST_NODE_CAST_IF(node)->if_false) { // only "if"
        PC_FusionEnd(&pc, &test);              // test
        PC_AddEnd(&pc, PC_Create(CONDITIONAL_JUMP,
                                 (PC_ARG)(int)(test.size + 1 + ift.size)));
        PC_FusionEnd(&pc, &ift); // if true
      } else {
        PC_FusionEnd(&pc, &test); // test
        PC_AddEnd(&pc, PC_Create(CONDITIONAL_JUMP,
                                 (PC_ARG)(int)(test.size + 1 + ift.size + 1)));
        PC_FusionEnd(&pc, &ift); // if true
        PC_AddEnd(&pc, PC_Create(JUMP, (PC_ARG)(int)(test.size + 1 + ift.size +
                                                     1 + iff.size)));
        PC_FusionEnd(&pc, &iff); // if false
      }
      break;
    }
    case AST_NODE_TYPE_WHILE:
      // TODO
      break;
    case AST_NODE_TYPE_STAT: {
      PrgmCode prgm;
      for (AST_NODE_STAT *cur = node; cur != NULL; cur = cur->next) {
        prgm = AST_ToCodeRec(AST_NODE_CAST_STAT(cur)->ptr);
        PC_FusionEnd(&pc, &prgm);
      }
      break;
    }
    }
  }
  return pc;
}

void PC_AddEnd(PrgmCode *pc, PCODE *code) {
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

void PC_AddBeg(PrgmCode *pc, PCODE *code) {
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
      cur->code->arg.int_t += dst->size; // Offset jump
  }
  dst->size += src->size;
}

void PrgmCodePrint(PrgmCode *pc) {
  size_t cpt = 0;
  for (PrgmPoint *cur = pc->head; cur != NULL; cur = cur->next, cpt++) {
    printf("[%ld] ", cpt);
    PC_Print(cur->code);
    printf("\n");
  }
}

void PrgmFreeOnlyPointsRec(PrgmPoint *point) {
  if (point)
    PrgmFreeOnlyPointsRec(point->next);
  free(point);
}
