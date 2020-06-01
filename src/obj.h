#ifndef _OBJ_H_
#define _OBJ_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

#define NB_OBJ_TYPE 5
typedef enum {
  OBJ_BOOL = 0,
  OBJ_INT = 1,
  OBJ_CHAR = 2,
  OBJ_STR = 3,
  OBJ_FUNC = 4
} OBJ_TYPE;

#define NB_OBJ_FUNC 22
typedef enum {
  __ADD__ = 0,
  __SUB__ = 1,
  __MUL__ = 2,
  __TDV__ = 3,
  __POW__ = 4,
  __FDV__ = 5,
  __MOD__ = 6,
  __NEG__ = 7,
  __POS__ = 8,
  __ABS__ = 9,
  __EQ__ = 10,
  __NE__ = 11,
  __LT__ = 12,
  __GT__ = 13,
  __LE__ = 14,
  __GE__ = 15,
  __LEN__ = 16,
  __GETITEM__ = 17,
  __STR__ = 18,
  __REPR__ = 19,
  __INT__ = 20,
  __DOUBLE__ = 21
} OBJ_PRIMITIVES;

typedef struct OBJ {
  OBJ_TYPE type;
  void *data;
  char *name;
  uint32_t cpt_usage;
} OBJ;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

OBJ *OBJ_Create(OBJ_TYPE type, void *value, char *name);

OBJ *OBJ_Affect(OBJ *dst, OBJ *src);
OBJ *OBJ_Call(OBJ *func, void *stack);

void OBJ_Print(OBJ *obj);

OBJ *OBJ_ApplyFunc1(OBJ_PRIMITIVES func, OBJ *obj1);
OBJ *OBJ_ApplyFunc2(OBJ_PRIMITIVES func, OBJ *obj1, OBJ *obj2);

#endif /* _OBJ_H_ */
