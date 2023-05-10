#include "CLine.h"

#include "CStyle.h"
#include "CContainer.h"

namespace SVG
{
	CLine::CLine(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CSvgGraphicsObject(oNode, pParent)
	{
		m_oX1.SetValue(oNode.GetAttribute(L"x1"));
		m_oY1.SetValue(oNode.GetAttribute(L"y1"));
		m_oX2.SetValue(oNode.GetAttribute(L"x2"));
		m_oY2.SetValue(oNode.GetAttribute(L"y2"));
	}

	void CLine::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetTransform(mAttributes, ushLevel, bHardMode);
		SetStroke(mAttributes, ushLevel, bHardMode);
		SetClip(mAttributes, ushLevel, bHardMode);
	}

	bool CLine::Draw(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip, const TSvgStyles *pOtherStyles) const
	{
		if (NULL == pRenderer)
			return false;

		TBounds oBounds = (NULL != m_pParent) ? m_pParent->GetBounds() : TBounds{0., 0., 0., 0.};

		double dParentWidth  = oBounds.m_dRight  - oBounds.m_dLeft;
		double dParentHeight = oBounds.m_dBottom - oBounds.m_dTop;

		double dX1 = m_oX1.ToDouble(NSCSS::Pixel, dParentWidth);
		double dY1 = m_oY1.ToDouble(NSCSS::Pixel, dParentHeight);
		double dX2 = m_oX2.ToDouble(NSCSS::Pixel, dParentWidth);
		double dY2 = m_oY2.ToDouble(NSCSS::Pixel, dParentHeight);

		StartPath(pRenderer, pDefs, bIsClip);

		pRenderer->PathCommandMoveTo(dX1, dY1);
		pRenderer->PathCommandLineTo(dX2, dY2);

		EndPath(pRenderer, pDefs, bIsClip, pOtherStyles);

		return true;
	}

	void CLine::ApplyStyle(IRenderer *pRenderer, const TSvgStyles *pStyles, const CDefs *pDefs, int &nTypePath, Aggplus::CMatrix &oOldMatrix) const
	{
		Apply(pRenderer, &pStyles->m_oTransform, oOldMatrix);

		if (Apply(pRenderer, &pStyles->m_oStroke, true))
			nTypePath += c_nStroke;
	}

	TBounds CLine::GetBounds() const
	{
		TBounds oBounds;

		oBounds.m_dLeft   = m_oX1.ToDouble(NSCSS::Pixel);
		oBounds.m_dTop    = m_oY1.ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  = m_oX2.ToDouble(NSCSS::Pixel);
		oBounds.m_dBottom = m_oY2.ToDouble(NSCSS::Pixel);

		return oBounds;
	}
}
