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
  HashTable /* objects */ *local; // TODO?
  void *data;
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
  __ADD__, // addition
  __SUB__, // soustraction
  __MUL__, // multiplication
  __TDV__, // division
  __POW__, // élévation à la puissance
  __FDV__, // division entière
  __MOD__, // modulo
  __NEG__, // opposé
  __POS__, // positif
  __ABS__, // valeur absolue

  // Opérateurs de comparaison
  __EQ__, // égal
  __NE__, // non égal
  __LT__, // strictement inférieur
  __GT__, // strictement supérieur
  __LE__, // inférieur ou égal
  __GE__, // supérieur ou égal

  // Opérateurs de conteneurs
  __LEN__,     // dimension
  __GETITEM__, // accès aux éléments

  // Méthodes d'affichage
  __STR__,  //  conversion en string pour print
  __REPR__, // affichage

  // Changement de type
  __INT__,    // -> toInt
  __DOUBLE__, // ->toDouble
} OBJ_PRIMITIVES;

typedef OBJ *(*OBJ_func0)(void);
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

OBJ *OBJ_Create(OBJ_TYPE type, void *value); // TODO : PRIVATE !

OBJ *OBJ_int_init(OBJ_int_t value);
OBJ_int_t OBJ_int_getRaw(OBJ *obj);

OBJ *OBJ_string_init(char *s, OBJ_int_t size);
OBJ_string *OBJ_string_getRaw(OBJ *obj);

OBJ *OBJ_func_init(OBJ_func_t value);
OBJ_func_t OBJ_func_getRaw(OBJ *obj);

OBJ *OBJ_class_init(char *name, void *data);
OBJ_class *OBJ_class_getRaw(OBJ *obj);

#endif /* _OBJ_H_ */
