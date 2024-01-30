#include "CContainer.h"

#include "../CSvgFile.h"
#include "../SvgUtils.h"

namespace SVG
{
	CGraphicsContainer::CGraphicsContainer(const std::wstring &wsName)
		: CRenderedObject(NSCSS::CNode(wsName, L"", L""))
	{}

	CGraphicsContainer::~CGraphicsContainer()
	{
		for (CRenderedObject* pObject : m_arObjects)
			pObject->m_pParent = NULL;
	}

	void CGraphicsContainer::SetData(XmlUtils::CXmlNode &oNode)
	{
		SetNodeData(oNode);

		m_oWindow.m_oX     .SetValue(oNode.GetAttribute(L"x"));
		m_oWindow.m_oY     .SetValue(oNode.GetAttribute(L"y"));
		m_oWindow.m_oWidth .SetValue(oNode.GetAttribute(L"width"), 0, true);
		m_oWindow.m_oHeight.SetValue(oNode.GetAttribute(L"height"), 0, true);

		const std::wstring wsViewBox = oNode.GetAttribute(L"viewBox");

		if (!wsViewBox.empty())
		{
			std::vector<double> arValues = StrUtils::ReadDoubleValues(wsViewBox);
			if (4 == arValues.size())
			{
				m_oViewBox.m_oX      = arValues[0];
				m_oViewBox.m_oY      = arValues[1];
				m_oViewBox.m_oWidth  = arValues[2];
				m_oViewBox.m_oHeight = arValues[3];
			}
		}
		else
			m_oViewBox = m_oWindow;
	}

	CGraphicsContainer::CGraphicsContainer(XmlUtils::CXmlNode& oNode, CRenderedObject *pParent)
		: CRenderedObject(oNode, pParent)
	{
		SetData(oNode);
	}

	CGraphicsContainer::CGraphicsContainer(double dWidth, double dHeight, XmlUtils::CXmlNode& oNode, CRenderedObject *pParent)
		: CRenderedObject(oNode, pParent), m_oWindow{0, 0, dWidth, dHeight}
	{}

	bool CGraphicsContainer::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles) const
	{
		Aggplus::CMatrix oOldTransform;

		if (!StartPath(pRenderer, pFile, oOldTransform, oMode))
			return false;

		for (const CRenderedObject* pObject : m_arObjects)
			pObject->Draw(pRenderer, pFile, oMode, pOtherStyles);

		EndPath(pRenderer, pFile, oOldTransform, oMode, pOtherStyles);

		return true;
	}

	TRect CGraphicsContainer::GetWindow() const
	{
		return m_oWindow;
	}

	TRect CGraphicsContainer::GetViewBox() const
	{
		return m_oViewBox;
	}

	TBounds CGraphicsContainer::GetBounds() const
	{
		TBounds oBounds, oTempBounds;

		oBounds.m_dLeft    = oBounds.m_dRight  = m_oWindow.m_oX.ToDouble(NSCSS::Pixel);
		oBounds.m_dTop     = oBounds.m_dBottom = m_oWindow.m_oY.ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  += m_oWindow.m_oWidth.ToDouble(NSCSS::Pixel);
		oBounds.m_dBottom += m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel);

		for (const CRenderedObject* pObject : m_arObjects)
		{
			oTempBounds = pObject->GetBounds();
			oBounds.m_dLeft   = std::min(oBounds.m_dLeft, oTempBounds.m_dLeft);
			oBounds.m_dTop    = std::min(oBounds.m_dTop, oTempBounds.m_dTop);
			oBounds.m_dRight  = std::max(oBounds.m_dRight, oTempBounds.m_dRight);
			oBounds.m_dBottom = std::max(oBounds.m_dBottom, oTempBounds.m_dBottom);
		}

		return oBounds;
	}

}
