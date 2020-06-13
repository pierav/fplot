/*
 * prgm.h
 *
 *  Created on: 13/06/2020
 *      Author: pirx
 */

#ifndef _PRGM_H_
#define _PRGM_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "fpcode.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

// Init
void PRGM_InitAdd(FPCODE *fpc);
void PRGM_InitEnd(void);

// Run
FPCODE *PRGM_GetNext(void);

// End
void PRGM_Free(void);

// Tools
void PRGM_Print(void);

#endif /* _PRMG_H_ */