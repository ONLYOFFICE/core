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
#include "XpsPage.h"
#include <stdio.h>
#include "../../DesktopEditor/common/StringExt.h"
#include "../../DesktopEditor/graphics/structures.h"

#include "Document.h"
#include "StaticResources.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifndef xpsUnitToMM
#define xpsUnitToMM(x) ((x) * 25.4 / 96)
#endif

#define IsFromResource(String) (!String.empty() && '{' == String[0])

namespace XPS
{
    static double GetAdvanceX(NSFonts::IFontManager* pFontManager, const unsigned int& unUnicode, const unsigned int& unGid, const bool& bGid)
	{
		if (bGid)
		{
			pFontManager->SetStringGID(TRUE);
            return pFontManager->MeasureChar2(unGid).fAdvanceX;
		}
		else
		{
			pFontManager->SetStringGID(FALSE);
            return pFontManager->MeasureChar2(unUnicode).fAdvanceX;
		}
	}
    static double GetAdvanceY(NSFonts::IFontManager* pFontManager, const unsigned int& unUnicode, const unsigned int& unGid, const bool& bGid)
	{
		if (bGid)
		{
			pFontManager->SetStringGID(TRUE);
            return pFontManager->MeasureChar2(unGid).fAdvanceY;
		}
		else
		{
			pFontManager->SetStringGID(FALSE);
            return pFontManager->MeasureChar2(unUnicode).fAdvanceY;
		}
	}	
    Page::Page(const std::wstring& wsPagePath, IFolder* wsRootPath, CFontList* pFontList, NSFonts::IFontManager* pFontManager, CDocument* pDocument)
	{
		m_wsPagePath   = wsPagePath;
		m_wsRootPath   = wsRootPath;
		m_pFontList    = pFontList;
		m_pFontManager = pFontManager;
		m_pDocument    = pDocument;
	}
	Page::~Page()
	{
	}
	void Page::GetSize(int& nW, int& nH) const
	{
		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromStringA(m_wsRootPath->readXml(m_wsPagePath)))
			return;

		if (!oReader.ReadNextNode())
			return;

		CWString wsNodeName = oReader.GetName(), wsAttrName;
		if (wsNodeName == L"mc:AlternateContent")
		{
			if (!oReader.IsEmptyNode())
			{
				int nAltDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nAltDepth))
				{
					wsNodeName = oReader.GetName();
					if (wsNodeName == L"mc:Choice")
					{
						CWString wsAttr;
						ReadAttribute(oReader, L"Requires", wsAttr);
						if (wsAttr == L"xps")
						{
							if (!oReader.IsEmptyNode())
							{
								int nAltDepth2 = oReader.GetDepth();
								while (oReader.ReadNextSiblingNode(nAltDepth2))
								{
									wsNodeName = oReader.GetName();
									if (wsNodeName == L"FixedPage")
									{
										ReadAttribute(oReader, L"Width", wsAttrName);
                                        nW = wsAttrName.tointeger();

										ReadAttribute(oReader, L"Height", wsAttrName);
                                        nH = wsAttrName.tointeger();
										break;
									}
								}
							}
							break;
						}
					}
					else if (wsNodeName == L"mc:Fallback")
					{
						if (!oReader.IsEmptyNode())
						{
							int nAltDepth2 = oReader.GetDepth();
							while (oReader.ReadNextSiblingNode(nAltDepth2))
							{
								wsNodeName = oReader.GetName();
								if (wsNodeName == L"FixedPage")
								{
									ReadAttribute(oReader, L"Width", wsAttrName);
                                    nW = wsAttrName.tointeger();

									ReadAttribute(oReader, L"Height", wsAttrName);
                                    nH = wsAttrName.tointeger();
									break;
								}
							}
						}
						break;
					}
				}
			}
		}
		else if (wsNodeName == L"FixedPage")
		{
			ReadAttribute(oReader, L"Width", wsAttrName);
            nW = wsAttrName.tointeger();

			ReadAttribute(oReader, L"Height", wsAttrName);
            nH = wsAttrName.tointeger();
		}
	}
#ifdef BUILDING_WASM_MODULE
	BYTE* Page::GetGlyphs()
	{
		// Будет освобожден в js
		BYTE* res = m_pPageMeta.GetBuffer();
		m_pPageMeta.ClearWithoutAttack();
		return res;
	}
	BYTE* Page::GetLinks()
	{
		NSWasm::CData oRes;
		oRes.SkipLen();
		for (const CPageLink& link : m_vLinks)
		{
			oRes.WriteString((BYTE*)link.sLink.c_str(), link.sLink.length());
			oRes.AddDouble(0.0);
			oRes.AddDouble(link.dX);
			oRes.AddDouble(link.dY);
			oRes.AddDouble(link.dW);
			oRes.AddDouble(link.dH);
		}
		oRes.WriteLen();

		BYTE* res = oRes.GetBuffer();
		oRes.ClearWithoutAttack();
		return res;
	}
    void  Page::GetGlyphs(IRenderer* m_pRenderer, const std::wstring& bsUnicodeText, unsigned short* pGids, double x, double y, bool bChangeFont)
    {
        // m_pInternal->GetUnicodes(bsUnicodeText);
        int nUnicodeLen = (int)bsUnicodeText.length();
        unsigned int* pTempUnicodes = new unsigned int[nUnicodeLen];
        int nTempUnicodesLen = 0;
        const wchar_t* pWchars = bsUnicodeText.c_str();

        if (sizeof(wchar_t) == 2)
        {
            for (int i = 0; i < nUnicodeLen; i++)
            {
                unsigned int code = (unsigned int)pWchars[i];
                if (code >= 0xD800 && code <= 0xDFFF && (i + 1) < nUnicodeLen)
                {
                    i++;
                    code = 0x10000 + (((code & 0x3FF) << 10) | (0x03FF & pWchars[i]));
                }
                pTempUnicodes[nTempUnicodesLen++] = code;
            }
        }
        else
            for (int i = 0; i < nUnicodeLen; i++)
                pTempUnicodes[nTempUnicodesLen++] = (unsigned int)pWchars[i];

        // m_pInternal->m_oWriter.WriteText(m_pTempUnicodes, (const int*)pGids, m_nTempUnicodesLen, x, y, w, h, m_pInternal->m_bIsChangedFontParamBetweenDrawText);
        // TODO: CheckTectClipRect();

        bool bIsDumpFont = false;
        std::wstring sCurrentFontName; double dFontSize;
        m_pRenderer->get_FontPath(&sCurrentFontName);
        m_pRenderer->get_FontSize(&dFontSize);
        std::wstring wsFontName = sCurrentFontName;
        NSStringExt::Replace(wsFontName, L"\\", L"/");
        wsFontName = GetFileName(wsFontName);
        NSStringExt::ToLower(wsFontName);
        int nCurrentFont = m_pFontList->GetFontId(wsFontName);
        if ((nCurrentFont != m_nCurrentFont) || (dFontSize != m_dCurrentFontSize))
        {
            m_nCurrentFont     = nCurrentFont;
            m_dCurrentFontSize = dFontSize;
            bIsDumpFont = true;
        }

        // m_oSmartText.CommandText(m_pTempUnicodes, (const int*)pGids, m_nTempUnicodesLen, x, y, w, h, bIsDumpFont, this);
        // 1) сначала определяем точку отсчета и направление baseline
        double _x1 = x;
        double _y1 = y;
        double _x2 = x + 1;
        double _y2 = y;
        double sx, shy, shx, sy, tx, ty, tmp;
        m_pRenderer->GetTransform(&sx, &shy, &shx, &sy, &tx, &ty);
        // m_pTransform->TransformPoint(_x1, _y1);
        tmp = _x1;
        _x1 = tmp * sx  + _y1 * shx + tx;
        _y1 = tmp * shy + _y1 * sy  + ty;
        // m_pTransform->TransformPoint(_x2, _y2);
        tmp = _x2;
        _x2 = tmp * sx  + _y2 * shx + tx;
        _y2 = tmp * shy + _y2 * sy  + ty;

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

        LONG nCountChars = m_oLine.GetCountChars();
        bool bIsNewLine  = true;
        if (nCountChars != 0)
        {
            if (_isConstX && m_oLine.m_bIsConstX && fabs(_b - m_oLine.m_dB) < 0.001)
                bIsNewLine = false;
            else if (!_isConstX && !m_oLine.m_bIsConstX && fabs(_k - m_oLine.m_dK) < 0.001 && fabs(_b - m_oLine.m_dB) < 0.001)
                bIsNewLine = false;
        }
        if (bIsNewLine && (nCountChars != 0))
        {
            // не совпала baseline. поэтому просто скидываем линию в поток
            DumpLine();
        }

        // теперь нужно определить сдвиг по baseline относительно destination точки
        nCountChars = m_oLine.GetCountChars();
        double dOffsetX = 0;
        if (nCountChars == 0)
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
            double _sx = _x1 - m_oLine.m_dEndX;
            double _sy = _y1 - m_oLine.m_dEndY;
            double len = sqrt(_sx * _sx + _sy * _sy);

            if (_sx * m_oLine.m_ex >= 0 && _sy * m_oLine.m_ey >= 0)
            {
                // продолжаем линию
                dOffsetX = len;

                // теперь посмотрим, может быть нужно вставить пробел??
                NSWasm::CHChar* pLastChar = m_oLine.GetTail();
                if (dOffsetX > (pLastChar->width + 0.5))
                {
                    // вставляем пробел. Пробел у нас будет не совсем пробел. А специфический
                    NSWasm::CHChar* pSpaceChar = m_oLine.AddTail();
                    pSpaceChar->x       = pLastChar->width;
                    pSpaceChar->width   = dOffsetX - pLastChar->width;
                    pSpaceChar->unicode = 0xFFFF;
                    pSpaceChar->gid     = 0xFFFF;
                    dOffsetX -= pLastChar->width;

                    m_oMeta.WriteBYTE(0);
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

        // смотрим, совпадает ли главная часть матрицы.
        bool bIsTransform = !(fabs(m_pLastTransform.sx()  - sx)  < 0.001 &&
                              fabs(m_pLastTransform.sy()  - sy)  < 0.001 &&
                              fabs(m_pLastTransform.shx() - shx) < 0.001 &&
                              fabs(m_pLastTransform.shy() - shy) < 0.001);
        if (bIsTransform)
            bIsDumpFont = true;

        LONG nColor1, nAlpha1;
        m_pRenderer->get_BrushColor1(&nColor1);
        m_pRenderer->get_BrushAlpha1(&nAlpha1);
        bool bIsColor = ((nColor1 != m_nLastBrushColor1) || (nAlpha1 != m_nLastBrushAlpha1));

        BYTE nLenMetaCommands = 0;
        if (bIsColor)
            nLenMetaCommands += 5;
        if (bIsTransform)
            nLenMetaCommands += 17;
        if (bIsDumpFont)
            nLenMetaCommands += 13;

        m_oMeta.WriteBYTE(nLenMetaCommands);

        double _dumpSize = m_dCurrentFontSize;
        double _dumpMtx[4];
        _dumpMtx[0] = sx;
        _dumpMtx[1] = shy;
        _dumpMtx[2] = shx;
        _dumpMtx[3] = sy;

        double dTextScale = std::min(sqrt(_dumpMtx[2] * _dumpMtx[2] + _dumpMtx[3] * _dumpMtx[3]),
                                     sqrt(_dumpMtx[0] * _dumpMtx[0] + _dumpMtx[1] * _dumpMtx[1]));

        if ((_dumpSize < 0.1 && dTextScale > 10) || (_dumpSize > 10 && dTextScale < 0.1))
        {
            _dumpSize *= dTextScale;

            _dumpMtx[0] /= dTextScale;
            _dumpMtx[1] /= dTextScale;
            _dumpMtx[2] /= dTextScale;
            _dumpMtx[3] /= dTextScale;
        }

        if (bIsDumpFont)
        {
            LONG nFontStyle;
            m_pRenderer->get_FontStyle(&nFontStyle);

            m_oMeta.WriteBYTE(41); // CMetafile::ctFontName
            m_oMeta.AddInt(m_nCurrentFont);
            m_oMeta.AddInt(nFontStyle);
            m_oMeta.WriteDouble(_dumpSize);
        }
        if (bIsTransform)
        {
            m_pLastTransform.SetElements(sx, shy, shx, sy);

            m_oLine.m_bIsSetUpTransform = true;
            m_oLine.m_sx  = sx;
            m_oLine.m_shx = shx;
            m_oLine.m_shy = shy;
            m_oLine.m_sy  = sy;

            m_oMeta.WriteBYTE(161); // CMetafile::ctCommandTextTransform
            m_oMeta.WriteDouble(_dumpMtx[0]);
            m_oMeta.WriteDouble(_dumpMtx[1]);
            m_oMeta.WriteDouble(_dumpMtx[2]);
            m_oMeta.WriteDouble(_dumpMtx[3]);
        }
        if (bIsColor)
        {
            m_nLastBrushColor1 = nColor1;
            m_nLastBrushAlpha1 = nAlpha1;

            m_oMeta.WriteBYTE(22); // CMetafile::ctBrushColor1

            LONG lBGR = m_nLastBrushColor1;
            m_oMeta.WriteBYTE((BYTE)(lBGR & 0xFF));
            m_oMeta.WriteBYTE((BYTE)((lBGR >> 8) & 0xFF));
            m_oMeta.WriteBYTE((BYTE)((lBGR >> 16) & 0xFF));
            m_oMeta.WriteBYTE((BYTE)m_nLastBrushAlpha1);
        }

        // все, baseline установлен. теперь просто продолжаем линию
        if (bIsDumpFont || bChangeFont)
        {
            m_pFontManager->LoadFontFromFile(sCurrentFontName, 0, m_dCurrentFontSize, 72.0, 72.0);
            m_pFontManager->AfterLoad();
        }

        double dKoef = m_dCurrentFontSize * 25.4 / (72 * abs(m_pFontManager->GetUnitsPerEm()));
        double dAscender  = abs(m_pFontManager->GetAscender())  * dKoef * dAbsVec;
        double dDescender = abs(m_pFontManager->GetDescender()) * dKoef * dAbsVec;

        if (m_oLine.m_dAscent < dAscender)
            m_oLine.m_dAscent = dAscender;
        if (m_oLine.m_dDescent < dDescender)
            m_oLine.m_dDescent = dDescender;

        double dPlusOffset = 0;
        unsigned int* input = NULL;
        if (pGids)
        {
            input = (unsigned int*)pGids;
            m_pFontManager->SetStringGID(TRUE);
        }
        else
        {
            input = pTempUnicodes;
            m_pFontManager->SetStringGID(FALSE);
        }

        for (int i = 0; i < nTempUnicodesLen; i++)
        {
            // double dW = m_oFontManager.MeasureString((const unsigned int*)(input + lIndex), 1, 0, 0, dBoxX, dBoxY, dBoxW, dBoxH);
            m_pFontManager->LoadString1(input + i, 1, 0, 0);
            TBBox _box  = m_pFontManager->MeasureString2();
            double dBoxW = abs(_box.fMaxX - _box.fMinX) * 25.4 / 72.0;

            NSWasm::CHChar* pChar = m_oLine.AddTail();
            pChar->unicode = pTempUnicodes[i];
            pChar->gid = pGids ? pGids[i] : 0xFFFF;

            pChar->x = dOffsetX;
            if (i != 0)
                dPlusOffset += dOffsetX;
            dOffsetX = dBoxW;

            pChar->width = dBoxW * dAbsVec;

            if (i != 0)
                m_oMeta.WriteBYTE(0);

            if (i == (nTempUnicodesLen - 1))
            {
                m_oLine.m_dEndX += dPlusOffset * m_oLine.m_ex;
                m_oLine.m_dEndY += dPlusOffset * m_oLine.m_ey;
            }
        }

        RELEASEARRAYOBJECTS(pTempUnicodes);
    }
    void  Page::DumpLine()
    {
        if (m_oLine.m_bIsSetUpTransform)
        {
            // выставится трансформ!!!
            // cравнивать нужно с ним!!!
            m_pLastTransform.SetElements(m_oLine.m_sx, m_oLine.m_shy, m_oLine.m_shx, m_oLine.m_sy);
        }

        // скидываем линию в поток pMeta
        BYTE mask = 0;
        if (fabs(m_oLine.m_ex - 1.0) < 0.001 && fabs(m_oLine.m_ey) < 0.001)
            mask |= 0x01;

        LONG lCountSpaces  = 0;
        LONG lCountSymbols = 0;
        LONG lCountWords   = 0;
        bool bIsLastSymbol = false;
        bool bIsGidExist   = false;

        LONG nCount = m_oLine.GetCountChars();
        for (LONG i = 0; i < nCount; i++)
        {
            NSWasm::CHChar* pChar = &m_oLine.m_pChars[i];
            if (pChar->gid != 0xFFFF)
            {
                mask |= 0x02;
                bIsGidExist = true;
            }

            if (0xFFFF == pChar->unicode || L' ' == pChar->unicode || L'\t' == pChar->unicode)
            {
                lCountSpaces++;
                if (bIsLastSymbol)
                {
                    bIsLastSymbol = false;
                    lCountWords++;
                }
            }
            else
            {
                bIsLastSymbol = true;
                lCountSymbols++;
            }
        }

        if (bIsLastSymbol)
            lCountWords++;

        if (nCount == 0)
        {
            m_oLine.Clear();
            m_oMeta.ClearNoAttack();
            return;
        }

        if (nCount > 1)
            mask |= 0x04;

        m_pPageMeta.WriteBYTE(160); // CMetafile::ctCommandTextLine
        m_pPageMeta.WriteBYTE(mask);

        m_pPageMeta.WriteDouble(m_oLine.m_dX);
        m_pPageMeta.WriteDouble(m_oLine.m_dY);

        if ((mask & 0x01) == 0)
        {
            m_pPageMeta.WriteDouble(m_oLine.m_ex);
            m_pPageMeta.WriteDouble(m_oLine.m_ey);
        }

        m_pPageMeta.WriteDouble(m_oLine.m_dAscent);
        m_pPageMeta.WriteDouble(m_oLine.m_dDescent);

        LONG _position = 0;
        if (nCount > 1)
        {
            _position = m_pPageMeta.GetSize();
            m_pPageMeta.AddInt(0);
        }

        BYTE* pBufferMeta = m_oMeta.GetBuffer();
        double dWidthLine = 0;

        double dCurrentGlyphLineOffset = 0;
        for (LONG lIndexChar = 0; lIndexChar < nCount; lIndexChar++)
        {
            NSWasm::CHChar* pChar = &m_oLine.m_pChars[lIndexChar];

            // все настроки буквы (m_oMeta)
            BYTE lLen = *pBufferMeta;
            pBufferMeta++;
            if (lLen > 0)
                m_pPageMeta.Write(pBufferMeta, lLen);
            pBufferMeta += lLen;
            // смещение относительно предыдущей буквы (у всех, кроме первой)
            // юникодное значение
            // гид (если bIsGidExist == true)
            // ширина буквы

            m_pPageMeta.WriteBYTE(80); // CMetafile::ctDrawText
            if (lIndexChar)
                m_pPageMeta.WriteDouble2(pChar->x);

            m_pPageMeta.WriteWCHAR(pChar->unicode);
            if (bIsGidExist)
                m_pPageMeta.WriteUSHORT(pChar->gid);
            m_pPageMeta.WriteDouble2(pChar->width);

            if (lIndexChar)
                dCurrentGlyphLineOffset += pChar->x;

            if (lIndexChar == (nCount - 1))
                dWidthLine = dCurrentGlyphLineOffset + pChar->width;
        }

        if (nCount > 1)
        {
            int* pWidthBuf = (int*)(m_pPageMeta.GetBuffer() + _position);
            *pWidthBuf = (int)(dWidthLine * 10000);
        }

        m_oLine.Clear();
        m_oMeta.ClearNoAttack();
        m_pPageMeta.WriteBYTE(162); // CMetafile::ctCommandTextLineEnd
    }
#endif
	void Page::Draw(IRenderer* pRenderer, bool* pbBreak)
	{
		#ifdef BUILDING_WASM_MODULE
		m_pPageMeta.Clear();
		m_pPageMeta.SkipLen();
		m_vLinks.clear();
		#endif
		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromStringA(m_wsRootPath->readXml(m_wsPagePath)))
			return;

		if (!oReader.ReadNextNode())
			return;

		CContextState oState(pRenderer);
		CWString wsNodeName = oReader.GetName();
		if (wsNodeName == L"mc:AlternateContent")
		{
			if (!oReader.IsEmptyNode())
			{
				int nAltDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nAltDepth))
				{
					wsNodeName = oReader.GetName();
					if (wsNodeName == L"mc:Choice")
					{
						CWString wsAttr;
						ReadAttribute(oReader, L"Requires", wsAttr);
						if (wsAttr == L"xps")
						{
							if (!oReader.IsEmptyNode())
							{
								int nAltDepth2 = oReader.GetDepth();
								while (oReader.ReadNextSiblingNode(nAltDepth2))
								{
									wsNodeName = oReader.GetName();
									if (wsNodeName == L"FixedPage")
									{
										DrawCanvas(oReader, pRenderer, &oState, pbBreak);
										break;
									}
								}
							}
							break;
						}
					}
					else if (wsNodeName == L"mc:Fallback")
					{
						if (!oReader.IsEmptyNode())
						{
							int nAltDepth2 = oReader.GetDepth();
							while (oReader.ReadNextSiblingNode(nAltDepth2))
							{
								wsNodeName = oReader.GetName();
								if (wsNodeName == L"FixedPage")
								{
									DrawCanvas(oReader, pRenderer, &oState, pbBreak);
									break;
								}
							}
						}
						break;
					}
				}
			}
		}
		else if (wsNodeName == L"FixedPage")
		{
			DrawCanvas(oReader, pRenderer, &oState, pbBreak);
		}
	#ifdef BUILDING_WASM_MODULE
		LONG nCount = m_oLine.GetCountChars();
		if (nCount)
			DumpLine();
		m_pPageMeta.WriteLen();
	#endif
	}
	void Page::DrawCanvas(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState, bool* pbBreak)
	{
		bool bTransform = false, bClip = false, bOpacity = false, bResource = false;
		if (oReader.MoveToFirstAttribute())
		{
            std::string wsAttrName = oReader.GetNameA();
			while (!wsAttrName.empty())
			{
                if (wsAttrName == "Clip")
                    bClip = ClipToRenderer(oReader.GetText().c_str(), pState);
                else if (wsAttrName == "RenderTransform")
                    bTransform = TransformToRenderer(oReader.GetText().c_str(), pState);
                else if (wsAttrName == "Opacity")
				{
					pState->PushOpacity(GetDouble(oReader.GetText()));
					bOpacity = true;
				}

				if (!oReader.MoveToNextAttribute())
					break;

                wsAttrName = oReader.GetNameA();
			}
		}
		oReader.MoveToElement();

		if (oReader.IsEmptyNode())
			return;

		CWString wsNodeName;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetName();

			if (wsNodeName == L"FixedPage.Resources")
			{
				bResource = ReadResource(oReader, pRenderer, pState);
			}
			else if (wsNodeName == L"Canvas.Resources")
			{
				bResource = ReadResource(oReader, pRenderer, pState);
			}
			else if (wsNodeName == L"Glyphs")
			{
				DrawGlyph(oReader, pRenderer, pState);
			}
			else if (wsNodeName == L"Canvas")
			{
				DrawCanvas(oReader, pRenderer, pState, pbBreak);
			}
			else if (wsNodeName == L"Canvas.RenderTransform" && !bTransform)
			{
				CWString wsTransform;
				ReadTransform(oReader, wsTransform);
				bTransform = TransformToRenderer(wsTransform.c_str(), pState);
			}
			else if (wsNodeName == L"Canvas.Clip" && !bClip)
			{
				CWString wsClip;
				ReadClip(oReader, wsClip);
				bClip = ClipToRenderer(wsClip.c_str(), pState);
			}
			else if (wsNodeName == L"Path")
			{
				DrawPath(oReader, pRenderer, pState);
			}
			else if (wsNodeName == L"mc:AlternateContent")
			{
				if (!oReader.IsEmptyNode())
				{
					int nAltDepth = oReader.GetDepth();
					while (oReader.ReadNextSiblingNode(nAltDepth))
					{
						wsNodeName = oReader.GetName();
						if (wsNodeName == L"mc:Choice")
						{
							CWString wsAttr;
							ReadAttribute(oReader, L"Requires", wsAttr);
							if (wsAttr == L"xps")
							{
								DrawCanvas(oReader, pRenderer, pState, NULL);
								break;
							}
						}
						else if (wsNodeName == L"mc:Fallback")
						{
							DrawCanvas(oReader, pRenderer, pState, NULL);
							break;
						}
					}
				}
			}

			if (NULL != pbBreak && *pbBreak)
				return;
		}		

		if (bClip)
			pState->PopClip();

		if (bTransform)
			pState->PopTransform();

		if (bOpacity)
			pState->PopOpacity();

		if (bResource)
			pState->PopResource();
	}
	bool Page::ReadResource(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState)
	{
		if (oReader.IsEmptyNode())
			return false;

		CWString wsNodeName;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetName();
			if (wsNodeName == L"ResourceDictionary")
			{
				CWString wsSource;
				ReadAttribute(oReader, L"Source", wsSource);
				if (!wsSource.empty())
				{
                    std::wstring wsPath = wsSource.c_stdstr();
					pState->PushResource(m_pDocument->GetStaticResource(wsPath.c_str()), false);
				}
				else
				{
					pState->PushResource(new CStaticResource(oReader), true);
				}

				return true;
			}
		}

		return false;
	}
	bool Page::ClipToRenderer(const wchar_t* wsString, CContextState* pState)
	{
		CWString wsClip;
		wsClip.create(wsString, true);
		if (!wsClip.empty())
		{
			if (IsFromResource(wsClip))
			{
				CWString wsPathTransform;
				pState->GetPathGeometry(wsClip, wsClip, wsPathTransform);
			}

			pState->PushClip(wsClip);
			return true;
		}
		return false;
	}
	bool Page::TransformToRenderer(const wchar_t* wsString, CContextState* pState)
	{
		CWString wsTransform = wsString;

		if (!wsTransform.empty())
		{
			if (IsFromResource(wsTransform))
				pState->GetTransform(wsTransform, wsTransform);

			std::vector<CWString> arrElements = wsTransform.split(',');
			double arrRes[6] ={ 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
            for (int nIndex = 0, nCount = std::min(6, (int)arrElements.size()); nIndex < nCount; nIndex++)
				arrRes[nIndex] = GetDouble(arrElements[nIndex]);

			pState->PushTransform(arrRes);
			return true;
		}
		return false;
	}
	void Page::DrawGlyph(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState)
	{
		double dFontSize = 10.0;
		bool bTransform = false, bClip = false, bOpacity = false;
		double dX = 0;
		double dY = 0;
		std::wstring wsFontPath;
		std::wstring wsIndicies;
		int nBidiLevel = 0;
		CWString wsClip, wsTransform;
		unsigned short* pUtf16    = NULL;
		unsigned short* pUtf16Ptr = NULL;
		unsigned int unUtf16Len = 0;
		CWString wsIndices;
		CWString wsFill;
		bool bIsSideways = false;

		bool bForceItalic = false;
		bool bForceBold   = false;

		if (oReader.MoveToFirstAttribute())
		{
			std::wstring wsAttrName = oReader.GetName();
			while (!wsAttrName.empty())
			{
				if (L"FontUri" == wsAttrName)
				{
					wsFontPath = oReader.GetText();
					std::wstring wsFontName = GetFileName(wsFontPath);

					if (wsFontPath.length() >= 0 && '.' == wsFontPath.at(0))
					{
						int nSlashPos = m_wsPagePath.find_last_of(L'/');
						if (std::wstring::npos == nSlashPos)
							nSlashPos = -1;

						std::wstring wsRelativePath = (std::wstring::npos == nSlashPos) ? m_wsPagePath : m_wsPagePath.substr(0, nSlashPos + 1);
						wsFontPath = wsRelativePath + wsFontPath;
					}
					else
					{
						wsFontPath = m_wsRootPath->getFullFilePath(wsFontPath);
					}

					std::wstring wsExt = GetFileExtension(wsFontPath);
					NSStringExt::ToLower(wsExt);
					if (L"odttf" == wsExt)
					{
						NSStringExt::ToLower(wsFontName);
                        m_pFontList->Check(wsFontName, wsFontPath, m_wsRootPath);
					}
                    else
                    {
                        // шрифт не odttf - надо добавить его во внешний сторадж шрифтов, если нужно
                        if (IFolder::iftZip == m_wsRootPath->getType() && NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage())
                        {
                            IFolder::CBuffer* buffer = NULL;
                            m_wsRootPath->read(wsFontPath, buffer);

                            if (NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage())
                                NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->Add(wsFontPath, buffer->Buffer, buffer->Size);

                            RELEASEOBJECT(buffer);
                        }
                    }
					wsFontPath = NormalizePath(wsFontPath);
					pRenderer->put_FontPath(wsFontPath);
				}
				else if (wsAttrName == L"Opacity")
				{
					double dOpacity;
					ReadSTDouble(oReader.GetText(), dOpacity);
					pState->PushOpacity(dOpacity);
					bOpacity = true;
				}
				else if (L"Clip" == wsAttrName)
				{
					wsClip.create(oReader.GetText(), true);
				}
				else if (L"Fill" == wsAttrName)
				{
					wsFill.create(oReader.GetText(), true);
				}
				else if (L"StyleSimulations" == wsAttrName)
				{
					CWString wsFontStyle = oReader.GetText();
					if (wsFontStyle == L"ItalicSimulation")
					{
						bForceItalic  = true;
					}
					else if (wsFontStyle == L"BoldSimulation")
					{
						bForceBold = true;
					}
					else if (wsFontStyle == L"BoldItalicSimulation")
					{
						bForceItalic = true;
						bForceBold   = true;
					}
				}
				else if (L"FontRenderingEmSize" == wsAttrName)
				{
					dFontSize = GetDouble(oReader.GetText());
				}
				else if (L"RenderTransform" == wsAttrName)
				{
					wsTransform.create(oReader.GetText(), true);
				}
				else if (L"UnicodeString" == wsAttrName)
				{
					CWString wsUnicodeString = oReader.GetText();
					if (!wsUnicodeString.empty())
					{
						pUtf16Ptr = NSStringExt::CConverter::GetUtf16FromUnicode(wsUnicodeString.c_str(), unUtf16Len);

						if (unUtf16Len >= 2 && '{' == pUtf16Ptr[0] && '}' == pUtf16Ptr[1])
						{
							pUtf16 = pUtf16Ptr + 2;
							unUtf16Len -= 2;
						}
						else
						{
							pUtf16 = pUtf16Ptr;
						}
					}
				}
				else if (L"OriginX" == wsAttrName)
				{
					dX = GetDouble(oReader.GetText());
				}
				else if (L"OriginY" == wsAttrName)
				{
					dY = GetDouble(oReader.GetText());
				}
				else if (L"Indices" == wsAttrName)
				{
					wsIndices.create(oReader.GetText(), true);
					wsIndicies = oReader.GetText();
				}
				else if (L"BidiLevel" == wsAttrName)
				{
					nBidiLevel = GetInteger(oReader.GetText());
				}
				else if (wsAttrName == L"IsSideways")
				{
					bIsSideways = GetBool(oReader.GetText());
				}

				if (!oReader.MoveToNextAttribute())
					break;

				wsAttrName = oReader.GetName();
			}
		}
		oReader.MoveToElement();

		CBrush* pBrush = NULL;
		bool bDeleteBrush = false;
		if (!wsFill.empty())
		{
			if (IsFromResource(wsFill))
			{
				pBrush = pState->GetBrush(wsFill);
				bDeleteBrush = false;
			}
			else
			{
				pBrush = ReadBrush(wsFill.c_str(), pState->GetCurrentOpacity());
				bDeleteBrush = true;
			}
		}

		if (!oReader.IsEmptyNode())
		{
			CWString wsNodeName;
			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				wsNodeName = oReader.GetName();
				if (wsNodeName == L"Glyphs.RenderTransform")
				{
					ReadTransform(oReader, wsTransform);
				}
				else if (wsNodeName == L"Glyphs.Fill" && !pBrush)
				{
					pBrush = ReadBrush(oReader, pState->GetCurrentOpacity());
					bDeleteBrush = true;
				}
			}
		}

		if (!pBrush || !pBrush->SetToRenderer(pRenderer))
		{
			if (bDeleteBrush)
				RELEASEOBJECT(pBrush);

			RELEASEARRAYOBJECTS(pUtf16Ptr);

			if (bClip)
				pState->PopClip();

			if (bTransform)
				pState->PopTransform();

			if (bOpacity)
				pState->PopOpacity();

			return;
		}

		// Сначала задается матрица преобразования, потом клип, потому что даже
		// если преобразование задано в дочерней ноде, а клип задан в атрибутах данной ноды,
		// то преобразование влияется на клип все равно.
		if (!wsTransform.empty())
		{
			bTransform = TransformToRenderer(wsTransform.c_str(), pState);
			if (dFontSize < 5)
			{
				double dDet = pState->NormalizeTransform();
				dFontSize *= dDet;
			}
		}

		if (!wsClip.empty())
		{
			bClip = ClipToRenderer(wsClip.c_str(), pState);
		}

		pRenderer->put_FontSize(dFontSize * 0.75);

		TIndicesEntry oEntry;
		int nIndicesPos = 0, nIndicesLen = wsIndices.size();
		int nUtf16Pos = 0;
		bool bRtoL = (nBidiLevel % 2 ? true : false);
		m_pFontManager->LoadFontFromFile(m_wsRootPath->getFullFilePath(wsFontPath), 0, (float)(dFontSize * 0.75), 96, 96);
		double dFontKoef = dFontSize / 100.0;

		bool bNeedItalic = false, bNeedBold = false, bChangeFont = true;
        NSFonts::IFontFile* pFile = m_pFontManager->GetFile();
        if (pFile)
		{
            if (!pFile->IsItalic() && bForceItalic)
				bNeedItalic = true;

            if (!pFile->IsBold() && bForceBold)
			{
				LONG lTextColor, lTextAlpha;
				pRenderer->get_BrushColor1(&lTextColor);
				pRenderer->get_BrushAlpha1(&lTextAlpha);
				pRenderer->put_PenColor(lTextColor);
				pRenderer->put_PenAlpha(lTextAlpha);
				pRenderer->put_PenSize(xpsUnitToMM(1));
				bNeedBold = true;
			}
		}

		if (!bIsSideways)
		{
			while (GetNextGlyph(wsIndices.c_str(), nIndicesPos, nIndicesLen, pUtf16, nUtf16Pos, unUtf16Len, oEntry))
			{
				double dAdvance, dRealAdvance;
				if (oEntry.bAdvance)
				{
					dAdvance = oEntry.dAdvance * dFontKoef;

					if (bRtoL)
						dRealAdvance = GetAdvanceX(m_pFontManager, oEntry.nUnicode, oEntry.nGid, oEntry.bGid);
					else
						dRealAdvance = dAdvance;
				}
				else
				{
					dAdvance = GetAdvanceX(m_pFontManager, oEntry.nUnicode, oEntry.nGid, oEntry.bGid);
					dRealAdvance = dAdvance;
				}

				if (bRtoL)
					dX -= dRealAdvance;

				double dXorigin = (oEntry.bHorOffset || oEntry.bVerOffset) ? dX + (bRtoL ? -oEntry.dHorOffset * dFontKoef : oEntry.dHorOffset * dFontKoef) : dX;
				double dYorigin = (oEntry.bHorOffset || oEntry.bVerOffset) ? dY - oEntry.dVerOffset * dFontKoef : dY;	

				if (bNeedItalic)
				{
					double dAlpha = sin(-15 * M_PI / 180);
					double pTransform[] ={ 1, 0, dAlpha, 1, -dAlpha * dYorigin, 0 };
					pState->PushTransform(pTransform);
				}

				if (oEntry.bGid)
                {
					pRenderer->CommandDrawTextExCHAR(oEntry.nUnicode, oEntry.nGid, xpsUnitToMM(dXorigin), xpsUnitToMM(dYorigin), 0, 0);
                #ifdef BUILDING_WASM_MODULE
                    std::wstring sUnicode;
                    sUnicode += wchar_t(oEntry.nUnicode);
                    GetGlyphs(pRenderer, sUnicode, &oEntry.nGid, xpsUnitToMM(dXorigin), xpsUnitToMM(dYorigin), bChangeFont);
                    bChangeFont = false;
                #endif
                }
				else
                {
                    LONG nRenType = 0;
                    pRenderer->get_Type(&nRenType);
                    if (c_nGrRenderer == nRenType)
                        pRenderer->put_FontStringGID(FALSE);

                    pRenderer->CommandDrawTextCHAR(oEntry.nUnicode,  xpsUnitToMM(dXorigin), xpsUnitToMM(dYorigin), 0, 0);
                #ifdef BUILDING_WASM_MODULE
                    std::wstring sUnicode;
                    sUnicode += wchar_t(oEntry.nUnicode);
                    GetGlyphs(pRenderer, sUnicode, NULL, xpsUnitToMM(dXorigin), xpsUnitToMM(dYorigin), bChangeFont);
                    bChangeFont = false;
                #endif
                }

				if (bNeedBold)
				{
					pRenderer->BeginCommand(c_nPathType);
					pRenderer->PathCommandStart();

					if (oEntry.bGid)
						pRenderer->PathCommandTextExCHAR(oEntry.nUnicode, oEntry.nGid, xpsUnitToMM(dXorigin), xpsUnitToMM(dYorigin), 0, 0);
					else
						pRenderer->PathCommandTextCHAR(oEntry.nUnicode, xpsUnitToMM(dXorigin), xpsUnitToMM(dYorigin), 0, 0);

					pRenderer->DrawPath(c_nStroke);
					pRenderer->EndCommand(c_nPathType);
					pRenderer->PathCommandEnd();
				}

				if (bNeedItalic)
					pState->PopTransform();

				if (!bRtoL)
					dX += dAdvance;
				else
					dX -= (dAdvance - dRealAdvance);
			}
		}
		else
		{
			while (GetNextGlyph(wsIndices.c_str(), nIndicesPos, nIndicesLen, pUtf16, nUtf16Pos, unUtf16Len, oEntry))
			{
				double dAdvanceX = GetAdvanceX(m_pFontManager, oEntry.nUnicode, oEntry.nGid, oEntry.bGid);
				double dAdvanceY = GetAdvanceY(m_pFontManager, oEntry.nUnicode, oEntry.nGid, oEntry.bGid);

				double dAdvance;
				if (oEntry.bAdvance)
					dAdvance = oEntry.dAdvance * dFontKoef;
				else
					dAdvance = dAdvanceY;

				double dXorigin = (oEntry.bHorOffset || oEntry.bVerOffset) ? dX + oEntry.dHorOffset * dFontKoef : dX;
				double dYorigin = (oEntry.bHorOffset || oEntry.bVerOffset) ? dY - oEntry.dVerOffset * dFontKoef : dY;
				if (bNeedItalic)
				{
					double dAlpha = sin(15 * M_PI / 180);
					double pTransform[] ={ 1, dAlpha, 0, 1, 0, -dAlpha * dXorigin };
					pState->PushTransform(pTransform);
				}
				double pTransform[] ={ 0, -1, 1, 0, dXorigin + dAdvanceY, dYorigin + dAdvanceX / 2 };
				pState->PushTransform(pTransform);

				if (oEntry.bGid)
                {
					pRenderer->CommandDrawTextExCHAR(oEntry.nUnicode, oEntry.nGid, 0, 0, 0, 0);
                #ifdef BUILDING_WASM_MODULE
                    std::wstring sUnicode;
                    sUnicode += wchar_t(oEntry.nUnicode);
                    GetGlyphs(pRenderer, sUnicode, &oEntry.nGid, 0, 0, bChangeFont);
                    bChangeFont = false;
                #endif
                }
				else
                {
					pRenderer->CommandDrawTextCHAR(oEntry.nUnicode, 0, 0, 0, 0);
                #ifdef BUILDING_WASM_MODULE
                    std::wstring sUnicode;
                    sUnicode += wchar_t(oEntry.nUnicode);
                    GetGlyphs(pRenderer, sUnicode, NULL, 0, 0, bChangeFont);
                    bChangeFont = false;
                #endif
                }

				if (bNeedBold)
				{
					pRenderer->BeginCommand(c_nPathType);
					pRenderer->PathCommandStart();

					if (oEntry.bGid)
						pRenderer->PathCommandTextExCHAR(oEntry.nUnicode, oEntry.nGid, 0, 0, 0, 0);
					else
						pRenderer->PathCommandTextCHAR(oEntry.nUnicode, 0, 0, 0, 0);

					pRenderer->DrawPath(c_nStroke);
					pRenderer->EndCommand(c_nPathType);
					pRenderer->PathCommandEnd();
				}

				pState->PopTransform();

				if (bNeedItalic)
					pState->PopTransform();

				dX += dAdvance;
			}
		}

		if (bDeleteBrush)
			RELEASEOBJECT(pBrush);

		RELEASEARRAYOBJECTS(pUtf16Ptr);

		if (bClip)
			pState->PopClip();

		if (bTransform)
			pState->PopTransform();

		if (bOpacity)
			pState->PopOpacity();
	}
	void Page::DrawPath(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState)
	{
		bool bTransform = false, bClip = false, bOpacity = false;

		double dPenSize = 1.0;

		bool bStroke = false;
		bool bFill   = false;

		int nFillBgr = 0, nFillAlpha = 255, nStrokeBgr = 0, nStrokeAlpha = 255;

		BYTE nDashCap   = Aggplus::LineCapFlat;
		BYTE nStartCap  = Aggplus::LineCapFlat;
		BYTE nEndCap    = Aggplus::LineCapFlat;
		BYTE nJoinStyle = Aggplus::LineJoinMiter;
		double dMiter   = 10.0;

		double* pDashPattern  = NULL;
		LONG lDashPatternSize = 0;
		double dDashOffset    = 0.0;
		CWString wsFill;

		CWString wsClip, wsTransform, wsPathData, wsPathTransform;
	#ifdef BUILDING_WASM_MODULE
		std::vector<CDocument::CDocumentStructure>::iterator find = m_pDocument->m_vStructure.end();
	#endif
		if (oReader.MoveToFirstAttribute())
		{
			std::wstring wsAttrName = oReader.GetName();
			while (!wsAttrName.empty())
			{				
				if (L"RenderTransform" == wsAttrName)
				{
					wsTransform.create(oReader.GetText(), true);
				}
				else if (L"Clip" == wsAttrName)
				{
					wsClip.create(oReader.GetText(), true);
				}
				else if (L"Opacity" == wsAttrName)
				{
					double dOpacity;
					ReadSTDouble(oReader.GetText(), dOpacity);
					pState->PushOpacity(dOpacity);
					bOpacity = true;
				}
				else if (L"Stroke" == wsAttrName)
				{
					std::wstring wsStrokeColor = oReader.GetText();
					GetBgra(wsStrokeColor, nStrokeBgr, nStrokeAlpha);
					bStroke = true;
				}
				else if (L"StrokeThickness" == wsAttrName)
				{
					std::wstring wsPenSize = oReader.GetText();
					dPenSize = GetDouble(wsPenSize);
				}
				else if (L"StrokeDashArray" == wsAttrName)
				{
					std::wstring wsDashArray = oReader.GetText();
					std::vector<std::wstring> arrDashArray = NSStringExt::Split(wsDashArray, ' ');
					int nDashArrayCount = arrDashArray.size();
					if (nDashArrayCount > 0)
					{
						pDashPattern = new double[nDashArrayCount];
						if (pDashPattern)
						{
							lDashPatternSize = nDashArrayCount;
							for (int nIndex = 0; nIndex < nDashArrayCount; nIndex++)
							{
								pDashPattern[nIndex] = GetDouble(arrDashArray.at(nIndex));
							}
						}
					}
				}
				else if (L"StrokeDashOffset" == wsAttrName)
				{
					std::wstring wsDashOffset = oReader.GetText();
					dDashOffset = GetDouble(wsDashOffset);
				}
				else if (L"StrokeDashCap" == wsAttrName)
				{
                    nDashCap = GetCapStyle(oReader.GetTextA());
				}
				else if (L"StrokeEndLineCap" == wsAttrName)
				{
                    nEndCap = GetCapStyle(oReader.GetTextA());
				}
				else if (L"StrokeStartLineCap" == wsAttrName)
				{
                    nStartCap = GetCapStyle(oReader.GetTextA());
				}
				else if (L"StrokeLineJoin" == wsAttrName)
				{
					CWString wsJoin = oReader.GetText();
					if (wsJoin == L"Miter")
						nJoinStyle = Aggplus::LineJoinMiter;
					else if (wsJoin == L"Bevel")
						nJoinStyle = Aggplus::LineJoinBevel;
					else if (wsJoin == L"Round")
						nJoinStyle = Aggplus::LineJoinRound;
				}
				else if (L"StrokeMiterLimit" == wsAttrName)
				{
					std::wstring wsMiterLimit = oReader.GetText();
					dMiter = GetDouble(wsMiterLimit);
				}
				else if (L"Fill" == wsAttrName)
				{
					wsFill.create(oReader.GetText(), true);
				}
				else if (L"Data" == wsAttrName)
				{
					wsPathData.create(oReader.GetText(), true);
				}
			#ifdef BUILDING_WASM_MODULE
				else if (L"Name" == wsAttrName)
				{
					std::wstring wsNameTarget = oReader.GetText();
					find = std::find_if(m_pDocument->m_vStructure.begin(), m_pDocument->m_vStructure.end(), [wsNameTarget](const CDocument::CDocumentStructure& str){ return str.wsTarget == wsNameTarget; });
				}
				else if (L"FixedPage.NavigateUri" == wsAttrName)
				{
					double pdA, pdB, pdC, pdD, pdE, pdF;
					pRenderer->GetTransform(&pdA, &pdB, &pdC, &pdD, &pdE, &pdF);
					Aggplus::CMatrix oTransform(pdA, pdB, pdC, pdD, pdE, pdF);
					double x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0;

					CPageLink oLink = {0, 0, 0, 0, ""};
					std::wstring wsPath = wsPathData.c_stdstr();
					size_t nFindX = wsPath.find(L"M ");
					if (nFindX != std::wstring::npos)
					{
						nFindX += 2;
						size_t nFindEndX = wsPath.find(L',', nFindX);
						if (nFindEndX != std::wstring::npos)
						{
							x1 = GetDouble(wsPath.substr(nFindX, nFindEndX - nFindX));
							size_t nFindY = nFindEndX + 1;
							size_t nFindEndY = wsPath.find(L' ', nFindY);
							if (nFindEndY != std::wstring::npos)
								y1 = GetDouble(wsPath.substr(nFindY, nFindEndY - nFindY));
							oTransform.TransformPoint(x1, y1);
						}
					}
					nFindX = wsPath.find(L"L ");
					if (nFindX != std::wstring::npos)
					{
						nFindX += 2;
						size_t nFindEndX = wsPath.find(L',', nFindX);
						if (nFindEndX != std::wstring::npos)
						{
							x2 = GetDouble(wsPath.substr(nFindX, nFindEndX - nFindX));
							size_t nFindY = nFindEndX + 1;
							size_t nFindEndY = wsPath.find(L' ', nFindY);
							if (nFindEndY != std::wstring::npos)
								y2 = GetDouble(wsPath.substr(nFindY, nFindEndY - nFindY));
							oTransform.TransformPoint(x2, y2);
						}
					}
					nFindX = wsPath.find(L"L ", nFindX);
					if (nFindX != std::wstring::npos)
					{
						nFindX += 2;
						size_t nFindEndX = wsPath.find(L',', nFindX);
						if (nFindEndX != std::wstring::npos)
						{
							x3 = GetDouble(wsPath.substr(nFindX, nFindEndX - nFindX));
							size_t nFindY = nFindEndX + 1;
							size_t nFindEndY = wsPath.find(L' ', nFindY);
							if (nFindEndY != std::wstring::npos)
								y3 = GetDouble(wsPath.substr(nFindY, nFindEndY - nFindY));
							oTransform.TransformPoint(x3, y3);
						}
					}
					// Верхний левый угол
					oLink.dX = x1 == x2 ? fmin(x1, x3) : fmin(x1, x2);
					oLink.dY = y1 == y2 ? fmin(y1, y3) : fmin(y1, y2);
					oLink.dH = x1 == x2 ? abs(y1 - y2) : abs(y1 - y3);
					oLink.dW = y1 == y2 ? abs(x1 - x2) : abs(x1 - x3);

					std::wstring wsNameTarget = oReader.GetText();
					if (wsNameTarget.find(L"http") == 0)
					{
						oLink.sLink = U_TO_UTF8(wsNameTarget);
						m_vLinks.push_back(oLink);
					}
					else
					{
						// координата назначения на странице назначения
						size_t nSharp = wsNameTarget.find(L'#');
						if (nSharp != std::wstring::npos)
						{
							std::map<std::wstring, int>::iterator find = m_pDocument->m_mInternalLinks.find(wsNameTarget.substr(nSharp + 1));
							if (find != m_pDocument->m_mInternalLinks.end())
							{
								oLink.sLink = '#' + std::to_string(find->second);
								m_vLinks.push_back(oLink);
							}
						}
					}
				}
			#endif

				if (!oReader.MoveToNextAttribute())
					break;

				wsAttrName = oReader.GetName();
			}
		}
		oReader.MoveToElement();
	#ifdef BUILDING_WASM_MODULE
		if (find != m_pDocument->m_vStructure.end())
		{
			std::wstring wsPath = wsPathData.c_stdstr();
			size_t nFindY = wsPath.find(L',');
			if (nFindY != std::wstring::npos)
			{
				size_t nFindEndY = wsPath.find(L' ', ++nFindY);
				if (nFindEndY != std::wstring::npos)
					// координата назначения на странице назначения
					find->dY = GetDouble(wsPath.substr(nFindY, nFindEndY - nFindY));
			}
		}
	#endif

		CBrush* pBrush = NULL;
		bool bDeleteBrush = false;
		if (!wsFill.empty())
		{
			if (IsFromResource(wsFill))
			{
				pBrush = pState->GetBrush(wsFill);
				bDeleteBrush = false;
			}
			else
			{
				pBrush = ReadBrush(wsFill.c_str(), pState->GetCurrentOpacity());
				bDeleteBrush = true;
			}
		}

		if (bStroke)
		{
			pRenderer->put_PenColor(nStrokeBgr & 0x00FFFFFF);
			pRenderer->put_PenAlpha(nStrokeAlpha * pState->GetCurrentOpacity());
		}

		if (!oReader.IsEmptyNode())
		{
			CWString wsNodeName;
			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				wsNodeName = oReader.GetName();
				if (wsNodeName == L"Path.RenderTransform")
				{
					ReadTransform(oReader, wsTransform);
				}
				else if (wsNodeName == L"Path.Clip")
				{
					ReadClip(oReader, wsClip);
				}
				else if (wsNodeName == L"Path.Fill" && !pBrush)
				{
					pBrush = ReadBrush(oReader, pState->GetCurrentOpacity());
					bDeleteBrush = true;
				}
				else if (wsNodeName == L"Path.Stroke" && !bStroke)
				{
					bStroke = StrokeToRenderer(oReader, pRenderer, pState);
				}
				else if (wsNodeName == L"Path.Data" && wsPathData.empty())
				{
					ReadPathData(oReader, wsPathData, wsPathTransform);
				}
			}
		}

		if (pBrush)
		{
			if (pBrush->IsImageBrush())
				((CImageBrush*)pBrush)->SetPaths(m_wsRootPath, GetPath(m_wsPagePath).c_str());

			bFill = pBrush->SetToRenderer(pRenderer);
			if (bDeleteBrush)
				RELEASEOBJECT(pBrush);
		}

		// Сначала задается матрица преобразования, потом клип, потому что даже
		// если преобразование задано в дочерней ноде, а клип задан в атрибутах данной ноды,
		// то преобразование влияется на клип все равно.
		if (!wsTransform.empty())
		{
			bTransform = TransformToRenderer(wsTransform.c_str(), pState);
		}

		if (!wsClip.empty())
		{
			bClip = ClipToRenderer(wsClip.c_str(), pState);
		}

		if (pDashPattern)
		{
			for (LONG lIndex = 0; lIndex < lDashPatternSize; lIndex++)
			{
				pDashPattern[lIndex] = xpsUnitToMM(pDashPattern[lIndex] * dPenSize);
			}

			pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
			pRenderer->PenDashPattern(pDashPattern, lDashPatternSize);
			pRenderer->put_PenDashOffset(xpsUnitToMM(dDashOffset * dPenSize));
			pRenderer->put_PenLineStartCap(nDashCap);
			pRenderer->put_PenLineEndCap(nDashCap);
			delete[] pDashPattern;
		}
		else
		{
			pRenderer->put_PenDashStyle(Aggplus::DashStyleSolid);
			pRenderer->put_PenLineStartCap(nStartCap);
			pRenderer->put_PenLineEndCap(nEndCap);
		}

		pRenderer->put_PenLineJoin(nJoinStyle);
		if (nJoinStyle == Aggplus::LineJoinMiter)
			pRenderer->put_PenMiterLimit(xpsUnitToMM(dMiter));
		pRenderer->put_PenSize(xpsUnitToMM(dPenSize));

		pRenderer->BeginCommand(c_nPathType);
		pRenderer->PathCommandStart();

		if (IsFromResource(wsPathData))
			pState->GetPathGeometry(wsPathData, wsPathData, wsPathTransform);

		bool bPathTransform = false;
		if (!wsPathTransform.empty())
			bPathTransform = TransformToRenderer(wsPathTransform.c_str(), pState);

		bool bWindingFillMode = VmlToRenderer(wsPathData, pRenderer);

		int nMode = bStroke ? c_nStroke : 0;
		if (bFill)
			nMode |= (bWindingFillMode ? c_nWindingFillMode : c_nEvenOddFillMode);

		pRenderer->DrawPath(nMode);

		pRenderer->EndCommand(c_nPathType);
		pRenderer->PathCommandEnd();

		if (bPathTransform)
			pState->PopTransform();

		if (bTransform)
			pState->PopTransform();

		if (bClip)
			pState->PopClip();

		if (bOpacity)
			pState->PopOpacity();
	}	
	bool Page::StrokeToRenderer(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState)
	{
		if (!oReader.IsEmptyNode())
		{
			std::wstring wsNodeName;
			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				wsNodeName = oReader.GetName();
				wsNodeName = RemoveNamespace(wsNodeName);

				if (L"SolidColorBrush" == wsNodeName)
				{
					int nBgr, nAlpha;
					std::wstring wsColor;
					ReadAttribute(oReader, L"Color", wsColor);
					GetBgra(wsColor, nBgr, nAlpha);
					pRenderer->put_PenColor(nBgr & 0x00FFFFFF);
					pRenderer->put_PenAlpha((double)nAlpha * pState->GetCurrentOpacity());
					return true;
				}
			}
		}

		return false;
	}
	void Page::ReadPathData(XmlUtils::CXmlLiteReader& oReader, CWString& wsData, CWString& wsTransform)
	{
		wsData = L"";

		if (oReader.IsEmptyNode())
			return;

		CWString wsNodeName;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetName();
			if (wsNodeName == L"PathGeometry")
				return ReadPathGeometry(oReader, wsData, wsTransform);
		}
	}
}
