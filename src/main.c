#include "fpcode.h"
#include "grammar.y.h"
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>

#include "mem.h"
#include "mem_prgm.h"

#include "ast.h"
#include "ast_displayer.h"
#include "ast_traversal.h"

#include "po.h"
#include "po_objstack.h"

#include "utils/hashtable.h"

AST_NODE *BISON_Parse(void) {
  AST_NODE *root = NULL;

  yydebug = 0;
  int ret = yyparse(&root);

  assert(ret == 0);
  assert(root);
  return root;
}

int main(void) {
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
  HT_Insert(ht, "test12", (void *)12);

  printf("GET %p\n", HT_Get(ht, "test6"));
  HT_FPrint(stdout, ht);
  return 0;

  // auto flush stdout
  setvbuf(stdout, NULL, _IONBF, 0);

  // parse file
  AST_NODE *root = BISON_Parse();
  printf("DONE0\n");

  AST_DISPLAY_Text(root, 0);
  printf("DONE1\n");

  AST_DISPLAY_DotF(root, "out.dot");
  printf("DONE2\n");

  size_t size;
  PCODE **prgm = AST_ComputePrgm(root, &size);
  MEMPRGM_Init(prgm, size);
  printf("DONE3\n");

  MEMPRGM_Print();
  printf("DONE4\n");

  PO_Run();

  // State of PO
  MEM_Print();
  PO_OBJSTACK_Print();
  // Free
  // PRGM_Free();

  return 0;
}
