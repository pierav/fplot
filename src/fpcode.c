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

/*******************************************************************************
 * Variables
 ******************************************************************************/

const char *PC_TYPE_NAME[] = {
    "PUSH_SRC_VAR", "PUSH_DST_VAR",   "PUSH_CST",
    "POP",          "APPLY_OBJ_FUNC", "CALL",
    "AFFECT",       "JUMP",           "CONDITIONAL_JUMP"};

/*******************************************************************************
 * Public function
 ******************************************************************************/

void PC_FPrint(FILE *pf, PCODE *code) {
  assert(code);
  fprintf(pf, "(%s / ", PC_TYPE_NAME[code->type]);
  switch (code->type) {
  case PUSH_SRC_VAR: // fallthrough
  case PUSH_DST_VAR:
    fprintf(pf, "%s", code->arg.pchar_t);
    break;
  case PUSH_CST:
    OBJ_FPrint(pf, code->arg.pobj_t);
    break;
  case POP:
    break;
  case APPLY_OBJ_FUNC:
    fprintf(pf, "%s", OBJ_FUNCS_NAMES[code->arg.int_t]);
    break;
  case CALL:
    break;
  case AFFECT:
    break;
  case JUMP: // fallthrough
  case CONDITIONAL_JUMP:
    fprintf(pf, "(%d)", code->arg.int_t);
    break;
  }
  fprintf(pf, ")");
}

void PC_Print(PCODE *code) { PC_FPrint(stdout, code); }

const char *PC_GetName(PCODE *code) { return PC_TYPE_NAME[code->type]; }

PCODE *PC_Create(PC_TYPE type, PC_ARG arg) {
  PCODE *ret = malloc(sizeof(struct PCODE));
  assert(ret);
  ret->type = type;
  ret->arg = arg;
  return ret;
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/
