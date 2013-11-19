#ifndef _UTF8_H
#define _UTF8_H

static int MapUTF8(Unicode nUnicode, char *sBuffer, int nBufferSize) 
{
	if ( nUnicode <= 0x0000007f ) 
	{
		if ( nBufferSize < 1 ) 
		{
			return 0;
		}
		sBuffer[0] = (char)nUnicode;
		return 1;
	} 
	else if ( nUnicode <= 0x000007ff ) 
	{
		if ( nBufferSize < 2 ) 
		{
			return 0;
		}
		sBuffer[0] = (char)(0xc0 + (nUnicode >> 6));
		sBuffer[1] = (char)(0x80 + (nUnicode & 0x3f));
		return 2;
	} 
	else if ( nUnicode <= 0x0000ffff ) 
	{
		if ( nBufferSize < 3 ) 
		{
			return 0;
		}
		sBuffer[0] = (char)(0xe0 + (nUnicode >> 12));
		sBuffer[1] = (char)(0x80 + ((nUnicode >> 6) & 0x3f));
		sBuffer[2] = (char)(0x80 + (nUnicode & 0x3f));
		return 3;
	} 
	else if ( nUnicode <= 0x0010ffff ) 
	{
		if ( nBufferSize < 4 ) 
		{
			return 0;
		}
		sBuffer[0] = (char)(0xf0 + (nUnicode >> 18));
		sBuffer[1] = (char)(0x80 + ((nUnicode >> 12) & 0x3f));
		sBuffer[2] = (char)(0x80 + ((nUnicode >> 6) & 0x3f));
		sBuffer[3] = (char)(0x80 + (nUnicode & 0x3f));
		return 4;
	} 
	else 
	{
		return 0;
	}
}

static int MapUCS2(Unicode nUnicode, char *sBuffer, int nBufferSize) 
{
	if ( nUnicode <= 0xffff ) 
	{
		if ( nBufferSize < 2 ) 
		{
			return 0;
		}
		sBuffer[0] = (char)((nUnicode >> 8) & 0xff);
		sBuffer[1] = (char)(nUnicode & 0xff);
		return 2;
	} 
	else 
	{
		return 0;
	}
}

#endif /* _UTF8_H */