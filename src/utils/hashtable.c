/*
 * hashtable.c
 *
 *  Created on: 26/06/2020
 *      Author: pirx
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "hashtable.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

#define DEFAULT_SIZE 8

/*******************************************************************************
 * Types
 ******************************************************************************/

typedef uint64_t hash_t;

struct Cell {
  hash_t key;
  void *dat;
  struct Cell *next;
};
typedef struct Cell Cell;

struct HashTable {
  size_t len;
  struct Cell **tab;
};

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

hash_t compute_hash(char *str);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Public function
 ******************************************************************************/

HashTable *HT_Init(void) {
  HashTable *ret = malloc(sizeof(struct HashTable));
  ret->tab = malloc(sizeof(struct Cell *) * DEFAULT_SIZE);
  ret->len = DEFAULT_SIZE;
  for (size_t i = 0; i < ret->len; i++) {
    Cell *sentinel = malloc(sizeof(struct Cell));
    sentinel->next = NULL;
    ret->tab[i] = sentinel;
  }
  return ret;
}

void HT_Insert(HashTable *ht, char *str, void *dat) {
  hash_t key = compute_hash(str);
  // Ajout en tete
  Cell *cell = malloc(sizeof(struct Cell *));
  cell->next = ht->tab[key % ht->len];
  cell->key = key;
  cell->dat = dat;

  ht->tab[key % ht->len] = cell;
}

// get ht[str]. NULL if none
void *HT_Get(HashTable *ht, char *str) {
  hash_t key = compute_hash(str);
  for (Cell *cur = ht->tab[key % ht->len]; cur->next != NULL; cur = cur->next)
    if (cur->key == key)
      return cur->dat;
  return NULL;
}

void HT_FPrint(FILE *pf, HashTable *ht) {
  printf("***     HashTable     ***\n");
  for (size_t i = 0; i < ht->len; i++) {
    printf("---\n");
    for (Cell *cur = ht->tab[i]; cur->next != NULL; cur = cur->next)
      fprintf(pf, "K_%lu -> mem[%p]\n", cur->key, cur->dat);
  }
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/

hash_t compute_hash(char *str) {
  static const int p = 53; // [a-zA-Z_] #53 // Prime number
  hash_t hash_value = 0;
  hash_t p_pow = 1;
  for (char *c = str; *c != '\0'; c++) {
    hash_value += *c * p_pow; // +1 bc aaaa => 0
    p_pow = p_pow * p;
  }
  return hash_value;
}

// #[a-z] + #[A-Z] + #{_} + #[0-9]
// 26 + 26 + 1 + 10 = 63
/*
uint8_t encode(char c) { // MAP ASCII => 1...53
  if (islower(c))
    return c - 'a' + 1;
  if (isupper(c))
    return c - 'A' + 1 + 26;
  if (c == '_')
    return 53;
  assert(0); // ERR
}
*/
