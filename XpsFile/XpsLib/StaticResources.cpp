#include "StaticResources.h"

#include "../../Common/DocxFormat/Source/XML/xmlutils.h"
#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/graphics/structures.h"
#include "../../PdfWriter/PdfRenderer.h"
#include "../../DesktopEditor/common/File.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifndef xpsUnitToMM
#define xpsUnitToMM(x) ((x) * 25.4 / 96)
#endif

namespace XPS
{
	CStaticResource::CStaticResource(const wchar_t* wsPath)
	{
		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(wsPath))
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
	const wchar_t* CStaticResource::GetFigure(CWString& wsKey)
	{
		std::map<CWString, CWString>::iterator oIter = m_mFigures.find(wsKey);
		if (oIter != m_mFigures.end())
			return oIter->second.c_str();

		return NULL;
	}
	CBrush* CStaticResource::GetBrush(CWString& wsKey)
	{
		std::map<CWString, CBrush*>::iterator oIter = m_mBrushes.find(wsKey);
		if (oIter != m_mBrushes.end())
			return oIter->second;

		return NULL;
	}
	const wchar_t* CStaticResource::GetTransform(CWString& wsKey)
	{
		std::map<CWString, CWString>::iterator oIter = m_mTransforms.find(wsKey);
		if (oIter != m_mTransforms.end())
			return oIter->second.c_str();

		return NULL;
	}
	void CStaticResource::Parse(XmlUtils::CXmlLiteReader& oReader)
	{
		CWString wsNodeName;
		CWString wsAttrName;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetName();
			if (wsNodeName == L"PathGeometry")
			{
				CWString wsKey, wsValue;

				if (oReader.MoveToFirstAttribute())
				{
					wsAttrName = oReader.GetName();
					while (!wsAttrName.empty())
					{
						if (wsAttrName == L"x:Key")
							wsKey.create(oReader.GetText(), true);
						else if (wsAttrName == L"Figures")
							wsValue.create(oReader.GetText(), true);

						if (!oReader.MoveToNextAttribute())
							break;

						wsAttrName = oReader.GetName();
					}

					oReader.MoveToElement();
				}

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
		std::wstring wsPath = m_wsRoot.c_str();
		wsPath += m_wsPath.c_str();

		if (!NSFile::CFileBinary::Exists(wsPath))
		{
			wsPath = m_wsPage.c_str();
			wsPath += m_wsPath.c_str();
			if (!NSFile::CFileBinary::Exists(wsPath))
				return false;
		}

		pRenderer->put_BrushType(c_BrushTypeTexture);
		pRenderer->put_BrushTexturePath(wsPath);
		return true;
	}
	void CImageBrush::SetPaths(const wchar_t* wsRoot, const wchar_t* wsPage)
	{
		m_wsRoot.create(wsRoot, true);
		m_wsPage.create(wsPage, true);
	}
	bool CLinearGradientBrush::SetToRenderer(IRenderer* pRenderer)
	{
		if (!m_pColors || !m_pPositions || !m_lCount)
			return false;

		LONG lRendererType = c_nUnknownRenderer;
		pRenderer->get_Type(&lRendererType);
		if (c_nPDFWriter == lRendererType)
		{
			CPdfRenderer* pPdf = (CPdfRenderer*)pRenderer;
			pPdf->put_BrushGradientColors(m_pColors, m_pPositions, m_lCount);
			pPdf->SetLinearGradient(m_dX0, m_dY0, m_dX1, m_dY1);
		}
		else
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

		LONG lRendererType = c_nUnknownRenderer;
		pRenderer->get_Type(&lRendererType);
		if (c_nPDFWriter == lRendererType)
		{
			CPdfRenderer* pPdf = (CPdfRenderer*)pRenderer;
			pPdf->put_BrushGradientColors(m_pColors, m_pPositions, m_lCount);
			pPdf->SetRadialGradient(m_dXo, m_dYo, 0, m_dXc, m_dYc, max(m_dRadX, m_dRadY));
		}
		else
		{
			pRenderer->put_BrushType(c_BrushTypePathGradient2);
			pRenderer->put_BrushGradientColors(m_pColors, m_pPositions, m_lCount);
		}

		return true;
	}
	CBrush* ReadBrushNode(XmlUtils::CXmlLiteReader& oReader, const double& dCurOpacity, CWString* pwsKey)
	{
		CBrush* pBrush = NULL;
		CWString wsNodeName = oReader.GetName();
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
						pBrush = new CImageBrush(oReader.GetText());
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
							CWString wsOpacity = oReader.GetText();

							if (!wsOpacity.empty())
								dOpacity = GetDouble(wsOpacity.c_str());
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
							CWString wsOpacity = oReader.GetText();

							if (!wsOpacity.empty())
								dOpacity = GetDouble(wsOpacity.c_str());
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
					wsNodeName = oReader.GetName();
					if ((wsNodeName == L"LinearGradientBrush.GradientStops" && bLinearGradient)
						|| (wsNodeName == L"RadialGradientBrush.GradientStops" && !bLinearGradient))
					{
						ReadGradientStops(oReader, vColors, vPositions, dOpacity * dCurOpacity);
					}
				}
			}

			LONG lCount = min(vColors.size(), vPositions.size());
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