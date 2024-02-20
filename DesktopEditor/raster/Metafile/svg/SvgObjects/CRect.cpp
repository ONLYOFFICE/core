#include "CRect.h"
#include "CStyle.h"
#include "CContainer.h"
#include "../SvgTypes.h"

namespace SVG
{
	CRect::CRect(XmlUtils::CXmlNode& oNode, CRenderedObject *pParent)
		: CRenderedObject(oNode, pParent)
	{
		m_oRect.m_oX     .SetValue(oNode.GetAttribute(L"x"));
		m_oRect.m_oY     .SetValue(oNode.GetAttribute(L"y"));
		m_oRect.m_oWidth .SetValue(oNode.GetAttribute(L"width"));
		m_oRect.m_oHeight.SetValue(oNode.GetAttribute(L"height"));

		m_oRx.SetValue(oNode.GetAttribute(L"rx"));
		m_oRy.SetValue(oNode.GetAttribute(L"ry"));

		if (m_oRx.Empty() && !m_oRy.Empty())
			m_oRx = m_oRy;
		else if (!m_oRx.Empty() && m_oRy.Empty())
			m_oRy = m_oRx;
	}

	CRect::~CRect()
	{}

	void CRect::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		CRenderedObject::SetData(mAttributes, ushLevel, bHardMode);

		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);
	}

	bool CRect::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles) const
	{
		Aggplus::CMatrix oOldTransform;

		if (!StartPath(pRenderer, pFile, oOldTransform, oMode))
			return false;

		TBounds oBounds = (NULL != m_pParent) ? m_pParent->GetBounds() : TBounds{0., 0., 0., 0.};

		double dParentWidth  = oBounds.m_dRight  - oBounds.m_dLeft;
		double dParentHeight = oBounds.m_dBottom - oBounds.m_dTop;

		double dX      = m_oRect.m_oX     .ToDouble(NSCSS::Pixel, dParentWidth);
		double dY      = m_oRect.m_oY     .ToDouble(NSCSS::Pixel, dParentHeight);
		double dWidth  = m_oRect.m_oWidth .ToDouble(NSCSS::Pixel, dParentWidth);
		double dHeight = m_oRect.m_oHeight.ToDouble(NSCSS::Pixel, dParentHeight);

		if (m_oRx.Empty() && m_oRy.Empty())
		{
			pRenderer->PathCommandMoveTo(dX, dY);
			pRenderer->PathCommandLineTo(dX + dWidth, dY);
			pRenderer->PathCommandLineTo(dX + dWidth, dY + dHeight);
			pRenderer->PathCommandLineTo(dX, dY + dHeight);
			pRenderer->PathCommandClose();
		}
		else
		{
			double dRx = m_oRx.ToDouble(NSCSS::Pixel);
			double dRy = m_oRy.ToDouble(NSCSS::Pixel);

			pRenderer->PathCommandMoveTo(dX, dY + dRy);

			pRenderer->PathCommandLineTo(dX, dY + dHeight - dRy);
			pRenderer->PathCommandArcTo(dX, dY + dHeight - dRy * 2, dRx * 2., dRy * 2., 180., -90.);

			pRenderer->PathCommandLineTo(dX + dWidth - dRx, dY + dHeight);
			pRenderer->PathCommandArcTo(dX + dWidth - dRx * 2, dY + dHeight - dRy * 2., dRx * 2., dRy * 2., 90, -90.0);

			pRenderer->PathCommandLineTo(dX + dWidth, dY + dRy);
			pRenderer->PathCommandArcTo(dX + dWidth - dRx * 2.0, dY, dRx * 2.0, dRy * 2.0, 0.0, -90.0);

			pRenderer->PathCommandLineTo(dX + dRx, dY);
			pRenderer->PathCommandArcTo(dX, dY, dRx * 2.0, dRy * 2.0, 270, -90.0);

			pRenderer->PathCommandClose();
		}

		EndPath(pRenderer, pFile, oOldTransform, oMode, pOtherStyles);

		return true;
	}

	void CRect::ApplyStyle(IRenderer *pRenderer, const TSvgStyles *pStyles, const CSvgFile *pFile, int &nTypePath) const
	{
		if (ApplyStroke(pRenderer, &pStyles->m_oStroke))
			nTypePath += c_nStroke;

		if (ApplyFill(pRenderer, &pStyles->m_oFill, pFile, true))
			nTypePath += c_nWindingFillMode;
	}

	TBounds CRect::GetBounds() const
	{
		TBounds oBounds;

		oBounds.m_dLeft   = m_oRect.m_oX.ToDouble(NSCSS::Pixel);
		oBounds.m_dTop    = m_oRect.m_oY.ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  = oBounds.m_dLeft + m_oRect.m_oWidth.ToDouble(NSCSS::Pixel);
		oBounds.m_dBottom = oBounds.m_dTop  + m_oRect.m_oHeight.ToDouble(NSCSS::Pixel);

		return oBounds;
	}
}
