/* 
 * The contents of this file are subject to the IBM Common Public
 * License Version 0.5 (the "License"); you may not use this file 
 * except in compliance with the License. You may obtain a copy of 
 * the License at http://oss.software.ibm.com/developerworks/
 * opensource/license-cpl.html
 * 
 * The Original Code is the Open Exif Toolkit,
 * released November 13, 2000.
 *
 * The Initial Developer of the Original Code is Eastman 
 * Kodak Company. Portions created by Kodak are 
 * Copyright (C) 2001 Eastman Kodak Company. All 
 * Rights Reserved. 
 *
 * Portions of the Original Code are
 * Copyright (c) 1988-1996 Sam Leffler
 * Copyright (c) 1991-1996 Silicon Graphics, Inc.
 *
 * Creation Date: 02/12/2005
 *
 * Original Author: 
 * Sam Fryer       samuel.fryer@kodak.com
 *
 * Contributor(s): 
 * 
 */ 

 /*
 * Contains code from jdhuff.c, with the following (C) notice:
 *
 * Copyright (C) 1991-1998, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 */

// string.h is needed for memset...
#include <string.h> 

#include "ExifHuffmanTable.h"
namespace Exif{
ExifStatus ExifJpegHUFFTable::deriveTable()
{
    ExifStatus status = EXIF_OK;
    unsigned char code=0,i,k;
    unsigned short p0=0,p1=0,numsymbols=0;
    unsigned int l, lookbits, huffcode[257];

    // I spent a little bit of time consolidating what was
    // in the IJG's code ... then I gave up!
    for (l = 1; (l <= 16) && (status == EXIF_OK); l++) 
    {
        i = num_symbols[l];
        if (p0 + i <= 256)
        {
            for ( k = 0; k < i ; k++ )
            {
                huffcode[p0++] = code;
                code++;
            }
            code <<= 1;
        }
        else
            status = EXIF_JPEG_HUFFMAN_TABLE_ERROR;
        
        if (i > 0)
        {
            /* valoffset[l] = huffval[] index of 1st symbol of code length l,
             * minus the minimum code of length l
             */
            valoffset[l] = p1 - huffcode[p1];
            p1 += i;
            maxcode[l] = huffcode[p1-1]; /* maximum code of length l */
        } 
        else 
            /* -1 if no codes of this length */
            maxcode[l] = (unsigned int) 0xFFFFFFFF;	
    }
    numsymbols = p0;

    /* ensures jpeg_huff_decode terminates */
    maxcode[17] = (unsigned int) 0xFFFFFFFF; 


    /* Compute lookahead tables to speed up decoding.
     * First we set all the table entries to 0, indicating "too long";
     * then we iterate through the Huffman codes that are short enough and
     * fill in all the entries that correspond to bit sequences starting
     * with that code.
     */

    memset(look_nbits, 0, sizeof(look_nbits));

    p0 = 0;
    for (l = 1; l <= EXIF_HUFF_LOOKAHEAD; l++) 
    {
        for (i = 1; i <= num_symbols[l]; i++, p0++) 
        {
            /* l = current code's length,
               p0 = its index in huffcode[] & huffval[]. */
            /* Generate left-justified code followed by all
               possible bit sequences */
            lookbits = huffcode[p0] << (EXIF_HUFF_LOOKAHEAD-l);
            for (p1 = 1 << (EXIF_HUFF_LOOKAHEAD-l); p1 > 0; p1--) 
            {
	            look_nbits[lookbits] = l;
	            look_sym[lookbits] = symbol_values[p0];
	            lookbits++;
            }
        }
    }

    /* Validate symbols as being reasonable.
     * For AC tables, we make no check, but accept all byte values 0..255.
     * For DC tables, we require the symbols to be in range 0..15.
     * (Tighter bounds could be applied depending on the data depth and mode,
     * but this is sufficient to ensure safe decoding.)
     */
    if (mTableType == EXIF_HUFF_DC) 
    {
        for (i = 0; i < numsymbols; i++) 
            if (symbol_values[i] > 15)
	            status = EXIF_JPEG_HUFFMAN_TABLE_ERROR;
    }

    return status;
}

ExifStatus ExifJpegHUFFTable::setCustomTables(unsigned char bits[], 
                                              unsigned short bits_size,
                                              unsigned char codes[],
                                              unsigned short codes_size)
{
    ExifStatus err = EXIF_OK;
    int i;
    if (bits_size <= 17)
        for (i=0;i<bits_size;i++) num_symbols[i] = bits[i];
    else
        err = EXIF_ERROR;
        
    if ((err == EXIF_OK) && (codes_size <= 256)) 
        for (i=0;i<codes_size;i++) symbol_values[i] = codes[i];
    else
        err = EXIF_ERROR;
        
    return err;
}
}