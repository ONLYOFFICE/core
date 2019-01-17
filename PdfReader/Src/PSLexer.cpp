/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "PSLexer.h"

namespace PdfReader
{
	// '1' - означает пробел. '1' или '2' означает, что данным символом заканчивается
	// имя или команда.

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

	PSLexer::PSLexer(int(*GetCharFunc)(void *), void *pData)
	{
		GetCharFunction = GetCharFunc;
		m_pData = pData;
		m_nCharBuffer = -1;
	}

	PSLexer::~PSLexer()
	{
	}

	bool PSLexer::GetToken(char *sBuffer, int nSize, int *pnLength)
	{
		int nChar = 0;

		// Пропускаем пробелы и комментарии
		bool bComment = false;
		while (1)
		{
			if ((nChar = GetChar()) == EOF)
			{
				sBuffer[0] = '\0';
				*pnLength = 0;
				return false;
			}
			if (bComment)
			{
				if (nChar == '\x0a' || nChar == '\x0d')
				{
					bComment = false;
				}
			}
			else if (nChar == '%')
			{
				bComment = true;
			}
			else if (c_sSpecialChars[nChar] != 1)
			{
				break;
			}
		}
		int nLen = 0;
		sBuffer[nLen++] = nChar;
		if (nChar == '(')
		{
			bool bBackslash = false;
			while ((nChar = LookChar()) != EOF)
			{
				if (nLen < nSize - 1)
				{
					sBuffer[nLen++] = nChar;
				}
				GetChar();
				if (nChar == '\\')
				{
					bBackslash = true;
				}
				else if (!bBackslash && nChar == ')')
				{
					break;
				}
				else
				{
					bBackslash = false;
				}
			}
		}
		else if (nChar == '<')
		{
			while ((nChar = LookChar()) != EOF)
			{
				GetChar();
				if (nLen < nSize - 1 && c_sSpecialChars[nChar] != 1)
				{
					sBuffer[nLen++] = nChar;
				}
				if (nChar == '>')
				{
					break;
				}
			}
		}
		else if (nChar != '[' && nChar != ']')
		{
			while ((nChar = LookChar()) != EOF && !c_sSpecialChars[nChar])
			{
				GetChar();
				if (nLen < nSize - 1)
				{
					sBuffer[nLen++] = nChar;
				}
			}
		}
		sBuffer[nLen] = '\0';
		*pnLength = nLen;

		return true;
	}

	int PSLexer::LookChar()
	{
		if (m_nCharBuffer < 0)
		{
			m_nCharBuffer = (*GetCharFunction)(m_pData);
		}
		return m_nCharBuffer;
	}

	int PSLexer::GetChar()
	{
		if (m_nCharBuffer < 0)
		{
			m_nCharBuffer = (*GetCharFunction)(m_pData);
		}
		int nChar = m_nCharBuffer;
		m_nCharBuffer = -1;
		return nChar;
	}
}