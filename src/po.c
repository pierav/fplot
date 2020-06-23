/*
 * po.c
 *
 *  Created on: 23/06/2020
 *      Author: pirx
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "po.h"
#include "mem.h"
#include "mem_prgm.h"
#include "po_objstack.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

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

bool run = false;
size_t pc = 0; // Program counter
PCODE *code = NULL;

/*******************************************************************************
 * Public function
 ******************************************************************************/

void PO_Run(void) {
  run = true;
  while (run)
    PO_Iter();
}

void PO_Iter(void) {
  // GET mem[pc]
  code = MEMPRGM_Get(pc);
  if (code == NULL) {
    run = 0;
    return;
  }

  printf("[\e[34mFPC\e[39m]>>> ");
  printf("%s(", PC_GetName(code));

  // Exec code
  switch (code->type) {
  case PUSH_SRC_VAR: {
    printf("\"%s\"", code->arg.pchar_t);
    char *name = code->arg.pchar_t;
    OBJ *obj = MEM_GetObj(name);
    PO_OBJSTACK_Push(obj);
    pc++;
    break;
  }
  case PUSH_DST_VAR: {
    printf("\"%s\"", code->arg.pchar_t);
    char *name = code->arg.pchar_t;
    OBJ *obj = MEM_GetOrCreateObj(name);
    PO_OBJSTACK_Push(obj);
    pc++;
    break;
  }
  case PUSH_CST: {
    OBJ_Print(code->arg.pobj_t);
    OBJ *obj = code->arg.pobj_t;
    PO_OBJSTACK_Push(obj);
    pc++;
    break;
  }
  case POP:
    PO_OBJSTACK_Pop();
    pc++;
    break;
  case APPLY_OBJ_FUNC: {
    printf("\"%s\"", OBJ_FUNCS_NAMES[code->arg.int_t]);
    if (OBJ_NB_ARGS[code->arg.int_t] >= 1) {
      printf(", ");
      PO_OBJSTACK_PrintDebugOBJ(0);
    }
    if ((OBJ_NB_ARGS[code->arg.int_t] >= 2)) {
      printf(", ");
      PO_OBJSTACK_PrintDebugOBJ(1);
    }

    OBJ_PRIMITIVES func = code->arg.int_t;
    if (OBJ_NB_ARGS[func] == 2) {
      PO_OBJSTACK_Push(
          OBJ_ApplyFunc2(func, PO_OBJSTACK_Pop(), PO_OBJSTACK_Pop()));
    } else if ((OBJ_NB_ARGS[func] == 1)) {
      PO_OBJSTACK_Push(OBJ_ApplyFunc1(func, PO_OBJSTACK_Pop()));
    } else {
      printf("AYE AYE AYE !");
      assert(0);
    }
    pc++;
  } break;
  case CALL:
    pc++;
    // TODO
    break;
  case AFFECT: {
    OBJ *src = PO_OBJSTACK_Pop();
    OBJ *dst = PO_OBJSTACK_Pop();
    OBJ_Affect(dst, src);
    OBJ_Print(src);
    printf(", ");
    OBJ_Print(dst);
    pc++;
  } break;
  case JUMP:
    pc++; // TODO
    break;
  case CONDITIONAL_JUMP: {
    OBJ *test = PO_OBJSTACK_Pop();
    OBJ_Print(test);
    pc++; // TODO
  } break;
  default:
    printf("Invalid CODE\n");
    exit(1);
    break;
  }
  printf(")\e[39m\n");
  return;
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/
