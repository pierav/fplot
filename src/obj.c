/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "obj.h"
#include <assert.h>
#include <stdio.h>
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

const char *OBJ_TYPES_NAMES[] = {"BOOL", "INT", "CHAR", "STR", "FUNC"};

/*******************************************************************************
 * Public function
 ******************************************************************************/

OBJ *OBJ_Create(OBJ_TYPE type, void *value, char *name) {
  printf("NEW (%s) ", name);
  OBJ *ret = malloc(sizeof(struct OBJ));
  ret->type = type;
  ret->data = value;
  ret->name = name;
  ret->cpt_usage = 0;
  OBJ_Print(ret);
  printf("\n");
  return ret;
}

// obj =
OBJ *OBJ_Affect(OBJ *dst, OBJ *src) {

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
  printf("CALL %s(%s)>\n", func->name, "TODO");
  return func;
}

// obj + (__add__)
OBJ *OBJ_Add(OBJ *obj1, OBJ *obj2) {
  printf("ADD ");
  OBJ_Print(obj1);
  OBJ_Print(obj2);

  if (obj1->type != obj2->type) {
    printf("wrong type\n");
    exit(1);
  }
  OBJ *ret = calloc(1, sizeof(struct OBJ));
  ret->type = obj1->type;

  switch (ret->type) {
  case OBJ_INT:
    ret->data = malloc(sizeof(int));
    assert(ret->data);
    *(int *)ret->data = *(int *)obj1->data + *(int *)obj2->data;
    break;
  default:
    printf("No valid func\n");
    exit(1);
    break;
  }

  printf(" ==> ");
  OBJ_Print(ret);
  printf("\n");
  return ret;
}

// print(obj) __str__
void OBJ_Print(OBJ *obj) {
  if (obj == NULL) {
    printf("[#/!\\ NULL /!\\]");
    fflush(stdout);
    return;
    // exit(1);
  }
  printf("[%s#%s:", OBJ_TYPES_NAMES[obj->type], obj->name ? obj->name : "_");
  switch (obj->type) {
  case OBJ_INT:
    assert(obj->data);
    printf("%d", *(int *)obj->data);
    break;
  case OBJ_STR:
    printf("%s", (char *)obj->data);
    break;
  default:
    printf("TODO");
  }
  printf("]");
}

// Tests
OBJ *OBJ_INT_Create(int value, char *name) {
  int *p_data = (int *)malloc(sizeof(int));
  *p_data = value;
  return OBJ_Create(OBJ_INT, p_data, name);
}

void OBJ_Tu(void) {
  OBJ *x = OBJ_INT_Create(10, "x");
  OBJ_Print(x);
  OBJ *y = OBJ_INT_Create(100, "y");
  OBJ_Print(y);

  OBJ *xpy = OBJ_Add(x, y);
  OBJ_Print(xpy);
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/
