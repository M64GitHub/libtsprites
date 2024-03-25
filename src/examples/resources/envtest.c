
#include <endian.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include <unistd.h>

char **my_environ = NULL;

char *environ[] = {
    "_=./*local*/bin/env",
    "NVM_DIR=/home/m64/.nvm",
    "NVM_CD_FLAGS=-q",
    "NVM_BIN=/home/m64/.nvm/versions/node/v16.20.2/bin",
    "NVM_INC=/home/m64/.nvm/versions/node/v16.20.2/include/node",
    "P9K_SSH=0",
    "_P9K_SSH_TTY=/dev/pts/7",
    "\x00",
    "\x00",
    "\x00",
    "\x51"};

// MS: stripped down copy of getenv() for i386/amd64 linux
// __BYTE_ORDER == __LITTLE_ENDIAN || !_STRING_ARCH_unaligned is TRUE
// (__LITTLE_ENDIAN == __LITTLE_ENDIAN,
// _STRING_ARCH_unaligned: not defined

/* Return the value of the environment variable NAME.  This implementation
   is tuned a bit in that it assumes no environment variable has an empty
   name which of course should always be true.  We have a special case for
   one character names so that for the general case we can assume at least
   two characters which we can access.  By doing this we can avoid using the
   `strncmp' most of the time.  */
char *my_getenv(name) const char *name;
{
  size_t len = strlen(name);
  char **ep;
  uint16_t name_start;

  if (my_environ == NULL || name[0] == '\0')
    return NULL;

  if (name[1] == '\0') {
    /* The name of the variable consists of only one character.
       Therefore the first two characters of the environment entry are
       this character and a '=' character.  */
    name_start = ('=' << 8) | *(const unsigned char *)name;
    for (ep = my_environ; *ep != NULL; ++ep) {
      uint16_t ep_start =
          (((unsigned char *)*ep)[0] | (((unsigned char *)*ep)[1] << 8));
      if (name_start == ep_start)
        return &(*ep)[2];
    }
  } else {
    name_start = (((const unsigned char *)name)[0] |
                  (((const unsigned char *)name)[1] << 8));
    len -= 2;
    name += 2;

    for (ep = my_environ; *ep != NULL; ++ep) {
      uint16_t ep_start =
          (((unsigned char *)*ep)[0] | (((unsigned char *)*ep)[1] << 8));

      if (name_start == ep_start && !strncmp(*ep + 2, name, len) &&
          (*ep)[len + 2] == '=')
        return &(*ep)[len + 3];
    }
  }

  return NULL;
}

int main(int argc, char **argv) {
  char *rstring = 0;

  if (argc < 2)
    return 1;

  my_environ = (char **)environ;

  printf("looking for %s\n", argv[1]);

  rstring = my_getenv(argv[1]);

  if (!rstring) {
    printf("not found!\n");
    return 2;
  }

  printf("'%s' = '%s'\n", argv[1], rstring);

  return 0;
}

// MS: Original Version of getenv();
//
// /* Return the value of the environment variable NAME.  This implementation
//    is tuned a bit in that it assumes no environment variable has an empty
//    name which of course should always be true.  We have a special case for
//    one character names so that for the general case we can assume at least
//    two characters which we can access.  By doing this we can avoid using the
//    `strncmp' most of the time.  */
// char *getenv(name) const char *name;
// {
//   size_t len = strlen(name);
//   char **ep;
//   uint16_t name_start;
//
//   if (__environ == NULL || name[0] == '\0')
//     return NULL;
//
//   if (name[1] == '\0') {
//     /* The name of the variable consists of only one character.  Therefore
//        the first two characters of the environment entry are this character
//        and a '=' character.  */
// #if __BYTE_ORDER == __LITTLE_ENDIAN || !_STRING_ARCH_unaligned
//     name_start = ('=' << 8) | *(const unsigned char *)name;
// #else
// #if __BYTE_ORDER == __BIG_ENDIAN
//     name_start = '=' | ((*(const unsigned char *)name) << 8);
// #else
// #error "Funny byte order."
// #endif
// #endif
//     for (ep = __environ; *ep != NULL; ++ep) {
// #if _STRING_ARCH_unaligned
//       uint16_t ep_start = *(uint16_t *)*ep;
// #else
//       uint16_t ep_start =
//           (((unsigned char *)*ep)[0] | (((unsigned char *)*ep)[1] << 8));
// #endif
//       if (name_start == ep_start)
//         return &(*ep)[2];
//     }
//   } else {
// #if _STRING_ARCH_unaligned
//     name_start = *(const uint16_t *)name;
// #else
//     name_start = (((const unsigned char *)name)[0] |
//                   (((const unsigned char *)name)[1] << 8));
// #endif
//     len -= 2;
//     name += 2;
//
//     for (ep = __environ; *ep != NULL; ++ep) {
// #if _STRING_ARCH_unaligned
//       uint16_t ep_start = *(uint16_t *)*ep;
// #else
//       uint16_t ep_start =
//           (((unsigned char *)*ep)[0] | (((unsigned char *)*ep)[1] << 8));
// #endif
//
//       if (name_start == ep_start && !strncmp(*ep + 2, name, len) &&
//           (*ep)[len + 2] == '=')
//         return &(*ep)[len + 3];
//     }
//   }
//
//   return NULL;
// }
