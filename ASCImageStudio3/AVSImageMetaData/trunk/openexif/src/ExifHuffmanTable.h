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
#ifndef EXIFHUFFMANTABLE_H
#define EXIFHUFFMANTABLE_H

#include "ExifErrorCodes.h"


typedef enum { EXIF_HUFF_AC, EXIF_HUFF_DC } ExifHuffmanTableType;

/* # of bits of lookahead */
#define EXIF_HUFF_LOOKAHEAD 8       
 
namespace Exif 
{

//
//      A JPEG Huffman table. sized for 12-bit DCT compression.
//      (256 vs.172 entries)
//
class ExifJpegHUFFTable
{ 
    public:
        ExifJpegHUFFTable(ExifHuffmanTableType t = EXIF_HUFF_AC)
        : mTableType(t) {};
        
        /* number of symbols of each length 1->16 valid.  */
        unsigned char num_symbols[17];       

        /* values for symbols in order specified by JPEG. */
        unsigned char symbol_values[256];    
        
        ExifHuffmanTableType mTableType;
        
        // Routine to take the huffman table data a the JPG file 
        // and derive a usable huffman table
        ExifStatus deriveTable();
        
        // Set a custom already-encoded table
        ExifStatus setCustomTables(unsigned char bits[], 
                                   unsigned short bits_size,
                                   unsigned char codes[],
                                   unsigned short codes_size);
                                   
        // Derived values:
        
        /* huffval[] offset for codes of length k */
        /* valoffset[k] = huffval[] index of 1st symbol of code length k, less
         * the smallest code of length k; so given a code of length k, the
         * corresponding symbol is huffval[code + valoffset[k]]
         */
        unsigned short valoffset[17];

        /* largest code of length k (-1 if none) */
        /* (maxcode[17] is a sentinel to ensure jpeg_huff_decode terminates) */
        unsigned int maxcode[18];

        /* Lookahead tables: indexed by the next HUFF_LOOKAHEAD bits of
         * the input data stream.  If the next Huffman code is no more
         * than HUFF_LOOKAHEAD bits long, we can obtain its length and
         * the corresponding symbol directly from these tables.
         */
        unsigned char  look_sym[1<<EXIF_HUFF_LOOKAHEAD];
        unsigned int look_nbits[1<<EXIF_HUFF_LOOKAHEAD];
};
}
#endif
