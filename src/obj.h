#ifndef _OBJ_H_
#define _OBJ_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "utils/hashtable.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

// Types primitif
typedef int OBJ_int_t;
typedef char OBJ_char_t;

struct OBJ_string {
  char *s;
  OBJ_int_t size;
};
typedef struct OBJ_string OBJ_string;

struct OBJ_class { // Pour les types non primitif
  char *name;
  HashTable /* objects */ *local;
};
typedef struct OBJ_class OBJ_class;

typedef size_t OBJ_func_t;

// OBJ
#define NB_OBJ_TYPE 5
typedef enum {
  OBJ_BOOL,
  OBJ_INT,
  OBJ_CHAR,
  OBJ_STRING,
  OBJ_FUNC,
  OBJ_CLASS
} OBJ_TYPE;

typedef union OBJ_DATA {
  OBJ_int_t *intt;
  OBJ_char_t *chart;
  OBJ_string *stringt;
  OBJ_func_t *funct;
  OBJ_class *classt;
} OBJ_DATA;

typedef struct OBJ {
  OBJ_TYPE type;
  OBJ_DATA *data;

  uint32_t cpt_usage;
} OBJ;

// Primitives
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

typedef OBJ *(*OBJ_func1)(OBJ *);
typedef OBJ *(*OBJ_func2)(OBJ *, OBJ *);

// NULL ou OBJ {OBJ_TYPE_NULL, NULL, NULL, 0}
#define OBJ_NULL NULL

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern const char *OBJ_FUNCS_NAMES[NB_OBJ_FUNC];
extern const uint8_t OBJ_NB_ARGS[NB_OBJ_FUNC];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

OBJ *OBJ_int_init(OBJ_int_t value);
OBJ_int_t OBJ_int_getRaw(OBJ *obj);

OBJ *OBJ_string_init(char *s, OBJ_int_t size);
OBJ_string *OBJ_string_getRaw(OBJ *obj);

OBJ *OBJ_func_init(OBJ_func_t value);
OBJ_func_t OBJ_func_getRaw(OBJ *obj);

#endif /* _OBJ_H_ */
