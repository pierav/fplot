/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "fpcode.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

void PC_FPrintGen(FILE *pf, PCODE *code, int forTerm);

/*******************************************************************************
 * Variables
 ******************************************************************************/

const char *PC_TYPE_NAME[] = {
    "PUSH_CST",         "POP",    "APPLY_OBJ_FUNC", "CALL", "JUMP",
    "CONDITIONAL_JUMP", "RETURN", "STORE",          "LOAD", "CLASS_INIT"};

/*******************************************************************************
 * Public function
 ******************************************************************************/

void PC_Print(PCODE *code) { PC_FPrintGen(stdout, code, 1); }
void PC_FPrint(FILE *pf, PCODE *code) { PC_FPrintGen(pf, code, 0); }

const char *PC_GetName(PCODE *code) { return PC_TYPE_NAME[code->type]; }

PCODE *PC_Create(PC_TYPE type, PC_ARG arg) {
  PCODE *ret = malloc(sizeof(struct PCODE));
  assert(ret);
  ret->type = type;
  ret->arg = arg;
  return ret;
}

PCODE *PC_CreateJump(size_t arg) {
  return PC_Create(PC_TYPE_JUMP, (PC_ARG)arg);
}

PCODE *PC_CreateJumpCond(size_t arg) {
  return PC_Create(PC_TYPE_CONDITIONAL_JUMP, (PC_ARG)arg);
}

PCODE *PC_CreatePushCst(OBJ *obj) {
  return PC_Create(PC_TYPE_PUSH_CST, (PC_ARG)obj);
}

PCODE *PC_CreatePop(void) { return PC_Create(PC_TYPE_POP, (PC_ARG)0UL); }

PCODE *PC_CreateApply(size_t func) {
  return PC_Create(PC_TYPE_APPLY_OBJ_FUNC, (PC_ARG)func);
}

PCODE *PC_CreateCall(void) { return PC_Create(PC_TYPE_CALL, (PC_ARG)0UL); }

PCODE *PC_CreateReturn(void) { return PC_Create(PC_TYPE_RETURN, (PC_ARG)0UL); }

PCODE *PC_CreateStore(char *name) {
  return PC_Create(PC_TYPE_STORE, (PC_ARG)name);
}

PCODE *PC_CreateLoad(char *name) {
  return PC_Create(PC_TYPE_LOAD, (PC_ARG)name);
}
PCODE *PC_CreateClassInit(char *name) {
  return PC_Create(PC_TYPE_CLASS_INIT, (PC_ARG)name);
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/

void PC_FPrintGen(FILE *pf, PCODE *code, int forTerm) {
  assert(code);
  fprintf(pf, "%s ", PC_TYPE_NAME[code->type]);
  if (forTerm) {
    for (int i = strlen(PC_TYPE_NAME[code->type]); i < 19; i++)
      fprintf(pf, "-");
    fprintf(pf, " ");
  }
  switch (code->type) {
  case PC_TYPE_STORE: // fallthrough
  case PC_TYPE_LOAD:
  case PC_TYPE_CLASS_INIT:
    fprintf(pf, "%s", code->arg.pchar_t);
    break;
  case PC_TYPE_PUSH_CST:
    OBJ_FPrint(pf, code->arg.pobj_t);
    break;
  case PC_TYPE_APPLY_OBJ_FUNC:
    fprintf(pf, "%s", OBJ_FUNCS_NAMES[code->arg.int_t]);
    break;
  case PC_TYPE_JUMP: // fallthrough
  case PC_TYPE_CONDITIONAL_JUMP:
    fprintf(pf, "(%ld)", code->arg.int_t);
    break;
  case PC_TYPE_CALL: // fallthrough
  case PC_TYPE_POP:
  case PC_TYPE_RETURN:
    break;
  }
  fprintf(pf, " ");
}
