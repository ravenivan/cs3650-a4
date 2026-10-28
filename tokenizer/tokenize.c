/**
 * Tokenizer driver: reads one line from stdin and prints each token on its
 * own line.
 */
#include <stdio.h>

#include "../vector/vect.h"
#include "tokens.h"

#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  char buffer[BUFFER_SIZE];

  /* fgets returns NULL on end-of-file with nothing read. */
  if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
    return 0;
  }

  vect_t *tokens = tokenize(buffer);
  for (unsigned int i = 0; i < vect_size(tokens); i++) {
    printf("%s\n", vect_get(tokens, i));
  }

  vect_delete(tokens);
  return 0;
}
