#ifndef _JBIG2_ENCODER_H
#define _JBIG2_ENCODER_H

#include <vector>

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "../LeptonLib/allheaders.h"
#include "../LeptonLib/pix.h"
#include "jbig2enc.h"


#if defined(_WIN32) || defined (_WIN64)
// -----------------------------------------------------------------------------
// Windows, sadly, lacks asprintf
// -----------------------------------------------------------------------------
#include <stdarg.h>
int asprintf(char **strp, const char *fmt, ...) 
{
    va_list va;
    va_start(va, fmt);

    const int required = vsnprintf(NULL, 0, fmt, va);
    char *const buffer = (char *) malloc(required + 1);
    const int ret = vsnprintf(buffer, required + 1, fmt, va);
    *strp = buffer;

    va_end(va);

    return ret;
}
#endif

// -----------------------------------------------------------------------------
// Morphological operations for segmenting an image into text regions
// -----------------------------------------------------------------------------
static const char *segment_mask_sequence = "r11";
static const char *segment_seed_sequence = "r1143 + o4.4 + x4"; /* maybe o6.6 */
static const char *segment_dilation_sequence = "d3.3";

// -----------------------------------------------------------------------------
// Takes two pix as input, generated from the same original image:
//   1. pixb   - a binary thresholded image
//   2. piximg - a full color or grayscale image
// and segments them by finding the areas that contain color or grayscale
// graphics, removing those areas from the binary image, and doing the
// opposite for the full color/grayscale image.  The upshot is that after
// this routine has been run, the binary image contains only text and the
// full color image contains only the graphics.
//
// Both input images are modified by this procedure.  If no text is found,
// pixb is set to NULL.  If no graphics is found, piximg is set to NULL.
//
// Thanks to Dan Bloomberg for this
// -----------------------------------------------------------------------------

static PIX*
segment_image(PIX *pixb, PIX *piximg) {
  // Make seed and mask, and fill seed into mask
  PIX *pixmask4 = pixMorphSequence(pixb, (char *) segment_mask_sequence, 0);
  PIX *pixseed4 = pixMorphSequence(pixb, (char *) segment_seed_sequence, 0);
  PIX *pixsf4 = pixSeedfillBinary(NULL, pixseed4, pixmask4, 8);
  PIX *pixd4 = pixMorphSequence(pixsf4, (char *) segment_dilation_sequence, 0);

  // we want to force the binary mask to be the same size as the
  // input color image, so we have to do it this way...
  // is there a better way?
  // PIX *pixd = pixExpandBinary(pixd4, 4);
  PIX *pixd = pixCreate(piximg->w, piximg->h, 1);
  pixCopyResolution(pixd, piximg);
  expandBinaryPower2Low(pixd->data, pixd->w, pixd->h, pixd->wpl,
                        pixd4->data, pixd4->w, pixd4->h, pixd4->wpl, 4);

  pixDestroy(&pixd4);
  pixDestroy(&pixsf4);
  pixDestroy(&pixseed4);
  pixDestroy(&pixmask4);

  pixSubtract(pixb, pixb, pixd);

  // now see what we got from the segmentation
  static l_int32 *tab = NULL;
  if (tab == NULL) tab = makePixelSumTab8();

  // if no image portion was found, set the image pointer to NULL and return
  l_int32  pcount;
  pixCountPixels(pixd, &pcount, tab);
  if (pcount < 100) {
    pixDestroy(&pixd);
    return NULL;
  }

  // if no text portion found, set the binary pointer to NULL
  pixCountPixels(pixb, &pcount, tab);
  if (pcount < 100) {
    pixDestroy(&pixb);
  }

  PIX *piximg1;
  if (piximg->d == 1 || piximg->d == 8 || piximg->d == 32) {
    piximg1 = pixClone(piximg);
  } else if (piximg->d > 8) {
    piximg1 = pixConvertTo32(piximg);
  } else {
    piximg1 = pixConvertTo8(piximg, FALSE);
  }

  PIX *pixd1;
  if (piximg1->d == 32) {
    pixd1 = pixConvertTo32(pixd);
  } else if (piximg1->d == 8) {
    pixd1 = pixConvertTo8(pixd, FALSE);
  } else {
    pixd1 = pixClone(pixd);
  }
  pixDestroy(&pixd);

  pixRasteropFullImage(pixd1, piximg1, PIX_SRC | PIX_DST);

  pixDestroy(&piximg1);

  return pixd1;
}

#endif /* _JBIG2_ENCODER_H */
