/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "obj_func.h"
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
 * funcernal function declaration
 ******************************************************************************/

// Représentation textuelle
// Retourne un OBJ_STR
OBJ *OBJ_func__str__(OBJ *obj1);
OBJ *OBJ_func__repr__(OBJ *obj1);

/*******************************************************************************
 * Variables
 ******************************************************************************/

const void *OBJ_FUNC_FUNC[NB_OBJ_FUNC] = {
    /* ADD */ NULL,
    /* SUB */ NULL,
    /* MUL */ NULL,
    /* TDV */ NULL,
    /* POW */ NULL,
    /* FDV */ NULL,
    /* MOD */ NULL,
    /* NEG */ NULL,
    /* POS */ NULL,
    /* ABS */ NULL,
    /* EQ  */ NULL,
    /* NE  */ NULL,
    /* LT  */ NULL,
    /* GT  */ NULL,
    /* LE  */ NULL,
    /* GE  */ NULL,
    /* LEN */ NULL,
    /* GET */ NULL,
    /* STR  */ OBJ_func__str__,
    /* REPR  */ OBJ_func__repr__,
    /* int  */ NULL,
    /* DOUBLE  */ NULL};

/*******************************************************************************
 * Public function
 ******************************************************************************/

/*******************************************************************************
 * funcernal function
 ******************************************************************************/

// Représentation textuelle
// Retourne un OBJ_STR
OBJ *OBJ_func__str__(OBJ *obj1) {
  char *str = calloc(1, 20); // TODO
  sprintf(str, "fun@%ld", OBJ_func_getRaw(obj1));
  return OBJ_string_init(str, strlen(str));
}

OBJ *OBJ_func__repr__(OBJ *obj1) { return obj1; }
