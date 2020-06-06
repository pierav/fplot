/*

  Gestionnaire des objets
  TODO : Table de hachage !!!!

*/
/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "mem.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

#define MEM_SIZE 255

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

OBJ *get(char *name);
OBJ *set(OBJ *obj);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static OBJ *MEM_data[MEM_SIZE];
static size_t MEM_current_size = 0;

extern OBJ *MEM_ans; // TODO

/*******************************************************************************
 * Public function
 ******************************************************************************/

// Retourne l'objet de nom name
OBJ *MEM_GetObj(char *name) {
  // TODO; hash
  OBJ *ret = get(name);
  if (ret == NULL) {
    printf("USR# undefined varable : %s", name);
    exit(1);
  }
  return ret;
}

// Crée un objet de nom name
OBJ *MEM_CreateObj(OBJ_TYPE type, void *value, char *name) {
  if (get(name) != NULL) {
    printf("USR# ever defined varable : %s", name);
    exit(1);
  }
  return set(OBJ_Create(type, value, name));
}

// Retourne l'objet pointe par name. On le créer si inexistant
OBJ *MEM_GetOrCreateObj(char *name) {
  OBJ *ret = get(name);
  if (ret != NULL)
    return ret;
  return set(OBJ_Create(OBJ_STR, "NULL OBJ", name));
}

void MEM_Clear(void) { MEM_current_size = 0; }

float MEM_GetUsage(void) { return (float)MEM_current_size / MEM_SIZE; }

void MEM_Print(void) {
  printf("***   MEMORY[SIZE = %ld]   ***\n", MEM_current_size);
  for (size_t i = 0; i < MEM_current_size; i++) {
    printf("[%.5ld] >> ", i);
    OBJ_Print(MEM_data[i]);
    printf("\n");
  }
}

void MEM_Tu(void) {
  MEM_Clear();
  printf("MEM clear DONE\n");
  OBJ *obj;
  obj = MEM_CreateObj(OBJ_STR, "hello", "x");
  printf("OBJ ADDR: %p\n", obj);
  OBJ_Print(obj);
  OBJ_Print(MEM_CreateObj(OBJ_STR, "world", "y"));
  OBJ_Print(MEM_GetObj("x"));
  OBJ_Print(MEM_GetObj("y"));
  MEM_Print();
  printf("\nUsage : %f\n", MEM_GetUsage());
  OBJ_Print(MEM_CreateObj(OBJ_STR, "xx", "xx"));
  OBJ_Print(MEM_CreateObj(OBJ_STR, "xyz", "xyz"));
  printf("MEM_Tu OK");
  MEM_Print();
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/

// TODO HASH TABLE
OBJ *get(char *name) {
  for (size_t i = 0; i < MEM_current_size; i++) {
    if (strcmp(name, MEM_data[i]->name) == 0) {
      return MEM_data[i];
    }
  }
  return NULL;
}

OBJ *set(OBJ *obj) {
  assert(obj);
  assert(MEM_current_size < MEM_SIZE);
  MEM_data[MEM_current_size++] = obj;
  return obj;
}
