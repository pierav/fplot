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

void *PO_ALU_getFuncs(OBJ *obj);

/*******************************************************************************
 * Variables
 ******************************************************************************/

FILE *stdout_po_alu = NULL;

// Types primitifs
const char *OBJ_TYPES_NAMES[] = {"BOOL",          OBJ_INT_NAME,  "CHAR",
                                 OBJ_STRING_NAME, OBJ_FUNC_NAME, "Class"};

const void *OBJ_FULL_FUNC[] = {
    NULL, OBJ_INT_FUNC, NULL, OBJ_STRING_FUNC, OBJ_FUNC_FUNC, NULL};

// Classes
// TODO group
HashTable *objclass_full_func;
HashTable *objclass_init_func;

/*******************************************************************************
 * Public function
 ******************************************************************************/

void PO_ALU_Init(void) {
  stdout_po_alu = LB_Init(1024);
  // Initialisation de la table des classes (les types primitifs sont statiques)
  objclass_full_func = HT_Init();
  objclass_init_func = HT_Init();
  PO_ALU_setNewClass(OBJCLASS_LIST_NAME, (void **)OBJCLASS_LIST_FUNC,
                     (void *)OBJ_list__init__);
}

void PO_ALU_setNewClass(char *classname, void *funcs[], void *initfunc) {
  HT_Insert(objclass_full_func, classname, funcs);
  HT_Insert(objclass_init_func, classname, initfunc);
}

OBJ *PO_ALU_createObjclass(char *classname) {
  return ((OBJ_func0)HT_Get(objclass_init_func, classname))();
}

void PO_ALU_fprintObj(FILE *pf, OBJ *obj) {
  if (obj == NULL) { //
    fprintf(pf, "(NULL)");
    return;
  }
  fprintf(pf, "(%s:", OBJ_TYPES_NAMES[obj->type]);
  OBJ_string *os = (OBJ_string *)OBJ_ApplyFunc1(__STR__, obj)->data;
  fprintf(pf, os->s);
  fprintf(pf, ")");
}
/*******************************************************************************
 * Internal function
 ******************************************************************************/

void *PO_ALU_getFuncs(OBJ *obj) {
  assert(obj);
  // Type dynamique
  if (obj->type == OBJ_CLASS)
    return HT_Get(objclass_full_func, ((OBJ_class *)obj->data)->name);
  // Type statique
  return (void *)OBJ_FULL_FUNC[obj->type];
}

OBJ *OBJ_ApplyFunc1(OBJ_PRIMITIVES func, OBJ *obj1) {
  fprintf(stdout_po_alu, "[\e[33mOBJ\e[39m]>>> ");
  fprintf(stdout_po_alu, OBJ_FUNCS_NAMES[func]);
  OBJ_FPrint(stdout_po_alu, obj1);

  // type valide ? TODO delete
  const void *objfunctab = PO_ALU_getFuncs(obj1);
  if (objfunctab == NULL) {
    printf("Invalid type\n");
    assert(0);
  }

  // fonction valide ?
  const OBJ_func1 objfunc = ((OBJ_func1 *)objfunctab)[func];

  if (objfunc == NULL) {
    printf("Invalid func : %s.%s \n", OBJ_TYPES_NAMES[obj1->type],
           OBJ_FUNCS_NAMES[func]);
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
  const void *objfunctab = PO_ALU_getFuncs(obj1);
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
  case OBJ_CLASS:
    fprintf(pf, "%s", ((OBJ_class *)obj->data)->name);
    break;
  default:
    fprintf(pf, "TODO");
  }
  fprintf(pf, ")");
}

void OBJ_Print(OBJ *obj) { OBJ_FPrint(stdout, obj); }
