/*

  Gestionnaire des objets
  TODO : Table de hachage !!!!

*/
/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "mem.h"
#include "po_ctxstack.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

OBJ *get(char *name);
OBJ *set(char *name, OBJ *obj);

/*******************************************************************************
 * Variables
 ******************************************************************************/

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
  return set(name, OBJ_Create(type, value));
}

// Retourne l'objet pointe par name. On le créer si inexistant
OBJ *MEM_GetOrCreateObj(char *name) {
  OBJ *ret = get(name);
  if (ret != NULL)
    return ret;
  return set(name, OBJ_Create(OBJ_STR, "NULL OBJ"));
}

/*
void MEM_Printz(void) {
  printf("***   MEMORY[SIZE = %ld]   ***\n", MEM_current_size);
  for (size_t i = 0; i < MEM_current_size; i++) {
    printf("[%.5ld] >> ", i);
    OBJ_Print(MEM_data[i]);
    printf("\n");
  }
}
*/

/*******************************************************************************
 * Internal function
 ******************************************************************************/

// TODO HASH TABLE
OBJ *get(char *name) { return CTX_get(name); }

OBJ *set(char *name, OBJ *obj) {
  assert(obj);
  CTX_set(name, obj);
  return obj;
}
