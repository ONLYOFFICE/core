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
		virtual bool ReadFromString(const std::wstring& wsValue, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) = 0;
		virtual void Draw(IRenderer* pRenderer) const = 0;

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

	private:
		virtual Point GetPoint(int nIndex) const = 0;

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

		bool ReadFromString(const std::wstring& wsValue, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (wsValue.empty())
				return false;

			std::vector<double> arValues = StrUtils::ReadDoubleValues(wsValue);

			if (arValues.empty() || 0 != (arValues.size() % 2))
				return false;

			Point oTranslatePoint = {0, 0};

			if (bRelativeCoordinate && NULL != pPrevElement)
				oTranslatePoint = pPrevElement->GetPoint(-1);

			for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 2)
				m_arPoints.push_back(Point{arValues[unIndex] + oTranslatePoint.dX, arValues[unIndex + 1] + oTranslatePoint.dY});

			return true;
		}

		void Draw(IRenderer* pRenderer) const override
		{
			if (m_arPoints.empty())
				return;

			Point oPoint = GetPoint(-1);

			pRenderer->PathCommandMoveTo(oPoint.dX, oPoint.dY);
		}
	private:
		Point GetPoint(int nIndex) const override
		{
			if (m_arPoints.empty())
				return Point{0, 0};

			return m_arPoints[(nIndex > 0) ? nIndex : m_arPoints.size() + nIndex];
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

		bool ReadFromString(const std::wstring& wsValue, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (wsValue.empty())
				return false;

			std::vector<double> arValues = StrUtils::ReadDoubleValues(wsValue);

			if (arValues.empty() || 0 != (arValues.size() % 2))
				return false;

			if (!bRelativeCoordinate || NULL == pPrevElement)
			{
				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 2)
					m_arPoints.push_back(Point{arValues[unIndex], arValues[unIndex + 1]});

				return true;
			}
			else if(bRelativeCoordinate && NULL != pPrevElement)
			{
				Point oCurrentPoint = pPrevElement->GetPoint(-1);

				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 2)
				{
					oCurrentPoint.dX += arValues[unIndex];
					oCurrentPoint.dY += arValues[unIndex + 1];

					m_arPoints.push_back(oCurrentPoint);
				}

				return true;
			}

			return false;
		}

		void Draw(IRenderer* pRenderer) const override
		{
			if (m_arPoints.empty())
				return;

			for (unsigned int unIndex = 0; unIndex < m_arPoints.size(); ++unIndex)
				pRenderer->PathCommandLineTo(m_arPoints[unIndex].dX, m_arPoints[unIndex].dY);
		}
	private:
		Point GetPoint(int nIndex) const override
		{
			if (m_arPoints.empty())
				return Point{0, 0};

			return m_arPoints[(nIndex > 0) ? nIndex : m_arPoints.size() + nIndex];
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

		bool ReadFromString(const std::wstring& wsValue, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (wsValue.empty())
				return false;

			std::vector<double> arValues = StrUtils::ReadDoubleValues(wsValue);

			if (arValues.empty())
				return false;

			Point oPrevPoint = ((NULL != pPrevElement) ? pPrevElement->GetPoint(-1) : Point{0, 0});

			if (!bRelativeCoordinate || NULL == pPrevElement)
			{
				for (unsigned int unIndex = 0; unIndex < arValues.size(); ++unIndex)
					m_arPoints.push_back(Point{oPrevPoint.dX, arValues[unIndex]});

				return true;
			}
			else if(bRelativeCoordinate && NULL != pPrevElement)
			{
				Point oCurrentPoint = oPrevPoint;

				for (unsigned int unIndex = 0; unIndex < arValues.size(); ++unIndex)
				{
					oCurrentPoint.dY += arValues[unIndex];
					m_arPoints.push_back(oCurrentPoint);
				}

				return true;
			}

			return false;
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

		bool ReadFromString(const std::wstring& wsValue, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (wsValue.empty())
				return false;

			std::vector<double> arValues = StrUtils::ReadDoubleValues(wsValue);

			if (arValues.empty())
				return false;

			Point oPrevPoint = ((NULL != pPrevElement) ? pPrevElement->GetPoint(-1) : Point{0, 0});

			if (!bRelativeCoordinate || NULL == pPrevElement)
			{
				for (unsigned int unIndex = 0; unIndex < arValues.size(); ++unIndex)
					m_arPoints.push_back(Point{arValues[unIndex], oPrevPoint.dY});

				return true;
			}
			else if(bRelativeCoordinate && NULL != pPrevElement)
			{
				Point oCurrentPoint = oPrevPoint;

				for (unsigned int unIndex = 0; unIndex < arValues.size(); ++unIndex)
				{
					oCurrentPoint.dX += arValues[unIndex];
					m_arPoints.push_back(oCurrentPoint);
				}

				return true;
			}

			return false;
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

		bool ReadFromString(const std::wstring& wsValue, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (wsValue.empty())
				return false;

			std::vector<double> arValues = StrUtils::ReadDoubleValues(wsValue);

			if (arValues.empty() || 0 != (arValues.size() % 6))
				return false;

			if (!bRelativeCoordinate || NULL == pPrevElement)
			{
				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 6)
				{
					m_arPoints.push_back(Point{arValues[unIndex + 0], arValues[unIndex + 1]});
					m_arPoints.push_back(Point{arValues[unIndex + 2], arValues[unIndex + 3]});
					m_arPoints.push_back(Point{arValues[unIndex + 4], arValues[unIndex + 5]});
				}

				return true;
			}
			else if (bRelativeCoordinate && NULL != pPrevElement)
			{
				Point oPoint1{0, 0};
				Point oPoint2{0, 0};
				Point oCurrentPoint = pPrevElement->GetPoint(-1);

				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 6)
				{
					oPoint1 = oCurrentPoint + Point{arValues[unIndex + 0], arValues[unIndex + 1]};
				    oPoint2 = oCurrentPoint + Point{arValues[unIndex + 2], arValues[unIndex + 3]};
			        oCurrentPoint += Point{arValues[unIndex + 4], arValues[unIndex + 5]};

		            m_arPoints.push_back(oPoint1);
					m_arPoints.push_back(oPoint2);
					m_arPoints.push_back(oCurrentPoint);
	            }

	            return true;
            }

            return false;
        }

        void Draw(IRenderer* pRenderer) const override
		{
			if (m_arPoints.empty() || 0 != (m_arPoints.size() % 3))
				return;

			for (unsigned int unIndex = 0; unIndex < m_arPoints.size(); unIndex+= 3)
				pRenderer->PathCommandCurveTo(m_arPoints[unIndex + 0].dX, m_arPoints[unIndex + 0].dY,
				                              m_arPoints[unIndex + 1].dX, m_arPoints[unIndex + 1].dY,
				                              m_arPoints[unIndex + 2].dX, m_arPoints[unIndex + 2].dY);

//			pRenderer->PathCommandMoveTo(m_arPoints.back().dX, m_arPoints.back().dY);
//			pRenderer->PathCommandArcTo(m_arPoints.back().dX - 1, m_arPoints.back().dY - 1, 2, 2, 0, 360);
//			pRenderer->PathCommandMoveTo(m_arPoints.front().dX, m_arPoints.front().dY);

//			for (unsigned int unIndex = 0; unIndex < m_arPoints.size() - 1; ++unIndex)
//			{
//				pRenderer->PathCommandLineTo(m_arPoints[unIndex].dX, m_arPoints[unIndex].dY);

//				pRenderer->PathCommandArcTo(m_arPoints[unIndex].dX - 1, m_arPoints[unIndex].dY - 1, 2, 2, 0, 360);
//				pRenderer->PathCommandMoveTo(m_arPoints[unIndex].dX, m_arPoints[unIndex].dY);
//			}
		}

		void Rotate(double dAngle, Point oCenter)
		{
//			Matrix rotate = Matrix::Rotate(Point{oCenter.dX, oCenter.dY}, -dAngle * M_PI / 180);

//			for (unsigned int unIndex = 0; unIndex < m_arPoints.size() - 1; ++unIndex)
//			{
//				m_arPoints[unIndex] = rotate.Transform(m_arPoints[unIndex]);
//			}
		}
	private:
		Point GetPoint(int nIndex) const override
		{
			if (m_arPoints.empty())
				return Point{0, 0};

			return m_arPoints[(nIndex > 0) ? nIndex : m_arPoints.size() + nIndex];
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

		bool ReadFromString(const std::wstring& wsValue, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (wsValue.empty() || NULL == pPrevElement)
				return false;

			std::vector<double> arValues = StrUtils::ReadDoubleValues(wsValue);

			if (arValues.empty() || 0 != (arValues.size() % 4))
				return false;

			Point oFirstPoint = pPrevElement->GetPoint(-1);

			if (EPathElement::SBezier == pPrevElement->GetType() ||
			    EPathElement::CBezier == pPrevElement->GetType())
			{
				Point oPoint = pPrevElement->GetPoint(-2);

				oFirstPoint += oFirstPoint - oPoint;
			}

			if (!bRelativeCoordinate)
			{
				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 4)
				{
					m_arPoints.push_back(oFirstPoint);
					m_arPoints.push_back(Point{arValues[unIndex + 0], arValues[unIndex + 1]});
					m_arPoints.push_back(Point{arValues[unIndex + 2], arValues[unIndex + 3]});
				}

				return true;
			}
			else
			{
				Point oPoint1 = oFirstPoint;
				Point oPoint2{0, 0};
				Point oCurrentPoint = pPrevElement->GetPoint(-1);

				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 4)
				{
					oPoint1 += oCurrentPoint;
					oPoint2 = oCurrentPoint + Point{arValues[unIndex + 0], arValues[unIndex + 1]};
				    oCurrentPoint += Point{arValues[unIndex + 2], arValues[unIndex + 3]};

			        m_arPoints.push_back(oPoint1);
					m_arPoints.push_back(oPoint2);
					m_arPoints.push_back(oCurrentPoint);
		        }

		        return true;
	        }

	        return false;
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
	private:
		Point GetPoint(int nIndex) const override
		{
			if (m_arPoints.empty())
				return Point{0, 0};

			return m_arPoints[(nIndex > 0) ? nIndex : m_arPoints.size() + nIndex];
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

		bool ReadFromString(const std::wstring& wsValue, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (wsValue.empty() || NULL == pPrevElement)
				return false;

			std::vector<double> arValues = StrUtils::ReadDoubleValues(wsValue);

			if (arValues.empty() || 0 != (arValues.size() % 4))
				return false;

			Point oLastPoint = pPrevElement->GetPoint(-1);

			if (!bRelativeCoordinate)
			{
				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 4)
				{
					m_arPoints.push_back(oLastPoint);
					m_arPoints.push_back(Point{arValues[unIndex + 0], arValues[unIndex + 1]});
					m_arPoints.push_back(Point{arValues[unIndex + 2], arValues[unIndex + 3]});
				}

				return true;
			}
			else
			{
				Point oPoint1 = oLastPoint;
				Point oPoint2{0, 0};
				Point oCurrentPoint = oLastPoint;

				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 4)
				{
					oPoint1 += oCurrentPoint;
					oPoint2 = oCurrentPoint + Point{arValues[unIndex + 0], arValues[unIndex + 1]};
				    oCurrentPoint += Point{arValues[unIndex + 2], arValues[unIndex + 3]};

			        m_arPoints.push_back(oPoint1);
					m_arPoints.push_back(oPoint2);
					m_arPoints.push_back(oCurrentPoint);
		        }

		        return true;
	        }

	        return false;
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
	private:
		Point GetPoint(int nIndex) const override
		{
			if (m_arPoints.empty())
				return Point{0, 0};

			return m_arPoints[(nIndex > 0) ? nIndex : m_arPoints.size() + nIndex];
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

		bool ReadFromString(const std::wstring& wsValue, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (wsValue.empty() || NULL == pPrevElement)
				return false;

			std::vector<double> arValues = StrUtils::ReadDoubleValues(wsValue);

			if (arValues.empty() || 0 != (arValues.size() % 2))
				return false;

			Point oFirstPoint  = pPrevElement->GetPoint(-1);

			if (EPathElement::TBezier != pPrevElement->GetType() &&
			    EPathElement::QBezier != pPrevElement->GetType())
			{
				Point oSecondPoint = Point{arValues[arValues.size() - 2], arValues[arValues.size() - 1]};

			    if (bRelativeCoordinate)
					oSecondPoint += oFirstPoint;

				m_arPoints.push_back(oFirstPoint);
				m_arPoints.push_back(oSecondPoint);

				return true;
		    }

		    Point oSecondPoint = pPrevElement->GetPoint(-2);

			oSecondPoint = oFirstPoint + (oFirstPoint - oSecondPoint);

			if (!bRelativeCoordinate)
			{
				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 2)
				{
					m_arPoints.push_back(oFirstPoint);
					m_arPoints.push_back(oSecondPoint);
					m_arPoints.push_back(Point{arValues[unIndex + 0], arValues[unIndex + 1]});
				}

				return true;
			}
			else
	        {
			    Point oPoint1 = oFirstPoint;
				Point oPoint2 = oSecondPoint;
				Point oCurrentPoint = oFirstPoint;

				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 2)
				{
					oPoint1 += oCurrentPoint;
					oPoint2 = oCurrentPoint;
					oCurrentPoint += Point{arValues[unIndex + 0], arValues[unIndex + 1]};

				    m_arPoints.push_back(oPoint1);
					m_arPoints.push_back(oPoint2);
					m_arPoints.push_back(oCurrentPoint);
	            }

	                return true;
	        }

	    return false;
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
	private:
		Point GetPoint(int nIndex) const override
		{
			if (m_arPoints.empty())
				return Point{0, 0};

			return m_arPoints[(nIndex > 0) ? nIndex : m_arPoints.size() + nIndex];
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

		bool ReadFromString(const std::wstring& wsValue, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (wsValue.empty())
				return false;

			std::vector<double> arValues = StrUtils::ReadDoubleValues(wsValue);

			if (arValues.empty() || 0 != (arValues.size() % 7))
				return false;

			Point oLastPoint{0, 0};

			if (NULL != pPrevElement)
				oLastPoint = pPrevElement->GetPoint(-1);

			if (!bRelativeCoordinate || NULL == pPrevElement)
			{
				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 7)
				{
					m_arRadius.push_back(Point{arValues[unIndex + 0], arValues[unIndex + 1]});
					m_arXAxisRotation.push_back(arValues[unIndex + 2]);
					m_arLargeArcFlag.push_back(arValues[unIndex + 3]);
					m_arSweepFlag.push_back(arValues[unIndex + 4]);
					m_arPoints.push_back(oLastPoint);

					oLastPoint = Point{arValues[unIndex + 5], arValues[unIndex + 6]};

				    m_arPoints.push_back(oLastPoint);
				}
			}

			return false;
		}

		void Draw(IRenderer* pRenderer) const override
		{
			if (m_arPoints.empty())
				return;

			//TODO: данные вычисления неверные, поэтому необходимо написать реализацию заново

			for (unsigned int unIndex = 0; unIndex < m_arPoints.size() / 2; ++unIndex)
			{
				int	LargeFlag		=	static_cast<int> ( m_arLargeArcFlag[unIndex] );
				int	SweepFlag		=	static_cast<int> ( m_arSweepFlag[unIndex] );

				Point Start     =   m_arPoints[2 * unIndex];
				Point End		=	m_arPoints[2 * unIndex + 1];

				Point Center	=	GetCenter ( LargeFlag, SweepFlag, m_arRadius[unIndex], Start, End );

				double dStartAngle	=	GetAngle ( Center.dX, Center.dY, Start.dX, Start.dY );
				double dEndAngle	=	GetAngle ( Center.dX, Center.dY, End.dX, End.dY );

				double dSweep	=	0.0;

				GetArcAngles ( LargeFlag, SweepFlag, dStartAngle, dEndAngle, dSweep );

				if (0.0 != m_arXAxisRotation[unIndex])
				{
					CCBezierElement oCBezierElement = ConvertToCBezier(Center.dX - m_arRadius[unIndex].dX, Center.dY - m_arRadius[unIndex].dY, m_arRadius[unIndex].dX * 2.0, m_arRadius[unIndex].dY * 2.0, dStartAngle, dSweep);

					oCBezierElement.Rotate(-m_arXAxisRotation[unIndex], Center);

					oCBezierElement.Draw(pRenderer);
				}
				else
				{
					pRenderer->PathCommandArcTo(Center.dX - m_arRadius[unIndex].dX, Center.dY - m_arRadius[unIndex].dY, m_arRadius[unIndex].dX * 2.0, m_arRadius[unIndex].dY * 2.0, dStartAngle, dSweep);
				}
			}
		}
	private:
	    Point GetPoint(int nIndex) const override
		{
			if (m_arPoints.empty())
				return Point{0, 0};

			return m_arPoints[(nIndex > 0) ? nIndex : m_arPoints.size() + nIndex];
		}

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
				dAngle	+=	270.0;
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

		bool ReadFromString(const std::wstring& wsValue, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (NULL != pPrevElement)
				m_arPoints.push_back(pPrevElement->GetPoint(-1));

			return true;
		}

		void Draw(IRenderer* pRenderer) const override
		{
			pRenderer->PathCommandClose();
		}
	private:
		Point GetPoint(int nIndex) const override
		{
			return (m_arPoints.empty()) ? Point{0, 0} : m_arPoints.back();
		}
	};

	class CPath : public CSvgGraphicsObject
	{
	public:
		CPath(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);
		virtual ~CPath();

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CDefs *pDefs) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		TBounds GetBounds() const override;

		void ReadFromString(const std::wstring& wsValue);

		std::vector<IPathElement*> m_arElements;
	};
}

#endif // CPATH_H
