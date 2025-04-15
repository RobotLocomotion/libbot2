// -*- mode: c -*-
// vim: set filetype=c :

/*
 * This file is part of bot2-vis.
 *
 * bot2-vis is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-vis is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-vis. If not, see <https://www.gnu.org/licenses/>.
 */

// Adapted from F. Devernay's extensions to Nate Robbins' GLM library
//
// Source obtained from GLM-0.3.5 available from
// http://devernay.free.fr/hacks/glm/
//
// Changes:
//      [mwalter, May 23, 2011]: Simplified to support use of PPM, JPEG (via
//                               LIBJPEG), and PNG (via LIBPNG)

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "glmint.h"  // IWYU pragma: keep

#ifndef HAVE_STRDUP
// strdup is actually not a standard ANSI C or POSIX routine
// so implement a private one for GLM.  OpenVMS does not have a
// strdup; Linux's standard libc doesn't declare strdup by default
// (unless BSD or SVID interfaces are requested).
char* __glmStrdup(const char* string) {
  char* copy;

  copy = (char*)malloc(strlen(string) + 1);
  if (copy == NULL) {
    return NULL;
  }
  snprintf(copy, strlen(string), "%s", string);
  return copy;
}
#endif

// strip leading and trailing whitespace from a string and return a newly
// allocated string containing the result (or NULL if the string is only
// whitespace)
char* __glmStrStrip(const char* s) {
  int first;
  int last = strlen(s) - 1;
  int len;
  int i;
  char* rets;

  i = 0;
  while (i <= last &&
         (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r')) {
    i++;
  }
  if (i > last) {
    return NULL;
  }
  first = i;
  i = last;
  while (i > first &&
         (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r')) {
    i--;
  }
  last = i;
  len = last - first + 1;
  rets = (char*)malloc(len + 1);  // add a trailing 0
  memcpy(rets, s + first, len);
  rets[len] = 0;
  return rets;
}

void __glmWarning(char* format, ...) {
  va_list args;

  va_start(args, format);
  fprintf(stderr, "GLM: Warning: ");
  vfprintf(stderr, format, args);
  va_end(args);
  putc('\n', stderr);
}

// CENTRY
void __glmReportErrors(void) {
  GLenum error;

  while ((error = glGetError()) != GL_NO_ERROR) {
    __glmWarning("GL error: %s", gluErrorString(error));
  }
}
// ENDCENTRY

void __glmFatalError(char* format, ...) {
  va_list args;

  va_start(args, format);
  fprintf(stderr, "GLM: Fatal Error: ");
  vfprintf(stderr, format, args);
  va_end(args);
  putc('\n', stderr);
  exit(1);
}

void __glmFatalUsage(char* format, ...) {
  va_list args;

  va_start(args, format);
  fprintf(stderr, "GLM: Fatal API Usage: ");
  vfprintf(stderr, format, args);
  va_end(args);
  putc('\n', stderr);
  abort();
}

// glmDirName: return the directory given a path
//
// path - filesystem path
//
// NOTE: the return value should be free'd.
char* __glmDirName(char* path) {
  char* dir;
  char* s;

  dir = __glmStrdup(path);

  s = strrchr(dir, '/');
  if (s) {
    s[1] = '\0';
  } else {
    dir[0] = '\0';
  }

  return dir;
}
