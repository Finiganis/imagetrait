#ifndef MY_STRING_H
# define MY_STRING_H

int is_not_alphanum(int c);

char *my_strdup(const char *s);
size_t my_strlen(const char *s);
void *my_memalloc(size_t size);
void my_memdel(void **p);
char *my_strnew(size_t size);
void my_strdel(char **s);
char *my_strsub(char const *s, unsigned int start, size_t len);
char **my_strnew2d(size_t len);
int my_strnequ(const char *s1, const char *s2, size_t n);
int my_strequ(const char *s1, const char *s2);
int my_strncmp(const char *s1, const char *s2, size_t n);
int my_strcmp(const char *s1, const char *s2);
char *my_strchr(const char *s, int (*predicate)(int));
size_t my_strpos(const char *s, int (*predicate)(int));
char **my_strsplit(const char *s, int (*predicate)(int));
char **compact_tab_word(char **xs);
size_t my_word_count(const char *s, int (*predicate)(int));
void my_words_del(char ***words);
int print_str_array(char **xxs);

#endif /* MY_STRING_H */
