#include "CPath.h"

#include <algorithm>
#include "CMarker.h"

namespace SVG
{
	// IpathElement
	TBounds IPathElement::GetBounds() const
	{
		TBounds oBounds{0., 0., 0., 0.};

		for (const Point& oPoint : m_arPoints)
		{
			oBounds.m_dLeft   = std::min(oBounds.m_dLeft,   oPoint.dX);
			oBounds.m_dTop    = std::min(oBounds.m_dTop,    oPoint.dY);
			oBounds.m_dRight  = std::max(oBounds.m_dRight,  oPoint.dX);
			oBounds.m_dBottom = std::max(oBounds.m_dBottom, oPoint.dY);
		}

		return oBounds;
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

	void CLineElement::Draw(IRenderer *pRenderer) const
	{
		if (m_arPoints.empty())
			return;

		pRenderer->PathCommandLineTo(m_arPoints[0].dX, m_arPoints[0].dY);
	}

	// CVLineElement
	CVLineElement::CVLineElement(const Point &oPoint)
		: CLineElement(oPoint)
	{}

	EPathElement CVLineElement::GetType() const
	{
		return EPathElement::VLine;
	}

	CVLineElement *CVLineElement::CreateFromArray(std::vector<double> &arValues, bool bRelativeCoordinate, IPathElement *pPrevElement)
	{
		if (arValues.empty())
			return NULL;

		Point oTranslatePoint{0., 0.};

		if (bRelativeCoordinate && NULL != pPrevElement)
			oTranslatePoint = (*pPrevElement)[-1];

		CVLineElement *pVLineElement = new CVLineElement(Point{0, arValues[0]} + oTranslatePoint);

		arValues.erase(arValues.begin());

		return pVLineElement;
	}

	//CHLineElement
	CHLineElement::CHLineElement(const Point &oPoint)
		: CLineElement(oPoint)
	{}

	EPathElement CHLineElement::GetType() const
	{
		return EPathElement::HLine;
	}

	CHLineElement *CHLineElement::CreateFromArray(std::vector<double> &arValues, bool bRelativeCoordinate, IPathElement *pPrevElement)
	{
		if (arValues.empty())
			return NULL;

		Point oTranslatePoint{0., 0.};

		if (bRelativeCoordinate && NULL != pPrevElement)
			oTranslatePoint = (*pPrevElement)[-1];

		CHLineElement *pHLineElement = new CHLineElement(Point{arValues[0], 0} + oTranslatePoint);

		arValues.erase(arValues.begin());

		return pHLineElement;
	}

	//CCurveBezierElement
	CCBezierElement::CCBezierElement(const Point &oPoint1, const Point &oPoint2, const Point &oPointE)
	{
		m_arPoints.push_back(oPoint1);
		m_arPoints.push_back(oPoint2);
		m_arPoints.push_back(oPointE);
	}

	EPathElement CCBezierElement::GetType() const
	{
		return EPathElement::CBezier;
	}

	CCBezierElement *CCBezierElement::CreateFromArray(std::vector<double> &arValues, bool bRelativeCoordinate, IPathElement *pPrevElement)
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

	void CCBezierElement::Draw(IRenderer *pRenderer) const
	{
		if (3 != m_arPoints.size())
				return;

		pRenderer->PathCommandCurveTo(m_arPoints[0].dX, m_arPoints[0].dY,
									  m_arPoints[1].dX, m_arPoints[1].dY,
									  m_arPoints[2].dX, m_arPoints[2].dY);
	}

	//CSBezierElement
	CSBezierElement::CSBezierElement(const Point &oPoint1, const Point &oPoint2, const Point &oPointE)
		: CCBezierElement(oPoint1, oPoint2, oPointE)
	{}

	EPathElement CSBezierElement::GetType() const
	{
		return EPathElement::SBezier;
	}

	CSBezierElement *CSBezierElement::CreateFromArray(std::vector<double> &arValues, bool bRelativeCoordinate, IPathElement *pPrevElement)
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

		CSBezierElement *pSBezierElement = new CSBezierElement(oFirstPoint + oTranslatePoint,
															   Point{arValues[0], arValues[1]} + oTranslatePoint,
															   Point{arValues[2], arValues[3]} + oTranslatePoint);

		arValues.erase(arValues.begin(), arValues.begin() + 4);

		return pSBezierElement;
	}

	//CQBezierElement
	CQBezierElement::CQBezierElement(const Point &oPoint1, const Point &oPoint2, const Point &oPointE)
		: CCBezierElement(oPoint1, oPoint2, oPointE)
	{}

	EPathElement CQBezierElement::GetType() const
	{
		return EPathElement::QBezier;
	}

	CQBezierElement *CQBezierElement::CreateFromArray(std::vector<double> &arValues, bool bRelativeCoordinate, IPathElement *pPrevElement)
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

		CQBezierElement *pQBezierElement = new CQBezierElement(oLastPoint, Point{arValues[0], arValues[1]} + oTranslatePoint, Point{arValues[2], arValues[3]} + oTranslatePoint);

		arValues.erase(arValues.begin(), arValues.begin() + 4);

		return pQBezierElement;
	}

	//CTBezierElement
	CTBezierElement::CTBezierElement(const Point &oPoint1, const Point &oPoint2, const Point &oPointE)
		: CCBezierElement(oPoint1, oPoint2, oPointE)
	{}

	EPathElement CTBezierElement::GetType() const
	{
		return EPathElement::TBezier;
	}

	IPathElement *CTBezierElement::CreateFromArray(std::vector<double> &arValues, bool bRelativeCoordinate, IPathElement *pPrevElement)
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

			return new CLineElement(oSecondPoint + oTranslatePoint);
		}

		oSecondPoint = oFirstPoint + (oFirstPoint - oSecondPoint);

		CTBezierElement *pTBezierElement = new CTBezierElement(oFirstPoint + oTranslatePoint, oSecondPoint + oTranslatePoint, Point{arValues[0], arValues[1]} + oTranslatePoint);

		arValues.erase(arValues.begin(), arValues.begin() + 2);

		return pTBezierElement;
	}

	//CArcElement
	CArcElement::CArcElement(const Point &oRadius, double dXRotation, bool LargeArcFlag, bool bSweepFlag, const Point &oStart, const Point &oEnd)
		: m_oRadius(oRadius), m_dXAxisRotation(dXRotation), m_bLargeArcFlag(LargeArcFlag), m_bSweepFlag(bSweepFlag)
	{
		m_arPoints.push_back(oStart);
		m_arPoints.push_back(oEnd);
	}

	EPathElement CArcElement::GetType() const
	{
		return EPathElement::Arc;
	}

	CArcElement *CArcElement::CreateFromArray(std::vector<double> &arValues, bool bRelativeCoordinate, IPathElement *pPrevElement)
	{
		if (arValues.size() < 7)
			return NULL;

		Point oTranslatePoint{0., 0.};

		if (bRelativeCoordinate && NULL != pPrevElement)
			oTranslatePoint = (*pPrevElement)[-1];

		CArcElement *pArcElement = new CArcElement(Point{arValues[0], arValues[1]}, arValues[2],
												  (1 == arValues[3]) ? true : false, (1 == arValues[4]) ? true : false,
												  (NULL != pPrevElement) ? (*pPrevElement)[-1] : Point{0., 0.}, Point{arValues[5], arValues[6]} + oTranslatePoint);

		arValues.erase(arValues.begin(), arValues.begin() + 7);

		return pArcElement;
	}

	void CArcElement::Draw(IRenderer *pRenderer) const
	{
		if (m_arPoints.size() != 2)
			return;

		Point oRadius{m_oRadius};
		Point oCenter{0, 0};
		double dAngle = 0, dSweep = 0;

		CalculateData(m_arPoints[0], m_arPoints[1], oRadius, oCenter, m_dXAxisRotation, m_bLargeArcFlag, m_bSweepFlag, dAngle, dSweep);

		double dStartAngle = dAngle;
		double dEndAngle;

		Point oEndPoint, oControl1, oControl2;

		pRenderer->PathCommandLineTo(m_arPoints[0].dX, m_arPoints[0].dY);

		while (dStartAngle != (dAngle + dSweep))
		{
			if ((int)(dStartAngle / 90.) == dStartAngle / 90.)
				dEndAngle = dStartAngle + ((dSweep > 0.) ? 90. : -90.);
			else
				dEndAngle = (int)(dStartAngle / 90.) * 90.;

			if (std::abs(dAngle - dEndAngle) > std::abs(dSweep))
				dEndAngle = dAngle + dSweep;

			oEndPoint = GetPoint(oRadius, dEndAngle);
			oEndPoint.Rotate(m_dXAxisRotation);
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

			oControl1.Rotate(m_dXAxisRotation);
			oControl2.Rotate(m_dXAxisRotation);

			oControl1 += oCenter;
			oControl2 += oCenter;

			pRenderer->PathCommandCurveTo(oControl1.dX, oControl1.dY, oControl2.dX, oControl2.dY, oEndPoint.dX, oEndPoint.dY);

			dStartAngle = dEndAngle;
		}
	}

	Point CArcElement::GetPoint(const Point &oRadius, double dAngle)
	{
		return Point{oRadius.dX * std::cos(dAngle * M_PI / 180.), oRadius.dY * std::sin(dAngle * M_PI / 180.)};
	}

	void CArcElement::CalculateData(const Point &oFirst, const Point &oSecond, Point &oRadius, Point &oCenter, double dAngle, bool bLargeArc, bool bSweep, double &dStartAngle, double &dSweep)
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

		dStartAngle = std::acos(((dXp - dCpx) / oRadius.dX) / std::sqrt(std::pow((dXp - dCpx) / oRadius.dX, 2) + std::pow((dYp - dCpy) / oRadius.dY, 2))) * 180. / M_PI;
		dSweep      = std::acos((((dXp - dCpx) / oRadius.dX * (-dXp - dCpx) / oRadius.dX) + ((dYp - dCpy) / oRadius.dY * (-dYp - dCpy) / oRadius.dY)) / (std::sqrt(std::pow((dXp - dCpx) / oRadius.dX, 2) + std::pow((dYp - dCpy) / oRadius.dY, 2)) * std::sqrt(std::pow((-dXp - dCpx) / oRadius.dX, 2) + std::pow((-dYp - dCpy) / oRadius.dY, 2)))) * 180. / M_PI;

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
    #define ARCSTEP 0.5
    #define MINARCSTEP 0.01

	CPath::CPath(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CSvgGraphicsObject(oNode, pParent)
	{}

	CPath::~CPath()
	{
		for (IPathElement* pPathElement : m_arElements)
			delete pPathElement;
	}

	void CPath::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"d"))
			ReadFromString(mAttributes.at(L"d"));

		SetTransform(mAttributes, ushLevel, bHardMode);
		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);
		SetClip(mAttributes, ushLevel, bHardMode);
		SetMarker(mAttributes, ushLevel, bHardMode);
		SetMask(mAttributes, ushLevel, bHardMode);
	}

	bool CPath::Draw(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip, const TSvgStyles *pOtherStyles) const
	{
		if (NULL == pRenderer || m_arElements.empty())
			return false;

		StartPath(pRenderer, pDefs, bIsClip);

		for (const IPathElement* oElement : m_arElements)
			oElement->Draw(pRenderer);

		EndPath(pRenderer, pDefs, bIsClip, pOtherStyles);

		DrawMarkers(pRenderer, pDefs);

		return true;
	}

	IPathElement *CPath::operator[](int nIndex) const
	{
		if (m_arElements.empty() || (nIndex > 0 && nIndex >= m_arElements.size()) || (nIndex < 0 && -nIndex > m_arElements.size()))
			return NULL;

		return m_arElements[(nIndex >= 0) ? nIndex : m_arElements.size() + nIndex];
	}

	void CPath::ApplyStyle(IRenderer *pRenderer, const TSvgStyles *pStyles, const CDefs *pDefs, int &nTypePath, Aggplus::CMatrix &oOldMatrix) const
	{
		Apply(pRenderer, &pStyles->m_oTransform, oOldMatrix);

		if (Apply(pRenderer, &pStyles->m_oStroke))
			nTypePath += c_nStroke;

		if (Apply(pRenderer, &pStyles->m_oFill, pDefs, true))
			nTypePath += c_nWindingFillMode;
	}

	bool CPath::DrawMarkers(IRenderer *pRenderer, const CDefs *pDefs) const
	{
		if (NULL == pRenderer || NULL == pDefs || m_arElements.empty() || m_oStyles.m_oStroke.m_oWidth.Zero())
			return false;

		double dStrokeWidth = (m_oStyles.m_oStroke.m_oWidth.Empty()) ? 1. : m_oStyles.m_oStroke.m_oWidth.ToDouble(NSCSS::Pixel);

		std::vector<Point> arPoints(m_arElements.size());

		for (unsigned int unIndex = 0; unIndex < m_arElements.size(); ++unIndex)
			arPoints[unIndex] = (*m_arElements[unIndex])[0];

		if (!m_oMarkers.m_oStart.Empty() && NSCSS::NSProperties::ColorType::ColorUrl == m_oMarkers.m_oStart.GetType())
		{
			CMarker *pStartMarker = dynamic_cast<CMarker*>(pDefs->GetDef(m_oMarkers.m_oStart.ToWString()));

			if (NULL != pStartMarker)
			{
				pStartMarker->Update(pDefs);
				pStartMarker->Draw(pRenderer, {*arPoints.begin()}, dStrokeWidth);
			}
		}

		if (!m_oMarkers.m_oMid.Empty() && NSCSS::NSProperties::ColorType::ColorUrl == m_oMarkers.m_oMid.GetType())
		{
			CMarker *pMidMarker = dynamic_cast<CMarker*>(pDefs->GetDef(m_oMarkers.m_oMid.ToWString()));

			if (NULL != pMidMarker)
			{
				pMidMarker->Update(pDefs);
				pMidMarker->Draw(pRenderer, std::vector<Point>(arPoints.begin() + 1, arPoints.end() - 1), dStrokeWidth);
			}
		}

		if (!m_oMarkers.m_oEnd.Empty() && NSCSS::NSProperties::ColorType::ColorUrl == m_oMarkers.m_oEnd.GetType())
		{
			CMarker *pEndMarker = dynamic_cast<CMarker*>(pDefs->GetDef(m_oMarkers.m_oEnd.ToWString()));

			if (NULL != pEndMarker)
			{
				pEndMarker->Update(pDefs);
				pEndMarker->Draw(pRenderer, {*(arPoints.end() - 1)}, dStrokeWidth);
			}
		}

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
	}

	TBounds CPath::GetBounds() const
	{
		TBounds oBounds{0., 0., 0., 0.}, oTempBounds;

		for (const IPathElement* oElement : m_arElements)
		{
			oTempBounds = oElement->GetBounds();

			oBounds.m_dLeft   = std::min(oBounds.m_dLeft, oTempBounds.m_dLeft);
			oBounds.m_dTop    = std::min(oBounds.m_dTop, oTempBounds.m_dTop);
			oBounds.m_dRight  = std::max(oBounds.m_dRight, oTempBounds.m_dRight);
			oBounds.m_dBottom = std::max(oBounds.m_dBottom, oTempBounds.m_dBottom);
		}

		return oBounds;
	}

	void CPath::ReadFromString(const std::wstring &wsValue)
	{
		std::wstring::const_iterator oFirstPos = wsValue.begin();
		std::wstring::const_iterator oSecondPos = oFirstPos;

		IPathElement *pMoveElement = NULL;

		while (true)
		{
			oFirstPos = std::find_if(oSecondPos, wsValue.end(), iswalpha );

			if (wsValue.end() == oFirstPos)
				break;

			oSecondPos = std::find_if(oFirstPos + 1, wsValue.end(), iswalpha );

			std::vector<double> arValues = StrUtils::ReadDoubleValues(std::wstring(oFirstPos + 1, oSecondPos));

			switch(*oFirstPos)
			{
				case L'M':
				case L'm':
				{
					pMoveElement = CMoveElement::CreateFromArray(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements));

					if (AddElement(pMoveElement) && arValues.size() > 1)
						AddElements<CLineElement>(arValues, iswlower(*oFirstPos));

					break;
				}
				case L'L':
				case L'l':
				{
					if (NULL == pMoveElement)
						return;

					AddElements<CLineElement>(arValues, iswlower(*oFirstPos));
					break;
				}
				case L'H':
				case L'h':
				{
					if (NULL == pMoveElement)
						return;

					AddElements<CHLineElement>(arValues, iswlower(*oFirstPos));
					break;
				}
				case L'V':
				case L'v':
				{
					if (NULL == pMoveElement)
						return;

					AddElements<CVLineElement>(arValues, iswlower(*oFirstPos));
					break;
				}
				case L'C':
				case L'c':
				{
					if (NULL == pMoveElement)
						return;

					AddElements<CCBezierElement>(arValues, iswlower(*oFirstPos));
					break;
				}
				case L'S':
				case L's':
				{
					if (NULL == pMoveElement)
						return;

					AddElements<CSBezierElement>(arValues, iswlower(*oFirstPos));
					break;
				}
				case L'Q':
				case L'q':
				{
					if (NULL == pMoveElement)
						return;

					AddElements<CQBezierElement>(arValues, iswlower(*oFirstPos));
					break;
				}
				case L'T':
				case L't':
				{
					if (NULL == pMoveElement)
						return;

					AddElements<CTBezierElement>(arValues, iswlower(*oFirstPos));
					break;
				}
				case L'A':
				case L'a':
				{
					if (NULL == pMoveElement)
						return;

					AddElements<CArcElement>(arValues, iswlower(*oFirstPos));
					break;
				}
				case L'Z':
				case L'z':
				{
					if (NULL == pMoveElement)
						return;

					AddElement(new CCloseElement);
					pMoveElement = NULL;

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

	template<typename TypeElement>
	void CPath::AddElements(std::vector<double> &arValues, bool bRelativeCoordinate)
	{
		while(AddElement(TypeElement::CreateFromArray(arValues, bRelativeCoordinate, LASTELEMENT(m_arElements))));
	}

	CMovingPath::CMovingPath(const CPath *pPath)
	    : m_pPath(pPath), m_oPosition{DBL_MIN, DBL_MIN}
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
		case EPathElement::VLine:
		case EPathElement::HLine:
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
		case EPathElement::Arc:
		{
			CArcElement *pArcElement = (CArcElement*)m_pCurrentElement;
			Point oRadius{pArcElement->m_oRadius};

			if (0. == m_dStartAngle && m_dStartAngle == m_dEndAngle)
			{
				Point oCenter{0, 0};

				double dAngle, dSweep;

				pArcElement->CalculateData(pArcElement->m_arPoints[0], pArcElement->m_arPoints[1], oRadius, oCenter, 0, pArcElement->m_bLargeArcFlag, pArcElement->m_bSweepFlag, dAngle, dSweep);

				m_oLastPoint  = oCenter;
				m_oPosition   = oCenter + pArcElement->GetPoint(oRadius, dAngle);
				m_dStartAngle = dAngle;
				m_dEndAngle   = dAngle + dSweep;
				m_dArcStep = ARCSTEP * ((m_dStartAngle > m_dEndAngle) ? -1 : 1);
			}

			double dPrevValue = dX;

			while(std::abs(m_dEndAngle - m_dStartAngle) > ARCSTEP)
			{
				if (MINARCSTEP > std::abs(m_dArcStep))
				{
					m_dArcStep = ARCSTEP * ((m_dStartAngle > m_dEndAngle) ? -1 : 1);
					return true;
				}

				if (((0. < dPrevValue && 0. > dX) || (0. > dPrevValue && 0. < dX)))
					m_dArcStep /= 2.;

				if (dX > 0.)
					m_dStartAngle += m_dArcStep;
				else if (dX < 0)
					m_dStartAngle -= m_dArcStep;
				else
					return true;

				dPrevValue = dX;

				UpdatePosition(m_oLastPoint + CArcElement::GetPoint(oRadius, m_dStartAngle), dX);

				double dMainAngle = std::fmod(std::atan2(m_oLastPoint.dY - m_oPosition.dY, m_oLastPoint.dX - m_oPosition.dX) * 180. / M_PI, 360);

				int nFirstQuarter  = (int)(dMainAngle / 90) + ((dMainAngle > 0) ? 1 : -1);
				int nSecondQuarter = (int)(m_dAngle   / 90) + ((m_dAngle > 0)   ? 1 : -1) - 1;

				if ((nFirstQuarter * nSecondQuarter < 0 && std::abs(nSecondQuarter - nFirstQuarter) == 3) ||
					(nFirstQuarter * nSecondQuarter > 0 && std::abs(nSecondQuarter - nFirstQuarter) == 2))
					m_dAngle += 180;
			}

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
		m_dAngle = m_dCurveIndex = m_dStartAngle = m_dEndAngle = 0.;
		m_dCurveStep = CURVESTEP;
		m_dArcStep = ARCSTEP;
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

		m_dCurveIndex = m_dStartAngle = m_dEndAngle = 0.;
		m_oPosition = m_oLastPoint = oPoint;
		m_pCurrentElement = (*m_pPath)[m_unIndexElement++];
		return Move(dX);
	}
}
