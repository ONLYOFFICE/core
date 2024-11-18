/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#ifndef _HTMLTEXT_H
#define _HTMLTEXT_H

#include <vector>

namespace NSHtmlRenderer2
{

struct CHPos
{
	int nPage = -1;
	int nLine = -1;
	int nWord = -1;
	int nChar = -1;
};

struct CHSelect
{
	bool bIsSelection = false;
	CHPos oPos1;
	CHPos oPos2;
};

struct CHChar
{
	double m_dX;
	int nUnicode;
};

struct CHWord
{
	bool bIsSpace;
	double m_dX1, m_dX2;

	std::vector<CHChar*> m_arrChars;
};

struct CHLine
{
	double m_dX1, m_dY1, m_dX2, m_dY2;

	double m_ex;
	double m_ey;

	std::vector<CHWord*> m_arrWords;
};

struct CHPage
{
	std::vector<CHLine*> m_arrLines;
};

class CHText
{
public:
	CHText() {}
	CHText(unsigned int nPages) : m_arrPages(nPages, NULL) {}

public:
	void GetPage(unsigned int nPageIndex);

	CHPos GetNearestPos(unsigned int nPageIndex, double dX, double dY, bool bExcludeSpaces = false)
	{
		CHPos oRes;
		oRes.nPage = nPageIndex;

		CHPage* pPage = m_arrPages[nPageIndex];
		if (!pPage)
			return oRes;

		double dMinDistY = 0xFFFFFF;
		for (int iLine = 0; iLine < pPage->m_arrLines.size(); ++iLine)
		{
			CHLine* pLine = pPage->m_arrLines[iLine];
			if (pLine->m_ex == 1 && pLine->m_ey == 0)
			{ // Горизонтальная линия
				if (dY >= pLine->m_dY1 && dY <= pLine->m_dY2 && dX >= pLine->m_dX1 && dX <= pLine->m_dX2)
				{
					oRes.nLine = iLine;

					for (int iWord = 0; iWord < pLine->m_arrWords.size(); ++iWord)
					{
						CHWord* pWord = pLine->m_arrWords[iWord];
						if (dX >= pWord->m_dX1 && dX <= pWord->m_dX2)
						{
							if (bExcludeSpaces && pWord->bIsSpace)
							{
								if (pWord->m_dX2 - dX < dX - pWord->m_dX1)
								{
									oRes.nWord = iWord + 1;
									oRes.nChar = 0;
								}
								else
								{
									oRes.nWord = iWord - 1;
									oRes.nChar = pLine->m_arrWords[iWord - 1]->m_arrChars.size();
								}
								return oRes;
							}

							oRes.nWord = iWord;

							int iChar = 1;
							for (; iChar < pWord->m_arrChars.size(); ++iChar)
							{
								CHChar* pChar = pWord->m_arrChars[iWord];
								if (pChar->m_dX > dX)
								{
									oRes.nChar = --iChar;
									return oRes;
								}
							}
						}
					}
				}

				double dDistY = abs(dY - pLine->m_dY1);
				if (dDistY < dMinDistY)
				{
					dMinDistY = dDistY;
					oRes.nLine = iLine;

					if (dX < pLine->m_dX1)
						oRes.nWord = 0;
					else if (dX > pLine->m_dX2)
						oRes.nWord = pLine->m_arrWords.size() - 1;
					else
					{
						for (int iWord = 0; iWord < pLine->m_arrWords.size(); ++iWord)
						{
							CHWord* pWord = pLine->m_arrWords[iWord];
							if (dX >= pWord->m_dX1 && dX <= pWord->m_dX2)
							{
								if (bExcludeSpaces && pWord->bIsSpace)
								{
									if (pWord->m_dX2 - dX < dX - pWord->m_dX1)
									{
										oRes.nWord = iWord + 1;
										oRes.nChar = 0;
									}
									else
									{
										oRes.nWord = iWord - 1;
										oRes.nChar = pLine->m_arrWords[iWord - 1]->m_arrChars.size();
									}
									break;
								}

								oRes.nWord = iWord;

								int iChar = 1;
								for (; iChar < pWord->m_arrChars.size(); ++iChar)
								{
									CHChar* pChar = pWord->m_arrChars[iWord];
									if (pChar->m_dX > dX)
									{
										oRes.nChar = --iChar;
										break;
									}
								}
								break;
							}
						}
					}
				}
			}
			else
			{

			}
		}

		return oRes;
	}
	void SelectWholeWord(unsigned int nPageIndex, double dX, double dY)
	{
		CHPos oNearesPos = GetNearestPos(nPageIndex, dX, dY, true);
		if (oNearesPos.nChar < 0)
			return;

		m_oSelect.bIsSelection = true;
		m_oSelect.oPos1.nPage = nPageIndex;
		m_oSelect.oPos2.nPage = nPageIndex;
		m_oSelect.oPos1.nLine = oNearesPos.nLine;
		m_oSelect.oPos2.nLine = oNearesPos.nLine;
		m_oSelect.oPos1.nWord = oNearesPos.nWord;
		m_oSelect.oPos2.nWord = oNearesPos.nWord;

		CHWord* pWord = m_arrPages[nPageIndex]->m_arrLines[oNearesPos.nLine]->m_arrWords[oNearesPos.nWord];
		bool bIsOnSpace = pWord->m_arrChars[oNearesPos.nChar]->nUnicode == 32;
		bool bIsOnPuctuation = false;
	}
	CHSelect SelectWholeRow(unsigned int nPageIndex, double dX, double dY);
	CHSelect SelectWholePage(unsigned int nPageIndex);

private:
	CHSelect m_oSelect;
	std::vector<CHPage*> m_arrPages;
};

} // namespace HText

#endif // _HTMLTEXT_H
