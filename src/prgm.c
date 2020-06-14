/*
 * prgm.c
 *
 *  Created on: 13/06/2020
 *      Author: pirx
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "prgm.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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

// Head & queu of PRGM
static PRGM_NODE *head = NULL;
static PRGM_NODE *queu = NULL;

// Current node of execution
static PRGM_NODE *current = NULL;

/*******************************************************************************
 * Public function
 ******************************************************************************/

// Init
PRGM_NODE *PRGM_InitAdd(FPCODE *fpc) {
  if (head == NULL) {
    head = queu = malloc(sizeof(struct PRGM_NODE));
    queu->index = 0;
    assert(queu);
  } else {
    queu->next_if_true = malloc(sizeof(struct PRGM_NODE));
    queu->next_if_true->index = queu->index + 1;
    assert(queu);
    queu = queu->next_if_true;
  }
  queu->code = fpc;
  return queu;
}

void PRGM_InitEnd(void) {
  queu->next_if_true = NULL;
  current = head;
}

// Run
FPCODE *PRGM_GetNext(void) {
  if (current) {
    FPCODE *code = current->code;
    current = current->next_if_true;
    return code;
  }
  return NULL;
}

// End
void PRGM_Free(void) {
  // TODO
}

// Tools
void PRGM_Print(void) {
  for (PRGM_NODE *cur = head; cur != NULL; cur = cur->next_if_true) {
    printf("NODE[%d]: ", cur->index);
    FPC_Print(cur->code);
    printf("\n");
  }
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/
