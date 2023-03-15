#include "CEllipse.h"

#include "CStyle.h"
#include "CContainer.h"

namespace SVG
{
	CEllipse::CEllipse(XmlUtils::CXmlNode &oNode, CSvgGraphicsObject *pParent)
	    : CSvgGraphicsObject(oNode, pParent)
	{
		m_oCx.SetValue(oNode.GetAttribute(L"cx"));
		m_oCy.SetValue(oNode.GetAttribute(L"cy"));
		m_oRx.SetValue(oNode.GetAttribute(L"rx"));
		m_oRy.SetValue(oNode.GetAttribute(L"ry"));
	}

	void CEllipse::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetTransform(mAttributes, ushLevel, bHardMode);
		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);
		SetClip(mAttributes, ushLevel, bHardMode);
	}

	bool CEllipse::Draw(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip) const
	{
		if (NULL == pRenderer)
			return false;

		TBounds oBounds = (NULL != m_pParent) ? m_pParent->GetBounds() : TBounds{0., 0., 0., 0.};

		double dParentWidth  = oBounds.m_dRight  - oBounds.m_dLeft;
		double dParentHeight = oBounds.m_dBottom - oBounds.m_dTop;

		double dX  = m_oCx.ToDouble(NSCSS::Pixel, dParentWidth);
		double dY  = m_oCy.ToDouble(NSCSS::Pixel, dParentHeight);
		double dRx = m_oRx.ToDouble(NSCSS::Pixel, dParentWidth);
		double dRy = m_oRy.ToDouble(NSCSS::Pixel, dParentHeight);

		StartPath(pRenderer, pDefs, bIsClip);

		pRenderer->PathCommandMoveTo(dX + dRx, dY);
		pRenderer->PathCommandArcTo(dX - dRx, dY - dRy, dRx * 2.0, dRy * 2.0, 0, 360);

		EndPath(pRenderer, pDefs, bIsClip);

		return true;
	}

	CEllipse *CEllipse::Copy() const
	{
		return new CEllipse(*this);
	}

	void CEllipse::ApplyStyle(IRenderer *pRenderer, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer, oOldMatrix);
		ApplyStroke(pRenderer, nTypePath);
		ApplyFill(pRenderer, pDefs, nTypePath);
	}

	TBounds CEllipse::GetBounds() const
	{
		TBounds oBounds;

		oBounds.m_dLeft   = m_oCx.ToDouble(NSCSS::Pixel);
		oBounds.m_dTop    = m_oCy.ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  = oBounds.m_dLeft + m_oRx.ToDouble(NSCSS::Pixel);
		oBounds.m_dBottom = oBounds.m_dTop  + m_oRy.ToDouble(NSCSS::Pixel);;

		return oBounds;
	}
}
