/*
 * ExifIJGWriteFrame.c
 *
 * The contents of this file are subject to the IBM Common Public
 * License Version 0.5 (the "License"); you may not use this file 
 * except in compliance with the License. You may obtain a copy of 
 * the License at http://oss.software.ibm.com/developerworks/
 * opensource/license-cpl.html
 * 
 * The Initial Developer of the Original Code is Eastman 
 * Kodak Company. Portions created by Kodak are 
 * Copyright (C) 2004 Eastman Kodak Company. All 
 * Rights Reserved. 
 *
 * Creation Date: 12/04/2004
 *
 * Original Author: 
 * Sam Fryer samuel.fryer@kodak.com
 *
 * Contributor(s): 
 *
 *
 * Portions of this code were taken from the IJG jpeg toolkit version 6b
 *         from the file "jcmarker.c" which had the copyright notice 
 *         reproduced below.
 */

/*
 * jcmarker.c
 *
 * Copyright (C) 1991-1998, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains routines to write JPEG datastream markers.
 */

#include "ExifComp.h"

#ifdef OPENEXIF_DCF_COMPLIANT

#ifndef OPENEXIF_NO_IJG

#define JPEG_INTERNALS
#include "OpenExif_jinclude.h"
#include "OpenExifjpeglib.h"
namespace Exif{

#define M_DQT          0xDB
#define M_DHT          0xC4

void openexif_ijg_emit_byte (j_compress_ptr cinfo, int val)
/* Emit a byte */
{
  struct openexif_jpeg_destination_mgr * dest = cinfo->dest;

  *(dest->next_output_byte)++ = (JOCTET) val;
  if (--dest->free_in_buffer == 0) {
    if (! (*dest->empty_output_buffer) (cinfo))
      ERREXIT(cinfo, JERR_CANT_SUSPEND);
  }
}


void openexif_ijg_emit_2bytes (j_compress_ptr cinfo, int value)
/* Emit a 2-byte integer; these are always MSB first in JPEG files */
{
  openexif_ijg_emit_byte(cinfo, (value >> 8) & 0xFF);
  openexif_ijg_emit_byte(cinfo, value & 0xFF);
}

void (*openexif_orig_write_frame_header) (j_compress_ptr cinfo);
void openexif_write_frame_header (j_compress_ptr cinfo)
{
  int ci;
  openexif_jpeg_component_info *compptr;
  int prec[MAX_COMPONENTS];
  int index=0;
  JQUANT_TBL * qtbl = NULL;
  JQUANT_TBL * qtblList[MAX_COMPONENTS];
  int i=0;
  int length = 2;

  memset(qtblList,0,MAX_COMPONENTS*sizeof(JQUANT_TBL *));
  
  for (ci = 0, compptr = cinfo->comp_info; 
       ci < cinfo->num_components;
       ci++, compptr++) 
  {
    index = compptr->quant_tbl_no;
    qtbl = cinfo->quant_tbl_ptrs[index];

    if (qtbl == NULL)
      ERREXIT1(cinfo, JERR_NO_QUANT_TABLE, index);
      
    i=0;
    while(( qtblList[i] != NULL ) && ( qtblList[i] != qtbl ))
      i++;
    
    if ( qtblList[i] == NULL )
    {
      qtblList[i] = qtbl;
      prec[index] = 0;
      for (i = 0; i < DCTSIZE2; i++) 
      {
        if (qtbl->quantval[i] > 255)
          prec[index] = 1;
      }
      length += 1 + DCTSIZE2 + DCTSIZE2*prec[index];
    }
  }
  
  /* emit the DQT marker */
  openexif_ijg_emit_byte(cinfo, 0xFF);
  openexif_ijg_emit_byte(cinfo, M_DQT);

  openexif_ijg_emit_2bytes(cinfo, length);


  for (ci = 0, compptr = cinfo->comp_info; 
       ci < cinfo->num_components;
       ci++, compptr++) 
  {
    index = compptr->quant_tbl_no;
    qtbl = cinfo->quant_tbl_ptrs[index];

    if (! qtbl->sent_table) 
    {

      openexif_ijg_emit_byte(cinfo, index + (prec[index]<<4));

      for (i = 0; i < DCTSIZE2; i++) 
      {

        /* The table entries must be emitted in zigzag order. */

        unsigned int qval = qtbl->quantval[openexif_jpeg_natural_order[i]];
        if (prec[index])
          openexif_ijg_emit_byte(cinfo, (int) (qval >> 8));
        openexif_ijg_emit_byte(cinfo, (int) (qval & 0xFF));
      }

      qtbl->sent_table = TRUE;
    }
  }

  /* Now call the original to post the rest of the frame ... */
  (*openexif_orig_write_frame_header)(cinfo);
}


int compute_dht_length(JHUFF_TBL * htbl)
{
  int length, i;
  
  length = 17;
  for (i = 1; i <= 16; i++)
    length += htbl->bits[i];    
    
  return length;
}

void compute_htblList(JHUFF_TBL * htbl,JHUFF_TBL * htblList[],
                      int i, int index[])
{
  if (! htbl->sent_table)
  {
    int k=0;
    while(( htblList[k] != NULL ) && ( htblList[k] != htbl ))
      k++;

    if (( htblList[k] == NULL ) && ( htbl != NULL ) && (! htbl->sent_table))
    {
      htblList[k] = htbl;
      index[k] = i;
    }
  }
}       

void (*openexif_orig_write_scan_header) (j_compress_ptr cinfo);
void openexif_write_scan_header (j_compress_ptr cinfo)
{

  int i,j,k,c;
  openexif_jpeg_component_info *compptr;
  int length = 2;
  JHUFF_TBL * htblList[MAX_COMPONENTS];
  int index[MAX_COMPONENTS];

  if (!cinfo->arith_code) 
  {
    memset(htblList,0,MAX_COMPONENTS*sizeof(JHUFF_TBL *));
    
    /* Emit Huffman tables.
     * Note that emit_dht() suppresses any duplicate tables.
     */
     /* 
     * First, get the tables involved, without duplication....
     */
    for (i = 0; i < cinfo->comps_in_scan; i++) 
    {
      JHUFF_TBL * htbl = NULL;
      
      compptr = cinfo->cur_comp_info[i];

      if (cinfo->progressive_mode) 
      {
	    /* Progressive mode: only DC or only AC tables are used in one scan */
	    if (cinfo->Ss == 0) {
	      if (cinfo->Ah == 0)	/* DC needs no table for refinement scan */
            htbl = cinfo->dc_huff_tbl_ptrs[compptr->dc_tbl_no];
            compute_htblList(htbl,htblList,compptr->dc_tbl_no,index);
	    } else {
          htbl = cinfo->ac_huff_tbl_ptrs[compptr->ac_tbl_no];
          compute_htblList(htbl,htblList,compptr->ac_tbl_no | 0x10,index);
	    }
      } else {
	    /* Sequential mode: need both DC and AC tables */
        htbl = cinfo->dc_huff_tbl_ptrs[compptr->dc_tbl_no];
        compute_htblList(htbl,htblList,compptr->dc_tbl_no,index);
        htbl = cinfo->ac_huff_tbl_ptrs[compptr->ac_tbl_no];
        compute_htblList(htbl,htblList,compptr->ac_tbl_no | 0x10,index);
      }
    }

     /* 
     * Second, compute the total length....
     */
    i=0;
    while (htblList[i] != NULL)
    {
      length += compute_dht_length(htblList[i]);
      i++;
    }
    
     /* 
     * Third, write out the marker and length ...
     */
    openexif_ijg_emit_byte(cinfo, 0xFF);
    openexif_ijg_emit_byte(cinfo, M_DHT);
    openexif_ijg_emit_2bytes(cinfo, length);

  
     /* 
     * Then, write the tables out....
     */
    i=0;
    while (htblList[i] != NULL)
    {
      JHUFF_TBL * htbl = htblList[i];

      openexif_ijg_emit_byte(cinfo, index[i]);

      for (j = 1; j <= 16; j++)
        openexif_ijg_emit_byte(cinfo, htbl->bits[j]);
  
      c = 0;
      for (j = 1; j <= 16; j++)
        for (k = 0; k < htbl->bits[j]; k++,c++)
          openexif_ijg_emit_byte(cinfo, htbl->huffval[c]);

      htbl->sent_table = TRUE;
      i++;
    }
  }

  /* Now call the original to post the rest of the frame ... */
  (*openexif_orig_write_scan_header)(cinfo);
}

}
#endif

#endif
