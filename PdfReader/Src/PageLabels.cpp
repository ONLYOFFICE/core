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

#include "PageLabels.h"
#include "Array.h"
#include "Dict.h"
#include "XRef.h"
#include "List.h"
#include "StringExt.h"

namespace PdfReader
{
	PageLabels::PageLabels(Object* pPageLabelsObject, XRef* pXRef)
	{
		m_pNums = NULL;

		if (!pPageLabelsObject || !pPageLabelsObject->IsDict())
			return;

		Dict* pDict = pPageLabelsObject->GetDict();

		Object oNums;
		if (pDict->Search("Nums", &oNums)->IsArray())
		{
			int nCount = oNums.ArrayGetLength();
			int nIndex = 0;

			while (nIndex < nCount)
			{
				Object oEntry;
				oNums.ArrayGet(nIndex++, &oEntry);

				int nPageNum = 0, nStart = 1;
				StringExt* sPrefix = NULL;
				PageLabelStyle eLabelStyle = pageLabelNone;

				if (oEntry.IsInt())
					nPageNum = oEntry.GetInt();

				oEntry.Free();

				if (nIndex >= nCount)
					break;

				oNums.ArrayGet(nIndex++, &oEntry);
				if (!oEntry.IsDict())
					break;

				Dict* pEntryDict = oEntry.GetDict();

				Object oEntry2;
				pEntryDict->Search("S", &oEntry2);
				if (oEntry2.IsName())
				{
					if (oEntry2.IsName("D"))
						eLabelStyle = pageLabelDecimal;
					else if (oEntry2.IsName("R"))
						eLabelStyle = pageLabelUpperRoman;
					else if (oEntry2.IsName("r"))
						eLabelStyle = pageLabelLowerRoman;
					else if (oEntry2.IsName("A"))
						eLabelStyle = pageLabelUpperLetter;
					else if (oEntry2.IsName("a"))
						eLabelStyle = pageLabelLowerLetter;
				}

				oEntry2.Free();

				pEntryDict->Search("P", &oEntry2);
				if (oEntry2.IsString())
					sPrefix = oEntry2.GetString()->Copy();

				oEntry2.Free();

				pEntryDict->Search("St", &oEntry2);
				if (oEntry2.IsInt())
					nStart = oEntry2.GetInt();

				oEntry2.Free();
				oEntry.Free();

				if (!m_pNums)
					m_pNums = new CList();

				m_pNums->Append(new PageLabelsEntry(nPageNum, nStart, eLabelStyle, sPrefix));
			}
		}
	}
	PageLabels::~PageLabels()
	{
		if (m_pNums)
		{
			DeleteCList(m_pNums, PageLabelsEntry);
		}
	}
	StringExt* PageLabels::GetLabel(int nPageIndex)
	{
		if (!m_pNums)
			return NULL;

		StringExt* seResult = new StringExt();

		for (int nIndex = m_pNums->GetLength() - 1; nIndex >= 0; --nIndex)
		{
			PageLabelsEntry* pEntry = (PageLabelsEntry*)m_pNums->GetByIndex(nIndex);
			if (pEntry->m_nPageIndex <= nPageIndex)
			{
				if (pEntry->m_sPrefix)
					seResult->Append(pEntry->m_sPrefix);

				int nValue = (nPageIndex - pEntry->m_nPageIndex) + pEntry->m_nStart;

				if (pageLabelDecimal == pEntry->m_eStyle)
				{
					seResult->AppendFormat("{0:d}", nValue);
				}
				else if (pageLabelLowerLetter == pEntry->m_eStyle || pageLabelUpperLetter == pEntry->m_eStyle)
				{
					// Формат: a,..,z,aa,..,zz,aaa,...,zzz,...
					int nNum = nValue - 1;

					int nCount = (nNum - nNum % 26) / 26;
					int nOst   = nNum % 26;

					int nChar;
					if (pageLabelLowerLetter ==  pEntry->m_eStyle)
						nChar = nOst + 97;
					else
						nChar = nOst + 65;

					for (int nCharIndex = 0; nCharIndex < nCount + 1; ++nCharIndex)
						seResult->Append(nChar);
				}
				else if (pageLabelLowerRoman == pEntry->m_eStyle || pageLabelUpperRoman == pEntry->m_eStyle)
				{
					int nNum = nValue;

					// Переводим число Num в римскую систему исчисления
					const char *pRIMS[14] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I", " "};
					const char *pRims[14] = {"m", "cm", "d", "cd", "c", "xc", "l", "xl", "x", "ix", "v", "iv", "i", " "};

					int pVals[14] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1, 0};

					int nPos = 0;
					while (nNum > 0)
					{
						while (pVals[nPos] <= nNum)
						{
							seResult->Append(pageLabelLowerRoman == pEntry->m_eStyle ? pRims[nPos] : pRIMS[nPos]);
							nNum -= pVals[nPos];
						}

						nPos++;

						if (nPos >= 14)
							break;
					}
					break;
				}

				break;
			}
		}

		return seResult;
	}

	PageLabelsEntry::PageLabelsEntry(int nPageIndex, int nStart, PageLabelStyle eStyle, StringExt* sPrefix)
	{
		m_nPageIndex = nPageIndex;
		m_nStart     = nStart;
		m_eStyle     = eStyle;
		m_sPrefix    = sPrefix;
	}
	PageLabelsEntry::~PageLabelsEntry()
	{
		if (m_sPrefix)
			delete m_sPrefix;
	}
}
