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
//     [mwalter, May 23, 2011]: Simplified to support use of PPM, JPEG (via
//                              LIBJPEG), and PNG (via LIBPNG)

// Based heavily on example code in libpng. Some bugs fixed though.

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#include <png.h>
// IWYU pragma: no_include "pngconf.h"
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "glmint.h"  // IWYU pragma: keep

#define ERR_NO_ERROR 0
#define ERR_OPEN 1
#define ERR_MEM 2
#define ERR_PNGLIB 3
#define ERR_OPEN_WRITE 4
#define ERR_PNGLIB_WRITE 5
#define ERR_MEM_WRITE 6

static int pngerror = ERR_NO_ERROR;

// my setjmp buffer
static jmp_buf setjmp_buffer;

// called my libpng
static void warn_callback(png_structp ps, png_const_charp pc) {
  fprintf(stderr, "PNG warn: %s\n", pc);
  // TODO(ashuang): notify?
}

static void err_callback(png_structp ps, png_const_charp pc) {
  fprintf(stderr, "PNG error: %s\n", pc);

  // TODO(ashuang): store error message?
  longjmp(setjmp_buffer, 1);
}

GLubyte* glmReadPNG(const char* filename, GLboolean alpha, int* width_ret,
                    int* height_ret, int* type_ret) {
  png_structp png_ptr;
  png_infop info_ptr;
  png_uint_32 width, height;

  int bit_depth, color_type, interlace_type;
  FILE* fp;
  unsigned char* buffer;
  int y, bytes_per_row;
  int channels;
  int format;
  png_bytepp row_pointers;

  if ((fp = fopen(filename, "rb")) == NULL) {
    pngerror = ERR_OPEN;
    return NULL;
  }

  // Create and initialize the png_struct with the desired error handler
  // functions.  If you want to use the default stderr and longjump method,
  // you can supply NULL for the last three parameters.  We also supply the
  // the compiler header file version, so that we know if the application
  // was compiled with a compatible version of the library.  REQUIRED
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, err_callback,
                                   warn_callback);

  if (png_ptr == NULL) {
    pngerror = ERR_MEM;
    fclose(fp);
    return 0;
  }

  // Allocate/initialize the memory for image information.  REQUIRED.
  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL) {
    pngerror = ERR_MEM;
    fclose(fp);
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    return 0;
  }

  // Set error handling if you are using the setjmp/longjmp method (this is
  // the normal method of doing things with libpng).  REQUIRED unless you
  // set up your own error handlers in the png_create_read_struct() earlier.

  buffer = NULL;

  if (setjmp(setjmp_buffer)) {
    pngerror = ERR_PNGLIB;
    // Free all of the memory associated with the png_ptr and info_ptr
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    fclose(fp);
    // If we get here, we had a problem reading the file

    if (buffer) {
      free(buffer);
    }
    return NULL;
  }

  // Set up the input control if you are using standard C streams
  png_init_io(png_ptr, fp);

  // The call to png_read_info() gives us all of the information from the
  // PNG file before the first IDAT (image data chunk).  REQUIRED
  png_read_info(png_ptr, info_ptr);

  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
               &interlace_type, NULL, NULL);

  // Set up the data transformations you want.  Note that these are all
  // optional.  Only call them if you want/need them.  Many of the
  // transformations only work on specific types of images, and many
  // are mutually exclusive.

  // tell libpng to strip 16 bit/color files down to 8 bits/color
  png_set_strip_16(png_ptr);

  // expand paletted colors into true RGB triplets
  if (color_type == PNG_COLOR_TYPE_PALETTE) {
    png_set_expand(png_ptr);
  }

  // expand grayscale images to the full 8 bits from 1, 2, or 4 bits/pixel
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
    png_set_expand(png_ptr);
  }

  // expand paletted or RGB images with transparency to full alpha channels
  // so the data will be available as RGBA quartets.
  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
    png_set_expand(png_ptr);
  }

  png_read_update_info(png_ptr, info_ptr);

  channels = png_get_channels(png_ptr, info_ptr);

  // allocate the memory to hold the image using the fields of info_ptr.

  bytes_per_row = png_get_rowbytes(png_ptr, info_ptr);

  buffer = (unsigned char*)malloc(bytes_per_row * height);

  format = channels;

  row_pointers = (png_bytepp)malloc(height * sizeof(png_bytep));
  for (y = 0; y < height; y++) {
    row_pointers[height - y - 1] = buffer + y * bytes_per_row;
  }

  png_read_image(png_ptr, row_pointers);
  png_read_end(png_ptr, info_ptr);

  free(row_pointers);

  // clean up after the read, and free any memory allocated - REQUIRED
  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

  // close the file
  fclose(fp);

  // that's it
  if (buffer) {
    *width_ret = width;
    *height_ret = height;
    switch (format) {
      case 1:
        *type_ret = GL_LUMINANCE;
        break;
      case 2:
        *type_ret = GL_LUMINANCE_ALPHA;
        break;
      case 3:
        *type_ret = GL_RGB;
        break;
      case 4:
        *type_ret = GL_RGBA;
        break;
    }
    pngerror = ERR_NO_ERROR;
  } else {
    pngerror = ERR_MEM;
  }
  return buffer;
}
