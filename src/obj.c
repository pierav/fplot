/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "obj.h"
#include "obj_int.h"

#include "logbuffer.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

typedef OBJ *(*OBJ_func1)(OBJ *);
typedef OBJ *(*OBJ_func2)(OBJ *, OBJ *);

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

FILE *stdout_po_alu = NULL;

const char *OBJ_FUNCS_NAMES[NB_OBJ_FUNC] = {
    "__ADD__", "__SUB__",  "__MUL__", "__TDV__",   "__POW__", "__FDV__",
    "__MOD__", "__NEG__",  "__POS__", "__ABS__",   "__EQ__",  "__NE__",
    "__LT__",  "__GT__",   "__LE__",  "__GE__",    "__LEN__", "__GETITEM__",
    "__STR__", "__REPR__", "__INT__", "__DOUBLE__"};

const uint8_t OBJ_NB_ARGS[NB_OBJ_FUNC] = {2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 2,
                                          2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1};

const char *OBJ_TYPES_NAMES[] = {"BOOL", "INT", "CHAR", "STR", "FUNC"};

const void *OBJ_FULL_FUNC[] = {NULL, OBJ_INT_FUNC, NULL, NULL, NULL};

/*******************************************************************************
 * Public function
 ******************************************************************************/

void PO_ALU_Init(void) { stdout_po_alu = LB_Init(1024); }

OBJ *OBJ_Create(OBJ_TYPE type, void *value, char *name) {
  // printf("[\e[33mOBJ\e[39m]>>> ");
  // printf("NEW (%s) ", name);
  OBJ *ret = malloc(sizeof(struct OBJ));
  ret->type = type;
  ret->data = value;
  ret->name = name;
  ret->cpt_usage = 0;
  // OBJ_Print(ret);
  // printf("\n");
  return ret;
}

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
  fprintf(stdout_po_alu, "CALL %s(%s)>\n", func->name, "TODO");
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
    fprintf(pf, "<NULL>");
    return;
  }
  fprintf(pf, "<%s#%s:", OBJ_TYPES_NAMES[obj->type],
          obj->name ? obj->name : "_");
  switch (obj->type) {
  case OBJ_INT:
    assert(obj->data);
    fprintf(pf, "%d", *(int *)obj->data);
    break;
  case OBJ_STR:
    fprintf(pf, "%s", (char *)obj->data);
    break;
  case OBJ_FUNC:
    fprintf(pf, "%d", *(int *)obj->data);
    break;
  default:
    fprintf(pf, "TODO");
  }
  fprintf(pf, ">");
}

void OBJ_Print(OBJ *obj) { OBJ_FPrint(stdout, obj); }

/*******************************************************************************
 * Internal function
 ******************************************************************************/
