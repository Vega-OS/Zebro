/*
 *  @description: lexer.h
 *  @author: Ian Marco Moffett.
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>
#include <stddef.h>
#include <state.h>

typedef enum
{
  /* Reserved keywords */
  TT_PROC,
  TT_PUB
} tokentype_t;

struct token
{
  size_t line;
  size_t col;
  tokentype_t type;
};

/*
 *  Returns 1 if there are more tokens,
 *  otherwise 0.
 */

uint8_t scan(struct zebro_state *state, struct token *token_out);

#endif
