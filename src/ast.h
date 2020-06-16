/*
 * ast.h
 *
 *  Created on: 14/06/2020
 *      Author: pirx
 */

#ifndef _AST_H_
#define _AST_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "fpcode.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

enum AST_NODE_TYPE {
  AST_NODE_TYPE_PCODE,
  AST_NODE_TYPE_IF,
  AST_NODE_TYPE_WHILE,
  AST_NODE_TYPE_STAT
};
typedef enum AST_NODE_TYPE AST_NODE_TYPE;
extern const char *AST_NODE_TYPE_STR[];

// Abstract struct for all type;
typedef void AST_NODE;

struct AST_NODE_PCODE {
  // For all AST_NODE
  enum AST_NODE_TYPE type; // first pos
  // For PCODE
  FPCODE *code;
  AST_NODE *arg1;
  AST_NODE *arg2;
};
typedef struct AST_NODE_PCODE AST_NODE_PCODE;

struct AST_NODE_IF {
  // For all AST_NODE
  enum AST_NODE_TYPE type; // first pos
  // For IF
  AST_NODE *test;
  AST_NODE *if_true;
  AST_NODE *if_false;
};
typedef struct AST_NODE_IF AST_NODE_IF;

struct AST_NODE_STAT {
  // For all AST_NODE
  enum AST_NODE_TYPE type; // first pos
  // For STATEMENT
  struct AST_NODE_STAT *next; // list
  AST_NODE *ptr;
};
typedef struct AST_NODE_STAT AST_NODE_STAT;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

AST_NODE *AST_NODE_PCODE_Create(FPCODE *code, AST_NODE *arg1, AST_NODE *arg2);
AST_NODE *AST_NODE_IF_Create(AST_NODE *test, AST_NODE *if_true,
                             AST_NODE *if_false);
AST_NODE *AST_NODE_STAT_Create(AST_NODE_STAT *next, AST_NODE *ptr);

#endif /* _AST_H_ */
