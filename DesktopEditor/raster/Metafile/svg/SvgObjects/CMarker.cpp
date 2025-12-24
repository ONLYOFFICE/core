#include "CMarker.h"

#include "../SvgUtils.h"

namespace SVG
{
	CMarker::CMarker(CSvgReader& oReader)
		: CObject(oReader), m_enUnits{EMarkerUnits::StrokeWidth}, m_enOrient{EMarkerOrient::Angle},
	      m_dAngle(0.), m_oBounds{0., 0., 0., 0.}
	{
		m_oWindow.m_oWidth.SetValue(3, NSCSS::Pixel);
		m_oWindow.m_oHeight.SetValue(3, NSCSS::Pixel);
	}

	ObjectType CMarker::GetType() const
	{
		return AppliedObject;
	}

	void CMarker::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		if ("refX" == sName)
			m_oWindow.m_oX.SetValue(oReader.GetText());
		else if ("refY" == sName)
			m_oWindow.m_oY.SetValue(oReader.GetText());
		else if ("markerWidth" == sName)
			m_oWindow.m_oWidth.SetValue(oReader.GetText());
		else if ("markerHeight" == sName)
			m_oWindow.m_oHeight.SetValue(oReader.GetText());
		else if ("viewBox" == sName)
		{
			const  std::vector<double> arValues{StrUtils::ReadDoubleValues(oReader.GetText())};
			if (4 == arValues.size())
			{
				m_oViewBox.m_oX      = arValues[0];
				m_oViewBox.m_oY      = arValues[1];
				m_oViewBox.m_oWidth  = arValues[2];
				m_oViewBox.m_oHeight = arValues[3];
			}
		}
		else if ("markerUnits" == sName)
		{
			if (L"userSpaceOnUse" == oReader.GetText())
				m_enUnits = EMarkerUnits::UserSpaceOnUse;
		}
		else if ("orient" == sName)
		{
			const std::wstring& wsOrient{oReader.GetText()};

			if (L"auto" == wsOrient)
				m_enOrient = EMarkerOrient::Auto;
			else if (L"auto-start-reverse" == wsOrient)
				m_enOrient = EMarkerOrient::Auto_start_reverse;
			else if (!StrUtils::ReadAngle(wsOrient, m_dAngle))
				StrUtils::ReadDoubleValue(wsOrient, m_dAngle);
		}
		else
			CObject::SetAttribute(sName, oReader);
	}

	void CMarker::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{}

	void CMarker::Draw(IRenderer *pRenderer, const CSvgFile* pFile, const TMarkerExternData& oExternalData, CommandeMode oMode, const TSvgStyles* pOtherStyles, const CRenderedObject* pContexObject) const
	{
		if (NULL == oExternalData.m_pPoints || oExternalData.m_pPoints->empty() || m_arObjects.empty() || (EMarkerUnits::StrokeWidth == m_enUnits && Equals(0., oExternalData.m_dStroke)))
			return;

		const double dMaxScale = ((EMarkerUnits::StrokeWidth == m_enUnits) ? oExternalData.m_dStroke : 1.) *
		                         ((!m_oViewBox.m_oWidth.Empty() && !m_oViewBox.m_oHeight.Empty()) ?
		                              std::max((m_oWindow.m_oWidth.ToDouble(NSCSS::Pixel) / m_oViewBox.m_oWidth.ToDouble(NSCSS::Pixel)),
		                                       (m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel) / m_oViewBox.m_oHeight.ToDouble(NSCSS::Pixel))) :
		                              1.);

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
