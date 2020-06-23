#ifndef _PC_FPCODE_H_
#define _PC_FPCODE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "obj.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

enum PC_TYPE {
  PUSH_SRC_VAR,    // char * | Push variable existante
  PUSH_DST_VAR,    // char * | Push variable existante ou indéfinie
  PUSH_CST,        // OBJ *  | Push constante ("Hello", 10, 1e3, ...)
  POP,             // void   | Pop
  APPLY_OBJ_FUNC,  // void   | Applique une OBJ__FUNC__ en tete de pile
  CALL,            // void   | Applique une fonction utilisateur
  AFFECT,          // void   | Affecte le premier element de pile sur le second
  JUMP,            // INT    | Jump à l'adresse INT du programme
  CONDITIONAL_JUMP // void   | Jump si pop() == False
};
typedef enum PC_TYPE PC_TYPE;

union PC_ARG {
  char *pchar_t;
  OBJ *pobj_t;
  int int_t;
};
typedef union PC_ARG PC_ARG;

struct PCODE {
  PC_TYPE type;
  PC_ARG arg;
};

typedef struct PCODE PCODE;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void PC_FPrint(FILE *pf, PCODE *code);
void PC_Print(PCODE *code);
const char *PC_GetName(PCODE *code);
PCODE *PC_Create(PC_TYPE type, PC_ARG arg);

#endif /* _PC_FPCODE_H_ */
