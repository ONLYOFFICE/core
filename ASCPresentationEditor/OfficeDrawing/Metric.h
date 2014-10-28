#pragma once
#include <math.h>

#ifndef WIN32
    #include "./../../DesktopEditor/common/ASCVariant.h"
    #include "../../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

namespace NSPresentationEditor
{
	const double c_dMasterUnitsToInchKoef = 1.0 / 576;
	const double c_dInchToMillimetreKoef  = 25.4;
	const double c_dMasterUnitsToMillimetreKoef = c_dMasterUnitsToInchKoef * c_dInchToMillimetreKoef;

	class CMetricInfo
	{
	public:
		DWORD m_lUnitsHor;
		DWORD m_lUnitsVer;

		DWORD m_lMillimetresHor;
		DWORD m_lMillimetresVer;

	public:
		CMetricInfo()
		{
			m_lUnitsHor = 5000;
			m_lUnitsVer = 5000;

			m_lMillimetresHor = 5000;
			m_lMillimetresVer = 5000;
		}

		CMetricInfo& operator =(const CMetricInfo& oSrc)
		{
			m_lUnitsHor = oSrc.m_lUnitsHor;
			m_lUnitsVer = oSrc.m_lUnitsVer;
			m_lMillimetresHor = oSrc.m_lMillimetresHor;
			m_lMillimetresVer = oSrc.m_lMillimetresVer;

			return *this;
		}

		void SetUnitsContainerSize(DWORD lSizeX, DWORD lSizeY)
		{
			m_lUnitsHor = lSizeX;
			m_lUnitsVer = lSizeY;

			m_lMillimetresHor = (DWORD)(m_lUnitsHor * c_dMasterUnitsToMillimetreKoef);
			m_lMillimetresVer = (DWORD)(m_lUnitsVer * c_dMasterUnitsToMillimetreKoef);
		}
	};

	class CDoubleRect
	{
	public:
		double left;
		double top;
		double right;
		double bottom;

	public:
		CDoubleRect()
		{
			left	= 0;
			top		= 0;
			right	= 0;
			bottom	= 0;
		}
		CDoubleRect& operator=(const CDoubleRect& oSrc)
		{
			left	= oSrc.left;
			top		= oSrc.top;
			right	= oSrc.right;
			bottom	= oSrc.bottom;

			return *this;
		}
		CDoubleRect(const CDoubleRect& oSrc)
		{
			*this = oSrc;
		}
		inline bool IsEqual(const CDoubleRect& oSrc, double dEps = 0.01)
		{
			return ((fabs(left - oSrc.left) < dEps) && (fabs(top - oSrc.top) < dEps) && 
				(fabs(right - oSrc.right) < dEps) && (fabs(bottom - oSrc.bottom) < dEps));
		}

		inline double GetWidth() const
		{
			return right - left;
		}
		inline double GetHeight() const
		{
			return bottom - top;
		}
		inline void Scale(const double& dKoef)
		{
			left	*= dKoef;
			top		*= dKoef;
			right	*= dKoef;
			bottom	*= dKoef;
		}
	};

	//class CRectF
	//{
	//public:
	//	float X;
	//	float Y;
	//	float Width;
	//	float Height;

	//public:
	//	CRectF()
	//	{
	//		X		= 0;
	//		Y		= 0;
	//		Width	= 0;
	//		Height	= 0;
	//	}
	//	CRectF(const CRectF& oSrc)
	//	{
	//		*this = oSrc;
	//	}
	//	CRectF& operator=(const CRectF& oSrc)
	//	{
	//		X		= oSrc.X;
	//		Y		= oSrc.Y;
	//		Width	= oSrc.Width;
	//		Height	= oSrc.Height;

	//		return *this;
	//	}
	//	bool Equals(const CRect& oSrc)
	//	{
	//		return ((X == oSrc.X) && (Y == oSrc.Y) && (Width == oSrc.Width) && (Height == oSrc.Height));
	//	}
	//};

	class CGeomShapeInfo
	{
	public:
		class CPointD
		{
		public:
			double dX;
			double dY;
		public:
			CPointD()
			{
				dX = 0;
				dY = 0;
			}

			CPointD& operator= (const CPointD& oSrc)
			{
				dX = oSrc.dX;
				dY = oSrc.dY;

				return *this;
			}

			CPointD(const CPointD& oSrc)
			{
				*this = oSrc;
			}
		};

	public:
		double m_dLeft;
		double m_dTop;
		double m_dWidth;
		double m_dHeight;

		double m_dLimoX;
		double m_dLimoY;

		// нужен для регулировки по аспекту (limo)
		CPointD m_oCurPoint;

		double m_dRotate;
		bool m_bFlipH;
		bool m_bFlipV;

		LONG m_lOriginalWidth;
		LONG m_lOriginalHeight;

	public:
		CGeomShapeInfo()
		{
			m_dLeft = 0; 
			m_dTop = 0;
			m_dWidth = 720; 
			m_dHeight = 576;
			
			m_dLimoX = 0;
			m_dLimoY = 0;

			m_oCurPoint.dX = 0;
			m_oCurPoint.dY = 0;

			m_dRotate = 0.0;
			m_bFlipH = false;
			m_bFlipV = false;

			m_lOriginalWidth	= 0;
			m_lOriginalHeight	= 0;
		}
		~CGeomShapeInfo()
		{
		}

		CGeomShapeInfo& operator =(const CGeomShapeInfo& oSrc)
		{
			m_dLeft		= oSrc.m_dLeft; 
			m_dTop		= oSrc.m_dTop;
			m_dWidth	= oSrc.m_dWidth; 
			m_dHeight	= oSrc.m_dHeight;
			
			m_dLimoX	= oSrc.m_dLimoX;
			m_dLimoY	= oSrc.m_dLimoY;

			m_oCurPoint = oSrc.m_oCurPoint;

			m_dRotate	= oSrc.m_dRotate;
			m_bFlipH	= oSrc.m_bFlipH;
			m_bFlipV	= oSrc.m_bFlipV;

			m_lOriginalWidth	= oSrc.m_lOriginalWidth;
			m_lOriginalHeight	= oSrc.m_lOriginalHeight;

			return (*this);
		}

		inline void SetBounds(const CDoubleRect& oRect)
		{
			m_dLeft		= oRect.left;
			m_dTop		= oRect.top;
			m_dWidth	= oRect.GetWidth();	
			m_dHeight	= oRect.GetHeight();
		}

		inline LONG GetFlags()
		{
			LONG lFlags = 0;
			if (m_bFlipH)
				lFlags |= 0x0001;
			if (m_bFlipV)
				lFlags |= 0x0002;

			return lFlags;
		}
	};
}
