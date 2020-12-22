/*
 * obj_handler.h
 *
 *  Created on: 22/12/2020
 *      Author: pirx
 */

#ifndef _obj_handler_H_
#define _obj_handler_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "logbuffer.h"
#include "obj.h"
#include "obj_int.h"
#include "obj_string.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern FILE *stdout_po_alu;
extern const char *OBJ_TYPES_NAMES[];
extern const void *OBJ_FULL_FUNC[];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void PO_ALU_Init(void);

void OBJ_FPrint(FILE *pf, OBJ *obj);
void OBJ_Print(OBJ *obj);

OBJ *OBJ_ApplyFunc1(OBJ_PRIMITIVES func, OBJ *obj1);
OBJ *OBJ_ApplyFunc2(OBJ_PRIMITIVES func, OBJ *obj1, OBJ *obj2);

#endif /* _obj_handler_H_ */
