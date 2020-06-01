#ifndef _MEM_H_
#define _MEM_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "obj.h"
#include <stdbool.h>
#include <stdint.h>

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

// Retourne l'objet de nom name. NULL si inexistant
OBJ *MEM_GetObj(char *name);

// Crée un objet de nom name. NULL si déja existant
OBJ *MEM_CreateObj(OBJ_TYPE type, void *value, char *name);

// Retourne l'objet pointe par name. On le créer si inexistant
OBJ *MEM_GetOrCreateObj(OBJ_TYPE type, void *value, char *name);

void MEM_Clear(void);
float MEM_GetUsage(void);

void MEM_Tu(void);
void MEM_Print(void);

#endif /* _OBJ_H_ */
