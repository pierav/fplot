/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "obj.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

static OBJ *OBJ_Create(OBJ_TYPE type, void *value); // TODO : PRIVATE !

/*******************************************************************************
 * Variables
 ******************************************************************************/

const char *OBJ_FUNCS_NAMES[NB_OBJ_FUNC] = {
    "__ADD__", "__SUB__",  "__MUL__", "__TDV__",   "__POW__", "__FDV__",
    "__MOD__", "__NEG__",  "__POS__", "__ABS__",   "__EQ__",  "__NE__",
    "__LT__",  "__GT__",   "__LE__",  "__GE__",    "__LEN__", "__GETITEM__",
    "__STR__", "__REPR__", "__INT__", "__DOUBLE__"};

const uint8_t OBJ_NB_ARGS[NB_OBJ_FUNC] = {2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 2,
                                          2, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1};

/*******************************************************************************
 * Public function
 ******************************************************************************/

// Allocation
inline OBJ *OBJ_int_init(OBJ_int_t value) {
  OBJ_int_t *p_data = (OBJ_int_t *)malloc(sizeof(OBJ_int_t));
  *p_data = value;
  return OBJ_Create(OBJ_INT, p_data);
}

// Acces
inline OBJ_int_t OBJ_int_getRaw(OBJ *obj) {
  assert(obj);
  assert(obj->type == OBJ_INT);
  return *(OBJ_int_t *)obj->data;
}

// Allocation
inline OBJ *OBJ_string_init(char *s, OBJ_int_t size) {
  OBJ_string *value = malloc(sizeof(OBJ_string));
  value->s = strdup(s);
  value->size = size;
  return OBJ_Create(OBJ_STRING, value);
}

// Acces
inline OBJ_string *OBJ_string_getRaw(OBJ *obj) {
  assert(obj);
  assert(obj->type == OBJ_STRING);
  return (OBJ_string *)obj->data;
}

inline OBJ *OBJ_func_init(OBJ_func_t value) {
  OBJ_func_t *p_data = (OBJ_func_t *)malloc(sizeof(OBJ_func_t));
  *p_data = value;
  return OBJ_Create(OBJ_FUNC, p_data);
}

// Acces
inline OBJ_func_t OBJ_func_getRaw(OBJ *obj) {
  assert(obj);
  assert(obj->type == OBJ_FUNC);
  return (OBJ_func_t)obj->data;
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/

OBJ *OBJ_Create(OBJ_TYPE type, void *value) {
  // printf("[\e[33mOBJ\e[39m]>>> ");
  // printf("NEW (%s) ", name);
  OBJ *ret = malloc(sizeof(struct OBJ));
  ret->type = type;
  ret->data = value;
  ret->cpt_usage = 0;
  // OBJ_Print(ret);
  // printf("\n");
  return ret;
}
