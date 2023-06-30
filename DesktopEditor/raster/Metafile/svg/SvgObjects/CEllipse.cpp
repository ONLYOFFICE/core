#include "CEllipse.h"

#include "CStyle.h"
#include "CContainer.h"

namespace SVG
{
	CEllipse::CEllipse(XmlUtils::CXmlNode &oNode, CRenderedObject *pParent)
		: CRenderedObject(oNode, pParent)
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
		SetMask(mAttributes, ushLevel, bHardMode);
		SetDisplay(mAttributes, ushLevel, bHardMode);
	}

	bool CEllipse::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles) const
	{
		if (NULL == pRenderer || !m_oStyles.m_bDisplay)
			return false;

		TBounds oBounds = (NULL != m_pParent) ? m_pParent->GetBounds() : TBounds{0., 0., 0., 0.};

		double dParentWidth  = oBounds.m_dRight  - oBounds.m_dLeft;
		double dParentHeight = oBounds.m_dBottom - oBounds.m_dTop;

		double dX  = m_oCx.ToDouble(NSCSS::Pixel, dParentWidth);
		double dY  = m_oCy.ToDouble(NSCSS::Pixel, dParentHeight);
		double dRx = m_oRx.ToDouble(NSCSS::Pixel, dParentWidth);
		double dRy = m_oRy.ToDouble(NSCSS::Pixel, dParentHeight);

		StartPath(pRenderer, pFile, oMode);

		pRenderer->PathCommandMoveTo(dX + dRx, dY);
		pRenderer->PathCommandArcTo(dX - dRx, dY - dRy, dRx * 2.0, dRy * 2.0, 0, 360);

		EndPath(pRenderer, pFile, oMode, pOtherStyles);

		return true;
	}

	void CEllipse::ApplyStyle(IRenderer *pRenderer, const TSvgStyles *pStyles, const CSvgFile *pFile, int &nTypePath, Aggplus::CMatrix &oOldMatrix) const
	{
		Apply(pRenderer, &pStyles->m_oTransform, oOldMatrix);

		if (Apply(pRenderer, &pStyles->m_oStroke, true))
			nTypePath += c_nStroke;

		if (Apply(pRenderer, &pStyles->m_oFill, pFile, true))
			nTypePath += c_nWindingFillMode;
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
