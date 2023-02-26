/*
 *  @description: Zebro parser.
 *  @author: Ian Marco Moffett.
 */

#include <frontend/parser.h>
#include <frontend/lexer.h>
#include <frontend/symbol.h>
#include <frontend/ast.h>
#include <backend/gen_x64.h>
#include <diag.h>
#include <stdlib.h>

#define CHECK_UNREACHABLE                               \
  if (state->func_has_ret)                              \
  {                                                     \
    diag_warning(state, "Statement is unreachable\n");  \
  }

static struct token last_token;

/*
 *  If the last token
 *  is the start of a symbol.
 */
static uint8_t is_sym_start(void)
{
  switch (last_token.type)
  {
    case TT_PROC:
      return 1;
    default:
      return 0;
  }
}

/*
 *  Raises an error if the
 *  current token does not match
 *  `tt`.
 */

static void passert(struct zebro_state* state, tokentype_t tt,
                    const char *what)
{
  if (last_token.type != tt)
  {
    diag_err(state, "Expected %s\n", what);
    exit(1);
  }
}

static struct ast_node *primary_expr(struct zebro_state *state)
{
  struct ast_node *n;

  switch (last_token.type)
  {
    case TT_INTLIT:
      n = mkastleaf(AST_INTLIT, last_token.val_int);
      scan(state, &last_token);
      return n;
    default:
      diag_err(state, "Syntax error");
      exit(1);
  }
}

static ast_nodetype_t tok2op(struct zebro_state *state, tokentype_t type) {
  switch (type) {
    case TT_PLUS:
      return AST_ADD;
    case TT_MINUS:
      return AST_SUB;
    case TT_STAR:
      return AST_MUL;
    case TT_SLASH:
      return AST_DIV;
    default:
      diag_err(state, "Syntax error");
      exit(1);
  }

  __builtin_unreachable();
}

struct ast_node *binary_expr(struct zebro_state* state)
{
  struct ast_node *left = primary_expr(state);

  if (last_token.type == TT_SEMI)
  {
    return left;
  }

  ast_nodetype_t nodetype = tok2op(state, last_token.type);
  scan(state, &last_token);

  struct ast_node *right = binary_expr(state);
  passert(state, TT_SEMI, "';' after binary expression");
  return mkastnode(nodetype, left, NULL, right, 0);
}

/*
 *  Handle a return statement.
 *
 *  TODO: Check if return is valid.
 */

static struct ast_node *return_statement(struct zebro_state *state)
{
  struct ast_node *tree = NULL; 
  scan(state, &last_token);

  tree = mkastunary(AST_RET, binary_expr(state), 0);
  scan(state, &last_token);
  return tree;

}

/*
 *  A compound statement is a statement
 *  with n substatements like the following:
 *
 *  if (1) 
 *  {
 *    // ..
 *  }
 *
 *  or
 *
 *  proc func() -> int
 *  {
 *    // ..
 *  }
 */

static struct ast_node *compound_statement(struct zebro_state *state)
{
  struct ast_node *left;
  struct ast_node *tree;

  passert(state, TT_LBRACE, "'{'");
  scan(state, &last_token);

  uint8_t is_looping = 1;

  while (is_looping)
  {
    switch (last_token.type)
    {
      case TT_RETURN:
        CHECK_UNREACHABLE;

        state->func_has_ret = 1;
        tree = return_statement(state);

        if (last_token.type == TT_RBRACE)
        {
          return tree;
        }

        break;
      case TT_RBRACE:
        if (!state->func_has_ret)
        {
          diag_err(state, "function should return value but no return "
                          "statement found!\n");

          exit(1);
        }

        is_looping = 0;
        continue;
    }

    if (tree)
    {
      if (left == NULL)
      {
        left = tree;
      }
      else
      {
        left = mkastnode(AST_GLUE, left, NULL, tree, 0);
      }
    }
  }

  return tree;
}

/*
 *  Handles the `proc` keyword.
 *  
 *  For now this parses:
 *
 *  proc <identifier>() -> u8
 */
  
static struct ast_node *handle_proc(struct zebro_state *state)
{
  scan(state, &last_token); /* Assume we are still on `proc` and skip it */
  passert(state, TT_ID, "<identifier> after `proc`");

  char *ident = state->last_str;

  if (is_defined(state, ident))
  {
    diag_err(state, "Redefinition of \"%s\"\n", ident);
    exit(1);
  }
  
  scan(state, &last_token);
  passert(state, TT_LPAREN, "'(' after <identifier>");

  scan(state, &last_token);
  passert(state, TT_RPAREN, "')'");

  scan(state, &last_token);
  passert(state, TT_MINUS, "'->'");

  scan(state, &last_token);
  passert(state, TT_GT, "'->'");

  scan(state, &last_token);
  passert(state, TT_U8, "'u8'");    /* TODO: Check different types */

  scan(state, &last_token);
  
  sym_linkage_t linkage = state->pub ? L_EXTERNAL : L_INTERNAL;
  size_t sym_id = symtbl_push(state, ident, S_FUNCTION, P_U8, linkage);

  state->pub = 0;
  lexer_destroy_laststr(state); 

  struct ast_node *n = compound_statement(state);
  return mkastnode(AST_FUNC, mkastleaf(AST_ID, sym_id), NULL, n, 0);
}

static struct ast_node *check_token(struct zebro_state *state)
{
  if (last_token.type == TT_PUB)
  {
    scan(state, &last_token);
    state->pub = 1;

    if (!is_sym_start())
    {
      diag_err(state, "Expected function or variable after `pub`\n");
      exit(1);
    }
    
    if (last_token.type == TT_PROC)    /* Found `pub proc` */
    {
      return handle_proc(state);
    }
  }
  else if (last_token.type == TT_PROC)
  {
    handle_proc(state);
  }
}

void parse(struct zebro_state *state)
{

  gen_x64_begin(state);
  while (scan(state, &last_token))
  {
    if (last_token.type != TT_PROC && last_token.type != TT_PUB)
    {
      diag_err(state, "Expected 'proc' or 'pub'\n");
      exit(1);
    }
    struct ast_node *tree = check_token(state);
    x64_gen(state, tree, -1, -1);
  }
}
