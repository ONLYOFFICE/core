#ifndef CPATH_H
#define CPATH_H

#include "CObjectBase.h"
#include "../SvgTypes.h"
#include "../SvgUtils.h"

namespace SVG
{
	//	M(m)	-	move to
	//	Z(z)	-	close path
	//	L(l)	-	line to
	//	V(v)	-	vertical line
	//	H(h)	-	horizontal line
	//	C(c)	-	curve to			(x1 y1 x2 y2 x y)
	//  S(x)    -   shortened bezier    (x2 y2 x y)
	//	Q(q)	-	quadratic bezier	(x1 y1 x y)

	enum EPathElement
	{
		Move,
		Line,
		VLine,
		HLine,
		CBezier,
		SBezier,
		QBezier,
		TBezier,
		Arc,
		Close,
	};

	class IPathElement
	{
	public:
		virtual EPathElement GetType() const = 0;
		virtual void Draw(IRenderer* pRenderer) const = 0;
		virtual IPathElement* Copy() const = 0;

		TBounds GetBounds() const
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

		virtual Point operator[](int nIndex) const
		{
			if (m_arPoints.empty() || (nIndex > 0 && nIndex >= m_arPoints.size()) || (nIndex < 0 && -nIndex > m_arPoints.size()))
				return Point{0., 0.};

			return m_arPoints[(nIndex >= 0) ? nIndex : m_arPoints.size() + nIndex];
		};

	private:
		friend class CMoveElement;
		friend class CLineElement;
		friend class CVLineElement;
		friend class CHLineElement;
		friend class CCBezierElement;
		friend class CSBezierElement;
		friend class CQBezierElement;
		friend class CTBezierElement;
		friend class CArcElement;
		friend class CCloseElement;
		friend class CMovingPath;

		std::vector<Point> m_arPoints;
	};

	class CMoveElement : public IPathElement
	{
	public:
		CMoveElement(const Point& oPoint)
		{
			m_arPoints.push_back(oPoint);
		}

		EPathElement GetType() const override
		{
			return EPathElement::Move;
		}

		static CMoveElement* CreateFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL)
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

		void Draw(IRenderer* pRenderer) const override
		{
			if (m_arPoints.empty())
				return;

			pRenderer->PathCommandMoveTo(m_arPoints[0].dX, m_arPoints[0].dY);
		}

		CMoveElement* Copy() const override
		{
			return new CMoveElement(*this);
		}
	};

	class CLineElement : public IPathElement
	{
	public:
		CLineElement(const Point& oPoint)
		{
			m_arPoints.push_back(oPoint);
		}

		EPathElement GetType() const override
		{
			return EPathElement::Line;
		}

		static CLineElement* CreateFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL)
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

		void Draw(IRenderer* pRenderer) const override
		{
			if (m_arPoints.empty())
				return;

			pRenderer->PathCommandLineTo(m_arPoints[0].dX, m_arPoints[0].dY);
		}

		CLineElement* Copy() const override
		{
			return new CLineElement(*this);
		}
	};

	class CVLineElement : public CLineElement
	{
	public:
		CVLineElement(const Point& oPoint) : CLineElement(oPoint){}

		EPathElement GetType() const override
		{
			return EPathElement::VLine;
		}

		static CVLineElement* CreateFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL)
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

		CVLineElement* Copy() const override
		{
			return new CVLineElement(*this);
		}
	};

	class CHLineElement : public CLineElement
	{
	public:
		CHLineElement(const Point& oPoint) : CLineElement(oPoint){}

		EPathElement GetType() const override
		{
			return EPathElement::HLine;
		}

		static CHLineElement* CreateFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL)
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

		CHLineElement* Copy() const override
		{
			return new CHLineElement(*this);
		}
	};

	class CCBezierElement : public IPathElement
	{
	public:
		CCBezierElement(const Point& oPoint1, const Point& oPoint2, const Point& oPointE)
		{
			m_arPoints.push_back(oPoint1);
			m_arPoints.push_back(oPoint2);
			m_arPoints.push_back(oPointE);
		}

		CCBezierElement& operator+=(const CCBezierElement& oCBezierElement)
		{
			m_arPoints.insert(m_arPoints.end(), oCBezierElement.m_arPoints.begin(), oCBezierElement.m_arPoints.end());

			return *this;
		}

		EPathElement GetType() const override
		{
			return EPathElement::CBezier;
		}

		static CCBezierElement* CreateFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL)
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

		void Draw(IRenderer* pRenderer) const override
		{
			if (3 != m_arPoints.size())
				return;

			pRenderer->PathCommandCurveTo(m_arPoints[0].dX, m_arPoints[0].dY,
			                              m_arPoints[1].dX, m_arPoints[1].dY,
			                              m_arPoints[2].dX, m_arPoints[2].dY);
		}

		CCBezierElement* Copy() const override
		{
			return new CCBezierElement(*this);
		}

		void Rotate(double dAngle, Point oCenter)
		{
		}
	};

	class CSBezierElement : public CCBezierElement
	{
	public:
		CSBezierElement(const Point& oPoint1, const Point& oPoint2, const Point& oPointE)
		    : CCBezierElement(oPoint1, oPoint2, oPointE){}

		EPathElement GetType() const override
		{
			return EPathElement::SBezier;
		}

		static CSBezierElement* CreateFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL)
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

		CSBezierElement* Copy() const override
		{
			return new CSBezierElement(*this);
		}
	};

	class CQBezierElement : public CCBezierElement
	{
	public:
		CQBezierElement(const Point& oPoint1, const Point& oPoint2, const Point& oPointE)
		    : CCBezierElement(oPoint1, oPoint2, oPointE){}

		EPathElement GetType() const override
		{
			return EPathElement::QBezier;
		}

		static CQBezierElement* CreateFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL)
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

		CQBezierElement* Copy() const override
		{
			return new CQBezierElement(*this);
		}
	};

	class CTBezierElement : public CCBezierElement
	{
	public:
		CTBezierElement(const Point& oPoint1, const Point& oPoint2, const Point& oPointE)
		    : CCBezierElement(oPoint1, oPoint2, oPointE){}

		EPathElement GetType() const override
		{
			return EPathElement::TBezier;
		}

		static IPathElement* CreateFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL)
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

		CTBezierElement* Copy() const override
		{
			return new CTBezierElement(*this);
		}
	};

	class CArcElement : public IPathElement
	{
	public:
		CArcElement(const Point& oRadius, double dXRotation, bool LargeArcFlag, bool bSweepFlag, const Point& oStart, const Point& oEnd)
		    : m_oRadius(oRadius), m_dXAxisRotation(dXRotation), m_bLargeArcFlag(LargeArcFlag), m_bSweepFlag(bSweepFlag)
		{
			m_arPoints.push_back(oStart);
			m_arPoints.push_back(oEnd);
		}

		EPathElement GetType() const override
		{
			return EPathElement::Arc;
		}

		static CArcElement* CreateFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL)
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

		void Draw(IRenderer* pRenderer) const override
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

		CArcElement* Copy() const override
		{
			return new CArcElement(*this);
		}
	private:
		friend class CMovingPath;

		static Point GetPoint(const Point& oRadius, double dAngle)
		{
			return Point{oRadius.dX * std::cos(dAngle * M_PI / 180.), oRadius.dY * std::sin(dAngle * M_PI / 180.)};
		}

		static void CalculateData(const Point& oFirst, const Point& oSecond, Point& oRadius, Point& oCenter, double dAngle, bool bLargeArc, bool bSweep, double& dStartAngle, double& dSweep)
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

		Point  m_oRadius;
		double m_dXAxisRotation;
		bool   m_bLargeArcFlag;
		bool   m_bSweepFlag;
	};

	class CCloseElement : public IPathElement
	{
	public:
		CCloseElement(){}

		EPathElement GetType() const override
		{
			return EPathElement::Close;
		}

		void Draw(IRenderer* pRenderer) const override
		{
			pRenderer->PathCommandClose();
		}

		CCloseElement* Copy() const override
		{
			return new CCloseElement(*this);
		}
	};

	class CPath : public CSvgGraphicsObject
	{
	public:
		CPath(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);
		virtual ~CPath();

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CDefs *pDefs, bool bIsClip = false, const TSvgStyles* pOtherStyles = NULL) const override;

		IPathElement* operator[](int nIndex) const;
	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		TBounds GetBounds() const override;

		void ReadFromString(const std::wstring& wsValue);
		bool AddElement(IPathElement* pElement);
		template <typename TypeElement>
		void AddElements(std::vector<double>& arValues, bool bRelativeCoordinate);

		std::vector<IPathElement*> m_arElements;
	};

	class CMovingPath
	{
	public:
		CMovingPath(const CPath* pPath);

		bool Move(double dX);
		void ToStart();
		Point GetPosition() const;
		double GetAngle() const;
	private:
		void UpdatePosition(const Point& oPoint, double& dX);
		bool NextMove(double dX, const Point& oPoint);

		const CPath  *m_pPath;
		Point         m_oPosition;
		Point         m_oLastPoint;
		double        m_dAngle;
		IPathElement *m_pCurrentElement;
		unsigned int  m_unIndexElement;

		// Необходимо при рабое с кривыми Безье
		double        m_dCurveIndex;
		double        m_dCurveStep;

		// Необходимо при работе с Дугой
		double        m_dStartAngle;
		double        m_dEndAngle;
		double        m_dArcStep;
	};
}

#endif // CPATH_H
