#include "CPath.h"

#include <algorithm>

namespace SVG
{
    #define LASTELEMENT(array) (array.empty()) ? NULL : array.back()
    #define RANGEALIGMENT(value, left, rigth) if (value < left) value = left; else if (value > rigth) value = rigth;
    #define EPSILON 0.05
    #define CURVESTEP 0.05
    #define MINCURVESTEP 0.001
    #define ARCSTEP 0.5
    #define MINARCSTEP 0.01

	CPath::CPath(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CSvgGraphicsObject(oNode, pParent)
	{
		ReadFromString(oNode.GetAttribute(L"d"));
	}

	CPath::~CPath()
	{
		for (IPathElement* pPathElement : m_arElements)
			delete pPathElement;
	}

	void CPath::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetTransform(mAttributes, ushLevel, bHardMode);
		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);
		SetClip(mAttributes, ushLevel, bHardMode);
	}

	bool CPath::Draw(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip) const
	{
		if (NULL == pRenderer || m_arElements.empty())
			return false;

		StartPath(pRenderer, pDefs, bIsClip);

		for (const IPathElement* oElement : m_arElements)
			oElement->Draw(pRenderer);

		EndPath(pRenderer, pDefs, bIsClip);

		return true;
	}

	CPath *CPath::Copy() const
	{
		CPath* pNew = new CPath(*this);
		pNew->m_arElements.clear();

		for(const IPathElement* oElement : m_arElements)
			pNew->m_arElements.push_back(oElement->Copy());

		return pNew;
	}

	IPathElement *CPath::operator[](int nIndex) const
	{
		if (m_arElements.empty() || (nIndex > 0 && nIndex >= m_arElements.size()) || (nIndex < 0 && -nIndex > m_arElements.size()))
			return NULL;

		return m_arElements[(nIndex >= 0) ? nIndex : m_arElements.size() + nIndex];
	}

	void CPath::ApplyStyle(IRenderer *pRenderer, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer, oOldMatrix);
		ApplyStroke(pRenderer, nTypePath);
		ApplyFill(pRenderer, pDefs, nTypePath, true);
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
			if (0. == m_dStartAngle && m_dStartAngle == m_dEndAngle)
			{
				Point Center  = pArcElement->GetCenter (pArcElement->m_bLargeArcFlag, pArcElement->m_bSweepFlag, pArcElement->m_oRadius, (*pArcElement)[0], (*pArcElement)[1]);

				m_dStartAngle = pArcElement->GetAngle ( Center.dX, Center.dY, (*pArcElement)[0].dX, (*pArcElement)[0].dY);
				m_dEndAngle   = pArcElement->GetAngle ( Center.dX, Center.dY, (*pArcElement)[1].dX, (*pArcElement)[1].dY);

				double dSweep = 0.;

				if (!pArcElement->GetArcAngles(pArcElement->m_bLargeArcFlag, pArcElement->m_bSweepFlag, m_dStartAngle, m_dEndAngle, dSweep))
					return NextMove(dX, (*m_pCurrentElement)[-1]);

				m_dEndAngle = m_dStartAngle + dSweep;

				m_oPosition = Center + pArcElement->GetPoint(m_dStartAngle);

				if (m_dStartAngle > m_dEndAngle)
					m_dArcStep *= -1;

				m_oLastPoint = Center;
			}

			// TODO:: На самом деле точки вычисляются с небольшим смещением (по углу)
			// поэтому необходимо разобраться в этом
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
					return false;

				dPrevValue = dX;

				UpdatePosition(m_oLastPoint + pArcElement->GetPoint(m_dStartAngle), dX);
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
