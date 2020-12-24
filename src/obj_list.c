/*
 * obj_list.c
 *
 *  Created on: 23/12/2020
 *      Author: pirx
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "obj_list.h"
#include "assert.h"
#include "obj.h"
#include "utils/list.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

struct OBJ_list {
  List *dat;
};
typedef struct OBJ_list OBJ_list;

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/
OBJ *OBJ_list_init(List *l);
OBJ_list *OBJ_list_getRaw(OBJ *obj);

// Opérations arithmétiques
OBJ *OBJ_list__add__(OBJ *obj1, OBJ *obj2);

// Comparaisons
// Retourne un OBJ_BOOL TODO !
OBJ *OBJ_list__eq__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_list__ne__(OBJ *obj1, OBJ *obj2);

// Taille d'un element
// Retourne un OBJ_list
OBJ *OBJ_list__len__(OBJ *obj1);
OBJ *OBJ_list__getitem__(OBJ *obj1, OBJ *objarg);

// Représentation textuelle
// Retourne un OBJ_STR
OBJ *OBJ_list__str__(OBJ *obj1);
OBJ *OBJ_list__repr__(OBJ *obj1);

/*******************************************************************************
 * Variables
 ******************************************************************************/

const void *OBJCLASS_LIST_FUNC[NB_OBJ_FUNC] = {
    /* ADD */ OBJ_list__add__,
    /* SUB */ NULL,
    /* MUL */ NULL,
    /* TDV */ NULL,
    /* POW */ NULL,
    /* FDV */ NULL,
    /* MOD */ NULL,
    /* NEG */ NULL,
    /* POS */ NULL,
    /* ABS */ NULL,
    /* EQ  */ OBJ_list__eq__,
    /* NE  */ OBJ_list__ne__,
    /* LT  */ NULL,
    /* GT  */ NULL,
    /* LE  */ NULL,
    /* GE  */ NULL,
    /* LEN */ OBJ_list__len__,
    /* GET */ OBJ_list__getitem__,
    /* STR  */ OBJ_list__str__,
    /* REPR  */ OBJ_list__repr__,
    /* int  */ NULL,
    /* DOUBLE  */ NULL};

/*******************************************************************************
 * Public function
 ******************************************************************************/

OBJ *OBJ_list__init__(void) { return OBJ_list_init(LIST_init()); }

/*******************************************************************************
 * Internal function
 ******************************************************************************/

// Opérations arithmétiques
OBJ *OBJ_list__add__(OBJ *obj1, OBJ *obj2) {
  List *l1 = OBJ_list_getRaw(obj1)->dat;
  List *l2 = OBJ_list_getRaw(obj2)->dat;
  List *l = LIST_init();
  // TODO memcpy
  for (size_t i = 0; i < LIST_getSize(l1); i++)
    LIST_addEnd(l, LIST_get(l1, i));
  for (size_t i = 0; i < LIST_getSize(l2); i++)
    LIST_addEnd(l, LIST_get(l1, i));
  return OBJ_list_init(l);
}

// Comparaisons
// Retourne un OBJ_BOOL TODO !
OBJ *OBJ_list__eq__(OBJ *obj1, OBJ *obj2) { // On test les addrs du contenue
  List *l1 = OBJ_list_getRaw(obj1)->dat;
  List *l2 = OBJ_list_getRaw(obj2)->dat;
  if (LIST_getSize(l1) != LIST_getSize(l2))
    return OBJ_int_init(0);
  for (size_t i = 0; i < LIST_getSize(l1); i++) {
    if ((void *)LIST_get(l1, i) != (void *)LIST_get(l2, i))
      return OBJ_int_init(0);
  }
  return OBJ_int_init(1);
}
OBJ *OBJ_list__ne__(OBJ *obj1, OBJ *obj2) {
  // todo
  return OBJ_list__eq__(obj1, obj2);
}
// Taille d'un element
OBJ *OBJ_list__len__(OBJ *obj1) {
  return OBJ_int_init(LIST_getSize(OBJ_list_getRaw(obj1)->dat));
}

OBJ *OBJ_list__getitem__(OBJ *obj1, OBJ *objarg) {
  return LIST_get(OBJ_list_getRaw(obj1)->dat, OBJ_int_getRaw(objarg));
}

// Représentation textuelle
// Retourne un OBJ_STR
OBJ *OBJ_list__str__(OBJ *obj1) {
  assert(obj1);
  OBJ *ret = OBJ_string_init("[", 1);
  for (size_t i = 0; i < LIST_getSize(OBJ_list_getRaw(obj1)->dat); i++) {
    OBJ *oi = LIST_get(OBJ_list_getRaw(obj1)->dat, i);
    ret = OBJ_ApplyFunc2(__ADD__, ret, OBJ_ApplyFunc1(__STR__, oi));
  }
  return OBJ_ApplyFunc2(__ADD__, ret, OBJ_string_init("]", 1));
}

OBJ *OBJ_list__repr__(OBJ *obj1) { return OBJ_list__str__(obj1); }

OBJ *OBJ_list_init(List *l) {
  OBJ_list *ret = malloc(sizeof(struct OBJ_list));
  ret->dat = l;
  return OBJ_class_init(OBJCLASS_LIST_NAME, ret);
}

OBJ_list *OBJ_list_getRaw(OBJ *obj) {
  OBJ_class *c = OBJ_class_getRaw(obj);
  assert((void *)c->name == (void *)OBJCLASS_LIST_NAME); // TODO DELETE !
  assert(c->data);
  return (OBJ_list *)(c->data);
};
