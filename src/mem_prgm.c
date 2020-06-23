/*
 * mem_prgm.c
 *
 *  Created on: 23/06/2020
 *      Author: pirx
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "mem_prgm.h"
#include <assert.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

size_t mem_size;
PCODE **memprgm = NULL;

/*******************************************************************************
 * Public function
 ******************************************************************************/

void MEMPRGM_Init(PCODE **prgm, size_t prgmsize) {
  memprgm = prgm;
  mem_size = prgmsize;
}

PCODE *MEMPRGM_Get(size_t i) {
  assert(i < mem_size);
  return memprgm[i];
}

void MEMPRGM_Print(void) {
  printf("***     MEM-PRGM(%ld)     ***\n", mem_size);
  for (size_t i = 0; i < mem_size; i++) {
    printf("[%.5ld] ", i);
    PC_Print(memprgm[i]);
    printf("\n");
  }
}
/*******************************************************************************
 * Internal function
 ******************************************************************************/
