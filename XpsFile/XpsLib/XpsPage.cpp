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
//#include <fstream>

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

		CWString wsNodeName = oReader.GetNameNoNS(), wsAttrName;
		if (wsNodeName == L"AlternateContent")
		{
			if (!oReader.IsEmptyNode())
			{
				int nAltDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nAltDepth))
				{
					wsNodeName = oReader.GetNameNoNS();
					if (wsNodeName == L"Choice")
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
									wsNodeName = oReader.GetNameNoNS();
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
					else if (wsNodeName == L"Fallback")
					{
						if (!oReader.IsEmptyNode())
						{
							int nAltDepth2 = oReader.GetDepth();
							while (oReader.ReadNextSiblingNode(nAltDepth2))
							{
								wsNodeName = oReader.GetNameNoNS();
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
	void Page::Draw(IRenderer* pRenderer, bool* pbBreak)
	{
		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromStringA(m_wsRootPath->readXml(m_wsPagePath)))
			return;

		if (!oReader.ReadNextNode())
			return;

		CContextState oState(pRenderer);
		CWString wsNodeName = oReader.GetNameNoNS();
		if (wsNodeName == L"AlternateContent")
		{
			if (!oReader.IsEmptyNode())
			{
				int nAltDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nAltDepth))
				{
					wsNodeName = oReader.GetNameNoNS();
					if (wsNodeName == L"Choice")
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
									wsNodeName = oReader.GetNameNoNS();
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
					else if (wsNodeName == L"Fallback")
					{
						if (!oReader.IsEmptyNode())
						{
							int nAltDepth2 = oReader.GetDepth();
							while (oReader.ReadNextSiblingNode(nAltDepth2))
							{
								wsNodeName = oReader.GetNameNoNS();
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
			wsNodeName = oReader.GetNameNoNS();

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
			else if (wsNodeName == L"AlternateContent")
			{
				if (!oReader.IsEmptyNode())
				{
					int nAltDepth = oReader.GetDepth();
					while (oReader.ReadNextSiblingNode(nAltDepth))
					{
						wsNodeName = oReader.GetNameNoNS();
						if (wsNodeName == L"Choice")
						{
							CWString wsAttr;
							ReadAttribute(oReader, L"Requires", wsAttr);
							if (wsAttr == L"xps")
							{
								DrawCanvas(oReader, pRenderer, pState, NULL);
								break;
							}
						}
						else if (wsNodeName == L"Fallback")
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
			wsNodeName = oReader.GetNameNoNS();
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
                    wsFontPath = m_wsRootPath->getFullFilePath(wsFontPath);

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
				wsNodeName = oReader.GetNameNoNS();
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

		std::wstring sFullFontPath = m_wsRootPath->getFullFilePath(wsFontPath);
		if (NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage())
		{
			NSFonts::IFontStream* pMemoryStream = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->Get(sFullFontPath);
			if (!pMemoryStream)
				sFullFontPath = L"";
		}

		if (!sFullFontPath.empty())
			m_pFontManager->LoadFontFromFile(sFullFontPath, 0, (float)(dFontSize * 0.75), 96, 96);

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
                }
				else
                {
                    LONG nRenType = 0;
                    pRenderer->get_Type(&nRenType);
                    if (c_nGrRenderer == nRenType)
                        pRenderer->put_FontStringGID(FALSE);

                    pRenderer->CommandDrawTextCHAR(oEntry.nUnicode,  xpsUnitToMM(dXorigin), xpsUnitToMM(dYorigin), 0, 0);
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
                }
				else
                {
					pRenderer->CommandDrawTextCHAR(oEntry.nUnicode, 0, 0, 0, 0);
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
		std::vector<CDocument::CDocumentStructure>::iterator find = m_pDocument->m_vStructure.end();
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

                    NSWasm::CPageLinkItem oLink = {"", 0, 0, 0, 0, 0};
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
                    oLink.X = x1 == x2 ? fmin(x1, x3) : fmin(x1, x2);
                    oLink.Y = y1 == y2 ? fmin(y1, y3) : fmin(y1, y2);
                    oLink.H = x1 == x2 ? abs(y1 - y2) : abs(y1 - y3);
                    oLink.W = y1 == y2 ? abs(x1 - x2) : abs(x1 - x3);

					std::wstring wsNameTarget = oReader.GetText();
					if (wsNameTarget.find(L"http") == 0)
					{
                        oLink.Link = U_TO_UTF8(wsNameTarget);
                        m_oLinks.m_arLinks.push_back(oLink);
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
                                oLink.Link = '#' + std::to_string(find->second);
                                m_oLinks.m_arLinks.push_back(oLink);
							}
						}
					}
				}

				if (!oReader.MoveToNextAttribute())
					break;

				wsAttrName = oReader.GetName();
			}
		}
		oReader.MoveToElement();
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
				wsNodeName = oReader.GetNameNoNS();
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
				wsNodeName = oReader.GetNameNoNS();
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
			wsNodeName = oReader.GetNameNoNS();
			if (wsNodeName == L"PathGeometry")
				return ReadPathGeometry(oReader, wsData, wsTransform);
		}
	}
}
