#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define DEFAULT_WORD_CAPACITY 40
#define DEFAULT_WORD_ARRAY_CAPACITY 128
#define SCALING_FACTOR 2

// compare two given frobnicated string.
// Return 1 if the original string of s1 is 'greater than' that of s2,
// return 0 if equal, -1 otherwise.
int frobcmp(const void * s1, const void * s2);

// given two frobinated string's pointers, compare them using frobcmp.
int frobptrcmp(const void * s1, const void * s2);

int main() {
  size_t word_buf_cap = DEFAULT_WORD_CAPACITY;
  size_t array_buf_cap = DEFAULT_WORD_ARRAY_CAPACITY;
  char c;
  char * wordbuf = (char *) malloc(word_buf_cap * sizeof(char));
  if (wordbuf == NULL) {
    fprintf(stderr, "Failed to malloc for word. Error %d.\n", errno);
    exit(1);
  }
  char ** arrbuf = (char **) malloc(array_buf_cap * sizeof(char *));
  if (wordbuf == NULL) {
    fprintf(stderr, "Failed to malloc for word array. Error %d.\n", errno);
    exit(1);
  }
  int i = 0, word_count = 0;
  while (!feof(stdin)) {
    c = getchar();
    wordbuf[i] = c;
    i += 1;
    // if the word buffer is full, resize the word buffer
    if (i == word_buf_cap) {
      word_buf_cap *= SCALING_FACTOR;
      wordbuf = (char *) realloc(wordbuf, word_buf_cap * sizeof(char));
      if (wordbuf == NULL) {
        fprintf(stderr, "Failed to realloc for word. Error %d.\n", errno);
        exit(1);
      }
    }
    // hit the end of a word
    if (c == ' ') {
      arrbuf[word_count] = wordbuf;  // save the word to the word array
      word_buf_cap = DEFAULT_WORD_CAPACITY;  // reset the word buffer's size
      wordbuf = (char *) malloc(word_buf_cap * sizeof(char));
      if (wordbuf == NULL) {
        fprintf(stderr, "Failed to realloc for word. Error %d.\n", errno);
        exit(1);
      }
      i = 0;
      word_count += 1;
      // if the word array buffer is full, resize the word array buffer
      if (word_count == array_buf_cap) {
        array_buf_cap *= SCALING_FACTOR;
        arrbuf = (char **) realloc(arrbuf, array_buf_cap * sizeof (char *));
      if (wordbuf == NULL) {
        fprintf(stderr, "Failed to realloc for word array. Error %d.\n", errno);
        exit(1);
      }
      }
    }
  }

  // save the last word if it was not saved
  if (c != ' ') {
    wordbuf[i - 1] = ' ';
    arrbuf[word_count] = wordbuf;  // save the word to the word array
    word_count += 1;
  }

  qsort(arrbuf, word_count, sizeof(char *), frobptrcmp);

  // write to stdout
  for (int k = 0; k < word_count; k ++) {
    char * word = arrbuf[k];
    int i = 0;
    while (word[i] != ' ') {
      putchar(word[i]);
      i += 1;
    }
    putchar(word[i]);
  }

  // free the memory
  for (int k = 0; k < word_count; k ++) {
    free(arrbuf[k]);
  }
  free(arrbuf);
  return 0;
}

int frobcmp(const void * s1, const void * s2) {
  char *a = (char *) s1, *b = (char *) s2;
  while (*a != ' ' || *b != ' ') {
    if ((*a ^ 42) < (*b ^ 42))  return -1;
    if ((*a ^ 42) > (*b ^ 42))  return 1;
    a++; b++;
  }
  if (*a == ' ')  return -1;
  if (*b == ' ')  return 1;
  // the two frobnicated memory are exactly the same
  return 0;
}

int frobptrcmp(const void * s1, const void * s2) {
  return frobcmp(*(char **)s1, *(char **)s2);
}
