// Base64_RC4.h : implementation of the Base64 and RC4 algorithm class
//
/*
 * Base64_RC4.h  - Copyright (C) 2006 Jerry Jiang
 *
 * Email  :jerryJBL@gmail.com
 **********************************************************************
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * Version 1.0
 * Date:April 2009
  **********************************************************************
 *
 */

#ifndef _BASE64_RC4_
#define _BASE64_RC4_

const unsigned char B64_offset[256] =
{
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
	64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
	64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};
const char base64_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

class CBase64
{
public:
	CBase64(){}

	char *Encrypt(const char * srcp, int len, char * dstp)
	{
		register int i = 0;
		char *dst = dstp;

		for (i = 0; i < len - 2; i += 3)
		{
			*dstp++ = *(base64_map + ((*(srcp+i)>>2)&0x3f));
			*dstp++ = *(base64_map + ((*(srcp+i)<< 4)&0x30 | (*(srcp+i+1)>>4)&0x0f ));
			*dstp++ = *(base64_map + ((*(srcp+i+1)<<2)&0x3C | (*(srcp+i+2)>>6)&0x03));
			*dstp++ = *(base64_map + (*(srcp+i+2)&0x3f));
		}
		srcp += i;
		len -= i;

		if(len & 0x02 ) /* (i==2) 2 bytes left,pad one byte of '=' */
		{      
			*dstp++ = *(base64_map + ((*srcp>>2)&0x3f));
			*dstp++ = *(base64_map + ((*srcp<< 4)&0x30 | (*(srcp+1)>>4)&0x0f ));
			*dstp++ = *(base64_map + ((*(srcp+1)<<2)&0x3C) );
			*dstp++ = '=';
		}
		else if(len & 0x01 )  /* (i==1) 1 byte left,pad two bytes of '='  */
		{ 
			*dstp++ = *(base64_map + ((*srcp>>2)&0x3f));
			*dstp++ = *(base64_map + ((*srcp<< 4)&0x30));
			*dstp++ = '=';
			*dstp++ = '=';
		}

		*dstp = '\0';

		return dst;
	}

	void *Decrypt(const char * srcp, int len, char * dstp)
	{
		register int i = 0;
		void *dst = dstp;

		while(i < len)
		{
			*dstp++ = (B64_offset[*(srcp+i)] <<2 | B64_offset[*(srcp+i+1)] >>4);
			*dstp++ = (B64_offset[*(srcp+i+1)]<<4 | B64_offset[*(srcp+i+2)]>>2);
			*dstp++ = (B64_offset[*(srcp+i+2)]<<6 | B64_offset[*(srcp+i+3)] );
			i += 4;
		}
		srcp += i;
		
		if(*(srcp-2) == '=')  /* remove 2 bytes of '='  padded while encoding */
		{	 
			*(dstp--) = '\0';
			*(dstp--) = '\0';
		}
		else if(*(srcp-1) == '=') /* remove 1 byte of '='  padded while encoding */
			*(dstp--) = '\0';

		*dstp = '\0';

		return dst;
	};

	size_t B64_length(size_t len)
	{
		size_t  npad = len%3;
		size_t  size = (npad > 0)? (len +3-npad ) : len; // padded for multiple of 3 bytes
		return  (size*8)/6;
	}

	size_t Ascii_length(size_t len)
	{
		return  (len*6)/8;
	}
};

#define SWAP(a, b) ((a) ^= (b), (b) ^= (a), (a) ^= (b))

class CRC4 
{
private:
    unsigned char sbox[256];      /* Encryption array             */
    unsigned char key[256],k;     /* Numeric key values           */
	int  m, n, i, j, ilen;        /* Ambiguously named counters   */
public:

	  CRC4 () 
	  {
			memset(sbox,0,256);
			memset(key,0,256);
	  }
	  virtual ~CRC4 ()
	  {							
			memset(sbox,0,256);  /* remove Key traces in memory  */
			memset(key,0,256);   
	  }

	  char *Encrypt(char *pszText,const char *pszKey) 
	  {
		    i=0, j=0,n = 0;
			ilen = (int)strlen(pszKey);

			for (m = 0;  m < 256; m++)  /* Initialize the key sequence */
			{
				*(key + m)= *(pszKey + (m % ilen));
				*(sbox + m) = m;
			}

			for (m=0; m < 256; m++)
			{
				n = (n + *(sbox+m) + *(key + m)) &0xff;
				SWAP(*(sbox + m),*(sbox + n));
			}

			ilen = (int)strlen(pszText);
			for (m = 0; m < ilen; m++)
			{
				i = (i + 1) &0xff;
				j = (j + *(sbox + i)) &0xff;
				SWAP(*(sbox+i),*(sbox + j));  /* randomly Initialize the key sequence */
				k = *(sbox + ((*(sbox + i) + *(sbox + j)) &0xff ));
				if(k == *(pszText + m))       /* avoid '\0' beween the decoded text; */
					k = 0;
				*(pszText + m) ^=  k;
			}
			return pszText;

	  }

	  char *Decrypt(char *pszText,const char *pszKey)
	  {
		  return Encrypt(pszText,pszKey) ;  /* using the same function as encoding */
	  }
};

#endif