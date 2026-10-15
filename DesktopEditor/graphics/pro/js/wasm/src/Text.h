/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
		double m_lAscent;
		double m_lDescent;
		double m_lUnitsPerEm;

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

		bool m_bIsType3;
		double m_dType3GlyphWidth;

	public:
		CFontManagerWrapper() : m_pManager(NULL), m_bIsType3(false), m_dType3GlyphWidth(0) {}
		virtual ~CFontManagerWrapper()
		{
			RELEASEOBJECT(m_pManager);
		}

		void SetType3Metrics(double dAscent, double dDescent, double dUnitsPerEm, double dGlyphWidth)
		{
			m_bIsType3 = true;
			m_dType3GlyphWidth = dGlyphWidth;
			m_oCurrentInfo.m_lAscent     = dAscent;
			m_oCurrentInfo.m_lDescent    = dDescent;
			m_oCurrentInfo.m_lUnitsPerEm = dUnitsPerEm > 0 ? dUnitsPerEm : 1000;
		}
		void ClearType3()
		{
			m_bIsType3 = false;
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
		TBBox MeasureString(const unsigned int* symbols, const int& count, double x, double y)
		{
			if (m_bIsType3)
			{
				TBBox oBox;
				double dUnitsPerEm = m_oCurrentInfo.m_lUnitsPerEm > 0 ? m_oCurrentInfo.m_lUnitsPerEm : 1000;
				double dFontSizePt = m_pFont ? m_pFont->Size : 10.0;
				oBox.fMinX = (float)x;
				oBox.fMinY = (float)(-(m_oCurrentInfo.m_lAscent  / dUnitsPerEm) * dFontSizePt);
				oBox.fMaxX = (float)(x + m_dType3GlyphWidth);
				oBox.fMaxY = (float)( (m_oCurrentInfo.m_lDescent / dUnitsPerEm) * dFontSizePt);
				return oBox;
			}

			if (!m_pManager)
				return TBBox();

			m_pManager->LoadString1(symbols, count, (float)x, (float)y);
			return m_pManager->MeasureString2();
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
			// 1) first determine reference point and baseline direction
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

				if (bIsNewLine) // baseline didn't match. so just dump line to stream
					DumpLine();
			}

			// now need to determine offset along baseline relative to destination point
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
					// continue line
					dOffsetX = len;

					// now let's see if we need to insert a space
					NSWasm::CHChar* pLastChar = m_oLine.GetTail();
					if (dOffsetX > (pLastChar->width + 0.5))
					{
						if (pLastChar->unicode == 32 || pLastChar->unicode == 9)
						{
							pLastChar->width = dOffsetX;
						}
						else
						{
							// insert space. Our space will not be a regular space. But a specific one
							NSWasm::CHChar* pSpaceChar = m_oLine.AddTail();
							pLastChar = &m_oLine.m_pChars[m_oLine.m_lCharsTail - 2];
							pSpaceChar->x = pLastChar->width;
							pSpaceChar->width = dOffsetX - pLastChar->width;
							pSpaceChar->unicode = 0xFFFF;
							dOffsetX -= pLastChar->width;
						}
					}
				}
				else
				{
					// letter is shifted left relative to previous letter
					// check if new glyph lands inside a synthetic space (0xFFFF) in current line
					double sx = _x1 - m_oLine.m_dX;
					double sy = _y1 - m_oLine.m_dY;
					double newGlyphOffset = sx * m_oLine.m_ex + sy * m_oLine.m_ey;

					// walk through chars accumulating offset to find which char we hit
					double charOffset = 0;
					LONG nCount = m_oLine.GetCountChars();
					LONG nSplitIndex = -1;
					for (LONG i = 0; i < nCount; ++i)
					{
						NSWasm::CHChar* pCh = &m_oLine.m_pChars[i];
						if (i > 0)
							charOffset += pCh->x; // x here is offset from previous char
						if (pCh->unicode == 0xFFFF &&
							newGlyphOffset >= charOffset &&
							newGlyphOffset < charOffset + pCh->width)
						{
							nSplitIndex = i;
							break;
						}
					}

					if (nSplitIndex >= 0)
					{
						NSWasm::CHLine oSaved;
						oSaved = m_oLine;

						// --- Dump head: chars [0 .. nSplitIndex - 1] ---
						m_oLine.m_lCharsTail = nSplitIndex; // truncate before the 0xFFFF
						DumpLine(); // clears m_oLine

						// --- Dump tail: chars [nSplitIndex + 1 .. nCount - 1] ---
						LONG nTailStart = nSplitIndex + 1;
						LONG nTailCount = nCount - nTailStart;
						if (nTailCount > 0)
						{
							// Reconstruct baseline origin for the tail.
							// Walk offsets up to nTailStart to find its position along baseline.
							double tailOffset = 0;
							for (LONG i = 1; i <= nTailStart; ++i)
								tailOffset += oSaved.m_pChars[i].x;

							m_oLine.m_bIsConstX      = oSaved.m_bIsConstX;
							m_oLine.m_dK             = oSaved.m_dK;
							m_oLine.m_dB             = oSaved.m_dB;
							m_oLine.m_ex             = oSaved.m_ex;
							m_oLine.m_ey             = oSaved.m_ey;
							m_oLine.m_dAscent        = oSaved.m_dAscent;
							m_oLine.m_dDescent       = oSaved.m_dDescent;
							m_oLine.m_bIsSetUpTransform = oSaved.m_bIsSetUpTransform;
							m_oLine.m_sx             = oSaved.m_sx;
							m_oLine.m_sy             = oSaved.m_sy;
							m_oLine.m_shx            = oSaved.m_shx;
							m_oLine.m_shy            = oSaved.m_shy;

							// Origin of tail shifted along baseline
							m_oLine.m_dX = oSaved.m_dX + tailOffset * oSaved.m_ex;
							m_oLine.m_dY = oSaved.m_dY + tailOffset * oSaved.m_ey;
							m_oLine.m_dEndX = m_oLine.m_dX;
							m_oLine.m_dEndY = m_oLine.m_dY;

							// Copy tail chars; first tail char has x=0 (it's now the line origin)
							for (LONG i = 0; i < nTailCount; ++i)
							{
								NSWasm::CHChar* pDst = m_oLine.AddTail();
								*pDst = oSaved.m_pChars[nTailStart + i];
								if (i == 0)
									pDst->x = 0; // first char offset is always 0
							}
						}
					}

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
			{ // check if main part of matrix matches
				bIsDumpFont = true;
				*m_pLastTransform = *m_pTransform;
				m_oLine.m_bIsSetUpTransform = true;
				m_oLine.m_sx  = m_pTransform->sx();
				m_oLine.m_shx = m_pTransform->shx();
				m_oLine.m_shy = m_pTransform->shy();
				m_oLine.m_sy  = m_pTransform->sy();
			}

			// done, baseline is set. now just continue the line
			if (bIsDumpFont && !m_oFontManager.m_bIsType3)
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
				TBBox bBox = m_oFontManager.MeasureString((const unsigned int*)(input + i), 1, 0, 0);
				double dW = std::abs((bBox.fMaxX - bBox.fMinX) * 25.4 / 72.0);
				double dMeasureAscent  = std::abs(bBox.fMinY * 25.4 / 72.0);
				double dMeasureDescent = std::abs(bBox.fMaxY * 25.4 / 72.0);
				if (m_oLine.m_dAscent < dMeasureAscent)
					m_oLine.m_dAscent = dMeasureAscent;
				if (m_oLine.m_dDescent < dMeasureDescent)
					m_oLine.m_dDescent = dMeasureDescent;

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
				// transform will be set!!!
				// need to compare with it!!!
				m_pLastTransform->SetElements(m_oLine.m_sx, m_oLine.m_shy, m_oLine.m_shx, m_oLine.m_sy);
			}

			// dump line to pMeta stream
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

				m_pPageMeta->AddInt(pChar->unicode); // unicode value
				m_pPageMeta->WriteDouble(pChar->width); // letter width
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
