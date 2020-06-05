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

const char *FPC_TYPE_NAME[] = {"PUSH", "POP", "APPLY_OBJ_FUNC", "CALL",
                               "AFFECT"};

/*******************************************************************************
 * Public function
 ******************************************************************************/

void *FPC_RunFpcode(FPCODE *code) {
  assert(code);

  printf("============= run code %s on stack =>\n", FPC_TYPE_NAME[code->type]);
  FPC_PrintStack();
  switch (code->type) {
  case PUSH:
    FPC_Push(code->arg);
    break;
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
