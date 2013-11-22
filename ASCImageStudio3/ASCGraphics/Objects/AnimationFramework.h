#pragma once
#include <atlcoll.h>

#include "..\Interfaces\XMLUtils.h"
#include "..\..\..\..\Common\MediaFormatDefine.h"
#include "..\Interfaces\ImageSerializeObjects2.h"
#include "..\Interfaces\ASCGraphicsRenderer.h"
#include "..\Interfaces\ISStream.h"
#include "..\..\ASCImageStudio3\Common\ImagePaintComposeConstants.h"

#define CENTER_REVOLVE_FADE_TIME_ZONE_IN	0.4
#define CENTER_REVOLVE_FADE_TIME_ZONE_EXIT	0.1

namespace Animation
{
	enum PointPathType
	{
		MoveTo		=	0,
		LineTo		=	1,
		CurveTo		=	2,
		CloseLoop	=	3,
		HorzTo		=	4,
		VertTo		=	5,
		End			=	6
	};

	class CPoint
	{
	public:
		CPoint () : X(0.0), Y(0.0) 
		{

		}

		CPoint (double dX, double dY) : X(dX), Y(dY) 
		{

		}

		double	X;
		double	Y;
	};

	class CPathPoint
	{
	public:
		int Type;

		double X [ 4 ];
		double Y [ 4 ];
	};

	class CMotionPath
	{
	public:

		CMotionPath ()
		{

		}



		inline BOOL FromStr (const CString& sXml)
		{
			CXmlNode oXml;
			if (oXml.FromXmlString(sXml))
				return FromXml (oXml);

			return FALSE;
		}

		inline BOOL FromXml (CXmlNode& oXml)
		{
			XmlUtils::CXmlReader oXmlElements;
			if (oXmlElements.SetXmlString(oXml.GetXml()))
			{
				if (oXmlElements.ReadRootNode() && oXmlElements.ReadNodeList(_T("*")))
				{
					int nCount = oXmlElements.GetLengthList();
					for (int j = 0; j < nCount; ++j)
					{
						XmlUtils::CXmlNode oXmlPoint;
						if (oXmlPoint.FromXmlString(oXmlElements.ReadNodeXml(j)))
						{
							CPathPoint	oPathPoint;

							/*
							if ( CString ( _T("move") ) == oXmlPoint.GetName () )
							{
							oPathPoint.Type		=	MoveTo;

							oPathPoint.X [ 0 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("x"), _T("0.0") ) );
							oPathPoint.Y [ 0 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("y"), _T("0.0") ) );
							}
							*/

							if ( CString ( _T("line") ) == oXmlPoint.GetName () )
							{
								oPathPoint.Type		=	LineTo;

								oPathPoint.X [ 0 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("x0"), _T("0.0") ) );
								oPathPoint.Y [ 0 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("y0"), _T("0.0") ) );
								oPathPoint.X [ 1 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("x1"), _T("0.0") ) );
								oPathPoint.Y [ 1 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("y1"), _T("0.0") ) );
							}

							if ( CString ( _T("curve") ) == oXmlPoint.GetName () )
							{
								oPathPoint.Type		=	CurveTo;

								oPathPoint.X [ 0 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("x0"), _T("0.0") ) );
								oPathPoint.Y [ 0 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("y0"), _T("0.0") ) );
								oPathPoint.X [ 1 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("x1"), _T("0.0") ) );
								oPathPoint.Y [ 1 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("y1"), _T("0.0") ) );
								oPathPoint.X [ 2 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("x2"), _T("0.0") ) );
								oPathPoint.Y [ 2 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("y2"), _T("0.0") ) );
								oPathPoint.X [ 3 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("x3"), _T("0.0") ) );
								oPathPoint.Y [ 3 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("y3"), _T("0.0") ) );
							}

							m_Points.Add ( oPathPoint );
						}
					}
				}

				return TRUE;
			}

			return FALSE;

			//CXmlNodes oXmlNodes;
			//if ( oXmlNode.GetChildNodes ( oXmlNodes ) )
			//{
			//	for ( long i = 0; i < oXmlNodes.GetCount(); ++i )
			//	{
			//		CXmlNode oXmlPoint;
			//		if ( oXmlNodes.GetAt ( i, oXmlPoint ) )
			//		{
			//			CPathPoint	oPathPoint;

			//			

			//			//if ( CString ( _T("move") ) == oXmlPoint.GetName () )
			//			//{
			//			//oPathPoint.Type		=	MoveTo;

			//			//oPathPoint.X [ 0 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("x"), _T("0.0") ) );
			//			//oPathPoint.Y [ 0 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("y"), _T("0.0") ) );
			//			//}

			//			

			//			if ( CString ( _T("line") ) == oXmlPoint.GetName () )
			//			{
			//				oPathPoint.Type		=	LineTo;

			//				oPathPoint.X [ 0 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("x0"), _T("0.0") ) );
			//				oPathPoint.Y [ 0 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("y0"), _T("0.0") ) );
			//				oPathPoint.X [ 1 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("x1"), _T("0.0") ) );
			//				oPathPoint.Y [ 1 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("y1"), _T("0.0") ) );
			//			}

			//			if ( CString ( _T("curve") ) == oXmlPoint.GetName () )
			//			{
			//				oPathPoint.Type		=	CurveTo;

			//				oPathPoint.X [ 0 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("x0"), _T("0.0") ) );
			//				oPathPoint.Y [ 0 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("y0"), _T("0.0") ) );
			//				oPathPoint.X [ 1 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("x1"), _T("0.0") ) );
			//				oPathPoint.Y [ 1 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("y1"), _T("0.0") ) );
			//				oPathPoint.X [ 2 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("x2"), _T("0.0") ) );
			//				oPathPoint.Y [ 2 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("y2"), _T("0.0") ) );
			//				oPathPoint.X [ 3 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("x3"), _T("0.0") ) );
			//				oPathPoint.Y [ 3 ]	=	_tstof ( oXmlPoint.GetAttributeOrValue ( _T("y3"), _T("0.0") ) );
			//			}

			//			m_Points.Add ( oPathPoint );
			//		}
			//	}
			//}

			//return TRUE;
		}
		//
		inline BOOL GetTranslate (const double& dTimeFactor, Animation::CPoint& oPoint) const
		{
			oPoint.X			=	0.0;
			oPoint.Y			=	0.0;

			double dPoint		=	( (double) m_Points.GetSize () ) * dTimeFactor;
			int iPtn			=	(int) dPoint;
			double T			=	dPoint - (double)iPtn;

			if ( iPtn >= 0 && (int)m_Points.GetSize () > iPtn )
			{	
				if ( 0.0 == dTimeFactor )
				{
					oPoint.X	=	m_Points [ iPtn ].X [ 0 ]; 
					oPoint.Y	=	m_Points [ iPtn ].Y [ 0 ];

					return TRUE;
				}

				if ( LineTo == m_Points [ iPtn ].Type )
				{
					oPoint.X	=	m_Points [ iPtn ].X [ 0 ] + ( m_Points [ iPtn ].X [ 1 ] - m_Points [ iPtn ].X [ 0 ] ) * T;
					oPoint.Y	=	m_Points [ iPtn ].Y [ 0 ] + ( m_Points [ iPtn ].Y [ 1 ] - m_Points [ iPtn ].Y [ 0 ] ) * T;

					return TRUE;
				}

				if ( CurveTo == m_Points [ iPtn ].Type )
				{
					GetCubicBezierVal ( m_Points [ iPtn ], T, oPoint );

					return TRUE;
				}
			}

			return FALSE;
		}

		inline BOOL GetEndPoint (Animation::CPoint& oPoint) const
		{
			if ( m_Points.GetSize () )
			{
				CPathPoint oEndPoint = m_Points [ m_Points.GetSize () - 1 ];

				if ( LineTo == oEndPoint.Type )
				{
					oPoint.X	=	oEndPoint.X [ 1 ];
					oPoint.Y	=	oEndPoint.Y [ 1 ];

					return TRUE;
				}

				if ( CurveTo == oEndPoint.Type )
				{
					oPoint.X	=	oEndPoint.X [ 3 ];
					oPoint.Y	=	oEndPoint.Y [ 3 ];

					return TRUE;
				}
			}

			return FALSE;
		}

	private:

		inline void GetCubicBezierVal ( const CPathPoint& oCurve, const double& Factor, Animation::CPoint& oPoint ) const
		{
			double mum1		=	0.0;
			double mum13	=	0.0;
			double mu3		=	0.0;

			mum1			=	1.0 - Factor;
			mum13			=	mum1 * mum1 * mum1;
			mu3				=	Factor * Factor * Factor;

			oPoint.X		=	mum13 * oCurve.X [ 0 ] + 3.0 * Factor * mum1 * mum1 * oCurve.X [ 1 ] + 3.0 * Factor * Factor * mum1 * oCurve.X [ 2 ] + mu3 * oCurve.X [ 3 ];
			oPoint.Y		=	mum13 * oCurve.Y [ 0 ] + 3.0 * Factor * mum1 * mum1 * oCurve.Y [ 1 ] + 3.0 * Factor * Factor * mum1 * oCurve.Y [ 2 ] + mu3 * oCurve.Y [ 3 ];
		}

	private:

		CSimpleArray <CPathPoint> m_Points;
	};

	class CAnglePath
	{
	public:

		CAnglePath ()
		{

		}

		inline double GetVal (const double& dF)
		{
			if (0 == m_aArray.GetCount())
				return 0.0;

			if (0.0 >= dF)
				return m_aArray[0];
			if (1.0 <= dF)
				return m_aArray[m_aArray.GetCount () - 1];

			double dFrom	=	(((double)(m_aArray.GetCount ()) - 1.0) * dF);
			long nFrom		=	(long)dFrom;

			return ( m_aArray [ nFrom + 1 ] - m_aArray [ nFrom ] ) * (dFrom - (double)nFrom) + m_aArray [ nFrom ];
		}

	public:

		CAtlArray<double> m_aArray;
	};

	class CTimeFilter
	{
	public:

		CTimeFilter()
		{

		}

		inline void Add (double dTS, double dTSQ)
		{
			m_aTS.Add (	dTS );
			m_aTS.Add ( dTSQ );
		}

		inline double GetVal (const double& dF)
		{
			if (0 == m_aTS.GetCount())
				return 0.0;

			if ( 0.0 >= dF )
				return m_aTSQ[0];

			if ( 1.0 <= dF )
				return m_aTSQ[m_aTSQ.GetCount () - 1];

			for ( long i = 0; i < (long)m_aTS.GetCount () - 1; ++i )
			{
				if ( m_aTS[i] <= dF && m_aTS[i+1] < dF )
					return ( m_aTSQ [ i + 1 ] - m_aTSQ [ i ] ) * (m_aTS[i] - dF) + m_aTSQ [ i ];
			}

			return m_aTSQ [ m_aTSQ.GetCount () - 1 ];			
		}

	private:

		CAtlArray<double> m_aTS;	//	norm time ( 0,...,1 )
		CAtlArray<double> m_aTSQ;	//	transtime
	};
	class CSpeedModificator
	{
	public:

		CSpeedModificator()
		{
			m_dTimeBegin	=	0.0;
			m_dTimeEnd		=	0.0;
			m_dDuration		=	0.0;

			m_dAccel		=	0.0;		
			m_dDecel		=	0.0;

			m_bSpeedUp		=	FALSE;
		}

		inline void FromXmlNode (CXmlNode& oXmlNode)
		{
			m_dTimeBegin	=	_tstof ( oXmlNode.GetAttributeOrValue ( _T("begin"), _T("0.0") ) );
			m_dDuration		=	_tstof ( oXmlNode.GetAttributeOrValue ( _T("dur"), _T("0.0") ) );
			m_dTimeEnd		=	m_dTimeBegin + m_dDuration;

			m_dAccel		=	_tstof ( oXmlNode.GetAttributeOrValue ( _T("accel"), _T("0.0") ) ) * 0.5;	//	
			m_dDecel		=	_tstof ( oXmlNode.GetAttributeOrValue ( _T("decel"), _T("0.0") ) ) * 0.5;

			if (0.0 != m_dAccel || 0.0 != m_dDecel)
			{
				m_bSpeedUp	=	TRUE;
			}
		}

		inline double GetFactor (const double& dTime) const
		{
			if (0.0 >= m_dDuration)
				return 0.0;

			if (dTime > m_dTimeEnd)
				return 1.0;

			if (dTime < m_dTimeBegin)
				return 0.0;

			double dSpeed	=	(dTime - m_dTimeBegin) / m_dDuration;
			if (!m_bSpeedUp)
				return dSpeed;

			// формулы как таковой нету для правильного расчета, приблизительно был определен оптимальный вариант

			if (m_dAccel > 0.0 && 0.0 == m_dDecel)
			{
				double dA	=	1.0 - m_dAccel / m_dDuration;
				dSpeed		=	powf(dSpeed, 1.0 / dA); 
			}
			else if (0.0 == m_dAccel && m_dDecel > 0.0)
			{
				double dD	=	1.0 - m_dDecel / m_dDuration;
				dSpeed		=	powf(dSpeed, dD); 
			}
			else if (m_dAccel > 0.0 && m_dDecel > 0.0)
			{
				double dA	=	1.0 - m_dAccel / m_dDuration;
				double dD	=	1.0 - m_dDecel / m_dDuration;

				dSpeed 		=	( powf(dSpeed, dD) * dSpeed + powf(dSpeed, 1.0 / dA) * (1.0 - dSpeed) ); 
			}

			return dSpeed;
		}

	private:

		BOOL m_bSpeedUp;

		double m_dDuration;

		double m_dTimeBegin;
		double m_dTimeEnd;

		double m_dAccel;
		double m_dDecel;
	};
}

namespace Animation	
{		
	enum EffectType
	{
		BaseEffect				=	0,
		FadeInType				=	1,
		FadeExitType			=	2,
		MotionEffect			=	4,
		ScaleEffect				=	5,
		MediaCall				=	6,
		RotateEffect			=	7,
		EmphasisEffect			=	8
	};

	enum FadeEffectType
	{
		AppearEffect			=	1,	//	возникновение	/	исчезновение
		FlyInEffect				=	2,		
		SpiralEffect			=	15,
		SwivelEffect			=	19,
		ZoomEffect				=	23,	//	увеличение
		BoomerangEffect			=	25,
		BounceEffect			=	26,	//	выскакивание
		CreditsEffect			=	28,	// 
		FloatEffect				=	30,	//	поворот от 90 с фэйдом
		GrowAndTurnEffect		=	31,	
		PinWheelEffect			=	35,	//	колесо ( поворот на 720 градусов )
		RiseUpEffect			=	37,	//	TODO : fade exit
		AscendEffect			=	42,	//	плавное приближение снизу вверх
		CenterRevolveEffect		=	43,	//	поворот вокруг центра
		FadedSwivelEffect		=	45,	//	вращение с фэйдом
		DescendEffect			=	47,	//	плавное приближение сверху вниз
		SpinnerEffect			=	49,	//	поворот вокруго центра на 360 градусов
		ArcUpEffect				=	52, //	скачок вверх ( составно эффект {движение по траектори + fade zoom } )
		FadedZoomEffect			=	53,	//	увеличение с проявлением
		ExpandEffect			=	55
	};

	enum EntranceEffectType
	{
		ChangeFillColor			=	1,	//	цвет заливки
		ChangeFontColor			=	3,	//	цвет текста
		GrowAndShrinkEffect		=	6,	//	простое увеличение	
		ChangeColorLines		=	7,	//	цвет линий
		SpinEffect				=	8,	//	вращение				-	CRotateEffect
		ColorBlendEffect		=	19,	//	цвет объекта			-	object fill color
		FlashBulbEffect			=	26,	//	пульсация				-	состоит из нескольких комбинированых эффектов (TODO : цельный эффект-класс)
		FlickerEffect			=	27,	//	цветовая пульсация
		TeeterEffect			=	32,	//	качание					-	CRotateEffect
		BlinkEffect				=	35	//	мигание
	};

	enum EmphasisEffectType
	{
		TransparencyEffect		=	9,	//	временная прозрачность
		DarkenEffect			=	24,	//	затемнение
		DesaturateEffect		=	25,	//	приведение к серому
		LightenEffect			=	30	//	высветление
	};

	enum ComposeEffectType
	{
		BlindsEffect			=	3,
		BoxEffect				=	4,
		CheckBoardEffect		=	5,
		CircleEffect			=	6,
		DiamondEffect			=	8,
		DissolveEffect			=	9,
		PeekEffect				=	12,
		PlusEffect				=	13,
		RandomBarsEffect		=	14,
		SplitEffect				=	16,
		StripsEffect			=	18,
		WedgeEffect				=	20,
		WheelEffect				=	21,
		WipeEffect				=	22
	};
}

namespace Animation	
{	
	class CEffect
	{
	public:

		CEffect ()
		{
			m_nInnerType			=	BaseEffect;

			m_nID					=	-1;
			m_nType					=	0;

			m_TimeBegin				=	0.0;
			m_TimeEnd				=	0.0;

			m_nTextBlock			=	0;

			m_bRemoveEmptyBlocks	=	FALSE;
		}

		virtual BOOL FromXmlNode (CXmlNode& oXml)
		{
			m_nID					=	_tstol ( oXml.GetAttributeOrValue ( _T("id"), _T("-1") ) );
			m_nType					=	_tstol ( oXml.GetAttributeOrValue ( _T("type"), _T("-1") ) );

			m_TimeBegin				=	_tstof ( oXml.GetAttributeOrValue ( _T("begin"), _T("0.0") ) );
			m_TimeEnd				=	_tstof ( oXml.GetAttributeOrValue ( _T("dur"), _T("0.0") ) ) + m_TimeBegin;

			m_nTextBlock			=	_tstol ( oXml.GetAttributeOrValue ( _T("block"), _T("0") ) );
			m_bRemoveEmptyBlocks	=	_tstol ( oXml.GetAttributeOrValue ( _T("removeemptyblocks"), _T("0") ) );

			m_oSpeed.FromXmlNode(oXml);

			return FALSE;
		}

		virtual BOOL GetTransparency (const double& dTime, double& dFactor) const
		{
			dFactor	=	1.0;

			return TRUE;
		}
		virtual int GetDirection () const	
		{
			return -1;
		}

		//
		inline const int GetID () const		
		{
			return m_nID;
		}

		inline const int GetType () const		
		{
			return m_nType;
		}

		inline const int GetInnerType () const
		{
			return m_nInnerType;
		}


		inline const double GetBeginTime () const		
		{
			return m_TimeBegin;
		}
		inline const double GetEndTime () const		
		{
			return m_TimeEnd;
		}

		//
		inline BOOL InEffect (const double& dTime) const
		{
			return ( dTime >= m_TimeBegin && dTime <= m_TimeEnd );
		}

		static CEffect* BeforeEffects (CEffect* pEffect, CEffect* pEffect2)
		{
			if (pEffect->GetBeginTime () > pEffect2->GetBeginTime ()) 
				return pEffect;

			return pEffect2;
		}
	public:

		int					m_nInnerType;

		int					m_nID;				
		int					m_nType;			

		double				m_TimeBegin;
		double				m_TimeEnd;

		long				m_nTextBlock;		//	только для текста
		BOOL				m_bRemoveEmptyBlocks;

		CSpeedModificator	m_oSpeed;			//	для плавного старта или плавного завершения эффекта
	};

	class CMotionEffect : public CEffect
	{
	public:

		CMotionEffect ()
		{
			m_nInnerType	=	MotionEffect;

		}

		virtual BOOL FromXmlNode (CXmlNode& oXmlNode)
		{
			CEffect::FromXmlNode ( oXmlNode );

			if (m_oPath.FromStr ( oXmlNode.GetAttributeOrValue ( _T("path") ) ))
				return TRUE;

			return m_oPath.FromXml ( oXmlNode );
		}

		//
		inline BOOL GetTranslate (const double& dTime, Animation::CPoint& oMove) const
		{
			oMove.X		=	0.0;
			oMove.Y		=	0.0;

			if ( dTime >= m_TimeEnd )
			{
				m_oPath.GetEndPoint (oMove);
				return TRUE;
			}

			if ( (dTime < m_TimeBegin || dTime > m_TimeEnd) || (m_TimeBegin >= m_TimeEnd) )
				return FALSE;

			return m_oPath.GetTranslate (m_oSpeed.GetFactor(dTime), oMove);
		}

		inline BOOL GetEndPoint (Animation::CPoint& oPoint) const
		{
			return m_oPath.GetEndPoint (oPoint);
		}

	private:

		CMotionPath	m_oPath;
	};

	class CFadeInEffect : public CEffect	//	entrance
	{
	public:

		CFadeInEffect ()
		{
			m_nInnerType	=	FadeInType;
		}

		virtual BOOL FromXmlNode (CXmlNode& oXmlNode)
		{
			CEffect::FromXmlNode ( oXmlNode );

			m_nDirection	=	_tstol ( oXmlNode.GetAttributeOrValue ( _T("dir"), _T("-1") ) );

			return TRUE;
		}

		virtual int GetDirection ()	const	
		{
			return m_nDirection;
		}

		virtual BOOL GetTransparency (const double& dTime, double& dFactor) const
		{
			if ( (dTime < m_TimeBegin || dTime > m_TimeEnd) || (m_TimeBegin >= m_TimeEnd) )
				return FALSE;

			if (BlindsEffect	== m_nID ||
				BoxEffect		== m_nID ||
				CheckBoardEffect== m_nID ||
				CircleEffect	== m_nID ||
				DiamondEffect	== m_nID ||
				DissolveEffect	== m_nID ||
				PeekEffect		== m_nID ||
				PlusEffect		== m_nID ||
				RandomBarsEffect== m_nID ||
				SplitEffect		== m_nID ||
				StripsEffect	== m_nID ||
				WedgeEffect		== m_nID ||
				WheelEffect		== m_nID ||
				WipeEffect		== m_nID						
				)
			{
				dFactor		=	1.0;
				return TRUE;
			}

			dFactor		=	m_oSpeed.GetFactor(dTime);	//	( dTime - m_TimeBegin ) / ( m_TimeEnd - m_TimeBegin );

			if ( CenterRevolveEffect == m_nID )
			{
				if ( dFactor <= CENTER_REVOLVE_FADE_TIME_ZONE_IN )				//	40% time effect zone fade
				{
					dFactor	=	dFactor / CENTER_REVOLVE_FADE_TIME_ZONE_IN;
					return TRUE;
				}

				dFactor	=	1.0;
			}

			if ( FloatEffect == m_nID )
			{
				if ( dFactor <= 0.9 )				//	10% - fade
				{
					dFactor	=	dFactor / 0.9;
					return TRUE;
				}

				dFactor	=	1.0;
			}

			if ( BoomerangEffect == m_nID )
			{
				if ( dFactor <= 0.9 )				//	10% - fade
				{
					dFactor	=	dFactor / 0.9;
					return TRUE;
				}

				dFactor	=	1.0;
			} 

			if (BounceEffect == m_nID)
			{
				//if ( dFactor <= 0.7 )				//	10% - fade
				//{
				//	dFactor	=	dFactor / 0.7;
				//	return TRUE;
				//}

				dFactor	=	1.0;
			}

			return TRUE;
		}

	public:

		int		m_nDirection;	
	};

	class CFadeExitEffect : public CEffect	//	exit
	{
	public:

		CFadeExitEffect ()
		{
			m_nInnerType	=	FadeExitType;
		}

		virtual BOOL FromXmlNode (CXmlNode& oXmlNode)
		{
			CEffect::FromXmlNode ( oXmlNode );

			m_nDirection	=	_tstol ( oXmlNode.GetAttributeOrValue ( _T("dir"), _T("-1") ) );

			return TRUE;
		}

		virtual int GetDirection ()	const	
		{
			return m_nDirection;
		}

		virtual BOOL GetTransparency (const double& dTime, double& dFactor) const
		{
			if ( (dTime < m_TimeBegin || dTime > m_TimeEnd) || (m_TimeBegin >= m_TimeEnd) )
				return FALSE;

			if (BlindsEffect	== m_nID ||
				BoxEffect		== m_nID ||
				CheckBoardEffect== m_nID ||
				CircleEffect	== m_nID ||
				DiamondEffect	== m_nID ||
				DissolveEffect	== m_nID ||
				PeekEffect		== m_nID ||
				PlusEffect		== m_nID ||
				RandomBarsEffect== m_nID ||
				SplitEffect		== m_nID ||
				StripsEffect	== m_nID ||
				WedgeEffect		== m_nID ||
				WheelEffect		== m_nID ||
				WipeEffect		== m_nID						
				)
			{
				dFactor		=	1.0;
				return TRUE;
			}


			dFactor			=	1.0 - m_oSpeed.GetFactor(dTime);	//	1.0 - ( dTime - m_TimeBegin ) / ( m_TimeEnd - m_TimeBegin );

			if ( CenterRevolveEffect == m_nID )
			{
				if ( dFactor <= CENTER_REVOLVE_FADE_TIME_ZONE_EXIT )				//	40% time effect zone fade
				{
					dFactor	=	dFactor / CENTER_REVOLVE_FADE_TIME_ZONE_EXIT;
					return TRUE;
				}

				dFactor		=	1.0;
			}

			if (BounceEffect == m_nID)
			{
				dFactor	=	1.0;
			}

			return TRUE;
		}


	public:

		int	m_nDirection;
	};

	class CScaleEffect : public CEffect
	{
	public:
		CScaleEffect ()
		{
			m_nInnerType	=	ScaleEffect;
		}

		virtual BOOL FromXmlNode (CXmlNode& oXmlNode)
		{
			CEffect::FromXmlNode ( oXmlNode );

			m_oScale.X	=	_tstof ( oXmlNode.GetAttributeOrValue ( _T("sx"), _T("1.0") ) );
			m_oScale.Y	=	_tstof ( oXmlNode.GetAttributeOrValue ( _T("sy"), _T("1.0") ) );

			return TRUE;
		}

		inline Animation::CPoint GetScale (const double& dTime)
		{
			if ( dTime <= m_TimeBegin )
				return Animation::CPoint(1.0, 1.0);

			if (FlashBulbEffect == m_nID)
				return GetFlashBulb (dTime);

			if ( dTime >= m_TimeEnd )
				return m_oScale;

			double dFactor = m_oSpeed.GetFactor(dTime);

			return Animation::CPoint( (m_oScale.X - 1.0) * dFactor + 1.0, (m_oScale.Y - 1.0) * dFactor + 1.0 );
		}

	protected:

		inline Animation::CPoint GetFlashBulb (const double& dTime)
		{
			static const double MAX_SCALE = 0.05;

			if (dTime >= m_TimeEnd)
				return Animation::CPoint(1.0, 1.0);

			double dFactor = (dTime - m_TimeBegin) / (m_TimeEnd - m_TimeBegin);
			if (dFactor <= 0.5)
				return Animation::CPoint( MAX_SCALE * dFactor * 2.0 + 1.0, MAX_SCALE * dFactor * 2.0 + 1.0 );

			return Animation::CPoint( MAX_SCALE * (1.0 -( dFactor - 0.5 ) * 2.0) + 1.0, MAX_SCALE * (1.0 - ( dFactor - 0.5 ) * 2.0) + 1.0 );
		}

	protected:

		Animation::CPoint	m_oScale;
	};	

	class CRotateEffect : public CEffect
	{
	public:

		CRotateEffect ()
		{
			m_nInnerType	=	RotateEffect;
		}

		virtual BOOL FromXmlNode (CXmlNode& oXmlNode) 
		{
			CEffect::FromXmlNode ( oXmlNode );

			m_dToAngle	=	_tstof ( oXmlNode.GetAttributeOrValue ( _T("angle"), _T("0.0") ) );

			if (TeeterEffect == m_nID) 
			{
				m_oAngles.m_aArray.Add (0.0);
				m_oAngles.m_aArray.Add (2.0);
				m_oAngles.m_aArray.Add (-4.0);
				m_oAngles.m_aArray.Add (4.0);
				m_oAngles.m_aArray.Add (-4.0);
				m_oAngles.m_aArray.Add (2.0);
				m_oAngles.m_aArray.Add (.0);
			}

			return TRUE;
		}

		inline double GetAngle (const double& dTime)
		{
			if ( dTime <= m_TimeBegin )
				return 0.0;

			if (TeeterEffect == m_nID)
				return GetTeeterAngle (dTime);

			if ( dTime >= m_TimeEnd )
				return m_dToAngle;

			return m_oSpeed.GetFactor(dTime) * m_dToAngle;
		}

	protected:

		inline double GetTeeterAngle (const double& dTime)
		{
			if ( dTime >= m_TimeEnd )
				return 0.0;

			return m_oAngles.GetVal(m_oSpeed.GetFactor(dTime));			
		}

	protected:

		double		m_dToAngle;
		CAnglePath	m_oAngles;
	};		

	class CEmphasisEffect : public CEffect
	{
	public:
		CEmphasisEffect ()
		{
			m_nInnerType	=	EmphasisEffect;

			m_dTransparency	=	1.0;			//	for TransparencyEffect	
			m_nColor		=	0.0;
		}

		virtual BOOL FromXmlNode ( CXmlNode& oXmlNode )
		{
			CEffect::FromXmlNode ( oXmlNode );

			m_dTransparency	=	_tstof ( oXmlNode.GetAttributeOrValue ( _T("alpha"), _T("1.0") ) );
			m_nColor		=	_tstol ( oXmlNode.GetAttributeOrValue ( _T("color"), _T("0") ) );

			return TRUE;
		}

		virtual BOOL GetTransparency (const double& dTime, double& dFactor) const
		{
			if (FlashBulbEffect == m_nID)
				return GetFlashBulb ( dTime, dFactor );
			if (BlinkEffect == m_nID )
				return GetBlink ( dTime, dFactor );

			dFactor		=	m_dTransparency;

			return TRUE;
		}


		inline unsigned long GetColor ()
		{
			return m_nColor;
		}

	protected:

		inline double GetFlashBulb (const double& dTime, double& dFactor) const
		{
			static const double MAX_BLENDING	=	0.5;

			if (dTime <= m_TimeBegin || dTime >= m_TimeEnd)
			{
				dFactor				=	1.0;
				return TRUE;
			}

			double dTimeFactor		=	( dTime - m_TimeBegin ) / ( m_TimeEnd - m_TimeBegin );
			if (dTimeFactor <= 0.5)
			{
				dFactor				=	1.0 - MAX_BLENDING * dTimeFactor * 2.0;
				return TRUE;
			}

			dFactor					=	1.0 - MAX_BLENDING * ( 1.0 - ( dTimeFactor - 0.5 ) * 2.0 );

			return TRUE;
		}

		inline double GetBlink (const double& dTime, double& dFactor) const
		{
			static const double MAX_BLENDING	=	0.5;

			if (dTime <= m_TimeBegin || dTime >= m_TimeEnd)
			{
				dFactor				=	1.0;
				return TRUE;
			}

			double dTimeFactor		=	( dTime - m_TimeBegin ) / ( m_TimeEnd - m_TimeBegin );
			if (dTimeFactor <= 0.5)
			{
				dFactor				=	0.0;
				return TRUE;
			}

			dFactor					=	1.0;

			return TRUE;
		}

	protected:

		double			m_dTransparency;		//	[ 0.0 ~ 1.0 ]
		unsigned long	m_nColor;				//	for Color Transform Effett's
	};

	class CMediaCall : public CEffect	// only for ISStream
	{
	public:
		static const long Stop	=	0;
		static const long Play	=	1;
		static const long Pause	=	2;
	public:

		CMediaCall()
		{
			m_nInnerType	=	MediaCall;

			m_nCMD		=	1; // 0 - STOP
		}

		inline virtual BOOL FromXmlNode (CXmlNode& oXml)
		{
			CEffect::FromXmlNode (oXml);

			m_nCMD			=	_tstol ( oXml.GetAttributeOrValue ( _T("event"), _T("-1") ) );

			return TRUE;
		}

		inline long CMD() const
		{
			return m_nCMD;
		}

	protected:

		long m_nCMD;
	};
}

namespace Animation
{	
	class ILayer
	{
	public:

		ILayer ()
		{

		}

		virtual ~ILayer()
		{

		}

		virtual BOOL AddEffect (CEffect* pEffect) { return FALSE; }
		virtual BOOL SetTime (const double& dTime) { return FALSE; }
		virtual const double& GetTime () const { static const double def = 0.0; return def; }
		virtual BOOL GetTransform (Animation::CPoint& oMove) { return FALSE; }
		virtual BOOL GetTransparency (double& dFactor) { dFactor = 1.0;  return FALSE; }
		virtual Animation::CPoint GetScale (double dTime) { return Animation::CPoint(1.0,1.0); }
		virtual double GetEffectAngle (double dTime) { return 0.0; }
	};
}

namespace Animation
{
	class CMotionComposer
	{
	public:
		CMotionComposer ()
		{

		}

		inline BOOL Add ( CEffect* pEffect )
		{
			if ( pEffect )
			{
				if ( MotionEffect == pEffect->GetInnerType () )
				{
					m_refEffects.Add ( static_cast<CMotionEffect*> ( pEffect ) );

					return TRUE;
				}
			}

			return FALSE;
		}

		inline BOOL GetTranslate (const double& dTime, Animation::CPoint& oMove) const
		{
			oMove.X	=	0.0;
			oMove.Y	=	0.0;

			if ( 0 == m_refEffects.GetCount() )
				return FALSE;

			for ( long i = (long)m_refEffects.GetCount () - 1; i >= 0; --i  )
			{
				if ( m_refEffects[i]->GetTranslate ( dTime, oMove ) )
				{
					return TRUE;
				}
			}

			long nLeftEffect = GetLeftEffect ( dTime );		//	берем близкий эффект по времени слева
			if ( -1 != nLeftEffect )
				return m_refEffects [ nLeftEffect ]->GetEndPoint ( oMove );

			return FALSE;
		}

		inline BOOL CompileEffects (const double& dTime, CMotionEffect*& pEffect, CMotionEffect*& pStrafe)
		{
			if ( 0 == (int)m_refEffects.GetCount () )
				return FALSE;

			pStrafe	=	NULL;
			pEffect	=	NULL;

			double dBeginTime	=	-1.0;
			int nInd	=	-1;

			for ( int i = 0; i < (int) m_refEffects.GetCount(); ++i )
			{
				CMotionEffect* pRef	=	m_refEffects[i];
				if ( pRef->InEffect ( dTime ) || pRef->GetBeginTime () < dTime )
				{
					if (dBeginTime < pRef->GetBeginTime ())
					{
						nInd		=	i;
						dBeginTime	=	pRef->GetBeginTime ();
					}
				}
			}

			if (-1 != nInd)
				pEffect		=	m_refEffects[nInd];

			return TRUE;	
		}


	protected:

		inline long GetLeftEffect (const double& dTime) const		//	-1	-	failed left effect
		{
			if ( 0 == m_refEffects.GetCount() )
				return -1;

			double dTimeBegin = -1.0;
			double dTimeEnd = -1.0;

			long nInd = -1;

			for ( long i = (long)m_refEffects.GetCount () - 1; i >= 0; --i  )
			{
				CMotionEffect* effect = m_refEffects[i];

				if ( (m_refEffects[i]->GetEndTime() >= dTimeEnd) && (m_refEffects[i]->GetEndTime() <= dTime) )
				{
					dTimeEnd	=	m_refEffects[i]->GetEndTime();
					nInd		=	i;
				}
			}

			return nInd;
		}

	protected:

		CAtlArray <CMotionEffect*>	m_refEffects;
	};

	class CFadeComposer
	{
	public:
		CFadeComposer ()
		{
			m_dInTime		=	-1.0;
			m_dExitTime		=	-1.0;
		}

		inline BOOL Add (CEffect* pEffect)
		{
			if ( pEffect )
			{
				if ( FadeInType == pEffect->GetInnerType () )
				{
					m_refInEffects.Add ( static_cast<CFadeInEffect*> ( pEffect ) );

					if ( -1.0 == m_dInTime )
						m_dInTime	=	pEffect->GetBeginTime ();
					else
						m_dInTime	=	__min ( pEffect->GetBeginTime (), m_dInTime );

					return TRUE;
				}

				if ( FadeExitType == pEffect->GetInnerType () )
				{
					m_refExitEffects.Add ( static_cast<CFadeExitEffect*> ( pEffect ) );

					if ( -1.0 == m_dExitTime )
						m_dExitTime	=	pEffect->GetBeginTime ();
					else
						m_dExitTime	=	__min ( pEffect->GetBeginTime (), m_dExitTime );

					return TRUE;
				}

				if ( EmphasisEffect == pEffect->GetInnerType ())
				{
					m_refEmphasisEffects.Add (static_cast<CEmphasisEffect*>(pEffect));
					return TRUE;
				}
			}

			return FALSE;
		}

		inline BOOL GetTransparency (const double& dTime, double& dFactor)
		{
			return EffectTransparency ( dTime, dFactor );
		}
		inline BOOL CompileEffects (const double& dTime, CFadeInEffect*& pEffect, CFadeInEffect*& pStrafe)
		{
			if ( 0 == (int)m_refInEffects.GetCount () )
				return FALSE;

			int nZone			=	-1;
			double dZoneBegin	=	-1.0;

			// pStrafe - эффект у которого begin time самое большое
			double dStrafeBegin =	-1.0;
			int nStrafe			=	-1;

			for ( int i = 0; i < (int)m_refInEffects.GetCount(); ++i )
			{
				if ( FadeInType == m_refInEffects[i]->GetType() )
				{
					if ( (m_refInEffects[i]->InEffect ( dTime ) || m_refInEffects[i]->GetEndTime () <= dTime ) && 
						m_refInEffects[i]->GetBeginTime () >= dStrafeBegin )
					{
						nStrafe			=	i;
						pStrafe			=	(CFadeInEffect*)m_refInEffects[i];
						dStrafeBegin	=	m_refInEffects[i]->GetBeginTime ();
					}
				}

				if ( m_refInEffects[i]->InEffect ( dTime ) )
				{
					if ( FadeInType == m_refInEffects[i]->GetType() )
					{
						if ( m_refInEffects[i]->GetBeginTime () >= dZoneBegin )
						{
							nZone		=	i;
							pEffect		=	(CFadeInEffect*)m_refInEffects[i];
							dZoneBegin	=	m_refInEffects[i]->GetBeginTime ();
						}
					}
				}
			}

			if ( nStrafe != nZone && (-1 != nZone && -1 != nStrafe) )
			{
				if ( pStrafe->InEffect ( dTime ) )	// если находимся в зоне эффекта, иначе объект без эффекта
				{
					pEffect		=	pStrafe;
				}
			}

			if ( nStrafe == nZone && (-1 != nZone && -1 != nStrafe) )
			{
				return TRUE;
			}

			pEffect		=	NULL;

			return FALSE;	
		}

		inline BOOL CompilExitEffects (const double& dTime, CFadeExitEffect*& pEffect, CFadeExitEffect*& pStrafe)
		{
			if ( 0 == (int)m_refExitEffects.GetCount () )
				return FALSE;

			int nZone			=	-1;
			double dZoneBegin	=	-1.0;

			// pStrafe - эффект у которого begin time самое большое
			double dStrafeBegin =	-1.0;
			int nStrafe			=	-1;

			for ( int i = 0; i < (int)m_refExitEffects.GetCount(); ++i )
			{
				if ( FadeExitType == m_refExitEffects[i]->GetType() )
				{
					if ( (m_refExitEffects[i]->InEffect ( dTime ) || m_refExitEffects[i]->GetEndTime () <= dTime ) && 
						m_refExitEffects[i]->GetBeginTime () >= dStrafeBegin )
					{
						nStrafe			=	i;
						pStrafe			=	(CFadeExitEffect*)m_refExitEffects[i];
						dStrafeBegin	=	m_refExitEffects[i]->GetBeginTime ();
					}
				}

				if ( m_refExitEffects[i]->InEffect ( dTime ) )
				{
					if ( FadeExitType == m_refExitEffects[i]->GetType() )
					{
						if ( m_refExitEffects[i]->GetBeginTime () >= dZoneBegin )
						{
							nZone		=	i;
							pEffect		=	(CFadeExitEffect*)m_refExitEffects[i];
							dZoneBegin	=	m_refExitEffects[i]->GetBeginTime ();
						}
					}
				}
			}

			if ( nStrafe != nZone && (-1 != nZone && -1 != nStrafe) )
			{
				if ( pStrafe->InEffect ( dTime ) )	// если находимся в зоне эффекта, иначе объект без эффекта
				{
					pEffect		=	pStrafe;
				}
			}

			if ( nStrafe == nZone && (-1 != nZone && -1 != nStrafe) )
			{
				return TRUE;
			}

			pEffect		=	NULL;

			return FALSE;	
		}

	protected:
		inline BOOL FadeInAtBegin () const
		{
			if ( -1.0 != m_dInTime )
			{
				if ( (m_dInTime < m_dExitTime) || (m_dExitTime==-1))
					return TRUE;
			}

			return FALSE;
		}

		inline BOOL FadeExitAtBegin () const
		{
			if ( -1.0 != m_dExitTime )
			{
				if ( (m_dExitTime < m_dInTime) || (m_dInTime==-1))
					return TRUE;
			}

			return FALSE;
		}


		inline BOOL IsSmoothEffect (CEffect* pEffect) const  
		{
			// проверям эффект поддерживает плавное изменение прозрачности

			if ( ZoomEffect		==	pEffect->GetID () )
				return FALSE;
			if ( FlyInEffect	==	pEffect->GetID () )
				return FALSE;
			if ( CreditsEffect	==	pEffect->GetID () )
				return FALSE;
			if ( SpiralEffect	==	pEffect->GetID () )
				return FALSE;
			if ( AppearEffect	==	pEffect->GetID () )
				return FALSE;
			if ( SwivelEffect	==	pEffect->GetID () )
				return FALSE;
			//if ( BounceEffect	==	pEffect->GetID () )
			//	return FALSE;

			return TRUE;
		}


		inline BOOL EffectTransparency (const double& dTime, double& dFactor)
		{
			double dAlpha		=	GetEmphasisAlpha (dTime);

			BOOL bFade			=	FALSE;
			dFactor				=	min(dAlpha,dFactor);
			CEffect* pEffect	=	NULL;

			CFadeInEffect* pInEffect	=	NULL;
			CFadeInEffect* pIn			=	NULL;
			if ( CompileEffects ( dTime, pInEffect, pIn ) )
			{
				bFade			=	TRUE;
			}

			CFadeExitEffect* pExitEffect	=	NULL;
			CFadeExitEffect* pExit			=	NULL;
			if ( CompilExitEffects ( dTime, pExitEffect, pExit ) )
			{
				bFade			=	TRUE;
			}

			if ( FALSE == bFade )
			{
				if ( pIn && pExit )
				{
					pEffect = CEffect::BeforeEffects (pIn, pExit);
					if (FadeInType== pEffect->GetInnerType ())
						dFactor		=	min(dAlpha,dFactor);

					if (FadeExitType== pEffect->GetInnerType ())
						dFactor		=	0.0;

					return TRUE;
				}

				if ( NULL == pIn && pExit )
				{
					dFactor		=	0.0;
					return TRUE;
				}

				if ( pIn && NULL == pExit )
				{
					dFactor		=	min(dAlpha,dFactor);
					return TRUE;
				}

				if ( FadeInAtBegin () )
					dFactor		=	0.0;

				if ( FadeExitAtBegin () )
					dFactor		=	min(dAlpha,dFactor);

				return TRUE;
			}

			if ( pInEffect && pExitEffect )
				pEffect = CEffect::BeforeEffects (pInEffect, pExitEffect);

			if ( NULL == pInEffect && pExitEffect )
				pEffect = pExitEffect;

			if ( pInEffect && NULL == pExitEffect )
				pEffect = pInEffect;

			if ( pEffect )
			{
				pEffect->GetTransparency ( dTime, dFactor );

				if ( dFactor > 0.0 && (FALSE == IsSmoothEffect (pEffect)) )
					dFactor	=	dAlpha;

				dFactor		=	min(dAlpha,dFactor);

				return TRUE;
			}

			if ( pIn && pExit )
			{
				pEffect = CEffect::BeforeEffects (pIn, pExit);
				if (FadeInType== pEffect->GetInnerType ())
					dFactor		=	dAlpha;

				if (FadeExitType== pEffect->GetInnerType ())
					dFactor		=	0.0;

				return TRUE;
			}

			if ( NULL == pIn && pExit )
			{
				dFactor		=	0.0;
				return TRUE;
			}

			if ( pIn && NULL == pExit )
			{
				dFactor		=	dAlpha;
				return TRUE;
			}

			return TRUE;
		}

		inline double GetEmphasisAlpha (const double& dTime)
		{
			double dAlpha	=	1.0;

			for (long i = 0; i < (long)m_refEmphasisEffects.GetCount(); ++i)
			{
				CEmphasisEffect* pEffect = m_refEmphasisEffects[i];
				if (pEffect->InEffect (dTime))
					pEffect->GetTransparency (dTime,dAlpha); 
			}

			return dAlpha;
		}

	protected:

		CAtlArray <CFadeInEffect*>		m_refInEffects;			//	array of references 
		CAtlArray <CFadeExitEffect*>	m_refExitEffects;		//	array of references 

		CAtlArray <CEmphasisEffect*>	m_refEmphasisEffects;	//	array of references 

		double	m_dInTime;
		double	m_dExitTime;
	};

	class CScaleComposer
	{
	public:
		CScaleComposer ()
		{

		}

		inline BOOL Add (CEffect* pEffect)
		{
			if ( pEffect )
			{
				if ( ScaleEffect == pEffect->GetInnerType () )
				{
					m_refEffects.Add ( static_cast<CScaleEffect*> ( pEffect ) );
					return TRUE;
				}
			}

			return FALSE;
		}

		inline Animation::CPoint GetScale (double dTime)
		{
			if (0 == (long)m_refEffects.GetCount())
				return Animation::CPoint(1.0, 1.0);

			Animation::CPoint oScale(1.0, 1.0);

			for ( long i = 0; i < (long)m_refEffects.GetCount(); ++i )
			{
				Animation::CPoint oScaleEffect	=	m_refEffects[i]->GetScale(dTime);

				oScale.X	*=	oScaleEffect.X;
				oScale.Y	*=	oScaleEffect.Y;
			}

			return oScale;
		}

	protected:

		CAtlArray <CScaleEffect*> m_refEffects;		//	array of references 
	};

	class CRotateComposer
	{
	public:
		CRotateComposer ()
		{

		}

		inline BOOL Add (CEffect* pEffect)
		{
			if ( pEffect )
			{
				if ( RotateEffect == pEffect->GetInnerType () )
				{
					m_refEffects.Add ( static_cast<CRotateEffect*> ( pEffect ) );
					return TRUE;
				}
			}

			return FALSE;
		}

		inline double GetAngle (double dTime)
		{
			if (0 == (long)m_refEffects.GetCount())
				return 0.0;

			double dAngle	=	0.0;

			for ( long i = 0; i < (long)m_refEffects.GetCount(); ++i )
				dAngle +=	m_refEffects[i]->GetAngle(dTime);

			return dAngle;
		}

	protected:

		CAtlArray <CRotateEffect*> m_refEffects;		//	array of references 
	};


	class CAnimationsBuilder
	{
	public:
		CAnimationsBuilder ()
		{
		}

		CAnimationsBuilder (CXmlNode& oXmlNode, ILayer* pLayer = NULL) : m_pLayer (pLayer)
		{
			From (oXmlNode);
		}

		CAnimationsBuilder (const CString& Xml, ILayer* pLayer = NULL) : m_pLayer (pLayer)
		{
			CXmlNode oXmlNode;
			if (oXmlNode.FromXmlString(Xml))
				From (oXmlNode);
		}

		inline BOOL From (const CString& sXml)
		{
			CXmlNode oXmlNode;
			if (oXmlNode.FromXmlString(sXml))
				return From (oXmlNode);

			return FALSE;
		}

		inline BOOL From (CXmlNode& oXml)
		{
			if (m_pLayer)
			{
				XmlUtils::CXmlReader oXmlElements;
				oXmlElements.SetXmlString(oXml.GetXml());

				if (oXmlElements.ReadRootNode() && oXmlElements.ReadNodeList(_T("*")))
				{
					int nCount = oXmlElements.GetLengthList();
					for (int j = 0; j < nCount; ++j)
					{
						XmlUtils::CXmlNode oElem;
						if (oElem.FromXmlString(oXmlElements.ReadNodeXml(j)))
						{
							CEffect* pEffect	=	NULL;
							CString sEffectType	=	oElem.GetName ();

							if (CString ( _T("animate") ) == sEffectType )
								pEffect = new CEffect ();

							if (CString ( _T("animateMotion") ) == sEffectType )
								pEffect = new CMotionEffect ();

							if (CString ( _T("animateFadeIn") ) == sEffectType )
								pEffect = new CFadeInEffect ();

							if (CString ( _T("animateFadeExit") ) == sEffectType )
								pEffect = new CFadeExitEffect ();

							if (CString ( _T("animateScale") ) == sEffectType)
								pEffect = new CScaleEffect ();

							if (CString ( _T("animateRotate") ) == sEffectType)
								pEffect = new CRotateEffect ();

							if (CString ( _T("animateAlpha") ) == sEffectType)
								pEffect = new CEmphasisEffect ();

							if (CString ( _T("animateEmphasis") ) == sEffectType)
								pEffect = new CEmphasisEffect ();

							if (CString ( _T("animateColor") ) == sEffectType)
								pEffect	= new CEmphasisEffect ();

							if (CString ( _T("mediaCall") ) == sEffectType)
								pEffect = new CMediaCall ();

							if ( pEffect )
							{
								if ( pEffect->FromXmlNode (oElem) )
								{
									if ( m_pLayer ) 
										m_pLayer->AddEffect ( pEffect );
								}
							}
						}
					}
				}

				return TRUE;
			}

			return FALSE;

			/*
			if ( NULL == m_pLayer ) 
				return FALSE;

			CXmlNodes oXmlNodes;
			if ( oXmlNode.GetChildNodes ( oXmlNodes ) )
			{
				for ( long i = 0; i < oXmlNodes.GetCount(); ++i )
				{
					CXmlNode oXmlNode;
					if ( oXmlNodes.GetAt ( i, oXmlNode ) )
					{
						CEffect* pEffect	=	NULL;
						CString sEffectType	=	oXmlNode.GetName ();

						if (CString ( _T("animate") ) == sEffectType )
							pEffect = new CEffect ();

						if (CString ( _T("animateMotion") ) == sEffectType )
							pEffect = new CMotionEffect ();

						if (CString ( _T("animateFadeIn") ) == sEffectType )
							pEffect = new CFadeInEffect ();

						if (CString ( _T("animateFadeExit") ) == sEffectType )
							pEffect = new CFadeExitEffect ();

						if (CString ( _T("animateScale") ) == sEffectType)
							pEffect = new CScaleEffect ();

						if (CString ( _T("animateRotate") ) == sEffectType)
							pEffect = new CRotateEffect ();

						if (CString ( _T("animateAlpha") ) == sEffectType)
							pEffect = new CEmphasisEffect ();

						if (CString ( _T("animateEmphasis") ) == sEffectType)
							pEffect = new CEmphasisEffect ();

						if (CString ( _T("animateColor") ) == sEffectType)
							pEffect	= new CEmphasisEffect ();

						if (CString ( _T("mediaCall") ) == sEffectType)
							pEffect = new CMediaCall ();

						if ( pEffect )
						{
							if ( pEffect->FromXmlNode ( oXmlNode ) )
							{
								if ( m_pLayer ) 
									m_pLayer->AddEffect ( pEffect );
							}
						}
					}
				}
			}

			return TRUE;
			*/
		}

		inline void SetStorage (ILayer* pLayer)
		{
			m_pLayer = pLayer;
		}

	private:

		ILayer*	m_pLayer;
	};

	class CAnimationLayer : public ILayer
	{
	public:
		CAnimationLayer () 
		{
			m_dTime			=	0.0;
			m_bChangeColor	=	FALSE;

			m_oBuilder.SetStorage ( this );
		}

		~CAnimationLayer ()
		{
			Clear ();
		}

		inline void ClearReferences ()
		{
			m_Effects.RemoveAll ();
		}

		inline CAnimationsBuilder& GetBuilder ()
		{
			return m_oBuilder;
		}

		// ILayer

		virtual BOOL AddEffect ( CEffect* pEffect )
		{
			m_Effects.Add (pEffect);

			m_oMotionComposer.Add (pEffect);	
			m_oFadeComposer.Add (pEffect);
			m_oScaleComposer.Add (pEffect);
			m_oRotateComposer.Add (pEffect);

			if ( FadeInType == pEffect->GetInnerType () || FadeExitType == pEffect->GetInnerType ())
			{
				int nEffectID = pEffect->GetID();

				if (nEffectID == BlindsEffect		||
					nEffectID == BoxEffect			||
					nEffectID == CheckBoardEffect	||
					nEffectID == CircleEffect		||
					nEffectID == DiamondEffect		||
					nEffectID == DissolveEffect		||
					nEffectID == PeekEffect			||
					nEffectID == PlusEffect			||
					nEffectID == RandomBarsEffect	||
					nEffectID == SplitEffect		||
					nEffectID == StripsEffect		||
					nEffectID == WedgeEffect		||
					nEffectID == WheelEffect		||
					nEffectID == WipeEffect							
					)
				{
					m_arrComposes.Add (pEffect);
				}

				if (BounceEffect == nEffectID)
				{
					if (FadeInType == pEffect->GetInnerType ())
					{
						CFadeInEffect* pCompose = new CFadeInEffect ();
						if (pCompose)
						{
							pCompose->m_nInnerType	=	pEffect->m_nInnerType;
							pCompose->m_nID			=	WipeEffect;
							pCompose->m_nDirection	=	1;	
							pCompose->m_TimeBegin	=	pEffect->m_TimeBegin;
							pCompose->m_TimeEnd		=	pEffect->m_TimeBegin + (pEffect->m_TimeEnd - pEffect->m_TimeBegin) * 0.29;

							m_arrComposes.Add (pCompose);
							m_Effects.Add (pCompose);
						}
					}

					if (FadeExitType == pEffect->GetInnerType ())
					{
						CFadeExitEffect* pCompose = new CFadeExitEffect ();
						if (pCompose)
						{
							pCompose->m_nInnerType	=	pEffect->m_nInnerType;
							pCompose->m_nID			=	WipeEffect;
							pCompose->m_nDirection	=	1;
							pCompose->m_TimeBegin	=	pEffect->m_TimeBegin;
							pCompose->m_TimeEnd		=	pEffect->m_TimeBegin + (pEffect->m_TimeEnd - pEffect->m_TimeBegin) * 0.29;

							m_arrComposes.Add (pCompose);
							m_Effects.Add (pCompose);
						}
					}
				}
			}

			if (EmphasisEffect == pEffect->GetInnerType ())
			{
				m_refEmpEffects.Add (static_cast<CEmphasisEffect*>(pEffect));

				int nEffectID	=	pEffect->GetID();
				if (nEffectID == ChangeFillColor	||
					nEffectID == ChangeFontColor	||
					nEffectID == ChangeColorLines	||
					nEffectID == ColorBlendEffect	||
					nEffectID == FlashBulbEffect	||
					nEffectID == FlickerEffect		||
					nEffectID == DarkenEffect		||
					nEffectID == LightenEffect		||
					nEffectID == DesaturateEffect )
				{
					m_bChangeColor	=	TRUE;
				}
			}

			if (MediaCall == pEffect->GetInnerType())
			{
				m_arrMediaManage.Add(static_cast<CMediaCall*>(pEffect));
			}

			return TRUE;
		}

		virtual BOOL SetTime ( const double& dTime )
		{
			m_dTime	=	dTime;
			return TRUE;
		}

		virtual const double& GetTime () const
		{ 
			return m_dTime; 
		}

		virtual BOOL GetTransform (Animation::CPoint& oMove)
		{
			return m_oMotionComposer.GetTranslate ( m_dTime, oMove );
		}

		virtual BOOL GetTransparency ( double& dFactor ) 
		{
			return m_oFadeComposer.GetTransparency ( m_dTime, dFactor );
		}

		virtual Animation::CPoint GetScale (double dTime) 
		{
			return m_oScaleComposer.GetScale (dTime);
		}

		virtual double GetEffectAngle (double dTime) 
		{
			return m_oRotateComposer.GetAngle(dTime);
		}

		inline CAtlArray <CEffect*>& Effects()
		{
			return m_Effects;
		}
		// 

		inline BOOL IsChangeFillColor ()
		{
			return m_bChangeColor;
		}


		// CAnimationLayer

		inline BOOL GetFadeInEffect ( CFadeInEffect*& pEffect, CFadeInEffect*& pStrafe )
		{
			return m_oFadeComposer.CompileEffects ( m_dTime, pEffect, pStrafe );	
		}
		inline BOOL GetFadeExitEffect ( CFadeExitEffect*& pEffect, CFadeExitEffect*& pStrafe )
		{
			return m_oFadeComposer.CompilExitEffects ( m_dTime, pEffect, pStrafe );	
		}

		inline BOOL GetMotionEffect ( CMotionEffect*& pEffect, CMotionEffect*& pStrafe )
		{
			return m_oMotionComposer.CompileEffects ( m_dTime, pEffect, pStrafe );
		}

		inline BOOL GetEmphasisEffect ( CEmphasisEffect*& pEffect, CEmphasisEffect*& pStrafe )
		{
			int nZone			=	-1;
			double dZoneBegin	=	-1.0;

			double dStrafeBegin =	-1.0;
			int nStrafe			=	-1;

			double dTime		= GetTime ();

			for ( long i = 0; i < (long)m_Effects.GetCount(); ++i )
			{
				if (EmphasisEffect == m_Effects[i]->GetInnerType())
				{
					CEmphasisEffect* pEffect = (CEmphasisEffect*)m_Effects[i];

					if ( (pEffect->InEffect ( dTime ) || m_Effects[i]->GetEndTime () <= dTime ) && m_Effects[i]->GetBeginTime () >= dStrafeBegin )
					{
						nStrafe			=	i;
						pStrafe			=	pEffect;
						dStrafeBegin	=	pEffect->GetBeginTime ();
					}

					if ( pEffect->InEffect (dTime) )
					{
						if ( pEffect->GetBeginTime () >= dZoneBegin )
						{
							nZone		=	i;
							pEffect		=	pEffect;
							dZoneBegin	=	pEffect->GetBeginTime ();
						}
					}
				}
			}

			if ( nStrafe != nZone && (-1 != nZone && -1 != nStrafe) )
			{
				if ( pStrafe->InEffect ( dTime ) )	// если находимся в зоне эффекта, иначе объект без эффекта
				{
					pEffect		=	pStrafe;
				}
			}

			if ( nStrafe == nZone && (-1 != nZone && -1 != nStrafe) )
			{
				return TRUE;
			}

			pEffect		=	NULL;

			return FALSE;	
		}

		inline BOOL GetDesaturateFactor (double& dFactor)		//	эффект действует на все время показа объекта
		{
			double dTime				=	GetTime ();

			BOOL bLeftEffect			=	FALSE;		// уже был применен эффект приведение к серому
			BOOL bHaveInEffect			=	FALSE;		// находимся в зоне эффекта

			CEmphasisEffect* pInEffect	=	NULL;

			for ( long i = 0; i < (long)m_refEmpEffects.GetCount(); ++i )
			{
				if (EmphasisEffect == m_refEmpEffects[i]->GetInnerType())
				{
					CEmphasisEffect* pEffect = (CEmphasisEffect*)m_refEmpEffects[i];
					if ( DesaturateEffect == pEffect->GetID () )
					{
						if ( pEffect->InEffect (dTime) )
						{
							if (NULL == pInEffect)
								pInEffect	=	pEffect;
						}

						if ( pEffect->GetEndTime () <= dTime )
						{
							dFactor	= 0.0;			//	элемент должен быть серым
							return TRUE;
						}
					}
				}
			}

			if (pInEffect)	//	находимся в зоне эффекта, примененям постепенное изменение в сторону серого цвета
			{
				dFactor	=	1.0 - ( dTime - pInEffect->GetBeginTime() ) / ( pInEffect->GetEndTime () - pInEffect->GetBeginTime() );
				return TRUE;
			}

			return FALSE;	//	цвет не меняется
		}


		inline CAtlArray <CEmphasisEffect*>& GetEmpEffects ()
		{
			return m_refEmpEffects;
		}

		inline CEffect* GetCompose ()
		{
			if (0 == (long)m_arrComposes.GetCount())
				return NULL;

			double dTime		=	GetTime ();

			CEffect* pEffect	=	NULL;
			double dMaxTime		=	-1.0;
			for ( long i = 0; i < (long)m_arrComposes.GetCount(); ++i )
			{
				if ( m_arrComposes[i]->InEffect(dTime) )
				{
					if (NULL == pEffect)
					{
						pEffect		=	m_arrComposes[i];
						dMaxTime	=	pEffect->GetBeginTime ();	
					}
					else
					{
						if (dMaxTime < m_arrComposes[i]->GetBeginTime ())
						{
							pEffect		=	m_arrComposes[i];
							dMaxTime	=	pEffect->GetBeginTime ();	
						}
					}
				}

			}

			return pEffect;
		}


		//
		inline const CMediaCall* GetMediaManage (CMediaCall*& pEnd)
		{
			if (0 == (long)m_arrMediaManage.GetCount())
				return NULL;

			double dTime		=	GetTime ();

			CMediaCall* pEffect	=	NULL;
			double dMaxTime		=	-1.0;

			double dEndTime		=	-1.0;
			for ( long i = 0; i < (long)m_arrMediaManage.GetCount(); ++i )
			{
				if (m_arrMediaManage[i]->GetBeginTime () < dTime)
				{
					if (NULL == pEnd)
					{
						pEnd		=	m_arrMediaManage[i];
						dEndTime	=	pEnd->GetBeginTime ();
					}
					else
					{
						if (dEndTime < pEnd->GetEndTime ())
						{
							pEnd		=	m_arrMediaManage[i];
							dEndTime	=	pEnd->GetBeginTime ();
						}
					}
				}

				if ( m_arrMediaManage[i]->InEffect(dTime) )
				{
					if (NULL == pEffect)
					{
						pEffect		=	m_arrMediaManage[i];
						dMaxTime	=	pEffect->GetBeginTime ();	
					}
					else
					{
						if (dMaxTime < m_arrMediaManage[i]->GetBeginTime ())
						{
							pEffect		=	m_arrMediaManage[i];
							dMaxTime	=	pEffect->GetBeginTime ();	
						}
					}
				}
			}

			return pEffect;	
		}

	private:

		inline void Clear ()
		{
			for ( int i = 0; i < (int)m_Effects.GetCount (); ++i )
				RELEASEOBJECT ( m_Effects[i] );
		}

	private:

		double m_dTime;

		// objects

		CAtlArray <CEffect*> m_Effects;
		CAtlArray <CEmphasisEffect*> m_refEmpEffects;
		CAnimationsBuilder m_oBuilder;

		CAtlArray <CEffect*> m_arrComposes;

		// calculate objects

		CMotionComposer m_oMotionComposer;
		CFadeComposer	m_oFadeComposer;
		CScaleComposer	m_oScaleComposer;
		CRotateComposer	m_oRotateComposer;

		BOOL			m_bChangeColor;

		// for ISStream

		CAtlArray <CMediaCall*> m_arrMediaManage;
	};
}

namespace Animation
{
	class CCacheFile
	{
	public:
		CCacheFile()
		{
			m_nBufferSize	=	0;
			m_bInFile		=	FALSE;
		}

		virtual ~CCacheFile()
		{
			if (m_bInFile)
			{
				DeleteFile(m_sPath);
			}
		}

		inline void SetFileName (const CString& sPath)
		{
			m_sPath		=	sPath;
		}

		inline BOOL Status ()
		{
			return m_bInFile;
		}

		//
		inline BOOL DumpFrame (MediaCore::IAVSUncompressedVideoFrame* pImage, long nBufferSize)
		{
			if (pImage)
			{
				if (FALSE == m_bInFile)
				{
					if (m_sPath.GetLength())
					{
						CAtlFile oFile;

						if ( SUCCEEDED ( oFile.Create ( m_sPath, GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_NEW ) ) )
						{
							BYTE* pBuffer = NULL;
							pImage->get_Buffer(&pBuffer);

							if (pBuffer)
							{
								DWORD nFileBuff	=	0L;
								oFile.Write ( pBuffer, nBufferSize, &nFileBuff );
							}

							oFile.Close ();

							m_nBufferSize	=	nBufferSize;
							m_bInFile		=	TRUE;

							return TRUE;
						}
					}
				}
			}

			return FALSE;
		}

		inline BOOL ReadFileBuffer (BYTE* pBuffer)
		{
			if (m_sPath.GetLength())
			{
				CAtlFile oFile;
				if ( SUCCEEDED ( oFile.Create ( m_sPath, GENERIC_READ, FILE_SHARE_READ, OPEN_ALWAYS ) ) )
				{
					oFile.Read (pBuffer, m_nBufferSize);
					oFile.Close ();

					return TRUE;
				}
			}

			return FALSE;
		}

	private:

		CString			m_sPath;
		BOOL			m_bInFile;
		unsigned long	m_nBufferSize;
	};

	class CCacheImageFile : public CCacheFile
	{
	public:
		CCacheImageFile()
		{
			m_nWidth	=	0;
			m_nHeight	=	0;

			m_pImage	=	NULL;
		}

		~CCacheImageFile()
		{
			ClearMemory ();
		}


		inline BOOL Init (const CString& sPath, const CString& sFile)
		{
			LoadImageStudioImage (sFile);

			return Init (sPath, m_pImage);
		}

		inline BOOL Init (const CString& sPath, MediaCore::IAVSUncompressedVideoFrame* pFrame)
		{
			m_pImage	=	pFrame;

			if (m_pImage)
			{				
				m_pImage->get_Width (&m_nWidth);
				m_pImage->get_Height (&m_nHeight);

				if (m_nWidth > 1 && m_nHeight > 1)
				{
					CCacheFile::SetFileName (sPath);
					CCacheFile::DumpFrame (m_pImage, m_nWidth * m_nHeight * 4);

					return TRUE;
				}
			}

			return FALSE;
		}

		inline IUnknown* GetImage ()
		{
			if (NULL == m_pImage)
			{
				if (Status())
				{
					if ( SUCCEEDED ( CoCreateInstance ( __uuidof ( MediaCore::CAVSUncompressedVideoFrame ), NULL, CLSCTX_INPROC_SERVER, __uuidof ( MediaCore::IAVSUncompressedVideoFrame ), (void **)&m_pImage ) ) )
					{
						m_pImage->put_Width(m_nWidth);
						m_pImage->put_Height(m_nHeight);
						m_pImage->put_Stride(0, 4 * m_nWidth);

						m_pImage->put_AspectRatioX(m_nWidth);
						m_pImage->put_AspectRatioY(m_nHeight);

						m_pImage->put_ColorSpace (CSP_BGRA /*| CSP_VFLIP*/);
						m_pImage->AllocateBuffer(-1);

						BYTE* pBuffer = NULL;
						m_pImage->get_Buffer(&pBuffer);

						if (pBuffer)
						{
							if (ReadFileBuffer (pBuffer))
								return m_pImage;
						}

						RELEASEINTERFACE (m_pImage);
					}
				}
			}

			return m_pImage;
		}

		inline BOOL ClearMemory ()
		{
			RELEASEINTERFACE (m_pImage);

			return TRUE;
		}

	private:
		inline BOOL LoadImageStudioImage (const CString& filename)
		{
			ImageStudio::IImageTransforms* pTransforms = NULL;
			CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_INPROC, ImageStudio::IID_IImageTransforms, (void**)&pTransforms); 

			if (NULL == pTransforms)
				return FALSE;

			CStringW strXml = L"<ImageFile-LoadImage sourcepath='";
			strXml += CStringW(filename);
			strXml += L"'/>";

			VARIANT_BOOL vbRes = VARIANT_FALSE;
			BSTR bsXml = strXml.AllocSysString();
			pTransforms->SetXml(bsXml, &vbRes);
			pTransforms->Transform(&vbRes);

			if (vbRes  == VARIANT_TRUE)
			{
				VARIANT var;
				pTransforms->GetResult(0, &var);

				if (NULL != var.punkVal)
				{
					var.punkVal->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_pImage);
					RELEASEINTERFACE((var.punkVal));
					RELEASEINTERFACE (pTransforms);

					return TRUE;
				}
			}

			RELEASEINTERFACE (pTransforms);
			return FALSE;
		}

	private:

		long									m_nWidth;
		long									m_nHeight;

		MediaCore::IAVSUncompressedVideoFrame*	m_pImage;
	};

	class CCachedCommands
	{
	public:
		CCachedCommands ()
		{
			m_bComplete	=	FALSE;

			m_pSection	=	NULL;
			m_pImage	=	NULL;

			m_nWidth	=	0;
			m_nHeight	=	0;

			m_bInFile	=	FALSE;
		}

		~CCachedCommands ()
		{
			RELEASEOBJECT (m_pSection);
			RELEASEINTERFACE (m_pImage);
		}

		inline BOOL CreateImage (IAVSRenderer* pBaseCommander, const CDoubleRect& oBounds)
		{
			if (m_bComplete)
				return FALSE;

			m_bComplete			=	TRUE;
			m_oBounds			=	oBounds;
			m_pBaseCommander	=	pBaseCommander;

			if ( NULL == m_pImage )
			{
				m_dWidthMM		=	m_oBounds.GetWidth();
				m_dHeightMM		=	m_oBounds.GetHeight();

				double dDpiX	=	0.0;	pBaseCommander->get_DpiX ( &dDpiX );
				double dDpiY	=	0.0;	pBaseCommander->get_DpiY ( &dDpiY );

				double MMToInch =	25.4;

				m_nWidth		=	m_dWidthMM * dDpiX / MMToInch;
				m_nHeight		=	m_dHeightMM * dDpiY / MMToInch;

				if ( 0 == m_nWidth || 0 == m_nHeight )
					return FALSE;

				m_pSection	=	new NSDocumentViewer::CDIB ();
				if (m_pSection)
					m_pSection->Create(m_nWidth,m_nHeight);
			}

			return TRUE;
		}

		inline IAVSRenderer* GetRender ()
		{
			IAVSRenderer* pCommander	=	NULL;

			if ( SUCCEEDED ( CoCreateInstance ( __uuidof ( CAVSGraphicsRenderer ), NULL, CLSCTX_ALL, __uuidof ( IAVSRenderer ), (void**)&pCommander ) ) )
			{
				if ( pCommander )
				{
					CAVSGraphicsRenderer* pCCommander = static_cast <CAVSGraphicsRenderer*> ( pCommander );
					if ( pCCommander && m_pSection )
					{
						NSDocumentViewer::CDoubleRect oRect;

						oRect.left		=	0;
						oRect.top		=	0;
						oRect.right		=	m_nWidth;
						oRect.bottom	=	m_nHeight;

						pCCommander->m_oRenderer.Create ( m_pSection->m_pBits, oRect, m_nWidth, m_nHeight, m_pSection );

						pCommander->put_Width((float)m_dWidthMM);
						pCommander->put_Height((float)m_dHeightMM);

						VARIANT var;
						m_pBaseCommander->GetAdditionalParam ( L"FontManager", &var );

						if ( var.vt	== VT_UNKNOWN && NULL != var.punkVal )
						{
							pCommander->SetAdditionalParam ( L"FontManager", var );
						}

						/*

						RELEASEOBJECT ( m_pImageCache );
						m_pImageCache		=	new CImageFilesCache ();

						pGrRender->SetImageCache ( m_pImageCache );

						*/
					}
				}
			}

			return pCommander;
		}

		inline BOOL Refresh ()
		{
			if ( SUCCEEDED ( CoCreateInstance ( __uuidof ( MediaCore::CAVSUncompressedVideoFrame ), NULL, CLSCTX_INPROC_SERVER, __uuidof ( MediaCore::IAVSUncompressedVideoFrame ), (void **)&m_pImage ) ) )
			{
				if (m_pSection)
				{
					m_pImage->put_Width(m_nWidth);
					m_pImage->put_Height(m_nHeight);
					m_pImage->put_Stride(0, 4 * m_nWidth);

					m_pImage->put_AspectRatioX(m_nWidth);
					m_pImage->put_AspectRatioY(m_nHeight);

					m_pImage->put_ColorSpace ( (1<< 6) /* | (1<<31) */ );
					m_pImage->AllocateBuffer(-1);

					BYTE* pBuffer = NULL;
					m_pImage->get_Buffer(&pBuffer);
#ifdef _DEBUG
					//memset(pBuffer, 0x7F, 4 * m_nWidth * m_nHeight);
#endif
					long lStride		= 0; m_pImage->get_Stride(0, &lStride);
					if (lStride < 0)
					{
						LONG lSize = 4 * m_nWidth * m_nHeight;
						memcpy(pBuffer, m_pSection->m_pBits, lSize);
					}
					else
					{
						BYTE* pSrc = m_pSection->m_pBits;
						LONG lSize = 4 * m_nWidth;
						BYTE* pDst = pBuffer + lSize * (m_nHeight - 1);

						for ( int i = 0; i < m_nHeight; ++i )
						{
							memcpy(pDst, pSrc, lSize);
							pSrc += lSize;
							pDst -= lSize;
						}
					}

					RELEASEOBJECT (m_pSection);
				}
			}

			return TRUE;
		}

		inline MediaCore::IAVSUncompressedVideoFrame* GetImage ()
		{
			LoadFromFile ();

			return m_pImage;
		}

		//
		inline CCachedCommands* Copy ()
		{
			CCachedCommands* pCopy	=	new CCachedCommands ();
			if (pCopy)
			{
				//
				pCopy->m_nWidth		=	m_nWidth;
				pCopy->m_nHeight	=	m_nHeight;
				pCopy->m_dWidthMM	=	m_dWidthMM;
				pCopy->m_dHeightMM	=	m_dHeightMM;
				pCopy->m_oBounds	=	m_oBounds;
				pCopy->m_bComplete	=	m_bComplete;

				if ( SUCCEEDED (m_pImage->CreateDuplicate (2, (MediaCore::IAVSMediaData**)(&pCopy->m_pImage) )) )
					return pCopy;

				RELEASEOBJECT (pCopy);
			}

			return NULL;
		}


		//
		inline void SetFileName (const CString& sPath)
		{
			m_oFile.SetFileName (sPath);
		}

		inline BOOL MoveToFile ()
		{
			if (FALSE == m_oFile.Status())
			{
				if (m_pImage)
				{
					m_oFile.DumpFrame (m_pImage, 4 * m_nWidth * m_nHeight);	//	если раньше было сохранение в файл, то дальше будет освобождение памяти

					RELEASEINTERFACE (m_pImage);
					m_bInFile	=	TRUE;
				}
			}

			return FALSE;
		}

	private:
		inline BOOL LoadFromFile ()
		{
			if (NULL == m_pImage)
			{
				if (m_oFile.Status())
				{
					if ( SUCCEEDED ( CoCreateInstance ( __uuidof ( MediaCore::CAVSUncompressedVideoFrame ), NULL, CLSCTX_INPROC_SERVER, __uuidof ( MediaCore::IAVSUncompressedVideoFrame ), (void **)&m_pImage ) ) )
					{
						m_pImage->put_Width(m_nWidth);
						m_pImage->put_Height(m_nHeight);
						m_pImage->put_Stride(0, 4 * m_nWidth);

						m_pImage->put_AspectRatioX(m_nWidth);
						m_pImage->put_AspectRatioY(m_nHeight);

						m_pImage->put_ColorSpace ( (1<< 6) );
						m_pImage->AllocateBuffer(-1);

						BYTE* pBuffer = NULL;
						m_pImage->get_Buffer(&pBuffer);

						if (pBuffer)
							return m_oFile.ReadFileBuffer (pBuffer);

						RELEASEINTERFACE (m_pImage);
					}
				}
			}

			return FALSE;
		}

	private:

		BOOL				m_bComplete;

		IAVSRenderer*		m_pBaseCommander;

		int					m_nWidth;
		int					m_nHeight;
		double				m_dWidthMM;
		double				m_dHeightMM;

		MediaCore::IAVSUncompressedVideoFrame*	m_pImage;
		NSDocumentViewer::CDIB*					m_pSection;

		CDoubleRect			m_oBounds;

		BOOL				m_bInFile;
		CCacheFile			m_oFile;
	};

	class CCacheStorage
	{
	public:
		CCacheStorage ()
		{
			m_nMaxImages	=	50;
			m_nCacheSize	=	0;

			m_nIndexer		=	0;
		}

		~CCacheStorage ()
		{

		}

		inline void AddRef (CCachedCommands* pRef)
		{
			if (pRef)
			{
				m_arrRefImages.Add (pRef);

				CreateTempDir ();

				CString sFilePath;
				sFilePath.Format(_T("%s\\%s_%d.buf"), m_sPath, m_UID, m_nIndexer);
				pRef->SetFileName (sFilePath);

				++m_nIndexer;
			}
		}

		inline CString GenTempFilePath ()
		{
			CreateTempDir ();

			CString sFilePath;
			sFilePath.Format(_T("%s\\%s_%d.buf"), m_sPath, m_UID, m_nIndexer);

			++m_nIndexer;

			return sFilePath;
		}

	private:

		inline void Update ()
		{
			if (m_nCacheSize > 50)
			{
				if ((long)m_arrRefImages.GetCount ())
				{
					m_arrRefImages[0]->MoveToFile();
				}
			}
		}

		inline BOOL CreateTempDir ()
		{
			if ( 0 == m_sPath.GetLength () )
			{
				CString sTempPath;
				GetTempPath (4096, sTempPath.GetBuffer(4096));
				sTempPath.ReleaseBuffer();

				m_sPath		=	sTempPath;

				GUID* guid	=	new GUID (); 
				if (guid)
				{
					CoCreateGuid (guid);
					StringFromGUID2 (*guid, m_UID.GetBuffer(512), 512);
					m_UID.ReleaseBuffer();

					RELEASEOBJECT(guid);

					// ATLTRACE (_T("%s\n"),m_UID);

					CString sFilePath;
					sFilePath.Format(_T("%s\\%s"), m_sPath, m_UID);

					if ( ERROR_ALREADY_EXISTS != CreateDirectory ( sFilePath, NULL ) )
					{
						m_sPath.Format(_T("%s\\%s\\"), sTempPath, m_UID);
						m_UID	=	_T("");
					}
				}
			}

			return TRUE;
		}

	protected:

		long	m_nMaxImages;
		CString	m_sPath;

		long	m_nIndexer;
		CString m_UID;

		long	m_nCacheSize;
		CAtlArray<CCachedCommands*>	m_arrRefImages;
	};
}

namespace Animation
{
	class IShowElement
	{
	public:
		static const long ElementNone	=	0;
		static const long ElementPath	=	1;
		static const long ElementImage	=	2;
		static const long ElementText	=	3;
		static const long ElementStream	=	4;

	public:

		IShowElement ()
		{
			m_nType				=	ElementNone;

			m_pLayer			=	NULL;
			m_pCache			=	NULL;

			m_dTimeBegin		=	0.0;
			m_dTimeEnd			=	0.0;

			m_bInternalCache	=	TRUE;
		}

		virtual ~IShowElement()
		{

		}
		//
		virtual void FromXml (XmlUtils::CXmlNode& oXmlNode)	=	0;
		virtual void Render (IAVSRenderer* pCommander)		=	0;
		virtual const CDoubleRect& GetBounds ()				=	0;	
		virtual void ClearCache ()							=	0;
		//
		inline ILayer* GetBaseLayer ()	
		{
			return m_pLayer;
		};
		inline void SetBaseLayer (ILayer* pLayer)	
		{
			m_pLayer	=	pLayer;
		}
		inline void SetInternalCache (BOOL bInternal)
		{
			m_bInternalCache	=	bInternal;
		}

		inline const CDoubleRect& GetBaseBounds()		
		{
			return m_oBaseBounds;
		}

		inline long GetType ()	
		{
			return m_nType;
		}

		//
		inline BOOL IsStatic ()
		{
			return (NULL == m_pLayer);
		}

		virtual double GetAngle ()	
		{
			return 0.0;	
		}
		inline void SetTimes (double dBegin, double dEnd)
		{
			m_dTimeBegin	=	dBegin;
			m_dTimeEnd		=	dEnd;
		}
		inline void SetBaseBounds (const CDoubleRect& oBounds)
		{
			m_oBaseBounds	=	oBounds;
		}

		inline double GetBeginTime () 
		{
			return m_dTimeBegin;
		}
		inline double GetEndTime () 
		{
			return m_dTimeEnd;
		}

		//
		virtual void SetCacheReferences (CCacheStorage* pCache)
		{
			m_pCache	=	pCache;
		}

		virtual IUnknown* GetBufferImage ()
		{
			return NULL;
		}

	protected:

		long			m_nType;

		ILayer*			m_pLayer;

		double			m_dTimeBegin;
		double			m_dTimeEnd;

		CCacheStorage*	m_pCache;

		BOOL			m_bInternalCache;
		CDoubleRect		m_oBaseBounds;		//	FOR TEXT ONLY
	};

	class CCoordTransformer
	{
	public:
		class BoundRotate
		{
		public:
			inline static CDoubleRect Transform ( const CDoubleRect& oBounds, const double& dAngle )
			{
				CDoubleRect bounds;

				double d1 = oBounds.GetWidth ();
				double d2 = oBounds.GetHeight ();

				Aggplus::CMatrix oRightTransform;
				oRightTransform.Rotate (dAngle);
				oRightTransform.Translate (oBounds.GetWidth () * 0.5, oBounds.GetHeight () * 0.5);

				Aggplus::CMatrix oLeftTransform;
				oLeftTransform.Rotate (dAngle);
				oLeftTransform.Translate (-oBounds.GetWidth () * 0.5, oBounds.GetHeight () * 0.5);

				double OffsetX	=	max ( fabs(oLeftTransform.OffsetX()), fabs(oRightTransform.OffsetX()) );
				double OffsetY	=	max ( fabs(oLeftTransform.OffsetY()), fabs(oRightTransform.OffsetY()) );

				double dCenX	=	(oBounds.left + oBounds.right) * 0.5;
				double dCenY	=	(oBounds.top + oBounds.bottom) * 0.5;

				bounds.left		=	dCenX - OffsetX;
				bounds.top		=	dCenY - OffsetY;
				bounds.right	=	dCenX + OffsetX;
				bounds.bottom	=	dCenY + OffsetY;

				return bounds;
			}	
		};

	public:

		CCoordTransformer (double dWidthMM, double dHeightMM, long nFlags = 0) : m_dWidthMM ( dWidthMM ), m_dHeightMM ( dHeightMM )
		{
			m_pLayer		=	NULL;
			m_dAngle		=	0.0;

			BOOL bFlipX		=	(0 != (c_nParamFlipX & nFlags));
			BOOL bFlipY		=	(0 != (c_nParamFlipY & nFlags));

			REAL m11		=	bFlipX ? -1.0f : 1.0f;
			REAL m22		=	bFlipY ? -1.0f : 1.0f;

			m_oFlip.Scale(m11, m22);
		}

		inline BOOL ProcessFadeEffect (CEffect* pEffect, IShowElement* pElement, IAVSRenderer* pRenderer)
		{
			if (NULL == pElement)
				return FALSE;

			m_pElement			=	pElement;
			m_pLayer			=	m_pElement->GetBaseLayer ();

			if ( NULL == m_pLayer )
				return FALSE;

			m_dAngle			=	pElement->GetAngle ();
			m_oBounds			=	pElement->GetBounds ();	
			m_oCenter			=	Animation::CPoint(( m_oBounds.left + m_oBounds.right ) * 0.5, ( m_oBounds.top + m_oBounds.bottom ) * 0.5 );

			m_pLayer->GetTransform ( m_oMove );

			m_oTransform		=	Aggplus::CMatrix(); 

			int nEffectID		=	pEffect->GetID ();

			switch ( nEffectID )
			{
			case AppearEffect :			if ( Appear	( pRenderer, pEffect ) )		return TRUE;
				break;
			case FadedZoomEffect :		if ( FadeZoom ( pRenderer, pEffect ) )		return TRUE;	
				break;
			case GrowAndTurnEffect :	if ( GrowAndTurn ( pRenderer, pEffect ) )	return TRUE;
				break;
			case FlyInEffect :			if ( FlyIn ( pRenderer, pEffect ) )			return TRUE;	
				break;
			case ZoomEffect :			if ( Zoom ( pRenderer, pEffect ) )			return TRUE;
				break;
			case DescendEffect :		if ( Descend ( pRenderer, pEffect ) )		return TRUE;
				break;
			case AscendEffect :			if ( Ascend ( pRenderer, pEffect ) )		return TRUE;
				break;
			case SpinnerEffect :		if ( Spinner ( pRenderer, pEffect ) )		return TRUE;
				break;
			case PinWheelEffect :		if ( PinWheel ( pRenderer, pEffect ) )		return TRUE;
				break;
			case CreditsEffect :		if ( Credits ( pRenderer, pEffect ) )		return TRUE;
				break;
			case RiseUpEffect :			if ( RiseUp ( pRenderer, pEffect ) )		return TRUE;
				break;
			case CenterRevolveEffect :	if ( CenterRevolve ( pRenderer, pEffect ) )	return TRUE;
				break;
			case ArcUpEffect :			if ( ArcUp ( pRenderer, pEffect ) )			return TRUE;
				break;
			case SpiralEffect :			if ( Spiral ( pRenderer, pEffect ) )		return TRUE;
				break;
			case FadedSwivelEffect :	if ( FadedSwivel ( pRenderer, pEffect ) )	return TRUE;
				break;
			case ExpandEffect :			if ( Expand ( pRenderer, pEffect ) )		return TRUE;
				break; 
			case FloatEffect :			if ( Float ( pRenderer, pEffect ) )			return TRUE;
				break;
			case SwivelEffect :			if ( Swivel ( pRenderer, pEffect ) )		return TRUE;
				break; 
			case BoomerangEffect :		if ( Boomerang ( pRenderer, pEffect ) )		return TRUE;
				break; 
			case BounceEffect :			if ( Bounce ( pRenderer, pEffect ) )		return TRUE;
				break; 

			default : 
				break;
			}

			return FALSE;
		}


		//
		inline BOOL Rotate (IShowElement* pElement, IAVSRenderer* pCommander, long nFlags)
		{
			if (pElement && pCommander)
			{
				m_oBounds	=	pElement->GetBounds ();	
				m_oCenter	=	Animation::CPoint(( m_oBounds.left + m_oBounds.right ) * 0.5, ( m_oBounds.top + m_oBounds.bottom ) * 0.5 );

				Aggplus::CMatrix oTransform		=	GetRotateTransform ( pElement->GetAngle(), nFlags );

				double matrix [ 6 ];
				oTransform.GetElements (matrix);
				pCommander->SetTransform (matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5]);

				return TRUE;
			}

			return FALSE;
		}

		inline BOOL StaticTransform (IShowElement* pElement, IAVSRenderer* pCommander2, BOOL bAngleTransform = TRUE, float dMargin = 4.0)
		{
			if ( pElement && pCommander2 )
			{
				m_dAngle					=	pElement->GetAngle ();
				m_oBounds					=	pElement->GetBounds ();	
				m_oCenter					=	Animation::CPoint(( m_oBounds.left + m_oBounds.right ) * 0.5, ( m_oBounds.top + m_oBounds.bottom ) * 0.5 );

				CDoubleRect	oMarginBounds	=	CCoordTransformer::MarginBounds (m_oBounds, dMargin);
				if (bAngleTransform)
					oMarginBounds			=	BoundRotate::Transform (oMarginBounds, m_dAngle);

				Aggplus::CMatrix oAffineTrans;
				oAffineTrans.Translate (-oMarginBounds.left, -oMarginBounds.top);

				if (bAngleTransform)
					oAffineTrans.Multiply ( &GetRotateTransform (m_dAngle) );

				double matrix [ 6 ];
				oAffineTrans.GetElements (matrix);
				pCommander2->SetTransform (matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5]);

				return TRUE;
			}

			return FALSE;
		}


		inline Aggplus::CMatrix GetRotate (IShowElement* pElement)
		{
			if (pElement)
			{
				m_oBounds	=	pElement->GetBounds ();	
				m_oCenter	=	Animation::CPoint(( m_oBounds.left + m_oBounds.right ) * 0.5, ( m_oBounds.top + m_oBounds.bottom ) * 0.5 );

				return GetRotateTransform ( pElement->GetAngle () );
			}

			return Aggplus::CMatrix();
		}

		inline Aggplus::CMatrix GetInnerTransform (IShowElement* pElement)	//	преобразования над элементом которые сохраняются на все время отрисовки
		{
			if (pElement)
			{
				m_pLayer	=	pElement->GetBaseLayer ();
				m_oBounds	=	pElement->GetBounds ();	
				m_oCenter	=	Animation::CPoint(( m_oBounds.left + m_oBounds.right ) * 0.5, ( m_oBounds.top + m_oBounds.bottom ) * 0.5 );

				Animation::CPoint pScale = m_pLayer->GetScale (m_pLayer->GetTime());

				Aggplus::CMatrix oTransform;

				double dSpinAngle	=	m_pLayer->GetEffectAngle (m_pLayer->GetTime());

				Aggplus::CMatrix oTranslateOff;		oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
				Aggplus::CMatrix oRotate;			oRotate.Rotate (pElement->GetAngle () + dSpinAngle);
				Aggplus::CMatrix oScale;			oScale.Scale (pScale.X, pScale.Y);
				Aggplus::CMatrix oTranslateOn;		oTranslateOn.Translate ( m_oCenter.X, m_oCenter.Y );

				oTransform.Multiply ( &oTranslateOn );
				oTransform.Multiply ( &oRotate );
				oTransform.Multiply ( &oScale );
				oTransform.Multiply (&m_oFlip);
				oTransform.Multiply ( &oTranslateOff );

				return oTransform;
			}

			return Aggplus::CMatrix();
		}

		inline const Aggplus::CMatrix& GetTransform ()
		{
			return m_oTransform;
		}
		
		inline static BOOL FlipTransform (IShowElement* pElement, IAVSRenderer* pCommander, long nFlags)
		{
			if ((NULL!=pElement) && (NULL!=pCommander))
			{
				BOOL bFlipX	=	(0 != (c_nParamFlipX & nFlags));
				BOOL bFlipY	=	(0 != (c_nParamFlipY & nFlags));

				REAL m11	=	bFlipX ? -1.0f : 1.0f;
				REAL m22	=	bFlipY ? -1.0f : 1.0f;

				Aggplus::CMatrix oMatrix(1, 0, 0, 1, 0, 0);

				const CDoubleRect& oBounds = pElement->GetBounds();

				double dCentreX = ( oBounds.left + oBounds.right ) * 0.5;
				double dCentreY = ( oBounds.top  + oBounds.bottom ) * 0.5;

				oMatrix.Translate(-dCentreX, -dCentreY, Aggplus::MatrixOrderAppend);
				oMatrix.Scale(m11, m22, Aggplus::MatrixOrderAppend);
				oMatrix.Translate(dCentreX, dCentreY, Aggplus::MatrixOrderAppend);
				
				double dMatrix [ 6 ];	oMatrix.GetElements (dMatrix);
				pCommander->SetTransform (dMatrix[0], dMatrix[1], dMatrix[2], dMatrix[3], dMatrix[4], dMatrix[5]);

				return TRUE;
			}

			return FALSE;
		}	

		//
		static inline CDoubleRect MarginBounds (const CDoubleRect& oBounds, float dMargin = 4.0)
		{
			CDoubleRect	oMarginBounds		=	oBounds;

			oMarginBounds.bottom			+=	dMargin;
			oMarginBounds.left				-=	dMargin;
			oMarginBounds.top				-=	dMargin;
			oMarginBounds.right				+=	dMargin;

			//oMarginBounds.bottom			=	(long)oMarginBounds.bottom;
			//oMarginBounds.left			=	(long)oMarginBounds.left;
			//oMarginBounds.top				=	(long)(oMarginBounds.top+ 0.5);
			//oMarginBounds.right			=	(long)(oMarginBounds.right + 0.5);

			return oMarginBounds;
		}


	protected:
		inline BOOL Appear			( IAVSRenderer* pRenderer, CEffect* pEffect )
		{

			return TRUE;
		}

		inline BOOL FadeZoom		( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			double dComplete	=	GetCompleteness ( pEffect );	

			int nDirection		=	pEffect->GetDirection ();

			if ( 0x210 == nDirection )	//	OFFICE 2010 - OFF CENTER
			{							
				Aggplus::CMatrix oTranslateOff;		oTranslateOff.Translate ( - (m_oBounds.left + m_oBounds.right ) * 0.5, - (m_oBounds.top + m_oBounds.bottom ) * 0.5 );
				Aggplus::CMatrix oScale;			oScale.Scale ( dComplete, dComplete );
				Aggplus::CMatrix oTranslateOn;		oTranslateOn.Translate ( 
					m_dWidthMM * 0.5  - ( m_dWidthMM * 0.5  - ( m_oBounds.left + m_oBounds.right ) * 0.5 ) * dComplete,
					m_dHeightMM * 0.5 - ( m_dHeightMM * 0.5 - ( m_oBounds.top + m_oBounds.bottom ) * 0.5 ) * dComplete );

				m_oTransform.Multiply ( &oTranslateOn );
				m_oTransform.Multiply ( &oScale );
				m_oTransform.Multiply ( &oTranslateOff );
			}
			else
			{
				Aggplus::CMatrix oTranslateOff;		oTranslateOff.Translate ( - (m_oBounds.left + m_oBounds.right ) * 0.5, - (m_oBounds.top + m_oBounds.bottom ) * 0.5 );
				Aggplus::CMatrix oScale;			oScale.Scale ( dComplete, dComplete );
				Aggplus::CMatrix oTranslateOn;		oTranslateOn.Translate ( + (m_oBounds.left + m_oBounds.right ) * 0.5, + (m_oBounds.top + m_oBounds.bottom ) * 0.5 );

				m_oTransform.Multiply ( &oTranslateOn );
				m_oTransform.Multiply ( &oScale );
				m_oTransform.Multiply ( &oTranslateOff );
			}

			return TRUE;
		}

		inline BOOL GrowAndTurn		( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			double dComplete	=	GetCompleteness ( pEffect );	

			Aggplus::CMatrix oTranslateOff;		oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
			Aggplus::CMatrix oScale;			oScale.Scale ( dComplete, dComplete );
			Aggplus::CMatrix oRotate;			oRotate.Rotate ( 90 * ( 1.0 - dComplete ) );
			Aggplus::CMatrix oTranslateOn;		oTranslateOn.Translate ( m_oCenter.X, m_oCenter.Y );

			Aggplus::CMatrix oTranslateEffect;	oTranslateEffect.Translate ( m_oMove.X * m_dWidthMM, m_oMove.Y * m_dHeightMM );

			m_oTransform.Multiply ( &oTranslateOn );
			m_oTransform.Multiply ( &oScale );
			m_oTransform.Multiply ( &oRotate );
			m_oTransform.Multiply ( &oTranslateOff );

			return TRUE;
		}

		inline BOOL FlyIn			( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			double dComplete	=	GetCompleteness ( pEffect );	
			int nDirection		=	pEffect->GetDirection ();

			if (m_pElement)
			{
				m_oBounds		=	m_pElement->GetBaseBounds();
			}

			Aggplus::CMatrix oTranslateOff;	oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
			Aggplus::CMatrix oTranslateOn;	

			if ( 8 == nDirection )			//	Strafe To Left	
			{
				oTranslateOn.Translate ( m_oCenter.X + ( - m_oCenter.X - m_oBounds.GetWidth () ) * ( 1.0 - dComplete ),
					m_oCenter.Y );
			}
			else if ( 2 == nDirection )		//	Strafe To Right	
			{
				oTranslateOn.Translate ( m_oCenter.X + ( m_dWidthMM - m_oCenter.X + m_oBounds.GetWidth() ) * ( 1.0 - dComplete ),
					m_oCenter.Y );
			}
			else if ( 1 == nDirection )		//	Move Down	
			{
				oTranslateOn.Translate ( m_oCenter.X, 
					m_oCenter.Y + ( - m_oCenter.Y - m_oBounds.GetHeight() ) * ( 1.0 - dComplete ) );
			} 
			else if ( 4 == nDirection )		//	Move Up	
			{
				oTranslateOn.Translate ( m_oCenter.X, 
					m_oCenter.Y + ( m_dHeightMM - m_oCenter.Y + m_oBounds.GetHeight() ) * ( 1.0 - dComplete ) );
			}
			else if ( 3 == nDirection )		//		
			{
				oTranslateOn.Translate ( m_oCenter.X + ( m_dWidthMM - m_oCenter.X + m_oBounds.GetWidth() ) * ( 1.0 - dComplete ),
					m_oCenter.Y + ( - m_oCenter.Y - m_oBounds.GetHeight()  ) * ( 1.0 - dComplete ) );
			}
			else if ( 6 == nDirection )		//	Up Right	
			{
				oTranslateOn.Translate ( m_oCenter.X + ( m_dWidthMM - m_oCenter.X + m_oBounds.GetWidth() * 0.5 ) * ( 1.0 - dComplete ),
					m_oCenter.Y + ( m_dHeightMM - m_oCenter.Y + m_oBounds.GetHeight() ) * ( 1.0 - dComplete ) );
			}
			else if ( 9 == nDirection )		//		
			{
				oTranslateOn.Translate ( m_oCenter.X + ( - m_oCenter.X - m_oBounds.GetWidth () * 0.5 ) * ( 1.0 - dComplete ),
					m_oCenter.Y + ( - m_oCenter.Y - m_oBounds.GetHeight() ) * ( 1.0 - dComplete ) );
			}
			else if ( 12 == nDirection )		//	Up Right	
			{
				oTranslateOn.Translate ( m_oCenter.X + ( - m_oCenter.X - m_oBounds.GetWidth () * 0.5 ) * ( 1.0 - dComplete ),
					m_oCenter.Y + ( m_dHeightMM - m_oCenter.Y + m_oBounds.GetHeight() ) * ( 1.0 - dComplete ) );
			}
			else
			{
				oTranslateOn.Translate ( m_oCenter.X, m_oCenter.Y );
			}

			m_oTransform.Multiply ( &oTranslateOn );
			m_oTransform.Multiply ( &oTranslateOff );

			return TRUE;
		}

		inline BOOL Zoom			( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			double dComplete	=	GetCompleteness ( pEffect );	
			int nDirection		=	pEffect->GetDirection ();

			Aggplus::CMatrix oTranslateOff;	
			Aggplus::CMatrix oScale;			
			Aggplus::CMatrix oTranslateOn;		

			oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );

			if ( 0x210 == nDirection )		//	IN CENTER
			{							
				oScale.Scale ( dComplete, dComplete );
				oTranslateOn.Translate ( m_dWidthMM * 0.5  - ( m_dWidthMM * 0.5  - m_oCenter.X ) * dComplete, m_dHeightMM * 0.5 - ( m_dHeightMM * 0.5 - m_oCenter.Y ) * dComplete );
			}
			else if ( 0x10 == nDirection )	//	IN
			{
				if ( FadeExitType == pEffect->GetInnerType () )
				{
					oScale.Scale ( ( 1.0 - dComplete ) * 3.0 + 1.0, ( 1.0 - dComplete ) * 3.0 + 1.0 );
					oTranslateOn.Translate ( m_oCenter.X, m_oCenter.Y );
				}
				else
				{
					oScale.Scale ( dComplete, dComplete );
					oTranslateOn.Translate ( m_oCenter.X, m_oCenter.Y );
				}
			}
			else if ( 0x110 == nDirection )	//	IN SLIGHTLY
			{
				oScale.Scale ( 0.5 + dComplete * 0.5,  0.5 + dComplete * 0.5 );
				oTranslateOn.Translate ( m_oCenter.X, m_oCenter.Y );
			}
			else if ( 0x20 == nDirection )	//	OUT
			{
				if ( FadeExitType == pEffect->GetInnerType () )
				{
					oScale.Scale ( dComplete, dComplete );
					oTranslateOn.Translate ( m_oCenter.X, m_oCenter.Y );
				}
				else
				{
					oScale.Scale ( ( 1.0 - dComplete ) * 3.0 + 1.0, ( 1.0 - dComplete ) * 3.0 + 1.0 );
					oTranslateOn.Translate ( m_oCenter.X, m_oCenter.Y );
				}
			}
			else if ( 0x120 == nDirection )	//	OUT SLIGHTLY
			{
				oScale.Scale ( ( 1.0 - dComplete ) * 0.25 + 1.0, ( 1.0 - dComplete ) * 0.25 + 1.0 );
				oTranslateOn.Translate ( m_oCenter.X, m_oCenter.Y );
			}
			else if ( 0x24 == nDirection )	//	OUT BOTTOM
			{
				double ScaleFactor	=	12.0;

				oScale.Scale ( ( 1.0 - dComplete ) * ScaleFactor + 1.0, ( 1.0 - dComplete ) * ScaleFactor + 1.0 );
				oTranslateOn.Translate ( 
					m_oCenter.X + ( m_dWidthMM * 0.5 - m_oCenter.X + m_oBounds.GetWidth() * ScaleFactor * ( 1.0 - dComplete ) * 0.5 ) * ( 1.0 - dComplete ),
					m_oCenter.Y + ( m_dHeightMM - m_oCenter.Y + m_oBounds.GetHeight() * ScaleFactor ) * ( 1.0 - dComplete ) );
			}
			else 
			{
				oScale.Scale ( dComplete, dComplete );
				oTranslateOn.Translate ( m_oCenter.X, m_oCenter.Y );
			}

			m_oTransform.Multiply ( &oTranslateOn );
			m_oTransform.Multiply ( &oScale );
			m_oTransform.Multiply ( &oTranslateOff );

			return TRUE;
		}

		inline BOOL Descend			( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			static const double DESCEND_MOVE_Y_PERCENT	=	0.1;	//	#ppt_y-.1

			double dComplete	=	GetCompleteness ( pEffect );	

			Aggplus::CMatrix oTranslateOff;	
			Aggplus::CMatrix oTranslateOn;		

			oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
			oTranslateOn.Translate ( m_oCenter.X, m_oCenter.Y - ( m_dHeightMM * DESCEND_MOVE_Y_PERCENT ) * ( 1.0 - dComplete ) );

			m_oTransform.Multiply ( &oTranslateOn );
			m_oTransform.Multiply ( &oTranslateOff );

			return TRUE;
		}

		inline BOOL Ascend			( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			static const double ASCEND_MOVE_Y_PERCENT	=	0.1;	//	#ppt_y+.1

			double dComplete	=	GetCompleteness ( pEffect );	

			Aggplus::CMatrix oTranslateOff;	
			Aggplus::CMatrix oTranslateOn;		

			oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
			oTranslateOn.Translate ( m_oCenter.X, m_oCenter.Y + ( m_dHeightMM * ASCEND_MOVE_Y_PERCENT ) * ( 1.0 - dComplete ) );

			m_oTransform.Multiply ( &oTranslateOn );
			m_oTransform.Multiply ( &oTranslateOff );

			return TRUE;
		}

		inline BOOL Spinner			( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			double dComplete	=	GetCompleteness ( pEffect );	

			Aggplus::CMatrix oTranslateOff;		oTranslateOff.Translate ( - (m_oBounds.left + m_oBounds.right ) * 0.5, - (m_oBounds.top + m_oBounds.bottom ) * 0.5 );
			Aggplus::CMatrix oScale;			oScale.Scale ( dComplete, dComplete );
			Aggplus::CMatrix oRotate;			oRotate.Rotate ( 360.0 * (1.0-dComplete) * (1.0-dComplete) ); // * ( 1.0 - dComplete )) );
			Aggplus::CMatrix oTranslateOn;		oTranslateOn.Translate ( + (m_oBounds.left + m_oBounds.right ) * 0.5, + (m_oBounds.top + m_oBounds.bottom ) * 0.5 );

			Aggplus::CMatrix oTranslateEffect;	oTranslateEffect.Translate ( m_oMove.X * m_dWidthMM, m_oMove.Y * m_dHeightMM );

			m_oTransform.Multiply ( &oTranslateOn );
			m_oTransform.Multiply ( &oScale );
			m_oTransform.Multiply ( &oRotate );
			m_oTransform.Multiply ( &oTranslateOff );

			return TRUE;
		}

		inline BOOL PinWheel		( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			double dComplete	=	GetCompleteness ( pEffect );	

			Aggplus::CMatrix oTranslateOff;		oTranslateOff.Translate ( - (m_oBounds.left + m_oBounds.right ) * 0.5, - (m_oBounds.top + m_oBounds.bottom ) * 0.5 );
			Aggplus::CMatrix oScale;			oScale.Scale ( dComplete, dComplete );
			Aggplus::CMatrix oRotate;			oRotate.Rotate ( 360.0 * (( 1.0 - dComplete )) * 2.0 );
			Aggplus::CMatrix oTranslateOn;		oTranslateOn.Translate ( + (m_oBounds.left + m_oBounds.right ) * 0.5, + (m_oBounds.top + m_oBounds.bottom ) * 0.5 );

			Aggplus::CMatrix oTranslateEffect;	oTranslateEffect.Translate ( m_oMove.X * m_dWidthMM, m_oMove.Y * m_dHeightMM );

			m_oTransform.Multiply ( &oTranslateOn );
			m_oTransform.Multiply ( &oScale );
			m_oTransform.Multiply ( &oRotate );
			m_oTransform.Multiply ( &oTranslateOff );

			return TRUE;
		}

		inline BOOL Credits			( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			double dComplete	=	GetCompleteness ( pEffect );	

			if (m_pElement)
			{
				m_oBounds		=	m_pElement->GetBaseBounds();
			}

			Aggplus::CMatrix oTranslateOff;	oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
			Aggplus::CMatrix oTranslateOn;	

			oTranslateOn.Translate ( m_oCenter.X, - m_oBounds.GetHeight() + ( 2.0 * m_oBounds.GetHeight() + m_dHeightMM ) * ( 1.0 - dComplete ) );

			m_oTransform.Multiply ( &oTranslateOn );
			m_oTransform.Multiply ( &oTranslateOff );

			return TRUE;
		}		

		inline BOOL RiseUp			( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			static const double RISE_UP_MOVE_Y_TIME_FACTOR	=	0.03;

			double dComplete	=	GetCompleteness ( pEffect );	

			Aggplus::CMatrix oTranslateOff;	oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
			Aggplus::CMatrix oTranslateOn;	

			if ( dComplete < 0.9 )
			{
				oTranslateOn.Translate ( m_oCenter.X, 
					m_oCenter.Y - m_dHeightMM * RISE_UP_MOVE_Y_TIME_FACTOR + 
					( m_oBounds.GetHeight() * 0.5 + m_dHeightMM - m_oCenter.Y + m_dHeightMM * RISE_UP_MOVE_Y_TIME_FACTOR ) * ( 1.0 - dComplete / 0.9 ) );
			}
			else
			{
				oTranslateOn.Translate ( m_oCenter.X, m_oCenter.Y + ( - m_dHeightMM * RISE_UP_MOVE_Y_TIME_FACTOR ) * ( 1.0 - dComplete ) / 0.1 );	//	#ppt_y-.03
			}

			m_oTransform.Multiply ( &oTranslateOn );
			m_oTransform.Multiply ( &oTranslateOff );

			return TRUE;
		}		

		inline BOOL CenterRevolve	( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			static const double RISE_UP_MOVE_XY		=	0.31;
			double dTimeFactor	=	CalculateCompleteness ( pEffect );

			Aggplus::CMatrix oTranslateOff;	oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
			Aggplus::CMatrix oTranslateOn;	

			double dRadius		=	RISE_UP_MOVE_XY * m_dHeightMM * 0.5;

			dTimeFactor			=	max ( CENTER_REVOLVE_FADE_TIME_ZONE_IN, dTimeFactor );
			dTimeFactor			-=	CENTER_REVOLVE_FADE_TIME_ZONE_IN;
			dTimeFactor			/=	1.0 - CENTER_REVOLVE_FADE_TIME_ZONE_IN;

			double dComplete	=	dTimeFactor;	

			Aggplus::CMatrix mLocalTranslate;	mLocalTranslate.Translate ( 0, - dRadius );
			Aggplus::CMatrix mOffLocaleRotate;	mOffLocaleRotate.Rotate ( 180 * ( 1.0 - dComplete ) );
			Aggplus::CMatrix mLocalRotate;		mLocalRotate.Rotate ( -180 * ( 1.0 - dComplete ) );

			m_oTransform.Translate ( m_oCenter.X, m_oCenter.Y + dRadius );

			m_oTransform.Multiply ( &oTranslateOn );		//	обратное перемещение

			m_oTransform.Multiply ( &mOffLocaleRotate );	//	возмещаем поворот
			m_oTransform.Multiply ( &mLocalTranslate );		//	перемещение на радиус окружности поворота

			m_oTransform.Multiply ( &mLocalRotate );		//	зеркальный поворот

			m_oTransform.Multiply ( &oTranslateOff );		//	перенос в центр

			return TRUE;
		}
		inline BOOL ArcUp			( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			double dComplete	=	GetCompleteness ( pEffect );	
			int nDirection		=	pEffect->GetDirection ();

			m_oTransform		=	Aggplus::CMatrix(); 

			Aggplus::CMatrix oTranslateOff;	oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
			Aggplus::CMatrix oScale;		oScale.Scale ( 2.0 - dComplete, 2.0 - dComplete );
			Aggplus::CMatrix oTranslateOn;	oTranslateOn.Translate ( m_oCenter.X, m_oCenter.Y );

			m_oTransform.Multiply ( &oTranslateOn );
			m_oTransform.Multiply ( &oScale );
			m_oTransform.Multiply ( &oTranslateOff );

			return TRUE;
		}

		inline BOOL Spiral			( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			static const double __M_PI	=	3.14159265358979323846;

			double dComplete	=	GetCompleteness ( pEffect );	
			int nDirection		=	pEffect->GetDirection ();

			m_oTransform		=	Aggplus::CMatrix(); 

			Aggplus::CMatrix oTranslateOff;	oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
			Aggplus::CMatrix oScale;		oScale.Scale ( dComplete, dComplete );
			Aggplus::CMatrix oTranslateOn;	oTranslateOn.Translate ( m_oCenter.X, m_oCenter.Y );

			double angle	=	- 2.0 * __M_PI *( 1.0 - dComplete );
			double factor	=	( 1.0 - dComplete );

			//	<formula>#ppt_x+(cos(-2*pi*(1-$))*-#ppt_x-sin(-2*pi*(1-$))*(1-#ppt_y))*(1-$)</formula>
			//	<formula>#ppt_y+(sin(-2*pi*(1-$))*-#ppt_x+cos(-2*pi*(1-$))*(1-#ppt_y))*(1-$)</formula>

			Aggplus::CMatrix oSpiral;		oSpiral.Translate ( 
				( - m_oCenter.X * cos ( angle ) - sin ( angle ) * (m_dHeightMM - m_oCenter.Y) ) * ( 1.0 - dComplete ),
				( - m_oCenter.X * sin ( angle ) + cos ( angle ) * (m_dHeightMM - m_oCenter.Y) ) * ( 1.0 - dComplete ) );

			m_oTransform.Multiply ( &oTranslateOn );
			m_oTransform.Multiply ( &oSpiral );
			m_oTransform.Multiply ( &oScale );
			m_oTransform.Multiply ( &oTranslateOff );

			return TRUE;
		}		

		inline BOOL FadedSwivel		( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			static const double __M_PI	=	3.14159265358979323846;

			double dComplete			=	GetCompleteness ( pEffect );	

			Aggplus::CMatrix oTranslateOff;		oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
			Aggplus::CMatrix oScale;			oScale.Scale ( sin (2.5 * __M_PI * dComplete), 1.0 );	// #ppt_w*sin(2.5*pi*$)
			Aggplus::CMatrix oTranslateOn;		oTranslateOn.Translate ( m_oCenter.X, + m_oCenter.Y );

			m_oTransform.Multiply ( &oTranslateOn );
			m_oTransform.Multiply ( &oScale );
			m_oTransform.Multiply ( &oTranslateOff );

			return TRUE;
		}

		inline BOOL Expand			( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			static const double __M_PI	=	3.14159265358979323846;

			double dComplete			=	GetCompleteness ( pEffect );	

			Aggplus::CMatrix oTranslateOff;		oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
			Aggplus::CMatrix oScale;			oScale.Scale ( dComplete * 0.3 + 0.7, 1.0 );				//	#ppt_w*0.70 ~ #ppt_w
			Aggplus::CMatrix oTranslateOn;		oTranslateOn.Translate ( m_oCenter.X, + m_oCenter.Y );

			m_oTransform.Multiply ( &oTranslateOn );
			m_oTransform.Multiply ( &oScale );
			m_oTransform.Multiply ( &oTranslateOff );

			return TRUE;
		}

		inline BOOL Float			( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			double dComplete	=	CalculateCompleteness ( pEffect );

			BOOL bMinZoneUp		=	(dComplete > 0.9 );

			Aggplus::CMatrix oTranslateOff;			oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
			Aggplus::CMatrix oTranslateOn;			oTranslateOn.Translate ( m_oCenter.X, + m_oCenter.Y );

			m_oTransform.Multiply ( &oTranslateOn );

			if (bMinZoneUp)
			{
				Aggplus::CMatrix oTranslateMin;		oTranslateMin.Translate (-m_dWidthMM * 0.05 * ( 1.0 - dComplete ) / 0.1, 
					m_dHeightMM * 0.05 * ( 1.0 - dComplete ) / 0.1);
				m_oTransform.Multiply ( &oTranslateMin );
			}
			else
			{
				Aggplus::CMatrix oTranslateMin;		oTranslateMin.Translate (-m_dWidthMM * 0.05, m_dHeightMM * 0.05);

				Aggplus::CMatrix oTranslateT4;		oTranslateT4.Translate (0.0, m_dHeightMM * 0.45);
				Aggplus::CMatrix oRotateT4;			oRotateT4.Rotate (-90 * ( 1.0 - dComplete / 0.9) );
				Aggplus::CMatrix oTranslateOffT4;	oTranslateOffT4.Translate (0.0, -m_dHeightMM * 0.45);

				m_oTransform.Multiply ( &oTranslateOffT4 );
				m_oTransform.Multiply ( &oRotateT4 );
				m_oTransform.Multiply ( &oTranslateT4 );
				m_oTransform.Multiply ( &oTranslateMin );
			}

			m_oTransform.Multiply ( &oTranslateOff );

			return TRUE;
		}
		inline BOOL Swivel			( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			static const double __M_PI	=	3.14159265358979323846;

			double dComplete			=	GetCompleteness ( pEffect );

			if ( 5 == pEffect->GetDirection () )	//	vertical
			{
				Aggplus::CMatrix oTranslateOff;		oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
				Aggplus::CMatrix oScale;			oScale.Scale ( 1.0, sin (2.5 * __M_PI * dComplete) );	
				Aggplus::CMatrix oTranslateOn;		oTranslateOn.Translate ( m_oCenter.X, + m_oCenter.Y );

				m_oTransform.Multiply ( &oTranslateOn );
				m_oTransform.Multiply ( &oScale );
				m_oTransform.Multiply ( &oTranslateOff );

				return TRUE;
			}

			Aggplus::CMatrix oTranslateOff;		oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
			Aggplus::CMatrix oScale;			oScale.Scale ( sin (2.5 * __M_PI * dComplete), 1.0 );	// #ppt_w*sin(2.5*pi*$)
			Aggplus::CMatrix oTranslateOn;		oTranslateOn.Translate ( m_oCenter.X, + m_oCenter.Y );

			m_oTransform.Multiply ( &oTranslateOn );
			m_oTransform.Multiply ( &oScale );
			m_oTransform.Multiply ( &oTranslateOff );

			return TRUE;
		}


		inline BOOL Boomerang		( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			double dComplete	=	CalculateCompleteness ( pEffect );

			BOOL bMinZoneUp		=	(dComplete > 0.9 );

			Aggplus::CMatrix oTranslateOff;			oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
			Aggplus::CMatrix oTranslateOn;			oTranslateOn.Translate ( m_oCenter.X, + m_oCenter.Y );

			m_oTransform.Multiply ( &oTranslateOn );

			if (bMinZoneUp)
			{
				Aggplus::CMatrix oTranslateMin;		oTranslateMin.Translate (0.0, m_dHeightMM * 0.1 * ( 1.0 - dComplete ) / 0.1);
				Aggplus::CMatrix oScale;			oScale.Scale ( 1.0  - ( 1.0 - dComplete) / 0.1 * 0.95, 1.0 );	

				m_oTransform.Multiply ( &oTranslateMin );
				m_oTransform.Multiply ( &oScale );
			}
			else
			{
				Aggplus::CMatrix oTranslateMin;		oTranslateMin.Translate (0.0, m_dHeightMM * 0.1);

				Aggplus::CMatrix oTranslateT4;		oTranslateT4.Translate (0.0, m_dHeightMM * 0.4);
				Aggplus::CMatrix oRotateT4;			oRotateT4.Rotate (-90 * ( 1.0 - dComplete / 0.9) );
				Aggplus::CMatrix oTranslateOffT4;	oTranslateOffT4.Translate (0.0, -m_dHeightMM * 0.4);
				Aggplus::CMatrix oScale;			oScale.Scale ( 0.05  + ( 1.0 - dComplete / 0.9), 1.0 );	

				m_oTransform.Multiply ( &oTranslateOffT4 );
				m_oTransform.Multiply ( &oRotateT4 );
				m_oTransform.Multiply ( &oTranslateT4 );
				m_oTransform.Multiply ( &oTranslateMin );
				m_oTransform.Multiply ( &oScale );
			}

			m_oTransform.Multiply ( &oTranslateOff );

			return TRUE;
		}
		inline BOOL Bounce			( IAVSRenderer* pRenderer, CEffect* pEffect )
		{
			// TODO : добавить TimeFilter
			// TODO : fadeExit -	переделать, на вариант с формулой

			static const double __M_PI	=	3.14159265358979323846;
			double dComplete			=	CalculateCompleteness ( pEffect );

			//	#ppt_y-sin(pi*$)/3		-	0.332				1/3
			//	#ppt_y-sin(pi*$)/9		-	0.332 ~ 0.644		1/3
			//	#ppt_y-sin(pi*$)/27		-	0.644 ~ 0.810		1/6
			//	#ppt_y-sin(pi*$)/81		-	0.810 ~ 0.974		1/6

			double dMoveY	=	- m_oCenter.Y;

			if ( FadeExitType == pEffect->GetInnerType () )
			{
			}
			else
			{
				if (dComplete <= 1.0 / 3.0)
				{
					dMoveY		=	 - m_oCenter.Y - m_dHeightMM * sin ( __M_PI * ( (dComplete ) / (1.0/3.0) )) / 3;
				}
				else if  (dComplete > 1.0 / 3.0 && dComplete <= 2.0 / 3.0)
				{
					dMoveY		=	 - m_oCenter.Y - m_dHeightMM * sin ( __M_PI * ( (dComplete - 1.0/3.0)/ (1.0/3.0) )) / 9;
				}
				else if  (dComplete >= 2.0 / 3.0 && dComplete <= 2.0 / 3.0 + 1.0 / 6.0)
				{
					dMoveY		=	 - m_oCenter.Y - m_dHeightMM * sin ( __M_PI * ( (dComplete - 2.0/3.0)/ (1.0/6.0) )) / 27;
				}
				else if  (dComplete >= 5.0 / 6.0 )
				{
					dMoveY		=	 - m_oCenter.Y - m_dHeightMM * sin ( __M_PI * ( (dComplete - 5.0/6.0)/ (1.0/6.0) )) / 81;
				}
			}

			Aggplus::CMatrix oTranslateOff;		
			if ( FadeExitType == pEffect->GetInnerType () )
				oTranslateOff.Translate ( - m_oCenter.X, -m_oCenter.Y );
			else
				oTranslateOff.Translate ( - m_oCenter.X - (m_dWidthMM * 0.25) * ( 1.0 - dComplete ), dMoveY );

			Aggplus::CMatrix oTranslateOn;		oTranslateOn.Translate ( m_oCenter.X, m_oCenter.Y );

			m_oTransform.Multiply (&oTranslateOn);
			m_oTransform.Multiply (&oTranslateOff);

			return TRUE;
		}

		//		
		inline double GetCompleteness (CEffect* pEffect)
		{
			double dComplete	=	0.0;	
			pEffect->GetTransparency ( m_pLayer->GetTime (), dComplete );

			return dComplete;
		}

		inline double CalculateCompleteness (CEffect* pEffect)
		{
			double dTime		=	m_pLayer->GetTime();
			double dBeginTime	=	pEffect->GetBeginTime();
			double dEndTime		=	pEffect->GetEndTime();
			double dDuration	=	dEndTime - dBeginTime;

			if ( dDuration <= 0.0 )
			{
				if ( FadeExitType == pEffect->GetInnerType () )
					return 1.0;

				return 0.0;
			}

			if ( FadeExitType == pEffect->GetInnerType () )
				return 1.0 - (dTime - dBeginTime) / dDuration;

			return	(dTime - dBeginTime) / dDuration;
		}

		inline Aggplus::CMatrix GetRotateTransform (const double& dAngle, long nFlags = 0)
		{
			BOOL bFlipX	=	(0 != (c_nParamFlipX & nFlags));
			BOOL bFlipY	=	(0 != (c_nParamFlipY & nFlags));

			REAL m11	=	bFlipX ? -1.0f : 1.0f;
			REAL m22	=	bFlipY ? -1.0f : 1.0f;

			Aggplus::CMatrix oTransform;

			Aggplus::CMatrix oTranslateOff;		oTranslateOff.Translate ( - m_oCenter.X, - m_oCenter.Y );
			Aggplus::CMatrix oRotate;			oRotate.Rotate (dAngle);
			Aggplus::CMatrix oFlip;				oFlip.Scale(m11, m22);

			Aggplus::CMatrix oTranslateOn;		oTranslateOn.Translate ( m_oCenter.X, + m_oCenter.Y );

			oTransform.Multiply (&oTranslateOn);
			oTransform.Multiply (&oRotate);
			oTransform.Multiply (&oFlip);
			oTransform.Multiply (&oTranslateOff);

			return oTransform;
		}

	protected:

		double m_dWidthMM;
		double m_dHeightMM;

		CDoubleRect m_oBounds;
		double	m_dAngle;
		Animation::CPoint m_oMove;
		Animation::CPoint m_oCenter;

		ILayer* m_pLayer;
		IShowElement* m_pElement;

		Aggplus::CMatrix m_oTransform;
		Aggplus::CMatrix m_oFlip;
	};

	class CColorTransformer
	{
	public:
		class CColorOperation
		{
		public:

			inline static unsigned long Darken (unsigned long nColor, double dFactor)	//	 TODO : переделать на более правильную формулу
			{
				Aggplus::CColor color(nColor);

				return Aggplus::CColor::MakeARGB (color.GetA(),
					color.GetR() * ( 0.57 + ( 1.0 - 0.57 ) * dFactor ),
					color.GetG() * ( 0.57 + ( 1.0 - 0.57 ) * dFactor ),
					color.GetB() * ( 0.57 + ( 1.0 - 0.57 ) * dFactor ) );		
			}

			inline static unsigned long Lighten (unsigned long nColor, double dFactor)	//	 TODO : переделать на более правильную формулу
			{
				Aggplus::CColor color(nColor);

				BYTE red		=	max(color.GetR(),1);
				BYTE green		=	max(color.GetG(),1);
				BYTE blue		=	max(color.GetB(),1);
				BYTE addColor	=	(BYTE) ( 255.0 * ( ( 1.0 - 0.57 ) * (1.0-dFactor) ) );

				return Aggplus::CColor::MakeARGB (color.GetA(),
					min((red   + addColor ), 255),
					min((green + addColor ), 255),
					min((blue  + addColor ), 255));		
			}

			inline static unsigned long Desaturate (unsigned long nColor, double dFactor)
			{
				Aggplus::CColor color(nColor);
				BYTE rgb	=	(BYTE)((color.GetR() + color.GetG() + color.GetB()) / 3.0);

				return Aggplus::CColor::MakeARGB (color.GetA(),
					color.GetR() * ( dFactor ) + rgb * ( 1.0 - dFactor ),
					color.GetG() * ( dFactor ) + rgb * ( 1.0 - dFactor ),
					color.GetB() * ( dFactor ) + rgb * ( 1.0 - dFactor ) );
			}

			inline static unsigned long LinearColor (unsigned long nColor, unsigned long nToColor, double dFactor)
			{
				Aggplus::CColor from(nColor);
				Aggplus::CColor to(nToColor);

				return Aggplus::CColor::MakeARGB (from.GetA(), 
					to.GetR() * ( dFactor ) + from.GetR() * ( 1.0 - dFactor ),
					to.GetG() * ( dFactor ) + from.GetG() * ( 1.0 - dFactor ),
					to.GetB() * ( dFactor ) + from.GetB() * ( 1.0 - dFactor ) );		
			}
		};

		class CColorStatus
		{
		public:

			CColorStatus ()
			{
				m_bStatus	=	FALSE;

				m_dComplete	=	0;
				m_nColor	=	0;

				m_bEndColor	=	FALSE;
				m_nEndColor	=	0;
			}

			inline BOOL Status () const
			{
				return m_bStatus;
			}

			BOOL			m_bStatus;

			double			m_dComplete;
			unsigned long	m_nColor;

			BOOL			m_bEndColor;
			unsigned long	m_nEndColor;
		};

	public:

		CColorTransformer (BOOL bBlindFillEffects	=	FALSE) : m_bBlindFillEffects(bBlindFillEffects), m_pLayer(NULL)
		{

		}

		inline BOOL CollectOperations (IShowElement* pElement)
		{
			m_pLayer	=	pElement->GetBaseLayer ();
			if (m_pLayer)
			{
				CAnimationLayer* pLayer	=	static_cast<CAnimationLayer*>(m_pLayer);
				if (pLayer)
				{
					CAtlArray<CEmphasisEffect*>& arrEffects	=	pLayer->GetEmpEffects ();
					if ((long)arrEffects.GetCount())
					{
						m_oDesaturateOp	=	CColorStatus ();											// приведение к серому
						GetFactor (DesaturateEffect, arrEffects, m_oDesaturateOp);

						m_oDarkenOp		=	CColorStatus ();											// затемнение
						GetFactor (DarkenEffect, arrEffects, m_oDarkenOp);

						m_oLightenOp	=	CColorStatus ();											// высветление
						GetFactor (LightenEffect, arrEffects, m_oLightenOp);

						m_oFlickerOp	=	CColorStatus ();											// цветовая пульсация
						GetFlickerStatus (FlickerEffect, arrEffects, m_oFlickerOp);

						m_oLinesColor	=	CColorStatus();												// цвет линий
						GetEffectColor (ChangeColorLines, arrEffects, m_oLinesColor);

						m_oFontColor	=	CColorStatus();												// цвет текста
						GetEffectColor (ChangeFontColor, arrEffects, m_oFontColor);

						if (m_bBlindFillEffects)	//	смешивание цветов идет общее для некоторых типов эффектов
						{
							CAtlArray<int> arrFillEffect;
							arrFillEffect.Add (ChangeFillColor);
							arrFillEffect.Add (ColorBlendEffect);

							m_oFillColor			=	CColorStatus();							// цвет заливки
							m_oFillColor.m_bStatus	=	GetEffectColor (arrFillEffect, arrEffects, m_oFillColor.m_dComplete, m_oFillColor.m_bEndColor, m_oFillColor.m_nEndColor, m_oFillColor.m_nColor);

							return (m_oDesaturateOp.Status () || m_oDarkenOp.Status () || m_oLightenOp.Status () ||	m_oFlickerOp.Status () ||
								m_oLinesColor.Status () || m_oFillColor.Status () || m_oFontColor.Status());
						}

						m_oFillColor				=	CColorStatus();
						GetEffectColor (ChangeFillColor, arrEffects, m_oFillColor);

						m_oColorBlend				=	CColorStatus();
						GetEffectColor (ColorBlendEffect, arrEffects, m_oColorBlend);

						return (m_oDesaturateOp.Status () || m_oDarkenOp.Status () || m_oLightenOp.Status () ||	m_oFlickerOp.Status () ||
							m_oLinesColor.Status () || m_oFillColor.Status () || m_oColorBlend.Status () ||	m_oFontColor.Status());
					}
				}
			}

			return FALSE;
		}


		//
		inline void ApplyColor (unsigned long& nColor)
		{
			if (m_oDesaturateOp.Status())
				nColor	=	CColorOperation::Desaturate (nColor, m_oDesaturateOp.m_dComplete);

			if (m_oDarkenOp.Status ())
				nColor	=	CColorOperation::Darken (nColor, m_oDarkenOp.m_dComplete);

			if (m_oLightenOp.Status())
				nColor	=	CColorOperation::Lighten (nColor, m_oLightenOp.m_dComplete);
		}

		inline long ApplyColor (long nColor)
		{
			if (m_oDesaturateOp.Status())
				nColor	=	CColorOperation::Desaturate (nColor, m_oDesaturateOp.m_dComplete);

			if (m_oDarkenOp.Status ())
				nColor	=	CColorOperation::Darken (nColor, m_oDarkenOp.m_dComplete);

			if (m_oLightenOp.Status())
				nColor	=	CColorOperation::Lighten (nColor, m_oLightenOp.m_dComplete);

			return nColor;
		}

		//
		inline long ApplyFlickerColor (long nColor )
		{
			if (m_oFlickerOp.Status())
			{
				if (m_oFlickerOp.m_dComplete > 0.5)
					m_oFlickerOp.m_dComplete	=	1.0 - m_oFlickerOp.m_dComplete;

				nColor	=	CColorOperation::LinearColor (nColor, m_oFlickerOp.m_nColor, m_oFlickerOp.m_dComplete * 2.0);
			}

			return nColor;
		}

		inline BOOL FlickerStatus ()
		{
			return m_oFlickerOp.Status();
		}

		//
		inline BOOL LinesStatus ()
		{
			return m_oLinesColor.Status();
		}

		inline long ApplyLinesColor (long nColor)
		{
			if (m_oLinesColor.m_bStatus)
			{
				if (m_oLinesColor.m_bEndColor)
					nColor	=	CColorOperation::LinearColor (m_oLinesColor.m_nEndColor, m_oLinesColor.m_nColor, m_oLinesColor.m_dComplete);
				else
					nColor	=	CColorOperation::LinearColor (nColor, m_oLinesColor.m_nColor, m_oLinesColor.m_dComplete);
			}

			return nColor;
		}

		//
		inline BOOL FillStatus ()
		{
			return m_oFillColor.Status();
		}

		inline long ApplyFillColor (long nColor)
		{
			if (m_oFillColor.m_bStatus)
			{
				if (m_oFillColor.m_bEndColor)
					nColor	=	CColorOperation::LinearColor (m_oFillColor.m_nEndColor, m_oFillColor.m_nColor, m_oFillColor.m_dComplete);
				else
					nColor	=	CColorOperation::LinearColor (nColor, m_oFillColor.m_nColor, m_oFillColor.m_dComplete);
			}

			return nColor;
		}

		//
		inline BOOL FillObjStatus ()
		{
			return m_oColorBlend.Status();
		}

		inline long ApplyFillObjColor (long nColor)
		{
			if (m_oColorBlend.m_bStatus)
			{
				if (m_oColorBlend.m_bEndColor)
					nColor	=	CColorOperation::LinearColor (m_oColorBlend.m_nEndColor, m_oColorBlend.m_nColor, m_oColorBlend.m_dComplete);
				else
					nColor	=	CColorOperation::LinearColor (nColor, m_oColorBlend.m_nColor, m_oColorBlend.m_dComplete);
			}

			return nColor;
		}

		// 
		inline BOOL FontColorStatus()
		{
			return m_oFontColor.m_bStatus;
		}
		inline long ApplyFontColor (long nColor)
		{
			if (m_oFontColor.m_bStatus)
			{
				if (m_oFontColor.m_bEndColor)
					nColor	=	CColorOperation::LinearColor (m_oFontColor.m_nEndColor, m_oFontColor.m_nColor, m_oFontColor.m_dComplete);
				else
					nColor	=	CColorOperation::LinearColor (nColor, m_oFontColor.m_nColor, m_oFontColor.m_dComplete);
			}

			return nColor;
		}
	private:

		inline BOOL GetFactor (int nEffectID, CAtlArray<CEmphasisEffect*>& arrEffects, CColorStatus& oStatus)
		{
			double dTime				=	m_pLayer->GetTime ();

			BOOL bLeftEffect			=	FALSE;		// уже был применен эффект 
			BOOL bHaveInEffect			=	FALSE;		// находимся в зоне эффекта

			CEmphasisEffect* pInEffect	=	NULL;

			for ( long i = 0; i < (long)arrEffects.GetCount(); ++i )
			{
				CEmphasisEffect* pEffect = (CEmphasisEffect*)arrEffects[i];
				if ( nEffectID == pEffect->GetID () )
				{
					if ( pEffect->InEffect (dTime) )
					{
						if (NULL == pInEffect)
							pInEffect	=	pEffect;
					}

					if ( pEffect->GetEndTime () <= dTime )
					{
						oStatus.m_dComplete	=	0.0;			//	элемент должен быть уже с выполенным цветовым преобразованием ( например затемнен )
						oStatus.m_bStatus	=	TRUE;

						return TRUE;
					}
				}
			}

			if (pInEffect)	//	находимся в зоне эффекта, примененям постепенное изменение в сторону нужного нам эффекта
			{
				oStatus.m_dComplete	=	1.0 - ( dTime - pInEffect->GetBeginTime() ) / ( pInEffect->GetEndTime () - pInEffect->GetBeginTime() );
				oStatus.m_bStatus	=	TRUE;

				return TRUE;
			}

			oStatus.m_bStatus		=	FALSE;
			return FALSE;	//	цвет не меняется
		}

		inline BOOL GetFlickerStatus (int nEffectID, CAtlArray<CEmphasisEffect*>& arrEffects, CColorStatus& oStatus)		//	эффект действует на все время показа объекта
		{
			oStatus.m_nEndColor			=	0.0;

			double dTime				=	m_pLayer->GetTime ();
			double dLeftTime			=	0.0;

			CEmphasisEffect* pInEffect	=	NULL;

			for ( long i = 0; i < (long)arrEffects.GetCount(); ++i )
			{
				CEmphasisEffect* pEffect = (CEmphasisEffect*)arrEffects[i];
				if ( nEffectID == pEffect->GetID () )
				{
					if ( pEffect->InEffect (dTime) )
					{
						if (NULL==pInEffect)
						{
							pInEffect	=	pEffect;
							dLeftTime	=	pInEffect->GetBeginTime ();
						}
						else
						{
							if (pEffect->GetBeginTime () >= dLeftTime)
							{
								pInEffect	=	pEffect;
								dLeftTime	=	pInEffect->GetBeginTime ();
							}
						}
					}
				}
			}

			if (pInEffect)	//	находимся в зоне эффекта, примененям постепенное изменение в сторону нужного нам эффекта
			{
				oStatus.m_dComplete		=	( dTime - pInEffect->GetBeginTime() ) / ( pInEffect->GetEndTime () - pInEffect->GetBeginTime() );
				oStatus.m_nColor		=	pInEffect->GetColor ();

				oStatus.m_bStatus		=	TRUE;
				return TRUE;
			}

			oStatus.m_bStatus			=	FALSE;
			return FALSE;	// не такого эффекта
		}

		inline BOOL GetEffectColor (int nEffectID, CAtlArray<CEmphasisEffect*>& arrEffects, CColorStatus& oStatus )
		{
			//	EndColor	-	эффекты у которых есть сохранение цвета на все время анимации
			//	Color		-	текущий цвет

			oStatus.m_nColor			=	0.0;
			oStatus.m_nEndColor			=	0.0;

			double dTime				=	m_pLayer->GetTime ();
			double dLeftTime			=	0.0;
			double dLeftEndTime			=	0.0;
			double dEndTime				=	0.0;

			CEmphasisEffect* pInEffect	=	NULL;
			CEmphasisEffect* pEndEffect	=	NULL;

			for ( long i = 0; i < (long)arrEffects.GetCount(); ++i )
			{
				CEmphasisEffect* pEffect = (CEmphasisEffect*)arrEffects[i];
				if ( nEffectID == pEffect->GetID () )
				{
					if ( pEffect->InEffect (dTime) || (pEffect->GetBeginTime() < dTime && pEffect->GetEndTime() < dTime) )
					{
						if (NULL == pInEffect)
						{
							pInEffect		=	pEffect;
							dLeftTime		=	pInEffect->GetBeginTime ();
						}
						else
						{
							if (pEffect->GetBeginTime () >= dLeftTime)
							{
								pInEffect	=	pEffect;
								dLeftTime	=	pInEffect->GetBeginTime ();
							}
						}
					}

					if (pEffect->GetEndTime() < dTime)
					{
						if (NULL == pEndEffect)
						{
							pEndEffect			=	pEffect;
							dEndTime			=	pEndEffect->GetBeginTime ();

							oStatus.m_nEndColor	=	pEndEffect->GetColor ();
							oStatus.m_bEndColor	=	TRUE;
						}
						else
						{
							if (pEffect->GetBeginTime () >= dEndTime)
							{
								pEndEffect			=	pEffect;
								dEndTime			=	pEndEffect->GetBeginTime ();

								oStatus.m_nEndColor	=	pEndEffect->GetColor ();
							}
						}
					}
				}
			}

			if (pInEffect)	//	находимся в зоне эффекта, примененям постепенное изменение в сторону нужного нам эффекта
			{
				oStatus.m_dComplete		=	( dTime - pInEffect->GetBeginTime() ) / ( pInEffect->GetEndTime () - pInEffect->GetBeginTime() );
				oStatus.m_nColor		=	pInEffect->GetColor ();
				oStatus.m_bStatus		=	TRUE;

				return TRUE;
			}

			if (pEndEffect)
			{
				oStatus.m_dComplete		=	1.0;
				oStatus.m_nColor		=	oStatus.m_nEndColor;
				oStatus.m_bStatus		=	TRUE;

				return TRUE;
			}

			oStatus.m_bStatus			=	FALSE;
			return FALSE;	// не такого эффекта
		}

		inline BOOL GetEffectColor (CAtlArray<int>& arrEffectIDS, CAtlArray<CEmphasisEffect*>& arrEffects, double& dFactor, BOOL& bEndColor, unsigned long& nEndColor, unsigned long& nCurColor )
		{
			//	nEndColor	-	эффекты у которых есть сохранение цвета на все время анимации
			//	nCurColor	-	текущий цвет

			nCurColor					=	0.0;
			nEndColor					=	0.0;

			double dTime				=	m_pLayer->GetTime ();
			double dLeftTime			=	0.0;
			double dLeftEndTime			=	0.0;
			double dEndTime				=	0.0;

			CEmphasisEffect* pInEffect	=	NULL;
			CEmphasisEffect* pEndEffect	=	NULL;

			for ( long i = 0; i < (long)arrEffects.GetCount(); ++i )
			{
				CEmphasisEffect* pEffect	=	arrEffects[i];
				int nEffectID				=	pEffect->GetID ();		
				for ( long j = 0; j < (long)arrEffectIDS.GetCount(); ++j )
				{
					if (arrEffectIDS[j] == nEffectID)
					{
						if ( pEffect->InEffect (dTime) || (pEffect->GetBeginTime() < dTime && pEffect->GetEndTime() < dTime) )
						{
							if (NULL == pInEffect)
							{
								pInEffect		=	pEffect;
								dLeftTime		=	pInEffect->GetBeginTime ();
							}
							else
							{
								if (pEffect->GetBeginTime () >= dLeftTime)
								{
									pInEffect	=	pEffect;
									dLeftTime	=	pInEffect->GetBeginTime ();
								}
							}
						}

						if (pEffect->GetEndTime() < dTime)
						{
							if (NULL == pEndEffect)
							{
								pEndEffect		=	pEffect;
								dEndTime		=	pEndEffect->GetBeginTime ();
								nEndColor		=	pEndEffect->GetColor ();

								bEndColor		=	TRUE;
							}
							else
							{
								if (pEffect->GetBeginTime () >= dEndTime)
								{
									pEndEffect	=	pEffect;
									dEndTime	=	pEndEffect->GetBeginTime ();
									nEndColor	=	pEndEffect->GetColor ();
								}
							}
						}
					}
				}
			}

			if (pInEffect)	//	находимся в зоне эффекта, примененям постепенное изменение в сторону нужного нам эффекта
			{
				dFactor		=	( dTime - pInEffect->GetBeginTime() ) / ( pInEffect->GetEndTime () - pInEffect->GetBeginTime() );
				nCurColor	=	pInEffect->GetColor ();

				return TRUE;
			}

			if (pEndEffect)
			{
				dFactor		=	1.0;
				nCurColor	=	nEndColor;

				return TRUE;
			}

			return FALSE;	// не такого эффекта
		}

	private:

		ILayer*			m_pLayer;

		BOOL			m_bBlindFillEffects;	//	все изменения с fill color соединяются в одну категорию

		CColorStatus	m_oDarkenOp;
		CColorStatus	m_oDesaturateOp;
		CColorStatus	m_oLightenOp;
		CColorStatus	m_oFlickerOp;

		CColorStatus	m_oLinesColor;
		CColorStatus	m_oFillColor;
		CColorStatus	m_oColorBlend;

		CColorStatus	m_oFontColor;
	};

	class CComposeTransformer
	{
	public:
		CComposeTransformer()
		{
			m_pTransforms	=	NULL;
			m_pFrame		=	NULL;
			m_pElement		=	NULL;			
			m_nComposeId	=	-1;
			m_pEffect		=	NULL;
			m_dTime			=	0.0;

			m_pBlendFrame	=	NULL;
			m_nBufferSize	=	0;

			m_bSwapSources	=	FALSE;
		}

		~CComposeTransformer()
		{
			ClearMemory ();
		}


		inline BOOL ApplyEffect (CEffect* pEffect, IShowElement* pElement, IUnknown*& pImage)
		{
			if (pEffect && pElement)
			{
				m_pEffect		=	pEffect;
				m_pElement		=	pElement;
				pImage			=	NULL;

				ILayer* pLayer	=	m_pElement->GetBaseLayer ();
				if (pLayer)
				{
					m_dTime		=	pLayer->GetTime();

					if (DoEffect())
					{
						pImage	=	m_pFrame;

						return TRUE;
					}
				}
			}

			return FALSE;
		}

		inline void ClearMemory ()
		{
			RELEASEINTERFACE(m_pTransforms);
			RELEASEINTERFACE(m_pBlendFrame);
			RELEASEINTERFACE(m_pFrame);
		}

	private:

		inline BOOL DoEffect ()
		{
			Setup ();

			if (m_pTransforms)
			{
				IUnknown* pImage	=	m_pElement->GetBufferImage ();

				if (NULL == pImage || NULL == GetBlendImage(pImage) ||  NULL == GetBufferImage (pImage))
					return FALSE;				

				double dTime		=	max( m_dTime - m_pEffect->GetBeginTime()- 0.0001, 0.0);

				m_pTransforms->SetTime (dTime);

				VARIANT vtFrom;	vtFrom.vt	=	VT_UNKNOWN;
				VARIANT vtTo;	vtTo.vt		=	VT_UNKNOWN;	
				VARIANT vtBuff;	vtBuff.vt	=	VT_UNKNOWN;

				if (m_bSwapSources)
				{
					m_pTransforms->SetTime (m_pEffect->GetEndTime() - m_dTime);

					if (FadeExitType == m_pEffect->GetInnerType ())
					{
						if ( dTime <= 0.0001 )
						{
							if (CopyBuffer ((MediaCore::IAVSUncompressedVideoFrame*)pImage, m_pFrame))
							{
								RELEASEINTERFACE(m_pTransforms);
								return TRUE;
							}
						}
						else
						{
							m_pBlendFrame->QueryInterface(&vtFrom.punkVal);
							m_pBlendFrame->Release();

							pImage->QueryInterface(&vtTo.punkVal);
							pImage->Release();
						}
					}

					if (FadeInType == m_pEffect->GetInnerType ())
					{					
						if ( dTime <= 0.0001 )
						{
							if (CopyBuffer ((MediaCore::IAVSUncompressedVideoFrame*)m_pBlendFrame, m_pFrame))
							{
								RELEASEINTERFACE(m_pTransforms);
								return TRUE;
							}
						}
						else
						{
							m_pBlendFrame->QueryInterface(&vtTo.punkVal);
							m_pBlendFrame->Release();

							pImage->QueryInterface(&vtFrom.punkVal);
							pImage->Release();
						}
					}
				}
				else
				{
					if (FadeExitType == m_pEffect->GetInnerType ())
					{
						if ( dTime <= 0.0001 )
						{
							if (CopyBuffer ((MediaCore::IAVSUncompressedVideoFrame*)pImage, m_pFrame))
							{
								RELEASEINTERFACE(m_pTransforms);
								return TRUE;
							}
						}
						else
						{
							m_pBlendFrame->QueryInterface(&vtTo.punkVal);
							m_pBlendFrame->Release();

							pImage->QueryInterface(&vtFrom.punkVal);	
							pImage->Release();
						}
					}
					else
					{
						m_pBlendFrame->QueryInterface(&vtFrom.punkVal);	
						m_pBlendFrame->Release();

						pImage->QueryInterface(&vtTo.punkVal);	pImage->AddRef();
						pImage->Release();
					}
				}

				m_pTransforms->SetSource ( 1, vtFrom );
				m_pTransforms->SetSource ( 2, vtTo );

				m_pFrame->QueryInterface(&vtBuff.punkVal);
				m_pFrame->Release();

				m_pTransforms->SetSource ( 0, vtBuff );				

				VARIANT_BOOL bStatus = VARIANT_FALSE;
				m_pTransforms->Transform ( &bStatus );

				if (VARIANT_TRUE == bStatus)
				{
					VARIANT vImg;
					m_pTransforms->GetResult ( 0, &vImg );

					if ( vImg.punkVal )
					{
						RELEASEINTERFACE (m_pFrame);
						vImg.punkVal->QueryInterface(&m_pFrame);
						RELEASEINTERFACE(vImg.punkVal);

						RELEASEINTERFACE(m_pTransforms);
						return TRUE;
					}
				}
			}

			RELEASEINTERFACE(m_pTransforms);
			return FALSE;
		}

		inline BOOL Setup ()
		{
			if (NULL == m_pTransforms)
			{
				if ( FAILED (CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_INPROC, ImageStudio::IID_IImageTransforms, (void**)&m_pTransforms) ) )
					return FALSE;
			}

			long nComposeId	= GetXmlEffectId ();
			if (-1 == nComposeId)
				return FALSE;

			if (m_pTransforms)
			{
				m_nComposeId	=	nComposeId;

				CString Effect;
				Effect.Format ( _T("<VideoCompose Time=\"%f\" effectid=\"%d\" />"), m_pEffect->GetEndTime() - m_pEffect->GetBeginTime(), m_nComposeId );

				VARIANT_BOOL bStatus = VARIANT_FALSE;
				m_pTransforms->SetXml ( CComBSTR ( Effect ), &bStatus );

				if (VARIANT_TRUE == bStatus)
					return TRUE;

				return TRUE;
			}

			return FALSE;
		}

		//
		inline IUnknown* GetBlendImage (IUnknown* pSrc)
		{
			if (NULL == m_pBlendFrame && pSrc)
			{
				MediaCore::IAVSUncompressedVideoFrame* pSource	=	NULL;
				pSrc->QueryInterface (&pSource);
				pSrc->Release ();

				if (pSource)
				{
					long Width	= 0; pSource->get_Width(&Width);
					long Height	= 0; pSource->get_Height(&Height);

					if ( SUCCEEDED ( CoCreateInstance ( __uuidof ( MediaCore::CAVSUncompressedVideoFrame ), NULL, CLSCTX_INPROC_SERVER, __uuidof ( MediaCore::IAVSUncompressedVideoFrame ), (void **)&m_pBlendFrame ) ) )
					{
						m_pBlendFrame->put_Width(Width);
						m_pBlendFrame->put_Height(Height);
						m_pBlendFrame->put_Stride(0, 4 * Width);

						m_pBlendFrame->put_AspectRatioX(Width);
						m_pBlendFrame->put_AspectRatioY(Height);

						m_pBlendFrame->put_ColorSpace ( (1<< 6)|(1<<31) );
						m_pBlendFrame->AllocateBuffer(-1);	

						m_nBufferSize	=	4 * Width * Height;
					}
				}
			}

			if (m_pBlendFrame)
			{
				m_pBlendFrame->put_ColorSpace ( (1<< 6)|(1<<31) );

				BYTE* pBuffer = NULL;
				m_pBlendFrame->get_Buffer(&pBuffer);
				if (pBuffer)
				{
					memset ( pBuffer, 0, m_nBufferSize );
				}
			}

			return m_pBlendFrame;
		}

		inline IUnknown* GetBufferImage (IUnknown* pSrc)
		{
			if (NULL == m_pFrame && pSrc)
			{
				MediaCore::IAVSUncompressedVideoFrame* pSource	=	NULL;
				pSrc->QueryInterface (&pSource);
				pSrc->Release ();

				if (pSource)
				{
					long Width	= 0;	pSource->get_Width(&Width);
					long Height	= 0;	pSource->get_Height(&Height);

					if ( SUCCEEDED ( CoCreateInstance ( __uuidof ( MediaCore::CAVSUncompressedVideoFrame ), NULL, CLSCTX_INPROC_SERVER, __uuidof ( MediaCore::IAVSUncompressedVideoFrame ), (void **)&m_pFrame ) ) )
					{
						m_pFrame->put_Width(Width);
						m_pFrame->put_Height(Height);
						m_pFrame->put_Stride(0, 4 * Width);

						m_pFrame->put_AspectRatioX(Width);
						m_pFrame->put_AspectRatioY(Height);

						m_pFrame->put_ColorSpace ( (1<< 6)|(1<<31) );
						m_pFrame->AllocateBuffer(-1);

						m_nBufferSize	=	4 * Width * Height;			
					}
				}
			}

			if (m_pFrame)
			{
				m_pFrame->put_ColorSpace ( (1<< 6)|(1<<31) );

				BYTE* pBuffer = NULL;
				m_pFrame->get_Buffer(&pBuffer);
				if (pBuffer)
				{
					memset ( pBuffer, 0, m_nBufferSize );
				}
			}

			return m_pFrame;
		}


		inline long GetXmlEffectId ()
		{
			int direction = m_pEffect->GetDirection ();

			if (m_pEffect->GetID() == WheelEffect)
			{
				if (FadeExitType == m_pEffect->GetInnerType ())
				{
					if ( 1 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nClockBottomReverse;

					if ( 2 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nClock2AnglesReverse;

					if ( 3 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nClock3AnglesReverse;

					if ( 4 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nClock4AnglesReverse;

					if ( 8 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nClock8AnglesReverse;
				}
				else
				{
					if ( 1 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nClockBottomClockwise;

					if ( 2 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nClock2AnglesClockwise;

					if ( 3 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nClock3AnglesClockwise;

					if ( 4 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nClock4AnglesClockwise;

					if ( 8 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nClock8AnglesClockwise;
				}
			}

			if (m_pEffect->GetID() == BlindsEffect)
			{
				if (FadeExitType == m_pEffect->GetInnerType ())
				{
					if ( 10 == direction )
						return 4907;	//	ImageStudio::Paint::Compose::Constants::c_nSlideSideTopToBottom;

					if ( 5 == direction )
						return 4906;	//	ImageStudio::Paint::Compose::Constants::c_nSlideSideLeftToRight;
				}
				else
				{
					if ( 10 == direction )
						return 4907;	//	ImageStudio::Paint::Compose::Constants::c_nSlideSideTopToBottom;

					if ( 5 == direction )
						return 4906;	//	ImageStudio::Paint::Compose::Constants::c_nSlideSideLeftToRight;
				}
			}

			if (m_pEffect->GetID() == WedgeEffect)
			{
				if (FadeExitType == m_pEffect->GetInnerType ())
				{
					return ImageStudio::Paint::Compose::Constants::c_nClockBottomOpposite;
				}
				else
				{
					return ImageStudio::Paint::Compose::Constants::c_nClockTopOpposite;
				}
			}

			if (m_pEffect->GetID() == CheckBoardEffect)
			{
				if (FadeExitType == m_pEffect->GetInnerType ())
				{
					if ( 10 == direction )	//	Across
						return ImageStudio::Paint::Compose::Constants::c_nWipeCheckerRightToLeft;

					if ( 5 == direction )	//	Vertical
						return ImageStudio::Paint::Compose::Constants::c_nWipeCheckerTopToBottom;

				}
				else
				{
					if ( 10 == direction )	//	Across
						return ImageStudio::Paint::Compose::Constants::c_nWipeCheckerRightToLeft;

					if ( 5 == direction )	//	Vertical
						return ImageStudio::Paint::Compose::Constants::c_nWipeCheckerTopToBottom;
				}
			}

			if (m_pEffect->GetID() == CircleEffect)
			{
				if (FadeExitType == m_pEffect->GetInnerType ())
				{
					if ( 16 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nWipeCenterCircleIn;

					if ( 32 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nWipeCenterCircleOut;
				}
				else
				{
					if ( 16 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nWipeCenterCircleOut;

					if ( 32 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nWipeCenterCircleIn;
				}
			}

			if (m_pEffect->GetID() == BoxEffect)
			{
				if (FadeExitType == m_pEffect->GetInnerType ())
				{
					if ( 16 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nWipeCenterBoxIn;

					if ( 32 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nWipeCenterBoxOut;
				}
				else
				{
					if ( 16 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nWipeCenterBoxOut;

					if ( 32 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nWipeCenterBoxIn;
				}
			}

			if (m_pEffect->GetID() == DiamondEffect)
			{
				if (FadeExitType == m_pEffect->GetInnerType ())
				{
					if ( 16 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nWipeCenterDiamondIn;

					if ( 32 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nWipeCenterDiamondOut;
				}
				else
				{
					if ( 16 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nWipeCenterDiamondOut;

					if ( 32 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nWipeCenterDiamondIn;
				}
			}

			if (m_pEffect->GetID() == PlusEffect)
			{
				if (FadeExitType == m_pEffect->GetInnerType ())
				{
					if ( 16 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nWipeCenterCrossIn;

					if ( 32 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nWipeCenterCrossOut;
				}
				else
				{
					if ( 16 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nWipeCenterCrossOut;

					if ( 32 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nWipeCenterCrossIn;
				}
			}

			if (m_pEffect->GetID() == RandomBarsEffect)
			{
				if (FadeExitType == m_pEffect->GetInnerType ())
				{
					if ( 10 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nBarsHorizontal;

					if ( 5 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nBarsVertical;
				}
				else
				{
					if ( 10 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nBarsHorizontal;

					if ( 5 == direction )
						return ImageStudio::Paint::Compose::Constants::c_nBarsVertical;
				}
			}

			if (m_pEffect->GetID() == PeekEffect)
			{
				if (FadeExitType == m_pEffect->GetInnerType ())
				{
					if ( 2 == direction )	//	RIGHT
						return ImageStudio::Paint::Compose::Constants::c_nPushSideLeftToRight;

					if ( 8 == direction )	//	LEFT
						return ImageStudio::Paint::Compose::Constants::c_nPushSideRightToLeft;

					if ( 1 == direction )	//	TOP
						return ImageStudio::Paint::Compose::Constants::c_nPushSideBottomToTop;

					if ( 4 == direction )	//	BOTTOM
						return ImageStudio::Paint::Compose::Constants::c_nPushSideTopToBottom;
				}
				else
				{
					if ( 2 == direction )	//	RIGHT
						return ImageStudio::Paint::Compose::Constants::c_nPushSideRightToLeft;

					if ( 8 == direction )	//	LEFT
						return ImageStudio::Paint::Compose::Constants::c_nPushSideLeftToRight;

					if ( 1 == direction )	//	TOP
						return ImageStudio::Paint::Compose::Constants::c_nPushSideTopToBottom;

					if ( 4 == direction )	//	BOTTOM
						return ImageStudio::Paint::Compose::Constants::c_nPushSideBottomToTop;
				}
			}

			if (m_pEffect->GetID() == WipeEffect)
			{
				if (FadeExitType == m_pEffect->GetInnerType ())
				{
					if ( 1 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nRevealUp;
					if ( 2 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nRevealLeft;
					if ( 4 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nRevealDown;
					if ( 8 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nRevealRight;
				}
				else
				{
					if ( 1 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nRevealDown;
					if ( 2 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nRevealRight;
					if ( 4 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nRevealUp;
					if ( 8 == direction )	
						return ImageStudio::Paint::Compose::Constants::c_nRevealLeft;
				}
			}

			if (m_pEffect->GetID() == SplitEffect)
			{
				if (FadeExitType == m_pEffect->GetInnerType ())
				{
					if ( 0x0000001A == direction )	//	Horizontal in
					{
						m_bSwapSources	=	TRUE;
						return ImageStudio::Paint::Compose::Constants::c_nSplitSmoothHorizontal;
					}

					if ( 0x0000002A == direction )	//	Horizontal out
					{
						return ImageStudio::Paint::Compose::Constants::c_nSplitSmoothHorizontal;
					}

					if ( 0x00000015 == direction )	//	Vertical in
					{
						m_bSwapSources	=	TRUE;
						return ImageStudio::Paint::Compose::Constants::c_nSplitSmoothVertical;
					}

					if ( 0x00000025 == direction )	//	Vertical out
					{
						return ImageStudio::Paint::Compose::Constants::c_nSplitSmoothVertical;
					}
				}
				else
				{
					if ( 0x0000001A == direction )	//	Horizontal in
					{
						m_bSwapSources	=	TRUE;
						return ImageStudio::Paint::Compose::Constants::c_nSplitSmoothHorizontal;
					}

					if ( 0x0000002A == direction )	//	Horizontal out
					{
						return ImageStudio::Paint::Compose::Constants::c_nSplitSmoothHorizontal;
					}

					if ( 0x00000015 == direction )	//	Vertical in
					{
						m_bSwapSources	=	TRUE;
						return ImageStudio::Paint::Compose::Constants::c_nSplitSmoothVertical;
					}

					if ( 0x00000025 == direction )	//	Vertical out
					{
						return ImageStudio::Paint::Compose::Constants::c_nSplitSmoothVertical;
					}
				}
			}

			if (m_pEffect->GetID() == StripsEffect)
			{
				if (FadeExitType == m_pEffect->GetInnerType ())
				{
					if ( 9 == direction )	//   Up left.  
						return ImageStudio::Paint::Compose::Constants::c_nMosaicLeftTop;

					if ( 3 == direction )	//   Up right.  
						return ImageStudio::Paint::Compose::Constants::c_nMosaicRightTop;

					if ( 6 == direction )	//   Down right. 
						return ImageStudio::Paint::Compose::Constants::c_nMosaicRightBottom;

					if ( 12 == direction )	//   Down left.  
						return ImageStudio::Paint::Compose::Constants::c_nMosaicLeftBottom;
				}
				else
				{
					if ( 9 == direction )	//   Up left.  
						return ImageStudio::Paint::Compose::Constants::c_nMosaicLeftTop;

					if ( 3 == direction )	//   Up right.  
						return ImageStudio::Paint::Compose::Constants::c_nMosaicRightTop;

					if ( 6 == direction )	//   Down right. 
						return ImageStudio::Paint::Compose::Constants::c_nMosaicRightBottom;

					if ( 12 == direction )	//   Down left.  
						return ImageStudio::Paint::Compose::Constants::c_nMosaicLeftBottom;
				}
			}

			if (m_pEffect->GetID() == DissolveEffect)
			{
				if (FadeExitType == m_pEffect->GetInnerType ())
				{
					return ImageStudio::Paint::Compose::Constants::c_nEnhancedAlphaNoise;
				}
				else
				{
					return ImageStudio::Paint::Compose::Constants::c_nEnhancedAlphaNoise;
				}
			}

			return -1;
		}		

		inline BOOL CopyBuffer (MediaCore::IAVSUncompressedVideoFrame* pFrom, MediaCore::IAVSUncompressedVideoFrame* pTo)
		{
			if (NULL != pFrom && NULL != pTo)
			{
				BYTE* pBuffer	=	NULL;	pFrom->get_Buffer(&pBuffer);
				BYTE* pToBuffer	=	NULL;	pTo->get_Buffer(&pToBuffer);

				pTo->put_ColorSpace ( (1<< 6));

				if ( NULL != pBuffer && NULL != pToBuffer)
				{
					memcpy (pToBuffer, pBuffer, m_nBufferSize );
					return TRUE;
				}
			}

			return FALSE;
		}

	private:

		double			m_dTime;
		long			m_nComposeId;
		IShowElement*	m_pElement;
		CEffect*		m_pEffect;

		BOOL			m_bSwapSources;
		unsigned long	m_nBufferSize;

		ImageStudio::IImageTransforms*			m_pTransforms;	//	NEED CLEAR FROM MEMORY
		MediaCore::IAVSUncompressedVideoFrame*	m_pBlendFrame;	//	NEED CLEAR FROM MEMORY
		MediaCore::IAVSUncompressedVideoFrame*	m_pFrame;		//	NEED CLEAR FROM MEMORY
	};

	class CBlendImage 
	{
	public:
		CBlendImage (IUnknown* pImage) : m_pRefImage (pImage), m_pCopyImage (NULL)
		{
		}

		~CBlendImage ()
		{
			RELEASEINTERFACE (m_pCopyImage);
		}

		inline IUnknown* Get (double dAlpha)
		{
			if (dAlpha < 1.0 )
			{
				if (DoOperation (dAlpha))
					return m_pCopyImage;
			}

			return m_pRefImage;
		}

	private:

		inline BOOL DoOperation (double dAlpha)
		{
			if (m_pRefImage)
			{
				MediaCore::IAVSUncompressedVideoFrame* pImage = NULL;
				m_pRefImage->QueryInterface (&pImage);
				m_pRefImage->Release ();

				if (pImage)
				{
					pImage->CreateDuplicate (2, (MediaCore::IAVSMediaData**)(&m_pCopyImage) );
					if (m_pCopyImage)
					{
						LONG Width			=	0;		((MediaCore::IAVSUncompressedVideoFrame*)m_pCopyImage)->get_Width(&Width);
						LONG Height			=	0;		((MediaCore::IAVSUncompressedVideoFrame*)m_pCopyImage)->get_Height(&Height);
						BYTE* pBuffer		=	NULL;	((MediaCore::IAVSUncompressedVideoFrame*)m_pCopyImage)->get_Buffer(&pBuffer);

						if (pImage && Width > 2 && Height > 2)
						{
							for ( long i = 0; i < Width * Height * 4; i += 4 )
								pBuffer	[ i + 3 ]	=	(BYTE)( (double)pBuffer	[ i + 3 ] * dAlpha);

							return TRUE;
						}
					}
				}
			}

			return FALSE;
		}

	private:

		IUnknown* m_pRefImage;
		IUnknown* m_pCopyImage;
	};

	class CShowElement : public IShowElement
	{
	public:

		CShowElement ()
		{
			m_pActualEffect	=	NULL;
		}

		inline void UpdateTransform (IAVSRenderer* pCommander, long nFlags)
		{
			CCoordTransformer oTransformer (m_dXWidthMM, m_dYHeightMM, nFlags);

			if ( m_pLayer )
			{		
				Aggplus::CMatrix oAffineTrans;

				CMotionEffect* pMEffect		=	NULL;
				CMotionEffect* pTimeLine	=	NULL;
				if ( ((CAnimationLayer*)m_pLayer)->GetMotionEffect(pMEffect, pTimeLine) )
				{
					if (pMEffect)
					{
						Animation::CPoint oMove; 					
						if (pMEffect->GetTranslate ( m_pLayer->GetTime(), oMove))
						{
							oAffineTrans.Translate ( oMove.X * m_dXWidthMM, oMove.Y * m_dYHeightMM );
						}
					}
				}

				m_pActualEffect			=	NULL;

				Aggplus::CMatrix oFadeInTransform;

				CFadeInEffect* pFadeIn	=	NULL;
				if ( ((CAnimationLayer*)m_pLayer)->GetFadeInEffect (pFadeIn, m_pActualEffect) )
				{
					if ( pFadeIn )
					{
						if ( oTransformer.ProcessFadeEffect (pFadeIn, this, pCommander) )
						{
							oFadeInTransform	=	oTransformer.GetTransform ();

							oAffineTrans.Multiply (&oTransformer.GetTransform ());

							m_pActualEffect	=	NULL;	//	off 
						}
					}
				}

				Aggplus::CMatrix oFadeExitTransform;

				CFadeExitEffect* pExit		=	NULL;
				CFadeExitEffect* pLineExit	=	NULL;
				if ( ((CAnimationLayer*)m_pLayer)->GetFadeExitEffect (pExit, pLineExit) )
				{
					if ( pExit )
					{
						if ( oTransformer.ProcessFadeEffect (pExit, this, pCommander) )
						{
							oFadeExitTransform	=	oTransformer.GetTransform ();

							oAffineTrans.Multiply (&oTransformer.GetTransform ());
						}
					}
				}

				oAffineTrans.Multiply (&oTransformer.GetInnerTransform(this));

				double matrix [ 6 ];
				oAffineTrans.GetElements (matrix);
				pCommander->SetTransform (matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5]);

				return;
			}			

			oTransformer.Rotate (this, pCommander, nFlags);

			//if (0.0 != GetAngle ())
			//{
			//	oTransformer.Rotate (this, pCommander, nFlags);
			//}
			//else
			//{
			//	CCoordTransformer:FlipTransform(this, pCommander, nFlags);
				// pCommander->SetTransform ( 1, 0, 0, 1, 0, 0 );
			//}
		}

		inline BOOL NotValidTime ()
		{
			if (m_pLayer->GetTime() < m_dTimeBegin || m_dTimeEnd <= m_pLayer->GetTime())
				return TRUE;

			return FALSE;			
		}

	protected:

		double			m_dXWidthMM;
		double			m_dYHeightMM;

		CFadeInEffect*	m_pActualEffect;
	};	
}

namespace Animation
{
	class CAnimateImage : public CShowElement, public ImageStudio::Serialize::Paint::Common::CDrawImageFromFile
	{
	public:

		CAnimateImage ()
		{
			m_nType			=	ElementImage;

			m_pLayer		=	NULL;
			m_bstrPath		=	NULL;
			m_pCachedImage	=	NULL;
		}

		CAnimateImage (ILayer* pLayer, CCachedCommands* pCachedImage, CDoubleRect oBounds, double dAngle, double dPW, double dPH, long lFlags) 
		{
			m_nType			=	ElementImage;
			m_lFlags		=	lFlags;

			m_pLayer		=	pLayer;
			m_pCachedImage	=	pCachedImage;
			m_oBounds		=	oBounds;
			Angle			=	dAngle;

			Left			=	m_oBounds.left;
			Top				=	m_oBounds.top;
			Right			=	m_oBounds.right;
			Bottom			=	m_oBounds.bottom;

			m_dWidthMM		=	dPW;
			m_dHeightMM		=	dPH;
			m_dXWidthMM		=	dPW;
			m_dYHeightMM	=	dPH;

			m_bstrPath		=	NULL;
		}

		~CAnimateImage ()
		{
			SysFreeString(m_bstrPath);
		}



		//
		virtual void Render (IAVSRenderer* pCommander)
		{
			if (NULL == pCommander)
				return;

			if ( m_pLayer )
			{
				if (CShowElement::NotValidTime ())
					return;
			}

			double dBlend	=	1.0;
			if (m_pLayer)
			{
				if (m_pLayer->GetTransparency (dBlend) )
				{
					if (dBlend == 0.0)
						return;
				}
			}

			if (DrawInnerCacheImage(pCommander,dBlend))
				return;

			if (NULL == m_oImage.GetImage())
			{
				CString strOld = FilePath;

				CFileDownloader* pDownloader = NULL;

				if (IsNeedDownload())
				{
					pDownloader = new CFileDownloader(FilePath, TRUE);
					pDownloader->StartWork( 1 );
					while ( pDownloader->IsRunned() )
					{
						::Sleep( 10 );
					}

					if ( pDownloader->IsFileDownloaded() )
					{
						FilePath = pDownloader->GetFilePath();
					}
				}

				if (m_pCache)
					m_oImage.Init ( m_pCache->GenTempFilePath(), CString ( FilePath ));

				RELEASEOBJECT(pDownloader);
			}

			if (m_oImage.GetImage())
			{
				UpdateTransform (pCommander, m_lFlags);

				pCommander->put_Width((float)m_dWidthMM);
				pCommander->put_Height((float)m_dHeightMM);

				if ( m_pLayer )
				{
					if ( m_pActualEffect )
					{
						if ( CreditsEffect == m_pActualEffect->GetID () )	// После такого эффекта объект больше не появляется
							return;
					}

					CEffect* pCompose		=	((CAnimationLayer*)m_pLayer)->GetCompose();
					if (pCompose)
					{
						IUnknown* pImage	=	NULL;
						if (m_oBuffCompose.ApplyEffect (pCompose, this, pImage) )
						{
							CBlendImage oBlendImage (pImage);
							pCommander->DrawImage (oBlendImage.Get(dBlend), (float)Left, (float)(Top), (float)(Right - Left), (float)(Bottom - Top));
							return;
						}
					}
				}

				CBlendImage oBlendImage (m_oImage.GetImage());
				pCommander->DrawImage ( oBlendImage.Get(dBlend), (float)Left, (float)(Top), (float)(Right - Left), (float)(Bottom - Top));
			}
		}
		virtual void FromXml (XmlUtils::CXmlNode& oXmlNode)
		{
			InternalFromXmlNode ( oXmlNode );

			//  <timeline type = "1"  begin="0.000000" end="30000.000000" fadein="0" fadeout="0" completeness="1.0"/>

			XmlUtils::CXmlNode oTimeNode;
			if ( oXmlNode.GetNode ( _T("timeline"), oTimeNode ) )
			{
				m_dTimeBegin	=	_tstof ( oTimeNode.GetAttributeOrValue ( _T("begin"), _T("0.0") ) );
				m_dTimeEnd		=	_tstof ( oTimeNode.GetAttributeOrValue ( _T("end"), _T("0.0") ) );
			}

			m_oBounds.left		=	Left;
			m_oBounds.top		=	Top;
			m_oBounds.right		=	Right;
			m_oBounds.bottom	=	Bottom;

			m_dXWidthMM			=	m_dWidthMM;
			m_dYHeightMM		=	m_dHeightMM;

			m_oBaseBounds		=	m_oBounds;
		}

		//
		virtual const CDoubleRect& GetBounds ()
		{ 
			return m_oBounds;
		}

		virtual double GetAngle ()	
		{
			return Angle;	
		}

		//
		virtual void ClearCache ()
		{
			m_oBuffCompose.ClearMemory ();
			m_oImage.ClearMemory ();
		}
		virtual IUnknown* GetBufferImage ()
		{
			if (m_pCachedImage)
				return m_pCachedImage->GetImage();

			if (m_oImage.GetImage ())
				return m_oImage.GetImage ();

			return NULL;
		}

	private:

		inline BOOL DrawInnerCacheImage (IAVSRenderer* pCommander, double dBlend)
		{
			if (m_pCachedImage)
			{
				if (m_pCachedImage->GetImage ())
				{
					UpdateTransform (pCommander, m_lFlags);

					pCommander->put_Width((float)m_dWidthMM);
					pCommander->put_Height((float)m_dHeightMM);

					if ( m_pLayer )
					{
						if ( m_pActualEffect )
						{
							if ( CreditsEffect == m_pActualEffect->GetID () )	// После такого эффекта объект больше не появляется
								return TRUE;
						}

						CEffect* pCompose		=	((CAnimationLayer*)m_pLayer)->GetCompose();
						if (pCompose)
						{
							IUnknown* pImage	=	NULL;
							if (m_oBuffCompose.ApplyEffect (pCompose, this, pImage) )
							{
								CBlendImage oBlendImage (pImage);
								pCommander->DrawImage (oBlendImage.Get(dBlend), (float)Left, (float)(Top), (float)(Right - Left), (float)(Bottom - Top));
								return TRUE;
							}
						}
					}

					CBlendImage oBlendImage (m_pCachedImage->GetImage());
					pCommander->DrawImage ( oBlendImage.Get(dBlend), (float)Left, (float)(Top), (float)(Right - Left), (float)(Bottom - Top));

					return TRUE;
				}
			}

			return FALSE;
		}


	protected:

		BSTR				m_bstrPath;
		CCacheImageFile		m_oImage;
		CDoubleRect			m_oBounds;

		CCachedCommands*	m_pCachedImage;
		CComposeTransformer	m_oBuffCompose;
	};

	class CAnimatePath : public CShowElement, public ImageStudio::Serialize::Paint::Common::CDrawGraphicPath
	{
	public:

		CAnimatePath ()
		{
			m_nType			=	ElementPath;

			m_pLayer		=	NULL;
			m_bsTexturePath	=	NULL;
			m_bSmoothAlpha	=	TRUE;

			m_pCImage		=	NULL;
		}

		~CAnimatePath ()
		{
			if ( m_bsTexturePath )
				SysFreeString(m_bsTexturePath);

			RELEASEOBJECT(m_pCImage);
		}

		//
		virtual void Render (IAVSRenderer* pCommander)
		{
			if (NULL == pCommander)
				return;

			if ( m_pLayer )
			{
				if (CShowElement::NotValidTime ())
					return;
			}

			pCommander->put_Width((float)m_dWidthMM);
			pCommander->put_Height((float)m_dHeightMM);

			if ( m_bInternalCache )
			{
				if ( DrawToStaticCache (pCommander) )
					return;

				if ( DrawToDynamicCache (pCommander) )
					return;
			}

			DrawToCommander (pCommander);
		}

		virtual void FromXml (XmlUtils::CXmlNode& oXmlNode)
		{
			InternalFromXmlNode ( oXmlNode );

			m_bsTexturePath		=	Brush.m_oBrush.TexturePath.AllocSysString();

			//  <timeline type = "1"  begin="0.000000" end="30000.000000" fadein="0" fadeout="0" completeness="1.0"/>

			XmlUtils::CXmlNode oTimeNode;
			if ( oXmlNode.GetNode ( _T("timeline"), oTimeNode ) )
			{
				m_dTimeBegin	=	_tstof ( oTimeNode.GetAttributeOrValue ( _T("begin"), _T("0.0") ) );
				m_dTimeEnd		=	_tstof ( oTimeNode.GetAttributeOrValue ( _T("end"), _T("0.0") ) );
			}

			m_dXWidthMM			=	m_dWidthMM;
			m_dYHeightMM		=	m_dHeightMM;

			m_oBaseBounds		=	m_oBounds;
		}

		//
		virtual const CDoubleRect& GetBounds ()
		{
			return m_oBounds;
		}

		virtual double GetAngle ()	
		{
			return m_dAngle;	
		}

		//
		virtual void ClearCache ()
		{
			m_oImageCache.MoveToFile ();

			if (m_pCImage)
			{
				m_pCImage->ClearCache();
			}
		}

		virtual void SetCacheReferences (CCacheStorage* pCache)
		{
			IShowElement::SetCacheReferences (pCache);

			if (pCache)
				pCache->AddRef (&m_oImageCache);
		}

	private:

		inline BOOL DrawToStaticCache (IAVSRenderer* pCommander)
		{
			if (NULL == m_pLayer)
			{
				CDoubleRect	oMarginBounds	=	CCoordTransformer::MarginBounds (m_oBounds);
				oMarginBounds				=	CCoordTransformer::BoundRotate::Transform (oMarginBounds, m_dAngle);

				if ( m_oImageCache.CreateImage ( pCommander, oMarginBounds ) )
				{
					IAVSRenderer* pCommander2 =	m_oImageCache.GetRender ();
					if ( pCommander2 )
					{		
						CCoordTransformer oTransformer(m_dXWidthMM, m_dYHeightMM);
						oTransformer.StaticTransform (this, pCommander2);

						DrawToCommander ( pCommander2, FALSE );

						m_oImageCache.Refresh ();
					}

					RELEASEINTERFACE ( pCommander2 );
				}

				if ( m_oImageCache.GetImage () )
				{
					pCommander->SetTransform ( 1, 0, 0, 1, 0, 0 );
					pCommander->DrawImage ( m_oImageCache.GetImage (), (float)oMarginBounds.left, (float)(oMarginBounds.top), (float)(oMarginBounds.GetWidth()), (float)(oMarginBounds.GetHeight()));
				}

				return TRUE;
			}

			return FALSE;;
		}


		inline BOOL DrawToDynamicCache (IAVSRenderer* pCommander)
		{
			if (m_pLayer)
			{
				if ( ((CAnimationLayer*)m_pLayer)->IsChangeFillColor() )	//	увы это не можем закэшировать
					return FALSE;

				CDoubleRect	oMarginBounds	=	CCoordTransformer::MarginBounds (m_oBounds);

				if ( m_oImageCache.CreateImage ( pCommander, oMarginBounds ) )
				{
					IAVSRenderer* pCommander2 =	m_oImageCache.GetRender ();
					if ( pCommander2 )
					{		
						CCoordTransformer oTransformer(m_dXWidthMM, m_dYHeightMM);
						oTransformer.StaticTransform (this, pCommander2, FALSE);

						DrawToCommander ( pCommander2, FALSE, TRUE );

						m_oImageCache.Refresh ();
					}

					RELEASEINTERFACE ( pCommander2 );
				}

				if (NULL == m_pCImage)
					m_pCImage	=	new CAnimateImage (m_pLayer, &m_oImageCache, oMarginBounds, m_dAngle, m_dXWidthMM, m_dYHeightMM, m_lFlags);

				if (m_pCImage)
				{
					m_pCImage->SetTimes (m_dTimeBegin,m_dTimeEnd); 
					m_pCImage->SetBaseBounds (m_oBaseBounds);
					m_pCImage->Render (pCommander); 
				}

				return TRUE;
			}

			return FALSE;
		}

		inline BOOL DrawToCommander (IAVSRenderer* pCommander, BOOL bUpdateTransform = TRUE, BOOL BlendOff	= FALSE)
		{
			if (BlendOff)
			{
				pCommander->put_PenAlpha(Pen.m_oPen.Alpha);
				pCommander->put_BrushAlpha1(Brush.m_oBrush.Alpha1);
				pCommander->put_BrushAlpha2(Brush.m_oBrush.Alpha2);
			}
			else
			{
				double dTransparency	=	1.0;
				if ( m_pLayer )
				{
					if ( m_pLayer->GetTransparency ( dTransparency ) )
					{
						if ( 0.0 == dTransparency )
							return TRUE;

						pCommander->put_PenAlpha ( static_cast<LONG> ( Pen.m_oPen.Alpha * dTransparency ) );
						pCommander->put_BrushAlpha1 ( static_cast<LONG> ( Brush.m_oBrush.Alpha1 * dTransparency ) );
						pCommander->put_BrushAlpha2 ( static_cast<LONG> ( Brush.m_oBrush.Alpha2 * dTransparency ) );
					}
					else
					{
						pCommander->put_PenAlpha(Pen.m_oPen.Alpha);
						pCommander->put_BrushAlpha1(Brush.m_oBrush.Alpha1);
						pCommander->put_BrushAlpha2(Brush.m_oBrush.Alpha2);
					}
				}
				else
				{
					pCommander->put_PenAlpha(Pen.m_oPen.Alpha);
					pCommander->put_BrushAlpha1(Brush.m_oBrush.Alpha1);
					pCommander->put_BrushAlpha2(Brush.m_oBrush.Alpha2);
				}
			}

			if ( bUpdateTransform )
			{
				UpdateTransform (pCommander, m_lFlags);
			}

			if ( m_pLayer )
			{
				if ( m_pActualEffect )
				{
					if ( CreditsEffect == m_pActualEffect->GetID () )	// После такого эффекта объект больше не появляется
						return TRUE;
				}
			}

			long nPenColor				=	Pen.m_oPen.Color;
			long nBrushColor01			=	Brush.m_oBrush.Color1;
			long nBrushColor02			=	Brush.m_oBrush.Color2;

			if (m_pLayer)
			{
				CColorTransformer oTransformer (TRUE);
				if (oTransformer.CollectOperations (this))
				{
					nPenColor			=	oTransformer.ApplyColor (nPenColor);
					nBrushColor01		=	oTransformer.ApplyColor (nBrushColor01);
					nBrushColor02		=	oTransformer.ApplyColor (nBrushColor02);

					if (oTransformer.FlickerStatus())
					{
						nBrushColor01	=	oTransformer.ApplyFlickerColor (nBrushColor01);
						nBrushColor02	=	oTransformer.ApplyFlickerColor (nBrushColor02);
					}

					if (oTransformer.LinesStatus())
					{
						nPenColor		=	oTransformer.ApplyLinesColor (nPenColor);
					}

					if (oTransformer.FillStatus ())
					{
						nBrushColor01	=	oTransformer.ApplyFillColor (nBrushColor01);
						nBrushColor02	=	oTransformer.ApplyFillColor (nBrushColor02);
					}
				}
			}

			pCommander->put_PenColor(nPenColor);
			pCommander->put_PenSize(Pen.m_oPen.Size);
			pCommander->put_PenDashStyle(Pen.m_oPen.DashStyle);
			pCommander->put_PenLineStartCap(Pen.m_oPen.LineStartCap);
			pCommander->put_PenLineEndCap(Pen.m_oPen.LineEndCap);
			pCommander->put_PenLineJoin(Pen.m_oPen.LineJoin);
			pCommander->put_PenDashOffset(Pen.m_oPen.DashOffset);
			pCommander->put_PenAlign(Pen.m_oPen.Align);
			pCommander->put_PenMiterLimit(Pen.m_oPen.MiterLimit);
			//pCommander->put_PenDashPattern([in, satype("double")] SAFEARRAY* pPattern);

			pCommander->put_BrushType(Brush.m_oBrush.Type);
			pCommander->put_BrushColor1(nBrushColor01);
			pCommander->put_BrushColor2(nBrushColor02);
			pCommander->put_BrushTexturePath(m_bsTexturePath); 
			pCommander->put_BrushTextureMode(Brush.m_oBrush.TextureMode);
			pCommander->put_BrushTextureAlpha(Brush.m_oBrush.TextureAlpha);
			pCommander->put_BrushLinearAngle(Brush.m_oBrush.LinearAngle);
			pCommander->BrushRect(Brush.m_oBrush.Rectable, Brush.m_oBrush.Rect.X, Brush.m_oBrush.Rect.Y, Brush.m_oBrush.Rect.Width, Brush.m_oBrush.Rect.Height );

			pCommander->PathCommandStart();
			pCommander->BeginCommand(c_nPathType);

			PointF pointCur; pointCur.X = 0; pointCur.Y = 0;
			for (int nIndex = 0; nIndex < m_arParts.GetSize(); ++nIndex)
			{
				m_arParts[nIndex].Draw(pCommander, pointCur);
			}

			LONG lType = 0;
			if (m_bStroke)
			{
				lType = 1;
			}

			if (m_bFill)
			{
				lType += c_nWindingFillMode;
			}

			pCommander->DrawPath(lType);

			pCommander->EndCommand(c_nPathType);
			pCommander->PathCommandEnd ();

			return TRUE;
		}

	protected:

		BSTR	m_bsTexturePath;
		BOOL	m_bSmoothAlpha;

		CCachedCommands		m_oImageCache;		//	STATIC 
		CAnimateImage*		m_pCImage;			//	DYNAMIC
	};

	class CAnimateText : public CShowElement, public ImageStudio::Serialize::Paint::Common::CTextMeasurer
	{
	public:
		class CParagraph : public CShowElement
		{
		public:
			CParagraph(TextMeasurer2 oMeasurer, long nTB, CAnimateText* pText) : m_oMeasurer (oMeasurer), m_lFlags(0)
			{		
				m_pImage			=	NULL;

				if (pText)
				{
					m_dXWidthMM		=	pText->m_dWidthMetric;
					m_dYHeightMM	=	pText->m_dHeightMetric;					
					m_dAngle		=	pText->GetAngle ();
					m_oBounds		=	pText->GetBounds ();
					m_oBaseBounds	=	pText->GetBounds ();

					m_dTimeBegin	=	pText->GetBeginTime ();
					m_dTimeEnd		=	pText->GetEndTime ();

					m_dAlpha1		=	0.0;
					m_dAlpha2		=	0.0;

					m_lFlags		=	pText->m_lFlags;
				}

				InitEffects (nTB, (CAnimationLayer*)pText->GetBaseLayer());
			}

			~CParagraph()
			{
				m_oLayer.ClearReferences ();

				RELEASEOBJECT (m_pImage);
			}

			inline void SetTime (double dTime)
			{
				m_oLayer.SetTime (dTime);
			}

			inline void SetAlpha (double dA1, double dA2)
			{
				m_dAlpha1	=	dA1;
				m_dAlpha2	=	dA2;
			}
			//
			virtual void Render (IAVSRenderer* pCommander)
			{
				DrawImage (pCommander);
			}

			virtual void FromXml (XmlUtils::CXmlNode& oXmlNode)
			{

			}

			virtual void ClearCache ()
			{
				m_oImageCache.MoveToFile ();

				if (m_pImage)
					m_pImage->ClearCache();
			}
			virtual void SetCacheReferences (CCacheStorage* pCache)
			{
				IShowElement::SetCacheReferences (pCache);

				if (pCache)
					pCache->AddRef (&m_oImageCache);
			}

			//
			virtual const CDoubleRect& GetBounds ()
			{
				return m_oBounds;
			}

			virtual double GetAngle ()	
			{
				return m_dAngle;	
			}

		private:

			inline void InitEffects (long nTB, CAnimationLayer* pLayer)
			{
				if (pLayer)
				{
					CAtlArray <CEffect*>& oEffects	=	pLayer->Effects();
					for ( long i = 0; i < (long)oEffects.GetCount(); ++i )
					{
						if ( nTB == oEffects.GetAt(i)->m_nTextBlock )
							m_oLayer.AddEffect (oEffects.GetAt(i));
					}

					SetBaseLayer (&m_oLayer);

#define PT_TO_MM (25.4f/72.0f)

					m_oBounds.top		=	m_oMeasurer.GetOffset() * PT_TO_MM;
					m_oBounds.bottom	=	m_oBounds.top + m_oMeasurer.GetHeight()* PT_TO_MM;
				}
			}

			inline BOOL DrawImage (IAVSRenderer* pCommander)
			{	
				/*
				if ( m_oLayer.IsChangeFillColor() )	//	увы это не можем закэшировать
				{
				RELEASEOBJECT (m_pImage);
				return FALSE;
				}
				*/

				CDoubleRect	oMarginBounds	=	CCoordTransformer::MarginBounds (m_oBounds, 0.0);

				if ( m_oImageCache.CreateImage ( pCommander, oMarginBounds ) )
				{
					IAVSRenderer* pCommander2 =	m_oImageCache.GetRender ();
					if ( pCommander2 )
					{		
						CCoordTransformer oTransformer (m_dXWidthMM, m_dYHeightMM);
						oTransformer.StaticTransform (this, pCommander2, FALSE, 0.0);

						DrawToCommander ( pCommander2, FALSE, TRUE );

						m_oImageCache.Refresh ();
					}

					RELEASEINTERFACE ( pCommander2 );
				}

				if (NULL == m_pImage)		
				{
					m_pImage = new CAnimateImage (&m_oLayer, &m_oImageCache, oMarginBounds, m_dAngle, m_dXWidthMM, m_dYHeightMM, m_lFlags);
					if (m_pImage)
					{
						m_pImage->SetTimes (m_dTimeBegin,m_dTimeEnd); 
						m_pImage->SetBaseBounds (m_oBounds);
						m_pImage->SetBaseBounds (m_oBaseBounds);
					}
				}

				if (m_pImage)
					m_pImage->Render (pCommander); 

				return TRUE;
			}

			inline BOOL DrawToCommander (IAVSRenderer* pCommander, BOOL bUpdateTransform = TRUE, BOOL BlendOff	= FALSE)
			{
				if ( bUpdateTransform )
				{
					UpdateTransform (pCommander, 0);
				}

				m_oMeasurer.DrawFirstPage (pCommander, 1.0);

				return TRUE;
			}

		private:

			TextMeasurer2		m_oMeasurer;
			CAnimationLayer		m_oLayer;

			double				m_dAngle;
			CDoubleRect			m_oBounds;

			CCachedCommands		m_oImageCache;			//	STATIC 
			CAnimateImage*		m_pImage;				//	DYNAMIC

			double				m_dAlpha1;
			double				m_dAlpha2;

			long				m_lFlags;
		};

		class CParagraphComposer
		{
		public:
			CParagraphComposer ()
			{
				m_nStatus	=	-1;	//	Open

				m_bSetCache	=	FALSE;
				m_pCache	=	NULL;
			}

			~CParagraphComposer ()
			{
				for ( long i = 0; i < (long)m_arParagraphs.GetCount(); ++i )
					RELEASEOBJECT (m_arParagraphs[i]);
			}

			inline BOOL Init (IAVSRenderer* pCommander, CAnimateText* pText)
			{
				if (pText)
				{
					if (pCommander)
					{
						pText->PrepareFirstPage (pCommander);

						if ( HaveSplitToSections (pText) )
						{
							CArray<TextMeasurer2> oMeasurers;

							bool removeEmpty	=	false;
							ILayer* pLayer		=	pText->GetBaseLayer ();
							if (pLayer)
							{
								CAnimationLayer* pALayer = static_cast<CAnimationLayer*> (pLayer);
								if (pALayer)
								{
									CAtlArray <CEffect*>& oEffects	=	pALayer->Effects();
									for ( long i = 0; i < (long)oEffects.GetCount(); ++i )
									{
										if (oEffects.GetAt(i)->m_bRemoveEmptyBlocks)
										{
											removeEmpty	=	true;
											break;
										}
									}
								}
							}

							pText->TruncateByBlocks (oMeasurers, removeEmpty);

							if ((long)oMeasurers.GetCount())
							{		
								for ( long i = 0; i < (long)oMeasurers.GetCount(); ++i)
								{
									CParagraph*	pPar = new CParagraph (oMeasurers.GetAt(i), i, pText );
									if (pPar)
									{
										m_arParagraphs.Add (pPar);
									}
								}

								m_nStatus	=	1;

								return TRUE;
							}
						}
					}
				}

				m_nStatus		=	0;

				return FALSE;
			}

			inline long Status ()
			{
				return m_nStatus;
			}

			inline BOOL Render (IAVSRenderer* pCommander, ILayer* pLayer)
			{
				if (m_nStatus > 0)
				{
					SetCache ();

					if (pLayer)
						SetTime (pLayer->GetTime());

					for ( long i = 0; i < (long)m_arParagraphs.GetCount(); ++i )
						m_arParagraphs [i]->Render (pCommander);

					return TRUE;
				}

				return FALSE;
			}

			inline void ClearCache ()
			{
				if (m_nStatus > 0)
				{
					for ( long i = 0; i < (long)m_arParagraphs.GetCount(); ++i )
						m_arParagraphs [i]->ClearCache ();
				}
			}


			inline void SetCacheReferences (CCacheStorage* pCache)
			{
				m_pCache	=	pCache;
			}

		private:

			inline BOOL HaveSplitToSections (CAnimateText* pText)	//	
			{
				if (pText)
				{
					ILayer* Layer	=	pText->GetBaseLayer ();
					if (NULL == Layer)
						return FALSE;

					CAnimationLayer* pLayer	=	static_cast<CAnimationLayer*>(Layer);
					if (pLayer)
					{
						CAtlArray <CEffect*>& oEffects	=	pLayer->Effects();
						if ( (long)oEffects.GetCount() <= 1 )
							return FALSE;

						long nBlock	= oEffects.GetAt(0)->m_nTextBlock;

						for ( long i = 0; i < (long)oEffects.GetCount(); ++i )
						{
							long Cur = oEffects.GetAt(i)->m_nTextBlock;
							if (nBlock != Cur)
								return TRUE;
						}
					}
				}

				return FALSE;
			}

			inline void SetTime (double dTime)
			{
				for ( long i = 0; i < (long)m_arParagraphs.GetCount(); ++i )
					m_arParagraphs [i]->SetTime (dTime);
			}

			inline void SetCache ()
			{
				if (m_bSetCache)
					return;

				if (m_pCache)
				{
					long Count = (long)m_arParagraphs.GetCount();
					if (Count)
					{
						for ( long i = 0; i < Count; ++i )
							m_arParagraphs [i]->SetCacheReferences (m_pCache);

						m_bSetCache	=	TRUE;
					}
				}
			}

		private:

			LONG	m_nStatus;

			CAtlArray <CParagraph*> m_arParagraphs;

			BOOL	m_bSetCache;
			CCacheStorage* m_pCache;
		};

	public:

		CAnimateText (IAVSFontManager* pFontManager) : ImageStudio::Serialize::Paint::Common::CTextMeasurer(pFontManager)
		{
			m_nType					=	ElementText;

			m_bStatus				=	FALSE;

			m_pLayer				=	NULL;
			m_bApplyColorTransform	=	FALSE;
			m_pTextBase				=	NULL;

			m_pFontManager			=	pFontManager;
			m_pCImage				=	NULL;

			m_nUseMeasurers			=	-1;
			m_bInitMeasurers		=	FALSE;
		}

		~CAnimateText ()
		{
			RELEASEOBJECT(m_pTextBase);
			RELEASEOBJECT(m_pCImage);
		}
		//
		virtual void Render (IAVSRenderer* pCommander)
		{
			if (NULL == pCommander)
				return;

			if (m_pLayer)
			{
				if (CShowElement::NotValidTime ())
					return;
			}

			pCommander->put_Width((float)m_dWidthMetric);
			pCommander->put_Height((float)m_dHeightMetric);

			if (-1 == m_oParComposer.Status ())
				m_oParComposer.Init (pCommander, this);

			if (1 == m_oParComposer.Status ())
			{
				m_oParComposer.Render (pCommander, m_pLayer);
				return;
			}

			if (m_bInternalCache)
			{
				if ( DrawToStaticCache (pCommander) )
					return;

				if ( DrawToDynamicCache (pCommander) )
					return;
			}

			double dTransparency	=	1.0;
			if ( m_pLayer )
			{
				if ( m_pLayer->GetTransparency ( dTransparency ) )
				{
					if ( dTransparency == 0.0 )
						return;
				}
			}

			UpdateTransform (pCommander, m_lFlags);

			if ( m_pLayer )
			{
				if ( m_pActualEffect )
				{
					if ( CreditsEffect == m_pActualEffect->GetID () )	// После такого эффекта объект больше не появляется
						return;
				}
			}

			m_oBrush.m_oBrush.Alpha1	=	dTransparency * m_nAlpha1;
			m_oBrush.m_oBrush.Alpha2	=	dTransparency * m_nAlpha2;

			if (NULL==m_pTextBase)
				m_pTextBase = new CTextMeasurer(m_pFontManager);
			if (m_pTextBase)
			{						
				m_pTextBase->PrepareFirstPage (pCommander);
			}

			PrepareFirstPage (pCommander);

			ApplyColorTransform ();			

			CAnimateText::DrawFirstPage (pCommander, dTransparency);			
		}

		virtual void FromXml (XmlUtils::CXmlNode& oXmlNode)
		{
			m_dWidthMetric		=	XmlUtils::GetDouble ( oXmlNode.GetAttributeOrValue ( _T("widthmm"),  _T("100") ) );
			m_dHeightMetric		=	XmlUtils::GetDouble ( oXmlNode.GetAttributeOrValue ( _T("heightmm"), _T("100") ) );

			CTextMeasurer::InternalFromXmlNode ( oXmlNode );

			XmlUtils::CXmlNode oTimeNode;
			if ( oXmlNode.GetNode ( _T("timeline"), oTimeNode ) )
			{
				m_dTimeBegin	=	_tstof ( oTimeNode.GetAttributeOrValue ( _T("begin"), _T("0.0") ) );
				m_dTimeEnd		=	_tstof ( oTimeNode.GetAttributeOrValue ( _T("end"), _T("0.0") ) );
			}

			m_nAlpha1			=	m_oBrush.m_oBrush.Alpha1;
			m_nAlpha2			=	m_oBrush.m_oBrush.Alpha2;

			m_dXWidthMM			=	m_dWidthMetric;
			m_dYHeightMM		=	m_dHeightMetric;

			if (NULL==m_pTextBase)
				m_pTextBase		=	new CTextMeasurer(m_pFontManager);
			if (m_pTextBase)
			{						
				m_pTextBase->InternalFromXmlNode (oXmlNode);
			}

			m_oBounds.left		=	m_oLayoutRect.GetLeft ();
			m_oBounds.top		=	m_oLayoutRect.GetTop ();
			m_oBounds.right		=	m_oLayoutRect.GetRight ();
			m_oBounds.bottom	=	m_oLayoutRect.GetBottom ();

			XmlUtils::CXmlNode oBoundsNode;
			if ( oXmlNode.GetNode ( _T("bounds"), oBoundsNode ) )
			{
				m_oBaseBounds.left		=	Strings::ToDouble(oBoundsNode.GetAttributeOrValue(_T("left"),	_T("0")));
				m_oBaseBounds.top		=	Strings::ToDouble(oBoundsNode.GetAttributeOrValue(_T("top"),	_T("0")));
				m_oBaseBounds.right		=	Strings::ToDouble(oBoundsNode.GetAttributeOrValue(_T("right"),	_T("0")));
				m_oBaseBounds.bottom	=	Strings::ToDouble(oBoundsNode.GetAttributeOrValue(_T("bottom"),	_T("0")));
			}

			if (m_oBaseBounds.GetWidth () < m_oBounds.GetWidth ())
			{
				m_oBaseBounds.left		=	m_oBounds.left;
				m_oBaseBounds.right		=	m_oBounds.right;
			}

			if (m_oBaseBounds.GetHeight () < m_oBounds.GetHeight ())
			{
				m_oBaseBounds.top		=	m_oBounds.top;
				m_oBaseBounds.bottom	=	m_oBounds.bottom;
			}
		}

		//
		virtual const CDoubleRect& GetBounds ()
		{
			return m_oBounds;
		}

		virtual double GetAngle ()	
		{
			return m_dAngle;	
		}


		//		
		virtual void ClearCache ()
		{
			m_oImageCache.MoveToFile ();

			if (m_pCImage)
			{
				m_pCImage->ClearCache();
			}

			m_oParComposer.ClearCache ();
		}

		virtual void SetCacheReferences (CCacheStorage* pCache)
		{
			IShowElement::SetCacheReferences (pCache);

			if (pCache)
				pCache->AddRef (&m_oImageCache);

			m_oParComposer.SetCacheReferences (pCache);
		}

	protected:

		inline BOOL ApplyColorTransform ()
		{
			if (m_pLayer)
			{
				CColorTransformer oTransformer;
				if (oTransformer.CollectOperations (this))
				{
					if (NULL==m_pTextBase)
						m_pTextBase = new CTextMeasurer(m_pFontManager);
					if (NULL==m_pTextBase)
						return FALSE;

					if (0 == m_pTextBase->m_arrPages.GetCount () || 0 == m_arrPages.GetCount())
						return FALSE;

					CPage* pPage		=	m_pTextBase->m_arrPages[0];
					CPage* pDrawPage	=	m_arrPages[0];
					if ( NULL != pPage && NULL != pDrawPage )
					{	
						if (oTransformer.FlickerStatus())
						{
							CAtlArray<CLine*>& arrLines			=	pPage->GetRefLines ();
							CAtlArray<CLine*>& arrDrawLines		=	pDrawPage->GetRefLines ();
							for ( long i = 0; i < (long)arrLines.GetCount (); ++i )
							{
								CAtlArray<CItem*>* pItems		=	arrLines[i]->GetItems();
								CAtlArray<CItem*>* pDrawItems	=	arrDrawLines[i]->GetItems();
								if ( NULL != pItems && NULL != pDrawItems )
								{
									for ( long j = 0; j < (long)pItems->GetCount (); ++j)
									{
										CSymbols* pItem			=	(CSymbols*)pItems->operator[](j);									
										CSymbols* pDrawItem		=	(CSymbols*)pDrawItems->operator[](j);									

										pDrawItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1			=	oTransformer.ApplyColor (pItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1);
										pDrawItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color2			=	oTransformer.ApplyColor (pItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color2);
										pDrawItem->GetCharacter()->m_oAttributes.m_oShadow.m_oShadow.Color			=	oTransformer.ApplyColor (pItem->GetCharacter()->m_oAttributes.m_oShadow.m_oShadow.Color);
										pDrawItem->GetCharacter()->m_oAttributes.m_oBackground.m_oBackground.Color	=	oTransformer.ApplyColor (pItem->GetCharacter()->m_oAttributes.m_oBackground.m_oBackground.Color);
										//pDrawItem->GetCharacter()->m_oAttributes.m_oEdge.m_oEdge.Color			=	oTransformer.ApplyColor (pItem->GetCharacter()->m_oAttributes.m_oEdge.m_oEdge.Color);

										pDrawItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1			=	oTransformer.ApplyFlickerColor (pItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1);
										pDrawItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color2			=	oTransformer.ApplyFlickerColor (pItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color2);
										pDrawItem->GetCharacter()->m_oAttributes.m_oShadow.m_oShadow.Color			=	oTransformer.ApplyFlickerColor (pItem->GetCharacter()->m_oAttributes.m_oShadow.m_oShadow.Color);
										pDrawItem->GetCharacter()->m_oAttributes.m_oBackground.m_oBackground.Color	=	oTransformer.ApplyFlickerColor (pItem->GetCharacter()->m_oAttributes.m_oBackground.m_oBackground.Color);

										if (oTransformer.FillObjStatus ())
										{
											pDrawItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1		=	oTransformer.ApplyFillObjColor (pItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1);
											pDrawItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color2		=	oTransformer.ApplyFillObjColor (pItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color2);
										}

										if (oTransformer.FontColorStatus())
										{
											pDrawItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1		=	oTransformer.ApplyFontColor (pItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1);
											pDrawItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color2		=	oTransformer.ApplyFontColor (pItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color2);
										}
									}

									m_bApplyColorTransform	=	TRUE;
									return TRUE;
								}
							}
						}
						else
						{
							CAtlArray<CLine*>& arrLines			=	pPage->GetRefLines ();
							CAtlArray<CLine*>& arrDrawLines		=	pDrawPage->GetRefLines ();
							for ( long i = 0; i < (long)arrLines.GetCount (); ++i )
							{
								CAtlArray<CItem*>* pItems		=	arrLines[i]->GetItems();
								CAtlArray<CItem*>* pDrawItems	=	arrDrawLines[i]->GetItems();
								if ( NULL != pItems && NULL != pDrawItems )
								{
									for ( long j = 0; j < (long)pItems->GetCount (); ++j)
									{
										CSymbols* pItem			=	(CSymbols*)pItems->operator[](j);									
										CSymbols* pDrawItem		=	(CSymbols*)pDrawItems->operator[](j);									

										pDrawItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1			=	oTransformer.ApplyColor (pItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1);
										pDrawItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color2			=	oTransformer.ApplyColor (pItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color2);
										pDrawItem->GetCharacter()->m_oAttributes.m_oShadow.m_oShadow.Color			=	oTransformer.ApplyColor (pItem->GetCharacter()->m_oAttributes.m_oShadow.m_oShadow.Color);
										pDrawItem->GetCharacter()->m_oAttributes.m_oBackground.m_oBackground.Color	=	oTransformer.ApplyColor (pItem->GetCharacter()->m_oAttributes.m_oBackground.m_oBackground.Color);
										//pDrawItem->GetCharacter()->m_oAttributes.m_oEdge.m_oEdge.Color			=	oTransformer.ApplyColor (pItem->GetCharacter()->m_oAttributes.m_oEdge.m_oEdge.Color);

										if (oTransformer.FillObjStatus ())
										{
											pDrawItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1		=	oTransformer.ApplyFillObjColor (pItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1);
											pDrawItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color2		=	oTransformer.ApplyFillObjColor (pItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color2);
										}

										if (oTransformer.FontColorStatus())
										{
											pDrawItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1		=	oTransformer.ApplyFontColor (pItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1);
											pDrawItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color2		=	oTransformer.ApplyFontColor (pItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color2);
										}
									}

									m_bApplyColorTransform	=	TRUE;
									return TRUE;
								}
							}
						}
					}
				}

				SetDefaultColors ();	//	сбрасывание цветовую трансформу
			}

			return FALSE;
		}

		inline BOOL SetDefaultColors ()
		{
			if (NULL==m_pTextBase)
				m_pTextBase = new CTextMeasurer(m_pFontManager);
			if (NULL==m_pTextBase)
				return FALSE;

			if ( 0 == m_pTextBase->m_arrPages.GetCount () || 0 == m_arrPages.GetCount() )
				return FALSE;

			if (m_bApplyColorTransform)
			{
				ImageStudio::Serialize::Paint::Common::CTextMeasurer::CPage* pPage		=	m_pTextBase->m_arrPages[0];
				ImageStudio::Serialize::Paint::Common::CTextMeasurer::CPage* pDrawPage	=	m_arrPages[0];
				if (pPage)
				{
					CAtlArray<ImageStudio::Serialize::Paint::Common::CTextMeasurer::CLine*>& arrLines		=	pPage->GetRefLines ();
					CAtlArray<ImageStudio::Serialize::Paint::Common::CTextMeasurer::CLine*>& arrDrawLines	=	pDrawPage->GetRefLines ();
					for ( long i = 0; i < (long)arrLines.GetCount (); ++i )
					{
						CAtlArray<ImageStudio::Serialize::Paint::Common::CTextMeasurer::CItem*>* pItems	=	arrLines[i]->GetItems();
						CAtlArray<ImageStudio::Serialize::Paint::Common::CTextMeasurer::CItem*>* pDrawItems	=	arrDrawLines[i]->GetItems();

						for ( long j = 0; j < (long)pItems->GetCount (); ++j)
						{
							ImageStudio::Serialize::Paint::Common::CTextMeasurer::CSymbols* pItem = (ImageStudio::Serialize::Paint::Common::CTextMeasurer::CSymbols*)pItems->operator[](j);									
							ImageStudio::Serialize::Paint::Common::CTextMeasurer::CSymbols* pDrawItem = (ImageStudio::Serialize::Paint::Common::CTextMeasurer::CSymbols*)pDrawItems->operator[](j);									

							pDrawItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1			=	pItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color1;
							pDrawItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color2			=	pItem->GetCharacter()->m_oAttributes.m_oBrush.m_oBrush.Color2;
							pDrawItem->GetCharacter()->m_oAttributes.m_oShadow.m_oShadow.Color			=	pItem->GetCharacter()->m_oAttributes.m_oShadow.m_oShadow.Color;
							pDrawItem->GetCharacter()->m_oAttributes.m_oBackground.m_oBackground.Color	=	pItem->GetCharacter()->m_oAttributes.m_oBackground.m_oBackground.Color;
							//pDrawItem->GetCharacter()->m_oAttributes.m_oEdge.m_oEdge.Color			=	pItem->GetCharacter()->m_oAttributes.m_oEdge.m_oEdge.Color;
						}
					}

					m_bApplyColorTransform	=	FALSE;

					return TRUE;
				}
			}

			return FALSE;
		}

		//
		inline BOOL DrawToStaticCache (IAVSRenderer* pCommander)
		{
			if (NULL == m_pLayer)
			{
				if (FALSE == m_bStatus)
				{
					if (NULL==m_pTextBase)
						m_pTextBase = new CTextMeasurer(m_pFontManager);
					if (m_pTextBase)
						m_pTextBase->PrepareFirstPage (pCommander);

					PrepareFirstPage ( pCommander );

					GetBounds ();

					m_bStatus	=	TRUE;
				}

				CDoubleRect	oMarginBounds	=	CCoordTransformer::MarginBounds (m_oBounds);
				oMarginBounds				=	CCoordTransformer::BoundRotate::Transform (oMarginBounds, m_dAngle);

				if ( m_oImageCache.CreateImage ( pCommander, oMarginBounds ) )
				{
					IAVSRenderer* pCommander2 =	m_oImageCache.GetRender ();
					if ( pCommander2 )
					{		
						CCoordTransformer oTransformer(m_dXWidthMM, m_dYHeightMM);
						oTransformer.StaticTransform (this, pCommander2);

						DrawToCommander ( pCommander2, FALSE );

						m_oImageCache.Refresh ();
					}

					RELEASEINTERFACE ( pCommander2 );
				}

				if ( m_oImageCache.GetImage () )
				{
					pCommander->SetTransform ( 1, 0, 0, 1, 0, 0 );
					pCommander->DrawImage ( m_oImageCache.GetImage (), (float)oMarginBounds.left, (float)(oMarginBounds.top), (float)(oMarginBounds.GetWidth()), (float)(oMarginBounds.GetHeight()));
				}

				return TRUE;
			}

			return FALSE;;
		}

		inline BOOL DrawToDynamicCache (IAVSRenderer* pCommander)
		{
			if (m_pLayer)
			{
				if ( ((CAnimationLayer*)m_pLayer)->IsChangeFillColor() )	//	увы это не можем закэшировать
					return FALSE;

				if (FALSE == m_bStatus)
				{
					if (NULL==m_pTextBase)
						m_pTextBase = new CTextMeasurer(m_pFontManager);
					if (m_pTextBase)
						m_pTextBase->PrepareFirstPage (pCommander);

					PrepareFirstPage ( pCommander );

					GetBounds ();

					m_bStatus	=	TRUE;
				}

				CDoubleRect	oMarginBounds	=	CCoordTransformer::MarginBounds (m_oBounds, 0.0);

				if ( m_oImageCache.CreateImage ( pCommander, oMarginBounds ) )
				{
					IAVSRenderer* pCommander2 =	m_oImageCache.GetRender ();
					if ( pCommander2 )
					{		
						CCoordTransformer oTransformer(m_dXWidthMM, m_dYHeightMM);
						oTransformer.StaticTransform (this, pCommander2, FALSE, 0.0);

						DrawToCommander ( pCommander2, FALSE, TRUE );

						m_oImageCache.Refresh ();
					}

					RELEASEINTERFACE ( pCommander2 );
				}

				if (NULL == m_pCImage)				
					m_pCImage	=	new CAnimateImage (m_pLayer, &m_oImageCache, oMarginBounds, m_dAngle, m_dXWidthMM, m_dYHeightMM, m_lFlags);

				if (m_pCImage)
				{
					m_pCImage->SetTimes (m_dTimeBegin,m_dTimeEnd); 
					m_pCImage->SetBaseBounds (m_oBaseBounds);
					m_pCImage->Render (pCommander); 
				}

				return TRUE;
			}

			return FALSE;
		}

		inline BOOL DrawToCommander (IAVSRenderer* pCommander, BOOL bUpdateTransform = TRUE, BOOL BlendOff	= FALSE)
		{
			double dTransparency			=	1.0;
			if (BlendOff)
			{
				m_oBrush.m_oBrush.Alpha1	=	m_nAlpha1;
				m_oBrush.m_oBrush.Alpha2	=	m_nAlpha2;
			}
			else
			{
				if ( m_pLayer )
				{
					if ( m_pLayer->GetTransparency ( dTransparency ) )
					{
						if ( dTransparency == 0.0 )
							return TRUE;
					}

					m_oBrush.m_oBrush.Alpha1	=	dTransparency * m_nAlpha1;
					m_oBrush.m_oBrush.Alpha2	=	dTransparency * m_nAlpha2;
				}
				else
				{
					m_oBrush.m_oBrush.Alpha1	=	m_nAlpha1;
					m_oBrush.m_oBrush.Alpha2	=	m_nAlpha2;
				}
			}

			//pCommander->put_Width(m_dWidthMetric);
			//pCommander->put_Height(m_dHeightMetric);

			if ( bUpdateTransform )
			{
				UpdateTransform (pCommander, m_lFlags);
			}

			if ( m_pLayer )
			{
				if ( m_pActualEffect )
				{
					if ( CreditsEffect == m_pActualEffect->GetID () )	// После такого эффекта объект больше не появляется
						return TRUE;
				}
			}

			if (NULL==m_pTextBase)
				m_pTextBase = new CTextMeasurer(m_pFontManager);
			if (m_pTextBase)
				m_pTextBase->PrepareFirstPage (pCommander);

			PrepareFirstPage ( pCommander );

			ApplyColorTransform ();			

			CAnimateText::DrawFirstPage ( pCommander, dTransparency );

			return TRUE;
		}

	protected:

		BOOL				m_bStatus;

		double				m_dWidthMetric;
		double				m_dHeightMetric;

		CDoubleRect			m_oBounds;

		long				m_nAlpha1;
		long				m_nAlpha2;

		BOOL				m_bApplyColorTransform;

		CTextMeasurer*		m_pTextBase;
		IAVSFontManager*	m_pFontManager;

		CCachedCommands		m_oImageCache;			//	STATIC 
		CAnimateImage*		m_pCImage;				//	DYNAMIC

		CParagraphComposer	m_oParComposer;
		CParagraphComposer	m_oDefParComposer;

		int						m_nUseMeasurers;	//	-1 - CALC STATUS
		BOOL					m_bInitMeasurers;
		CArray<TextMeasurer2>	m_oMeasurers;
		CArray<TextMeasurer2>	m_oBaseMeasurers;
	};

	class CStream : public CShowElement
	{
	public:

		CStream()
		{
			m_nType			=	ElementStream;

			m_pRefStream	=	NULL;
			m_pRefFrame		=	NULL;	//	NOT CACHED ELEMENT

			m_dAngle		=	0.0;
		}

		~CStream()
		{

		}

		//
		virtual void Render (IAVSRenderer* pCommander)
		{
			if (NULL == pCommander)
				return;

			if ( m_pLayer )
			{
				if (CShowElement::NotValidTime ())
					return;
			}

			double dBlend	=	1.0;
			if (m_pLayer)
			{
				if (m_pLayer->GetTransparency (dBlend) )
				{
					if (dBlend == 0.0)
						return;
				}

				CMediaCall* pEnd		=	NULL;
				const CMediaCall* pCall	=	((CAnimationLayer*)m_pLayer)->GetMediaManage(pEnd);
				if (pCall)
				{
					if (CMediaCall::Play == pCall->CMD())
					{
						BOOL bStop		=	FALSE;
						if (pEnd)
						{
							if (CMediaCall::Stop == pEnd->CMD() || CMediaCall::Pause == pEnd->CMD())	//	поток был остановлен
								bStop	=	TRUE;						
						}

						if (bStop)
						{
							if (m_pRefStream)
								m_pRefStream->Seek(pEnd->GetEndTime());
						}
						else
						{
							if (m_pRefStream)
								m_pRefStream->Seek(m_pLayer->GetTime() - pCall->GetBeginTime());
						}
					}
				}
				else
				{					
					if (pEnd)															// последня команда с меткой
					{
						//if (CMediaCall::Play == pEnd->CMD())
						{
							if (m_pRefStream)
								m_pRefStream->Seek(pEnd->GetEndTime());
						}
					}
					else
					{
						if (m_pRefStream)
							m_pRefStream->Seek(0.0);								//	сброс на начало сцены
					}
				}
			}

			IUnknown* pSImage	=	GetFrame ();
			if (pSImage)
			{
				UpdateTransform (pCommander, 0);

				pCommander->put_Width((float)m_dXWidthMM);
				pCommander->put_Height((float)m_dYHeightMM);

				if ( m_pLayer )
				{
					if ( m_pActualEffect )
					{
						if ( CreditsEffect == m_pActualEffect->GetID () )	// После такого эффекта объект больше не появляется
							return;
					}

					CEffect* pCompose		=	((CAnimationLayer*)m_pLayer)->GetCompose();
					if (pCompose)
					{
						IUnknown* pImage	=	NULL;
						if (m_oBuffCompose.ApplyEffect (pCompose, this, pImage) )
						{
							CBlendImage oBlendImage (pImage);
							pCommander->DrawImage (oBlendImage.Get(dBlend), (float)m_oBounds.left, (float)(m_oBounds.top), (float)(m_oBounds.right - m_oBounds.left), (float)(m_oBounds.bottom - m_oBounds.top));
							return;
						}
					}
				}

				CBlendImage oBlendImage (pSImage);
				pCommander->DrawImage (oBlendImage.Get(dBlend), (float)m_oBounds.left, (float)(m_oBounds.top), (float)(m_oBounds.right - m_oBounds.left), (float)(m_oBounds.bottom - m_oBounds.top));
			}
		}

		virtual void FromXml (XmlUtils::CXmlNode& oXmlNode)
		{
			m_dXWidthMM			=	Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("widthmetric"),_T("0")));
			m_dYHeightMM		=	Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("heightmetric"),_T("0")));

			m_dAngle			=	Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("angle"), _T("0")));

			m_oBounds.left		=	Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("left"),	_T("0")));
			m_oBounds.top		=	Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("top"),	_T("0")));
			m_oBounds.right		=	Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("right"),	_T("0")));
			m_oBounds.bottom	=	Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("bottom"),_T("0")));

			m_oBaseBounds		=	m_oBounds;

			XmlUtils::CXmlNode oTimeNode;
			if ( oXmlNode.GetNode ( _T("timeline"), oTimeNode ) )
			{
				m_dTimeBegin	=	_tstof ( oTimeNode.GetAttributeOrValue ( _T("begin"), _T("0.0") ) );
				m_dTimeEnd		=	_tstof ( oTimeNode.GetAttributeOrValue ( _T("end"), _T("0.0") ) );
			}

		}

		//
		inline void SetStream (ISStream* pStream)
		{
			m_pRefStream	=	pStream;
		}
		virtual const CDoubleRect& GetBounds ()
		{ 
			return m_oBounds;
		}
		virtual double GetAngle ()	
		{
			return m_dAngle;	
		}

		//
		virtual void ClearCache ()
		{

		}
		virtual IUnknown* GetBufferImage ()
		{
			return GetFrame();
		}

	private:

		inline IUnknown* GetFrame ()
		{
			m_pRefFrame	=	NULL;

			if (m_pRefStream)
				m_pRefStream->GetFrame (&m_pRefFrame);

			return m_pRefFrame;
		}

	private:

		double		m_dAngle;
		CDoubleRect	m_oBounds;

		ISStream*	m_pRefStream;			//	DO NOT RELEASE - REFERENCE
		IUnknown*	m_pRefFrame;			//	REFENCE BUFFER ELEMENT (NONE CACHED)

		CComposeTransformer	m_oBuffCompose;
	};

	class CBuilderAnimateDrawers
	{
	public:
		CBuilderAnimateDrawers (IAVSFontManager* pFontManager) : m_pFontManager (pFontManager)
		{

		}

		inline IShowElement* FromXml (XmlUtils::CXmlNode& oXmlNode)
		{
			IShowElement* pObj	=	NULL;

			CString node	=	oXmlNode.GetName ();

			if ( CString (_T("ImagePaint-DrawGraphicPathAnimate") ) == node)
				pObj	=	new Animation::CAnimatePath ();
			else	if (CString ( _T("ImagePaint-DrawGraphicPath") ) == node)
				pObj	=	new Animation::CAnimatePath ();
			else	if (CString ( _T("ImagePaint-DrawImageFromFileAnimate") ) == node)
				pObj	=	new Animation::CAnimateImage ();
			else	if (CString ( _T("ImagePaint-DrawImageFromFile") ) == node)
				pObj	=	new Animation::CAnimateImage ();
			else	if (CString ( _T("ImagePaint-DrawTextEx") ) == node)
				pObj	=	new Animation::CAnimateText (m_pFontManager);
			else	if (CString ( _T("ImagePaint-DrawTextAnimateEx") ) == node)
				pObj	=	new Animation::CAnimateText (m_pFontManager);
			else	if (CString ( _T("VideoStream") ) == node)
				pObj	=	new Animation::CStream ();

			if ( pObj )
				pObj->FromXml (oXmlNode);

			return pObj;
		}

	private:
		IAVSFontManager* m_pFontManager;
	};
}