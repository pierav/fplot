/*
 * liste.c
 *
 *  Created on: 23/12/2020
 *      Author: pirx
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

#define DEFAULT_SIZE 10

/*******************************************************************************
 * Types
 ******************************************************************************/

struct List {
  size_t size;
  size_t occupation;
  void **dat;
};

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

static int update_size(List *l, size_t desired_size);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Public function
 ******************************************************************************/

List *LIST_init() {
  List *ret = malloc(sizeof(struct List));
  ret->size = DEFAULT_SIZE;
  ret->occupation = 0;
  ret->dat = malloc(sizeof(void *) * ret->size);
  return ret;
}

List *LIST_addEnd(List *l, void *dat) {
  update_size(l, l->occupation + 1);
  l->dat[l->occupation] = dat;
  l->occupation += 1;
  return l;
}

void *LIST_removeEnd(List *l) {
  assert(l->occupation);
  l->occupation -= 1;
  return l->dat[l->occupation];
}

void *LIST_get(List *l, size_t index) {
  assert(index < l->occupation);
  return l->dat[index];
}

size_t LIST_getSize(List *l) { return l->occupation; }

void LIST_free(List *l) {
  free(l->dat);
  free(l);
}

void TU_List(void) {
  List *l = LIST_init();
  const size_t B = 12 * DEFAULT_SIZE;

  // Ajout
  for (size_t k = 0; k < B; k++) {
    assert(LIST_getSize(l) == k);
    LIST_addEnd(l, (void *)k);
    assert(LIST_getSize(l) == k + 1);
  }

  // Get
  for (size_t i = 0; i < LIST_getSize(l); i++) {
    assert((size_t)LIST_get(l, i) == i);
  }

  // Remove
  for (size_t k = 0; k < B; k++)
    assert((size_t)LIST_removeEnd(l) == B - k - 1);

  // Free
  LIST_free(l);
  // printf("YEA\n");
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/

static int update_size(List *l, size_t desired_size) {
  if (l->size < (desired_size + desired_size / 2)) {
    l->size *= 2;
    l->dat = realloc(l->dat, l->size * sizeof(void *));
    return 1;
  }
  return 0;
}
