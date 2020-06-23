/*
 * mem_prgm.h
 *
 *  Created on: 23/06/2020
 *      Author: pirx
 */

#ifndef _mem_prgm_H_
#define _mem_prgm_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "fpcode.h"
#include <stdlib.h>

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

void MEMPRGM_Init(PCODE **prgm, size_t prgmsize);
PCODE *MEMPRGM_Get(size_t i);
void MEMPRGM_Print(void);

#endif /* _mem_prgm_H_ */
