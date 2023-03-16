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
		IPathElement() {};
		virtual ~IPathElement() {};

		virtual EPathElement GetType() const = 0;
		virtual void Draw(IRenderer* pRenderer) const = 0;
		virtual IPathElement* Copy() const = 0;

		template<typename ElementType>
		static ElementType* CreateFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL)
		{
			ElementType* pElement = new ElementType;
			if (!pElement->ReadFromArray(arValues, bRelativeCoordinate, pPrevElement))
				RELEASEOBJECT(pElement);

			return pElement;
		}

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
			if (m_arPoints.empty())
				return Point{0., 0.};

			return m_arPoints[(nIndex >= 0) ? nIndex : m_arPoints.size() + nIndex];
		};

	private:
		virtual bool ReadFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) = 0;

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

		std::vector<Point> m_arPoints;
	};

	class CMoveElement : public IPathElement
	{
	public:
		CMoveElement(){}

		EPathElement GetType() const override
		{
			return EPathElement::Move;
		}

		bool ReadFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (arValues.size() < 2)
				return false;

			Point oTranslatePoint{0., 0.};

			if (bRelativeCoordinate && NULL != pPrevElement)
				oTranslatePoint = (*pPrevElement)[-1];

			m_arPoints.push_back(Point{arValues[0], arValues[1]} + oTranslatePoint);

			arValues.erase(arValues.begin(), arValues.begin() + 2);

			return true;
		}

		void Draw(IRenderer* pRenderer) const override
		{
			if (m_arPoints.empty())
				return;

			Point oPoint = (*this)[-1];

			pRenderer->PathCommandMoveTo(oPoint.dX, oPoint.dY);
		}

		CMoveElement* Copy() const override
		{
			return new CMoveElement(*this);
		}
	};

	class CLineElement : public IPathElement
	{
	public:
		CLineElement(){}

		EPathElement GetType() const override
		{
			return EPathElement::Line;
		}

		bool ReadFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (arValues.size() < 2)
				return false;

			Point oTranslatePoint{0., 0.};

			if (bRelativeCoordinate && NULL != pPrevElement)
				oTranslatePoint = (*pPrevElement)[-1];

			while (arValues.size() > 1)
			{
				m_arPoints.push_back(Point{arValues[0], arValues[1]} + oTranslatePoint);

				if (bRelativeCoordinate)
					oTranslatePoint = m_arPoints.back();

				arValues.erase(arValues.begin(), arValues.begin() + 2);
			}

			return true;
		}

		void Draw(IRenderer* pRenderer) const override
		{
			if (m_arPoints.empty())
				return;

			for (unsigned int unIndex = 0; unIndex < m_arPoints.size(); ++unIndex)
				pRenderer->PathCommandLineTo(m_arPoints[unIndex].dX, m_arPoints[unIndex].dY);
		}

		CLineElement* Copy() const override
		{
			return new CLineElement(*this);
		}
	};

	class CVLineElement : public CLineElement
	{
	public:
		CVLineElement(){}

		EPathElement GetType() const override
		{
			return EPathElement::VLine;
		}

		bool ReadFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (arValues.empty())
				return false;

			Point oTranslatePoint{0., 0.};

			if (bRelativeCoordinate && NULL != pPrevElement)
				oTranslatePoint = (*pPrevElement)[-1];

			while (!arValues.empty())
			{
				m_arPoints.push_back(Point{0, arValues[0]} + oTranslatePoint);

				if (bRelativeCoordinate)
					oTranslatePoint = m_arPoints.back();

				arValues.erase(arValues.begin());
			}

			return true;
		}

		CVLineElement* Copy() const override
		{
			return new CVLineElement(*this);
		}
	};

	class CHLineElement : public CLineElement
	{
	public:
		CHLineElement(){}

		EPathElement GetType() const override
		{
			return EPathElement::HLine;
		}

		bool ReadFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (arValues.empty())
				return false;

			Point oTranslatePoint{0., 0.};

			if (bRelativeCoordinate && NULL != pPrevElement)
				oTranslatePoint = (*pPrevElement)[-1];

			while (!arValues.empty())
			{
				m_arPoints.push_back(Point{arValues[0], 0} + oTranslatePoint);

				if (bRelativeCoordinate)
					oTranslatePoint = m_arPoints.back();

				arValues.erase(arValues.begin());
			}

			return true;
		}

		CHLineElement* Copy() const override
		{
			return new CHLineElement(*this);
		}
	};

	class CCBezierElement : public IPathElement
	{
	public:
		CCBezierElement(){}
		virtual ~CCBezierElement() {};

		CCBezierElement(double dX1, double dY1, double dX2, double dY2, double dXE, double dYE)
		{
			m_arPoints.push_back(Point{dX1, dY1});
			m_arPoints.push_back(Point{dX2, dY2});
			m_arPoints.push_back(Point{dXE, dYE});
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

		bool ReadFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (arValues.size() < 6)
				return false;

			Point oTranslatePoint{0., 0.};

			if (bRelativeCoordinate && NULL != pPrevElement)
				oTranslatePoint = (*pPrevElement)[-1];

			while (arValues.size() > 5)
			{
				m_arPoints.push_back(Point{arValues[0], arValues[1]} + oTranslatePoint);
				m_arPoints.push_back(Point{arValues[2], arValues[3]} + oTranslatePoint);
				m_arPoints.push_back(Point{arValues[4], arValues[5]} + oTranslatePoint);

				if (bRelativeCoordinate)
					oTranslatePoint = m_arPoints.back();

				arValues.erase(arValues.begin(), arValues.begin() + 6);
			}

			return true;
		}

		void Draw(IRenderer* pRenderer) const override
		{
			if (m_arPoints.empty() || 0 != (m_arPoints.size() % 3))
				return;

			for (unsigned int unIndex = 0; unIndex < m_arPoints.size(); unIndex+= 3)
				pRenderer->PathCommandCurveTo(m_arPoints[unIndex + 0].dX, m_arPoints[unIndex + 0].dY,
				                              m_arPoints[unIndex + 1].dX, m_arPoints[unIndex + 1].dY,
				                              m_arPoints[unIndex + 2].dX, m_arPoints[unIndex + 2].dY);
		}

		CCBezierElement* Copy() const override
		{
			return new CCBezierElement(*this);
		}

		void Rotate(double dAngle, Point oCenter)
		{
		}
	};

	class CSBezierElement : public IPathElement
	{
	public:
		CSBezierElement(){}

		EPathElement GetType() const override
		{
			return EPathElement::SBezier;
		}

		bool ReadFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (arValues.size() < 4)
				return false;

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

			while (arValues.size() > 3)
			{
				m_arPoints.push_back(oFirstPoint + oTranslatePoint);
				m_arPoints.push_back(Point{arValues[0], arValues[1]} + oTranslatePoint);
				m_arPoints.push_back(Point{arValues[2], arValues[3]} + oTranslatePoint);

				if (bRelativeCoordinate)
					oTranslatePoint = m_arPoints.back();

				arValues.erase(arValues.begin(), arValues.begin() + 4);
			}

			return true;
		}

		void Draw(IRenderer* pRenderer) const override
		{
			if (m_arPoints.empty() || 0 != (m_arPoints.size() % 3))
				return;

			for (unsigned int unIndex = 0; unIndex < m_arPoints.size(); unIndex+= 3)
				pRenderer->PathCommandCurveTo(m_arPoints[unIndex + 0].dX, m_arPoints[unIndex + 0].dY,
				                              m_arPoints[unIndex + 1].dX, m_arPoints[unIndex + 1].dY,
				                              m_arPoints[unIndex + 2].dX, m_arPoints[unIndex + 2].dY);
		}

		CSBezierElement* Copy() const override
		{
			return new CSBezierElement(*this);
		}
	};

	class CQBezierElement : public IPathElement
	{
	public:
		CQBezierElement(){}

		EPathElement GetType() const override
		{
			return EPathElement::QBezier;
		}

		bool ReadFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (arValues.size() < 4)
				return false;

			Point oLastPoint{0., 0.}, oTranslatePoint{0., 0.};

			if (NULL != pPrevElement)
			{
				oLastPoint = (*pPrevElement)[-1];
				if (bRelativeCoordinate)
					oTranslatePoint = oLastPoint;
			}

			while (arValues.size() > 3)
			{
				m_arPoints.push_back(oLastPoint);
				m_arPoints.push_back(Point{arValues[0], arValues[1]} + oTranslatePoint);
				m_arPoints.push_back(Point{arValues[2], arValues[3]} + oTranslatePoint);

				oLastPoint = m_arPoints.back();

				if (bRelativeCoordinate)
					oTranslatePoint = oLastPoint;

				arValues.erase(arValues.begin(), arValues.begin() + 4);
			}

			return true;
		}

		void Draw(IRenderer* pRenderer) const override
		{
			if (m_arPoints.empty() || 0 != (m_arPoints.size() % 3))
				return;

			for (unsigned int unIndex = 0; unIndex < m_arPoints.size(); unIndex+= 3)
				pRenderer->PathCommandCurveTo(m_arPoints[unIndex + 0].dX, m_arPoints[unIndex + 0].dY,
				                              m_arPoints[unIndex + 1].dX, m_arPoints[unIndex + 1].dY,
				                              m_arPoints[unIndex + 2].dX, m_arPoints[unIndex + 2].dY);
		}

		CQBezierElement* Copy() const override
		{
			return new CQBezierElement(*this);
		}
	};

	class CTBezierElement : public IPathElement
	{
	public:
		CTBezierElement(){}

		EPathElement GetType() const override
		{
			return EPathElement::TBezier;
		}

		bool ReadFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (arValues.size() < 2)
				return false;

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

			    if (bRelativeCoordinate)
					oSecondPoint += oFirstPoint;

				m_arPoints.push_back(oFirstPoint);
				m_arPoints.push_back(oFirstPoint);
				m_arPoints.push_back(oSecondPoint);

				return true;
		    }

		    while (arValues.size() > 1)
		    {
			    oSecondPoint = oFirstPoint + (oFirstPoint - oSecondPoint);

				m_arPoints.push_back(oFirstPoint + oTranslatePoint);
				m_arPoints.push_back(oSecondPoint + oTranslatePoint);
				m_arPoints.push_back(Point{arValues[0], arValues[1]} + oTranslatePoint);

				oFirstPoint = m_arPoints.back();

				if (bRelativeCoordinate)
					oTranslatePoint = oFirstPoint;

				arValues.erase(arValues.begin(), arValues.begin() + 2);
		    }

		    return true;
	    }

	    void Draw(IRenderer* pRenderer) const override
		{
			if (m_arPoints.empty() || 0 != (m_arPoints.size() % 3))
				return;

			for (unsigned int unIndex = 0; unIndex < m_arPoints.size(); unIndex+= 3)
				pRenderer->PathCommandCurveTo(m_arPoints[unIndex + 0].dX, m_arPoints[unIndex + 0].dY,
				                              m_arPoints[unIndex + 1].dX, m_arPoints[unIndex + 1].dY,
				                              m_arPoints[unIndex + 2].dX, m_arPoints[unIndex + 2].dY);
		}

		CTBezierElement* Copy() const override
		{
			return new CTBezierElement(*this);
		}
	};

	class CArcElement : public IPathElement
	{
	public:
		CArcElement(){}

		EPathElement GetType() const override
		{
			return EPathElement::Arc;
		}

		bool ReadFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (arValues.size() < 7)
				return false;

			Point oTranslatePoint{0., 0.};

			if (bRelativeCoordinate && NULL != pPrevElement)
				oTranslatePoint = (*pPrevElement)[-1];

			while(arValues.size() > 6)
			{
				m_arRadius.push_back(Point{arValues[0], arValues[1]});
				m_arXAxisRotation.push_back(arValues[2]);
				m_arLargeArcFlag.push_back(arValues[3]);
				m_arSweepFlag.push_back(arValues[4]);

				m_arPoints.push_back((NULL != pPrevElement) ? (*pPrevElement)[-1] : Point{0., 0.});
				m_arPoints.push_back(Point{arValues[5], arValues[6]} + oTranslatePoint);

				if (bRelativeCoordinate)
					oTranslatePoint = m_arPoints.back();

				arValues.erase(arValues.begin(), arValues.begin() + 7);
			}

			return true;
		}

		void Draw(IRenderer* pRenderer) const override
		{
			if (m_arPoints.empty())
				return;

			//TODO: данные вычисления необходимо проверить

			for (unsigned int unIndex = 0; unIndex < m_arPoints.size() / 2; ++unIndex)
			{
				int	LargeFlag		=	m_arLargeArcFlag[unIndex];
				int	SweepFlag		=	m_arSweepFlag[unIndex];

				Point Start         =	m_arPoints[unIndex + 0];
				Point End			=	m_arPoints[unIndex + 1];

				Point Center		=	GetCenter ( LargeFlag, SweepFlag, m_arRadius[unIndex], Start, End );

				double dStartAngle	=	GetAngle ( Center.dX, Center.dY, Start.dX, Start.dY);
				double dEndAngle	=	GetAngle ( Center.dX, Center.dY, End.dX, End.dY);

				double dSweep		=	0.0;

				if (GetArcAngles ( LargeFlag, SweepFlag, dStartAngle, dEndAngle, dSweep ))
					pRenderer->PathCommandArcTo(Center.dX - m_arRadius[unIndex].dX, Center.dY - m_arRadius[unIndex].dY, m_arRadius[unIndex].dX * 2.0, m_arRadius[unIndex].dY * 2.0, dStartAngle, dSweep);
			}
		}

		CArcElement* Copy() const override
		{
			return new CArcElement(*this);
		}
	private:

		// Данная часть кода взята из GraphicsPath.cpp
		CCBezierElement ConvertToCBezier(double dX, double dY, double dWidth, double dHeight, double dStartAngle, double dSweepAngle) const
		{
			double dNewStartAngle = -dStartAngle;
			double dNewSweepAngle = -dSweepAngle;

			INT bClockDirection = FALSE;
			double dEndAngle = 360 - ( dNewSweepAngle + dNewStartAngle );
			double dSrtAngle = 360 - dNewStartAngle;
			if( dNewSweepAngle > 0 )
				bClockDirection = TRUE;

			if( fabs(dNewSweepAngle) >= 360 ) // Целый эллипс
			{
				return Ellipse(dX + dWidth / 2, dY + dHeight / 2, dWidth / 2, dHeight / 2);
			}
			else // Дуга эллипса
			{
				return EllipseArc(dX + dWidth / 2, dY + dHeight / 2, dWidth / 2, dHeight / 2, dSrtAngle, dEndAngle, bClockDirection);
			}
		}

		CCBezierElement Ellipse(double dX, double dY, double dXRad, double dYRad) const
		{
			double c_fKappa = 0.552;

			CCBezierElement oCBezierElement;

			oCBezierElement.m_arPoints.push_back(Point{dX - dXRad, dY + dYRad * c_fKappa});
			oCBezierElement.m_arPoints.push_back(Point{dX - dXRad * c_fKappa, dY + dYRad});
			oCBezierElement.m_arPoints.push_back(Point{dX, dY + dYRad});

			oCBezierElement.m_arPoints.push_back(Point{dX + dXRad * c_fKappa, dY + dYRad});
			oCBezierElement.m_arPoints.push_back(Point{dX + dXRad, dY + dYRad * c_fKappa});
			oCBezierElement.m_arPoints.push_back(Point{dX + dXRad, dY});

			oCBezierElement.m_arPoints.push_back(Point{dX + dXRad, dY - dYRad * c_fKappa});
			oCBezierElement.m_arPoints.push_back(Point{dX + dXRad * c_fKappa, dY - dYRad});
			oCBezierElement.m_arPoints.push_back(Point{dX, dY - dYRad});

			oCBezierElement.m_arPoints.push_back(Point{dX - dXRad * c_fKappa, dY - dYRad});
			oCBezierElement.m_arPoints.push_back(Point{dX - dXRad, dY - dYRad * c_fKappa});
			oCBezierElement.m_arPoints.push_back(Point{dX - dXRad, dY});

			return oCBezierElement;
		}

		CCBezierElement EllipseArc(double dX, double dY, double dXRad, double dYRad, double dAngle1, double dAngle2, INT bClockDirection) const
		{
			CCBezierElement oCBezierElement;

			while ( dAngle1 < 0 )
				dAngle1 += 360;

			while ( dAngle1 > 360 )
				dAngle1 -= 360;

			while ( dAngle2 < 0 )
				dAngle2 += 360;

			while ( dAngle2 >= 360 )
				dAngle2 -= 360;

			if ( !bClockDirection )
			{
				if ( dAngle1 <= dAngle2 )
					oCBezierElement = EllipseArc2( dX, dY, dXRad, dYRad, dAngle1, dAngle2, FALSE );
				else
				{
					oCBezierElement += EllipseArc2( dX, dY, dXRad, dYRad, dAngle1, 360, FALSE );
					oCBezierElement += EllipseArc2( dX, dY, dXRad, dYRad, 0, dAngle2, FALSE );
				}
			}
			else
			{
				if ( dAngle1 >= dAngle2 )
					oCBezierElement = EllipseArc2( dX, dY, dXRad, dYRad, dAngle1, dAngle2, TRUE );
				else
				{
					oCBezierElement += EllipseArc2( dX, dY, dXRad, dYRad, dAngle1, 0, TRUE );
					oCBezierElement += EllipseArc2( dX, dY, dXRad, dYRad, 360, dAngle2, TRUE );
				}
			}
			return oCBezierElement;
		}

		CCBezierElement EllipseArc2(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, INT bClockDirection) const
		{
			// переведем углы в радианы
			CCBezierElement oCBezierElement;

			double dAngle1 = fAngle1 * M_PI / 180;
			double dAngle2 = fAngle2 * M_PI / 180;

			// Выясним в каких четвертях находятся начальная и конечная точки
			unsigned int nFirstPointQuard  = int(fAngle1) / 90 + 1;
			unsigned int nSecondPointQuard = int(fAngle2) / 90 + 1;
			nSecondPointQuard = std::min( 4, std::max( 1, (int)nSecondPointQuard ) );
			nFirstPointQuard  = std::min( 4, std::max( 1, (int)nFirstPointQuard ) );

			double fEndX = 0.0, fEndY = 0.0;

			// Дальше рисуем по четверям

			double dStartAngle = dAngle1;
			double dEndAngle = 0;

			if ( !bClockDirection )
			{
				for( unsigned int nIndex = nFirstPointQuard; nIndex <= nSecondPointQuard; nIndex++ )
				{
					if ( nIndex == nSecondPointQuard )
						dEndAngle = dAngle2;
					else
						dEndAngle = (90 * (nIndex ) ) * 3.141592f / 180;
					if ( !( nIndex == nFirstPointQuard ) )
						dStartAngle = (90 * (nIndex - 1 ) ) * 3.141592f / 180;

					oCBezierElement += EllipseArc3(fX, fY, fXRad, fYRad, AngToEllPrm( dStartAngle, fXRad, fYRad ), AngToEllPrm( dEndAngle, fXRad, fYRad ), &fEndX, &fEndY, FALSE);
				}
			}
			else
			{
				for( unsigned int nIndex = nFirstPointQuard; nIndex >= nSecondPointQuard; nIndex-- )
				{
					if ( nIndex == nFirstPointQuard )
						dStartAngle = dAngle1;
					else
						dStartAngle = (90 * (nIndex ) ) * 3.141592f / 180;
					if ( !( nIndex == nSecondPointQuard ) )
						dEndAngle = (90 * (nIndex - 1 ) ) * 3.141592f / 180;
					else
						dEndAngle = dAngle2;

					oCBezierElement += EllipseArc3(fX, fY, fXRad, fYRad, AngToEllPrm( dStartAngle, fXRad, fYRad ), AngToEllPrm( dEndAngle, fXRad, fYRad ), &fEndX, &fEndY, FALSE);
				}
			}

			return oCBezierElement;
		}

		CCBezierElement EllipseArc3(double fX, double fY, double fXRad, double fYRad, double dAngle1, double dAngle2, double *pfXCur, double *pfYCur, INT bClockDirection) const
		{
			// Рассчитаем начальную, конечную и контрольные точки
			double fX1  = 0.0, fX2  = 0.0, fY1  = 0.0, fY2  = 0.0;
			double fCX1 = 0.0, fCX2 = 0.0, fCY1 = 0.0, fCY2 = 0.0;

			double fAlpha = sin( dAngle2 - dAngle1 ) * ( sqrt( 4.0 + 3.0 * tan( (dAngle2 - dAngle1) / 2.0 ) * tan( (dAngle2 - dAngle1) / 2.0 ) ) - 1.0 ) / 3.0;

			fX1 = fX + fXRad * cos( dAngle1 );
			fY1 = fY + fYRad * sin( dAngle1 );

			fX2 = fX + fXRad * cos( dAngle2 );
			fY2 = fY + fYRad * sin( dAngle2 );

			fCX1 = fX1 - fAlpha * fXRad * sin ( dAngle1 );
			fCY1 = fY1 + fAlpha * fYRad * cos ( dAngle1 );

			fCX2 = fX2 + fAlpha * fXRad * sin ( dAngle2 );
			fCY2 = fY2 - fAlpha * fYRad * cos ( dAngle2 );

			CCBezierElement oCBezierElement;

			if ( !bClockDirection )
			{
				oCBezierElement = CCBezierElement(fCX1, fCY1, fCX2, fCY2, fX2, fY2);

				*pfXCur = fX2;
				*pfYCur = fY2;
			}
			else
			{
				oCBezierElement = CCBezierElement(fCX2, fCY2, fCX1, fCY1, fX1, fY1);

				*pfXCur = fX1;
				*pfYCur = fY1;
			}

			return oCBezierElement;
		}

		double AngToEllPrm(double fAngle, double fXRad, double fYRad) const
		{
			// Функция для перевода реального угла в параметрическое задание эллписа
			// т.е. x= a cos(t) y = b sin(t) - параметрическое задание эллписа.
			// x = r cos(p), y = r sin(p) => t = atan2( sin(p) / b, cos(p) / a );
			return atan2( sin( fAngle ) / fYRad,  cos( fAngle ) / fXRad );
		}
		//-------------------------

		Point GetCenter(int LargeFlag, int SweepFlag, Point Radi, Point P1, Point P2) const
		{
			double RadF	=	Radi.dY / Radi.dX;

			Point Begin	=	Point { P1.dX * RadF, P1.dY };
			Point End	=	Point { P2.dX * RadF, P2.dY };

			Point Mid	=	Point { ( Begin.dX + End.dX ) * 0.5, ( Begin.dY + End.dY ) * 0.5 };
			Point Vec	=	Point { Begin.dX - End.dX, Begin.dY - End.dY };

			double HChord	=  sqrt ( Vec.dX * Vec.dX + Vec.dY * Vec.dY ) * 0.5;

			Point Rot;

			if ( LargeFlag == SweepFlag )
				Rot		=	Point { -Vec.dY, Vec.dX };
			else
				Rot		=	Point { Vec.dY, -Vec.dX };

			Rot.dX		/=	( HChord * 2.0 );
			Rot.dY		/=	( HChord * 2.0 );

			double Ch	=	sqrt ( abs ( Radi.dY * Radi.dY - HChord * HChord ) );

			return Point { ( Mid.dX + Ch * Rot.dX ) / RadF, Mid.dY + Ch * Rot.dY };
		}

		double GetAngle(const Point& oFirstPoint, const Point& oSecondPoint) const
		{
			Point oThirdPoint{oFirstPoint.dX - oSecondPoint.dX, oFirstPoint.dY - oSecondPoint.dX};

			return std::atan2(oThirdPoint.dY, oThirdPoint.dX) * (180. / M_PI);
		}

		double GetAngle(const double& CX, const double& CY, const double& X, const double& Y) const
		{
			double dAngle = 0.0;

			if (0.000001 > std::fabs(X - CX) && 0.000001 > std::fabs(Y - CY) )
				return 0.0;

			if ( (X - CX) > 0.0 && 0.000001 > std::fabs(Y - CY) )
				return 0.0;
			if ( (X - CX) < 0.0 && 0.000001 > std::fabs(Y - CY) )
				return 180.0;
			if ( 0.000001 > std::fabs(X - CX) && 0.0 > (Y - CY) )
				return 270.0;
			if ( 0.000001 > std::fabs(X - CX) && 0.0 < (Y - CY) )
				return 90.0;

			dAngle	=	atan ( fabs ( X - CX ) / fabs ( Y - CY ) ) * 180.0 / M_PI;

			if ( (X - CX) > 0.0 && (Y - CY) > 0.0 )			//	1
			{
				dAngle	=	90.0 - dAngle;
			}

			if ( (X - CX) < 0.0 && (Y - CY) > 0.0 )			//	2
			{
				dAngle	+=	90.0;
			}

			if ( ( (X - CX) < 0.0) && ((Y - CY) < 0.0) )	//	3
			{
				dAngle	=	( 90.0 - dAngle ) + 180.0;
			}

			if ( (X - CX) > 0.0 && (Y - CY) < 0.0 )			//	4
			{
				dAngle	+=	-90.0;
			}

			return dAngle;
		}

		bool GetArcAngles(int LargeFlag, int SweepFlag, const double& dStartAngle, const double& dEndAngle, double& dSweep) const
		{
			dSweep		=	0.0;

			if ( 1 == LargeFlag && 0 == SweepFlag )
			{
				if ( abs ( dEndAngle - dStartAngle ) > 180 )
					dSweep	=	dEndAngle - dStartAngle;
				else
					dSweep	=	- ( 360 - abs ( dEndAngle - dStartAngle ) );

				if ( SweepFlag )
					dSweep	=	abs ( dSweep );
				else
					dSweep	=	-abs ( dSweep );
			}
			else if ( 0 == LargeFlag && 1 == SweepFlag )
			{
				if ( abs ( dEndAngle - dStartAngle ) < 180 )
					dSweep	=	dEndAngle - dStartAngle;
				else
					dSweep	=	360 - abs ( dEndAngle - dStartAngle );

				if ( SweepFlag )
					dSweep	=	abs ( dSweep );
				else
					dSweep	=	-abs ( dSweep );
			}
			else if ( 0 == LargeFlag && 0 == SweepFlag )
			{
				if ( abs ( dEndAngle - dStartAngle ) > 180 )
					dSweep	=	dEndAngle - dStartAngle;
				else
					dSweep	=	- ( 360 - abs ( dEndAngle - dStartAngle ) );

				if ( SweepFlag )
					dSweep	=	abs ( dEndAngle - dStartAngle );
				else
					dSweep	=	-abs ( dEndAngle - dStartAngle );
			}
			else if ( 1 == LargeFlag && 1 == SweepFlag )
			{
				if ( abs ( dEndAngle - dStartAngle ) > 180 )
					dSweep	=	dEndAngle - dStartAngle;
				else
					dSweep	=	- ( 360 - abs ( dEndAngle - dStartAngle ) );

				if ( SweepFlag )
					dSweep	=	abs ( dSweep );
				else
					dSweep	=	-abs ( dSweep );
			}

			return true;
		}

		std::vector<Point>     m_arRadius;
		std::vector<double>    m_arXAxisRotation;
		std::vector<double>    m_arLargeArcFlag;
		std::vector<double>    m_arSweepFlag;
	};

	class CCloseElement : public IPathElement
	{
	public:
		CCloseElement(){}

		EPathElement GetType() const override
		{
			return EPathElement::Close;
		}

		bool ReadFromArray(std::vector<double>& arValues, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (NULL != pPrevElement)
				m_arPoints.push_back((*pPrevElement)[-1]);

			return true;
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

		bool Draw(IRenderer* pRenderer, const CDefs *pDefs, bool bIsClip = false) const override;

		CPath* Copy() const override;

		IPathElement* operator[](int nIndex) const;
	private:
		void ApplyStyle(IRenderer* pRenderer, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		TBounds GetBounds() const override;

		void ReadFromString(const std::wstring& wsValue);

		std::vector<IPathElement*> m_arElements;
	};
}

#endif // CPATH_H
