#include "CEllipse.h"

#include "CContainer.h"

namespace SVG
{
	CEllipse::CEllipse(CSvgReader& oReader, CRenderedObject* pParent)
		: CRenderedObject(oReader, pParent)
	{}

	void CEllipse::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		CRenderedObject::SetData(mAttributes, ushLevel, bHardMode);

		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);
	}

	void CEllipse::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		if ("cx" == sName)
			m_oCx.SetValue(oReader.GetDouble());
		else if ("cy" == sName)
			m_oCy.SetValue(oReader.GetDouble());
		else if ("rx" == sName)
			m_oRx.SetValue(oReader.GetDouble());
		else if ("ry" == sName)
			m_oRy.SetValue(oReader.GetDouble());
		else
			CRenderedObject::SetAttribute(sName, oReader);
	}

	bool CEllipse::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles, const CRenderedObject* pContexObject) const
	{
		Aggplus::CMatrix oOldTransform;

		if (!StartPath(pRenderer, pFile, oOldTransform, oMode))
			return false;

		TBounds oBounds = (NULL != m_pParent) ? m_pParent->GetBounds() : TBounds{0., 0., 0., 0.};

		double dParentWidth  = oBounds.m_dRight  - oBounds.m_dLeft;
		double dParentHeight = oBounds.m_dBottom - oBounds.m_dTop;

		double dX  = m_oCx.ToDouble(NSCSS::Pixel, dParentWidth);
		double dY  = m_oCy.ToDouble(NSCSS::Pixel, dParentHeight);
		double dRx = m_oRx.ToDouble(NSCSS::Pixel, dParentWidth);
		double dRy = m_oRy.ToDouble(NSCSS::Pixel, dParentHeight);

		pRenderer->PathCommandMoveTo(dX + dRx, dY);
		pRenderer->PathCommandArcTo(dX - dRx, dY - dRy, dRx * 2.0, dRy * 2.0, 0, 360);

		EndPath(pRenderer, pFile, oOldTransform, oMode, pOtherStyles, pContexObject);

		return true;
	}

	void CEllipse::ApplyStyle(IRenderer *pRenderer, const TSvgStyles *pStyles, const CSvgFile *pFile, int &nTypePath, const CRenderedObject* pContexObject) const
	{
		if (ApplyStroke(pRenderer, &pStyles->m_oStroke, false, pContexObject))
			nTypePath += c_nStroke;

		if (ApplyFill(pRenderer, &pStyles->m_oFill, pFile, true, pContexObject))
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
