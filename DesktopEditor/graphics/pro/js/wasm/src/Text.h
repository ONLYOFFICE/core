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
#ifndef _ASC_HTMLRENDERER_TEXT_H_
#define _ASC_HTMLRENDERER_TEXT_H_

#include <algorithm>

#include "serialize.h"
#include "../../../../structures.h"
#include "../../../Fonts.h"

namespace NSHtmlRenderer
{
	struct CHFontInfo
	{
		int m_lAscent;
		int m_lDescent;
		int m_lUnitsPerEm;

		CHFontInfo() : m_lAscent(0), m_lDescent(0), m_lUnitsPerEm(0) {}
		CHFontInfo(const CHFontInfo& oSrc) : m_lAscent(oSrc.m_lAscent), m_lDescent(oSrc.m_lDescent), m_lUnitsPerEm(oSrc.m_lUnitsPerEm) {}
		CHFontInfo& operator=(const CHFontInfo& oSrc)
		{
			m_lAscent = oSrc.m_lAscent;
			m_lDescent = oSrc.m_lDescent;
			m_lUnitsPerEm = oSrc.m_lUnitsPerEm;
			return *this;
		}
	};

	class CFontManagerWrapper
	{
	private:
		NSFonts::IFontManager* m_pManager;
	public:
		NSStructures::CFont* m_pFont;
		CHFontInfo m_oCurrentInfo;

	public:
		CFontManagerWrapper() : m_pManager(NULL) {}
		virtual ~CFontManagerWrapper()
		{
			RELEASEOBJECT(m_pManager);
		}

		void Init(NSFonts::IApplicationFonts* pApplicationFonts, int nCacheSize = 0)
		{
			RELEASEOBJECT(m_pManager);
			m_pManager = pApplicationFonts->GenerateFontManager();
			if (nCacheSize)
			{
				NSFonts::IFontsCache* pFontCache = NSFonts::NSFontCache::Create();
				pFontCache->SetStreams(pApplicationFonts->GetStreams());
				pFontCache->SetCacheSize(nCacheSize);
				m_pManager->SetOwnerCache(pFontCache);
			}
		}
		void SetStringGID(INT bGid)
		{
			m_pManager->SetStringGID(bGid);
		}
		void LoadCurrentFont()
		{
			if (m_pFont->Path.empty())
				LoadFontByName(m_pFont->Name, m_pFont->Size, m_pFont->GetStyle());
			else
				LoadFontByFile(m_pFont->Path, m_pFont->Size);
		}
		double MeasureString(const unsigned int* symbols, const int& count, double x, double y)
		{
			if (!m_pManager)
				return 0;

			m_pManager->LoadString1(symbols, count, (float)x, (float)y);
			TBBox _box = m_pManager->MeasureString2();

			return abs((_box.fMaxX - _box.fMinX) * 25.4 / 72.0);
		}

	private:
		inline void LoadFontByName(const std::wstring& strName, const double& dSize, const LONG& lStyle)
		{
			m_pManager->LoadFontByName(strName, dSize, lStyle, 72.0, 72.0);
			LoadFontMetrics();
		}
		inline void LoadFontByFile(const std::wstring& strPath, const double& dSize)
		{
			m_pManager->LoadFontFromFile(strPath, 0, dSize, 72.0, 72.0);
			LoadFontMetrics();
		}
		void LoadFontMetrics()
		{
			m_pManager->AfterLoad();
			m_oCurrentInfo.m_lAscent     = abs(m_pManager->GetAscender());
			m_oCurrentInfo.m_lDescent    = abs(m_pManager->GetDescender());
			m_oCurrentInfo.m_lUnitsPerEm = abs(m_pManager->GetUnitsPerEm());
		}
	};

	struct CHText
	{
		CFontManagerWrapper m_oFontManager;
		Aggplus::CMatrix* m_pTransform;
		Aggplus::CMatrix* m_pLastTransform;

		NSWasm::CHLine m_oLine;
		NSWasm::CData* m_pPageMeta;

		LONG m_nCountParagraphs;
		LONG m_nCountWords;
		LONG m_nCountSymbols;
		LONG m_nCountSpaces;

		CHText() : m_oFontManager(), m_oLine()
		{
			m_nCountParagraphs = 0;
			m_nCountWords = 0;
			m_nCountSymbols = 0;
			m_nCountSpaces = 0;
		}

		void ClearStatistics()
		{
			m_nCountParagraphs = 0;
			m_nCountWords = 0;
			m_nCountSymbols = 0;
			m_nCountSpaces = 0;
		}
		void ClosePage()
		{
			if (m_oLine.GetCountChars())
				DumpLine();
		}
		void CommandText(const int* pUnicodes, const int* pGids, const int& nCount, const double& x, const double& y, bool bIsDumpFont)
		{
			// 1) сначала определяем точку отсчета и направление baseline
			double _x1 = x;
			double _y1 = y;
			double _x2 = x + 1;
			double _y2 = y;
			m_pTransform->TransformPoint(_x1, _y1);
			m_pTransform->TransformPoint(_x2, _y2);

			double _k = 0;
			double _b = 0;
			bool _isConstX = false;
			if (fabs(_x1 - _x2) < 0.001)
			{
				_isConstX = true;
				_b = _x1;
			}
			else
			{
				_k = (_y1 - _y2) / (_x1 - _x2);
				_b = _y1 - _k * _x1;
			}

			double dAbsVec = sqrt((_x1 - _x2) * (_x1 - _x2) + (_y1 - _y2) * (_y1 - _y2));
			if (dAbsVec == 0)
				dAbsVec = 1;

			bool bIsNewLine = true;
			if (m_oLine.GetCountChars())
			{
				if (_isConstX && m_oLine.m_bIsConstX && fabs(_b - m_oLine.m_dB) < 0.001)
					bIsNewLine = false;
				else if (!_isConstX && !m_oLine.m_bIsConstX && fabs(_k - m_oLine.m_dK) < 0.001 && fabs(_b - m_oLine.m_dB) < 0.001)
					bIsNewLine = false;

				if (bIsNewLine) // не совпала baseline. поэтому просто скидываем линию в поток
					DumpLine();
			}

			// теперь нужно определить сдвиг по baseline относительно destination точки
			double dOffsetX = 0;
			LONG nCountChars = m_oLine.GetCountChars();
			if (0 == nCountChars)
			{
				m_oLine.m_bIsConstX = _isConstX;
				m_oLine.m_dK = _k;
				m_oLine.m_dB = _b;

				m_oLine.m_dX = _x1;
				m_oLine.m_dY = _y1;

				m_oLine.m_ex = (_x2 - _x1) / dAbsVec;
				m_oLine.m_ey = (_y2 - _y1) / dAbsVec;

				m_oLine.m_dEndX = _x1;
				m_oLine.m_dEndY = _y1;
			}
			else
			{
				double sx = _x1 - m_oLine.m_dEndX;
				double sy = _y1 - m_oLine.m_dEndY;
				double len = sqrt(sx*sx + sy*sy);

				if (sx * m_oLine.m_ex >= 0 && sy * m_oLine.m_ey >= 0)
				{
					// продолжаем линию
					dOffsetX = len;

					// теперь посмотрим, может быть нужно вставить пробел??
					NSWasm::CHChar* pLastChar = m_oLine.GetTail();
					if (dOffsetX > (pLastChar->width + 0.5))
					{
						// вставляем пробел. Пробел у нас будет не совсем пробел. А специфический
						NSWasm::CHChar* pSpaceChar = m_oLine.AddTail();
						pLastChar = &m_oLine.m_pChars[m_oLine.m_lCharsTail - 2];
						pSpaceChar->x = pLastChar->width;
						pSpaceChar->width = dOffsetX - pLastChar->width;
						pSpaceChar->unicode = 0xFFFF;
						dOffsetX -= pLastChar->width;
					}
				}
				else
				{
					// буква сдвинута влево относительно предыдущей буквы
					// на такую ситуацию реагируем просто - просто начинаем новую линию,
					// предварительно сбросив старую
					DumpLine();

					m_oLine.m_bIsConstX = _isConstX;

					m_oLine.m_dX = _x1;
					m_oLine.m_dY = _y1;

					m_oLine.m_dK = _k;
					m_oLine.m_dB = _b;

					m_oLine.m_ex = (_x2 - _x1) / dAbsVec;
					m_oLine.m_ey = (_y2 - _y1) / dAbsVec;
				}

				m_oLine.m_dEndX = _x1;
				m_oLine.m_dEndY = _y1;
			}

			if (!Aggplus::CMatrix::IsEqual(m_pLastTransform, m_pTransform, 0.001, true))
			{ // смотрим, совпадает ли главная часть матрицы
				bIsDumpFont = true;
				*m_pLastTransform = *m_pTransform;
				m_oLine.m_bIsSetUpTransform = true;
				m_oLine.m_sx  = m_pTransform->sx();
				m_oLine.m_shx = m_pTransform->shx();
				m_oLine.m_shy = m_pTransform->shy();
				m_oLine.m_sy  = m_pTransform->sy();
			}

			// все, baseline установлен. теперь просто продолжаем линию
			if (bIsDumpFont)
				m_oFontManager.LoadCurrentFont();

			double dKoef = m_oFontManager.m_pFont->Size * 25.4 / (72 * m_oFontManager.m_oCurrentInfo.m_lUnitsPerEm);
			double dAscender  = m_oFontManager.m_oCurrentInfo.m_lAscent  * dKoef * dAbsVec;
			double dDescender = m_oFontManager.m_oCurrentInfo.m_lDescent * dKoef * dAbsVec;

			if (m_oLine.m_dAscent < dAscender)
				m_oLine.m_dAscent = dAscender;
			if (m_oLine.m_dDescent < dDescender)
				m_oLine.m_dDescent = dDescender;

			const int* input = NULL;
			if (NULL != pGids)
			{
				input = pGids;
				m_oFontManager.SetStringGID(TRUE);
			}
			else
			{
				input = pUnicodes;
				m_oFontManager.SetStringGID(FALSE);
			}

			double dPlusOffset = 0;
			double dPrevW = dOffsetX;
			for (int i = 0; i < nCount; ++i)
			{
				double dW = m_oFontManager.MeasureString((const unsigned int*)(input + i), 1, 0, 0);

				NSWasm::CHChar* pChar = m_oLine.AddTail();
				pChar->unicode = pUnicodes[i];

				pChar->x = dPrevW;
				if (i)
					dPlusOffset += dPrevW;
				dPrevW = dW;
				pChar->width = dW * dAbsVec;

				if (i == nCount - 1)
				{
					m_oLine.m_dEndX += dPlusOffset * m_oLine.m_ex;
					m_oLine.m_dEndY += dPlusOffset * m_oLine.m_ey;
				}
			}
		}
		void DumpLine()
		{
			LONG nCount = m_oLine.GetCountChars();
			if (!nCount)
			{
				m_oLine.Clear();
				return;
			}

			if (m_oLine.m_bIsSetUpTransform)
			{
				// выставится трансформ!!!
				// cравнивать нужно с ним!!!
				m_pLastTransform->SetElements(m_oLine.m_sx, m_oLine.m_shy, m_oLine.m_shx, m_oLine.m_sy);
			}

			// скидываем линию в поток pMeta
			m_pPageMeta->WriteDouble(m_oLine.m_dX);
			m_pPageMeta->WriteDouble(m_oLine.m_dY);

			bool bHorizontal = false;
			if (fabs(m_oLine.m_ex - 1.0) < 0.001 && fabs(m_oLine.m_ey) < 0.001)
				bHorizontal = true;
			m_pPageMeta->WriteBYTE(!bHorizontal ? 1 : 0);
			if (!bHorizontal)
			{
				m_pPageMeta->WriteDouble(m_oLine.m_ex);
				m_pPageMeta->WriteDouble(m_oLine.m_ey);
			}

			m_pPageMeta->WriteDouble(m_oLine.m_dAscent);
			m_pPageMeta->WriteDouble(m_oLine.m_dDescent);

			m_nCountParagraphs++;

			// width
			LONG _position = m_pPageMeta->GetSize();
			m_pPageMeta->AddInt(0);

			double dWidthLine = 0;
			double dCurrentGlyphLineOffset = 0;
			m_pPageMeta->AddInt(nCount);
			NSWasm::CHChar* pChar = NULL;
			bool bIsLastSymbol = false;
			for (LONG lIndexChar = 0; lIndexChar < nCount; ++lIndexChar)
			{
				pChar = &m_oLine.m_pChars[lIndexChar];

				if (lIndexChar)
				{
					m_pPageMeta->WriteDouble(pChar->x);
					dCurrentGlyphLineOffset += pChar->x;
				}

				if (pChar->unicode == 0xFFFF || pChar->unicode == 32 || pChar->unicode == 9)
				{
					m_nCountSpaces++;
					if (bIsLastSymbol)
					{
						bIsLastSymbol = false;
						m_nCountWords++;
					}
				}
				else
				{
					m_nCountSymbols++;
					bIsLastSymbol = true;
				}

				m_pPageMeta->AddInt(pChar->unicode); // юникодное значение
				m_pPageMeta->WriteDouble(pChar->width); // ширина буквы
			}
			if (bIsLastSymbol)
				m_nCountWords++;

			if (pChar)
				dWidthLine = dCurrentGlyphLineOffset + pChar->width;
			m_pPageMeta->AddInt((int)(dWidthLine * 10000.0), _position);
			m_oLine.Clear();
		}
	};
}

#endif // _ASC_HTMLRENDERER_TEXT_H_
