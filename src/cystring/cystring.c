#include "cystring.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 128

#define TYPE_ARRAY_TO_STR(TYPE, FORMAT_SPECIFIER)                         \
  char *TYPE##_array_to_str(TYPE *ptr, size_t length) {                   \
    char *middle;                                                         \
    if (length == 0) {                                                    \
      middle = strdup("");                                                \
    } else {                                                              \
      middle = compose_str(FORMAT_SPECIFIER, ptr[0]);                     \
      for (int i = 1; i < length; i++) {                                  \
        char *next = compose_str("%s " FORMAT_SPECIFIER, middle, ptr[i]); \
        free(middle);                                                     \
        middle = next;                                                    \
      }                                                                   \
    }                                                                     \
    char *str = compose_str("[ %s ]", middle);                            \
    free(middle);                                                         \
    return str;                                                           \
  }

char *compose_str(const char *format, ...) {
  va_list args;
  va_start(args, format);

  size_t buffer_size = INITIAL_BUFFER_SIZE;
  char *str;

  while (true) {
    str = malloc(buffer_size);
    if (NULL == str) return NULL;
    va_list args_loop;
    va_copy(args_loop, args);

    // try to write to str and break if successful
    int num_bytes_written = vsnprintf(str, buffer_size, format, args_loop);
    if (num_bytes_written < buffer_size) {
      if (num_bytes_written < 0) {
        return NULL;  // there must have been an encoding error
      }
      va_end(args_loop);
      break;
    }
    va_end(args_loop);

    // we need more buffer!
    size_t prev_buffer_size = buffer_size;
    buffer_size *= 2;
    if (prev_buffer_size >= buffer_size) {
      return NULL;  // the buffer size overflowed
    }
    free(str);
  }
  va_end(args);

  return str;
}

TYPE_ARRAY_TO_STR(int, "%d")
TYPE_ARRAY_TO_STR(double, "%.3e")
