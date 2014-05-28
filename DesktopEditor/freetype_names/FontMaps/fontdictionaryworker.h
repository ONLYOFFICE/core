#ifndef _FONT_DICTIONARY_WORKER_H

#include "FontDictionary.h"

namespace NSFontDictionary
{
	static bool CorrectParamsFromDictionary(CString& wsFamily, BOOL& bBold, BOOL& bItalic, BOOL& bFixedWidth, BYTE* pPanose, 
		ULONG& ulRange1, ULONG& ulRange2, ULONG& ulRange3, ULONG& ulRange4, ULONG& ulCodeRange1, ULONG& ulCodeRange2,
		USHORT& usWeight, USHORT& usWidth, SHORT& sFamilyClass,
		SHORT& shAvgCharWidth, SHORT& shAscent, SHORT& shDescent, SHORT& shLineGap, SHORT& shXHeight, SHORT& shCapHeight,
		BOOL& bIsStyle, BOOL& bIsFixed, BOOL& bIsPanose, BOOL& bIsRanges, 
		BOOL& bIsWeight, BOOL& bIsWidth, BOOL& bIsFamilyClass, 
		BOOL& bIsAvgWidth, BOOL& bIsAscent, BOOL& bIsDescent, BOOL& bIsLineGap, BOOL& bIsXHeight, BOOL& bIsCapHeight)
	{
		int nNameLen = wsFamily.GetLength();
		if (nNameLen == 0)
			return false;

		bool bIsAscii = true;
		wchar_t* pName = wsFamily.GetBuffer();

		for (int i = 0; i < nNameLen; ++i)
		{
			if (pName[i] > 255 || pName[i] < 0)
			{
				bIsAscii = false;
				break;
			}
		}

		const FD_Font* pFont = NULL;

		if (bIsAscii)
		{
			int nStartIndex = FD_Ascii_Names_Offsets[pName[0]];

			if (-1 != nStartIndex)
			{
				int nIndex = -1;
				for (int i = nStartIndex; i < FONTS_DICT_ASCII_NAMES_COUNT; ++i)
				{
					const char* _name = FD_Ascii_Names[i].m_name;
					if (pName[0] != (wchar_t)_name[0])
						break;

					bool bIsEqual = true;
					for (int j = 1; j < nNameLen; ++j)
					{
						if ('\0' == _name[j] && j != (nNameLen - 1))
						{
							bIsEqual = false;
							break;
						}
						if (pName[j] != (wchar_t)_name[j])
						{
							bIsEqual = false;
							break;
						}
					}

					if (bIsEqual)
					{
						nIndex = i;
						break;
					}
				}

				if (nIndex != -1)
				{
					const FD_FontMapRec* pRec = &FD_Ascii_Names[nIndex];
					int nFontIndex = -1;

					int nStyle = 0;
					if (bIsStyle && bItalic)
						nStyle |= 1;
					if (bIsStyle && bBold)
						nStyle |= 2;

					switch (nStyle)
					{
					case 1:
						{
							if (pRec->m_index_i != -1)
								nFontIndex = pRec->m_index_i;
							else if (pRec->m_index_bi != -1)
								nFontIndex = pRec->m_index_bi;
							else if (pRec->m_index_r != -1)
								nFontIndex = pRec->m_index_r;
							else
								nFontIndex = pRec->m_index_b;
							break;
						}
					case 2:
						{
							if (pRec->m_index_b != -1)
								nFontIndex = pRec->m_index_b;
							else if (pRec->m_index_bi != -1)
								nFontIndex = pRec->m_index_bi;
							else if (pRec->m_index_r != -1)
								nFontIndex = pRec->m_index_r;
							else
								nFontIndex = pRec->m_index_i;
							break;
						}
					case 3:
						{
							if (pRec->m_index_bi != -1)
								nFontIndex = pRec->m_index_bi;
							else if (pRec->m_index_b != -1)
								nFontIndex = pRec->m_index_b;
							else if (pRec->m_index_i != -1)
								nFontIndex = pRec->m_index_i;
							else
								nFontIndex = pRec->m_index_r;
							break;
						}
					case 0:
					default:
						{
							if (pRec->m_index_r != -1)
								nFontIndex = pRec->m_index_r;
							else if (pRec->m_index_i != -1)
								nFontIndex = pRec->m_index_i;
							else if (pRec->m_index_b != -1)
								nFontIndex = pRec->m_index_b;
							else
								nFontIndex = pRec->m_index_bi;
							break;
						}
					}

					if (nFontIndex != -1)
						pFont = &FD_Ascii_Files[nFontIndex];
				}
			}
		}
		else
		{
			int nIndex = -1;
			for (int i = 0; i < FONTS_DICT_UNICODE_NAMES_COUNT; ++i)
			{
				const wchar_t* _name = FD_Unicode_Names[i].m_name;
				if (pName[0] != _name[0])
					continue;

				bool bIsEqual = true;
				for (int j = 1; j < nNameLen; ++j)
				{
					if (((wchar_t)'\0') == _name[j] && j != (nNameLen - 1))
					{
						bIsEqual = false;
						break;
					}
					if (pName[j] != _name[j])
					{
						bIsEqual = false;
						break;
					}
				}

				if (bIsEqual)
				{
					nIndex = i;
					break;
				}
			}

			if (nIndex != -1)
			{
				const FD_FontMapRecW* pRec = &FD_Unicode_Names[nIndex];
				int nFontIndex = -1;

				int nStyle = 0;
				if (bIsStyle && bItalic)
					nStyle |= 1;
				if (bIsStyle && bBold)
					nStyle |= 2;

				switch (nStyle)
				{
				case 1:
					{
						if (pRec->m_index_i != -1)
							nFontIndex = pRec->m_index_i;
						else if (pRec->m_index_bi != -1)
							nFontIndex = pRec->m_index_bi;
						else if (pRec->m_index_r != -1)
							nFontIndex = pRec->m_index_r;
						else
							nFontIndex = pRec->m_index_b;
						break;
					}
				case 2:
					{
						if (pRec->m_index_b != -1)
							nFontIndex = pRec->m_index_b;
						else if (pRec->m_index_bi != -1)
							nFontIndex = pRec->m_index_bi;
						else if (pRec->m_index_r != -1)
							nFontIndex = pRec->m_index_r;
						else
							nFontIndex = pRec->m_index_i;
						break;
					}
				case 3:
					{
						if (pRec->m_index_bi != -1)
							nFontIndex = pRec->m_index_bi;
						else if (pRec->m_index_b != -1)
							nFontIndex = pRec->m_index_b;
						else if (pRec->m_index_i != -1)
							nFontIndex = pRec->m_index_i;
						else
							nFontIndex = pRec->m_index_r;
						break;
					}
				case 0:
				default:
					{
						if (pRec->m_index_r != -1)
							nFontIndex = pRec->m_index_r;
						else if (pRec->m_index_i != -1)
							nFontIndex = pRec->m_index_i;
						else if (pRec->m_index_b != -1)
							nFontIndex = pRec->m_index_b;
						else
							nFontIndex = pRec->m_index_bi;
						break;
					}
				}

				if (nFontIndex != -1)
					pFont = &FD_Ascii_Files[nFontIndex];
			}
		}

		if (NULL == pFont)
			return false;

		// name - делаем ascii, чтобы сработал подбор
		wsFamily = (CString(pFont->m_name));

		// fixed
		bIsFixed = TRUE;
		bFixedWidth = pFont->m_bIsFixed == 1 ? TRUE : FALSE;

		// panose
		bIsPanose = TRUE;
		memcpy(pPanose, pFont->m_aPanose, 10);

		// ranges
		bIsRanges = TRUE;
		ulRange1 = pFont->m_ulUnicodeRange1;
		ulRange2 = pFont->m_ulUnicodeRange2;
		ulRange3 = pFont->m_ulUnicodeRange3;
		ulRange4 = pFont->m_ulUnicodeRange4;
		ulCodeRange1 = pFont->m_ulCodePageRange1;
		ulCodeRange2 = pFont->m_ulCodePageRange2;

		bIsWeight = TRUE;
		bIsWeight = pFont->m_usWeigth;

		bIsWidth = TRUE;
		usWidth = pFont->m_usWidth;

		bIsAvgWidth = TRUE;
		shAvgCharWidth = pFont->m_shAvgCharWidth;

		bIsAscent = TRUE;
		shAscent = pFont->m_shAscent;

		bIsDescent = TRUE;
		shDescent = pFont->m_shDescent;

		bIsXHeight = TRUE;
		shXHeight = pFont->m_shXHeight;

		bIsCapHeight = TRUE;
		shCapHeight = pFont->m_shCapHeight;

		return true;
	}
}

#endif /* _FONT_DICTIONARY_WORKER_H */