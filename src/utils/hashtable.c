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
#include <string.h>

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
  char *name;
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

HashTable *HT_Insert(HashTable *ht, char *str, void *dat) {
  hash_t key = compute_hash(str);
  // Ajout en tete
  Cell *cell = malloc(sizeof(struct Cell));
  assert(cell);
  cell->next = ht->tab[key % ht->len];
  cell->key = key;
  cell->dat = dat;
  cell->name = strdup(str);

  ht->tab[key % ht->len] = cell;
  return ht;
}

// get ht[str]. NULL if none
void *HT_Get(HashTable *ht, char *str) {
  hash_t key = compute_hash(str);
  for (Cell *cur = ht->tab[key % ht->len]; cur->next != NULL; cur = cur->next)
    if (cur->key == key)
      return cur->dat;
  return NULL;
}

void HT_FPrintRaw(FILE *pf, HashTable *ht) {
  printf("***     HashTable     ***\n");
  for (size_t i = 0; i < ht->len; i++) {
    printf("---\n");
    for (Cell *cur = ht->tab[i]; cur->next != NULL; cur = cur->next)
      fprintf(pf, "%s / K_%lu -> mem[%p]\n", cur->name, cur->key, cur->dat);
  }
}

void HT_FPrintKeys(FILE *pf, HashTable *ht, char *delimitor) {
  for (size_t i = 0; i < ht->len; i++) {
    for (Cell *cur = ht->tab[i]; cur->next != NULL; cur = cur->next)
      fprintf(pf, "%s%s", cur->name, delimitor);
  }
}

void TU_HashTable(void) {

  HashTable *ht = HT_Init();

  HT_Insert(ht, "test1", (void *)1);
  HT_Insert(ht, "test2", (void *)2);
  HT_Insert(ht, "test3", (void *)3);
  HT_Insert(ht, "test4", (void *)4);
  HT_Insert(ht, "test5", (void *)5);
  HT_Insert(ht, "test6", (void *)6);
  HT_Insert(ht, "test7", (void *)7);
  HT_Insert(ht, "test8", (void *)8);
  HT_Insert(ht, "test9", (void *)9);
  HT_Insert(ht, "test10", (void *)10);
  HT_Insert(ht, "test11", (void *)11);
  HT_Insert(ht, "azerty", (void *)12345);
  HT_Insert(ht, "123456", (void *)99999);

  assert(HT_Get(ht, "test1") == (void *)1);
  assert(HT_Get(ht, "test2") == (void *)2);
  assert(HT_Get(ht, "test3") == (void *)3);
  assert(HT_Get(ht, "test4") == (void *)4);
  assert(HT_Get(ht, "test5") == (void *)5);
  assert(HT_Get(ht, "test6") == (void *)6);
  assert(HT_Get(ht, "test7") == (void *)7);
  assert(HT_Get(ht, "test8") == (void *)8);
  assert(HT_Get(ht, "test9") == (void *)9);
  assert(HT_Get(ht, "test10") == (void *)10);
  assert(HT_Get(ht, "test11") == (void *)11);
  assert(HT_Get(ht, "azerty") == (void *)12345);
  assert(HT_Get(ht, "123456") == (void *)99999);

  HT_FPrintRaw(stdout, ht);
  HT_FPrintKeys(stdout, ht, "\n");
  HT_FPrintKeys(stdout, ht, " ");
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
