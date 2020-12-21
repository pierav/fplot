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
#include "po_ctxstack.h"
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

void flushall(void);

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
  fprintf(stdout_po, "=== begin PO ===\n");
  fflush(stdout_po);

  CTX_Init();
  PO_ALU_Init();
  CTX_enter(); // On entre dans le contexte d'entree
  flushall();

  run = true;
  while (run) {
    PO_Iter();
    flushall();
  }
  fprintf(stdout_po, "=== end PO ===\n");
  flushall();
}

void PO_Iter(void) {
  // GET mem[pc]
  code = MEMPRGM_Get(CTX_PC_getMainPc(), CTX_PC_Get());
  if (code == NULL) { // EOP
    run = 0;
    return;
  }

  fprintf(stdout_po, "[\e[34mFPC\e[39m]>>> [%ld][%ld] ", CTX_PC_getMainPc(),
          CTX_PC_Get());
  PC_FPrint(stdout_po, code);
  fprintf(stdout_po, "(");

  // Exec code
  switch (code->type) {
  case PC_TYPE_PUSH_SRC_VAR: {
    char *name = code->arg.pchar_t;
    OBJ *obj = MEM_GetObj(name);
    fprintf(stdout_po, "\"%s\":", name);
    PO_OBJSTACK_Push(obj);
    CTX_PC_Inc();
    break;
  }
  case PC_TYPE_PUSH_DST_VAR: {
    char *name = code->arg.pchar_t;
    OBJ *obj = MEM_GetOrCreateObj(name);
    fprintf(stdout_po, "\"%s\":", name);
    OBJ_FPrint(stdout_po, obj);
    PO_OBJSTACK_Push(obj);
    CTX_PC_Inc();
    break;
  }
  case PC_TYPE_PUSH_CST: {
    OBJ *obj = code->arg.pobj_t;
    PO_OBJSTACK_Push(obj);
    CTX_PC_Inc();
    break;
  }
  case PC_TYPE_POP:
    PO_OBJSTACK_Pop();
    CTX_PC_Inc();
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
    CTX_PC_Inc();
  } break;
  case PC_TYPE_AFFECT: {
    OBJ *src = PO_OBJSTACK_Pop();
    OBJ *dst = PO_OBJSTACK_Pop();
    OBJ_Affect(dst, src);
    OBJ_FPrint(stdout_po, src);
    fprintf(stdout_po, ", ");
    OBJ_FPrint(stdout_po, dst);
    CTX_PC_Inc();
  } break;
  case PC_TYPE_JUMP:
    CTX_PC_Add(code->arg.int_t);
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
      CTX_PC_Inc();
    else // Jump if false
      CTX_PC_Add(code->arg.int_t);
  } break;
  case PC_TYPE_CALL: {
    fprintf(stdout_po, "CALL: ");
    OBJ *func = PO_OBJSTACK_Pop(); // POP func
    OBJ_FPrint(stdout_po, func);
    // new context
    CTX_enter();
    CTX_PC_setMainPc(*(int *)func->data + 1);
    CTX_PC_Set(0);

  } break;
  case PC_TYPE_RETURN: {
    // Valeur de retour en tete de pile a la fin de la func
    //    PO_OBJSTACK_Push(OBJ_NULL); // Default return TODO delete me
    // Recuperation du contexte
    // CTX_PC_Set(0); // reset PC TODO
    CTX_leave();
    // Inc cur
    CTX_PC_Inc();
  } break;
  case PC_TYPE_AFFECT_ARG: {
    char *name = code->arg.pchar_t;
    fprintf(stdout_po, name);
    CTX_set(name, PO_OBJSTACK_Pop());
    CTX_PC_Inc();
  } break;
  default:
    printf("Invalid CODE\n");
    flushall();
    printf("\n");
    assert(0);
    break;
  }

  fprintf(stdout_po, ")\e[39m\n");
  fflush(stdout_po);
  return;
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/

void flushall(void) {
  fflush(stdout_po);
  fflush(stdout_po_alu);
  fflush(stdout_po_ctx);
}
