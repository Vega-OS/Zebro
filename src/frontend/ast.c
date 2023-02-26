/*
 *  @description: Abstract syntax tree.
 *  @author: Ian Marco Moffett.
 */

#include <frontend/ast.h>
#include <sysdeps/mem.h>

struct ast_node **allocd_nodes = NULL;
static size_t allocd_nodes_count = 0;


struct ast_node *mkastnode(ast_nodetype_t op, struct ast_node *left,
                           struct ast_node *mid, struct ast_node *right,
                           size_t val_int)
{
  if (allocd_nodes == NULL)
  {
    allocd_nodes = zebro_malloc(sizeof(struct ast_node *));
  }

  struct ast_node *n = zebro_malloc(sizeof(struct ast_node));
  n->op = op;
  n->left = left;
  n->mid = mid;
  n->right = right;
  n->val_int = val_int;

  allocd_nodes[allocd_nodes_count++] = n;
  allocd_nodes = zebro_realloc(allocd_nodes,
                               sizeof(struct ast_node *) * (allocd_nodes_count + 1)
  );
  return n;
}

struct ast_node *mkastleaf(ast_nodetype_t op, size_t val_int)
{
  return mkastnode(op, NULL, NULL, NULL, val_int);
}

struct ast_node *mkastunary(ast_nodetype_t op, struct ast_node *left,
                            size_t val_int)
{
  return mkastnode(op, left, NULL, NULL, val_int);
}


void ast_cleanup_nodes(void)
{
  if (allocd_nodes == NULL)
  {
    return;
  }

  for (size_t i = 0; i < allocd_nodes_count; ++i)
  {
    zebro_free(allocd_nodes[i]);
  }

  zebro_free(allocd_nodes);
  allocd_nodes = NULL;
}
