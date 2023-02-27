/*
 *  @description: Zebro lexer.
 *  @author: Ian Marco Moffett.
 */

#include <frontend/lexer.h>
#include <sysdeps/mem.h>
#include <stdio.h>
#include <diag.h>
#include <stdlib.h>
#include <string.h>

#define IS_LETTER(lexeme) \
  ((lexeme >= 'a' && lexeme <= 'z') || (lexeme >= 'A' && lexeme <= 'Z'))

#define IS_DIGIT(lexeme) \
  (lexeme >= '0' && lexeme <= '9')

#define SHOULD_SKIP(lexeme) \
  (lexeme == '\r' || lexeme == '\n' || lexeme == '\f' || lexeme == ' ')

static char *scanstr(char lexeme, struct zebro_state *state)
{
  char *buf = zebro_malloc(sizeof(char));
  if (buf == NULL)      /* Ensure allocation was a success */
  {
    printf("INTERNAL ERROR: Could not allocate memory in %s:%s\n", 
           __FILE__, __func__
    );
    exit(1);
  }

  size_t buf_idx = 0;
  uint8_t is_looping = 1;

  while (1)
  {
    buf[buf_idx++] = lexeme;
    buf = zebro_realloc(buf, sizeof(char) * (buf_idx + 1));
    lexeme = fgetc(state->fp);
    
    /*
     *  The reason why we can accept digits
     *  is because an identifier can be like
     *  the following:
     *
     *  test123
     */
    if (!IS_LETTER(lexeme) && lexeme != '_' && !IS_DIGIT(lexeme))
    {
      state->putback = lexeme;    /* Use this lexeme later */
      break;
    }

    ++state->col;
  }
  
  buf[buf_idx++] = '\0';
  return buf;
}


static int chrpos(const char *s, char lexeme)
{
  char* p = strchr(s, lexeme);
  return p ? p - s : -1;
}

static uint64_t scanint(struct zebro_state *state, char lexeme) {
  int k = 0, val = 0;

  while ((k = chrpos("0123456789", lexeme)) >= 0)
  {
    val = val * 10 + k;
    lexeme = fgetc(state->fp);
    ++state->col;
  }

  /* Spare the non integer */
  state->putback = lexeme;
  return val;
}

static void checkstr(char *str, struct zebro_state *state,
                     struct token *token_out)
{

  token_out->line = state->line;
  token_out->col = state->col;

  if (strcmp(str, "pub") == 0)
  {
    token_out->type = TT_PUB;
  }
  else if (strcmp(str, "proc") == 0)
  {
    token_out->type = TT_PROC;
  }
  else if (strcmp(str, "u8") == 0)
  {
    token_out->type = TT_U8;
  }
  else if (strcmp(str, "return") == 0)
  {
    token_out->type = TT_RETURN;
  }
  else
  {
    token_out->type = TT_ID;
  }
}

static void check_lexeme(struct token *token_out, struct zebro_state *state,
                         char lexeme)
{
  switch (lexeme)
  {
    case '(':
      token_out->type = TT_LPAREN;
      break;
    case ')':
      token_out->type = TT_RPAREN;
      break;
    case '-':
      token_out->type = TT_MINUS;
      break;
    case '>':
      token_out->type = TT_GT;
      break;
    case '{':
      token_out->type = TT_LBRACE;
      break;
    case '}':
      token_out->type = TT_RBRACE;
      break;
    case ';':
      token_out->type = TT_SEMI;
      break;
    default:
      if (IS_LETTER(lexeme) || lexeme == '_')
      {
        char *str = scanstr(lexeme, state);
        checkstr(str, state, token_out);

        if (token_out->type != TT_ID)
        {
          zebro_free(str);
        }
        else
        {
          state->last_str = str;
        }

        break;
      }
      else if (IS_DIGIT(lexeme))
      {
        token_out->type = TT_INTLIT;
        token_out->val_int = scanint(state, lexeme);
        break;
      }

    diag_err(state, "Unrecognized token '%c' found while scanning\n", lexeme);
    exit(1);
  }
}

uint8_t scan(struct zebro_state *state, struct token *token_out)
{
  char lexeme;
  
  if (state->putback != '\0')
  {
    /* Use a a lexeme that was saved for later */
    lexeme = state->putback;
    state->putback = '\0';
  }
  else
  {
    lexeme = fgetc(state->fp);
  }

  while (SHOULD_SKIP(lexeme) && lexeme != EOF) 
  {
    if (lexeme == '\n')
    {
      ++state->line;
      state->col = 1;
    }

    lexeme = fgetc(state->fp); 
  }

  if (lexeme == EOF)
  {
    return 0;
  } 

  check_lexeme(token_out, state, lexeme);
  
  ++state->col;
  return 1;
}


void lexer_destroy_laststr(struct zebro_state *state)
{
  if (state->last_str != NULL)
  {
    zebro_free(state->last_str);
    state->last_str = NULL;
  }
}
