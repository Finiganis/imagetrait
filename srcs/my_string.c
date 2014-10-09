#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "my_string.h"

size_t	my_strlen(const char *s) {
  int i;

  for (i = 0; s[i] != '\0'; i++) {}
  return (i);
}

void *my_memalloc(size_t size) {
  if (size) {
    void *new_ptr = malloc(size);
    if (new_ptr != NULL) {
      memset(new_ptr, 0, size);
      return (new_ptr);
    }
  }
  return (NULL);
}

char *my_strchr(const char *s, int (*predicate)(int)) {
  for (size_t i = 0; i <= my_strlen(s); i++) {
    if (predicate(s[i])) {
      return ((char *) &s[i]);
    }
  }
  return (NULL);
}

size_t my_strpos(const char *s, int (*predicate)(int)) {
  size_t i = 0;

  for (; !predicate(s[i]) && s[i]; i++) { }
  return (i);
}

char *my_strnew(size_t size) {
  return ((char *) my_memalloc(sizeof(char) * (size + 1)));
}

char *my_strsub(char const *s, unsigned int start, size_t len) {
  if (s != NULL) {
    char *new_str = my_strnew(len);
    strncpy(new_str, (char *) s + start, len);
    return (new_str);
  } else {
    return (NULL);
  }
}

int my_strncmp(const char *s1, const char *s2, size_t n) {
  for (size_t i = 0; i < n && (s1[i] || s2[i]); i++) {
    if (s1[i] != s2[i]) {
      return (s1[i] - s2[i]);
    }
  }
  return (0);
}

int my_strnequ(const char *s1, const char *s2, size_t n) {
  return (!my_strncmp(s1, s2, n));
}

int my_strequ(const char *s1, const char *s2) {
  return (!my_strcmp(s1, s2));
}

int my_strcmp(const char *s1, const char *s2) {
  for (size_t i = 0; s1[i] || s2[i]; i++) {
    if (s1[i] != s2[i]) {
      return (s1[i] - s2[i]);
    }
  }
  return (0);
}

void my_memdel(void **p) {
  if (p != NULL && *p != NULL) {
    free(*p);
    *p = NULL;
  }
}

void my_strdel(char **s) {
  my_memdel((void **) s);
}

char *my_strdup(const char *s1) {
  if (s1) {
    char *cpy = my_strnew(my_strlen(s1));
    strcpy(cpy, s1);
    return (cpy);
  } else {
    return (NULL);
  }
}
