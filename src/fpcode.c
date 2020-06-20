/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "fpcode.h"
#include "mem.h"
#include "obj.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

#define STACK_SIZE 1024

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

void PC_Push(OBJ *obj);
OBJ *PC_Pop(void);
void PC_Apply(OBJ_PRIMITIVES);
void *PC_Call(OBJ *obj_func);
void PC_Affect(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static OBJ *stack[STACK_SIZE];
static size_t i_stack = 0; // Point sur un case vide

const char *PC_TYPE_NAME[] = {
    "PUSH_SRC_VAR", "PUSH_DST_VAR",   "PUSH_CST",
    "POP",          "APPLY_OBJ_FUNC", "CALL",
    "AFFECT",       "JUMP",           "CONDITIONAL_JUMP"};

/*******************************************************************************
 * Public function
 ******************************************************************************/

void PC_PrintCodeWhenExec(PCODE *code) {
  assert(code);
  printf("[\e[34mFPC\e[39m]>>> ");
  printf("%s(", PC_TYPE_NAME[code->type]);
  switch (code->type) {
  case PUSH_SRC_VAR:
    printf("\"%s\"", code->arg.pchar_t);
    break;
  case PUSH_DST_VAR:
    printf("\"%s\"", code->arg.pchar_t);
    break;
  case PUSH_CST:
    OBJ_Print(code->arg.pobj_t);
    break;
  case POP:
    break;
  case APPLY_OBJ_FUNC:
    printf("\"%s\"", OBJ_FUNCS_NAMES[code->arg.int_t]);
    if (OBJ_NB_ARGS[code->arg.int_t] >= 1) {
      printf(", ");
      OBJ_Print(stack[i_stack - 1]);
    }
    if ((OBJ_NB_ARGS[code->arg.int_t] >= 2)) {
      printf(", ");
      OBJ_Print(stack[i_stack - 2]);
    }
    break;
  case CALL:
    break;
  case AFFECT:
    OBJ_Print(stack[i_stack - 1]);
    printf(", ");
    OBJ_Print(stack[i_stack - 2]);
    break;
  case JUMP:
    break;
  case CONDITIONAL_JUMP:
    PC_Pop();
    break;
  default:
    printf("Unkonw code !!!");
    break;
  }
  printf(")\e[39m\n");
}

void PC_FPrint(FILE *pf, PCODE *code) {
  assert(code);
  fprintf(pf, "[%s / ", PC_TYPE_NAME[code->type]);
  switch (code->type) {
  case PUSH_SRC_VAR:
    fprintf(pf, "%s", code->arg.pchar_t);
    break;
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
  case JUMP:
    fprintf(pf, "(%d)", code->arg.int_t);
    break;
  case CONDITIONAL_JUMP:
    fprintf(pf, "(%d)", code->arg.int_t);
    break;
  }
  fprintf(pf, "]");
}

void PC_Print(PCODE *code) { PC_FPrint(stdout, code); }

void *PC_RunFpcode(PCODE *code) {
  assert(code);
  PC_PrintCodeWhenExec(code);
  // FPC_PrintStack();
  switch (code->type) {
  case PUSH_SRC_VAR: {
    char *name = code->arg.pchar_t;
    OBJ *obj = MEM_GetObj(name);
    PC_Push(obj);
    break;
  }
  case PUSH_DST_VAR: {
    char *name = code->arg.pchar_t;
    OBJ *obj = MEM_GetOrCreateObj(name);
    PC_Push(obj);
    break;
  }
  case PUSH_CST: {
    OBJ *obj = code->arg.pobj_t;
    PC_Push(obj);
    break;
  }
  case POP:
    PC_Pop();
    break;
  case APPLY_OBJ_FUNC:
    PC_Apply(code->arg.int_t);
    break;
  case CALL:
    PC_Call(code->arg.pobj_t);
    break;
  case AFFECT:
    PC_Affect();
    break;
  case JUMP:
    break;
  case CONDITIONAL_JUMP:

    break;
  default:
    printf("Invalid CODE\n");
    exit(1);
    break;
  }
  return NULL;
}

void PC_PrintStack(void) {
  for (size_t i = 0; i < i_stack; i++) {
    printf("PC[%.5ld]:", i);
    OBJ_Print(stack[i]);
    printf("\n");
  }
}

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

void PC_Push(OBJ *obj) {
  assert(i_stack < STACK_SIZE - 1);
  stack[i_stack++] = obj;
}

OBJ *PC_Pop(void) {
  assert(i_stack < STACK_SIZE - 1);
  return stack[i_stack-- - 1];
}

void PC_Apply(OBJ_PRIMITIVES func) {
  if (OBJ_NB_ARGS[func] == 2) {
    PC_Push(OBJ_ApplyFunc2(func, PC_Pop(), PC_Pop()));
  } else if ((OBJ_NB_ARGS[func] == 1)) {
    PC_Push(OBJ_ApplyFunc1(func, PC_Pop()));
  } else {
    printf("AYE AYE AYE !");
    assert(0);
  }
}

void *PC_Call(OBJ *obj_func) { /* TODO */
  return obj_func;
}

void PC_Affect(void) {
  OBJ *src = PC_Pop();
  OBJ *dst = PC_Pop();
  OBJ_Affect(dst, src);
}
