/*
 * po_ctxstack.c
 *
 *  Created on: 20/12/2020
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "po_ctxstack.h"
#include "obj.h"
#include "utils/hashtable.h"
#include <stdint.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

#define CTX_STACK_SIZE 1000

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

static void htformat(FILE *pf, char *name, void *obj);

/*******************************************************************************
 * Variables
 ******************************************************************************/

HashTable *ctxstack[CTX_STACK_SIZE] = {0};
uint32_t ctxIndex = 0;

HashTable *curctx = NULL;

/*******************************************************************************
 * Public function
 ******************************************************************************/

void CTX_enter() {
  ctxstack[ctxIndex] = HT_Init();
  curctx = ctxstack[ctxIndex++];
}

void CTX_leave() {
  // HT_free(ctxstack[ctxIndex]);
  ctxstack[ctxIndex] = NULL;
  curctx = ctxstack[ctxIndex--];
}

void CTX_set(char *name, OBJ *o) { HT_Insert(curctx, name, o); }

OBJ *CTX_get(char *name) { return HT_Get(curctx, name); }

void CTX_printCur() { HF_fprintFormat(stdout, curctx, htformat); }

/*******************************************************************************
 * Internal function
 ******************************************************************************/

static void htformat(FILE *pf, char *name, void *obj) {
  fprintf(pf, "%s:", name);
  OBJ_FPrint(pf, obj);
  fprintf(pf, "\n");
}
