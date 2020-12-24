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
 * internal function declaration
 ******************************************************************************/

// Opérations arithmétiques
OBJ *OBJ_int__add__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_int__sub__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_int__mul__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_int__tdv__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_int__pow__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_int__mod__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_int__fdv__(OBJ *obj1, OBJ *obj2);
// Opération arithmétiques unaire
OBJ *OBJ_int__neg__(OBJ *obj1);
OBJ *OBJ_int__pos__(OBJ *obj1);
OBJ *OBJ_int__abs__(OBJ *obj1);
// Comparaisons Retourne un OBJ_BOOL
OBJ *OBJ_int__eq__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_int__ne__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_int__lt__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_int__gt__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_int__le__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_int__ge__(OBJ *obj1, OBJ *obj2);
// Taille d'un element Retourne un OBJ_int
OBJ *OBJ_int__len__(OBJ *obj1);
OBJ *OBJ_int__getitem__(OBJ *obj1, OBJ *objarg);
// Représentation textuelle Retourne un OBJ_STR
OBJ *OBJ_int__str__(OBJ *obj1);
OBJ *OBJ_int__repr__(OBJ *obj1);
// Changement de type
OBJ *OBJ_int__int__(OBJ *obj);
OBJ *OBJ_int__double__(OBJ *obj);
OBJ *OBJ_int__bool__(OBJ *obj);

/*******************************************************************************
 * Variables
 ******************************************************************************/

const void *OBJ_INT_FUNC[NB_OBJ_FUNC] = {
    OBJ_int__add__, OBJ_int__sub__,     OBJ_int__mul__, OBJ_int__tdv__,
    OBJ_int__pow__, OBJ_int__fdv__,     OBJ_int__mod__, OBJ_int__neg__,
    OBJ_int__pos__, OBJ_int__abs__,     OBJ_int__eq__,  OBJ_int__ne__,
    OBJ_int__lt__,  OBJ_int__gt__,      OBJ_int__le__,  OBJ_int__ge__,
    OBJ_int__len__, OBJ_int__getitem__, OBJ_int__str__, OBJ_int__repr__,
    OBJ_int__int__, OBJ_int__double__};

/*******************************************************************************
 * Public function
 ******************************************************************************/

/*******************************************************************************
 * internal function
 ******************************************************************************/

// Opérations arithmétiques
OBJ *OBJ_int__add__(OBJ *obj1, OBJ *obj2) {
  return OBJ_int_init(OBJ_int_getRaw(obj1) + OBJ_int_getRaw(obj2));
}

OBJ *OBJ_int__sub__(OBJ *obj1, OBJ *obj2) {
  return OBJ_int_init(OBJ_int_getRaw(obj1) - OBJ_int_getRaw(obj2));
}

OBJ *OBJ_int__mul__(OBJ *obj1, OBJ *obj2) {
  return OBJ_int_init(OBJ_int_getRaw(obj1) * OBJ_int_getRaw(obj2));
}

OBJ *OBJ_int__tdv__(OBJ *obj1, OBJ *obj2) {
  return OBJ_int_init(OBJ_int_getRaw(obj1) / OBJ_int_getRaw(obj2));
}

OBJ *OBJ_int__pow__(OBJ *obj1, OBJ *obj2) { // TODO
  return OBJ_int_init(OBJ_int_getRaw(obj1) * OBJ_int_getRaw(obj2));
}

OBJ *OBJ_int__fdv__(OBJ *obj1, OBJ *obj2) {
  return OBJ_int_init(OBJ_int_getRaw(obj1) / OBJ_int_getRaw(obj2));
}

OBJ *OBJ_int__mod__(OBJ *obj1, OBJ *obj2) {
  return OBJ_int_init(OBJ_int_getRaw(obj1) % OBJ_int_getRaw(obj2));
}

//////////////////////////////// TODO//////////////////////////////////////////
// Opération arithmétiques unaire
OBJ *OBJ_int__neg__(OBJ *obj1) {
  assert(0);
  return OBJ_int_init(OBJ_int_getRaw(obj1) > 0);
}
OBJ *OBJ_int__pos__(OBJ *obj1) {
  assert(0);
  return obj1;
}
OBJ *OBJ_int__abs__(OBJ *obj1) {
  assert(0);
  return obj1;
}

// Comparaisons
// Retourne un OBJ_BOOL TODO !
OBJ *OBJ_int__eq__(OBJ *obj1, OBJ *obj2) {
  return OBJ_int_init(OBJ_int_getRaw(obj1) == OBJ_int_getRaw(obj2));
}
OBJ *OBJ_int__ne__(OBJ *obj1, OBJ *obj2) {
  return OBJ_int_init(OBJ_int_getRaw(obj1) != OBJ_int_getRaw(obj2));
}
OBJ *OBJ_int__lt__(OBJ *obj1, OBJ *obj2) {
  return OBJ_int_init(OBJ_int_getRaw(obj1) < OBJ_int_getRaw(obj2));
}
OBJ *OBJ_int__gt__(OBJ *obj1, OBJ *obj2) {
  return OBJ_int_init(OBJ_int_getRaw(obj1) > OBJ_int_getRaw(obj2));
}
OBJ *OBJ_int__le__(OBJ *obj1, OBJ *obj2) {
  return OBJ_int_init(OBJ_int_getRaw(obj1) <= OBJ_int_getRaw(obj2));
}
OBJ *OBJ_int__ge__(OBJ *obj1, OBJ *obj2) {
  return OBJ_int_init(OBJ_int_getRaw(obj1) >= OBJ_int_getRaw(obj2));
}

// Taille d'un element
// Retourne un OBJ_int
OBJ *OBJ_int__len__(OBJ *obj1) { return obj1; }
OBJ *OBJ_int__getitem__(OBJ *obj1, OBJ *objarg) {
  OBJ_int_t value = (OBJ_int_getRaw(obj1) & (1 << OBJ_int_getRaw(objarg))) > 0;
  return OBJ_int_init(value);
}

// Représentation textuelle
// Retourne un OBJ_STR
OBJ *OBJ_int__str__(OBJ *obj1) {
  char *str = calloc(1, 20);
  sprintf(str, "%d", OBJ_int_getRaw(obj1));
  return OBJ_string_init(str, strlen(str));
}
OBJ *OBJ_int__repr__(OBJ *obj1) { return OBJ_int__str__(obj1); }

// Changement de type
OBJ *OBJ_int__int__(OBJ *obj) { return obj; }
OBJ *OBJ_int__double__(OBJ *obj) { return obj; }
OBJ *OBJ_int__bool__(OBJ *obj) { return obj; }
