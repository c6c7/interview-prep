#include "cystring.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

char *compose_str(const char *format, ...) {
  va_list args, args_measure;
  va_start(args, format);

  va_copy(args_measure, args);
  int str_size = 1 + vsnprintf(NULL, 0, format, args_measure);
  va_end(args_measure);

  char *str = malloc(str_size);
  vsnprintf(str, str_size, format, args);
  va_end(args);

  return str;
}
