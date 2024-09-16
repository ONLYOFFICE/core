#include "CMarker.h"

#include "../../../graphics/pro/Graphics.h"
#include "../SvgUtils.h"

namespace SVG
{
	CMarker::CMarker(XmlUtils::CXmlNode &oNode)
		: CObject(oNode), m_oBounds{0., 0., 0., 0.}
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
			m_enUnits = Marker_UserSpaceOnUse;
		else
			m_enUnits = Marker_StrokeWidth;
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

	void CMarker::Draw(IRenderer *pRenderer, const CSvgFile* pFile, const std::vector<Point> &arPoints, double dStrokeWidth, CommandeMode oMode, const TSvgStyles* pOtherStyles, const CRenderedObject* pContexObject) const
	{
		if (arPoints.empty() || Equals(0., dStrokeWidth) || m_arObjects.empty())
			return;

		const double dMaxScale = ((Marker_StrokeWidth == m_enUnits) ? dStrokeWidth : 1.) * std::max((m_oWindow.m_oWidth.ToDouble(NSCSS::Pixel) / m_oViewBox.m_oWidth.ToDouble(NSCSS::Pixel)), (m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel) / m_oViewBox.m_oHeight.ToDouble(NSCSS::Pixel)));

		double dM11, dM12, dM21, dM22, dDx, dDy;
		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dDx, &dDy);

		Aggplus::CMatrix oTransform(dM11, dM12, dM21, dM22, dDx, dDy);

		const double dSkipX = m_oWindow.m_oX.ToDouble(NSCSS::Pixel) * dMaxScale;
		const double dSkipY = m_oWindow.m_oY.ToDouble(NSCSS::Pixel) * dMaxScale;

		oTransform.Translate(-dSkipX, -dSkipY);

		for (Point oPoint : arPoints)
		{
			oTransform.TransformPoint(oPoint.dX, oPoint.dY);

			pRenderer->SetTransform(dM11 * dMaxScale, dM12, dM21, dM22 * dMaxScale, oPoint.dX, oPoint.dY);

			for (const CRenderedObject* pObject : m_arObjects)
				pObject->Draw(pRenderer, pFile, oMode, pOtherStyles, pContexObject);
		}

		pRenderer->SetTransform(dM11, dM12, dM21, dM22, dDx, dDy);
	}
}
