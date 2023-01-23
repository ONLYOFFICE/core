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
#include "StaticResources.h"

#include "../../DesktopEditor/xml/include/xmlutils.h"
#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/graphics/structures.h"
#include "../../DesktopEditor/common/File.h"

#ifndef DISABLE_PDF_CONVERTATION
#include "../../PdfFile/PdfFile.h"
#endif

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifndef xpsUnitToMM
#define xpsUnitToMM(x) ((x) * 25.4 / 96)
#endif

namespace XPS
{
	CStaticResource::CStaticResource(const std::string& wsPath)
	{
		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromStringA(wsPath))
			return;

		Parse(oReader);
	}
	CStaticResource::CStaticResource(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		Parse(oReader);
	}
	CStaticResource::~CStaticResource()
	{
	}
	bool CStaticResource::GetFigure(CWString& wsKey, CWString& wsData)
	{
		std::map<CWString, CWString>::iterator oIter = m_mFigures.find(wsKey);
		if (oIter != m_mFigures.end())
		{
			wsData = oIter->second;
			return true;
		}

		return false;
	}
	CBrush* CStaticResource::GetBrush(CWString& wsKey)
	{
		std::map<CWString, CBrush*>::iterator oIter = m_mBrushes.find(wsKey);
		if (oIter != m_mBrushes.end())
			return oIter->second;

		return NULL;
	}
	bool CStaticResource::GetTransform(CWString& wsKey, CWString& wsTransform)
	{
		std::map<CWString, CWString>::iterator oIter = m_mTransforms.find(wsKey);
		if (oIter != m_mTransforms.end())
		{			
			wsTransform = oIter->second;
			return true;
		}

		return false;
	}
	void CStaticResource::Parse(XmlUtils::CXmlLiteReader& oReader)
	{
		CWString wsNodeName;
		CWString wsAttrName;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetNameNoNS();
			if (wsNodeName == L"PathGeometry")
			{				
				CWString wsKey, wsValue, wsTrasform;
				ReadPathGeometry(oReader, wsValue, wsTrasform, &wsKey);				

				if (!wsKey.empty() && !wsValue.empty())
					AddFigure(wsKey, wsValue);
			}
			else if (wsNodeName == L"SolidColorBrush"
					 || wsNodeName == L"ImageBrush"
					 || wsNodeName == L"LinearGradientBrush"
					 || wsNodeName == L"RadialGradientBrush")
			{
				CWString wsKey;
				CBrush* pBrush = ReadBrushNode(oReader, 1.0, &wsKey);
				if (pBrush)
					AddBrush(wsKey, pBrush);
			}
			else if (wsNodeName == L"MatrixTransform")
			{
				CWString wsKey, wsValue;
				ReadMatrixTransform(oReader, wsValue, &wsKey);
				if (!wsKey.empty() && !wsValue.empty())
					AddTransform(wsKey, wsValue);
			}
		}
	}
	void CStaticResource::AddFigure(const CWString& wsKey, const CWString& wsValue)
	{
		m_mFigures.insert(std::pair<CWString, CWString>(wsKey, wsValue));
	}
	void CStaticResource::AddBrush(const CWString& wsKey, CBrush* pBrush)
	{
		m_mBrushes.insert(std::pair<CWString, CBrush*>(wsKey, pBrush));
	}
	void CStaticResource::AddTransform(const CWString& wsKey, const CWString& wsValue)
	{
		m_mTransforms.insert(std::pair<CWString, CWString>(wsKey, wsValue));
	}
	bool CSolidBrush::SetToRenderer(IRenderer* pRenderer)
	{
		pRenderer->put_BrushType(c_BrushTypeSolid);
		pRenderer->put_BrushColor1(m_nBgr);
		pRenderer->put_BrushAlpha1(m_nAlpha);
		return true;
	}
	bool CImageBrush::SetToRenderer(IRenderer* pRenderer)
	{
		if (!m_wsRoot)
			return false;

		std::wstring wsPath = m_wsPath.c_stdstr();
		if (!m_wsRoot->exists(wsPath))
		{
			wsPath = m_wsPage.c_stdstr() + m_wsPath.c_stdstr();
			if (!m_wsRoot->exists(wsPath))
				return false;
		}

#ifndef BUILDING_WASM_MODULE
        pRenderer->put_BrushType(c_BrushTypeTexture);
        pRenderer->put_BrushTexturePath(m_wsRoot->getFullFilePath(wsPath));
        return true;
#endif

		IFolder::CBuffer* buffer = NULL;
		m_wsRoot->read(wsPath, buffer);
		int nBase64BufferLen = NSBase64::Base64EncodeGetRequiredLength(buffer->Size);
		BYTE* pbBase64Buffer = new BYTE[nBase64BufferLen + 64];
		if (true == NSBase64::Base64Encode(buffer->Buffer, buffer->Size, pbBase64Buffer, &nBase64BufferLen))
		{
			pRenderer->put_BrushType(c_BrushTypeTexture);
			pRenderer->put_BrushTexturePath(L"data:," + NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(pbBase64Buffer, nBase64BufferLen));
			RELEASEARRAYOBJECTS(pbBase64Buffer);
			RELEASEOBJECT(buffer);
			return true;
		}
		RELEASEARRAYOBJECTS(pbBase64Buffer);
		RELEASEOBJECT(buffer);
		return false;
	}
	void CImageBrush::SetPaths(IFolder* wsRoot, const wchar_t* wsPage)
	{
		m_wsRoot = wsRoot;
		m_wsPage.create(wsPage, true);
	}
	bool CLinearGradientBrush::SetToRenderer(IRenderer* pRenderer)
	{
		if (!m_pColors || !m_pPositions || !m_lCount)
			return false;

#ifndef DISABLE_PDF_CONVERTATION
		LONG lRendererType = c_nUnknownRenderer;
		pRenderer->get_Type(&lRendererType);

		if (c_nPDFWriter == lRendererType)
		{
			CPdfFile* pPdf = (CPdfFile*)pRenderer;
			pPdf->put_BrushGradientColors(m_pColors, m_pPositions, m_lCount);
			pPdf->SetLinearGradient(m_dX0, m_dY0, m_dX1, m_dY1);
		}
		else
#endif
		{
			double dX = m_dX1 - m_dX0, dY = m_dY1 - m_dY0;
			double dHyp = sqrt(dX * dX + dY * dY);
			double dAngle = acos(dX / dHyp) * 180 / M_PI;
			pRenderer->put_BrushType(c_BrushTypePathGradient1);
			pRenderer->put_BrushGradientColors(m_pColors, m_pPositions, m_lCount);
			pRenderer->put_BrushLinearAngle(dAngle);
		}
		return true;
	}
	bool CRadialGradientBrush::SetToRenderer(IRenderer* pRenderer)
	{
		if (!m_pColors || !m_pPositions || !m_lCount)
			return false;

#ifndef DISABLE_PDF_CONVERTATION
		LONG lRendererType = c_nUnknownRenderer;
		pRenderer->get_Type(&lRendererType);

		if (c_nPDFWriter == lRendererType)
		{
			CPdfFile* pPdf = (CPdfFile*)pRenderer;
			pPdf->put_BrushGradientColors(m_pColors, m_pPositions, m_lCount);
            pPdf->SetRadialGradient(m_dXo, m_dYo, 0, m_dXc, m_dYc, std::max(m_dRadX, m_dRadY));
		}
		else
#endif
		{
			pRenderer->put_BrushType(c_BrushTypePathGradient2);
			pRenderer->put_BrushGradientColors(m_pColors, m_pPositions, m_lCount);
		}

		return true;
	}
	CBrush* ReadBrushNode(XmlUtils::CXmlLiteReader& oReader, const double& dCurOpacity, CWString* pwsKey)
	{
		CBrush* pBrush = NULL;
		CWString wsNodeName = oReader.GetNameNoNS();
		if (wsNodeName == L"SolidColorBrush")
		{
			int nBgr = 0, nAlpha = 255;
			double dOpacity = 1.0;
			if (oReader.GetAttributesCount() > 0 && oReader.MoveToFirstAttribute())
			{
				CWString wsAttrName = oReader.GetName();
				while (!wsAttrName.empty())
				{
					if (wsAttrName == L"Color")
					{
						ReadSTColor(oReader.GetText(), nBgr, nAlpha);
					}
					else if (wsAttrName == L"Opacity")
					{
						ReadSTDouble(oReader.GetText(), dOpacity);
					}
					else if (wsAttrName == L"x:Key" && pwsKey)
					{
						pwsKey->create(oReader.GetText(), true);
					}

					if (!oReader.MoveToNextAttribute())
						break;

					wsAttrName = oReader.GetName();
				}
				oReader.MoveToElement();
			}
			pBrush = new CSolidBrush(nBgr, nAlpha * dCurOpacity * dOpacity);
		}
		else if (wsNodeName == L"ImageBrush")
		{
			if (oReader.GetAttributesCount() > 0 && oReader.MoveToFirstAttribute())
			{
				CWString wsAttrName = oReader.GetName();
				while (!wsAttrName.empty())
				{
					if (wsAttrName == L"ImageSource")
					{
                        pBrush = new CImageBrush(oReader.GetText().c_str());
					}
					else if (wsAttrName == L"x:Key" && pwsKey)
					{
						pwsKey->create(oReader.GetText(), true);
					}

					if (!oReader.MoveToNextAttribute())
						break;

					wsAttrName = oReader.GetName();
				}
				oReader.MoveToElement();
			}
		}
		else if (wsNodeName == L"LinearGradientBrush" || wsNodeName == L"RadialGradientBrush")
		{
			bool bLinearGradient = (wsNodeName == L"LinearGradientBrush" ? true : false);

			double dOpacity = 1.0;
			if (bLinearGradient)
			{
				double dX0 = 0, dY0 = 0, dX1 = 1, dY1 = 1;
				if (oReader.GetAttributesCount() > 0 && oReader.MoveToFirstAttribute())
				{
					CWString wsAttrName = oReader.GetName();
					while (!wsAttrName.empty())
					{
						if (wsAttrName == L"StartPoint")
						{
							ReadSTPoint(oReader.GetText(), dX0, dY0);
						}
						else if (wsAttrName == L"EndPoint")
						{
							ReadSTPoint(oReader.GetText(), dX1, dY1);
						}
						else if (wsAttrName == L"Opacity")
						{
							std::wstring wsOpacity = oReader.GetText();

							if (!wsOpacity.empty())
								dOpacity = GetDouble(wsOpacity);
						}
						else if (wsAttrName == L"x:Key" && pwsKey)
						{
							pwsKey->create(oReader.GetText(), true);
						}

						if (!oReader.MoveToNextAttribute())
							break;

						wsAttrName = oReader.GetName();
					}
					oReader.MoveToElement();
				}
				pBrush = new CLinearGradientBrush(xpsUnitToMM(dX0), xpsUnitToMM(dY0), xpsUnitToMM(dX1), xpsUnitToMM(dY1));
			}
			else
			{
				double dX1 = 0, dY1 = 0, dRx = 0, dRy = 0, dX0 = 0, dY0 = 0;
				if (oReader.GetAttributesCount() > 0 && oReader.MoveToFirstAttribute())
				{
					CWString wsAttrName = oReader.GetName();
					while (!wsAttrName.empty())
					{
						if (wsAttrName == L"Center")
						{
							ReadSTPoint(oReader.GetText(), dX1, dY1);
						}
						else if (wsAttrName == L"GradientOrigin")
						{
							ReadSTPoint(oReader.GetText(), dX0, dY0);
						}
						else if (wsAttrName == L"Opacity")
						{
							std::wstring wsOpacity = oReader.GetText();

							if (!wsOpacity.empty())
								dOpacity = GetDouble(wsOpacity);
						}
						else if (wsAttrName == L"RadiusX")
						{
							ReadSTDouble(oReader.GetText(), dRx);
						}
						else if (wsAttrName == L"RadiusY")
						{
							ReadSTDouble(oReader.GetText(), dRy);
						}
						else if (wsAttrName == L"x:Key" && pwsKey)
						{
							pwsKey->create(oReader.GetText(), true);
						}

						if (!oReader.MoveToNextAttribute())
							break;

						wsAttrName = oReader.GetName();
					}
					oReader.MoveToElement();
				}
				pBrush = new CRadialGradientBrush(xpsUnitToMM(dX1), xpsUnitToMM(dY1), xpsUnitToMM(dX0), xpsUnitToMM(dY0), xpsUnitToMM(dRx), xpsUnitToMM(dRy));
			}

			if (!pBrush)
				return NULL;

			std::vector<LONG> vColors;
			std::vector<double> vPositions;
			if (!oReader.IsEmptyNode())
			{
				int nGrDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nGrDepth))
				{
					wsNodeName = oReader.GetNameNoNS();
					if ((wsNodeName == L"LinearGradientBrush.GradientStops" && bLinearGradient)
						|| (wsNodeName == L"RadialGradientBrush.GradientStops" && !bLinearGradient))
					{
						ReadGradientStops(oReader, vColors, vPositions, dOpacity * dCurOpacity);
					}
				}
			}

			LONG lCount = std::min(vColors.size(), vPositions.size());
			if (lCount <= 0)
			{
				delete pBrush;
				return NULL;
			}

			LONG* pColors = new LONG[lCount];
			if (!pColors)
			{
				delete pBrush;
				return NULL;
			}

			double* pPositions = new double[lCount];
			if (!pPositions)
			{
				RELEASEARRAYOBJECTS(pColors);
				delete pBrush;
				return NULL;
			}

			for (LONG lIndex = 0; lIndex < lCount; lIndex++)
			{
				pColors[lIndex] = vColors[lIndex];
				pPositions[lIndex] = vPositions[lIndex];
			}

			((CGradientBrush*)pBrush)->SetGradientStops(pColors, pPositions, lCount);
		}

		return pBrush;
	}
	CBrush* ReadBrush(XmlUtils::CXmlLiteReader& oReader, const double& dCurOpacity, CWString* pwsKey)
	{
		CBrush* pBrush = NULL;

		// TODO: 1. Трансформы здесь не реализованы, потому что их невозможно учесть с текущим интерфейсом рендерера
		//       2. Не реализован VisualBrush
		if (!oReader.IsEmptyNode())
		{
			CWString wsNodeName;
			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				pBrush = ReadBrushNode(oReader, dCurOpacity, pwsKey);
				if (pBrush)
					break;
			}
		}

		return pBrush;
	}	
	CBrush* ReadBrush(const wchar_t* wsBrush, const double& dCurOpacity)
	{
		int nBgr, nAlpha;
		ReadSTColor(wsBrush, nBgr, nAlpha);
		return new CSolidBrush(nBgr, nAlpha * dCurOpacity);
	}
}
