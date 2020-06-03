#ifndef _OBJ_H_
#define _OBJ_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

#define NB_OBJ_TYPE 5
typedef enum {
  OBJ_BOOL = 0,
  OBJ_INT = 1,
  OBJ_CHAR = 2,
  OBJ_STR = 3,
  OBJ_FUNC = 4
} OBJ_TYPE;

#define NB_OBJ_FUNC (22)
typedef enum {
  // Opérations mathématiques
  __ADD__ = 0, // addition
  __SUB__ = 1, // soustraction
  __MUL__ = 2, // multiplication
  __TDV__ = 3, // division
  __POW__ = 4, // élévation à la puissance
  __FDV__ = 5, // division entière
  __MOD__ = 6, // modulo
  __NEG__ = 7, // opposé
  __POS__ = 8, // positif
  __ABS__ = 9, // valeur absolue

  // Opérateurs de comparaison
  __EQ__ = 10, // égal
  __NE__ = 11, // non égal
  __LT__ = 12, // strictement inférieur
  __GT__ = 13, // strictement supérieur
  __LE__ = 14, // inférieur ou égal
  __GE__ = 15, // supérieur ou égal

  // Opérateurs de conteneurs
  __LEN__ = 16,     // dimension
  __GETITEM__ = 17, // accès aux éléments

  // Méthodes d'affichage
  __STR__ = 18,  //  conversion en string pour print
  __REPR__ = 19, // affichage

  // Changement de type
  __INT__ = 20,    // -> toInt
  __DOUBLE__ = 21, // ->toDouble
} OBJ_PRIMITIVES;

typedef struct OBJ {
  OBJ_TYPE type;
  void *data;
  char *name;
  uint32_t cpt_usage;
} OBJ;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

OBJ *OBJ_Create(OBJ_TYPE type, void *value, char *name);

OBJ *OBJ_Affect(OBJ *dst, OBJ *src);
OBJ *OBJ_Call(OBJ *func, void *stack);

void OBJ_Print(OBJ *obj);

OBJ *OBJ_ApplyFunc1(OBJ_PRIMITIVES func, OBJ *obj1);
OBJ *OBJ_ApplyFunc2(OBJ_PRIMITIVES func, OBJ *obj1, OBJ *obj2);

#endif /* _OBJ_H_ */
