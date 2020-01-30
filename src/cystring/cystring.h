#ifndef CYSTRING_H
#define CYSTRING_H

/* Similar to sprintf except this function will
 * allocate a new string.
 */
char *compose_str(const char *format, ...);

#endif
