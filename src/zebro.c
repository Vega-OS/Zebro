/*
 *  @description: Zebro main.
 *  @author: Ian Marco Moffett.
 */

#include <stdio.h>
#include <version.h>
#include <string.h>
#include <stdlib.h>
#include <frontend/state.h>
#include <frontend/parser.h>
#include <sysdeps/mem.h>

static const char help_msg[] = {
  "ZebroCompiler v" VERSION "\n"
  "--help: Bring up this message\n"
};

static inline void help(void)
{
  printf(help_msg);
  exit(1);
}

static void compile(struct zebro_state *state, const char *filename)
{
  state->fp = fopen(filename, "r");
  state->current_filename = filename;

  if (state->fp == NULL)
  {
    extern const char* __progname;
    printf("%s: \033[0;31merror: \033[39mFailed to open \"%s\"\n",
           __progname, filename
    );
    exit(1);
  }
  
  parse(state);
  fclose(state->fp);
  memset(state, 0, sizeof(struct zebro_state));  // Reset all state.
}

static void handle_args(struct zebro_state *state, int argc, char **argv)
{
  for (int i = 1; i < argc; ++i)
  {
    if (strcmp(argv[i], "--help") == 0)
    {
      help();
    }
    else if (argv[i][0] != '-')
    {
      compile(state, argv[i]);
    }
    else
    {
      help();
    }
  }
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    printf("Usage: %s <files>\n", argv[0]);
    printf("Please pass --help for more info\n");
    return 1;
  }

  struct zebro_state state = {0};
  state.line = 1;
  state.col = 1;

  handle_args(&state, argc, argv); 
  return 0;
}
