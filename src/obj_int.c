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
 * Internal function declaration
 ******************************************************************************/

// Opérations arithmétiques
OBJ *OBJ_INT__add__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_INT__sub__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_INT__mul__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_INT__tdv__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_INT__pow__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_INT__mod__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_INT__fdv__(OBJ *obj1, OBJ *obj2);
// Opération arithmétiques unaire
OBJ *OBJ_INT__neg__(OBJ *obj1);
OBJ *OBJ_INT__pos__(OBJ *obj1);
OBJ *OBJ_INT__abs__(OBJ *obj1);
// Comparaisons Retourne un OBJ_BOOL
OBJ *OBJ_INT__eq__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_INT__ne__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_INT__lt__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_INT__gt__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_INT__le__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_INT__ge__(OBJ *obj1, OBJ *obj2);
// Taille d'un element Retourne un OBJ_INT
OBJ *OBJ_INT__len__(OBJ *obj1);
OBJ *OBJ_INT__getitem__(OBJ *obj1, OBJ *objarg);
// Représentation textuelle Retourne un OBJ_STR
OBJ *OBJ_INT__str__(OBJ *obj1);
OBJ *OBJ_INT__repr__(OBJ *obj1);
// Changement de type
OBJ *OBJ_INT__int__(OBJ *obj);
OBJ *OBJ_INT__double__(OBJ *obj);
OBJ *OBJ_INT__bool__(OBJ *obj);

/*******************************************************************************
 * Variables
 ******************************************************************************/

const void *OBJ_INT_FUNC[NB_OBJ_FUNC] = {
    OBJ_INT__add__, OBJ_INT__sub__,     OBJ_INT__mul__, OBJ_INT__tdv__,
    OBJ_INT__pow__, OBJ_INT__fdv__,     OBJ_INT__mod__, OBJ_INT__neg__,
    OBJ_INT__pos__, OBJ_INT__abs__,     OBJ_INT__eq__,  OBJ_INT__ne__,
    OBJ_INT__lt__,  OBJ_INT__gt__,      OBJ_INT__le__,  OBJ_INT__ge__,
    OBJ_INT__len__, OBJ_INT__getitem__, OBJ_INT__str__, OBJ_INT__repr__,
    OBJ_INT__int__, OBJ_INT__double__};

/*******************************************************************************
 * Public function
 ******************************************************************************/

/*******************************************************************************
 * Internal function
 ******************************************************************************/

// Opérations arithmétiques
OBJ *OBJ_INT__add__(OBJ *obj1, OBJ *obj2) {
  return OBJ_INT_Init(OBJ_INT_GetRaw(obj1) + OBJ_INT_GetRaw(obj2));
}

OBJ *OBJ_INT__sub__(OBJ *obj1, OBJ *obj2) {
  return OBJ_INT_Init(OBJ_INT_GetRaw(obj1) - OBJ_INT_GetRaw(obj2));
}

OBJ *OBJ_INT__mul__(OBJ *obj1, OBJ *obj2) {
  return OBJ_INT_Init(OBJ_INT_GetRaw(obj1) * OBJ_INT_GetRaw(obj2));
}

OBJ *OBJ_INT__tdv__(OBJ *obj1, OBJ *obj2) {
  return OBJ_INT_Init(OBJ_INT_GetRaw(obj1) / OBJ_INT_GetRaw(obj2));
}

OBJ *OBJ_INT__pow__(OBJ *obj1, OBJ *obj2) { // TODO
  return OBJ_INT_Init(OBJ_INT_GetRaw(obj1) * OBJ_INT_GetRaw(obj2));
}

OBJ *OBJ_INT__fdv__(OBJ *obj1, OBJ *obj2) {
  return OBJ_INT_Init(OBJ_INT_GetRaw(obj1) / OBJ_INT_GetRaw(obj2));
}

OBJ *OBJ_INT__mod__(OBJ *obj1, OBJ *obj2) {
  return OBJ_INT_Init(OBJ_INT_GetRaw(obj1) % OBJ_INT_GetRaw(obj2));
}

//////////////////////////////// TODO//////////////////////////////////////////
// Opération arithmétiques unaire
OBJ *OBJ_INT__neg__(OBJ *obj1) {
  assert(0);
  return OBJ_INT_Init(OBJ_INT_GetRaw(obj1) > 0);
}
OBJ *OBJ_INT__pos__(OBJ *obj1) {
  assert(0);
  return obj1;
}
OBJ *OBJ_INT__abs__(OBJ *obj1) {
  assert(0);
  return obj1;
}

// Comparaisons
// Retourne un OBJ_BOOL TODO !
OBJ *OBJ_INT__eq__(OBJ *obj1, OBJ *obj2) {
  return OBJ_INT_Init(OBJ_INT_GetRaw(obj1) == OBJ_INT_GetRaw(obj2));
}
OBJ *OBJ_INT__ne__(OBJ *obj1, OBJ *obj2) {
  return OBJ_INT_Init(OBJ_INT_GetRaw(obj1) != OBJ_INT_GetRaw(obj2));
}
OBJ *OBJ_INT__lt__(OBJ *obj1, OBJ *obj2) {
  return OBJ_INT_Init(OBJ_INT_GetRaw(obj1) < OBJ_INT_GetRaw(obj2));
}
OBJ *OBJ_INT__gt__(OBJ *obj1, OBJ *obj2) {
  return OBJ_INT_Init(OBJ_INT_GetRaw(obj1) > OBJ_INT_GetRaw(obj2));
}
OBJ *OBJ_INT__le__(OBJ *obj1, OBJ *obj2) {
  return OBJ_INT_Init(OBJ_INT_GetRaw(obj1) <= OBJ_INT_GetRaw(obj2));
}
OBJ *OBJ_INT__ge__(OBJ *obj1, OBJ *obj2) {
  return OBJ_INT_Init(OBJ_INT_GetRaw(obj1) >= OBJ_INT_GetRaw(obj2));
}

// Taille d'un element
// Retourne un OBJ_INT
OBJ *OBJ_INT__len__(OBJ *obj1) { return obj1; }
OBJ *OBJ_INT__getitem__(OBJ *obj1, OBJ *objarg) {
  OBJ_int_t value = (OBJ_INT_GetRaw(obj1) & (1 << OBJ_INT_GetRaw(objarg))) > 0;
  return OBJ_INT_Init(value);
}

// Représentation textuelle
// Retourne un OBJ_STR
OBJ *OBJ_INT__str__(OBJ *obj1) { return obj1; }
OBJ *OBJ_INT__repr__(OBJ *obj1) { return obj1; }

// Changement de type
OBJ *OBJ_INT__int__(OBJ *obj) { return obj; }
OBJ *OBJ_INT__double__(OBJ *obj) { return obj; }
OBJ *OBJ_INT__bool__(OBJ *obj) { return obj; }
