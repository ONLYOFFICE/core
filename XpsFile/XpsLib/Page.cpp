#include "Page.h"
#include <stdio.h>
#include "../../DesktopEditor/common/String.h"
#include "../../DesktopEditor/graphics/structures.h"

#include "Document.h"
#include "StaticResources.h"

#include <ctime>

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifndef xpsUnitToMM
#define xpsUnitToMM(x) ((x) * 25.4 / 96)
#endif

#define IsFromResource(String) (!String.empty() && '{' == String[0])

namespace XPS
{
	static double GetAdvance(CFontManager* pFontManager, const std::wstring& wsChar, const std::wstring& wsGid, const bool& bGid)
	{
		if (bGid)
		{
			pFontManager->SetStringGID(TRUE);
			pFontManager->LoadString1(wsGid, 0, 0);
		}
		else
		{
			pFontManager->SetStringGID(FALSE);
			pFontManager->LoadString1(wsChar, 0, 0);
		}
		TBBox oBox = pFontManager->MeasureString2();
		return (oBox.fMaxX - oBox.fMinX);
	}
	Page::Page(const std::wstring& wsPagePath, const std::wstring& wsRootPath, CFontList* pFontList, CFontManager* pFontManager, CDocument* pDocument)
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
		clock_t oBeginTime = clock();

		if (!oReader.FromFile(m_wsPagePath))
			return;

		clock_t oEndTime = clock();
		double dElapsedSecs = double(oEndTime - oBeginTime) / CLOCKS_PER_SEC;
		printf("%S %fseconds\n", m_wsPagePath.c_str(), dElapsedSecs);

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
										nW = XmlUtils::GetInteger(wsAttrName.c_str());

										ReadAttribute(oReader, L"Height", wsAttrName);
										nH = XmlUtils::GetInteger(wsAttrName.c_str());
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
									nW = XmlUtils::GetInteger(wsAttrName.c_str());

									ReadAttribute(oReader, L"Height", wsAttrName);
									nH = XmlUtils::GetInteger(wsAttrName.c_str());
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
			nW = XmlUtils::GetInteger(wsAttrName.c_str());

			ReadAttribute(oReader, L"Height", wsAttrName);
			nH = XmlUtils::GetInteger(wsAttrName.c_str());
		}
	}
	void Page::Draw(IRenderer* pRenderer, bool* pbBreak)
	{
		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(m_wsPagePath.c_str()))
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
	}
	void Page::DrawCanvas(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState, bool* pbBreak)
	{
		bool bTransform = false, bClip = false, bOpacity = false, bResource = false;
		if (oReader.MoveToFirstAttribute())
		{
			CWString wsAttrName = oReader.GetName();
			while (!wsAttrName.empty())
			{
				if (wsAttrName == L"Clip")
					bClip = ClipToRenderer(oReader.GetText(), pState);
				else if (wsAttrName == L"RenderTransform")
					bTransform = TransformToRenderer(oReader.GetText(), pState);
				else if (wsAttrName == L"Opacity")
				{
					pState->PushOpacity(GetDouble(oReader.GetText()));
					bOpacity = true;
				}

				if (!oReader.MoveToNextAttribute())
					break;

				wsAttrName = oReader.GetName();
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
					std::wstring wsPath = m_wsRootPath + wsSource.c_str();
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
			std::vector<std::wstring> arrElements = NSStringExt::Split(wsString, L',');
			double arrRes[6] ={ 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
			for (int nIndex = 0, nCount = min(6, arrElements.size()); nIndex < nCount; nIndex++)
				arrRes[nIndex] = GetDouble(arrElements[nIndex]);

			pState->PushTransform(arrRes);
			return true;
		}
		return false;
	}
	void Page::DrawGlyph(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState)
	{
		int nBgr = 0, nAlpha = 255;
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

		if (oReader.MoveToFirstAttribute())
		{
			std::wstring wsAttrName = oReader.GetName();
			while (!wsAttrName.empty())
			{
				if (L"FontUri" == wsAttrName)
				{
					wsFontPath = oReader.GetText();
					std::wstring wsFontName = GetFileName(wsFontPath);
					wsFontPath = m_wsRootPath + L"/" + wsFontPath;
					std::wstring wsExt = GetFileExtension(wsFontPath);
					NSStringExt::ToLower(wsExt);
					if (L"odttf" == wsExt)
					{
						NSStringExt::ToLower(wsFontName);
						m_pFontList->Check(wsFontName, wsFontPath);
					}
					wsFontPath = NormalizePath(wsFontPath);
					pRenderer->put_FontPath(wsFontPath);
				}
				else if (wsAttrName == L"Opacity")
				{
					pState->PushOpacity(GetDouble(oReader.GetText()));
					bOpacity = true;
				}
				else if (L"Clip" == wsAttrName)
				{
					wsClip.create(oReader.GetText(), true);
				}
				else if (L"Fill" == wsAttrName)
				{
					std::wstring wsFontColor = oReader.GetText();
					ReadAttribute(oReader, L"Fill", wsFontColor);
					GetBgra(wsFontColor, nBgr, nAlpha);
				}
				else if (L"StyleSimulations" == wsAttrName)
				{
					std::wstring wsFontStyle = oReader.GetText();
					if (L"ItalicSimulation" == wsFontStyle)
						pRenderer->put_FontStyle(0x02);
					else if (L"BoldSimulation" == wsFontStyle)
						pRenderer->put_FontStyle(0x01);
					else if (L"BoldItalicSimulation" == wsFontStyle)
						pRenderer->put_FontStyle(0x03);
				}
				else if (L"FontRenderingEmSize" == wsAttrName)
				{
					std::wstring wsFontSize = oReader.GetText();
					dFontSize = GetDouble(wsFontSize);
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
					std::wstring wsTextX = oReader.GetText();
					dX = GetDouble(wsTextX);
				}
				else if (L"OriginY" == wsAttrName)
				{
					std::wstring wsTextY = oReader.GetText();
					dY = GetDouble(wsTextY);
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

				if (!oReader.MoveToNextAttribute())
					break;

				wsAttrName = oReader.GetName();
			}
		}
		oReader.MoveToElement();

		if (!oReader.IsEmptyNode())
		{
			std::wstring wsNodeName;
			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				wsNodeName = oReader.GetName();
				wsNodeName = RemoveNamespace(wsNodeName);

				if (L"Glyphs.RenderTransform" == wsNodeName)
				{
					ReadTransform(oReader, wsTransform);
				}
			}
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

		pRenderer->put_BrushColor1(nBgr & 0x00FFFFFF);
		pRenderer->put_BrushAlpha1(((double)nAlpha * pState->GetCurrentOpacity()));
		pRenderer->put_BrushType(c_BrushTypeSolid);
		pRenderer->put_FontSize(dFontSize * 0.75);

		TIndicesEntry oEntry;
		int nIndicesPos = 0, nIndicesLen = wsIndices.size();
		int nUtf16Pos = 0;
		bool bRtoL = (nBidiLevel % 2 ? true : false);
		m_pFontManager->LoadFontFromFile(wsFontPath, 0, (float)(dFontSize * 0.75), 96, 96);
		double dFontKoef = dFontSize / 100.0;

		while (GetNextGlyph(wsIndices.c_str(), nIndicesPos, nIndicesLen, pUtf16, nUtf16Pos, unUtf16Len, oEntry))
		{
			std::wstring wsChar = NSStringExt::CConverter::GetUnicodeFromUTF32((const unsigned int*)(&(oEntry.nUnicode)), 1);
			unsigned int unGid = oEntry.nGid;
			std::wstring wsGid  = oEntry.bGid ? NSStringExt::CConverter::GetUnicodeFromUTF32((const unsigned int*)(&(unGid)), 1) : L"";

			double dAdvance, dRealAdvance;
			if (oEntry.bAdvance)
			{
				dAdvance = oEntry.dAdvance * dFontKoef;

				if (bRtoL)
					dRealAdvance = GetAdvance(m_pFontManager, wsChar, wsGid, oEntry.bGid);
				else
					dRealAdvance = dAdvance;
			}
			else
			{
				dAdvance = GetAdvance(m_pFontManager, wsChar, wsGid, oEntry.bGid);
				dRealAdvance = dAdvance;
			}

			if (bRtoL)
				dX -= dRealAdvance;

			if (oEntry.bHorOffset || oEntry.bVerOffset)
				pRenderer->CommandDrawTextEx(wsChar, wsGid, xpsUnitToMM(dX + (bRtoL ? -oEntry.dHorOffset * dFontKoef : oEntry.dHorOffset * dFontKoef)), xpsUnitToMM(dY - oEntry.dVerOffset * dFontKoef), 0, 0, 0, 0);
			else
				pRenderer->CommandDrawTextEx(wsChar, wsGid, xpsUnitToMM(dX), xpsUnitToMM(dY), 0, 0, 0, 0);

			if (!bRtoL)
				dX += dAdvance;
			else
				dX -= (dAdvance - dRealAdvance);
		}

		RELEASEARRAYOBJECTS(pUtf16Ptr);

		if (bClip)
			pState->PopClip();

		if (bTransform)
			pState->PopTransform();

		if (bOpacity)
			pState->PopOpacity();
	}
	CWString Page::ReadClip(XmlUtils::CXmlLiteReader& oReader)
	{
		CWString wsClip;
		// TODO: Реализовать чтение Clip
		return wsClip;
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

		CWString wsClip, wsTransform, wsPathData, wsPathTransform;
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
					pState->PushOpacity(GetDouble(oReader.GetText()));
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
					nDashCap = GetCapStyle(oReader.GetText());
				}
				else if (L"StrokeEndLineCap" == wsAttrName)
				{
					nEndCap = GetCapStyle(oReader.GetText());
				}
				else if (L"StrokeStartLineCap" == wsAttrName)
				{
					nStartCap = GetCapStyle(oReader.GetText());
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
					CWString wsMiterLimit = oReader.GetText();
					dMiter = GetDouble(wsMiterLimit.c_str());
				}
				else if (L"Fill" == wsAttrName)
				{
					bFill = true;
					std::wstring wsFill = oReader.GetText();
					GetBgra(wsFill, nFillBgr, nFillAlpha);
				}
				else if (L"Data" == wsAttrName)
				{
					wsPathData.create(oReader.GetText(), true);
				}

				if (!oReader.MoveToNextAttribute())
					break;

				wsAttrName = oReader.GetName();
			}
		}
		oReader.MoveToElement();

		if (bStroke)
		{
			pRenderer->put_PenColor(nStrokeBgr & 0x00FFFFFF);
			pRenderer->put_PenAlpha(nStrokeAlpha * pState->GetCurrentOpacity());
		}

		if (bFill)
		{
			pRenderer->put_BrushType(c_BrushTypeSolid);
			pRenderer->put_BrushColor1(nFillBgr & 0x00FFFFFF);
			pRenderer->put_BrushAlpha1(nFillAlpha * pState->GetCurrentOpacity());
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
					wsClip = ReadClip(oReader);
				}
				else if (wsNodeName == L"Path.Fill" && !bFill)
				{
					bFill = FillToRenderer(oReader, pRenderer, pState);
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
			pRenderer->put_PenDashOffset(dDashOffset);
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

		bool bWindingFillMode = VmlToRenderer(wsPathData.c_str(), pRenderer);

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
	bool Page::FillToRenderer  (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState)
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
					pRenderer->put_BrushType(c_BrushTypeSolid);
					pRenderer->put_BrushColor1(nBgr & 0x00FFFFFF);
					pRenderer->put_BrushAlpha1((double)nAlpha * pState->GetCurrentOpacity());
					return true;
				}
				else if (L"ImageBrush" == wsNodeName)
				{
					std::wstring wsImageSource;
					ReadAttribute(oReader, L"ImageSource", wsImageSource);
					pRenderer->put_BrushType(c_BrushTypeTexture);
					pRenderer->put_BrushTexturePath(m_wsRootPath + wsImageSource);
					return true;
				}
			}
		}

		return false;
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