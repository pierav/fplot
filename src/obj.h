#ifndef _OBJ_H_
#define _OBJ_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/hashtable.h"

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

typedef struct OBJ_DATA_FUNC {
  HashTable *namespace;

} OBJ_DATA_FUNC;

typedef union OBJ_DATA {
  bool *pbool;
  int *pint;
  char *pchar;
  char *pstr;
  OBJ_DATA_FUNC *func;
} OBJ_DATA;

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

extern const char *OBJ_FUNCS_NAMES[NB_OBJ_FUNC];
extern const uint8_t OBJ_NB_ARGS[NB_OBJ_FUNC];
extern const char *OBJ_TYPES_NAMES[];
extern const void *OBJ_FULL_FUNC[];

extern FILE *stdout_po_alu;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void PO_ALU_Init(void);

OBJ *OBJ_Create(OBJ_TYPE type, void *value, char *name);

OBJ *OBJ_Affect(OBJ *dst, OBJ *src);
OBJ *OBJ_Call(OBJ *func);

void OBJ_FPrint(FILE *pf, OBJ *obj);
void OBJ_Print(OBJ *obj);

OBJ *OBJ_ApplyFunc1(OBJ_PRIMITIVES func, OBJ *obj1);
OBJ *OBJ_ApplyFunc2(OBJ_PRIMITIVES func, OBJ *obj1, OBJ *obj2);

#endif /* _OBJ_H_ */
