#include "CGradient.h"

namespace SVG
{
	CStopElement::CStopElement(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CDefObject(oNode, pParent)
	{
		m_oColor.SetValue(oNode.GetAttribute(L"stop-color"));
		if (m_oOffset.SetValue(oNode.GetAttribute(L"offset")) && m_oOffset > 1.)
			m_oOffset /= 100;
		m_oColor.SetOpacity(oNode.GetAttribute(L"stop-opacity"));
	}

	SvgDigit CStopElement::GetOffset() const
	{
		return m_oOffset;
	}

	SvgColor CStopElement::GetColor() const
	{
		return m_oColor;
	}

	void CStopElement::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{}

	CGradient::CGradient()
	{}

	bool CGradient::Apply(IRenderer *pRenderer) const
	{
		if (NULL == pRenderer)
			return false;

		std::vector<LONG> arColors;
		std::vector<double> arPositions;

		for (const CStopElement* pStopElement : m_arObjects)
		{
			arColors.push_back(((LONG)pStopElement->GetColor().ToInt() + (pStopElement->GetColor().GetOpacity() << 24)));
			arPositions.push_back(pStopElement->GetOffset().ToDouble());
		}

		if (arColors.size() < 2)
			return false;

		pRenderer->put_BrushGradientColors(arColors.data(), arPositions.data(), arColors.size());

		return true;
	}

	CLinearGradient::CLinearGradient(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CDefObject(oNode, pParent)
	{
		m_oX1.SetValue(oNode.GetAttribute(L"x1"));
		m_oY1.SetValue(oNode.GetAttribute(L"y1"));
		m_oX2.SetValue(oNode.GetAttribute(L"x2"));
		m_oY2.SetValue(oNode.GetAttribute(L"y2"));
	}

	void CLinearGradient::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{}

	bool CLinearGradient::Apply(IRenderer *pRenderer, const CDefs *pDefs, const TBounds &oObjectBounds)
	{
		if (!CGradient::Apply(pRenderer))
			return false;

		pRenderer->put_BrushType(c_BrushTypePathGradient1);

		double dX1 = m_oX1.ToDouble(NSCSS::Pixel);
		double dY1 = m_oY1.ToDouble(NSCSS::Pixel);
		double dX2 = m_oX2.ToDouble(NSCSS::Pixel);
		double dY2 = m_oY2.ToDouble(NSCSS::Pixel);

		double dWidthKoef  = 1.;
		double dHeightKoef = 1.;

		if (NSCSS::UnitMeasure::Percent == m_oX2.GetUnitMeasure())
			dWidthKoef*= dX2 / 100.;
		else if (NSCSS::UnitMeasure::None ==  m_oX2.GetUnitMeasure() && dX2 > 1.)
			dWidthKoef = dX2;

		if (NSCSS::UnitMeasure::Percent == m_oY2.GetUnitMeasure())
			dHeightKoef*= dY2 / 100.;
		else if (NSCSS::UnitMeasure::None ==  m_oY2.GetUnitMeasure() && dY2 > 1.)
			dHeightKoef = dY2;

		double dAngle = 0.;

		if (std::fabs(dX2 - dX1) >= 0. || fabs(dY2 - dY1) >= 0.)
			dAngle = 180. * std::atan2(dY2 - dY1, dX2 - dX1) / M_PI;

		pRenderer->put_BrushLinearAngle(dAngle);
		pRenderer->BrushBounds(oObjectBounds.m_dLeft, oObjectBounds.m_dTop, (oObjectBounds.m_dRight - oObjectBounds.m_dLeft) * dWidthKoef, (oObjectBounds.m_dBottom - oObjectBounds.m_dTop) * dHeightKoef);

		return true;
	}

	CRadialGradient::CRadialGradient(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CDefObject(oNode, pParent)
	{}

	void CRadialGradient::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"cx"))
			m_oCx.SetValue(mAttributes.at(L"cx"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"cy"))
			m_oCy.SetValue(mAttributes.at(L"cy"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"r"))
			m_oR.SetValue(mAttributes.at(L"r"), ushLevel, bHardMode);
	}

	bool CRadialGradient::Apply(IRenderer *pRenderer, const CDefs *pDefs, const TBounds &oObjectBounds)
	{
		if (!CGradient::Apply(pRenderer))
			return false;

		pRenderer->put_BrushType(c_BrushTypePathGradient2);

		double dCx = m_oCx.ToDouble(NSCSS::Pixel) * (oObjectBounds.m_dRight - oObjectBounds.m_dLeft);
		double dCy = m_oCy.ToDouble(NSCSS::Pixel) * (oObjectBounds.m_dBottom - oObjectBounds.m_dTop);
		double dR  = m_oR .ToDouble(NSCSS::Pixel) * (oObjectBounds.m_dRight - oObjectBounds.m_dLeft);

		pRenderer->put_BrushCenterPoint(dCx, dCy);
		pRenderer->put_BrushRadius(dR);

		pRenderer->BrushBounds(oObjectBounds.m_dLeft, oObjectBounds.m_dTop, oObjectBounds.m_dRight - oObjectBounds.m_dLeft, oObjectBounds.m_dBottom - oObjectBounds.m_dTop);

		return true;
	}

}
