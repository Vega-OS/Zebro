/*
 *  @description: lexer.h
 *  @author: Ian Marco Moffett.
 */

#ifndef LEXER_H
#define LEXER_H

#include <state.h>
#include <stdint.h>
#include <stddef.h>

typedef enum
{
  /* Reserved keywords */
  TT_PROC,
  TT_PUB,
  TT_U8,
  
  /* Misc */
  TT_ID,          /* Identifier */
  TT_LPAREN,      /* '(' */
  TT_RPAREN,      /* ')' */
  TT_MINUS,       /* '-' */
  TT_GT,          /* '> */
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
void lexer_destroy_laststr(struct zebro_state *state);

#endif
