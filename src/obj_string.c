/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "obj.h"
#include <assert.h>
#include <math.h>
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
 * stringernal function declaration
 ******************************************************************************/

// Opérations arithmétiques
OBJ *OBJ_string__add__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_string__mul__(OBJ *obj1, OBJ *obj2);

// Comparaisons
// Retourne un OBJ_BOOL TODO !
OBJ *OBJ_string__eq__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_string__ne__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_string__lt__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_string__gt__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_string__le__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_string__ge__(OBJ *obj1, OBJ *obj2);

// Taille d'un element
// Retourne un OBJ_string
OBJ *OBJ_string__len__(OBJ *obj1);
OBJ *OBJ_string__getitem__(OBJ *obj1, OBJ *objarg);

// Représentation textuelle
// Retourne un OBJ_STR
OBJ *OBJ_string__str__(OBJ *obj1);
OBJ *OBJ_string__repr__(OBJ *obj1);
// Changement de type
OBJ *OBJ_string__int__(OBJ *obj);
OBJ *OBJ_string__double__(OBJ *obj);
OBJ *OBJ_string__bool__(OBJ *obj);

/*******************************************************************************
 * Variables
 ******************************************************************************/

const void *OBJ_STRING_FUNC[NB_OBJ_FUNC] = {
    /* ADD */ OBJ_string__add__,
    /* SUB */ NULL,
    /* MUL */ OBJ_string__mul__,
    /* TDV */ NULL,
    /* POW */ NULL,
    /* FDV */ NULL,
    /* MOD */ NULL,
    /* NEG */ NULL,
    /* POS */ NULL,
    /* ABS */ NULL,
    /* EQ  */ OBJ_string__eq__,
    /* NE  */ OBJ_string__ne__,
    /* LT  */ OBJ_string__lt__,
    /* GT  */ OBJ_string__gt__,
    /* LE  */ OBJ_string__le__,
    /* GE  */ OBJ_string__ge__,
    /* LEN */ OBJ_string__len__,
    /* GET */ OBJ_string__getitem__,
    /* STR  */ OBJ_string__str__,
    /* REPR  */ OBJ_string__repr__,
    /* INT  */ OBJ_string__int__,
    /* DOUBLE  */ OBJ_string__double__};

/*******************************************************************************
 * Public function
 ******************************************************************************/

/*******************************************************************************
 * stringernal function
 ******************************************************************************/

// Opérations arithmétiques
OBJ *OBJ_string__add__(OBJ *obj1, OBJ *obj2) {
  OBJ_string *s1 = OBJ_string_GetRaw(obj1);
  OBJ_string *s2 = OBJ_string_GetRaw(obj2);
  char *result = malloc(s1->size + s2->size + 1);
  strcpy(result, s1->s);
  strcat(result, s2->s);
  return OBJ_string_Init(result, s1->size + s2->size);
}

OBJ *OBJ_string__mul__(OBJ *obj1, OBJ *obj2) {
  return OBJ_string__add__(obj1, obj2); // TODO
}

// Comparaisons
// Retourne un OBJ_BOOL TODO !
OBJ *OBJ_string__eq__(OBJ *obj1, OBJ *obj2) {
  char *s1 = OBJ_string_GetRaw(obj1)->s;
  char *s2 = OBJ_string_GetRaw(obj2)->s;
  return OBJ_INT_Init(strcmp(s1, s2) == 0);
}
OBJ *OBJ_string__ne__(OBJ *obj1, OBJ *obj2) {
  char *s1 = OBJ_string_GetRaw(obj1)->s;
  char *s2 = OBJ_string_GetRaw(obj2)->s;
  return OBJ_INT_Init(strcmp(s1, s2) != 0);
}
OBJ *OBJ_string__lt__(OBJ *obj1, OBJ *obj2) {
  char *s1 = OBJ_string_GetRaw(obj1)->s;
  char *s2 = OBJ_string_GetRaw(obj2)->s;
  return OBJ_INT_Init(strcmp(s1, s2) < 0);
}
OBJ *OBJ_string__gt__(OBJ *obj1, OBJ *obj2) {
  char *s1 = OBJ_string_GetRaw(obj1)->s;
  char *s2 = OBJ_string_GetRaw(obj2)->s;
  return OBJ_INT_Init(strcmp(s1, s2) > 0);
}
OBJ *OBJ_string__le__(OBJ *obj1, OBJ *obj2) {
  char *s1 = OBJ_string_GetRaw(obj1)->s;
  char *s2 = OBJ_string_GetRaw(obj2)->s;
  return OBJ_INT_Init(strcmp(s1, s2) <= 0);
}
OBJ *OBJ_string__ge__(OBJ *obj1, OBJ *obj2) {
  char *s1 = OBJ_string_GetRaw(obj1)->s;
  char *s2 = OBJ_string_GetRaw(obj2)->s;
  return OBJ_INT_Init(strcmp(s1, s2) >= 0);
}

// Taille d'un element
// Retourne un OBJ_string
OBJ *OBJ_string__len__(OBJ *obj1) {
  return OBJ_INT_Init(OBJ_string_GetRaw(obj1)->size);
}
OBJ *OBJ_string__getitem__(OBJ *obj1, OBJ *objarg) {
  OBJ_int_t index = OBJ_INT_GetRaw(objarg);
  assert(index < OBJ_string_GetRaw(obj1)->size);
  return OBJ_INT_Init((OBJ_string_GetRaw(obj1)->s)[index]);
}

// Représentation textuelle
// Retourne un OBJ_STR
OBJ *OBJ_string__str__(OBJ *obj1) { return obj1; }
OBJ *OBJ_string__repr__(OBJ *obj1) { return obj1; }

// Changement de type
OBJ *OBJ_string__int__(OBJ *obj) {
  return OBJ_INT_Init(atoi(OBJ_string_GetRaw(obj)->s));
}
OBJ *OBJ_string__double__(OBJ *obj) {
  assert(0 && "TODO");
  return obj;
}
OBJ *OBJ_string__bool__(OBJ *obj) {
  assert(0 && "TODO");
  return obj;
}
