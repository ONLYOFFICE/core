#include "CPath.h"

#include <algorithm>
#include "CMarker.h"
#include "../CSvgFile.h"
#include "../SvgUtils.h"
#include <cfloat>

namespace SVG
{
	IPathElement::~IPathElement()
	{
	}

	#define ISPATHCOMMAND(wchar) L'M' == wchar || L'm' == wchar || L'Z' == wchar || L'z' == wchar || L'L' == wchar || L'l' == wchar || L'H' == wchar || L'h' == wchar || L'V' == wchar || L'v' == wchar || L'C' == wchar || L'c' == wchar || L'S' == wchar || L's' == wchar || L'Q' == wchar || L'q' == wchar || L'T' == wchar || L't' == wchar || L'A' == wchar || L'a' == wchar
	// IpathElement
	TBounds IPathElement::GetBounds() const
	{
		TBounds oBounds{DBL_MAX, DBL_MAX, -DBL_MAX, -DBL_MAX};

		for (const Point& oPoint : m_arPoints)
		{
			oBounds.m_dLeft   = std::min(oBounds.m_dLeft,   oPoint.dX);
			oBounds.m_dTop    = std::min(oBounds.m_dTop,    oPoint.dY);
			oBounds.m_dRight  = std::max(oBounds.m_dRight,  oPoint.dX);
			oBounds.m_dBottom = std::max(oBounds.m_dBottom, oPoint.dY);
		}

		return oBounds;
	}

	UINT IPathElement::GetPointCount() const
	{
		return m_arPoints.size();
	}

	Point IPathElement::operator[](int nIndex) const
	{
		if (m_arPoints.empty() || (nIndex > 0 && nIndex >= m_arPoints.size()) || (nIndex < 0 && -nIndex > m_arPoints.size()))
			return Point{0., 0.};

		return m_arPoints[(nIndex >= 0) ? nIndex : m_arPoints.size() + nIndex];
	}

	//CMoveElement
	CMoveElement::CMoveElement(const Point &oPoint)
	{
		m_arPoints.push_back(oPoint);
	}

	CMoveElement::~CMoveElement()
	{
	}

	EPathElement CMoveElement::GetType() const
	{
		return EPathElement::Move;
	}

	CMoveElement *CMoveElement::CreateFromArray(std::vector<double> &arValues, bool bRelativeCoordinate, IPathElement *pPrevElement)
	{
		if (arValues.size() < 2)
			return NULL;

		Point oTranslatePoint{0., 0.};

		if (bRelativeCoordinate && NULL != pPrevElement)
			oTranslatePoint = (*pPrevElement)[-1];

		CMoveElement *pMoveElement = new CMoveElement(Point{arValues[0], arValues[1]} + oTranslatePoint);

		arValues.erase(arValues.begin(), arValues.begin() + 2);

		return pMoveElement;
	}

	void CMoveElement::Draw(IRenderer *pRenderer) const
	{
		if (m_arPoints.empty())
			return;

		pRenderer->PathCommandMoveTo(m_arPoints[0].dX, m_arPoints[0].dY);
	}

	//CLineElement
	CLineElement::CLineElement(const Point &oPoint)
	{
		m_arPoints.push_back(oPoint);
	}

	CLineElement::~CLineElement()
	{}

	EPathElement CLineElement::GetType() const
	{
		return EPathElement::Line;
	}

	CLineElement *CLineElement::CreateFromArray(std::vector<double> &arValues, bool bRelativeCoordinate, IPathElement *pPrevElement)
	{
		if (arValues.size() < 2)
			return NULL;

		Point oTranslatePoint{0., 0.};

		if (bRelativeCoordinate && NULL != pPrevElement)
			oTranslatePoint = (*pPrevElement)[-1];

		CLineElement *pLineElement = new CLineElement(Point{arValues[0], arValues[1]} + oTranslatePoint);

		arValues.erase(arValues.begin(), arValues.begin() + 2);

		return pLineElement;
	}

	CLineElement *CLineElement::CreateFromVArray(std::vector<double> &arValues, bool bRelativeCoordinate, IPathElement *pPrevElement)
	{
		if (arValues.empty())
			return NULL;

		Point oTranslatePoint{0., 0.};

		if (NULL != pPrevElement)
			oTranslatePoint = (*pPrevElement)[-1];

		CLineElement *pLineElement = new CLineElement(Point{oTranslatePoint.dX, arValues[0] + ((bRelativeCoordinate) ? oTranslatePoint.dY : 0)});

		arValues.erase(arValues.begin());

		return pLineElement;
	}

	CLineElement *CLineElement::CreateFromHArray(std::vector<double> &arValues, bool bRelativeCoordinate, IPathElement *pPrevElement)
	{
		if (arValues.empty())
			return NULL;

		Point oTranslatePoint{0., 0.};

		if (NULL != pPrevElement)
			oTranslatePoint = (*pPrevElement)[-1];

		CLineElement *pLineElement = new CLineElement(Point{arValues[0] + ((bRelativeCoordinate) ? oTranslatePoint.dX : 0), oTranslatePoint.dY});

		arValues.erase(arValues.begin());

		return pLineElement;
	}

	void CLineElement::Draw(IRenderer *pRenderer) const
	{
		if (m_arPoints.empty())
			return;

		pRenderer->PathCommandLineTo(m_arPoints[0].dX, m_arPoints[0].dY);
	}

	//CCurveBezierElement
	CCBezierElement::CCBezierElement(const Point &oPoint1, const Point &oPoint2, const Point &oPointE, EPathElement enType)
		: m_enType(enType)
	{
		m_arPoints.push_back(oPoint1);
		m_arPoints.push_back(oPoint2);
		m_arPoints.push_back(oPointE);
	}
	CCBezierElement::~CCBezierElement()
	{
	}

	EPathElement CCBezierElement::GetType() const
	{
		return m_enType;
	}

	IPathElement *CCBezierElement::CreateFromArray(std::vector<double> &arValues, bool bRelativeCoordinate, IPathElement *pPrevElement)
	{
		if (arValues.size() < 6)
				return NULL;

		Point oTranslatePoint{0., 0.};

		if (bRelativeCoordinate && NULL != pPrevElement)
			oTranslatePoint = (*pPrevElement)[-1];

		CCBezierElement *pCBezierElement = new CCBezierElement(Point{arValues[0], arValues[1]} + oTranslatePoint,
															   Point{arValues[2], arValues[3]} + oTranslatePoint,
															   Point{arValues[4], arValues[5]} + oTranslatePoint);

		arValues.erase(arValues.begin(), arValues.begin() + 6);

		return pCBezierElement;
	}

	IPathElement *CCBezierElement::CreateFromSArray(std::vector<double> &arValues, bool bRelativeCoordinate, IPathElement *pPrevElement)
	{
		if (arValues.size() < 4)
			return NULL;

		Point oFirstPoint{0., 0.}, oTranslatePoint{0., 0.};

		if (NULL != pPrevElement)
		{
			oFirstPoint = (*pPrevElement)[-1];
			if (bRelativeCoordinate)
				oTranslatePoint = oFirstPoint;
		}

		if (EPathElement::SBezier == pPrevElement->GetType() ||
			EPathElement::CBezier == pPrevElement->GetType())
			oFirstPoint += oFirstPoint - (*pPrevElement)[-2];

		CCBezierElement *pCBezierElement = new CCBezierElement(oFirstPoint,
															   Point{arValues[0], arValues[1]} + oTranslatePoint,
															   Point{arValues[2], arValues[3]} + oTranslatePoint,
															   EPathElement::SBezier);

		arValues.erase(arValues.begin(), arValues.begin() + 4);

		return pCBezierElement;
	}

	IPathElement *CCBezierElement::CreateFromQArray(std::vector<double> &arValues, bool bRelativeCoordinate, IPathElement *pPrevElement)
	{
		if (arValues.size() < 4)
			return NULL;

		Point oLastPoint{0., 0.}, oTranslatePoint{0., 0.};

		if (NULL != pPrevElement)
		{
			oLastPoint = (*pPrevElement)[-1];
			if (bRelativeCoordinate)
				oTranslatePoint = oLastPoint;
		}

		CCBezierElement *pCBezierElement = new CCBezierElement(oLastPoint, Point{arValues[0], arValues[1]} + oTranslatePoint, Point{arValues[2], arValues[3]} + oTranslatePoint, EPathElement::QBezier);

		arValues.erase(arValues.begin(), arValues.begin() + 4);

		return pCBezierElement;
	}

	IPathElement *CCBezierElement::CreateFromTArray(std::vector<double> &arValues, bool bRelativeCoordinate, IPathElement *pPrevElement)
	{
		if (arValues.size() < 2)
			return NULL;

		Point oFirstPoint{0., 0.}, oSecondPoint{0., 0.}, oTranslatePoint{0., 0.};

		if (NULL != pPrevElement)
		{
			oFirstPoint  = (*pPrevElement)[-1];
			oSecondPoint = (*pPrevElement)[-2];
			if (bRelativeCoordinate)
				oTranslatePoint = oFirstPoint;
		}

		if (EPathElement::TBezier != pPrevElement->GetType() &&
			EPathElement::QBezier != pPrevElement->GetType())
		{
			oSecondPoint = Point{arValues[arValues.size() - 2], arValues[arValues.size() - 1]};

			arValues.erase(arValues.end() - 2, arValues.end());

			return new CLineElement(oSecondPoint + oTranslatePoint);
		}

		oSecondPoint = oFirstPoint + (oFirstPoint - oSecondPoint);

		CCBezierElement *pCBezierElement = new CCBezierElement(oFirstPoint, oSecondPoint, Point{arValues[0], arValues[1]} + oTranslatePoint, EPathElement::TBezier);

		arValues.erase(arValues.begin(), arValues.begin() + 2);

		return pCBezierElement;
	}

	std::vector<IPathElement *> CCBezierElement::CreateFromArc(std::vector<double> &arValues, bool bRelativeCoordinate, IPathElement *pPrevElement)
	{
		if (arValues.size() < 7 || Equals(0., arValues[0]) || Equals(0.,  arValues[1]))
			return std::vector<IPathElement *>();

		Point oTranslatePoint{0., 0.};

		if (bRelativeCoordinate && NULL != pPrevElement)
			oTranslatePoint = (*pPrevElement)[-1];

		Point oSrartPoint{(*pPrevElement)[-1]};
		Point oSecondPoint{arValues[5] + oTranslatePoint.dX, arValues[6] + oTranslatePoint.dY};

		if (oSrartPoint == oSecondPoint)
		{
			arValues.erase(arValues.begin(), arValues.begin() + 7);
			return std::vector<IPathElement *>();
		}

		std::vector<IPathElement *> arCurves;

		Point oRadius{arValues[0], arValues[1]};
		Point oCenter{0, 0};
		double dAngle = 0, dSweep = 0;

		CalculateArcData(oSrartPoint, oSecondPoint, oRadius, oCenter, arValues[2],  Equals(1., arValues[3]) ? true : false, Equals(1., arValues[4]) ? true : false, dAngle, dSweep);

		double dStartAngle = dAngle;
		double dEndAngle;

		Point oEndPoint, oControl1, oControl2;

		while (dStartAngle != (dAngle + dSweep))
		{
			if ((int)(dStartAngle / 90.) == dStartAngle / 90.)
				dEndAngle = dStartAngle + ((dSweep > 0.) ? 90. : -90.);
			else
			{
				dEndAngle = copysign(ceil(std::abs(dStartAngle) / 90.), dStartAngle) * ((dSweep > 0. || dStartAngle < 0.) ? 90. : -90.);

				if (dStartAngle < 0. && dSweep > 0.)
					dEndAngle += 90.;
			}
			if (std::abs(dAngle - dEndAngle) > std::abs(dSweep))
				dEndAngle = dAngle + dSweep;

			oEndPoint = Point{oRadius.dX * std::cos(dEndAngle * M_PI / 180.), oRadius.dY * std::sin(dEndAngle * M_PI / 180.)};
			oEndPoint.Rotate(arValues[2]);
			oEndPoint += oCenter;

			double dSweepRad = (dEndAngle - dStartAngle) * M_PI / 180.;

			if (std::abs(dSweepRad) > M_PI)
			{
				dSweepRad -= M_PI * 2.;
				dSweepRad = fmod(dSweepRad, M_PI * 2.);
			}

			double dFactor = (4. / 3.) * std::tan(dSweepRad / 4.);

			double distToCtrPointX = std::sqrt(oRadius.dX * oRadius.dX * (1 + dFactor * dFactor));
			double distToCtrPointY = std::sqrt(oRadius.dY * oRadius.dY * (1 + dFactor * dFactor));

			double dAngle1 = dStartAngle * M_PI / 180. + std::atan(dFactor);
			double dAngle2 = dEndAngle   * M_PI / 180. - std::atan(dFactor);

			oControl1 = {std::cos(dAngle1) * distToCtrPointX, std::sin(dAngle1) * distToCtrPointY};
			oControl2 = {std::cos(dAngle2) * distToCtrPointX, std::sin(dAngle2) * distToCtrPointY};

			oControl1.Rotate(arValues[2]);
			oControl2.Rotate(arValues[2]);

			oControl1 += oCenter;
			oControl2 += oCenter;

			arCurves.push_back(new CCBezierElement(oControl1, oControl2, oEndPoint));

			dStartAngle = dEndAngle;
		}

		arValues.erase(arValues.begin(), arValues.begin() + 7);

		return arCurves;
	}

	void CCBezierElement::Draw(IRenderer *pRenderer) const
	{
		if (3 != m_arPoints.size())
				return;

		pRenderer->PathCommandCurveTo(m_arPoints[0].dX, m_arPoints[0].dY,
		                              m_arPoints[1].dX, m_arPoints[1].dY,
		                              m_arPoints[2].dX, m_arPoints[2].dY);
	}

	inline double ClampSinCos(const double& d)
	{
		if (d < -1) return -1;
		if (d > 1)  return 1;
		return d;
	}

	void CCBezierElement::CalculateArcData(const Point &oFirst, const Point &oSecond, Point &oRadius, Point &oCenter, double dAngle, bool bLargeArc, bool bSweep, double &dStartAngle, double &dSweep)
	{
		dAngle *= M_PI / 180.;

		double dXp = ((oFirst.dX - oSecond.dX) / 2. * std::cos(dAngle)) + ((oFirst.dY - oSecond.dY) / 2. * std::sin(dAngle));
		double dYp = ((oFirst.dY - oSecond.dY) / 2. * std::cos(dAngle)) - ((oFirst.dX - oSecond.dX) / 2. * std::sin(dAngle));

		double dLambda = ((dXp * dXp) / (oRadius.dX * oRadius.dX)) + ((dYp * dYp) / (oRadius.dY * oRadius.dY));
		if(dLambda > 1.)
		{
			oRadius.dX *= std::sqrt(dLambda);
			oRadius.dY *= std::sqrt(dLambda);
		}

		double dRxRy = oRadius.dX * oRadius.dY;
		double dRxYp = oRadius.dX * dYp;
		double dRyXp = oRadius.dY * dXp;
		double dSumOfSq = dRxYp * dRxYp + dRyXp * dRyXp;

		double dCoef = std::sqrt(std::abs(dRxRy * dRxRy - dSumOfSq) / dSumOfSq) * ((bLargeArc == bSweep) ? -1 : 1);

		double dCpx =  dCoef * dRxYp / oRadius.dY;
		double dCpy = -dCoef * dRyXp / oRadius.dX;

		oCenter = Point{dCpx * std::cos(dAngle) - dCpy * std::sin(dAngle) + (oFirst.dX + oSecond.dX) / 2., dCpx * std::sin(dAngle) + dCpy * std::cos(dAngle) + (oFirst.dY + oSecond.dY) / 2.};

		double dStartAngleCos = ((dXp - dCpx) / oRadius.dX) / std::sqrt(std::pow((dXp - dCpx) / oRadius.dX, 2) + std::pow((dYp - dCpy) / oRadius.dY, 2));
		dStartAngle = std::acos(ClampSinCos(dStartAngleCos)) * 180. / M_PI;

		double dSweepAngleCos = (((dXp - dCpx) / oRadius.dX * (-dXp - dCpx) / oRadius.dX) + ((dYp - dCpy) / oRadius.dY * (-dYp - dCpy) / oRadius.dY)) / (std::sqrt(std::pow((dXp - dCpx) / oRadius.dX, 2) + std::pow((dYp - dCpy) / oRadius.dY, 2)) * std::sqrt(std::pow((-dXp - dCpx) / oRadius.dX, 2) + std::pow((-dYp - dCpy) / oRadius.dY, 2)));
		dSweep = std::acos(ClampSinCos(dSweepAngleCos)) * 180. / M_PI;

		if (((dYp - dCpy) / oRadius.dY) < 0)
			dStartAngle *= -1;

		if ((((dXp - dCpx) / oRadius.dX * (-dYp - dCpy) / oRadius.dY) - ((dYp - dCpy) / oRadius.dY * (-dXp - dCpx) / oRadius.dX)) < 0)
			dSweep *= -1;

		if (!bSweep && dSweep > 0)
			dSweep -= 360;

		if (bSweep && dSweep < 0)
			dSweep += 360;
	}

	//CCloseElement
	CCloseElement::CCloseElement()
	{}

	CCloseElement::~CCloseElement()
	{
	}

	EPathElement CCloseElement::GetType() const
	{
		return EPathElement::Close;
	}

	void CCloseElement::Draw(IRenderer *pRenderer) const
	{
		pRenderer->PathCommandClose();
	}

	#define LASTELEMENT(array) (array.empty()) ? NULL : array.back()
    #define RANGEALIGMENT(value, left, rigth) if (value < left) value = left; else if (value > rigth) value = rigth;
    #define EPSILON 0.05
    #define CURVESTEP 0.05
    #define MINCURVESTEP 0.001

	CPath::CPath(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent, bool bChechCommands)
		: CRenderedObject(oNode, pParent), m_bEvenOddRule(false)
	{
		if (bChechCommands)
			ReadFromString(oNode.GetAttribute(L"d"));
	}

	CPath::~CPath()
	{
		for (IPathElement* pPathElement : m_arElements)
			delete pPathElement;
	}

	void CPath::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		CRenderedObject::SetData(mAttributes, ushLevel, bHardMode);

		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);
		SetOpacity(mAttributes, ushLevel, bHardMode);
		SetMarker(mAttributes, ushLevel, bHardMode);

		std::map<std::wstring, std::wstring>::const_iterator oIter = mAttributes.find(L"fill-rule");

		if (mAttributes.end() != oIter)
		{
			if (L"evenodd" == oIter->second) m_bEvenOddRule = true;
			else if (L"nonzero" == oIter->second) m_bEvenOddRule = false;
		}
	}

	bool CPath::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles, const CRenderedObject* pContexObject) const
	{
		Aggplus::CMatrix oOldTransform;

		if (m_arElements.empty() || !StartPath(pRenderer, pFile, oOldTransform, oMode))
			return false;

		for (const IPathElement* oElement : m_arElements)
			oElement->Draw(pRenderer);

		EndPath(pRenderer, pFile, oOldTransform, oMode, pOtherStyles, pContexObject);

		DrawMarkers(pRenderer, pFile, oMode);

		return true;
	}

	IPathElement *CPath::operator[](int nIndex) const
	{
		if (m_arElements.empty() || (nIndex > 0 && nIndex >= m_arElements.size()) || (nIndex < 0 && -nIndex > m_arElements.size()))
			return NULL;

		return m_arElements[(nIndex >= 0) ? nIndex : m_arElements.size() + nIndex];
	}

	void CPath::ApplyStyle(IRenderer *pRenderer, const TSvgStyles *pStyles, const CSvgFile *pFile, int &nTypePath, const CRenderedObject* pContexObject) const
	{
		if (ApplyStroke(pRenderer, &pStyles->m_oStroke, false, pContexObject))
			nTypePath += c_nStroke;

		if (ApplyFill(pRenderer, &pStyles->m_oFill, pFile, true, pContexObject))
			nTypePath += (m_bEvenOddRule) ? c_nEvenOddFillMode : c_nWindingFillMode;
	}

	bool CPath::DrawMarkers(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles* pOtherStyles, const CRenderedObject* pContexObject) const
	{
		if (NULL == pRenderer || NULL == pFile || m_arElements.empty() || m_oStyles.m_oStroke.m_oWidth.Zero() ||
			(m_oMarkers.m_oStart.Empty() && m_oMarkers.m_oMid.Empty() && m_oMarkers.m_oEnd.Empty()))
			return false;

		Aggplus::CMatrix oOldMatrix;

		if (!StartPath(pRenderer, pFile, oOldMatrix, oMode))
			return false;

		const double dStrokeWidth = (m_oStyles.m_oStroke.m_oWidth.Empty()) ? 1. : m_oStyles.m_oStroke.m_oWidth.ToDouble(NSCSS::Pixel);

		#define CALCULATE_ANGLE(firstPoint, secondPoint) std::atan2(secondPoint.dY - firstPoint.dY, secondPoint.dX - firstPoint.dX) * 180. / M_PI

		if (!m_oMarkers.m_oStart.Empty() && NSCSS::NSProperties::ColorType::ColorUrl == m_oMarkers.m_oStart.GetType())
		{
			CMarker *pStartMarker = dynamic_cast<CMarker*>(pFile->GetMarkedObject(m_oMarkers.m_oStart.ToWString()));

			if (NULL != pStartMarker)
			{
				TMarkerExternData oExternalData;
				oExternalData.m_dStroke = dStrokeWidth;
				oExternalData.m_pPoints = new std::vector<TPointData>(1);

				int unIndex = FindIndexFirstNotEmpty();

				if (0 <= unIndex)
				{
					const IPathElement* pFirstElement{m_arElements[unIndex]};

					(*oExternalData.m_pPoints)[0].m_oPoint = (*pFirstElement)[0];

					if (pStartMarker->NeedExternalAngle())
					{
						Point oCurent{(*pFirstElement)[0]};
						Point oNext;

						if (pFirstElement->GetPointCount() > 1)
							oNext = (*pFirstElement)[1];
						else if (unIndex < m_arElements.size() - 1 && EPathElement::Close != m_arElements[unIndex + 1]->GetType() && EPathElement::Move != m_arElements[unIndex + 1]->GetType())
							oNext = (*m_arElements[unIndex + 1])[0];

						(*oExternalData.m_pPoints)[0].m_dAngle = CALCULATE_ANGLE(oCurent, oNext);

						if (EMarkerOrient::Auto_start_reverse == pStartMarker->GetOrientType())
							(*oExternalData.m_pPoints)[0].m_dAngle += 180.;
					}

					pStartMarker->Draw(pRenderer, pFile, oExternalData, oMode, pOtherStyles, this);
				}
			}
		}

		if (!m_oMarkers.m_oMid.Empty() && NSCSS::NSProperties::ColorType::ColorUrl == m_oMarkers.m_oMid.GetType())
		{
			CMarker *pMidMarker = dynamic_cast<CMarker*>(pFile->GetMarkedObject(m_oMarkers.m_oMid.ToWString()));

			if (NULL != pMidMarker)
			{
				TMarkerExternData oExternalData;
				oExternalData.m_dStroke = dStrokeWidth;
				oExternalData.m_pPoints = new std::vector<TPointData>(m_arElements.size() - 2);

				for (unsigned int unIndex = 1; unIndex < m_arElements.size() - 1; ++unIndex)
				{
					if (EPathElement::Close != m_arElements[unIndex]->GetType())
						(*oExternalData.m_pPoints)[unIndex - 1].m_oPoint = (*m_arElements[unIndex])[-1];

					if (pMidMarker->NeedExternalAngle())
					{
						const Point oCurrent{(*m_arElements[unIndex])[0]};
						const Point oPrev{(*m_arElements[unIndex + 1])[-1]};

						(*oExternalData.m_pPoints)[unIndex - 1].m_dAngle = CALCULATE_ANGLE(oCurrent, oPrev);
					}
				}

				pMidMarker->Draw(pRenderer, pFile, oExternalData, oMode, pOtherStyles, this);
			}
		}

		if (!m_oMarkers.m_oEnd.Empty() && NSCSS::NSProperties::ColorType::ColorUrl == m_oMarkers.m_oEnd.GetType())
		{
			CMarker *pEndMarker = dynamic_cast<CMarker*>(pFile->GetMarkedObject(m_oMarkers.m_oEnd.ToWString()));

			if (NULL != pEndMarker)
			{
				TMarkerExternData oExternalData;
				oExternalData.m_dStroke = dStrokeWidth;
				oExternalData.m_pPoints = new std::vector<TPointData>(1);

				int unIndex = FindIndexFirstNotEmpty(true);

				if (0 <= unIndex)
				{
					const IPathElement* pLastElement{m_arElements[unIndex]};

					(*oExternalData.m_pPoints)[0].m_oPoint = (*pLastElement)[-1];

					if (pEndMarker->NeedExternalAngle())
					{
						Point oCurent{(*pLastElement)[-1]};
						Point oPrev;

						if (pLastElement->GetPointCount() > 1)
							oPrev = (*pLastElement)[-2];
						else if (unIndex > 0 && EPathElement::Close != m_arElements[unIndex - 1]->GetType() && EPathElement::Move != m_arElements[unIndex - 1]->GetType())
							oPrev = (*m_arElements[unIndex - 1])[0];

						(*oExternalData.m_pPoints)[0].m_dAngle = CALCULATE_ANGLE(oPrev, oCurent);
					}

					pEndMarker->Draw(pRenderer, pFile, oExternalData, oMode, pOtherStyles, this);
				}
			}
		}

		EndPath(pRenderer, pFile, oOldMatrix, oMode, pOtherStyles, pContexObject);

		return true;
	}

	void CPath::SetMarker(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"marker-start"))
			m_oMarkers.m_oStart.SetValue(mAttributes.at(L"marker-start"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"marker-mid"))
			m_oMarkers.m_oMid.SetValue(mAttributes.at(L"marker-mid"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"marker-end"))
			m_oMarkers.m_oEnd.SetValue(mAttributes.at(L"marker-end"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"marker"))
		{
			m_oMarkers.m_oStart.SetValue(mAttributes.at(L"marker"), ushLevel, bHardMode);
			m_oMarkers.m_oMid  .SetValue(mAttributes.at(L"marker"), ushLevel, bHardMode);
			m_oMarkers.m_oEnd  .SetValue(mAttributes.at(L"marker"), ushLevel, bHardMode);
		}
	}

	TBounds CPath::GetBounds() const
	{
		TBounds oBounds{DBL_MAX, DBL_MAX, -DBL_MAX, -DBL_MAX}, oTempBounds;

		for (const IPathElement* oElement : m_arElements)
		{
			oTempBounds = oElement->GetBounds();

			oBounds.m_dLeft   = std::min(oBounds.m_dLeft,   oTempBounds.m_dLeft);
			oBounds.m_dTop    = std::min(oBounds.m_dTop,    oTempBounds.m_dTop);
			oBounds.m_dRight  = std::max(oBounds.m_dRight,  oTempBounds.m_dRight);
			oBounds.m_dBottom = std::max(oBounds.m_dBottom, oTempBounds.m_dBottom);
		}

		return oBounds;
	}

	const int CPath::FindIndexFirstNotEmpty(bool bReverseSearch) const
	{
		if (!bReverseSearch)
		{
			std::vector<IPathElement*>::const_iterator itFound = std::find_if(m_arElements.cbegin(), m_arElements.cend(), [](const IPathElement* pElement){ return EPathElement::Close != pElement->GetType(); });
			if (m_arElements.cend() != itFound)
				return itFound - m_arElements.cbegin();
		}
		else
		{
			std::vector<IPathElement*>::const_reverse_iterator itFound = std::find_if(m_arElements.crbegin(), m_arElements.crend(), [](const IPathElement* pElement){ return EPathElement::Close != pElement->GetType(); });
			if (m_arElements.crend() != itFound)
				return itFound.base() - m_arElements.cbegin() - 1;
		}

		return -1;
	}

	void CPath::ReadFromString(const std::wstring &wsValue)
	{
		CMoveElement *pMoveElement = NULL;

		std::wstring::const_iterator oFirstPos  = std::find_if(wsValue.begin(), wsValue.end(), [](wchar_t wChar){return ISPATHCOMMAND(wChar);});
		std::wstring::const_iterator oSecondPos = oFirstPos;

		while (oFirstPos != wsValue.end())
		{
			if (L'z' == *oFirstPos || L'Z' == *oFirstPos)
			{
				if (NULL == pMoveElement)
					return;

				AddElement(new CCloseElement);
				oSecondPos = ++oFirstPos;
				continue;
			}

			oSecondPos = std::find_if(oFirstPos + 1, wsValue.end(), [](wchar_t wChar){return ISPATHCOMMAND(wChar);});

			std::vector<double> arValues = StrUtils::ReadDoubleValues(oFirstPos, oSecondPos);

			switch(*oFirstPos)
			{
				case L'M':
				case L'm':
				{
					IPathElement *pLastElement =  LASTELEMENT(m_arElements);

					if (NULL != pLastElement && Close == pLastElement->GetType())
						pLastElement = pMoveElement;

					pMoveElement = CMoveElement::CreateFromArray(arValues, iswlower(*oFirstPos), pLastElement);

					if (AddElement(pMoveElement) && arValues.size() > 1)
						while(AddElement(CLineElement::CreateFromArray(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements))));

					break;
				}
				case L'L':
				case L'l':
				{
					if (NULL == pMoveElement)
						return;

					while(AddElement(CLineElement::CreateFromArray(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements))));
					break;
				}
				case L'H':
				case L'h':
				{
					if (NULL == pMoveElement)
						return;

					while(AddElement(CLineElement::CreateFromHArray(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements))));
					break;
				}
				case L'V':
				case L'v':
				{
					if (NULL == pMoveElement)
						return;

					while(AddElement(CLineElement::CreateFromVArray(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements))));
					break;
				}
				case L'C':
				case L'c':
				{
					if (NULL == pMoveElement)
						return;

					while(AddElement(CCBezierElement::CreateFromArray(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements))));
					break;
				}
				case L'S':
				case L's':
				{
					if (NULL == pMoveElement)
						return;

					while(AddElement(CCBezierElement::CreateFromSArray(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements))));
					break;
				}
				case L'Q':
				case L'q':
				{
					if (NULL == pMoveElement)
						return;

					while(AddElement(CCBezierElement::CreateFromQArray(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements))));
					break;
				}
				case L'T':
				case L't':
				{
					if (NULL == pMoveElement)
						return;

					while(AddElement(CCBezierElement::CreateFromTArray(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements))));
					break;
				}
				case L'A':
				case L'a':
				{
					if (NULL == pMoveElement)
						return;

					std::vector<IPathElement*> arCurves;

					while(true)
					{
						arCurves = CCBezierElement::CreateFromArc(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements));

						if (arCurves.empty())
							break;

						for (IPathElement* pCurve : arCurves)
							AddElement(pCurve);
					}

					break;
				}
			}
			oFirstPos = oSecondPos;
		}
	}

	bool CPath::AddElement(IPathElement *pElement)
	{
		if (NULL == pElement)
			return false;

		m_arElements.push_back(pElement);
		return true;
	}

	CMovingPath::CMovingPath(const CPath *pPath)
	    : m_pPath(pPath), m_oPosition{0., 0.}
	{
		ToStart();
	}

	bool CMovingPath::Move(double dX)
	{
		if (NULL == m_pCurrentElement)
			return false;

		switch (m_pCurrentElement->GetType())
		{
		case EPathElement::Move:
		case  EPathElement::Close:
		{
			m_oPosition = m_oLastPoint = (*m_pCurrentElement)[0];
			m_pCurrentElement = (*m_pPath)[m_unIndexElement++];
			return Move(dX);
		}
		case EPathElement::Line:
		{
			Point oPoint{(*m_pCurrentElement)[0]};

			double dDx = oPoint.dX - m_oPosition.dX;
			double dDy = oPoint.dY - m_oPosition.dY;

			double dLineLength = std::sqrt(std::pow(dDx, 2) + std::pow(dDy, 2));
			m_dAngle           = std::atan2(dDy, dDx);

			if (dLineLength >= dX)
			{
				m_oLastPoint = m_oPosition;
				m_oPosition += {std::cos(m_dAngle) * dX, std::sin(m_dAngle) * dX};
				m_dAngle *= 180. / M_PI;
				return true;
			}
			else
			{
				m_pCurrentElement = (*m_pPath)[m_unIndexElement++];
				m_oPosition = m_oLastPoint = oPoint;
				return Move(dX - dLineLength);
			}
		}
		case EPathElement::CBezier:
		case EPathElement::SBezier:
		case EPathElement::QBezier:
		case EPathElement::TBezier:
		{
			Point oCurvePoint{0., 0.};
			double dPrevValue = dX;

			while((dX < 0 && m_dCurveIndex > 0) || (dX > 0 && m_dCurveIndex < 1))
			{
				if (MINCURVESTEP > m_dCurveStep)
				{
					m_dCurveStep  = CURVESTEP;
					return true;
				}

				if ((0. < dPrevValue && 0. > dX) || (0. > dPrevValue && 0. < dX))
					m_dCurveStep /= 2.;

				if (dX > 0.)
					m_dCurveIndex += m_dCurveStep;
				else if (dX < 0. && m_dCurveIndex > 0.)
					m_dCurveIndex -= m_dCurveStep;
				else
					return false;

				RANGEALIGMENT(m_dCurveIndex, 0., 1.);

				oCurvePoint.dX = std::pow((1. - m_dCurveIndex), 3) * m_oLastPoint.dX + 3 * std::pow((1. - m_dCurveIndex), 2) * m_dCurveIndex * (*m_pCurrentElement)[0].dX + 3 * (1. - m_dCurveIndex)* std::pow(m_dCurveIndex, 2) * (*m_pCurrentElement)[1].dX + std::pow(m_dCurveIndex, 3) * (*m_pCurrentElement)[2].dX;
				oCurvePoint.dY = std::pow((1. - m_dCurveIndex), 3) * m_oLastPoint.dY + 3 * std::pow((1. - m_dCurveIndex), 2) * m_dCurveIndex * (*m_pCurrentElement)[0].dY + 3 * (1. - m_dCurveIndex)* std::pow(m_dCurveIndex, 2) * (*m_pCurrentElement)[1].dY + std::pow(m_dCurveIndex, 3) * (*m_pCurrentElement)[2].dY;

				dPrevValue = dX;

				UpdatePosition(oCurvePoint, dX);
			}

			m_dCurveStep  = CURVESTEP;

			return NextMove(dX, (*m_pCurrentElement)[-1]);
		}
		default: return false;
		}

		return false;
	}

	void CMovingPath::ToStart()
	{
		if (NULL == m_pPath)
			return;

		m_unIndexElement = 0;
		m_pCurrentElement = (*m_pPath)[m_unIndexElement++];
		m_oPosition = m_oLastPoint = (*m_pCurrentElement)[0];
		m_dAngle = m_dCurveIndex = 0.;
		m_dCurveStep = CURVESTEP;
	}

	Point CMovingPath::GetPosition() const
	{
		return m_oPosition;
	}

	double CMovingPath::GetAngle() const
	{
		return m_dAngle;
	}

	void CMovingPath::UpdatePosition(const Point &oPoint, double &dX)
	{
		double dDx = oPoint.dX - m_oPosition.dX;
		double dDy = oPoint.dY - m_oPosition.dY;

		dX -= std::sqrt(std::pow(dDx, 2) + std::pow(dDy, 2)) * ((dX < 0) ? (-1) : 1);
		m_dAngle = std::atan2(dDy, dDx) * 180. / M_PI;

		m_oPosition = oPoint;
	}

	bool CMovingPath::NextMove(double dX, const Point &oPoint)
	{
		if (std::abs(dX) < EPSILON)
		{
			m_dCurveStep  = CURVESTEP;
			return true;
		}

		m_dCurveIndex = 0.;
		m_oPosition = m_oLastPoint = oPoint;
		m_pCurrentElement = (*m_pPath)[m_unIndexElement++];
		return Move(dX);
	}
}
