#include "fpcode.h"
#include "grammar.y.h"
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>

#include "mem_prgm.h"

#include "ast.h"
#include "ast_displayer.h"
#include "ast_traversal.h"

#include "po.h"
#include "po_ctxstack.h"
#include "po_objstack.h"

#include "utils/hashtable.h"

extern int yy_flex_debug;

AST_NODE *BISON_Parse(void) {
  AST_NODE *root = NULL;

  yydebug = 0;
  yy_flex_debug = 0;

  int ret = yyparse(&root);
  assert(ret == 0);
  assert(root);
  return root;
}

int main(void) {
  // TU_HashTable();

  // auto flush stdout
  setvbuf(stdout, NULL, _IONBF, 0);

  // parse file
  AST_NODE *root = BISON_Parse();
  printf("DONE0\n");

  AST_DISPLAY_Text(root, 0);
  printf("DONE1\n");

  AST_DISPLAY_DotF(root, "out.dot");
  printf("DONE2\n");

  size_t mainsize;
  size_t *subsizes;
  PCODE ***prgm = AST_ComputePrgm(root, &mainsize, &subsizes);
  MEMPRGM_Init(prgm, mainsize, subsizes);
  printf("DONE3\n");

  PO_Run();

  // State of PO
  PO_OBJSTACK_Print();
  CTX_printCur();
  // Free
  // PRGM_Free();

  return 0;
}
