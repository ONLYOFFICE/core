#include "CCircle.h"

#include "CContainer.h"
#include "CStyle.h"

namespace SVG
{
	CCircle::CCircle(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CSvgGraphicsObject(oNode, pParent)
	{
		m_oCx.SetValue(oNode.GetAttribute(L"cx"));
		m_oCy.SetValue(oNode.GetAttribute(L"cy"));
		m_oR .SetValue(oNode.GetAttribute(L"r"));
	}

	void CCircle::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetTransform(mAttributes, ushLevel, bHardMode);
		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);
		SetClip(mAttributes, ushLevel, bHardMode);
	}

	bool CCircle::Draw(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip) const
	{
		if (NULL == pRenderer)
			return false;

		TBounds oBounds = (NULL != m_pParent) ? m_pParent->GetBounds() : TBounds{0., 0., 0., 0.};

		double dX = m_oCx.ToDouble(NSCSS::Pixel, oBounds.m_dRight  - oBounds.m_dLeft);
		double dY = m_oCy.ToDouble(NSCSS::Pixel, oBounds.m_dBottom - oBounds.m_dTop);
		double dR = m_oR .ToDouble(NSCSS::Pixel);

		StartPath(pRenderer, pDefs, bIsClip);

		pRenderer->PathCommandMoveTo(dX + dR, dY);
		pRenderer->PathCommandArcTo(dX - dR, dY - dR, dR * 2.0, dR * 2.0, 0, 360);

		EndPath(pRenderer, pDefs, bIsClip);

		return true;
	}

	CCircle *CCircle::Copy() const
	{
		return new CCircle(*this);
	}

	void CCircle::ApplyStyle(IRenderer *pRenderer, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer, oOldMatrix);
		ApplyStroke(pRenderer, nTypePath);
		ApplyFill(pRenderer, pDefs, nTypePath, true);
	}

	TBounds CCircle::GetBounds() const
	{
		TBounds oBounds;

		oBounds.m_dLeft   = (m_oCx - m_oR / 2).ToDouble(NSCSS::Pixel);
		oBounds.m_dTop    = (m_oCy - m_oR / 2).ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  = oBounds.m_dLeft + m_oR.ToDouble(NSCSS::Pixel);
		oBounds.m_dBottom = oBounds.m_dTop  + m_oR.ToDouble(NSCSS::Pixel);

		return oBounds;
	}

}
