#include "CMarker.h"

#include "../../../graphics/pro/Graphics.h"
#include "../SvgUtils.h"

namespace SVG
{
	CMarker::CMarker(XmlUtils::CXmlNode &oNode)
		: CObject(oNode), m_dAngle(0.), m_oBounds{0., 0., 0., 0.}
	{
		m_oWindow.m_oX     .SetValue(oNode.GetAttribute(L"refX"));
		m_oWindow.m_oY     .SetValue(oNode.GetAttribute(L"refY"));

		m_oWindow.m_oWidth .SetValue(oNode.GetAttribute(L"markerWidth",  L"3"));
		m_oWindow.m_oHeight.SetValue(oNode.GetAttribute(L"markerHeight", L"3"));

		m_oViewBox = m_oWindow;

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

		const std::wstring& wsUnits = oNode.GetAttribute(L"markerUnits");

		if (L"userSpaceOnUse" == wsUnits)
			m_enUnits = EMarkerUnits::UserSpaceOnUse;
		else
			m_enUnits = EMarkerUnits::StrokeWidth;

		const std::wstring& wsOrient = oNode.GetAttribute(L"orient");

		if (L"auto" == wsOrient)
			m_enOrient = EMarkerOrient::Auto;
		else if (L"auto-start-reverse" == wsOrient)
			m_enOrient = EMarkerOrient::Auto_start_reverse;
		else
		{
			m_enOrient = EMarkerOrient::Angle;
			if (!StrUtils::ReadAngle(wsOrient, m_dAngle))
				StrUtils::ReadDoubleValue(wsOrient, m_dAngle);
		}
	}

	CMarker::~CMarker()
	{
	}

	ObjectType CMarker::GetType() const
	{
		return AppliedObject;
	}

	void CMarker::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{}

	void CMarker::Draw(IRenderer *pRenderer, const CSvgFile* pFile, const TMarkerExternData& oExternalData, CommandeMode oMode, const TSvgStyles* pOtherStyles, const CRenderedObject* pContexObject) const
	{
		if (NULL == oExternalData.m_pPoints || oExternalData.m_pPoints->empty() || m_arObjects.empty() || (EMarkerUnits::StrokeWidth == m_enUnits && Equals(0., oExternalData.m_dStroke)))
			return;

		const double dMaxScale = ((EMarkerUnits::StrokeWidth == m_enUnits) ? oExternalData.m_dStroke : 1.) * std::max((m_oWindow.m_oWidth.ToDouble(NSCSS::Pixel) / m_oViewBox.m_oWidth.ToDouble(NSCSS::Pixel)), (m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel) / m_oViewBox.m_oHeight.ToDouble(NSCSS::Pixel)));

		double dM11, dM12, dM21, dM22, dDx, dDy;
		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dDx, &dDy);

		const double dSkipX = m_oWindow.m_oX.ToDouble(NSCSS::Pixel) * dMaxScale;
		const double dSkipY = m_oWindow.m_oY.ToDouble(NSCSS::Pixel) * dMaxScale;

		for (const TPointData& oPoint : *oExternalData.m_pPoints)
		{
			Aggplus::CMatrix oTransform(dM11, dM12, dM21, dM22, dDx, dDy);
			oTransform.Translate(oPoint.m_oPoint.dX - dSkipX, oPoint.m_oPoint.dY - dSkipY);

			if (EMarkerOrient::Angle == m_enOrient)
				oTransform.RotateAt(m_dAngle, -dSkipX, -dSkipY);
			else if (!Equals(0., oPoint.m_dAngle))
				oTransform.RotateAt(oPoint.m_dAngle, -dSkipX, -dSkipY);

			Aggplus::CMatrix oTransform2(oTransform);
			oTransform2.Scale(dMaxScale, dMaxScale);

			pRenderer->SetTransform(oTransform2.sx(), oTransform2.shy(), oTransform2.shx(), oTransform2.sy(), oTransform.tx(), oTransform.ty());

			for (const CRenderedObject* pObject : m_arObjects)
				pObject->Draw(pRenderer, pFile, oMode, pOtherStyles, pContexObject);
		}

		pRenderer->SetTransform(dM11, dM12, dM21, dM22, dDx, dDy);
	}

	bool CMarker::NeedExternalAngle() const
	{
		return m_enOrient != EMarkerOrient::Angle;
	}

	EMarkerOrient CMarker::GetOrientType() const
	{
		return m_enOrient;
	}
}
