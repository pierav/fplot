#ifndef _MEM_H_
#define _MEM_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "obj.h"
#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

#define OBJ_INT_NAME "Integer"

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern const void *OBJ_INT_FUNC[];

/*******************************************************************************
 * Prototypes
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

// Comparaisons
// Retourne un OBJ_BOOL
OBJ *OBJ_INT__eq__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_INT__ne__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_INT__lt__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_INT__gt__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_INT__le__(OBJ *obj1, OBJ *obj2);
OBJ *OBJ_INT__ge__(OBJ *obj1, OBJ *obj2);

// Taille d'un element
// Retourne un OBJ_INT
OBJ *OBJ_INT__len__(OBJ *obj1);
OBJ *OBJ_INT__getitem__(OBJ *obj1);

// Représentation textuelle
// Retourne un OBJ_STR
OBJ *OBJ_INT__str__(OBJ *obj1);
OBJ *OBJ_INT__repr__(OBJ *obj1);

// Changement de type
OBJ *OBJ_INT__int__(OBJ *obj);
OBJ *OBJ_INT__double__(OBJ *obj);
OBJ *OBJ_INT__bool__(OBJ *obj);

#endif /* _OBJ_H_ */
