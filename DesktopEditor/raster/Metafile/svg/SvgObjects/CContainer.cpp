#include "CContainer.h"

#include "../CSvgFile.h"
#include "../CSvgParser.h"

namespace SVG
{
	CGraphicsContainer::CGraphicsContainer(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CSvgGraphicsObject(oNode, pParent)
	{}

	CGraphicsContainer::CGraphicsContainer(double dWidth, double dHeight, XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CSvgGraphicsObject(oNode, pParent), m_oWindow{0, 0, dWidth, dHeight}
	{}

	void CGraphicsContainer::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"x"))
			m_oWindow.m_oX.SetValue(mAttributes.at(L"x"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"y"))
			m_oWindow.m_oY.SetValue(mAttributes.at(L"y"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"width"))
			m_oWindow.m_oWidth.SetValue(mAttributes.at(L"width"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"height"))
			m_oWindow.m_oHeight.SetValue(mAttributes.at(L"height"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"viewBox"))
		{
			std::vector<double> arValues = StrUtils::ReadDoubleValues(mAttributes.at(L"viewBox"));

			m_oViewBox.m_oX      = arValues[0];
			m_oViewBox.m_oY      = arValues[1];
			m_oViewBox.m_oWidth  = arValues[2];
			m_oViewBox.m_oHeight = arValues[3];
		}
	}

//	bool CGraphicsContainer::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
//	{
//		std::wstring wsNodeName = oNode.GetName();

//		if (L"svg"     != wsNodeName &&
//		    L"g"       != wsNodeName &&
//		    L"xml"     != wsNodeName /*&&
//			L"defs"    != wsNodeName &&
//			L"pattern" != wsNodeName &&
//			L"linearGradient" != wsNodeName &&
//			L"radialGradient" != wsNodeName*/)
//			return false;

////		Clear();

//		SaveNodeData(oNode);

//		return true;
//	}

	bool CGraphicsContainer::Draw(IRenderer *pRenderer, const CDefs *pDefs) const
	{
		if (NULL == pRenderer)
			return false;

		for (const CSvgGraphicsObject* pObject : m_arObjects)
			pObject->Draw(pRenderer, pDefs);

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

	void CGraphicsContainer::ApplyStyle(IRenderer *pRenderer, const CDefs *pDefs, int &nTypePath, Aggplus::CMatrix &oOldMatrix) const
	{}

	TBounds CGraphicsContainer::GetBounds() const
	{
		TBounds oBounds{0., 0., 0., 0.}, oTempBounds;

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
