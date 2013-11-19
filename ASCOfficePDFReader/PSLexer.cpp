#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include "PSLexer.h"

//------------------------------------------------------------------------

// '1' - означает пробел. '1' или '2' означает, что данным символом заканчиваетс€
// им€ или команда.

static char c_sSpecialChars[256] = 
{
//  0x 1x 2x 3x 4x 5x 6x 7x 8x 9x ax bx cx dx ex fx

	1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0,   // 0x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1x
	1, 0, 0, 0, 0, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2,   // 2x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0,   // 3x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 4x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0,   // 5x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 6x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0,   // 7x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 8x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 9x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // ax
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // bx
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // cx
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // dx
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // ex
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0    // fx
};

//------------------------------------------------------------------------

PSLexer::PSLexer(int (*GetCharFunc)(void *), void *pData) 
{
	GetCharFunction = GetCharFunc;
	m_pData = pData;
	m_nCharBuffer = -1;
}

PSLexer::~PSLexer() 
{
}

BOOL PSLexer::GetToken(char *sBuffer, int nSize, int *pnLength) 
{
	int nChar = 0;

	// ѕропускаем пробелы и комментарии
	BOOL bComment = FALSE;
	while (1) 
	{
		if ( ( nChar = GetChar() ) == EOF ) 
		{
			sBuffer[0] = '\0';
			*pnLength = 0;
			return FALSE;
		}
		if ( bComment ) 
		{
			if ( nChar == '\x0a' || nChar == '\x0d' ) 
			{
				bComment = FALSE;
			}
		} 
		else if ( nChar == '%' )
		{
			bComment = TRUE;
		} 
		else if ( c_sSpecialChars[nChar] != 1 ) 
		{
			break;
		}
	}
	int nLen = 0;
	sBuffer[nLen++] = nChar;
	if ( nChar == '(' ) 
	{
		BOOL bBackslash = FALSE;
		while ( ( nChar = LookChar() ) != EOF ) 
		{
			if ( nLen < nSize - 1 ) 
			{
				sBuffer[nLen++] = nChar;
			}
			GetChar();
			if ( nChar == '\\' ) 
			{
				bBackslash = TRUE;
			} 
			else if ( !bBackslash && nChar == ')' ) 
			{
				break;
			} 
			else 
			{
				bBackslash = FALSE;
			}
		}
	} 
	else if ( nChar == '<' ) 
	{
		while ( ( nChar = LookChar() ) != EOF ) 
		{
			GetChar();
			if ( nLen < nSize - 1 && c_sSpecialChars[nChar] != 1 ) 
			{
				sBuffer[nLen++] = nChar;
			}
			if ( nChar == '>' ) 
			{
				break;
			}
		}
	} 
	else if ( nChar != '[' && nChar != ']' ) 
	{
		while ( ( nChar = LookChar() ) != EOF && !c_sSpecialChars[nChar] ) 
		{
			GetChar();
			if ( nLen < nSize - 1 ) 
			{
				sBuffer[nLen++] = nChar;
			}
		}
	}
	sBuffer[nLen] = '\0';
	*pnLength = nLen;

	return TRUE;
}

int PSLexer::LookChar() 
{
	if ( m_nCharBuffer < 0 ) 
	{
		m_nCharBuffer = (*GetCharFunction)(m_pData);
	}
	return m_nCharBuffer;
}

int PSLexer::GetChar()
{
	if ( m_nCharBuffer < 0 ) 
	{
		m_nCharBuffer = (*GetCharFunction)(m_pData);
	}
	int nChar = m_nCharBuffer;
	m_nCharBuffer = -1;
	return nChar;
}
