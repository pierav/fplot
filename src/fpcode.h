#ifndef _PC_FPCODE_H_
#define _PC_FPCODE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

enum FPC_TYPE {
  PUSH_SRC_VAR,    // char * | Push variable existante
  PUSH_DST_VAR,    // char * | Push variable existante ou indéfinie
  PUSH_CST,        // OBJ *  | Push constante ("Hello", 10, 1e3, ...)
  POP,             // void   | Pop
  APPLY_OBJ_FUNC,  // void   | Applique une OBJ__FUNC__ en tete de pile
  CALL,            // void   | Applique une fonction utilisateur
  AFFECT,          // void   | Affecte le premier element de pile sur le second
  JUMP,            // void
  CONDITIONAL_JUMP // void
};

typedef enum FPC_TYPE FPC_TYPE;

struct FPCODE {
  FPC_TYPE type;
  void *arg;
};

typedef struct FPCODE FPCODE;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void *FPC_RunFpcode(FPCODE *code);

void FPC_PrintStack(void);
void FPC_FPrint(FILE *pf, FPCODE *code);
void FPC_Print(FPCODE *code);

FPCODE *FPC_Create(FPC_TYPE type, void *arg);
#endif /* _PC_FPCODE_H_ */
