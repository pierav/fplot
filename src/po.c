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
#include "po_pc.h"

#include "mem.h"
#include "mem_prgm.h"
#include "po_objstack.h"

#include "logbuffer.h"

#include "obj.h"
#include "obj_int.h"

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
PCODE *code = NULL;

FILE *stdout_po;

/*******************************************************************************
 * Public function
 ******************************************************************************/

void PO_Run(void) {
  stdout_po = LB_Init(1024);
  PO_PC_Init();
  PO_ALU_Init();

  run = true;
  while (run) {
    PO_Iter();
    fflush(stdout_po);
    fflush(stdout_po_alu);
    fflush(stdout_po_pc);
  }
}

void PO_Iter(void) {
  // GET mem[pc]
  code = MEMPRGM_Get(PO_PC_Get());
  if (code == NULL) { // EOP
    run = 0;
    return;
  }

  fprintf(stdout_po, "[\e[34mFPC\e[39m]>>> [%ld]", PO_PC_Get());
  PC_FPrint(stdout_po, code);
  fprintf(stdout_po, "(");

  // Exec code
  switch (code->type) {
  case PC_TYPE_PUSH_SRC_VAR: {
    char *name = code->arg.pchar_t;
    OBJ *obj = MEM_GetObj(name);
    fprintf(stdout_po, "\"%s\":", name);
    OBJ_FPrint(stdout_po, obj);
    PO_OBJSTACK_Push(obj);
    PO_PC_Inc();
    break;
  }
  case PC_TYPE_PUSH_DST_VAR: {
    char *name = code->arg.pchar_t;
    OBJ *obj = MEM_GetOrCreateObj(name);
    fprintf(stdout_po, "\"%s\":", name);
    OBJ_FPrint(stdout_po, obj);
    PO_OBJSTACK_Push(obj);
    PO_PC_Inc();
    break;
  }
  case PC_TYPE_PUSH_CST: {
    OBJ *obj = code->arg.pobj_t;
    OBJ_FPrint(stdout_po, obj);
    PO_OBJSTACK_Push(obj);
    PO_PC_Inc();
    break;
  }
  case PC_TYPE_POP:
    PO_OBJSTACK_Pop();
    PO_PC_Inc();
    break;
  case PC_TYPE_APPLY_OBJ_FUNC: {
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
    PO_PC_Inc();
  } break;
  case PC_TYPE_AFFECT: {
    OBJ *src = PO_OBJSTACK_Pop();
    OBJ *dst = PO_OBJSTACK_Pop();
    OBJ_Affect(dst, src);
    OBJ_FPrint(stdout_po, src);
    fprintf(stdout_po, ", ");
    OBJ_FPrint(stdout_po, dst);
    PO_PC_Inc();
  } break;
  case PC_TYPE_JUMP:
    PO_PC_Add(code->arg.int_t);
    break;
  case PC_TYPE_CONDITIONAL_JUMP: {
    OBJ *test = PO_OBJSTACK_Pop();
    fprintf(stdout_po, "test:");
    OBJ_FPrint(stdout_po, test);
    OBJ *res_test = OBJ_ApplyFunc1(__INT__, test);
    fprintf(stdout_po, "::");
    OBJ_FPrint(stdout_po, res_test);
    fprintf(stdout_po, "%s", *(int *)res_test->data ? ":true" : ":false");
    if (*(int *)res_test->data) // do nothing
      PO_PC_Inc();
    else // Jump if false
      PO_PC_Add(code->arg.int_t);
  } break;
  default:
    printf("Invalid CODE\n");
    assert(0);
    break;
  case PC_TYPE_CALL:
    assert(0);
    PO_OBJSTACK_Pop(); // POP func //
    PO_PC_Inc();
    // TODO
    break;
  }

  fprintf(stdout_po, ")\e[39m\n");
  fflush(stdout_po);
  return;
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/
