#if !defined(AUTOCOMPLETE_H)

#include <stddef.h>

void add_line(char* line);

void get_suggestions(char* input, const char** suggestions, size_t max_suggestions);

#endif
