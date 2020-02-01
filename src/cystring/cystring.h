#ifndef CYSTRING_H
#define CYSTRING_H

#include <stdlib.h>

#define TYPE_ARRAY_TO_STR_H(TYPE, FORMAT_SPECIFIER) \
  char *TYPE##_array_to_str(TYPE *ptr, size_t length);

/* Similar to sprintf except this function will
 * allocate a new string.
 */
char *compose_str(const char *format, ...);

TYPE_ARRAY_TO_STR_H(int, "%d")

TYPE_ARRAY_TO_STR_H(double, "%.3e")

#endif
