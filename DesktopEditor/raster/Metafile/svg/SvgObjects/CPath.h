#ifndef CPATH_H
#define CPATH_H

#include "CObjectBase.h"

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

	struct PathPoint
	{
		double dX;
		double dY;

		PathPoint& operator+=(const PathPoint& oPoint)
		{
			dX += oPoint.dX;
			dY += oPoint.dY;

			return *this;
		}

		PathPoint& operator-=(const PathPoint& oPoint)
		{
			dX -= oPoint.dX;
			dY -= oPoint.dY;

			return *this;
		}

		PathPoint operator+(const PathPoint& oPoint)
		{
			return PathPoint{dX + oPoint.dX, dY + oPoint.dY};
		}

		PathPoint operator-(const PathPoint& oPoint)
		{
			return PathPoint{dX - oPoint.dX, dY - oPoint.dY};
		}
	};

	class IPathElement
	{
	public:
		IPathElement() {};

		virtual EPathElement GetType() const = 0;
		virtual bool ReadFromString(const std::wstring& wsValue, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) = 0;
		virtual void Draw(IRenderer* pRenderer) const = 0;
	private:
		virtual PathPoint GetPoint(int nIndex) const = 0;

		std::vector<double> ReadDoubleValues(const std::wstring& wsValue)
		{
			std::vector<double> arValues;

			std::wstring::const_iterator oFirstPos = wsValue.begin();
			std::wstring::const_iterator oSecondPos = oFirstPos;

			while (true)
			{
				oFirstPos  = std::find_if(oSecondPos, wsValue.end(), [](const wchar_t& wChar){ return iswdigit(wChar) || L'-' == wChar; });

				if (wsValue.end() == oFirstPos)
					break;

				oSecondPos = std::find_if(oFirstPos + 1,  wsValue.end(), [](const wchar_t& wChar){ return iswspace(wChar) || L',' == wChar; });

				arValues.push_back(std::stod(std::wstring(oFirstPos, oSecondPos)));
			}

			return arValues;

		}

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

		std::vector<PathPoint> m_arPoints;
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

			std::vector<double> arValues = ReadDoubleValues(wsValue);

			if (arValues.empty() || 0 != (arValues.size() % 2))
				return false;

			PathPoint oTranslatePoint = {0, 0};

			if (bRelativeCoordinate && NULL != pPrevElement)
				oTranslatePoint = pPrevElement->GetPoint(-1);

			for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 2)
				m_arPoints.push_back(PathPoint{arValues[unIndex] + oTranslatePoint.dX, arValues[unIndex + 1] + oTranslatePoint.dY});

			return true;
		}

		void Draw(IRenderer* pRenderer) const override
		{
			if (m_arPoints.empty())
				return;

			PathPoint oPoint = GetPoint(-1);

			pRenderer->PathCommandMoveTo(oPoint.dX, oPoint.dY);
		}
	private:
		PathPoint GetPoint(int nIndex) const override
		{
			if (m_arPoints.empty())
				return PathPoint{0, 0};

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

			std::vector<double> arValues = ReadDoubleValues(wsValue);

			if (arValues.empty() || 0 != (arValues.size() % 2))
				return false;

			if (!bRelativeCoordinate || NULL == pPrevElement)
			{
				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 2)
					m_arPoints.push_back(PathPoint{arValues[unIndex], arValues[unIndex + 1]});

				return true;
			}
			else if(bRelativeCoordinate && NULL != pPrevElement)
			{
				PathPoint oCurrentPoint = pPrevElement->GetPoint(-1);

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
		PathPoint GetPoint(int nIndex) const override
		{
			if (m_arPoints.empty())
				return PathPoint{0, 0};

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

			std::vector<double> arValues = ReadDoubleValues(wsValue);

			if (arValues.empty() || 0 != (arValues.size() % 2))
				return false;

			PathPoint oPrevPoint = ((NULL != pPrevElement) ? pPrevElement->GetPoint(-1) : PathPoint{0, 0});

			if (!bRelativeCoordinate || NULL == pPrevElement)
			{
				for (unsigned int unIndex = 0; unIndex < arValues.size(); ++unIndex)
					m_arPoints.push_back(PathPoint{oPrevPoint.dX, arValues[unIndex]});

				return true;
			}
			else if(bRelativeCoordinate && NULL != pPrevElement)
			{
				PathPoint oCurrentPoint = oPrevPoint;

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

			std::vector<double> arValues = ReadDoubleValues(wsValue);

			if (arValues.empty() || 0 != (arValues.size() % 2))
				return false;

			PathPoint oPrevPoint = ((NULL != pPrevElement) ? pPrevElement->GetPoint(-1) : PathPoint{0, 0});

			if (!bRelativeCoordinate || NULL == pPrevElement)
			{
				for (unsigned int unIndex = 0; unIndex < arValues.size(); ++unIndex)
					m_arPoints.push_back(PathPoint{arValues[unIndex], oPrevPoint.dY});

				return true;
			}
			else if(bRelativeCoordinate && NULL != pPrevElement)
			{
				PathPoint oCurrentPoint = oPrevPoint;

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

		EPathElement GetType() const override
		{
			return EPathElement::CBezier;
		}

		bool ReadFromString(const std::wstring& wsValue, bool bRelativeCoordinate, IPathElement* pPrevElement = NULL) override
		{
			if (wsValue.empty())
				return false;

			std::vector<double> arValues = ReadDoubleValues(wsValue);

			if (arValues.empty() || 0 != (arValues.size() % 6))
				return false;

			if (!bRelativeCoordinate || NULL == pPrevElement)
			{
				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 6)
				{
					m_arPoints.push_back(PathPoint{arValues[unIndex + 0], arValues[unIndex + 1]});
					m_arPoints.push_back(PathPoint{arValues[unIndex + 2], arValues[unIndex + 3]});
					m_arPoints.push_back(PathPoint{arValues[unIndex + 4], arValues[unIndex + 5]});
				}

				return true;
			}
			else if (bRelativeCoordinate && NULL != pPrevElement)
			{
				PathPoint oPoint1{0, 0};
				PathPoint oPoint2{0, 0};
				PathPoint oCurrentPoint = pPrevElement->GetPoint(-1);

				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 6)
				{
					oPoint1 = oCurrentPoint + PathPoint{arValues[unIndex + 0], arValues[unIndex + 1]};
				    oPoint2 = oCurrentPoint + PathPoint{arValues[unIndex + 2], arValues[unIndex + 3]};
			        oCurrentPoint += PathPoint{arValues[unIndex + 4], arValues[unIndex + 5]};

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
		PathPoint GetPoint(int nIndex) const override
		{
			if (m_arPoints.empty())
				return PathPoint{0, 0};

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

			std::vector<double> arValues = ReadDoubleValues(wsValue);

			if (arValues.empty() || 0 != (arValues.size() % 4))
				return false;

			PathPoint oFirstPoint = pPrevElement->GetPoint(-1);

			if (EPathElement::SBezier == pPrevElement->GetType() ||
			    EPathElement::CBezier == pPrevElement->GetType())
			{
				PathPoint oPoint = pPrevElement->GetPoint(-2);

				oFirstPoint += oFirstPoint - oPoint;
			}

			if (!bRelativeCoordinate)
			{
				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 4)
				{
					m_arPoints.push_back(oFirstPoint);
					m_arPoints.push_back(PathPoint{arValues[unIndex + 0], arValues[unIndex + 1]});
					m_arPoints.push_back(PathPoint{arValues[unIndex + 2], arValues[unIndex + 3]});
				}

				return true;
			}
			else
			{
				PathPoint oPoint1 = oFirstPoint;
				PathPoint oPoint2{0, 0};
				PathPoint oCurrentPoint = pPrevElement->GetPoint(-1);

				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 4)
				{
					oPoint1 += oCurrentPoint;
					oPoint2 = oCurrentPoint + PathPoint{arValues[unIndex + 0], arValues[unIndex + 1]};
				    oCurrentPoint += PathPoint{arValues[unIndex + 2], arValues[unIndex + 3]};

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
		PathPoint GetPoint(int nIndex) const override
		{
			if (m_arPoints.empty())
				return PathPoint{0, 0};

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

			std::vector<double> arValues = ReadDoubleValues(wsValue);

			if (arValues.empty() || 0 != (arValues.size() % 4))
				return false;

			PathPoint oLastPoint = pPrevElement->GetPoint(-1);

			if (!bRelativeCoordinate)
			{
				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 4)
				{
					m_arPoints.push_back(oLastPoint);
					m_arPoints.push_back(PathPoint{arValues[unIndex + 0], arValues[unIndex + 1]});
					m_arPoints.push_back(PathPoint{arValues[unIndex + 2], arValues[unIndex + 3]});
				}

				return true;
			}
			else
			{
				PathPoint oPoint1 = oLastPoint;
				PathPoint oPoint2{0, 0};
				PathPoint oCurrentPoint = oLastPoint;

				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 4)
				{
					oPoint1 += oCurrentPoint;
					oPoint2 = oCurrentPoint + PathPoint{arValues[unIndex + 0], arValues[unIndex + 1]};
				    oCurrentPoint += PathPoint{arValues[unIndex + 2], arValues[unIndex + 3]};

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
		PathPoint GetPoint(int nIndex) const override
		{
			if (m_arPoints.empty())
				return PathPoint{0, 0};

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

			std::vector<double> arValues = ReadDoubleValues(wsValue);

			if (arValues.empty() || 0 != (arValues.size() % 2))
				return false;

			PathPoint oFirstPoint  = pPrevElement->GetPoint(-1);

			if (EPathElement::TBezier != pPrevElement->GetType() &&
			    EPathElement::QBezier != pPrevElement->GetType())
			{
				PathPoint oSecondPoint = PathPoint{arValues[arValues.size() - 2], arValues[arValues.size() - 1]};

			    if (bRelativeCoordinate)
					oSecondPoint += oFirstPoint;

				m_arPoints.push_back(oFirstPoint);
				m_arPoints.push_back(oSecondPoint);

				return true;
		    }

		    PathPoint oSecondPoint = pPrevElement->GetPoint(-2);

			oSecondPoint = oFirstPoint + (oFirstPoint - oSecondPoint);

			if (!bRelativeCoordinate)
			{
				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 2)
				{
					m_arPoints.push_back(oFirstPoint);
					m_arPoints.push_back(oSecondPoint);
					m_arPoints.push_back(PathPoint{arValues[unIndex + 0], arValues[unIndex + 1]});
				}

				return true;
			}
			else
	        {
			    PathPoint oPoint1 = oFirstPoint;
				PathPoint oPoint2 = oSecondPoint;
				PathPoint oCurrentPoint = oFirstPoint;

				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 2)
				{
					oPoint1 += oCurrentPoint;
					oPoint2 = oCurrentPoint;
					oCurrentPoint += PathPoint{arValues[unIndex + 0], arValues[unIndex + 1]};

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
		PathPoint GetPoint(int nIndex) const override
		{
			if (m_arPoints.empty())
				return PathPoint{0, 0};

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

			std::vector<double> arValues = ReadDoubleValues(wsValue);

			if (arValues.empty() || 0 != (arValues.size() % 7))
				return false;

			if (NULL != pPrevElement)
				m_oLastPoint = pPrevElement->GetPoint(-1);

			if (!bRelativeCoordinate || NULL == pPrevElement)
			{
				for (unsigned int unIndex = 0; unIndex < arValues.size(); unIndex += 7)
				{
					m_arRadius.push_back(PathPoint{arValues[unIndex + 0], arValues[unIndex + 1]});
					m_arXAxisRotation.push_back(arValues[unIndex + 2]);
					m_arLargeArcFlag.push_back(arValues[unIndex + 3]);
					m_arSweepFlag.push_back(arValues[unIndex + 4]);
					m_arPoints.push_back(PathPoint{arValues[unIndex + 5], arValues[unIndex + 6]});
				}
			}

			return false;
		}

		void Draw(IRenderer* pRenderer) const override
		{
			if (m_arPoints.empty())
				return;

			PathPoint LastPoint = m_oLastPoint;

			//TODO: данные вычисления неверные, поэтому необходимо написать реализацию заново

			for (unsigned int unIndex = 0; unIndex < m_arPoints.size(); unIndex += 2)
			{
				int	LargeFlag		=	static_cast<int> ( m_arLargeArcFlag[unIndex] );
				int	SweepFlag		=	static_cast<int> ( m_arSweepFlag[unIndex] );

				PathPoint End		=	m_arPoints[unIndex];

				PathPoint Center	=	GetCenter ( LargeFlag, SweepFlag, m_arRadius[unIndex], LastPoint, End );

				double dStartAngle	=	GetAngle ( Center.dX, Center.dY, LastPoint.dX, LastPoint.dY );
				double dEndAngle	=	GetAngle ( Center.dX, Center.dY, End.dX, End.dY );

				double dSweep	=	0.0;

				if ( GetArcAngles ( LargeFlag, SweepFlag, dStartAngle, dEndAngle, dSweep ) )
				{
					pRenderer->PathCommandArcTo(Center.dX - m_arRadius[unIndex].dX, Center.dY - m_arRadius[unIndex].dY, m_arRadius[unIndex].dX * 2.0, m_arRadius[unIndex].dY * 2.0, dStartAngle, dSweep);
				}

//				RenderArcAppx (element->GetX ( i + 1 ), Center, Point ( element->GetX ( i ), element->GetY ( i ) ), dStartAngle, dSweep);

//				if (0.0 != m_arXAxisRotation[unIndex])
//				{
//					Matrix rotate			=	Matrix::RotateAtPoint(Point(Center.X, Center.Y), -dRotAngle * M_PI / 180);

//					CArcToCubicBezier oSplitter(Center.X - Radi.X, Center.Y - Radi.Y, Radi.X * 2.0, Radi.Y * 2.0, dStartAngle - dRotAngle, dSweep);
//					for (size_t i = 0; i < oSplitter.GetCurves().size(); ++i)
//					{
//						CubicBezier oBezier	=	oSplitter.GetCurves()[i];

//						Point oPointU		=	rotate.TranformPoint(oBezier.m_Points[0]);
//						Point oPointV		=	rotate.TranformPoint(oBezier.m_Points[1]);
//						Point oPointE		=	rotate.TranformPoint(oBezier.m_Points[2]);

//						m_render->PathCommandCurveTo(oPointU.X, oPointU.Y,	oPointV.X, oPointV.Y, oPointE.X, oPointE.Y);
//					}
//				}
//				else
//				{
//				    pRenderer->PathCommandArcTo(Center.dX - m_arRadius[unIndex].dX, Center.dY - m_arRadius[unIndex].dY, m_arRadius[unIndex].dX * 2.0, m_arRadius[unIndex].dY * 2.0, dStartAngle, dSweep);
//				}

				LastPoint = End;
			}
		}
	private:
		PathPoint GetPoint(int nIndex) const override
		{
			if (m_arPoints.empty())
				return PathPoint{0, 0};

			return m_arPoints[(nIndex > 0) ? nIndex : m_arPoints.size() + nIndex];
		}

		PathPoint GetCenter(int LargeFlag, int SweepFlag, PathPoint Radi, PathPoint P1, PathPoint P2) const
		{
			double RadF	=	Radi.dY / Radi.dX;

			PathPoint Begin	=	PathPoint { P1.dX * RadF, P1.dY };
			PathPoint End	=	PathPoint { P2.dX * RadF, P2.dY };

			PathPoint Mid	=	PathPoint { ( Begin.dX + End.dX ) * 0.5, ( Begin.dY + End.dY ) * 0.5 };
			PathPoint Vec	=	PathPoint { Begin.dX - End.dX, Begin.dY - End.dY };

			double HChord	=  sqrt ( Vec.dX * Vec.dX + Vec.dY * Vec.dY ) * 0.5;

			PathPoint Rot;

			if ( LargeFlag == SweepFlag )
				Rot		=	PathPoint { -Vec.dY, Vec.dX };
			else
				Rot		=	PathPoint { Vec.dY, -Vec.dX };

			Rot.dX		/=	( HChord * 2.0 );
			Rot.dY		/=	( HChord * 2.0 );

			double Ch	=	sqrt ( abs ( Radi.dY * Radi.dY - HChord * HChord ) );

			return PathPoint { ( Mid.dX + Ch * Rot.dX ) / RadF, Mid.dY + Ch * Rot.dY };
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

		std::vector<PathPoint> m_arRadius;
		std::vector<double>    m_arXAxisRotation;
		std::vector<double>    m_arLargeArcFlag;
		std::vector<double>    m_arSweepFlag;

		PathPoint              m_oLastPoint;
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
		PathPoint GetPoint(int nIndex) const override
		{
			return (m_arPoints.empty()) ? PathPoint{0, 0} : m_arPoints.back();
		}
	};

	class CPath : public CObjectBase
	{
	public:
		CPath(CObjectBase* pParent = NULL);
		virtual ~CPath();

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer) override;
	private:
		void ApplyStyle(IRenderer* pRenderer) override;

		void ReadFromString(const std::wstring& wsValue);

		std::vector<IPathElement*> m_arElements;
	};
}

#endif // CPATH_H
