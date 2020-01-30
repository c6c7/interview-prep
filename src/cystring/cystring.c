#include "cystring.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_BUFFER_SIZE 128

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
