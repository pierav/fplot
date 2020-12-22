/*
 * po_ctxstack.c
 *
 *  Created on: 20/12/2020
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "po_ctxstack.h"
#include "logbuffer.h"
#include "obj_handler.h"
#include "utils/hashtable.h"
#include <assert.h>
#include <stdint.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

#define CTX_STACK_SIZE 1000

/*******************************************************************************
 * Types
 ******************************************************************************/

struct Context {
  // Espace des variables
  HashTable *namespace;
  // pc courant
  size_t pc;
  size_t mainpc;
};
typedef struct Context Context;

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

static void htformat(FILE *pf, char *name, void *obj);
static OBJ *CTX_get(char *name);

/*******************************************************************************
 * Variables
 ******************************************************************************/

FILE *stdout_po_ctx = NULL;

Context *ctxstack[CTX_STACK_SIZE] = {0};
uint32_t ctxIndex = 0;

Context *curctx = NULL;

/*******************************************************************************
 * Public function
 ******************************************************************************/

void CTX_Init(void) { stdout_po_ctx = LB_Init(1024); }

// STACK
void CTX_enter() {
  assert(ctxIndex < CTX_STACK_SIZE - 1);
  fprintf(stdout_po_ctx, "[\e[32mCTX\e[39m]>>> enter CTX [%d]\n", ctxIndex);
  ctxstack[ctxIndex] = malloc(sizeof(Context));
  curctx = ctxstack[ctxIndex++];
  curctx->namespace = HT_Init();
  curctx->pc = 0;
  curctx->mainpc = 0;
}

void CTX_leave() {
  assert(ctxIndex > 0);
  fprintf(stdout_po_ctx, "[\e[32mCTX\e[39m]>>> leave CTX [%d]\n", ctxIndex - 1);
  // HT_free(ctxstack[ctxIndex]);
  ctxIndex--;
  ctxstack[ctxIndex] = NULL;
  curctx = ctxstack[ctxIndex - 1];
}

// Namespace
// link name obj
OBJ *CTX_setObj(char *name, OBJ *o) {
  fprintf(stdout_po_ctx, "[\e[32mCTX\e[39m]>>> link %s <->", name);
  OBJ_FPrint(stdout_po_ctx, o);
  fprintf(stdout_po_ctx, "\n");
  HT_Insert(curctx->namespace, name, o);
  return o;
}

// Retourne l'objet de nom name
OBJ *CTX_getObj(char *name) {
  OBJ *ret = CTX_get(name);
  if (ret == NULL) {
    printf("USR# undefined varable : %s", name);
    exit(1);
  }
  return ret;
}

void CTX_printCur() {
  fprintf(stdout_po_ctx, "Namespace :\n");
  HF_fprintFormat(stdout, curctx->namespace, htformat);
  fprintf(stdout_po_ctx, "PC: %ld | MPC: %ld\n", CTX_PC_Get(),
          CTX_PC_getMainPc());
}

void CTX_PC_Set(size_t npc) {
  int delta = npc - curctx->pc; // debug
  curctx->pc = npc;
  fprintf(stdout_po_ctx, "[\e[32m PC\e[39m]>>> pc <- %ld [+%d]\n", curctx->pc,
          delta);
}

void CTX_PC_Inc(void) { CTX_PC_Add(1); }

size_t CTX_PC_Get(void) { return curctx->pc; }

void CTX_PC_Add(size_t dpc) { CTX_PC_Set(curctx->pc + dpc); }

void CTX_PC_setMainPc(size_t value) {
  curctx->mainpc = value;
  fprintf(stdout_po_ctx, "[\e[32m PC\e[39m]>>> mainpc <- %ld]\n",
          curctx->mainpc);
}

size_t CTX_PC_getMainPc() { return curctx->mainpc; }

/*******************************************************************************
 * Internal function
 ******************************************************************************/

static OBJ *CTX_get(char *name) { return HT_Get(curctx->namespace, name); }

static void htformat(FILE *pf, char *name, void *obj) {
  fprintf(pf, "%s:", name);
  OBJ_FPrint(pf, obj);
  fprintf(pf, "\n");
}
