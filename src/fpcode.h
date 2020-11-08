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
  PC_TYPE_PUSH_SRC_VAR,     // char * | Push variable existante
  PC_TYPE_PUSH_DST_VAR,     // char * | Push variable existante ou indéfinie
  PC_TYPE_PUSH_CST,         // OBJ *  | Push constante ("Hello", 10, 1e3, ...)
  PC_TYPE_POP,              // void   | Pop
  PC_TYPE_APPLY_OBJ_FUNC,   // void   | Applique une OBJ__FUNC__ en tete de pile
  PC_TYPE_CALL,             // void   | Applique une fonction utilisateur
  PC_TYPE_AFFECT,           // void   | POP() <- POP()
  PC_TYPE_JUMP,             // INT    | Jump à l'adresse INT du programme
  PC_TYPE_CONDITIONAL_JUMP, // INT    | Jump si pop() == False
  PC_TYPE_RETURN,           // OBJ *  | PC <- POP(); PUSH(OBJ)
  PC_TYPE_AFFECT_ARG        // char * | NS[char*] <- POP()
};
typedef enum PC_TYPE PC_TYPE;

union PC_ARG {
  OBJ_PRIMITIVES ifunc;
  char *pchar_t;
  OBJ *pobj_t;
  size_t int_t;
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

PCODE *PC_CreateJump(size_t arg);
PCODE *PC_CreateJumpCond(size_t arg);
PCODE *PC_CreatePushSrc(char *name);
PCODE *PC_CreatePushDst(char *name);
PCODE *PC_CreatePushCst(OBJ *obj);
PCODE *PC_CreatePop(void);
PCODE *PC_CreateApply(size_t func);
PCODE *PC_CreateAffect(void);
PCODE *PC_CreateReturn(void);
PCODE *PC_CreateAffectArg(char *);

#endif /* _PC_FPCODE_H_ */
