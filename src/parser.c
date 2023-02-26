/*
 *  @description: Zebro parser.
 *  @author: Ian Marco Moffett.
 */

#include <parser.h>
#include <lexer.h>

static struct token last_token;


void parse(struct zebro_state *state)
{
  while (scan(state, &last_token))
  {
  }
}
