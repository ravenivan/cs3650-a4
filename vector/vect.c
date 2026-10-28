/**
 * Vector implementation.
 *
 * - Implement each of the functions to create a working growable array (vector).
 * - Do not change any of the structs
 * - When submitting, You should not have any 'printf' statements in your vector 
 *   functions.
 *
 * IMPORTANT: The initial capacity and the vector's growth factor should be 
 * expressed in terms of the configuration constants in vect.h
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "vect.h"

/** Main data structure for the vector. */
struct vect {
  char **data;             /* Array containing the actual data. */
  unsigned int size;       /* Number of items currently in the vector. */
  unsigned int capacity;   /* Maximum number of items the vector can hold before growing. */
};

/** Returns a newly allocated copy of the given string. The caller owns it. */
static char *copy_string(const char *str) {
  assert(str != NULL);
  size_t len = strlen(str);
  char *copy = malloc(len + 1);
  assert(copy != NULL);
  memcpy(copy, str, len + 1); /* includes the '\0' */
  return copy;
}

/**
 * Grows the underlying array by VECT_GROWTH_FACTOR, capped at
 * VECT_MAX_CAPACITY. realloc keeps the existing pointers in place.
 */
static void vect_grow(vect_t *v) {
  assert(v != NULL);
  assert(v->capacity < VECT_MAX_CAPACITY);
 
  unsigned long new_capacity = (unsigned long) v->capacity * VECT_GROWTH_FACTOR;
  if (new_capacity > VECT_MAX_CAPACITY) {
    new_capacity = VECT_MAX_CAPACITY;
  }
 
  char **new_data = realloc(v->data, new_capacity * sizeof(char *));
  assert(new_data != NULL);
 
  v->data = new_data;
  v->capacity = (unsigned int) new_capacity;
}

/** Construct a new empty vector. */
vect_t *vect_new() {

  vect_t *v = malloc(sizeof(vect_t));
  assert(v != NULL);
 
  v->data = calloc(VECT_INITIAL_CAPACITY, sizeof(char *));
  assert(v->data != NULL);
 
  v->size = 0;
  v->capacity = VECT_INITIAL_CAPACITY;
  return v;

  return v;
}

/** Delete the vector, freeing all memory it occupies. */
void vect_delete(vect_t *v) {

  assert(v != NULL);
 
  for (unsigned int i = 0; i < v->size; i++) {
    free(v->data[i]);
  }
  free(v->data);
  free(v);

}

/** Get the element at the given index. */
const char *vect_get(vect_t *v, unsigned int idx) {
  assert(v != NULL);
  assert(idx < v->size);
  return v->data[idx];

  return NULL;
}

/** Get a copy of the element at the given index. The caller is responsible
 *  for freeing the memory occupied by the copy. */
char *vect_get_copy(vect_t *v, unsigned int idx) {
  assert(v != NULL);
  assert(idx < v->size);
  return copy_string(v->data[idx]);

  return NULL;
}
/** Set the element at the given index. */
void vect_set(vect_t *v, unsigned int idx, const char *elt) {
  assert(v != NULL);
  assert(elt != NULL);
  assert(idx < v->size);
 
  /* Copy before freeing, in case elt points at the element being replaced. */
  char *copy = copy_string(elt);
  free(v->data[idx]);
  v->data[idx] = copy;

}

/** Add an element to the back of the vector. */
void vect_add(vect_t *v, const char *elt) {
  assert(v != NULL);
  assert(elt != NULL);
 
  if (v->size == v->capacity) {
    vect_grow(v);
  }
  v->data[v->size] = copy_string(elt);
  v->size++;

}

/** Remove the last element from the vector. */
void vect_remove_last(vect_t *v) {
  assert(v != NULL);
  assert(v->size > 0);
 
  v->size--;
  free(v->data[v->size]);
  v->data[v->size] = NULL;

}

/** The number of items currently in the vector. */
unsigned int vect_size(vect_t *v) {
  assert(v != NULL);
  return v->size;

}

/** The maximum number of items the vector can hold before it has to grow. */
unsigned int vect_current_capacity(vect_t *v) {
  assert(v != NULL);
  return v->capacity;
}

