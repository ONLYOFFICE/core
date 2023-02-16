#include "CGradient.h"

namespace SVG
{
	CStopElement::CStopElement(CObjectBase *pParent)
	    : CObjectBase(pParent)
	{}

	SvgDigit CStopElement::GetOffset() const
	{
		return m_oOffset;
	}

	SvgColor CStopElement::GetColor() const
	{
		return m_oColor;
	}

	void CStopElement::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"stop-color"))
			m_oColor.SetValue(mAttributes.at(L"stop-color"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"offset"))
			m_oOffset.SetValue(mAttributes.at(L"offset"), ushLevel, bHardMode);
	}

	bool CStopElement::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		if (!oNode.IsValid())
			return false;

		SaveNodeData(oNode);

		return true;
	}

	bool CStopElement::Draw(IRenderer *pRenderer, CDefs *pDefs) const
	{
		return true;
	}

	void CStopElement::ApplyStyle(IRenderer *pRenderer, CDefs *pDefs, int &nTypePath, Aggplus::CMatrix &oOldMatrix) const
	{}

	TBounds CStopElement::GetBounds() const
	{
		return {0., 0., 0., 0.};
	}

	CGradient::CGradient(CObjectBase *pParent)
	    : CContainer(pParent)
	{}

	bool CGradient::Apply(IRenderer *pRenderer, CDefs *pDefs, const double dParentWidth, const double dParentHeight)
	{
		if (NULL == pRenderer || NULL == pDefs)
			return false;

		std::vector<LONG> arColors;
		std::vector<double> arPositions;

		CStopElement *pStopElement = NULL;

		for (CObjectBase* pObject : m_arObjects)
		{
			pStopElement = dynamic_cast<CStopElement*>(pObject);
			if (NULL != pStopElement)
			{
				arColors.push_back(pStopElement->GetColor().ToInt());
				arPositions.push_back(pStopElement->GetOffset().ToDouble());
				pStopElement = NULL;
			}
		}

		pRenderer->put_BrushGradientColors(arColors.data(), arPositions.data(), arColors.size());

		return true;
	}

	CLinearGradient::CLinearGradient(CObjectBase *pParent)
	    : CGradient(pParent)
	{}

	void CLinearGradient::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"x1"))
			m_oX1.SetValue(mAttributes.at(L"x1"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"y1"))
			m_oY1.SetValue(mAttributes.at(L"y1"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"x2"))
			m_oX2.SetValue(mAttributes.at(L"x2"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"y2"))
			m_oY2.SetValue(mAttributes.at(L"y2"), ushLevel, bHardMode);
	}

	bool CLinearGradient::Apply(IRenderer *pRenderer, CDefs *pDefs, const double dParentWidth, const double dParentHeight)
	{
		if (CGradient::Apply(pRenderer, pDefs, dParentWidth, dParentHeight))
			pRenderer->put_BrushType(c_BrushTypePathNewLinearGradient);
		else
			return false;

		return true;
	}
}
