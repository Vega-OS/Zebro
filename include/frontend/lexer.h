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
  TT_RETURN,

  /* Digit types */
  TT_INTLIT,
  
  /* Misc */
  TT_ID,          /* Identifier */
  TT_LPAREN,      /* '(' */
  TT_RPAREN,      /* ')' */
  TT_PLUS,        /* '+' */
  TT_MINUS,       /* '-' */
  TT_STAR,        /* '*' */
  TT_SLASH,       /* '/' */
  TT_GT,          /* '>' */
  TT_LBRACE,      /* '{' */
  TT_RBRACE,      /* '}' */
  TT_SEMI,        /* ';' */ 
} tokentype_t;

struct token
{
  size_t line;
  size_t col;
  ssize_t val_int;
  tokentype_t type;
};

/*
 *  Returns 1 if there are more tokens,
 *  otherwise 0.
 */

uint8_t scan(struct zebro_state *state, struct token *token_out);
void lexer_destroy_laststr(struct zebro_state *state);

#endif
