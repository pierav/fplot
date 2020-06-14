/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "obj.h"
#include "obj_int.h"
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
  printf("[\e[33mOBJ\e[39m]>>> ");
  printf("MV  ");
  OBJ_Print(dst);
  printf(" <- ");
  OBJ_Print(src);

  assert(dst);
  assert(src);

  dst->data = src->data;
  dst->type = src->type;

  printf(" ==> ");
  OBJ_Print(dst);
  printf("\n");
  return dst;
}

// obj()
// TODO
OBJ *OBJ_Call(OBJ *func, void *stack) {
  printf("[\e[33mOBJ\e[39m]>>> ");
  printf("CALL %s(%s)>\n", func->name, "TODO");
  return func;
}

OBJ *OBJ_ApplyFunc1(OBJ_PRIMITIVES func, OBJ *obj1) {
  printf("[\e[33mOBJ\e[39m]>>> ");
  printf(OBJ_FUNCS_NAMES[func]);
  OBJ_Print(obj1);

  // type valide ? TODO delete
  const void *objfunctab = OBJ_FULL_FUNC[obj1->type];
  if (objfunctab == NULL) {
    printf("Invalid type\n");
    return obj1;
  }

  // fonction valide ?
  const OBJ_func1 objfunc = ((OBJ_func1 *)objfunctab)[func];

  if (objfunc == NULL) {
    printf("Invalid func\n");
    return obj1;
  }

  // all OK
  OBJ *ret = objfunc(obj1);
  printf(" ==> ");
  OBJ_Print(ret);
  printf("\n");
  return ret;
}

OBJ *OBJ_ApplyFunc2(OBJ_PRIMITIVES func, OBJ *obj1, OBJ *obj2) {
  printf("[\e[33mOBJ\e[39m]>>> ");
  printf(OBJ_FUNCS_NAMES[func]);
  OBJ_Print(obj1);
  OBJ_Print(obj2);

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
  printf(" ==> ");
  OBJ_FPrint(stdout, ret);
  printf("\n");
  return ret;
}

// print(obj) __str__
void OBJ_FPrint(FILE *pf, OBJ *obj) {
  if (obj == NULL) {
    fprintf(pf, "(#/!\\ NULL /!\\)");
    fflush(pf);
    return;
    // exit(1);
  }
  fprintf(pf, "(%s#%s:", OBJ_TYPES_NAMES[obj->type],
          obj->name ? obj->name : "_");
  switch (obj->type) {
  case OBJ_INT:
    assert(obj->data);
    fprintf(pf, "%d", *(int *)obj->data);
    break;
  case OBJ_STR:
    fprintf(pf, "%s", (char *)obj->data);
    break;
  default:
    fprintf(pf, "TODO");
  }
  fprintf(pf, ")");
}

void OBJ_Print(OBJ *obj) { OBJ_FPrint(stdout, obj); }

/*******************************************************************************
 * Internal function
 ******************************************************************************/
