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
	static inline void PageCommandL(std::vector<std::wstring>& arrElements, double& dCurX, double& dCurY, IRenderer* pRenderer, int& nPos)
	{
		dCurX = GetDouble(arrElements[nPos + 1]);
		dCurY = GetDouble(arrElements[nPos + 2]);
		nPos += 2;
		pRenderer->PathCommandLineTo(xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
	}
	static inline void PageCommandA(std::vector<std::wstring>& arrElements, double& dCurX, double& dCurY, IRenderer* pRenderer, int& nPos)
	{
		Aggplus::CMatrix tmatx, rmatx, smatx, itmatx, irmatx, ismatx;
		double x_cur_temp = dCurX;
		double y_cur_temp = dCurY;
		double x_end = GetDouble(arrElements[nPos + 6]);
		double y_end = GetDouble(arrElements[nPos + 7]);

		tmatx.Translate(-dCurX, -dCurY);
		tmatx.TransformPoint(x_cur_temp, y_cur_temp);
		tmatx.TransformPoint(x_end, y_end);

		double angle = GetDouble(arrElements[nPos + 3]);
		rmatx.Rotate(-angle);
		rmatx.TransformPoint(x_cur_temp, y_cur_temp);
		rmatx.TransformPoint(x_end, y_end);

		double rx = GetDouble(arrElements[nPos + 1]);
		double ry = GetDouble(arrElements[nPos + 2]);
		smatx.Scale(ry / rx, 1);
		smatx.TransformPoint(x_cur_temp, y_cur_temp);
		smatx.TransformPoint(x_end, y_end);

		double mid_x = (x_cur_temp + x_end) / 2.0, mid_y = (y_cur_temp + y_end) / 2.0;
		double vect_x = x_end - x_cur_temp, vect_y = y_end - y_cur_temp;
		double length = sqrt(vect_x * vect_x + vect_y * vect_y);
		double halfChord = length / 2.0;
		bool isLarge = GetBool(arrElements[nPos + 4]);
		bool isCCW	 = !GetBool(arrElements[nPos + 5]);
		double rotated_x, rotated_y;

		if (isLarge == isCCW)
		{
			rotated_x = -vect_y;
			rotated_y = vect_x;
		}
		else
		{
			rotated_x = vect_y;
			rotated_y = -vect_x;
		}

		rotated_x = rotated_x / length;
		rotated_y = rotated_y / length;

		double centerDistance = sqrt(max(ry * ry - halfChord * halfChord, 0));
		double center_x = mid_x + centerDistance * rotated_x, center_y = mid_y + centerDistance * rotated_y;

		double angle1 = atan2(y_cur_temp - center_y, x_cur_temp - center_x);
		double angle2 = atan2(y_end - center_y, x_end - center_x);

		if ((!isCCW) && (abs(y_end - center_y) < 0.000001) && (x_end - center_x < 0))
			angle2 = -M_PI;
		if ((isCCW) && (abs(y_cur_temp - center_y) < 0.000001) && (x_cur_temp - center_x < 0))
			angle1 = -M_PI;

		if (isLarge == (abs(angle2 - angle1) < M_PI))
		{
			if (angle1 < angle2)
				angle1 += 2 * M_PI;
			else
				angle2 += 2 * M_PI;
		}
		if (abs(angle2 - angle1) == M_PI)
		{
			if ((angle1 < angle2) && (isCCW) && (angle1 != -M_PI) && (angle1 != 0.0) && (angle1 != M_PI))
				angle1 += 2 * M_PI;
		}

		itmatx.Translate(dCurX, dCurY);
		irmatx.Rotate(angle);
		ismatx.Scale(rx / ry, 1);

		int max = (int)((4 * (rx + ry) * abs(angle2 - angle1) / (2 * M_PI)) / 1/*tolerance*/);

		double x, y;
		for (int counter = 0; counter <= max; counter++)
		{
			double angle_cur = ((max - counter) * angle1 + counter * angle2) / max;
			x = center_x + ry * cos(angle_cur);
			y = center_y + ry * sin(angle_cur);

			// Transform the point back
			ismatx.TransformPoint(x, y);
			irmatx.TransformPoint(x, y);
			itmatx.TransformPoint(x, y);
			pRenderer->PathCommandLineTo(xpsUnitToMM(x), xpsUnitToMM(y));
		}

		dCurX = x;
		dCurY = y;
		nPos += 7;
	}
	static inline void PageCommandC(std::vector<std::wstring>& arrElements, double& dCurX, double& dCurY, double& dCpX, double& dCpY, IRenderer* pRenderer, int& nPos)
	{
		dCpX  = GetDouble(arrElements[nPos + 3]);
		dCpY  = GetDouble(arrElements[nPos + 4]);
		dCurX = GetDouble(arrElements[nPos + 5]);
		dCurY = GetDouble(arrElements[nPos + 6]);

		pRenderer->PathCommandCurveTo(xpsUnitToMM(GetDouble(arrElements[nPos + 1])), xpsUnitToMM(GetDouble(arrElements[nPos + 2])), xpsUnitToMM(dCpX), xpsUnitToMM(dCpY), xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
		nPos += 6;
	}
	static inline void PageCommandQ(std::vector<std::wstring>& arrElements, double& dCurX, double& dCurY, IRenderer* pRenderer, int& nPos)
	{
		double x1 = 2.0 * GetDouble(arrElements[nPos + 1]);
		double y1 = 2.0 * GetDouble(arrElements[nPos + 2]);
		double x2 =       GetDouble(arrElements[nPos + 3]);
		double y2 =       GetDouble(arrElements[nPos + 4]);

		pRenderer->PathCommandCurveTo(xpsUnitToMM((dCurX + x1) / 3.0), xpsUnitToMM((dCurY + y1) / 3.0), xpsUnitToMM((x1 + x2) / 3.0), xpsUnitToMM((y1 + y2) / 3.0), xpsUnitToMM(x2), xpsUnitToMM(y2));
		dCurX = x2;
		dCurY = y2;
		nPos += 4;
	}
	static inline void PageCommandS(std::vector<std::wstring>& arrElements, double& dCurX, double& dCurY, double& dCpX, double& dCpY, IRenderer* pRenderer, int& nPos)
	{
		dCpX = 2 * dCurX - dCpX;
		dCpY = 2 * dCurY - dCpY;

		dCurX = GetDouble(arrElements[nPos + 3]);
		dCurY = GetDouble(arrElements[nPos + 4]);
		pRenderer->PathCommandCurveTo(xpsUnitToMM(dCpX), xpsUnitToMM(dCpY),
									  xpsUnitToMM(GetDouble(arrElements[nPos + 1])), xpsUnitToMM(GetDouble(arrElements[nPos + 2])),
									  xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));

		dCpX = GetDouble(arrElements[nPos + 1]);
		dCpY = GetDouble(arrElements[nPos + 2]);
		nPos += 4;
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

		CContextState oState;
		DrawCanvas(oReader, pRenderer, &oState, pbBreak);




		/*XmlUtils::CXmlNode oNode;


		clock_t oBeginTime = clock();

		if (!oNode.FromXmlFile(m_wsPagePath.c_str()))
			return;

		clock_t oEndTime = clock();
		double dElapsedSecs = double(oEndTime - oBeginTime) / CLOCKS_PER_SEC;
		printf("%S %fseconds\n", m_wsPagePath.c_str(), dElapsedSecs);

		if (L"FixedPage" != oNode.GetName())
			return;

		CContextState oState;

		XmlUtils::CXmlNode oNodeResources;
		if (oNode.GetNode(L"FixedPage.Resources", oNodeResources))
		{
			XmlUtils::CXmlNode oNodeDictionary;
			if (oNodeResources.GetNode(L"ResourceDictionary", oNodeDictionary))
			{
				std::wstring wsXmlSource = oNodeDictionary.GetAttribute(L"Source");
				if (L"" != wsXmlSource)
				{
					std::wstring wsPath = m_wsRootPath + wsXmlSource;

					XmlUtils::CXmlNode oNodeSource;

					clock_t oBeginTime = clock();

					oNodeSource.FromXmlFile(wsPath.c_str());

					clock_t oEndTime = clock();
					double dElapsedSecs = double(oEndTime - oBeginTime) / CLOCKS_PER_SEC;
					printf("%S %fseconds\n", wsPath.c_str(), dElapsedSecs);
					
					if (oNodeSource.IsValid())
					{
						XmlUtils::CXmlNodes arrNodes;
						if (oNodeSource.GetNodes(L"PathGeometry", arrNodes))
						{
							for (int nIndex = 0, nCount = arrNodes.GetCount(); nIndex < nCount; nIndex++)
							{
								XmlUtils::CXmlNode oNode;
								arrNodes.GetAt(nIndex, oNode);
								std::wstring wsKey   = oNode.GetAttribute(L"x:Key");
								std::wstring wsValue = oNode.GetAttribute(L"Figures");

								wsKey = L"{StaticResource " + wsKey + L"}";
								oState.AddFigure(wsKey, wsValue);
							}
						}
					}
				}
				else
				{
					XmlUtils::CXmlNodes arrNodes;
					if (oNodeDictionary.GetNodes(L"PathGeometry", arrNodes))
					{
						for (int nIndex = 0, nCount = arrNodes.GetCount(); nIndex < nCount; nIndex++)
						{
							XmlUtils::CXmlNode oNode;
							arrNodes.GetAt(nIndex, oNode);
							
							std::wstring wsKey   = oNode.GetAttribute(L"x:Key");
							std::wstring wsValue = oNode.GetAttribute(L"Figures");

							wsKey = (L"{StaticResource " + wsKey + L"}");
							oState.AddFigure(wsKey, wsValue);
						}
					}
				}
			}
		}

		DrawCanvas(oNode, pRenderer, &oState, pbBreak);
		return;*/
	}
	void Page::DrawCanvas(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState, bool* pbBreak)
	{
		std::wstring wsClip;
		ReadAttribute(oReader, L"Clip", wsClip);
		bool bClip = false;
		if (L"" != wsClip)
		{
			std::wstring wsValue = pState->GetFigure(wsClip);
			if (L"" != wsValue)
				wsClip = wsValue;

			bClip = true;
			pRenderer->PathCommandStart();
			pRenderer->BeginCommand(c_nClipType);
			pRenderer->put_ClipMode(0);
			pRenderer->BeginCommand(c_nPathType);

			VmlToRenderer(wsClip, pRenderer);

			pRenderer->EndCommand(c_nPathType);
			pRenderer->EndCommand(c_nClipType);
			pRenderer->PathCommandEnd();
		}

		std::wstring wsTransform;
		ReadAttribute(oReader, L"RenderTransform", wsTransform);
		bool bTransform = false;
		if (L"" != wsTransform)
		{
			bTransform = true;
			TransformToRenderer(wsTransform, pRenderer, pState);
		}

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
				{
					CanvasTransform(oReader, pRenderer, pState);
					bTransform = true;
				}
			}
			else if (L"Path" == wsNodeName)
			{
				DrawPath(oReader, pRenderer, pState);
			}

			if (NULL != pbBreak && *pbBreak)
				return;
		}		

		if (bClip)
		{
			pRenderer->BeginCommand(c_nResetClipType);
			pRenderer->EndCommand(c_nResetClipType);
		}

		if (bTransform)
			ResetTransform(pRenderer, pState);
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
					
					XmlUtils::CXmlLiteReader oSourceReader;

					clock_t oBeginTime = clock();

					if (!oSourceReader.FromFile(wsPath))
						return;

					std::wstring wsNodeText;
					while (oSourceReader.ReadNextNode())
					{
						wsNodeName = oSourceReader.GetName();
						if (L"PathGeometry" == wsNodeName)
						{
							std::wstring wsKey, wsValue;

							if (oSourceReader.MoveToFirstAttribute())
							{
								std::wstring wsAttrName = oSourceReader.GetName();
								while (!wsAttrName.empty())
								{
									if (L"x:Key" == wsAttrName)
										wsKey = oSourceReader.GetText();
									else if (L"Figures" == wsAttrName)
										wsValue = oSourceReader.GetText();

									if (!oSourceReader.MoveToNextAttribute())
										break;

									wsAttrName = oSourceReader.GetName();
								}

								oSourceReader.MoveToElement();
							}

							wsKey = L"{StaticResource " + wsKey + L"}";
							pState->AddFigure(wsKey, wsValue);
						}
					}

					clock_t oEndTime = clock();
					double dElapsedSecs = double(oEndTime - oBeginTime) / CLOCKS_PER_SEC;
					printf("%S %fseconds\n", wsPath.c_str(), dElapsedSecs);
				}
				else
				{
					if (oReader.IsEmptyNode())
						return;

					int nPathDepth = oReader.GetDepth();
					while (oReader.ReadNextSiblingNode(nPathDepth))
					{
						wsNodeName = oReader.GetName();
						if (L"PathGeometry" == wsNodeName)
						{
							std::wstring wsKey, wsValue;
							ReadAttribute(oReader, L"x:Key", wsKey);
							ReadAttribute(oReader, L"Figures", wsValue);
							wsKey = L"{StaticResource " + wsKey + L"}";
							pState->AddFigure(wsKey, wsValue);
						}
					}
				}
			}
		}
	}
	void Page::DrawCanvas(XmlUtils::CXmlNode& oCanvasNode, IRenderer* pRenderer, CContextState* pState, bool* pbBreak)
	{
		bool bClip = false;
		std::wstring wsClip = oCanvasNode.GetAttribute(L"Clip", L"");
		if (L"" != wsClip)
		{
			std::wstring wsValue = pState->GetFigure(wsClip);
			if (L"" != wsValue)
				wsClip = wsValue;

			bClip = true;
			pRenderer->PathCommandStart();
			pRenderer->BeginCommand(c_nClipType);
			pRenderer->put_ClipMode(0);
			pRenderer->BeginCommand(c_nPathType);

			VmlToRenderer(wsClip, pRenderer);

			pRenderer->EndCommand(c_nPathType);
			pRenderer->EndCommand(c_nClipType);
			pRenderer->PathCommandEnd();
		}

		bool bTransform = false;
		std::wstring wsTransform = oCanvasNode.GetAttribute(L"RenderTransform", L"");
		if (L"" != wsTransform)
		{
			bTransform = true;
			TransformToRenderer(wsTransform, pRenderer, pState);
		}

		XmlUtils::CXmlNodes	arrNodes;
		XmlUtils::CXmlNode	oNode;
		std::wstring        wsNodeName;
		oCanvasNode.GetNodes(L"*", arrNodes);
		for (int nIndex = 0, nCount = arrNodes.GetCount(); nIndex < nCount; nIndex++)
		{
			arrNodes.GetAt(nIndex, oNode);
			wsNodeName = oNode.GetName();
			wsNodeName = RemoveNamespace(wsNodeName);

			if (L"Glyphs" == wsNodeName)
				DrawGlyph(oNode, pRenderer, pState);
			else if (L"Canvas" == wsNodeName)
				DrawCanvas(oNode, pRenderer, pState, pbBreak);
			else if (L"Canvas.RenderTransform" == wsNodeName)
			{
				if (!bTransform)
				{
					CanvasTransform(oNode, pRenderer, pState);
					bTransform = true;
				}
			}
			else if (L"Path" == wsNodeName)
				DrawPath(oNode, pRenderer, pState);

			if (NULL != pbBreak)
			{
				if (*pbBreak)
					return;
			}
		}

		if (bClip)
		{
			pRenderer->BeginCommand(c_nResetClipType);
			pRenderer->EndCommand(c_nResetClipType);
		}

		if (bTransform)
			ResetTransform(pRenderer, pState);
	}
	bool Page::VmlToRenderer(std::wstring& wsString, IRenderer* pRenderer)
	{
		bool bResult = false;
		double dCurX = 0.0, dCurY = 0.0;
		double dCpX = 0.0, dCpY = 0.0;

		bool bPrevCommandIsCurve = false;

		PrepareVmlString(wsString);
		std::vector<std::wstring> arrElements = NSString::Split(wsString, L" ,", false);

		int nElementsCount = (int)arrElements.size();
		for (int nPos = 0; nPos < nElementsCount; nPos++)
		{
			bPrevCommandIsCurve = false;
			if (L"F" == arrElements[nPos])
			{
				bResult = GetBool(arrElements[nPos + 1]);
			}
			else if (L"M" == arrElements[nPos] || L"m" == arrElements[nPos])
			{
				dCurX = GetDouble(arrElements[nPos + 1]);
				dCurY = GetDouble(arrElements[nPos + 2]);
				nPos += 2;
				pRenderer->PathCommandMoveTo(xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
			}
			else if (L"L" == arrElements[nPos] || L"l" == arrElements[nPos])
			{
				PageCommandL(arrElements, dCurX, dCurY, pRenderer, nPos);

				if (nPos + 2 < nElementsCount)
				{
					while ((nPos + 2 < nElementsCount) && (!IsAlpha(arrElements[nPos + 1][0])))
					{
						PageCommandL(arrElements, dCurX, dCurY, pRenderer, nPos);
					}
				}
			}
			else if (L"A" == arrElements[nPos] || L"a" == arrElements[nPos])
			{
				PageCommandA(arrElements, dCurX, dCurY, pRenderer, nPos);

				if (nPos + 7 < nElementsCount)
				{
					while ((nPos + 7 < nElementsCount) && (!IsAlpha(arrElements[nPos + 1][0])))
					{
						PageCommandA(arrElements, dCurX, dCurY, pRenderer, nPos);
					}
				}
			}
			else if (L"H" == arrElements[nPos] || L"h" == arrElements[nPos])
			{
				dCurX = GetDouble(arrElements[nPos + 1]);
				pRenderer->PathCommandLineTo(xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
				nPos += 1;
			}
			else if (L"V" == arrElements[nPos] || L"v" == arrElements[nPos])
			{
				dCurY = GetDouble(arrElements[nPos + 1]);
				pRenderer->PathCommandLineTo(xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
				nPos += 1;
			}
			else if (L"C" == arrElements[nPos] || L"c" == arrElements[nPos])
			{
				PageCommandC(arrElements, dCurX, dCurY, dCpX, dCpY, pRenderer, nPos);

				if (nPos + 6 < nElementsCount)
				{
					while ((nPos + 6 < nElementsCount) && (!IsAlpha(arrElements[nPos + 1][0])))
					{
						PageCommandC(arrElements, dCurX, dCurY, dCpX, dCpY, pRenderer, nPos);
					}
				}
				bPrevCommandIsCurve = true;
			}
			else if (L"Q" == arrElements[nPos] || L"q" == arrElements[nPos])
			{
				PageCommandQ(arrElements, dCurX, dCurY, pRenderer, nPos);

				if (nPos + 4 < nElementsCount)
				{
					while ((nPos + 4 < nElementsCount) && (!IsAlpha(arrElements[nPos + 1][0])))
					{
						PageCommandQ(arrElements, dCurX, dCurY, pRenderer, nPos);
					}
				}
			}
			else if (L"S" == arrElements[nPos] || L"s" == arrElements[nPos])
			{
				if ((!bPrevCommandIsCurve) || (nPos == 0))
				{
					dCpX = dCurX;
					dCpY = dCurY;
				}

				PageCommandS(arrElements, dCurX, dCurY, dCpX, dCpY, pRenderer, nPos);

				if (nPos + 4 < nElementsCount)
				{
					while ((nPos + 4 < nElementsCount) && (!IsAlpha(arrElements[nPos + 1][0])))
					{
						PageCommandS(arrElements, dCurX, dCurY, dCpX, dCpY, pRenderer, nPos);
					}
				}
				bPrevCommandIsCurve = true;
			}
			else if (L"Z" == arrElements[nPos] || L"Z" == arrElements[nPos])
			{
				pRenderer->PathCommandClose();
			}			
		}

		return bResult;
	}
	void Page::PrepareVmlString(std::wstring& wsString)
	{
		const wchar_t* wsVml = L"FMLHVCQSAZfmlhvcqsaz";

		std::wstring wsResult;

		int nPos = wsString.find(wsVml);
		while (std::wstring::npos != nPos)
		{
			wsResult += wsString.substr(0, nPos);
			wsResult += L" ";
			wsResult += wsString.substr(nPos, 1);
			wsResult += L" ";
			wsString.erase(0, nPos + 1);
			nPos = wsString.find(wsVml);
		}
		wsResult += wsString;
		wsString = wsResult;
	}
	void Page::TransformToRenderer(const std::wstring& wsString, IRenderer* pRenderer, CContextState* pState)
	{
		std::vector<std::wstring> arrElements = NSString::Split(wsString, L',');

		double arrRes[6] ={ 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
		for (int nIndex = 0, nCount = min(6, arrElements.size()); nIndex < nCount; nIndex++)
			arrRes[nIndex] = GetDouble(arrElements[nIndex]);

		pState->PushTransform(arrRes);
		pState->SetTransformToRenderer(pRenderer);
	}
	void Page::ResetTransform(IRenderer* pRenderer, CContextState* pState)
	{
		pState->PopTransform();
		pState->SetTransformToRenderer(pRenderer);
	}
	void Page::DrawGlyph(XmlUtils::CXmlNode& oRootNode, IRenderer* pRenderer, CContextState* pState)
	{
		std::wstring wsFontPath = oRootNode.GetAttribute(L"FontUri", L"");
		if (L"" != wsFontPath)
		{
			std::wstring wsFontName = GetFileName(wsFontPath);
			wsFontPath = m_wsRootPath + L"/" + wsFontPath;
			std::wstring wsExt = GetFileExtension(wsFontPath);
			NSString::ToLower(wsExt);
			if(L"odttf" == wsExt)
			{
				NSString::ToLower(wsFontName);
				m_pFontList->Check(wsFontName, wsFontPath);
			}
			wsFontPath = NormalizePath(wsFontPath);
			pRenderer->put_FontPath(wsFontPath);
		}

		int nBgr, nAlpha;
		std::wstring wsFontColor = oRootNode.GetAttribute(L"Fill", L"#FF000000");
		GetBgra(wsFontColor, nBgr, nAlpha);
		pRenderer->put_BrushColor1(nBgr & 0x00FFFFFF);
		pRenderer->put_BrushAlpha1(nAlpha);
		pRenderer->put_BrushType(c_BrushTypeSolid);

		std::wstring wsFontStyle = oRootNode.GetAttribute(L"StyleSimulations", L"");
		if (L"ItalicSimulation" == wsFontStyle)
			pRenderer->put_FontStyle(0x02);
		else if (L"BoldSimulation" == wsFontStyle)
			pRenderer->put_FontStyle(0x01);
		else if (L"BoldItalicSimulation" == wsFontStyle)
			pRenderer->put_FontStyle(0x03);

		std::wstring wsFontSize = oRootNode.GetAttribute(L"FontRenderingEmSize");
		double dFontSize = GetDouble(wsFontSize);

		bool bTransform = false;
		std::wstring wsTransform = oRootNode.GetAttribute(L"RenderTransform", L"");
		if(L"" != wsTransform)
		{
			TransformToRenderer(wsTransform, pRenderer, pState);
			bTransform = true;

			if(dFontSize < 5)
			{
				double dDet = pState->NormalizeTransform();
				dFontSize *= dDet;
				pState->SetTransformToRenderer(pRenderer);
			}
		}
		pRenderer->put_FontSize(dFontSize * 0.75);

		std::wstring wsText = oRootNode.GetAttribute(L"UnicodeString", L"");
		std::wstring wsTextX = oRootNode.GetAttribute(L"OriginX");
		std::wstring wsTextY = oRootNode.GetAttribute(L"OriginY");

		int nTextLen = wsText.length();
		double dX = GetDouble(wsTextX);
		double dY = GetDouble(wsTextY);

		std::wstring wsChar = wsText.substr(0, 1);

		std::wstring wsIndicies = oRootNode.GetAttribute(L"Indices", L"");
		std::vector<std::vector<std::wstring>> arrElements = Split(wsIndicies, L';', L',');

		m_pFontManager->LoadFontFromFile(wsFontPath, 0, (float)(dFontSize * 0.75), 96, 96);
		for(int nIndex = 0; nIndex < nTextLen - 1; nIndex++)
		{
			if (nIndex >= arrElements.size())
				arrElements.push_back(std::vector<std::wstring>());

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

			wsChar = wsText.substr(nIndex + 1, 1);
		}

		if (nTextLen > 0)
		{
			pRenderer->CommandDrawText(wsChar, xpsUnitToMM(dX), xpsUnitToMM(dY), 0, 0, 0);
		}

		if (bTransform)
			ResetTransform(pRenderer, pState);
	}
	void Page::DrawGlyph(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState)
	{
		int nBgr = 0, nAlpha = 255;
		double dFontSize = 10.0;
		bool bTransform = false;
		int nTextLen = 0;
		std::wstring wsText;
		double dX = 0;
		double dY = 0;
		std::wstring wsFontPath;
		std::wstring wsIndicies;
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
					NSString::ToLower(wsExt);
					if (L"odttf" == wsExt)
					{
						NSString::ToLower(wsFontName);
						m_pFontList->Check(wsFontName, wsFontPath);
					}
					wsFontPath = NormalizePath(wsFontPath);
					pRenderer->put_FontPath(wsFontPath);
				}
				else if (L"Fill" == wsAttrName)
				{
					int nBgr, nAlpha;
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
					std::wstring wsTransform = oReader.GetText();
					if (!wsTransform.empty())
					{
						TransformToRenderer(wsTransform, pRenderer, pState);
						bTransform = true;

						if (dFontSize < 5)
						{
							double dDet = pState->NormalizeTransform();
							dFontSize *= dDet;
							pState->SetTransformToRenderer(pRenderer);
						}
					}
				}
				else if (L"UnicodeString" == wsAttrName)
				{
					wsText = oReader.GetText();
					nTextLen = wsText.length();
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

				if (!oReader.MoveToNextAttribute())
					break;

				wsAttrName = oReader.GetName();
			}
		}
		oReader.MoveToElement();

		pRenderer->put_BrushColor1(nBgr & 0x00FFFFFF);
		pRenderer->put_BrushAlpha1(nAlpha);
		pRenderer->put_BrushType(c_BrushTypeSolid);
		pRenderer->put_FontSize(dFontSize * 0.75);

		std::wstring wsChar = wsText.substr(0, 1);
		std::vector<std::vector<std::wstring>> arrElements = Split(wsIndicies, L';', L',');

		m_pFontManager->LoadFontFromFile(wsFontPath, 0, (float)(dFontSize * 0.75), 96, 96);
		for (int nIndex = 0; nIndex < nTextLen - 1; nIndex++)
		{
			if (nIndex >= arrElements.size())
				arrElements.push_back(std::vector<std::wstring>());

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

			wsChar = wsText.substr(nIndex + 1, 1);
		}

		if (nTextLen > 0)
		{
			pRenderer->CommandDrawText(wsChar, xpsUnitToMM(dX), xpsUnitToMM(dY), 0, 0, 0);
		}

		if (bTransform)
			ResetTransform(pRenderer, pState);
	}
	void Page::CanvasTransform(XmlUtils::CXmlNode& oRootNode, IRenderer* pRenderer, CContextState* pState)
	{
		XmlUtils::CXmlNode oTransformNode;
		oRootNode.GetNode(L"MatrixTransform", oTransformNode);
		std::wstring wsMatrix = oTransformNode.GetAttribute(L"Matrix");
		TransformToRenderer(wsMatrix, pRenderer, pState);
	}
	void Page::CanvasTransform(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState)
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
				TransformToRenderer(wsMatrix, pRenderer, pState);
				break;
			}
		}
	}
	void Page::DrawPath(XmlUtils::CXmlNode& oRootNode, IRenderer* pRenderer, CContextState* pState)
	{
		bool bTransform = false;
		std::wstring wsTransform = oRootNode.GetAttribute(L"RenderTransform", L"");
		if (L"" != wsTransform)
		{
			TransformToRenderer(wsTransform, pRenderer, pState);
			bTransform = true;
		}

		int nAlpha, nBgr;
		std::wstring wsStrokeColor = oRootNode.GetAttribute(L"Stroke", L"#00FFFFFF");
		std::wstring wsPenSize = oRootNode.GetAttribute(L"StrokeThickness", L"1.0");
		GetBgra(wsStrokeColor, nBgr, nAlpha);
		pRenderer->put_PenColor(nBgr & 0x00FFFFFF);
		pRenderer->put_PenAlpha(nAlpha);
		pRenderer->put_PenSize(xpsUnitToMM(GetDouble(wsPenSize)));

		std::wstring wsFill = oRootNode.GetAttribute(L"Fill");
		if (L"" != wsFill)
		{
			GetBgra(wsFill, nBgr, nAlpha);
			pRenderer->put_BrushType(c_BrushTypeSolid);
			pRenderer->put_BrushColor1(nBgr & 0x00FFFFFF);
			pRenderer->put_BrushAlpha1(nAlpha);
		}
		else
		{
			XmlUtils::CXmlNode oFillNode;
			if (oRootNode.GetNode(L"Path.Fill", oFillNode))
				FillToRenderer(oFillNode, pRenderer);
			else
			{
				pRenderer->put_BrushAlpha1(0);
				pRenderer->put_BrushAlpha2(0);
			}
		}

		std::wstring wsData = oRootNode.GetAttribute(L"Data");
		if (L"" == wsData)
		{
			GetDataFromNode(wsData, oRootNode);
		}

		if (L'{' == wsData[0])
		{
			std::wstring wsValue = pState->GetFigure(wsData);
			if (L"" != wsValue)
				wsData = wsValue;
			else
			{
				if (bTransform)
					ResetTransform(pRenderer, pState);

				return;
			}
		}

		pRenderer->BeginCommand(c_nPathType);
		pRenderer->PathCommandStart();

		bool bWindingFillMode = false;
		if (L'{' != wsData[0])
			bWindingFillMode = VmlToRenderer(wsData, pRenderer);

		int nFillMode = bWindingFillMode ? c_nWindingFillMode | c_nStroke : c_nEvenOddFillMode | c_nStroke;
		pRenderer->DrawPath(nFillMode);

		pRenderer->EndCommand(c_nPathType);
		pRenderer->PathCommandEnd();

		if (bTransform)
			ResetTransform(pRenderer, pState);
	}
	void Page::DrawPath(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState)
	{
		bool bTransform = false;
		bool bFillSetted = false;
		double dPenSize = 1.0;
		std::wstring wsData;
		bool bStroke = false;
		if (oReader.MoveToFirstAttribute())
		{
			std::wstring wsAttrName = oReader.GetName();
			while (!wsAttrName.empty())
			{				
				if (L"RenderTransform" == wsAttrName)
				{
					std::wstring wsTransform = oReader.GetText();
					if (!wsTransform.empty())
					{
						TransformToRenderer(wsTransform, pRenderer, pState);
						bTransform = true;
					}
				}
				else if (L"Stroke" == wsAttrName)
				{
					int nStrokeAlpha = 255, nStrokeBgr = 0;
					std::wstring wsStrokeColor = oReader.GetText();
					GetBgra(wsStrokeColor, nStrokeBgr, nStrokeAlpha);
					pRenderer->put_PenColor(nStrokeBgr & 0x00FFFFFF);
					pRenderer->put_PenAlpha(nStrokeAlpha);
					bStroke = true;
				}
				else if (L"StrokeThickness" == wsAttrName)
				{
					std::wstring wsPenSize = oReader.GetText();
					dPenSize = GetDouble(wsPenSize);
				}
				else if (L"Fill" == wsAttrName)
				{
					bFillSetted = true;
					std::wstring wsFill = oReader.GetText();
					int nFillBgra, nFillAlpha;
					GetBgra(wsFill, nFillBgra, nFillAlpha);
					pRenderer->put_BrushType(c_BrushTypeSolid);
					pRenderer->put_BrushColor1(nFillBgra & 0x00FFFFFF);
					pRenderer->put_BrushAlpha1(nFillAlpha);
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

		pRenderer->put_PenSize(xpsUnitToMM(dPenSize));

		bool bFill = bFillSetted;
		if (!bFillSetted)
		{
			if (!oReader.IsEmptyNode())
			{
				std::wstring wsNodeName;
				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					wsNodeName = oReader.GetName();
					wsNodeName = RemoveNamespace(wsNodeName);

					if (L"Path.Fill" == wsNodeName)
					{
						bFill = FillToRenderer(oReader, pRenderer);
					}
				}
			}
		}

		if (L"" == wsData)
			ReadPathData(oReader, wsData);

		if (L'{' == wsData[0])
		{
			std::wstring wsValue = pState->GetFigure(wsData);
			if (L"" != wsValue)
				wsData = wsValue;
			else
			{
				if (bTransform)
					ResetTransform(pRenderer, pState);

				return;
			}
		}

		pRenderer->BeginCommand(c_nPathType);
		pRenderer->PathCommandStart();

		bool bWindingFillMode = false;
		if (L'{' != wsData[0])
			bWindingFillMode = VmlToRenderer(wsData, pRenderer);

		int nMode = bStroke ? c_nStroke : 0;
		if (bFill)
			nMode |= (bWindingFillMode ? c_nWindingFillMode : c_nEvenOddFillMode);

		pRenderer->DrawPath(nMode);
		pRenderer->EndCommand(c_nPathType);
		pRenderer->PathCommandEnd();

		if (bTransform)
			ResetTransform(pRenderer, pState);
	}
	void Page::FillToRenderer(XmlUtils::CXmlNode& oRootNode, IRenderer* pRenderer)
	{
		XmlUtils::CXmlNode oBrushNode;
		if (oRootNode.GetNode(L"SolidColorBrush", oBrushNode))
		{
			int nBgr, nAlpha;
			std::wstring wsColor = oBrushNode.GetAttribute(L"Color", L"#00FFFFFF");
			GetBgra(wsColor, nBgr, nAlpha);
			pRenderer->put_BrushType(c_BrushTypeSolid);
			pRenderer->put_BrushColor1(nBgr & 0x00FFFFFF);
			pRenderer->put_BrushAlpha1(nAlpha);
		}
		else if (oRootNode.GetNode(L"ImageBrush", oBrushNode))
		{
			std::wstring wsImageSource = oBrushNode.GetAttribute(L"ImageSource");
			pRenderer->put_BrushType(c_BrushTypeTexture);
			pRenderer->put_BrushTexturePath(m_wsRootPath + wsImageSource);
		}
		else
		{
			pRenderer->put_BrushAlpha1(0);
			pRenderer->put_BrushAlpha2(0);
		}
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
	void Page::GetDataFromNode(std::wstring& wsString, XmlUtils::CXmlNode& oNode)
	{
		wsString = L"";

		XmlUtils::CXmlNode oPathDataNode;
		oNode.GetNode(L"Path.Data", oPathDataNode);

		XmlUtils::CXmlNode oGeometryNode;
		oPathDataNode.GetNode(_T("PathGeometry"), oGeometryNode);

		std::wstring wsFillMode = oGeometryNode.GetAttribute(L"FillRule", L"EvenOdd");
		if (L"EvenOdd" == wsFillMode)
			wsString += L"F 0 ";
		else
			wsString += L"F 1 ";

		XmlUtils::CXmlNodes arrFigureNodes;
		XmlUtils::CXmlNode	oFigureNode;
		XmlUtils::CXmlNodes arrSegmentNodes;
		XmlUtils::CXmlNode	oSegmentNode;
		oGeometryNode.GetNodes(L"PathFigure", arrFigureNodes);
		for (int nFigureIndex = 0, nFiguresCount = arrFigureNodes.GetCount(); nFigureIndex < nFiguresCount; nFigureIndex++)
		{			
			arrFigureNodes.GetAt(nFigureIndex, oFigureNode);

			std::wstring wsStartPoint = oFigureNode.GetAttribute(L"StartPoint");
			wsString += L" M " + wsStartPoint;

			oFigureNode.GetNodes(L"*", arrSegmentNodes);
			for (int nSegmentIndex = 0, nSegmentsCount = arrSegmentNodes.GetCount(); nSegmentIndex < nSegmentsCount; nSegmentIndex++)
			{
				arrSegmentNodes.GetAt(nSegmentIndex, oSegmentNode);
				std::wstring wsName = oSegmentNode.GetName();
				if (L"PolyLineSegment" == wsName)
				{
					wsString += L" L ";
					wsString += oSegmentNode.GetAttribute(L"Points");
				}
				else if (L"PolyBezierSegment" == wsName)
				{
					wsString += L" C ";
					wsString += oSegmentNode.GetAttribute(L"Points");
				}
				else if (L"PolyQuadraticBezierSegment" == wsName)
				{
					wsString += L" Q ";
					wsString += oSegmentNode.GetAttribute(L"Points");
				}
				else if (L"ArcSegment" == wsName)
				{
					wsString += L" A ";
					wsString += oSegmentNode.GetAttribute(L"Size");
					wsString += L" ";
					wsString += oSegmentNode.GetAttribute(L"RotationAngle");
					wsString += L" ";

					std::wstring wsIsLargeArc = oSegmentNode.GetAttribute(L"IsLargeArc");
					if (GetBool(wsIsLargeArc))
						wsString += L"1 ";
					else
						wsString += L"0 ";

					std::wstring wsSweepDirection = oSegmentNode.GetAttribute(L"SweepDirection");
					if (L"Counterclockwise" == wsSweepDirection)
						wsString += L"0 ";
					else
						wsString += L"1 ";
					wsString += oSegmentNode.GetAttribute(L"Point");
				}
			}

			std::wstring wsClosed = oFigureNode.GetAttribute(L"IsClosed");
			if (GetBool(wsClosed))
				wsString += L" Z ";
		}
	}	
	void Page::ReadPathData(XmlUtils::CXmlLiteReader& oReader, std::wstring& wsData)
	{
		wsData = L"";

		if (oReader.IsEmptyNode())
			return;

		std::wstring wsNodeName;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			wsNodeName = oReader.GetName();
			if (L"Path.Data" == wsNodeName)
			{
				if (!oReader.IsEmptyNode())
				{
					int nDataDepth = oReader.GetDepth();
					while (oReader.ReadNextSiblingNode(nParentDepth))
					{
						wsNodeName = oReader.GetName();
						if (L"PathGeometry" == wsNodeName)
						{
							return ReadPathGeometry(oReader, wsData);
						}
					}
				}
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