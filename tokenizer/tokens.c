/**
 * Shell tokenizer.
 *
 * Splits a line into tokens:
 *   - special single-character tokens: ( ) < > ; |
 *   - quoted strings: "..." becomes one token (quotes removed), and special
 *     characters and whitespace inside it lose their meaning
 *   - words: maximal runs of non-special, non-whitespace, non-quote characters
 * Whitespace (space, tab, newline) separates tokens and is never part of one,
 * except inside quotes.
 *
 * Structured as a small DFA: the main loop looks at the current character and
 * dispatches to the state that handles it; each state returns the index where
 * scanning should resume.
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../vector/vect.h"
#include "tokens.h"

/** Returns 1 if c is whitespace that separates tokens. */
static int is_whitespace(char c) {
  return c == ' ' || c == '\t' || c == '\n';
}

/** Returns 1 if c is a single-character special token. */
static int is_special(char c) {
  return c == '(' || c == ')' || c == '<' || c == '>' || c == ';' || c == '|';
}

/** Returns 1 if c ends a word. */
static int ends_word(char c) {
  return c == '\0' || c == '"' || is_whitespace(c) || is_special(c);
}

/** Adds the len characters starting at start as a single token. */
static void add_substring(vect_t *tokens, const char *start, size_t len) {
  char *token = malloc(len + 1);
  assert(token != NULL);
  memcpy(token, start, len);
  token[len] = '\0';
  vect_add(tokens, token); /* vect_add makes its own copy */
  free(token);
}

/**
 * Reads a quoted string whose opening quote is at input[i]. Adds everything up
 * to the closing quote (or end of input, if unterminated) as one token.
 * Returns the index just past the closing quote.
 */
static size_t read_quoted(const char *input, size_t i, vect_t *tokens) {
  assert(input[i] == '"');
  size_t start = i + 1;
  size_t end = start;
  while (input[end] != '\0' && input[end] != '"') {
    end++;
  }
  add_substring(tokens, input + start, end - start);
  return input[end] == '"' ? end + 1 : end;
}

/**
 * Reads a word starting at input[i] and adds it as a token.
 * Returns the index of the first character after the word.
 */
static size_t read_word(const char *input, size_t i, vect_t *tokens) {
  size_t end = i;
  while (!ends_word(input[end])) {
    end++;
  }
  add_substring(tokens, input + i, end - i);
  return end;
}

/** Splits the given line into a newly allocated vector of tokens. */
vect_t *tokenize(const char *input) {
  assert(input != NULL);
  vect_t *tokens = vect_new();

  size_t i = 0;
  while (input[i] != '\0') {
    char c = input[i];
    if (is_whitespace(c)) {
      i++;
    } else if (is_special(c)) {
      add_substring(tokens, input + i, 1);
      i++;
    } else if (c == '"') {
      i = read_quoted(input, i, tokens);
    } else {
      i = read_word(input, i, tokens);
    }
  }
  return tokens;
}
