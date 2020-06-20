/*
 * ast.c
 *
 *  Created on: 14/06/2020
 *      Author: pirx
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "ast.h"
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

const char *AST_NODE_TYPE_STR[] = {"PCODE", "IF", "WHILE", "STAT"};

/*******************************************************************************
 * Public function
 ******************************************************************************/

AST_NODE *AST_NODE_PCODE_Create(PCODE *code, AST_NODE *arg1, AST_NODE *arg2) {
  // AST_NODE_DisplayRecu(fils);
  AST_NODE_PCODE *ret = malloc(sizeof(struct AST_NODE_PCODE));
  ret->type = AST_NODE_TYPE_PCODE;
  ret->arg1 = arg1;
  ret->arg2 = arg2;
  ret->code = code;
  return (AST_NODE *)ret;
}

AST_NODE *AST_NODE_IF_Create(AST_NODE *test, AST_NODE *if_true,
                             AST_NODE *if_false) {
  AST_NODE_IF *ret = malloc(sizeof(struct AST_NODE_IF));
  ret->type = AST_NODE_TYPE_IF;
  ret->test = test;
  ret->if_true = if_true;
  ret->if_false = if_false;
  return (AST_NODE *)ret;
}

AST_NODE *AST_NODE_STAT_Create(AST_NODE_STAT *next, AST_NODE *ptr) {
  AST_NODE_STAT *ret = malloc(sizeof(struct AST_NODE_STAT));
  ret->type = AST_NODE_TYPE_STAT;
  ret->next = next;
  ret->ptr = ptr;
  return (AST_NODE *)ret;
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/
