/*
 *  @description: ast.h
 *  @author: Ian Marco Moffett.
 */

#ifndef AST_H
#define AST_H

#include <stddef.h>
#include <stdint.h>

typedef enum
{
  AST_FUNC,
} ast_nodetype_t;

struct ast_node
{
  ast_nodetype_t op;
  struct ast_node *left;
  struct ast_node *mid;
  struct ast_node *right;

  union
  {
    size_t val_int;
    size_t id;
  };
};

/*
 *  Creates a whole AST node.
 */

struct ast_node *mkastnode(ast_nodetype_t op, struct ast_node *left,
                           struct ast_node *mid, struct ast_node *right,
                           size_t val_int);

/*
 *  Creates a leaf AST node (no child nodes)
 */

struct ast_node *mkastleaf(ast_nodetype_t op, size_t val_int);

/*
 *  Creates a unary AST node (one child node)
 */

struct ast_node *mkastunary(ast_nodetype_t op, struct ast_node *left,
                            size_t val_int);


void ast_cleanup_nodes(void);

#endif
