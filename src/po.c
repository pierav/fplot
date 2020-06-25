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

#include "logbuffer.h"

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

FILE *stdout_po;

/*******************************************************************************
 * Public function
 ******************************************************************************/

void PO_Run(void) {
  stdout_po = LB_Init(2048);

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

  fprintf(stdout_po, "[\e[34mFPC\e[39m]>>> ");
  fprintf(stdout_po, "%s(", PC_GetName(code));

  // Exec code
  switch (code->type) {
  case PUSH_SRC_VAR: {
    char *name = code->arg.pchar_t;
    OBJ *obj = MEM_GetObj(name);
    fprintf(stdout_po, "\"%s\":", name);
    OBJ_FPrint(stdout_po, obj);
    PO_OBJSTACK_Push(obj);
    pc++;
    break;
  }
  case PUSH_DST_VAR: {
    char *name = code->arg.pchar_t;
    OBJ *obj = MEM_GetOrCreateObj(name);
    fprintf(stdout_po, "\"%s\":", name);
    OBJ_FPrint(stdout_po, obj);
    PO_OBJSTACK_Push(obj);
    pc++;
    break;
  }
  case PUSH_CST: {
    OBJ *obj = code->arg.pobj_t;
    OBJ_FPrint(stdout_po, obj);
    PO_OBJSTACK_Push(obj);
    pc++;
    break;
  }
  case POP:
    PO_OBJSTACK_Pop();
    pc++;
    break;
  case APPLY_OBJ_FUNC: {
    fprintf(stdout_po, "\"%s\"", OBJ_FUNCS_NAMES[code->arg.int_t]);
    if (OBJ_NB_ARGS[code->arg.int_t] >= 1) {
      fprintf(stdout_po, ", ");
      PO_OBJSTACK_PrintDebugOBJ(stdout_po, 0);
    }
    if ((OBJ_NB_ARGS[code->arg.int_t] >= 2)) {
      fprintf(stdout_po, ", ");
      PO_OBJSTACK_PrintDebugOBJ(stdout_po, 1);
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
    OBJ_FPrint(stdout_po, src);
    fprintf(stdout_po, ", ");
    OBJ_FPrint(stdout_po, dst);
    pc++;
  } break;
  case JUMP:
    pc++; // TODO
    break;
  case CONDITIONAL_JUMP: {
    OBJ *test = PO_OBJSTACK_Pop();
    // OBJ_Print(test);
    pc++; // TODO
  } break;
  default:
    printf("Invalid CODE\n");
    exit(1);
    break;
  }
  fprintf(stdout_po, ")\e[39m\n");
  fflush(stdout_po);
  return;
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/
