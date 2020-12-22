/*
 * obj_handler.c
 *
 *  Created on: 22/12/2020
 *      Author: pirx
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "obj_handler.h"
#include <assert.h>

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

FILE *stdout_po_alu = NULL;

const char *OBJ_TYPES_NAMES[] = {"BOOL", OBJ_INT_NAME, "CHAR", OBJ_STRING_NAME,
                                 "FUNC"};

const void *OBJ_FULL_FUNC[] = {NULL, OBJ_INT_FUNC, NULL, OBJ_STRING_FUNC, NULL};

/*******************************************************************************
 * Public function
 ******************************************************************************/

/*******************************************************************************
 * Internal function
 ******************************************************************************/

void PO_ALU_Init(void) { stdout_po_alu = LB_Init(1024); }

// obj =
OBJ *OBJ_Affect(OBJ *dst, OBJ *src) {
  fprintf(stdout_po_alu, "[\e[33mOBJ\e[39m]>>> ");
  fprintf(stdout_po_alu, "MV  ");
  OBJ_FPrint(stdout_po_alu, dst);
  fprintf(stdout_po_alu, " <- ");
  OBJ_FPrint(stdout_po_alu, src);

  assert(dst);
  assert(src);

  dst->data = src->data;
  dst->type = src->type;

  fprintf(stdout_po_alu, " ==> ");
  OBJ_FPrint(stdout_po_alu, dst);
  fprintf(stdout_po_alu, "\n");
  return dst;
}

// obj()
// TODO
OBJ *OBJ_Call(OBJ *func) {
  fprintf(stdout_po_alu, "[\e[33mOBJ\e[39m]>>> ");
  fprintf(stdout_po_alu, "CALL %s(%s)>\n", "TODO", "TODO");
  return func;
}

OBJ *OBJ_ApplyFunc1(OBJ_PRIMITIVES func, OBJ *obj1) {
  fprintf(stdout_po_alu, "[\e[33mOBJ\e[39m]>>> ");
  fprintf(stdout_po_alu, OBJ_FUNCS_NAMES[func]);
  OBJ_FPrint(stdout_po_alu, obj1);

  // type valide ? TODO delete
  const void *objfunctab = OBJ_FULL_FUNC[obj1->type];
  if (objfunctab == NULL) {
    printf("Invalid type\n");
    assert(0);
  }

  // fonction valide ?
  const OBJ_func1 objfunc = ((OBJ_func1 *)objfunctab)[func];

  if (objfunc == NULL) {
    printf("Invalid func\n");
    assert(0);
  }

  // all OK
  OBJ *ret = objfunc(obj1);
  fprintf(stdout_po_alu, " ==> ");
  OBJ_FPrint(stdout_po_alu, ret);
  fprintf(stdout_po_alu, "\n");
  return ret;
}

OBJ *OBJ_ApplyFunc2(OBJ_PRIMITIVES func, OBJ *obj1, OBJ *obj2) {
  fprintf(stdout_po_alu, "[\e[33mOBJ\e[39m]>>> ");
  fprintf(stdout_po_alu, OBJ_FUNCS_NAMES[func]);
  OBJ_FPrint(stdout_po_alu, obj1);
  OBJ_FPrint(stdout_po_alu, obj2);

  // type valide ? TODO delete
  const void *objfunctab = OBJ_FULL_FUNC[obj1->type];
  if (objfunctab == NULL) {
    printf("Invalid type\n");
    return obj1;
  }

  // fonction valide ?
  const OBJ_func2 objfunc = ((OBJ_func2 *)objfunctab)[func];

  if (objfunc == NULL) {
    printf("Invalid func\n");
    return obj1;
  }

  // all OK
  OBJ *ret = objfunc(obj1, obj2);
  fprintf(stdout_po_alu, " ==> ");
  OBJ_FPrint(stdout_po_alu, ret);
  fprintf(stdout_po_alu, "\n");
  return ret;
}

// print(obj) __str__
void OBJ_FPrint(FILE *pf, OBJ *obj) {
  if (obj == NULL) { //
    fprintf(pf, "(NULL)");
    return;
  }
  fprintf(pf, "(%s:", OBJ_TYPES_NAMES[obj->type]);
  switch (obj->type) {
  case OBJ_INT:
    assert(obj->data);
    fprintf(pf, "%d", *(OBJ_int_t *)obj->data);
    break;
  case OBJ_STRING:
    fprintf(pf, "%s", ((OBJ_string *)obj->data)->s);
    break;
  case OBJ_FUNC:
    fprintf(pf, "%d", *(OBJ_int_t *)obj->data);
    break;
  default:
    fprintf(pf, "TODO");
  }
  fprintf(pf, ")");
}

void OBJ_Print(OBJ *obj) { OBJ_FPrint(stdout, obj); }
