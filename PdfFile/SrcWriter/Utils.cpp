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
#include "Utils.h"
#include <vector>

namespace PdfWriter
{
	BYTE* MemCpy(BYTE* pDst, const BYTE *pSrc, unsigned int unLen)
	{
		if (unLen > 0)
			memcpy(pDst, pSrc, unLen);

		return pDst;
	}
	int   StrLen(const char* sString, int nMaxLen)
	{
		int nLen = 0;

		if (!sString)
			return 0;

		while (*sString != 0 && (nMaxLen < 0 || nLen < nMaxLen))
		{
			sString++;
			nLen++;
		}

		return nLen;
	}
	BYTE* StrCpy(char* sDst, const char* sSrc, char* pEnd)
	{
		if (NULL != sSrc)
		{
			while (pEnd > sDst && *sSrc != 0)
				*sDst++ = *sSrc++;
		}

		*sDst = 0;

		return (BYTE*)sDst;
	}
	void  MemSet(void *pBuf, BYTE nChar, unsigned int unLen)
	{
		memset(pBuf, nChar, unLen);
	}
	char* ItoA  (char *str, int nVal, char *eptr)
	{
		char* sTemp;
		char sBuf[INT_LEN + 1];

		if (nVal < 0)
		{
			if (nVal < LIMIT_MIN_INT)
				nVal = LIMIT_MIN_INT;
			*str++ = '-';
			nVal = -nVal;
		}
		else if (nVal > LIMIT_MAX_INT)
		{
			nVal = LIMIT_MAX_INT;
		}
		else if (nVal == 0)
		{
			*str++ = '0';
		}

		sTemp = sBuf + INT_LEN;
		*sTemp-- = 0;

		while (nVal > 0)
		{
			*sTemp = (char)(nVal % 10) + '0';
			nVal /= 10;
			sTemp--;
		}

		sTemp++;
		while (str < eptr && *sTemp != 0)
			*str++ = *sTemp++;

		*str = 0;

		return str;
	}
	char* ItoA2 (char  *str, unsigned int nVal, unsigned int nLen)
	{
		char* sT;
		char* sU;

		if (nVal > LIMIT_MAX_INT)
			nVal = LIMIT_MAX_INT;

		sU = str + nLen - 1;
		*sU = 0;
		sT = sU - 1;
		while (nVal > 0 && sT >= str)
		{
			*sT = (char)(nVal % 10) + '0';
			nVal /= 10;
			sT--;
		}

		while (str <= sT)
			*sT-- = '0';

		return str + nLen - 1;
	}
	int   StrCmp(const char* s1, const char* s2)
	{
		if (!s1 || !s2)
		{
			if (!s1 && !s2)
				return 0;
			if (!s1 && s2)
				return -1;
			else
				return 1;
		}

		while (*s1 == *s2)
		{
			s1++;
			s2++;
			if (*s1 == 0 || *s2 == 0)
				break;
		}

		return (BYTE)*s1 - (BYTE)*s2;
	}
	char* FtoA  (char* sDst, double dVal, char* pEnd)
	{
		int nNPartVal = 0;
		int nFPartVal = 0;
		char sBuf[REAL_LEN + 1];
		char* sptr = sDst;
		char* sTemp;

		unsigned int nIndex = 0;

		if (dVal > LIMIT_MAX_REAL)
			dVal = LIMIT_MAX_REAL;
		else if (dVal < LIMIT_MIN_REAL)
			dVal = LIMIT_MIN_REAL;

		sTemp = sBuf + REAL_LEN;
		*sTemp-- = 0;

		if (dVal < 0)
		{
			*sDst++ = '-';
			dVal = -dVal;
		}

		// разделяем целую и дробную части
		nNPartVal = (int)(dVal + 0.000005);
		nFPartVal = (int)((float)(dVal - nNPartVal + 0.000005) * 100000);

		// пишем дробную часть
		for (nIndex = 0; nIndex < 5; nIndex++)
		{
			*sTemp = (char)(nFPartVal % 10) + '0';
			nFPartVal /= 10;
			sTemp--;
		}

		// пишем целую часть
		*sTemp-- = '.';
		*sTemp = '0';
		if (nNPartVal == 0)
			sTemp--;

		while (nNPartVal > 0)
		{
			*sTemp = (char)(nNPartVal % 10) + '0';
			nNPartVal /= 10;
			sTemp--;
		}

		sTemp++;

		while (sDst <= pEnd && *sTemp != 0)
			*sDst++ = *sTemp++;
		sDst--;


		// TODO: при избавлении от нулей при сдвиге конец строки тоже нужно чистить
		// пример число -00.90123 результат "-0.901234"

		while (sDst > sptr)
		{
			if (*sDst == '0')
				*sDst = 0;
			else {
				if (*sDst == '.')
					*sDst = 0;
				break;
			}
			sDst--;
		}

		return (*sDst == 0) ? sDst : ++sDst;
	}
	void  UIntChangeBit(unsigned int& nValue, short nBit)
	{
		// работаем только с 4-байтовыми числами
		if (nBit < 0 || nBit > 31)
			return;

		unsigned int unBitNum = 1 << nBit;
		if (nValue & unBitNum)
			nValue ^= unBitNum;
		else
			nValue |= unBitNum;
	}
}
