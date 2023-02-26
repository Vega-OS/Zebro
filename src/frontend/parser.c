/*
 *  @description: Zebro parser.
 *  @author: Ian Marco Moffett.
 */

#include <frontend/parser.h>
#include <frontend/lexer.h>
#include <diag.h>
#include <stdlib.h>

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

/*
 *  Handles the `proc` keyword.
 *  
 *  For now this parses:
 *
 *  proc <identifier>() -> u8
 */
  
static void handle_proc(struct zebro_state *state)
{
  scan(state, &last_token); /* Assume we are still on `proc` and skip it */
  passert(state, TT_ID, "<identifier> after `proc`");
  
  scan(state, &last_token);
  passert(state, TT_LPAREN, "'(' after <identifier>");

  scan(state, &last_token);
  passert(state, TT_RPAREN, "')'");

  scan(state, &last_token);
  passert(state, TT_MINUS, "'->'");

  scan(state, &last_token);
  passert(state, TT_GT, "'->'");

  scan(state, &last_token);
  passert(state, TT_U8, "'u8'");
}

static void check_token(struct zebro_state *state)
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
      handle_proc(state);
    }
  }
}

void parse(struct zebro_state *state)
{
  while (scan(state, &last_token))
  {
    check_token(state);
  }
}
