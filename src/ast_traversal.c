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

// Chainage de PCODE
struct PrgmPoint {
  PCODE *code;
  struct PrgmPoint *next;
};
typedef struct PrgmPoint PrgmPoint;

// Liste de PCODE (Représentation intermédiaire)
struct PrgmCode {
  PrgmPoint *head; // première instruction
  PrgmPoint *queu; // dernière instruction
  size_t size;     // nombre d'instruction
};
typedef struct PrgmCode PrgmCode;

// Chainage de PrgmCode
struct PrgmCodePoint {
  uint64_t id;     // index fonction (toutes les fonctions sont anomynes)
  PrgmPoint *code; // Code de la fonction
  struct PrgmCodePoint *next; // Point suivant
};
typedef struct PrgmCodePoint PrgmCodePoint;

// Desctiption d'un package = une liste de PRGM
struct PrgmPkg {
  char *name;            // Nom du package
  PrgmCodePoint *head;   // liste de fonction
  size_t size;           // nombre de fonctions
  PrgmPoint *entrypoint; // Point d'entrée du module
};
typedef struct PrgmPkg PrgmPkg;

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

PrgmCode AST_ToCodeRec(AST_NODE *node, PrgmPkg *outPkg);
// PrgmCode
void PC_AddEnd(PrgmCode *pc, PCODE *code);
void PC_AddBeg(PrgmCode *pc, PCODE *code);
void PC_FusionEnd(PrgmCode *dst, PrgmCode *src);
// PrgmPoint
void PrgmPointPrint(PrgmPoint *pp);
void PrgmPointFreeOnlyWrapper(PrgmPoint *point);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Public function
 ******************************************************************************/

PCODE ***AST_ComputePrgm(AST_NODE *root, size_t *outmainsize,
                         size_t **outsubsizes) {
  PrgmPkg *pkg = malloc(sizeof(struct PrgmPkg));
  pkg->name = "PKGNAME";
  pkg->head = NULL;
  pkg->size = 0;

  // Compute PRGM
  PrgmCode pc = AST_ToCodeRec(root, pkg); // Return global code
  pkg->entrypoint = pc.head;

  // Display
  printf("PKG[%s]\n", pkg->name);
  printf("\tID[ENTRY]\n");
  size_t cpt = 0;
  for (PrgmPoint *pp = pkg->entrypoint; pp != NULL; pp = pp->next, cpt++) {
    printf("\t\t[%4ld] ", cpt);
    PC_Print(pp->code);
    printf("\n");
  }
  for (PrgmCodePoint *cur = pkg->head; cur != NULL; cur = cur->next) {
    printf("\tID[%ld]\n", cur->id);
    cpt = 0;
    for (PrgmPoint *pp = cur->code; pp != NULL; pp = pp->next, cpt++) {
      printf("\t\t[%4ld] ", cpt);
      PC_Print(pp->code);
      printf("\n");
    }
  }

  /* Inline PrgmPkg */
  // Tableau de tableau de pointeur de PCODE;
  size_t mainsize = pkg->size + 1;
  size_t *subsizes = malloc(sizeof(size_t) * mainsize);
  assert(subsizes);
  PCODE ***prgm = malloc(sizeof(struct PCODE **) * mainsize);
  assert(prgm);
  size_t size = 0;
  for (PrgmPoint *pp = pkg->entrypoint; pp != NULL; pp = pp->next, size++)
    ;
  subsizes[0] = size;
  prgm[0] = malloc(sizeof(struct PCODE *) * subsizes[0]);
  size_t i = 0;
  for (PrgmPoint *pp = pkg->entrypoint; pp != NULL; pp = pp->next, i++) {
    prgm[0][i] = pp->code;
  }

  for (PrgmCodePoint *cur = pkg->head; cur != NULL; cur = cur->next) {
    size_t size = 0;
    for (PrgmPoint *pp = cur->code; pp != NULL; pp = pp->next, size++)
      ;
    subsizes[cur->id + 1] = size;
    prgm[cur->id + 1] = calloc(sizeof(struct PCODE *), subsizes[cur->id + 1]);
    assert(prgm[cur->id + 1]);
    size_t i = 0;
    for (PrgmPoint *pp = cur->code; pp != NULL; pp = pp->next, i++) {
      prgm[cur->id + 1][i] = pp->code;
    }
  }

  printf("PKG[%s]\n", pkg->name);
  for (size_t fi = 0; fi < mainsize; fi++) {
    printf("\tID[%ld]\n", fi - 1);
    for (size_t i = 0; i < subsizes[fi]; i++) {
      printf("\t\t[%4ld] ", i);
      PC_Print(prgm[fi][i]);
      printf("\n");
    }
  }

  *outmainsize = mainsize;
  *outsubsizes = subsizes;
  return prgm;
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/

PrgmCode AST_ToCodeRec(AST_NODE *node, PrgmPkg *outPkg) {
  PrgmCode pc = {NULL, NULL, 0};
  if (node) { // processing
    switch (AST_NODE_GET_TYPE(node)) {
    case AST_NODE_TYPE_PCODE: {
      PrgmCode arg1 = AST_ToCodeRec(AST_NODE_CAST_PCODE(node)->arg1, outPkg);
      PrgmCode arg2 = AST_ToCodeRec(AST_NODE_CAST_PCODE(node)->arg2, outPkg);
      PC_FusionEnd(&pc, &arg1);
      PC_FusionEnd(&pc, &arg2);
      PC_AddEnd(&pc, AST_NODE_CAST_PCODE(node)->code);
      break;
    }
    case AST_NODE_TYPE_IF: {
      PrgmCode test = AST_ToCodeRec(AST_NODE_CAST_IF(node)->test, outPkg);
      PrgmCode ift = AST_ToCodeRec(AST_NODE_CAST_IF(node)->if_true, outPkg);
      PrgmCode iff = AST_ToCodeRec(AST_NODE_CAST_IF(node)->if_false, outPkg);
      if (!AST_NODE_CAST_IF(node)->if_false) { // only "if"
        PC_FusionEnd(&pc, &test);              // test
        PC_AddEnd(&pc, PC_CreateJumpCond(1 + ift.size));
        PC_FusionEnd(&pc, &ift); // if true
      } else {
        PC_FusionEnd(&pc, &test); // test
        PC_AddEnd(&pc, PC_CreateJumpCond(1 + ift.size + 1));
        PC_FusionEnd(&pc, &ift); // if true
        PC_AddEnd(&pc, PC_CreateJump(1 + iff.size));
        PC_FusionEnd(&pc, &iff); // if false
      }
      break;
    }
    case AST_NODE_TYPE_WHILE: {
      PrgmCode test = AST_ToCodeRec(AST_NODE_CAST_WHILE(node)->test, outPkg);
      PrgmCode code =
          AST_ToCodeRec(AST_NODE_CAST_WHILE(node)->while_true, outPkg);
      PC_FusionEnd(&pc, &test);
      PC_AddEnd(&pc, PC_CreateJumpCond(1 + code.size + 1));
      PC_FusionEnd(&pc, &code);
      PC_AddEnd(&pc, PC_CreateJump(-test.size - 1 - code.size));
      break;
    }
    case AST_NODE_TYPE_STAT: {
      PrgmCode prgm;
      for (AST_NODE_STAT *cur = node; cur != NULL; cur = cur->next) {
        prgm = AST_ToCodeRec(AST_NODE_CAST_STAT(cur)->ptr, outPkg);
        PC_FusionEnd(&pc, &prgm);
      }
      break;
    }
    case AST_NODE_TYPE_FUNC_DEC: {
      // Création du code de la fonction
      // Code de la fonction
      PrgmCode args = AST_ToCodeRec(AST_NODE_CAST_FUNC_DEC(node)->args, outPkg);
      PrgmCode code = AST_ToCodeRec(AST_NODE_CAST_FUNC_DEC(node)->data, outPkg);
      PC_FusionEnd(&args, &code);
      // Force return if not set
      if (args.queu) {
        if (args.queu->code->type != PC_TYPE_RETURN) {
          PC_AddEnd(&args, PC_CreatePushCst(OBJ_NULL));
          PC_AddEnd(&args, PC_CreateReturn());
        }
      } else {
        PC_AddEnd(&args, PC_CreatePushCst(OBJ_NULL));
        PC_AddEnd(&args, PC_CreateReturn());
      }
      PrgmCodePoint *point = malloc(sizeof(PrgmCodePoint));
      point->code = args.head; // première instruction
      point->id = outPkg->size;
      // Ajout en tete
      point->next = outPkg->head;
      outPkg->head = point;
      outPkg->size += 1;

      // Code hors de la fonction
      PC_AddEnd(&pc, PC_CreatePushCst(OBJ_Create(OBJ_FUNC, &point->id, NULL)));
      break;
    }
    case AST_NODE_TYPE_FUNC_CALL: {
      PrgmCode args =
          AST_ToCodeRec(AST_NODE_CAST_FUNC_CALL(node)->args, outPkg);
      PrgmCode func =
          AST_ToCodeRec(AST_NODE_CAST_FUNC_CALL(node)->func, outPkg);
      PC_FusionEnd(&pc, &args);
      PC_FusionEnd(&pc, &func);
      PC_AddEnd(&pc, PC_CreateCall());
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
  dst->size += src->size;
}

void PrgmPointPrint(PrgmPoint *pp) {
  for (size_t cpt = 0; pp != NULL; pp = pp->next, cpt++) {
    printf("\t[%ld] ", cpt);
    PC_Print(pp->code);
    printf("\n");
  }
}

void PrgmPointFreeOnlyWrapper(PrgmPoint *point) {
  if (point)
    PrgmPointFreeOnlyWrapper(point->next);
  free(point);
}
