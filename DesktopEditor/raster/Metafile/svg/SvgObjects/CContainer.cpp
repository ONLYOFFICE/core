#include "CContainer.h"

#include "../CSvgFile.h"
#include "../CSvgParser.h"

namespace SVG
{
	CGraphicsContainer::CGraphicsContainer(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CSvgGraphicsObject(oNode, pParent)
	{
		m_oWindow.m_oX     .SetValue(oNode.GetAttribute(L"x"));
		m_oWindow.m_oY     .SetValue(oNode.GetAttribute(L"y"));
		m_oWindow.m_oWidth .SetValue(oNode.GetAttribute(L"width"));
		m_oWindow.m_oHeight.SetValue(oNode.GetAttribute(L"height"));

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
	}

	CGraphicsContainer::CGraphicsContainer(double dWidth, double dHeight, XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
		: CSvgGraphicsObject(oNode, pParent), m_oWindow{0, 0, dWidth, dHeight}
	{}

	void CGraphicsContainer::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{}

	bool CGraphicsContainer::Draw(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip, const TSvgStyles *pOtherStyles) const
	{
		if (NULL == pRenderer)
			return false;

		for (const CSvgGraphicsObject* pObject : m_arObjects)
			pObject->Draw(pRenderer, pDefs, bIsClip, pOtherStyles);

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

	void CGraphicsContainer::ApplyStyle(IRenderer *pRenderer, const TSvgStyles *pStyles, const CDefs *pDefs, int &nTypePath, Aggplus::CMatrix &oOldMatrix) const
	{}

	TBounds CGraphicsContainer::GetBounds() const
	{
		TBounds oBounds, oTempBounds;

		oBounds.m_dLeft    = oBounds.m_dRight  = m_oWindow.m_oX.ToDouble(NSCSS::Pixel);
		oBounds.m_dTop     = oBounds.m_dBottom = m_oWindow.m_oY.ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  += m_oWindow.m_oWidth.ToDouble(NSCSS::Pixel);
		oBounds.m_dBottom += m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel);

		for (const CSvgGraphicsObject* pObject : m_arObjects)
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
