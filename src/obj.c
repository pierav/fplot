/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <assert.h>
#include <obj.h>
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

/*******************************************************************************
 * Public function
 ******************************************************************************/

OBJ *OBJ_Create(OBJ_TYPE type, void *value, char *name) {
  OBJ *ret = malloc(sizeof(struct OBJ));
  ret->type = type;
  ret->data = value;
  ret->name = name;
  ret->cpt_usage = 0;
  return ret;
}

OBJ *OBJ_INT_Create(int value, char *name) {
  int *p_data = (int *)malloc(sizeof(int));
  *p_data = value;
  return OBJ_Create(OBJ_INT, p_data, name);
}

OBJ *OBJ_Add(OBJ *obj1, OBJ *obj2) {
  if (obj1->type != obj2->type) {
    perror("wrong type\n");
    exit(1);
  }
  OBJ *ret = OBJ_Create(obj1->type, NULL, NULL);
  switch (obj1->type) {
  case OBJ_INT:
    ret->data = malloc(sizeof(int));
    *(int *)ret->data = *(int *)obj1->data + *(int *)obj2->data;
    break;
  default:
    perror("No valid func\n");
    exit(1);
  }
  return ret;
}

void OBJ_Print(OBJ *obj) {
  printf("OBJ#%s:", obj->name ? obj->name : "NONAME");
  switch (obj->type) {
  case OBJ_INT:
    printf("%d", *(int *)obj->data);
    break;
  default:
    printf("No __str__ found");
  }
  printf("\n");
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/
