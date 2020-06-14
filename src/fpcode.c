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

void FPC_Push(OBJ *obj);
OBJ *FPC_Pop(void);
void FPC_Apply(OBJ_PRIMITIVES);
void *FPC_Call(OBJ *obj_func);
void FPC_Affect(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static OBJ *stack[STACK_SIZE];
static size_t i_stack = 0; // Point sur un case vide

const char *FPC_TYPE_NAME[] = {
    "PUSH_SRC_VAR", "PUSH_DST_VAR",   "PUSH_CST",
    "POP",          "APPLY_OBJ_FUNC", "CALL",
    "AFFECT",       "JUMP",           "CONDITIONAL_JUMP"};

/*******************************************************************************
 * Public function
 ******************************************************************************/

void FPC_PrintCodeWhenExec(FPCODE *code) {
  assert(code);
  printf("[\e[34mFPC\e[39m]>>> ");
  printf("%s(", FPC_TYPE_NAME[code->type]);
  switch (code->type) {
  case PUSH_SRC_VAR:
    printf("\"%s\"", (char *)code->arg);
    break;
  case PUSH_DST_VAR:
    printf("\"%s\"", (char *)code->arg);
    break;
  case PUSH_CST:
    OBJ_Print((OBJ *)code->arg);
    break;
  case POP:
    break;
  case APPLY_OBJ_FUNC:
    printf("\"%s\"", OBJ_FUNCS_NAMES[(OBJ_PRIMITIVES)code->arg]);
    if (OBJ_NB_ARGS[(OBJ_PRIMITIVES)code->arg] >= 1) {
      printf(", ");
      OBJ_Print(stack[i_stack - 1]);
    }
    if ((OBJ_NB_ARGS[(OBJ_PRIMITIVES)code->arg] >= 2)) {
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
    FPC_Pop();
    break;
  default:
    printf("Unkonw code !!!");
    break;
  }
  printf(")\e[39m\n");
}

void FPC_Print(FPCODE *code) {
  assert(code);
  printf("[%s / ", FPC_TYPE_NAME[code->type]);
  switch (code->type) {
  case PUSH_SRC_VAR:
    printf("\"%s\"", (char *)code->arg);
    break;
  case PUSH_DST_VAR:
    printf("\"%s\"", (char *)code->arg);
    break;
  case PUSH_CST:
    OBJ_Print((OBJ *)code->arg);
    break;
  case POP:
    break;
  case APPLY_OBJ_FUNC:
    printf("\"%s\"", OBJ_FUNCS_NAMES[(OBJ_PRIMITIVES)code->arg]);
    break;
  case CALL:
    break;
  case AFFECT:
    break;
  case JUMP:
    break;
  case CONDITIONAL_JUMP:
    printf("node[%d]", (int)code->arg);
    break;
  }
  printf("]");
}

void *FPC_RunFpcode(FPCODE *code) {
  assert(code);
  FPC_PrintCodeWhenExec(code);
  // FPC_PrintStack();
  switch (code->type) {
  case PUSH_SRC_VAR: {
    char *name = code->arg;
    OBJ *obj = MEM_GetObj(name);
    FPC_Push(obj);
    break;
  }
  case PUSH_DST_VAR: {
    char *name = code->arg;
    OBJ *obj = MEM_GetOrCreateObj(name);
    FPC_Push(obj);
    break;
  }
  case PUSH_CST: {
    OBJ *obj = code->arg;
    FPC_Push(obj);
    break;
  }
  case POP:
    FPC_Pop();
    break;
  case APPLY_OBJ_FUNC:
    FPC_Apply((OBJ_PRIMITIVES)code->arg);
    break;
  case CALL:
    FPC_Call(code->arg);
    break;
  case AFFECT:
    FPC_Affect();
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

void FPC_PrintStack(void) {
  for (size_t i = 0; i < i_stack; i++) {
    printf("FPC[%.5ld]:", i);
    OBJ_Print(stack[i]);
    printf("\n");
  }
}

FPCODE *FPC_Create(FPC_TYPE type, void *arg) {
  FPCODE *ret = malloc(sizeof(FPCODE));
  assert(ret);
  ret->type = type;
  ret->arg = arg;
  return ret;
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/

void FPC_Push(OBJ *obj) {
  assert(i_stack < STACK_SIZE - 1);
  stack[i_stack++] = obj;
}

OBJ *FPC_Pop(void) {
  assert(i_stack < STACK_SIZE - 1);
  return stack[i_stack-- - 1];
}

void FPC_Apply(OBJ_PRIMITIVES func) {
  if (OBJ_NB_ARGS[func] == 2) {
    FPC_Push(OBJ_ApplyFunc2(func, FPC_Pop(), FPC_Pop()));
  } else if ((OBJ_NB_ARGS[func] == 1)) {
    FPC_Push(OBJ_ApplyFunc1(func, FPC_Pop()));
  } else {
    printf("AYE AYE AYE !");
    assert(0);
  }
}

void *FPC_Call(OBJ *obj_func) { /* TODO */
  return obj_func;
}

void FPC_Affect(void) {
  OBJ *src = FPC_Pop();
  OBJ *dst = FPC_Pop();
  OBJ_Affect(dst, src);
}
