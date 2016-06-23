#pragma once

#include <string>
#include <boost/algorithm/string.hpp>

#include "../../../../DesktopEditor/graphics/GraphicsPath.h"

#define _USE_MATH_DEFINES
#include <math.h>//M_PI
#include <vector>

namespace NSCustomShapesConvert
{
	const double c_dMasterUnitsToInchKoef		= 1.0 / 576;
	const double c_dInchToMillimetreKoef		= 25.4;
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
	class CDoublePoint
	{
	public:
		double dX;
		double dY;
	public:
		CDoublePoint()
		{
			dX = 0;
			dY = 0;
		}

		CDoublePoint& operator= (const CDoublePoint& oSrc)
		{
			dX = oSrc.dX;
			dY = oSrc.dY;

			return *this;
		}

		CDoublePoint(const CDoublePoint& oSrc)
		{
			*this = oSrc;
		}
	};

	enum RulesType
	{
		// VML
		rtLineTo			= 0,	// 2*
		rtCurveTo			= 1,	// 6*
		rtMoveTo			= 2,	// 2
		
		rtClose				= 3,	// 0
		rtEnd				= 4,	// 0
		
		rtRMoveTo			= 5,	// 2*
		rtRLineTo			= 6,	// 2*
		rtRCurveTo			= 7,	// 6*
		
		rtNoFill			= 8,	// 0
		rtNoStroke			= 9,	// 0
		
		rtAngleEllipseTo	= 10,	// 6*
		rtAngleEllipse		= 11,	// 6*
		
		rtArc				= 12,	// 8* 
		rtArcTo				= 13,	// 8*
		
		rtClockwiseArcTo	= 14,	// 8*
		rtClockwiseArc		= 15,	// 8*
		
		rtEllipticalQuadrX	= 16,	// 2*
		rtEllipticalQuadrY	= 17,	// 2*

		rtQuadrBesier		= 18,	// 2 + 2*

		rtFillColor			= 20, 
		rtLineColor			= 21,

		// OOXML
		rtOOXMLMoveTo		= 0 + 100,	// 2
		rtOOXMLLineTo		= 1 + 100,	// 2*
		rtOOXMLCubicBezTo	= 2 + 100,	// 6*
		rtOOXMLArcTo		= 3 + 100,	// 8*	
		rtOOXMLQuadBezTo	= 4 + 100,	// 2 + 2*
		rtOOXMLClose		= 5 + 100,	// 0
		rtOOXMLEnd			= 6	+ 100	// 0
	};

	
	class CGraphicPath
	{
	public:
		//virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
		//{
		//	Metric		= XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
		//	m_bStroke	= (1 == XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("stroke"), _T("0"))));
		//	m_bFill		= (1 == XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("fill"), _T("0"))));

		//	m_dWidthMM	= XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("widthmm"), _T("210")));
		//	m_dHeightMM = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("heightmm"), _T("190")));

		//	m_dAngle	= XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("angle"), _T("0")));
		//	m_lFlags	= XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("flags"), _T("0")));

		//	m_oBounds.left		= XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("bounds-left"),		_T("0")));
		//	m_oBounds.top		= XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("bounds-top"),		_T("0")));
		//	m_oBounds.right		= XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("bounds-right"),	_T("0")));
		//	m_oBounds.bottom	= XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("bounds-bottom"),	_T("0")));

		//	XmlUtils::CXmlNodes oNodes;
		//	oXmlNode.GetNodes(_T("part"), oNodes);
		//	for (int nIndex = 0; nIndex < oNodes.GetCount(); ++nIndex)
		//	{
		//		CPart oPart;
		//		XmlUtils::CXmlNode oNode;
		//		oNodes.GetAt(nIndex, oNode);
		//		oPart.FromXmlNode(oNode);
  //              m_arParts.push_back(oPart);
		//	}

		//	//XmlUtils::CXmlNode oPenNode;
		//	//if (oXmlNode.GetNode(_T("pen"), oPenNode))
		//	//{
		//	//	Pen.FromXmlNode(oPenNode);
		//	//}
		//	//XmlUtils::CXmlNode oBrushNode;
		//	//if (oXmlNode.GetNode(_T("brush"), oBrushNode))
		//	//{
		//	//	Brush.FromXmlNode(oBrushNode);
		//	//}
		//}					
		virtual void InternalClear()
		{
			m_lFlags	= 0;
		}

	public:

		CGraphicPath()
		{
			InternalClear();
		}
	public:

		class CPart
		{
		public:
			RulesType					m_eType;
            std::vector<CDoublePoint>	m_arPoints;

		public:
            CPart() : m_eType(rtMoveTo), m_arPoints()
			{
			}
			CPart& operator=(const CPart& oSrc)
			{
				m_eType	= oSrc.m_eType;
                this->m_arPoints.clear();
                for (int nIndex = 0; nIndex < oSrc.m_arPoints.size(); ++nIndex)
				{
                    this->m_arPoints.push_back(oSrc.m_arPoints[nIndex]);
				}
				return (*this);
			}
			~CPart()
			{
                this->m_arPoints.clear();
			}
			//void FromXmlNode(XmlUtils::CXmlNode& oNode)
			//{
			//	std::wstring strName = oNode.GetAttribute(_T("name"));
			//	if (_T("moveto")				== strName) m_eType = rtMoveTo;
			//	else if (_T("lineto")			== strName) m_eType = rtLineTo;
			//	else if (_T("curveto")			== strName) m_eType = rtCurveTo;
			//	else if (_T("rmoveto")			== strName) m_eType = rtRMoveTo;
			//	else if (_T("rlineto")			== strName) m_eType = rtRLineTo;
			//	else if (_T("rcurveto")			== strName) m_eType = rtRCurveTo;
			//	else if (_T("ellipseto")		== strName) m_eType = rtAngleEllipseTo;
			//	else if (_T("ellipse")			== strName) m_eType = rtAngleEllipse;
			//	else if (_T("arc")				== strName) m_eType = rtArc;
			//	else if (_T("arcto")			== strName) m_eType = rtArcTo;
			//	else if (_T("clockwisearcto")	== strName) m_eType = rtClockwiseArcTo;
			//	else if (_T("clockwisearc")		== strName) m_eType = rtClockwiseArc;
			//	else if (_T("ellipticalx")		== strName) m_eType = rtEllipticalQuadrX;
			//	else if (_T("ellipticaly")		== strName) m_eType = rtEllipticalQuadrY;
			//	else if (_T("qbesier")			== strName) m_eType = rtQuadrBesier;
			//	else m_eType = rtClose;

			//	std::wstring strPath = oNode.GetAttribute(_T("path"));
			//	if (_T("") == strPath)
			//		return;

   //             std::vector<std::wstring> arStrNums;
			//	ParseString(_T(" "), strPath, &arStrNums);

			//	bool bIsX = true;
			//	int nCurPoint = 0;
   //             for (int nIndex = 0; nIndex < arStrNums.size(); ++nIndex)
			//	{
			//		if (bIsX)
			//		{
			//			++nCurPoint;
			//			CDoublePoint point;
   //                     this->m_arPoints.push_back(point);
   //                     this->m_arPoints[nCurPoint - 1].dX = XmlUtils::GetDouble(arStrNums[nIndex]);
			//		}
			//		else
			//		{
   //                     this->m_arPoints[nCurPoint - 1].dY = XmlUtils::GetDouble(arStrNums[nIndex]);
			//		}
			//		bIsX = !bIsX;
			//	}
			//}
			void ParseString(std::wstring strDelimeters, std::wstring strSource, 
                std::vector<std::wstring>& pArrayResults, bool bIsCleared = true)
			{
				if (bIsCleared)
                    pArrayResults.clear();

				std::wstring resToken;
				int curPos= 0;

				boost::algorithm::split(pArrayResults, strSource, boost::algorithm::is_any_of(strDelimeters), boost::algorithm::token_compress_on);

			}

			void CheckLastPoint(IRenderer* pRenderer, CDoublePoint& pointCur)
			{
				if (NULL == pRenderer)
					return;

				pRenderer->PathCommandGetCurrentPoint(&pointCur.dX, &pointCur.dY);
			}
			double GetAngle(double fCentreX, double fCentreY, double fX, double fY)
			{
				//  -   + (..  )
				double dX = fX - fCentreX;
				double dY = fY - fCentreY;

				double modDX = abs(dX);
				double modDY = abs(dY);

				if ((modDX < 0.01) && (modDY < 0.01))
				{
					return 0;
				}
				if ((modDX < 0.01) && (dY < 0))
				{
					return -90;
				}
				else if (modDX < 0.01)
				{
					return 90;
				}
				if ((modDY < 0.01) && (dX < 0))
				{
					return 180;
				}
				else if (modDY < 0.01)
				{
					return 0;
				}

				double fAngle = atan(dY / dX);
				fAngle *= double(180 / M_PI);
				if (dX > 0 && dY > 0)
				{
					return fAngle;
				}
				else if (dX > 0 && dY < 0)
				{
					return fAngle;
				}
				else if (dX < 0 && dY > 0)
				{
					//return fAngle + 180;
					return 180 + fAngle;
				}
				else
				{
					//return fAngle + 180;
					return fAngle - 180;
				}
			}

			inline double GetSweepAngle(const double& angleStart, const double& angleEnd)
			{
				if (angleStart >= angleEnd)
					return angleEnd - angleStart;
				else
					return angleEnd - angleStart - 360;
			}

			void ApplyElliptical(bool& bIsX, double& angleStart, double& angleSweet, 
				double& Left, double& Top, double& Width, double& Height, const CDoublePoint& pointCur)
			{
				//   (x - y - x...)
				if (bIsX)
				{
					angleStart = -90;
					angleSweet = 90;

					if ((Width < 0) && (Height < 0))
					{
						angleStart = 90;
						Width *= -1;
						Height *= -1;
						Left	= pointCur.dX - Width / 2;
						Top		= pointCur.dY - Height;
					}
					else if ((Width < 0) && (Height > 0))
					{
						angleStart = -90;
						angleSweet = -90;
						Width *= -1;
						Left	= pointCur.dX - Width / 2;
						Top		= pointCur.dY;
					}
					else if ((Width > 0) && (Height < 0))
					{
						angleStart = 90;
						angleSweet = -90;
						Height *= -1;
						Left	= pointCur.dX - Width / 2;
						Top		= pointCur.dY - Height;
					}
					else
					{
						Left	= pointCur.dX - Width / 2;
						Top		= pointCur.dY;
					}
				}
				else
				{
					angleStart = 180;
					angleSweet = -90;

					if ((Width < 0) && (Height < 0))
					{
						angleStart = 0;
						Width *= -1;
						Height *= -1;
						Left	= pointCur.dX - Width;
						Top		= pointCur.dY - Height / 2;
					}
					else if ((Width < 0) && (Height > 0))
					{
						angleStart = 0;
						angleSweet = 90;
						Width *= -1;
						Left	= pointCur.dX - Width;
						Top		= pointCur.dY - Height / 2;
					}
					else if ((Width > 0) && (Height < 0))
					{
						angleStart = 180;
						angleSweet = 90;
						Height *= -1;
						Left	= pointCur.dX;
						Top		= pointCur.dY - Height / 2;
					}
					else
					{
						Left	= pointCur.dX;
						Top		= pointCur.dY - Height / 2;
					}
				}
				bIsX = !bIsX;
			}


            void GetSafearrayPoints(IRenderer* pRenderer, double** ppArray, size_t& nCountOut, CDoublePoint& pointCur, bool bR = false)
            {
                if (NULL == ppArray)
                    return;

                int nCount = this->m_arPoints.size();

                nCountOut = 2 * (nCount + 1);

                double* pArray = new double [nCountOut];
                double* pBuffer = pArray;

                memset (pBuffer, 0, nCountOut * sizeof(double));

                *pBuffer = pointCur.dX; ++pBuffer;
                *pBuffer = pointCur.dY; ++pBuffer;

                if (bR)
                {
                    for (int nIndex = 0; nIndex < nCount; ++nIndex)
                    {
                        *pBuffer = (this->m_arPoints[nIndex].dX + pointCur.dX); ++pBuffer;
                        *pBuffer = (this->m_arPoints[nIndex].dY + pointCur.dY); ++pBuffer;

                        if (nIndex == (nCount - 1))
                        {
                            pointCur.dX += this->m_arPoints[nIndex].dX;
                            pointCur.dY += this->m_arPoints[nIndex].dY;
                        }
                    }
                }
                else
                {
                    for (int nIndex = 0; nIndex < nCount; ++nIndex)
                    {
                        *pBuffer = this->m_arPoints[nIndex].dX; ++pBuffer;
                        *pBuffer = this->m_arPoints[nIndex].dY; ++pBuffer;

                        if (nIndex == (nCount - 1))
                        {
                            pointCur.dX = this->m_arPoints[nIndex].dX;
                            pointCur.dY = this->m_arPoints[nIndex].dY;
                        }
                    }
                }
                *ppArray = pArray;

            }

			void Draw(IRenderer* pRenderer, CDoublePoint& pointCur)
			{
				switch (m_eType)
				{
				case rtMoveTo:
					{
                        if (0 < this->m_arPoints.size())
						{
                            pointCur.dX = this->m_arPoints[0].dX;
                            pointCur.dY = this->m_arPoints[0].dY;
                            pRenderer->PathCommandMoveTo(this->m_arPoints[0].dX, this->m_arPoints[0].dY);
						}
						break;
					}
				case rtLineTo:
					{
                        double* pArray = NULL;
                        size_t nCount = 0;
                        GetSafearrayPoints(pRenderer, &pArray, nCount, pointCur);

						if (NULL != pArray)
						{
                            pRenderer->PathCommandLinesTo(pArray, nCount /*this->m_arPoints.size()*/);
						}

						break;
					}
				case rtCurveTo:
					{
                        double* pArray = NULL;
                        size_t nCount = 0;
                        GetSafearrayPoints(pRenderer, &pArray, nCount, pointCur);

						if (NULL != pArray)
						{
                            pRenderer->PathCommandCurvesTo (pArray, nCount/*this->m_arPoints.size()*/);
						}

						break;
					}
				case rtClose:
					{
						pRenderer->PathCommandClose();
						break;
					}
				case rtRMoveTo:
					{
                        if (0 < this->m_arPoints.size())
						{
                            pointCur.dX = this->m_arPoints[0].dX + pointCur.dX;
                            pointCur.dY = this->m_arPoints[0].dY + pointCur.dY;
							pRenderer->PathCommandMoveTo(pointCur.dX, pointCur.dY);
						}
						break;
					}
				case rtRLineTo:
					{                       
                        double* pArray = NULL;
                        size_t nCount = 0;
                        GetSafearrayPoints(pRenderer, &pArray, nCount, pointCur, TRUE);

						if (NULL != pArray)
						{
                            pRenderer->PathCommandLinesTo(pArray, nCount/*this->m_arPoints.size()*/);
						}

						break;
					}
				case rtRCurveTo:
					{                        
                        double* pArray = NULL;
                        size_t nCount = 0;
                        GetSafearrayPoints(pRenderer, &pArray, nCount, pointCur, TRUE);

						if (NULL != pArray)
						{
                            pRenderer->PathCommandCurvesTo(pArray, nCount/*this->m_arPoints.size()*/);
						}
						break;
					}
				case rtAngleEllipseTo:
					{
						int nFigure = 0;
                        while ((nFigure + 3) <= this->m_arPoints.size())
						{
                            double nLeft	= this->m_arPoints[nFigure].dX - this->m_arPoints[nFigure + 1].dX / 2;
                            double nTop		= this->m_arPoints[nFigure].dY - this->m_arPoints[nFigure + 1].dY / 2;

							pRenderer->PathCommandArcTo(nLeft, nTop,
                                this->m_arPoints[nFigure + 1].dX, this->m_arPoints[nFigure + 1].dY,
                                this->m_arPoints[nFigure + 2].dX, this->m_arPoints[nFigure + 2].dY);

							nFigure += 3;
						}

						CheckLastPoint(pRenderer, pointCur);
						break;
					}
				case rtAngleEllipse:
					{
						pRenderer->PathCommandStart();
						int nFigure = 0;
                        while ((nFigure + 3) <= this->m_arPoints.size())
						{
                            double nLeft	= this->m_arPoints[nFigure].dX - this->m_arPoints[nFigure + 1].dX / 2;
                            double nTop		= this->m_arPoints[nFigure].dY - this->m_arPoints[nFigure + 1].dY / 2;

							pRenderer->PathCommandArcTo(nLeft, nTop,
                                this->m_arPoints[nFigure + 1].dX, this->m_arPoints[nFigure + 1].dY,
                                this->m_arPoints[nFigure + 2].dX, this->m_arPoints[nFigure + 2].dY);

							nFigure += 3;
						}

						CheckLastPoint(pRenderer, pointCur);
						break;
					}
				case rtArc:
					{
						pRenderer->PathCommandStart();
						int nFigure = 0;
                        while ((nFigure + 4) <= this->m_arPoints.size())
						{
                            double nCentreX = (this->m_arPoints[nFigure].dX + this->m_arPoints[nFigure + 1].dX) / 2;
                            double nCentreY = (this->m_arPoints[nFigure].dY + this->m_arPoints[nFigure + 1].dY) / 2;

							double angleStart = GetAngle(nCentreX, nCentreY, 
                                this->m_arPoints[nFigure + 2].dX, this->m_arPoints[nFigure + 2].dY);

							double angleEnd = GetAngle(nCentreX, nCentreY, 
                                this->m_arPoints[nFigure + 3].dX, this->m_arPoints[nFigure + 3].dY);

                            pRenderer->PathCommandArcTo(this->m_arPoints[nFigure].dX, this->m_arPoints[nFigure].dY,
                                this->m_arPoints[nFigure + 1].dX - this->m_arPoints[nFigure].dX,
                                this->m_arPoints[nFigure + 1].dY - this->m_arPoints[nFigure].dY,
								angleStart, GetSweepAngle(angleStart, angleEnd));

							nFigure += 4;
						}

						CheckLastPoint(pRenderer, pointCur);
						break;
					}
				case rtArcTo:
					{
						int nFigure = 0;
                        while ((nFigure + 4) <= this->m_arPoints.size())
						{
                            double nCentreX = (this->m_arPoints[nFigure].dX + this->m_arPoints[nFigure + 1].dX) / 2;
                            double nCentreY = (this->m_arPoints[nFigure].dY + this->m_arPoints[nFigure + 1].dY) / 2;

							double angleStart = GetAngle(nCentreX, nCentreY, 
                                this->m_arPoints[nFigure + 2].dX, this->m_arPoints[nFigure + 2].dY);

							double angleEnd = GetAngle(nCentreX, nCentreY, 
                                this->m_arPoints[nFigure + 3].dX, this->m_arPoints[nFigure + 3].dY);

                            pRenderer->PathCommandArcTo(this->m_arPoints[nFigure].dX, this->m_arPoints[nFigure].dY,
                                this->m_arPoints[nFigure + 1].dX - this->m_arPoints[nFigure].dX,
                                this->m_arPoints[nFigure + 1].dY - this->m_arPoints[nFigure].dY,
								angleStart, GetSweepAngle(angleStart, angleEnd));

							nFigure += 4;
						}

						CheckLastPoint(pRenderer, pointCur);
						break;
					}
				case rtClockwiseArcTo:
					{
						int nFigure = 0;
                        while ((nFigure + 4) <= this->m_arPoints.size())
						{
                            double nCentreX = (this->m_arPoints[nFigure].dX + this->m_arPoints[nFigure + 1].dX) / 2;
                            double nCentreY = (this->m_arPoints[nFigure].dY + this->m_arPoints[nFigure + 1].dY) / 2;

							double angleStart = GetAngle(nCentreX, nCentreY, 
                                this->m_arPoints[nFigure + 2].dX, this->m_arPoints[nFigure + 2].dY);

							double angleEnd = GetAngle(nCentreX, nCentreY, 
                                this->m_arPoints[nFigure + 3].dX, this->m_arPoints[nFigure + 3].dY);

                            pRenderer->PathCommandArcTo(this->m_arPoints[nFigure].dX, this->m_arPoints[nFigure].dY,
                                this->m_arPoints[nFigure + 1].dX - this->m_arPoints[nFigure].dX,
                                this->m_arPoints[nFigure + 1].dY - this->m_arPoints[nFigure].dY,
								angleStart, 360 + GetSweepAngle(angleStart, angleEnd));

							nFigure += 4;
						}

						CheckLastPoint(pRenderer, pointCur);
						break;
					}
				case rtClockwiseArc:
					{
						pRenderer->PathCommandStart();
						int nFigure = 0;
                        while ((nFigure + 4) <= this->m_arPoints.size())
						{
                            double nCentreX = (this->m_arPoints[nFigure].dX + this->m_arPoints[nFigure + 1].dX) / 2;
                            double nCentreY = (this->m_arPoints[nFigure].dY + this->m_arPoints[nFigure + 1].dY) / 2;

							double angleStart = GetAngle(nCentreX, nCentreY, 
                                this->m_arPoints[nFigure + 2].dX, this->m_arPoints[nFigure + 2].dY);

							double angleEnd = GetAngle(nCentreX, nCentreY, 
                                this->m_arPoints[nFigure + 3].dX, this->m_arPoints[nFigure + 3].dY);

                            pRenderer->PathCommandArcTo(this->m_arPoints[nFigure].dX, this->m_arPoints[nFigure].dY,
                                this->m_arPoints[nFigure + 1].dX - this->m_arPoints[nFigure].dX,
                                this->m_arPoints[nFigure + 1].dY - this->m_arPoints[nFigure].dY,
								angleStart, 360 + GetSweepAngle(angleStart, angleEnd));

							nFigure += 4;
						}

						CheckLastPoint(pRenderer, pointCur);
						break;
					}
				case rtEllipticalQuadrX:
					{
						bool bIsX = true;
						CheckLastPoint(pRenderer, pointCur);

                        int nCount = this->m_arPoints.size();
						for (int nIndex = 0; nIndex < nCount; ++nIndex)
						{
							double x1 = pointCur.dX;
							double y1 = pointCur.dY;

                            double x2 = this->m_arPoints[nIndex].dX;
                            double y2 = this->m_arPoints[nIndex].dY;

							double dRadX = fabs(x1 - x2);
							double dRadY = fabs(y1 - y2);

							AddEllipticalQuadr(pRenderer, bIsX, x1, y1, x2, y2, dRadX, dRadY);

							pointCur.dX = x2;
							pointCur.dY = y2;
						}

						break;
					}
				case rtEllipticalQuadrY:
					{
						bool bIsX = false;
						CheckLastPoint(pRenderer, pointCur);

                        int nCount = this->m_arPoints.size();
						for (int nIndex = 0; nIndex < nCount; ++nIndex)
						{
							double x1 = pointCur.dX;
							double y1 = pointCur.dY;

                            double x2 = this->m_arPoints[nIndex].dX;
                            double y2 = this->m_arPoints[nIndex].dY;

							double dRadX = fabs(x1 - x2);
							double dRadY = fabs(y1 - y2);

							AddEllipticalQuadr(pRenderer, bIsX, x1, y1, x2, y2, dRadX, dRadY);

							pointCur.dX = x2;
							pointCur.dY = y2;
						}

						break;
					}
				case rtQuadrBesier:
					{                        
                        double* pArray = NULL;
                        size_t nCount = 0;
                        GetSafearrayPoints(pRenderer, &pArray, nCount, pointCur, TRUE);

						if (NULL != pArray)
						{
                            pRenderer->PathCommandLinesTo(pArray, nCount/*this->m_arPoints.size()*/);
						}

						CheckLastPoint(pRenderer, pointCur);
						break;
					}
				default: break;
				};
			}

			inline void AddEllipticalQuadr(IRenderer*& pRenderer, bool& bIsX, double& x1, double& y1, double& x2, double& y2, double& dRadX, double& dRadY)
			{
				if (bIsX)
				{
					if ((x2 >= x1) && (y2 >= y1))
						pRenderer->PathCommandArcTo(x1 - dRadX, y1, 2 * dRadX, 2 * dRadY, -90, 90);
					else if ((x2 >= x1) && (y2 <= y1))
						pRenderer->PathCommandArcTo(x1 - dRadX, y1 - 2 * dRadY, 2 * dRadX, 2 * dRadY, 90, -90);
					else if ((x2 <= x1) && (y2 >= y1))
						pRenderer->PathCommandArcTo(x1 - dRadX, y1, 2 * dRadX, 2 * dRadY, -90, -90);
					else if ((x2 <= x1) && (y2 <= y1))
						pRenderer->PathCommandArcTo(x1 - dRadX, y1 - 2 * dRadY, 2 * dRadX, 2 * dRadY, 90, 90);
				}
				else
				{
					if ((x2 >= x1) && (y2 >= y1))
						pRenderer->PathCommandArcTo(x1, y1 - dRadY, 2 * dRadX, 2 * dRadY, 180, -90);
					else if ((x2 >= x1) && (y2 <= y1))
						pRenderer->PathCommandArcTo(x1, y1 - dRadY, 2 * dRadX, 2 * dRadY, 180, 90);
					else if ((x2 <= x1) && (y2 >= y1))
						pRenderer->PathCommandArcTo(x1 - 2 * dRadX, y1 - dRadY, 2 * dRadX, 2 * dRadY, 0, 90);
					else if ((x2 <= x1) && (y2 <= y1))
						pRenderer->PathCommandArcTo(x1 - 2 * dRadX, y1 - dRadY, 2 * dRadX, 2 * dRadY, 0, -90);
				}
				bIsX = !bIsX;
			}
		};

		void AddRuler(const RulesType& eType)
		{
            int lCount = m_arParts.size();
			
			CPart oPart;
			oPart.m_eType = eType;
			
            m_arParts.push_back(oPart);
		}
		void AddPoint(const double& x, const double& y)
		{
            int lCount = m_arParts.size();
			if (0 != lCount)
			{
				CDoublePoint point;
				point.dX = x;
				point.dY = y;
                m_arParts[lCount - 1].m_arPoints.push_back(point);
			}
		}

		void Clear()
		{
            m_arParts.clear();
		}

	public:

        std::vector<CPart> m_arParts;

		int m_lFlags;

	};
}
