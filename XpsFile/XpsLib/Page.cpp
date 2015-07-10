#include "Page.h"
#include <stdio.h>
#include "../../DesktopEditor/common/String.h"
#include "../../DesktopEditor/graphics/structures.h"
#include "Document.h"

#include <ctime>

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifndef xpsUnitToMM
#define xpsUnitToMM(x) ((x) * 25.4 / 96)
#endif

namespace XPS
{
	Page::Page(const std::wstring& wsPagePath, const std::wstring& wsRootPath, CFontList* pFontList, CFontManager* pFontManager, CDocument* pDocument)
	{
		m_wsPagePath   = wsPagePath;
		m_wsRootPath   = wsRootPath;
		m_pFontList    = pFontList;
		m_pFontManager = pFontManager;
		m_pDocument    = pDocument;

		m_pStaticResource       = NULL;
		m_bDeleteStaticResource = false;

		m_nCounter = 0;
	}
	Page::~Page()
	{
		if (m_bDeleteStaticResource)
			RELEASEOBJECT(m_pStaticResource);
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

		std::wstring wsName = oReader.GetName();
		if (L"FixedPage" != wsName)
			return;

		ReadAttribute(oReader, L"Width", wsName);
		nW = XmlUtils::GetInteger(wsName.c_str());

		ReadAttribute(oReader, L"Height", wsName);
		nH = XmlUtils::GetInteger(wsName.c_str());
	}
	void Page::Draw(IRenderer* pRenderer, bool* pbBreak)
	{
		XmlUtils::CXmlLiteReader oReader;

		clock_t oBeginTime = clock();

		if (!oReader.FromFile(m_wsPagePath.c_str()))
			return;

		clock_t oEndTime = clock();
		double dElapsedSecs = double(oEndTime - oBeginTime) / CLOCKS_PER_SEC;
		printf("%S %fseconds\n", m_wsPagePath.c_str(), dElapsedSecs);

		if (!oReader.ReadNextNode())
			return;

		std::wstring wsNodeName = oReader.GetName();
		if (L"FixedPage" != wsNodeName)
			return;

		CContextState oState(pRenderer);
		DrawCanvas(oReader, pRenderer, &oState, pbBreak);
	}
	void Page::DrawCanvas(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState, bool* pbBreak)
	{
		bool bTransform = false, bClip = false, bOpacity = false;
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

		std::wstring wsNodeName;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetName();
			wsNodeName = RemoveNamespace(wsNodeName);

			if (L"FixedPage.Resources" == wsNodeName)
			{
				ReadPageResources(oReader, pRenderer, pState);
			}
			else if (L"Glyphs" == wsNodeName)
			{
				DrawGlyph(oReader, pRenderer, pState);
			}
			else if (L"Canvas" == wsNodeName)
			{
				DrawCanvas(oReader, pRenderer, pState, pbBreak);
			}
			else if (L"Canvas.RenderTransform" == wsNodeName)
			{
				if (!bTransform)
					bTransform = ReadTransform(oReader, pRenderer, pState);
			}
			else if (L"Path" == wsNodeName)
			{
				DrawPath(oReader, pRenderer, pState);
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
	}
	void Page::ReadPageResources(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState)
	{
		if (oReader.IsEmptyNode())
			return;

		std::wstring wsNodeName;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetName();
			if (L"ResourceDictionary" == wsNodeName)
			{
				std::wstring wsSource;
				ReadAttribute(oReader, L"Source", wsSource);
				if (!wsSource.empty())
				{
					std::wstring wsPath = m_wsRootPath + wsSource;

					m_pStaticResource = m_pDocument->GetStaticResource(wsPath);
					m_bDeleteStaticResource = false;					
				}
				else
				{
					m_pStaticResource = new CStaticResource(oReader);
					m_bDeleteStaticResource = true;
				}
			}
		}
	}	
	bool Page::ClipToRenderer(const wchar_t* wsString, CContextState* pState)
	{
		CWString wsClip;
		wsClip.create(wsString, true);
		if (!wsClip.empty())
		{
			if ('{' == wsClip[0] && wsClip.size() >= 17)
			{
				CWString wsKey((wchar_t*)(wsClip.c_str() + 16), false, wsClip.size() - 17);
				wsClip = m_pStaticResource->Get(wsKey.c_str());
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
		int nTextLen = 0;
		std::wstring wsText;
		double dX = 0;
		double dY = 0;
		std::wstring wsFontPath;
		std::wstring wsIndicies;
		int nBidiLevel = 0;
		CWString wsClip, wsTransform;
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
					wsText = oReader.GetText();
					nTextLen = wsText.length();

					if (nTextLen >= 2 && '{' == wsText.at(0) && '}' == wsText.at(1))
					{
						wsText = wsText.substr(2);
						nTextLen -= 2;
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
					wsTransform = ReadMatrixTransform(oReader);
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

		std::wstring wsChar = wsText.substr(0, 1);
		std::vector<std::vector<std::wstring>> arrElements = Split(wsIndicies, L';', L',');

		bool bRtoL = (nBidiLevel % 2 ? true : false);
		m_pFontManager->LoadFontFromFile(wsFontPath, 0, (float)(dFontSize * 0.75), 96, 96);
		for (int nIndex = 0; nIndex < nTextLen; nIndex++)
		{
			if (nIndex >= arrElements.size())
				arrElements.push_back(std::vector<std::wstring>());

			if (bRtoL)
			{
				if (arrElements.at(nIndex).size() >= 2)
				{
					dX -= GetDouble(arrElements.at(nIndex).at(1)) * dFontSize / 100.0;
				}
				else
				{
					m_pFontManager->LoadString1(wsChar, 0, 0);
					TBBox oBox = m_pFontManager->MeasureString2();
					dX -= (oBox.fMaxX - oBox.fMinX);
				}
				pRenderer->CommandDrawText(wsChar, xpsUnitToMM(dX), xpsUnitToMM(dY), 0, 0, 0);
			}
			else
			{
				pRenderer->CommandDrawText(wsChar, xpsUnitToMM(dX), xpsUnitToMM(dY), 0, 0, 0);
				if (arrElements.at(nIndex).size() >= 2)
				{
					dX += GetDouble(arrElements.at(nIndex).at(1)) * dFontSize / 100.0;
				}
				else
				{
					m_pFontManager->LoadString1(wsChar, 0, 0);
					TBBox oBox = m_pFontManager->MeasureString2();
					dX += (oBox.fMaxX - oBox.fMinX);
				}
			}

			wsChar = wsText.substr(nIndex + 1, 1);
		}

		if (bClip)
			pState->PopClip();

		if (bTransform)
			pState->PopTransform();

		if (bOpacity)
			pState->PopOpacity();
	}
	bool Page::ReadTransform(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState)
	{
		std::wstring wsNodeName;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetName();
			wsNodeName = RemoveNamespace(wsNodeName);

			if (L"MatrixTransform" == wsNodeName)
			{
				std::wstring wsMatrix;
				ReadAttribute(oReader, L"Matrix", wsMatrix);
				return TransformToRenderer(wsMatrix.c_str(), pState);
			}
		}

		return false;
	}
	CWString Page::ReadMatrixTransform(XmlUtils::CXmlLiteReader& oReader)
	{
		CWString wsNodeName, wsTransform;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetName();
			if (wsNodeName ==  L"MatrixTransform")
			{
				ReadAttribute(oReader, L"Matrix", wsTransform);
				break;
			}
		}
		return wsTransform;
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
		bool bFillSetted = false;
		double dPenSize = 1.0;
		std::wstring wsData;
		bool bStroke = false;

		int nFillBgr = 0, nFillAlpha = 255, nStrokeBgr = 0, nStrokeAlpha = 255;

		BYTE nDashCap   = Aggplus::LineCapFlat;
		BYTE nStartCap  = Aggplus::LineCapFlat;
		BYTE nEndCap    = Aggplus::LineCapFlat;
		BYTE nJoinStyle = Aggplus::LineJoinMiter;
		double dMiter   = 10.0;

		double* pDashPattern  = NULL;
		LONG lDashPatternSize = 0;
		double dDashOffset    = 0.0;

		CWString wsClip, wsTransform;

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
					bFillSetted = true;
					std::wstring wsFill = oReader.GetText();
					GetBgra(wsFill, nFillBgr, nFillAlpha);
				}
				else if (L"Data" == wsAttrName)
				{
					wsData = oReader.GetText();
				}

				if (!oReader.MoveToNextAttribute())
					break;

				wsAttrName = oReader.GetName();
			}
		}
		oReader.MoveToElement();

		bool bFill = bFillSetted;
		if (!bFillSetted)
		{
			if (!oReader.IsEmptyNode())
			{
				CWString wsNodeName;
				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					wsNodeName = oReader.GetName();
					if (wsNodeName == L"Path.Fill")
					{
						bFill = FillToRenderer(oReader, pRenderer);
					}
					else if (wsNodeName == L"Path.Data")
					{
						ReadPathData(oReader, wsData);
					}
					else if (wsNodeName == L"Path.RenderTransform")
					{
						wsTransform = ReadMatrixTransform(oReader);
					}
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

		pRenderer->put_PenLineJoin(nJoinStyle);
		if (nJoinStyle == Aggplus::LineJoinMiter)
			pRenderer->put_PenMiterLimit(xpsUnitToMM(dMiter));
		pRenderer->put_PenSize(xpsUnitToMM(dPenSize));

		pRenderer->BeginCommand(c_nPathType);
		pRenderer->PathCommandStart();

		bool bWindingFillMode = false;
		if (L'{' == wsData[0])
		{
			const wchar_t* wsValue = NULL;			
			if (m_pStaticResource)
			{
				CWString wsKey(((wchar_t*)wsData.c_str() + 16), false, wsData.length() - 17);
				wsValue = m_pStaticResource->Get(wsKey);
			}

			if (NULL != wsValue)
			{
				bWindingFillMode = VmlToRenderer(wsValue, pRenderer);
			}
			else
			{
				pRenderer->EndCommand(c_nPathType);
				pRenderer->PathCommandEnd();

				if (bClip)
					pState->PopClip();

				if (bTransform)
					pState->PopTransform();

				if (bOpacity)
					pState->PopOpacity();

				return;
			}
		}
		else
			bWindingFillMode = VmlToRenderer(wsData.c_str(), pRenderer);

		int nMode = bStroke ? c_nStroke : 0;
		if (bFill)
			nMode |= (bWindingFillMode ? c_nWindingFillMode : c_nEvenOddFillMode);

		pRenderer->DrawPath(nMode);
		pRenderer->EndCommand(c_nPathType);
		pRenderer->PathCommandEnd();

		if (bTransform)
			pState->PopTransform();

		if (bClip)
			pState->PopClip();

		if (bOpacity)
			pState->PopOpacity();
	}
	bool Page::FillToRenderer(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer)
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
					pRenderer->put_BrushAlpha1(nAlpha);
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
	void Page::ReadPathData(XmlUtils::CXmlLiteReader& oReader, std::wstring& wsData)
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
			{
				return ReadPathGeometry(oReader, wsData);
			}
		}
	}
	void Page::ReadPathGeometry(XmlUtils::CXmlLiteReader& oReader, std::wstring& wsData)
	{
		if (oReader.IsEmptyNode())
			return;

		std::wstring wsFillMode;
		ReadAttribute(oReader, L"FillRule", wsFillMode);

		if (L"EvenOdd" == wsFillMode)
			wsData += L"F 0 ";
		else
			wsData += L"F 1 ";

		std::wstring wsNodeName;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetName();
			if (L"PathFigure" == wsNodeName)
			{
				ReadPathFigure(oReader, wsData);
			}
		}
	}
	void Page::ReadPathFigure(XmlUtils::CXmlLiteReader& oReader, std::wstring& wsData)
	{
		if (oReader.IsEmptyNode())
			return;

		std::wstring wsStartPoint;
		ReadAttribute(oReader, L"StartPoint", wsStartPoint);
		wsData += L" M " + wsStartPoint;

		std::wstring wsNodeName;
		std::wstring wsText;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetName();
			wsText.empty();
			if (L"PolyLineSegment" == wsNodeName)
			{
				ReadAttribute(oReader, L"Points", wsText);
				wsData += L" L " + wsText;
			}
			else if (L"PolyBezierSegment" == wsNodeName)
			{
				ReadAttribute(oReader, L"Points", wsText);
				wsData += L" C " + wsText;
			}
			else if (L"PolyQuadraticBezierSegment" == wsNodeName)
			{
				ReadAttribute(oReader, L"Points", wsText);
				wsData += L" Q " + wsText;
			}
			else if (L"ArcSegment" == wsNodeName)
			{
				std::wstring wsSize, wsRotationAngle, wsIsLargeArc, wsSweepDirection, wsPoint;
				if (oReader.MoveToFirstAttribute())
				{
					std::wstring wsAttrName = oReader.GetName();
					while (!wsAttrName.empty())
					{
						if (L"Size" == wsAttrName)
							wsSize = oReader.GetText();
						else if (L"RotationAngle" == wsAttrName)
							wsRotationAngle = oReader.GetText();
						else if (L"IsLargeArc" == wsAttrName)
							wsIsLargeArc = oReader.GetText();
						else if (L"SweepDirection" == wsAttrName)
							wsSweepDirection = oReader.GetText();
						else if (L"Point" == wsAttrName)
							wsPoint = oReader.GetText();

						if (!oReader.MoveToNextAttribute())
							break;

						wsAttrName = oReader.GetName();
					}
					oReader.MoveToElement();
				}

				wsData += L" A " + wsSize + L" " + wsRotationAngle + L" ";
				if (GetBool(wsIsLargeArc))
					wsData += L"1 ";
				else
					wsData += L"0 ";

				if (L"Counterclockwise" == wsSweepDirection)
					wsData += L"0 ";
				else
					wsData += L"1 ";

				wsData += wsPoint;
			}			
		}

		std::wstring wsClosed;
		ReadAttribute(oReader, L"IsClosed", wsClosed);
		if (GetBool(wsClosed))
			wsData += L" Z ";
	}
}