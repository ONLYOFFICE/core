#pragma once
#include "GraphicsPath.h"
//#include "../../../../DesktopEditor/graphics/GraphicsPath.h"

#if defined(PPTX_DEF)
#include "PPTXShape/Formula.h"
#endif

#if defined(PPT_DEF)
#include "PPTShape/Formula.h"
using namespace NSGuidesVML;
#endif

#if defined(ODP_DEF)
#include "OdpShape/Formula.h"
#endif

namespace NSPresentationEditor
{
	static CString GetRulerVML(RulesType eRuler)
	{
		switch (eRuler)
		{
		case rtMoveTo:				{ return _T("m"); }
		case rtLineTo:				{ return _T("l"); }
		case rtCurveTo:				{ return _T("c"); }
		
		case rtClose:				{ return _T("x"); }
		case rtEnd:					{ return _T("e"); }
		
		case rtRMoveTo:				{ return _T("t"); }
		case rtRLineTo:				{ return _T("r"); }
		case rtRCurveTo:			{ return _T("v"); }
		
		case rtNoFill:				{ return _T("nf"); }
		case rtNoStroke:			{ return _T("ns"); }

		case rtAngleEllipseTo:		{ return _T("ae"); }
		case rtAngleEllipse:		{ return _T("al"); }
		
		case rtArc:					{ return _T("at"); }
		case rtArcTo:				{ return _T("ar"); }

		case rtClockwiseArcTo:		{ return _T("wa"); }
		case rtClockwiseArc:		{ return _T("wr"); }

		case rtEllipticalQuadrX:	{ return _T("qx"); }
		case rtEllipticalQuadrY:	{ return _T("qy"); }

		case rtQuadrBesier:			{ return _T("qb"); }
		default: break;
		};
		return _T("");
	}

	static RulesType GetRuler(const CString& strName, bool& bRes)
	{
		bRes = true;
		if		(_T("moveTo")		== strName)	return rtOOXMLMoveTo;
		else if (_T("lnTo")			== strName)	return rtOOXMLLineTo;
		else if (_T("cubicBezTo")	== strName)	return rtOOXMLCubicBezTo;
		else if (_T("close")		== strName)	return rtOOXMLClose;
		else if (_T("end")			== strName)	return rtOOXMLEnd;
		else if (_T("arcTo")		== strName)	return rtOOXMLArcTo;
		else if (_T("quadBezTo")	== strName)	return rtOOXMLQuadBezTo;

		//bRes = true;
		else if	((_T("m") == strName) || (_T("M") == strName))		return rtMoveTo;
		else if ((_T("l") == strName) || (_T("L") == strName))		return rtLineTo;
		else if ((_T("c") == strName) || (_T("C") == strName))		return rtCurveTo;
		else if ((_T("x") == strName) || (_T("Z") == strName))		return rtClose;
		else if ((_T("e") == strName) || (_T("N") == strName))		return rtEnd;
		else if (_T("t") == strName)								return rtRMoveTo;
		else if (_T("r") == strName)								return rtRLineTo;
		else if (_T("v") == strName)								return rtRCurveTo;
		else if ((_T("nf") == strName) || (_T("F") == strName))		return rtNoFill;
		else if ((_T("ns") == strName) || (_T("S") == strName))		return rtNoStroke;
		else if ((_T("ae") == strName) || (_T("T") == strName))		return rtAngleEllipseTo;
		else if ((_T("al") == strName) || (_T("U") == strName))		return rtAngleEllipse;
		else if ((_T("at") == strName) || (_T("A") == strName))		return rtArcTo;
		else if ((_T("ar") == strName) || (_T("B") == strName))		return rtArc;
		else if ((_T("wa") == strName) || (_T("W") == strName))		return rtClockwiseArcTo;
		else if ((_T("wr") == strName) || (_T("V") == strName))		return rtClockwiseArc;
		else if ((_T("qx") == strName) || (_T("X") == strName))		return rtEllipticalQuadrX;
		else if ((_T("qy") == strName) || (_T("Y") == strName))		return rtEllipticalQuadrY;
		else if ((_T("qb") == strName) || (_T("Q") == strName))		return rtQuadrBesier;
		else bRes = false;

		return rtEnd;
	}

	static double GetSweepAngle(const double& angleStart, const double& angleEnd)
	{
		if (angleStart >= angleEnd)
			return angleEnd - angleStart;
		else
			return angleEnd - angleStart - 360;
	}

	static CString GetRulerName(RulesType eRuler)
	{
		switch (eRuler)
		{
		case rtOOXMLMoveTo:		{ return _T("moveto"); }
		case rtOOXMLLineTo:		{ return _T("lineto"); }
		case rtOOXMLCubicBezTo:	{ return _T("curveto"); }
		case rtOOXMLArcTo:		{ return _T("ellipseto"); }
		case rtOOXMLQuadBezTo:	{ return _T("qbesier"); }
		case rtOOXMLClose:		{ return _T("close"); }
		case rtOOXMLEnd:		{ return _T("end"); }

		case rtMoveTo:				{ return _T("moveto"); }
		case rtLineTo:				{ return _T("lineto"); }
		case rtCurveTo:				{ return _T("curveto"); }
		
		case rtClose:				{ return _T("close"); }
		case rtEnd:					{ return _T("end"); }
		
		case rtRMoveTo:				{ return _T("rmoveto"); }
		case rtRLineTo:				{ return _T("rlineto"); }
		case rtRCurveTo:			{ return _T("rcurveto"); }
		
		case rtNoFill:				{ return _T("nofill"); }
		case rtNoStroke:			{ return _T("nostroke"); }

		case rtAngleEllipseTo:		{ return _T("ellipseto"); }
		case rtAngleEllipse:		{ return _T("ellipse"); }
		
		case rtArc:					{ return _T("arc"); }
		case rtArcTo:				{ return _T("arcto"); }

		case rtClockwiseArcTo:		{ return _T("clockwisearcto"); }
		case rtClockwiseArc:		{ return _T("clockwisearc"); }

		case rtEllipticalQuadrX:	{ return _T("ellipticalx"); }
		case rtEllipticalQuadrY:	{ return _T("ellipticaly"); }

		case rtQuadrBesier:			{ return _T("qbesier"); }
		default: break;
		};
		return _T("none");
	}

	static long GetCountPoints(RulesType eRuler)
	{
		switch (eRuler)
		{
		case rtOOXMLMoveTo:		return 1;
		case rtOOXMLLineTo:		return 1;
		case rtOOXMLQuadBezTo:	return 2;
		case rtOOXMLCubicBezTo:	return 3;
		case rtOOXMLArcTo:		return 3;//???
		case rtOOXMLClose:		return 0;
		case rtOOXMLEnd:		return 0;
		default: return 0;
		};
		return 0;
	}

	static long GetCountPoints2(RulesType eRuler, LONG lRepeatCount)
	{
		switch (eRuler)
		{
		case rtMoveTo:				
		case rtRMoveTo:
			{ return 1; }
		
		case rtLineTo:		
		case rtRLineTo:
			{ return lRepeatCount; }
		
		case rtCurveTo:		
		case rtRCurveTo:
			{ return 3 * lRepeatCount; }
		
		case rtNoFill:
		case rtNoStroke:
		case rtClose:
		case rtEnd:	
			{ return 0; }
		
		case rtAngleEllipseTo:
		case rtAngleEllipse:
			{ return lRepeatCount; }
		
		case rtArc:
		case rtArcTo:

		case rtClockwiseArcTo:
		case rtClockwiseArc:
			{ return lRepeatCount; }

		case rtEllipticalQuadrX:
		case rtEllipticalQuadrY:
			{ return 1 * lRepeatCount; }

		case rtQuadrBesier:			
			{ return /*2 * */lRepeatCount; }
		case rtFillColor:
		case rtLineColor:
			{
				return 1;
			}
		default: return 3 * lRepeatCount;
		};

		return 0;
	}
}

namespace NSPresentationEditor
{
	class CSlice
	{
	public:
		RulesType m_eRuler;
		CSimpleArray<POINT> m_arPoints;

	private:
		int m_nCountElementsPoint;

	public:
		CSlice(RulesType eType = rtMoveTo)
		{
			m_eRuler = eType;
			m_nCountElementsPoint = 0;
		}

		void AddParam(LONG lParam)
		{
			long lPoint = m_nCountElementsPoint % 2;
			if (0 == lPoint)
			{
				POINT point;
				point.x = lParam;
				point.y = 0;
				m_arPoints.Add(point);
			}
			else
			{
				m_arPoints[m_arPoints.GetSize() - 1].y = lParam;
			}
			++m_nCountElementsPoint;
		}
	#if defined(PPTX_DEF)
		void FromXML(XmlUtils::CXmlNode& Node, NSGuidesOOXML::CFormulaManager& pManager, double WidthKoef, double HeightKoef)
		{
			m_arPoints.RemoveAll();
			bool bRes = true;
			m_eRuler = GetRuler(Node.GetName(), bRes);
			if((m_eRuler != rtOOXMLClose) && (m_eRuler != rtOOXMLEnd))
			{
				if(m_eRuler != rtOOXMLArcTo)
				{
					XmlUtils::CXmlNodes list;
					Node.GetNodes(_T("pt"), list);
					for(long i = 0; i < GetCountPoints(m_eRuler); i++)
					{
						POINT lpoint;
						XmlUtils::CXmlNode pt;
						list.GetAt(i, pt);
						lpoint.x = (long)(pManager.GetValue(pt.GetAttribute(_T("x")))*WidthKoef);
						lpoint.y = (long)(pManager.GetValue(pt.GetAttribute(_T("y")))*HeightKoef);
						m_arPoints.Add(lpoint);
					}
				}
				else
				{
					POINT size;
					size.x = (long)(pManager.GetValue(Node.GetAttribute(_T("wR")))*WidthKoef);
					size.y = (long)(pManager.GetValue(Node.GetAttribute(_T("hR")))*HeightKoef);
					m_arPoints.Add(size);
					double stAng = pManager.GetValue(Node.GetAttribute(_T("stAng")));
					double swAng = pManager.GetValue(Node.GetAttribute(_T("swAng")));
					double stAng2 = atan2(HeightKoef * sin(stAng * RadKoef), WidthKoef * cos(stAng * RadKoef));
					double swAng2 = atan2(HeightKoef * sin((stAng + swAng) * RadKoef), WidthKoef * cos((stAng + swAng) * RadKoef)) - stAng2;
					POINT angle;
					angle.x = (long)(stAng2/RadKoef);//pManager.GetValue(Node.GetAttribute(_T("stAng")));
					angle.y = (long)(swAng2/RadKoef);//pManager.GetValue(Node.GetAttribute(_T("swAng")));
					if((angle.y > 0) && (swAng < 0)) angle.y = angle.y - 21600000;
					if((angle.y < 0) && (swAng > 0)) angle.y = angle.y + 21600000;
					if(angle.y == 0) angle.y = 21600000;
					//angle.x = angle.x / 60000;
					//angle.y = angle.y / 60000;
					m_arPoints.Add(angle);
				}
			}
		}
	#endif
		CString ToXml(CGeomShapeInfo& pGeomInfo, long w, long h, NSBaseShape::ClassType ClassType)
		{
			switch(ClassType)
			{
	#if defined(PPTX_DEF)
			case NSBaseShape::pptx:
				{
					CString strRes = GetRulerName(m_eRuler);

					if ((rtOOXMLClose == m_eRuler) || (rtOOXMLEnd == m_eRuler))
						return _T("<part name='") + strRes + _T("'/>");

					double dLeft = pGeomInfo.m_dLeft;
					double dTop = pGeomInfo.m_dTop;

					double dKoefX = max(pGeomInfo.m_dWidth, pGeomInfo.m_dHeight)/ShapeSize;//pGeomInfo.m_dWidth / ShapeSize;
					double dKoefY = max(pGeomInfo.m_dWidth, pGeomInfo.m_dHeight)/ShapeSize;//pGeomInfo.m_dHeight / ShapeSize;

					strRes = _T("<part name='") + strRes + _T("' path='");

					CString strPath = _T("");

					switch (m_eRuler)
					{
					case rtOOXMLMoveTo:
						{
							pGeomInfo.m_oCurPoint.dX = 0;
							pGeomInfo.m_oCurPoint.dY = 0;
						}
					case rtOOXMLLineTo:
					case rtOOXMLCubicBezTo:
						{
							int nCount = m_arPoints.GetSize();
							for (int nIndex = 0; nIndex < nCount; ++nIndex)
							{			
								double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
								double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

								//ApplyLimo(pGeomInfo, lX, lY);

								CString str = _T("");
								str.Format(_T("%lf %lf "), lX, lY);
								strPath += str;

								pGeomInfo.m_oCurPoint.dX = lX;
								pGeomInfo.m_oCurPoint.dY = lY;
							}
							break;
						}
					case rtOOXMLQuadBezTo:
						{
							CAtlArray<CGeomShapeInfo::CPointD> arPoints;
							arPoints.Add(pGeomInfo.m_oCurPoint);
							
							int nCount = m_arPoints.GetSize();
							for (int nIndex = 0; nIndex < nCount; ++nIndex)
							{			
								CGeomShapeInfo::CPointD oPoint;
								
								oPoint.dX = dLeft + (dKoefX * m_arPoints[nIndex].x);
								oPoint.dY = dTop  + (dKoefY * m_arPoints[nIndex].y);

								arPoints.Add(oPoint);
							}

							Bez2_3(arPoints, m_eRuler);

							strRes = GetRulerName(m_eRuler);
							strRes = _T("<part name='") + strRes + _T("' path='");
							
							size_t nCountNew = arPoints.GetCount();
							if (0 < nCountNew)
							{
								pGeomInfo.m_oCurPoint = arPoints[nCountNew - 1];
							}

							for (size_t nIndex = 0; nIndex < nCountNew; ++nIndex)
							{	
								CString str = _T("");
								str.Format(_T("%lf %lf "), arPoints[nIndex].dX, arPoints[nIndex].dY);
								strPath += str;
							}
							break;
						}
					case rtOOXMLArcTo:
						{
							double lX = pGeomInfo.m_oCurPoint.dX;
							double lY = pGeomInfo.m_oCurPoint.dY;
							double dAngleP = atan2(m_arPoints[0].x * sin(RadKoef * m_arPoints[1].x), m_arPoints[0].y * cos(RadKoef * m_arPoints[1].x));
							lX += (m_arPoints[0].x * dKoefX) * (-cos(dAngleP));//(-cos(RadKoef * m_arPoints[1].x));
							lY += (m_arPoints[0].y * dKoefY) * (-sin(dAngleP));//(-sin(RadKoef * m_arPoints[1].x));
							CString str;
							str = _T("");
							str.Format(_T("%lf %lf "), lX, lY);
							strPath += str;
							double cx = lX;
							double cy = lY;

							lX = (2 * dKoefX * m_arPoints[0].x);
							lY = (2 * dKoefY * m_arPoints[0].y);
							str = _T("");
							str.Format(_T("%lf %lf "), lX, lY);
							strPath += str;

							double dAngle = (m_arPoints[1].y + m_arPoints[1].x)* RadKoef;
							dAngleP = atan2(/*dKoefX */ m_arPoints[0].x * sin(dAngle), /*dKoefY */ m_arPoints[0].y * cos(dAngle));

							lX = (double)m_arPoints[1].x/60000.0;
							lY = (double)m_arPoints[1].y/60000.0;

							//if (lX < 0) 
							//	lX += 360;
							//if (lY < 0)
							//	lY += 360;
							//dRad = _hypot(m_arPoints[0].x * cos(dAngle) * dKoefX, m_arPoints[0].y * sin(dAngle) * dKoefY);
							pGeomInfo.m_oCurPoint.dX = cx + (m_arPoints[0].x * dKoefX) * cos(dAngleP);
							pGeomInfo.m_oCurPoint.dY = cy + (m_arPoints[0].y * dKoefY) * sin(dAngleP);

							str = _T("");
							str.Format(_T("%lf %lf "), lX, lY);
							strPath += str;
							break;
						}
					default:
						break;	
					};

					if (strPath.GetLength() > 1)
					{
						strPath.Delete(strPath.GetLength() - 1);
					}

					strRes += strPath;
					strRes += _T("' />");

					return strRes;
				}
	#endif
	#if defined(PPT_DEF)
			case NSBaseShape::ppt:
				{
					CString strRes = GetRulerName(m_eRuler);

					if ((rtClose == m_eRuler) || (rtNoFill == m_eRuler) ||
						(rtNoStroke == m_eRuler) || (rtEnd == m_eRuler))
						return _T("<part name='") + strRes + _T("'/>");

					double dLeft = pGeomInfo.m_dLeft;
					double dTop = pGeomInfo.m_dTop;

					double dKoefX = pGeomInfo.m_dWidth / w;
					double dKoefY = pGeomInfo.m_dHeight / h;

					strRes = _T("<part name='") + strRes + _T("' path='");

					CString strPath = _T("");

					switch (m_eRuler)
					{
					case rtMoveTo:
					case rtLineTo:
					case rtCurveTo:
					case rtEllipticalQuadrX:
					case rtEllipticalQuadrY:
						{
							if (rtMoveTo == m_eRuler)
							{
								pGeomInfo.m_oCurPoint.dX = 0;
								pGeomInfo.m_oCurPoint.dY = 0;
							}
							int nCount = m_arPoints.GetSize();
							for (int nIndex = 0; nIndex < nCount; ++nIndex)
							{			
								double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
								double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

								//ApplyLimo(pGeomInfo, lX, lY);

								CString str = _T("");
								str.Format(_T("%lf %lf "), lX, lY);
								
								//strPath += (NSStringUtils::ToString(lX) + _T(" "));
								//strPath += (NSStringUtils::ToString(lY) + _T(" "));
								strPath += str;

								pGeomInfo.m_oCurPoint.dX = lX;
								pGeomInfo.m_oCurPoint.dY = lY;
							}
							break;
						}
					case rtQuadrBesier:
						{
							CAtlArray<CGeomShapeInfo::CPointD> arPoints;
							arPoints.Add(pGeomInfo.m_oCurPoint);
							
							int nCount = m_arPoints.GetSize();
							for (int nIndex = 0; nIndex < nCount; ++nIndex)
							{			
								CGeomShapeInfo::CPointD oPoint;
								
								oPoint.dX = dLeft + (dKoefX * m_arPoints[nIndex].x);
								oPoint.dY = dTop  + (dKoefY * m_arPoints[nIndex].y);

								arPoints.Add(oPoint);
							}

							Bez2_3(arPoints, m_eRuler);

							strRes = GetRulerName(m_eRuler);
							strRes = _T("<part name='") + strRes + _T("' path='");
							
							size_t nCountNew = arPoints.GetCount();
							if (0 < nCountNew)
							{
								pGeomInfo.m_oCurPoint = arPoints[nCountNew - 1];
							}

							for (size_t nIndex = 0; nIndex < nCountNew; ++nIndex)
							{	
								CString str = _T("");
								str.Format(_T("%lf %lf "), arPoints[nIndex].dX, arPoints[nIndex].dY);
								strPath += str;
							}
							break;
						}
					case rtArcTo:
					case rtClockwiseArcTo:
						{
							int nCount = m_arPoints.GetSize();
							for (int nIndex = 0; nIndex < nCount; ++nIndex)
							{			
								double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
								double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

								//ApplyLimo(pGeomInfo, lX, lY);

								CString str = _T("");
								str.Format(_T("%lf %lf "), lX, lY);
								
								/*strPath += (NSStringUtils::ToString(lX) + _T(" "));
								strPath += (NSStringUtils::ToString(lY) + _T(" "));*/
								strPath += str;

								if (nIndex % 4 != 1)
								{
									pGeomInfo.m_oCurPoint.dX = lX;
									pGeomInfo.m_oCurPoint.dY = lY;
								}
							}
							break;
						}
					case rtArc:
					case rtClockwiseArc:
						{
							int nCount = m_arPoints.GetSize();
							for (int nIndex = 0; nIndex < nCount; ++nIndex)
							{			
								double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
								double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

								//ApplyLimo(pGeomInfo, lX, lY);

								CString str = _T("");
								str.Format(_T("%lf %lf "), lX, lY);
								
								/*strPath += (NSStringUtils::ToString(lX) + _T(" "));
								strPath += (NSStringUtils::ToString(lY) + _T(" "));*/
								strPath += str;

								if (nIndex % 4 > 1)
								{
									pGeomInfo.m_oCurPoint.dX = lX;
									pGeomInfo.m_oCurPoint.dY = lY;
								}
							}
							break;
						}
					case rtRMoveTo:
					case rtRLineTo:
					case rtRCurveTo:
						{
							int nCount = m_arPoints.GetSize();
							for (int nIndex = 0; nIndex < nCount; ++nIndex)
							{
								double lX = pGeomInfo.m_oCurPoint.dX + (dKoefX * m_arPoints[nIndex].x);
								double lY = pGeomInfo.m_oCurPoint.dY + (dKoefY * m_arPoints[nIndex].y);

								//ApplyLimo(pGeomInfo, lX, lY);

								lX -= pGeomInfo.m_oCurPoint.dX;
								lY -= pGeomInfo.m_oCurPoint.dY;

								CString str = _T("");
								str.Format(_T("%lf %lf "), lX, lY);
								
								/*strPath += (NSStringUtils::ToString(lX) + _T(" "));
								strPath += (NSStringUtils::ToString(lY) + _T(" "));*/
								strPath += str;

								pGeomInfo.m_oCurPoint.dX += lX;
								pGeomInfo.m_oCurPoint.dY += lY;
							}
							break;
						}
					case rtAngleEllipseTo:
					case rtAngleEllipse:
						{
							int nCount = m_arPoints.GetSize();
							for (int nIndex = 0; nIndex < nCount; ++nIndex)
							{			
								double lX = 0;
								double lY = 0;
								if (nIndex % 3 == 0)
								{
									lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
									lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

									CString str = _T("");
									str.Format(_T("%lf %lf "), lX, lY);
									
									//strPath += (NSStringUtils::ToString(lX) + _T(" "));
									//strPath += (NSStringUtils::ToString(lY) + _T(" "));
									strPath += str;
									continue;
								}
								else if (nIndex % 3 == 1)
								{
									lX = (2 * dKoefX * m_arPoints[nIndex].x);
									lY = (2 * dKoefY * m_arPoints[nIndex].y);

									CString str = _T("");
									str.Format(_T("%lf %lf "), lX, lY);
									
									/*strPath += (NSStringUtils::ToString(lX) + _T(" "));
									strPath += (NSStringUtils::ToString(lY) + _T(" "));*/
									strPath += str;
									continue;
								}
								else 
								{
									double dCx = dLeft + (dKoefX * m_arPoints[nIndex - 2].x);
									double dCy = dTop  + (dKoefY * m_arPoints[nIndex - 2].y);

									double dRadX = dKoefX * m_arPoints[nIndex - 1].x;
									double dRadY = dKoefY * m_arPoints[nIndex - 1].y;
									double dAng = (m_arPoints[nIndex].x + m_arPoints[nIndex].y) / pow2_16;
									dAng *= (M_PI / pow2_16);

									double dcos = cos(dAng);
									double dsin = sin(dAng);
									double rad = 1 / sqrt(dRadX * dRadX * dcos * dcos + dRadY * dRadY * dsin * dsin);

									pGeomInfo.m_oCurPoint.dX = (dCx + rad * dcos);
									pGeomInfo.m_oCurPoint.dY = (dCy + rad * dsin);

									lX = m_arPoints[nIndex].x / pow2_16;
									lY = m_arPoints[nIndex].y / pow2_16;
								}

								
								CString str = _T("");
								str.Format(_T("%lf %lf "), -lX, -lY/*GetSweepAngle(lX, lY)*/);
								
								strPath += str;
								//strPath += (NSStringUtils::ToString(lX) + _T(" "));
								//strPath += (NSStringUtils::ToString(lY - lX) + _T(" "));
							}
							break;
						}
					default:
						break;	
					};

					if (strPath.GetLength() > 1)
					{
						strPath.Delete(strPath.GetLength() - 1);
					}

					strRes += strPath;
					strRes += _T("' />");

					return strRes;
				}
	#endif
	#if defined(ODP_DEF)
			case NSBaseShape::odp:
				{
					CString strRes = GetRulerName(m_eRuler);

					if ((rtClose == m_eRuler) || (rtNoFill == m_eRuler) ||
						(rtNoStroke == m_eRuler) || (rtEnd == m_eRuler))
						return _T("<part name='") + strRes + _T("'/>");

					double dLeft = pGeomInfo.m_dLeft;
					double dTop = pGeomInfo.m_dTop;

					double dKoefX = pGeomInfo.m_dWidth / w;
					double dKoefY = pGeomInfo.m_dHeight / h;

					strRes = _T("<part name='") + strRes + _T("' path='");

					CString strPath = _T("");

					switch (m_eRuler)
					{
					case rtMoveTo:
					case rtLineTo:
					case rtCurveTo:
					case rtEllipticalQuadrX:
					case rtEllipticalQuadrY:
						{
							if (rtMoveTo == m_eRuler)
							{
								pGeomInfo.m_oCurPoint.dX = 0;
								pGeomInfo.m_oCurPoint.dY = 0;
							}
							int nCount = m_arPoints.GetSize();
							for (int nIndex = 0; nIndex < nCount; ++nIndex)
							{			
								double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
								double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

								//ApplyLimo(pGeomInfo, lX, lY);

								CString str = _T("");
								str.Format(_T("%lf %lf "), lX, lY);
								
								//strPath += (NSStringUtils::ToString(lX) + _T(" "));
								//strPath += (NSStringUtils::ToString(lY) + _T(" "));
								strPath += str;

								pGeomInfo.m_oCurPoint.dX = lX;
								pGeomInfo.m_oCurPoint.dY = lY;
							}
							break;
						}
					case rtQuadrBesier:
						{
							CAtlArray<CGeomShapeInfo::CPointD> arPoints;
							arPoints.Add(pGeomInfo.m_oCurPoint);
							
							int nCount = m_arPoints.GetSize();
							for (int nIndex = 0; nIndex < nCount; ++nIndex)
							{			
								CGeomShapeInfo::CPointD oPoint;
								
								oPoint.dX = dLeft + (dKoefX * m_arPoints[nIndex].x);
								oPoint.dY = dTop  + (dKoefY * m_arPoints[nIndex].y);

								arPoints.Add(oPoint);
							}

							Bez2_3(arPoints, m_eRuler);

							strRes = GetRulerName(m_eRuler);
							strRes = _T("<part name='") + strRes + _T("' path='");
							
							size_t nCountNew = arPoints.GetCount();
							if (0 < nCountNew)
							{
								pGeomInfo.m_oCurPoint = arPoints[nCountNew - 1];
							}

							for (size_t nIndex = 0; nIndex < nCountNew; ++nIndex)
							{	
								CString str = _T("");
								str.Format(_T("%lf %lf "), arPoints[nIndex].dX, arPoints[nIndex].dY);
								strPath += str;
							}
							break;
						}
					case rtArcTo:
					case rtClockwiseArcTo:
						{
							int nCount = m_arPoints.GetSize();
							for (int nIndex = 0; nIndex < nCount; ++nIndex)
							{			
								double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
								double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

								//ApplyLimo(pGeomInfo, lX, lY);

								CString str = _T("");
								str.Format(_T("%lf %lf "), lX, lY);
								
								/*strPath += (NSStringUtils::ToString(lX) + _T(" "));
								strPath += (NSStringUtils::ToString(lY) + _T(" "));*/
								strPath += str;

								if (nIndex % 4 != 1)
								{
									pGeomInfo.m_oCurPoint.dX = lX;
									pGeomInfo.m_oCurPoint.dY = lY;
								}
							}
							break;
						}
					case rtArc:
					case rtClockwiseArc:
						{
							int nCount = m_arPoints.GetSize();
							for (int nIndex = 0; nIndex < nCount; ++nIndex)
							{			
								double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
								double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

								//ApplyLimo(pGeomInfo, lX, lY);

								CString str = _T("");
								str.Format(_T("%lf %lf "), lX, lY);
								
								/*strPath += (NSStringUtils::ToString(lX) + _T(" "));
								strPath += (NSStringUtils::ToString(lY) + _T(" "));*/
								strPath += str;

								if (nIndex % 4 > 1)
								{
									pGeomInfo.m_oCurPoint.dX = lX;
									pGeomInfo.m_oCurPoint.dY = lY;
								}
							}
							break;
						}
					case rtAngleEllipseTo:
					case rtAngleEllipse:
						{
							int nCount = m_arPoints.GetSize();
							for (int nIndex = 0; nIndex < nCount; ++nIndex)
							{			
								double lX = 0;
								double lY = 0;
								if (nIndex % 3 == 0)
								{
									lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
									lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

									CString str = _T("");
									str.Format(_T("%lf %lf "), lX, lY);
									
									//strPath += (NSStringUtils::ToString(lX) + _T(" "));
									//strPath += (NSStringUtils::ToString(lY) + _T(" "));
									strPath += str;
									continue;
								}
								else if (nIndex % 3 == 1)
								{
									lX = (2 * dKoefX * m_arPoints[nIndex].x);
									lY = (2 * dKoefY * m_arPoints[nIndex].y);

									CString str = _T("");
									str.Format(_T("%lf %lf "), lX, lY);
									
									/*strPath += (NSStringUtils::ToString(lX) + _T(" "));
									strPath += (NSStringUtils::ToString(lY) + _T(" "));*/
									strPath += str;
									continue;
								}
								else 
								{
									double dCx = dLeft + (dKoefX * m_arPoints[nIndex - 2].x);
									double dCy = dTop  + (dKoefY * m_arPoints[nIndex - 2].y);

									double dRad = _hypot(dKoefX * m_arPoints[nIndex - 1].x, dKoefY * m_arPoints[nIndex - 1].y);
									double dAngle = (double)m_arPoints[nIndex].y / pow2_16;
									dAngle *= (3.14159265358979323846 / 180.0);

									//lX = m_arPoints[nIndex].x / pow2_16;
									//lY = m_arPoints[nIndex].y / pow2_16;
									int lx = m_arPoints[nIndex].x / pow2_16;
									int ly = m_arPoints[nIndex].y / pow2_16;

									if (lx < 0) lx += 360;
									if (ly < 0)     ly += 360;
									if (ly == lx) ly += 360;
									lX = lx;
									lY = ly;

									pGeomInfo.m_oCurPoint.dX = (dCx + dRad * cos(dAngle));//???
									pGeomInfo.m_oCurPoint.dY = (dCy - dRad * sin(dAngle));//???
								}

								
								CString str = _T("");
								str.Format(_T("%lf %lf "), lX, lY - lX);
								
								strPath += str;
								//strPath += (NSStringUtils::ToString(lX) + _T(" "));
								//strPath += (NSStringUtils::ToString(lY - lX) + _T(" "));
							}
							break;
						}
					default:
						break;	
					};

					if (strPath.GetLength() > 1)
					{
						strPath.Delete(strPath.GetLength() - 1);
					}

					strRes += strPath;
					strRes += _T("' />");

					return strRes;
				}
	#endif
			};

			return _T("");
		}

		void ToRenderer(CGraphicPath* pRenderer, CGeomShapeInfo& pGeomInfo, long w, long h, NSBaseShape::ClassType ClassType)
		{
			switch(ClassType)
			{
	#if defined(PPTX_DEF)
			case NSBaseShape::pptx:
			{
				if (rtOOXMLClose == m_eRuler)
				{
					pRenderer->AddRuler(rtClose);
					return;
				}
				if (rtOOXMLEnd == m_eRuler)
				{
					pRenderer->AddRuler(rtEnd);
					return;
				}
				
				double dLeft	= pGeomInfo.m_dLeft;
				double dTop		= pGeomInfo.m_dTop;

				double dKoefX = max(pGeomInfo.m_dWidth, pGeomInfo.m_dHeight) / ShapeSize;
				double dKoefY = max(pGeomInfo.m_dWidth, pGeomInfo.m_dHeight) / ShapeSize;

				switch (m_eRuler)
				{
				case rtOOXMLMoveTo:
					{
						pRenderer->AddRuler(rtMoveTo);

						pGeomInfo.m_oCurPoint.dX = 0;
						pGeomInfo.m_oCurPoint.dY = 0;
					}
				case rtOOXMLLineTo:
				case rtOOXMLCubicBezTo:
					{
						if (rtOOXMLLineTo == m_eRuler)
							pRenderer->AddRuler(rtLineTo);
						else if (rtOOXMLCubicBezTo == m_eRuler)
							pRenderer->AddRuler(rtCurveTo);

						int nCount = m_arPoints.GetSize();
						for (int nIndex = 0; nIndex < nCount; ++nIndex)
						{			
							double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
							double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

							pRenderer->AddPoint(lX, lY);

							pGeomInfo.m_oCurPoint.dX = lX;
							pGeomInfo.m_oCurPoint.dY = lY;
						}
						break;
					}
				case rtOOXMLQuadBezTo:
					{
						CAtlArray<CGeomShapeInfo::CPointD> arPoints;
						arPoints.Add(pGeomInfo.m_oCurPoint);
						
						int nCount = m_arPoints.GetSize();
						for (int nIndex = 0; nIndex < nCount; ++nIndex)
						{			
							CGeomShapeInfo::CPointD oPoint;
							
							oPoint.dX = dLeft + (dKoefX * m_arPoints[nIndex].x);
							oPoint.dY = dTop  + (dKoefY * m_arPoints[nIndex].y);

							arPoints.Add(oPoint);
						}

						Bez2_3(arPoints, m_eRuler);

						pRenderer->AddRuler(rtCurveTo);
						
						size_t nCountNew = arPoints.GetCount();
						if (0 < nCountNew)
						{
							pGeomInfo.m_oCurPoint = arPoints[nCountNew - 1];
						}

						for (size_t nIndex = 0; nIndex < nCountNew; ++nIndex)
						{	
							pRenderer->AddPoint(arPoints[nIndex].dX, arPoints[nIndex].dY);
						}
						break;
					}
				case rtOOXMLArcTo:
					{
						pRenderer->AddRuler(rtAngleEllipseTo);

						double lX = pGeomInfo.m_oCurPoint.dX;
						double lY = pGeomInfo.m_oCurPoint.dY;
						double dAngleP = atan2(m_arPoints[0].x * sin(RadKoef * m_arPoints[1].x), m_arPoints[0].y * cos(RadKoef * m_arPoints[1].x));
						lX += (m_arPoints[0].x * dKoefX) * (-cos(dAngleP));//(-cos(RadKoef * m_arPoints[1].x));
						lY += (m_arPoints[0].y * dKoefY) * (-sin(dAngleP));//(-sin(RadKoef * m_arPoints[1].x));
						
						pRenderer->AddPoint(lX, lY);

						double cx = lX;
						double cy = lY;

						lX = (2 * dKoefX * m_arPoints[0].x);
						lY = (2 * dKoefY * m_arPoints[0].y);

						pRenderer->AddPoint(lX, lY);

						double dAngle = (m_arPoints[1].y + m_arPoints[1].x)* RadKoef;
						dAngleP = atan2(/*dKoefX */ m_arPoints[0].x * sin(dAngle), /*dKoefY */ m_arPoints[0].y * cos(dAngle));

						lX = (double)m_arPoints[1].x/60000.0;
						lY = (double)m_arPoints[1].y/60000.0;

						pGeomInfo.m_oCurPoint.dX = cx + (m_arPoints[0].x * dKoefX) * cos(dAngleP);
						pGeomInfo.m_oCurPoint.dY = cy + (m_arPoints[0].y * dKoefY) * sin(dAngleP);

						pRenderer->AddPoint(lX, lY);
						break;
					}
				default:
					break;	
				};
				break;
			}
	#endif
	#if defined(PPT_DEF)
			case NSBaseShape::ppt:
			{
				if ((rtClose == m_eRuler) || (rtNoFill == m_eRuler) ||
					(rtNoStroke == m_eRuler) || (rtEnd == m_eRuler))
				{
					pRenderer->AddRuler(m_eRuler);
					return;
				}

				double dLeft	= pGeomInfo.m_dLeft;
				double dTop		= pGeomInfo.m_dTop;

				double dKoefX	= pGeomInfo.m_dWidth / w;
				double dKoefY	= pGeomInfo.m_dHeight / h;

				switch (m_eRuler)
				{
				case rtMoveTo:
				case rtLineTo:
				case rtCurveTo:
				case rtEllipticalQuadrX:
				case rtEllipticalQuadrY:
					{
						pRenderer->AddRuler(m_eRuler);
						if (rtMoveTo == m_eRuler)
						{
							pGeomInfo.m_oCurPoint.dX = 0;
							pGeomInfo.m_oCurPoint.dY = 0;
						}
						int nCount = m_arPoints.GetSize();
						for (int nIndex = 0; nIndex < nCount; ++nIndex)
						{			
							double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
							double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

							pRenderer->AddPoint(lX, lY);
							
							pGeomInfo.m_oCurPoint.dX = lX;
							pGeomInfo.m_oCurPoint.dY = lY;
						}
						break;
					}
				case rtQuadrBesier:
					{
						CAtlArray<CGeomShapeInfo::CPointD> arPoints;
						arPoints.Add(pGeomInfo.m_oCurPoint);
						
						int nCount = m_arPoints.GetSize();
						for (int nIndex = 0; nIndex < nCount; ++nIndex)
						{			
							CGeomShapeInfo::CPointD oPoint;
							
							oPoint.dX = dLeft + (dKoefX * m_arPoints[nIndex].x);
							oPoint.dY = dTop  + (dKoefY * m_arPoints[nIndex].y);

							arPoints.Add(oPoint);
						}

						Bez2_3(arPoints, m_eRuler);

						pRenderer->AddRuler(rtCurveTo);
						
						size_t nCountNew = arPoints.GetCount();
						if (0 < nCountNew)
						{
							pGeomInfo.m_oCurPoint = arPoints[nCountNew - 1];
						}

						for (size_t nIndex = 0; nIndex < nCountNew; ++nIndex)
						{	
							pRenderer->AddPoint(arPoints[nIndex].dX, arPoints[nIndex].dY);
						}
						break;
					}
				case rtArcTo:
				case rtClockwiseArcTo:
					{
						pRenderer->AddRuler(m_eRuler);
						
						int nCount = m_arPoints.GetSize();
						for (int nIndex = 0; nIndex < nCount; ++nIndex)
						{			
							double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
							double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

							pRenderer->AddPoint(lX, lY);

							if (nIndex % 4 != 1)
							{
								pGeomInfo.m_oCurPoint.dX = lX;
								pGeomInfo.m_oCurPoint.dY = lY;
							}
						}
						break;
					}
				case rtArc:
				case rtClockwiseArc:
					{
						pRenderer->AddRuler(m_eRuler);

						int nCount = m_arPoints.GetSize();
						for (int nIndex = 0; nIndex < nCount; ++nIndex)
						{			
							double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
							double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

							pRenderer->AddPoint(lX, lY);

							if (nIndex % 4 > 1)
							{
								pGeomInfo.m_oCurPoint.dX = lX;
								pGeomInfo.m_oCurPoint.dY = lY;
							}
						}
						break;
					}
				case rtRMoveTo:
				case rtRLineTo:
				case rtRCurveTo:
					{
						pRenderer->AddRuler(m_eRuler);
						
						int nCount = m_arPoints.GetSize();
						for (int nIndex = 0; nIndex < nCount; ++nIndex)
						{
							double lX = pGeomInfo.m_oCurPoint.dX + (dKoefX * m_arPoints[nIndex].x);
							double lY = pGeomInfo.m_oCurPoint.dY + (dKoefY * m_arPoints[nIndex].y);


							lX -= pGeomInfo.m_oCurPoint.dX;
							lY -= pGeomInfo.m_oCurPoint.dY;

							pRenderer->AddPoint(lX, lY);

							pGeomInfo.m_oCurPoint.dX += lX;
							pGeomInfo.m_oCurPoint.dY += lY;
						}
						break;
					}
				case rtAngleEllipseTo:
				case rtAngleEllipse:
					{
						pRenderer->AddRuler(m_eRuler);
						
						int nCount = m_arPoints.GetSize();
						for (int nIndex = 0; nIndex < nCount; ++nIndex)
						{			
							double lX = 0;
							double lY = 0;
							if (nIndex % 3 == 0)
							{
								lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
								lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

								pRenderer->AddPoint(lX, lY);
								continue;
							}
							else if (nIndex % 3 == 1)
							{
								lX = (2 * dKoefX * m_arPoints[nIndex].x);
								lY = (2 * dKoefY * m_arPoints[nIndex].y);

								pRenderer->AddPoint(lX, lY);
								continue;
							}
							else 
							{
								double dCx = dLeft + (dKoefX * m_arPoints[nIndex - 2].x);
								double dCy = dTop  + (dKoefY * m_arPoints[nIndex - 2].y);

								double dRadX = dKoefX * m_arPoints[nIndex - 1].x;
								double dRadY = dKoefY * m_arPoints[nIndex - 1].y;
								double dAng = (m_arPoints[nIndex].x + m_arPoints[nIndex].y) / pow2_16;
								dAng *= (M_PI / pow2_16);

								double dcos = cos(dAng);
								double dsin = sin(dAng);
								double rad = 1 / sqrt(dRadX * dRadX * dcos * dcos + dRadY * dRadY * dsin * dsin);

								pGeomInfo.m_oCurPoint.dX = (dCx + rad * dcos);
								pGeomInfo.m_oCurPoint.dY = (dCy + rad * dsin);

								lX = m_arPoints[nIndex].x / pow2_16;
								lY = m_arPoints[nIndex].y / pow2_16;
							}

							pRenderer->AddPoint(-lX, -lY/*GetSweepAngle(lX, lY)*/);
						}
						break;
					}
				default:
					break;	
				};
				break;
			}
	#endif
	#if defined(ODP_DEF)
			case NSBaseShape::odp:
			{
				if ((rtClose == m_eRuler) || (rtNoFill == m_eRuler) ||
					(rtNoStroke == m_eRuler) || (rtEnd == m_eRuler))
				{
					pRenderer->AddRuler(m_eRuler);
					return;
				}

				double dLeft = pGeomInfo.m_dLeft;
				double dTop = pGeomInfo.m_dTop;

				double dKoefX = pGeomInfo.m_dWidth / w;
				double dKoefY = pGeomInfo.m_dHeight / h;

				switch (m_eRuler)
				{
				case rtMoveTo:
				case rtLineTo:
				case rtCurveTo:
				case rtEllipticalQuadrX:
				case rtEllipticalQuadrY:
					{
						pRenderer->AddRuler(m_eRuler);
						
						if (rtMoveTo == m_eRuler)
						{
							pGeomInfo.m_oCurPoint.dX = 0;
							pGeomInfo.m_oCurPoint.dY = 0;
						}
						int nCount = m_arPoints.GetSize();
						for (int nIndex = 0; nIndex < nCount; ++nIndex)
						{			
							double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
							double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

							pRenderer->AddPoint(lX, lY);

							pGeomInfo.m_oCurPoint.dX = lX;
							pGeomInfo.m_oCurPoint.dY = lY;
						}
						break;
					}
				case rtQuadrBesier:
					{
						CAtlArray<CGeomShapeInfo::CPointD> arPoints;
						arPoints.Add(pGeomInfo.m_oCurPoint);
						
						int nCount = m_arPoints.GetSize();
						for (int nIndex = 0; nIndex < nCount; ++nIndex)
						{			
							CGeomShapeInfo::CPointD oPoint;
							
							oPoint.dX = dLeft + (dKoefX * m_arPoints[nIndex].x);
							oPoint.dY = dTop  + (dKoefY * m_arPoints[nIndex].y);

							arPoints.Add(oPoint);
						}

						Bez2_3(arPoints, m_eRuler);

						pRenderer->AddRuler(m_eRuler);
						
						size_t nCountNew = arPoints.GetCount();
						if (0 < nCountNew)
						{
							pGeomInfo.m_oCurPoint = arPoints[nCountNew - 1];
						}

						for (size_t nIndex = 0; nIndex < nCountNew; ++nIndex)
						{	
							pRenderer->AddPoint(arPoints[nIndex].dX, arPoints[nIndex].dY);
						}
						break;
					}
				case rtArcTo:
				case rtClockwiseArcTo:
					{
						pRenderer->AddRuler(m_eRuler);
						
						int nCount = m_arPoints.GetSize();
						for (int nIndex = 0; nIndex < nCount; ++nIndex)
						{			
							double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
							double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

							pRenderer->AddPoint(lX, lY);

							if (nIndex % 4 != 1)
							{
								pGeomInfo.m_oCurPoint.dX = lX;
								pGeomInfo.m_oCurPoint.dY = lY;
							}
						}
						break;
					}
				case rtArc:
				case rtClockwiseArc:
					{
						pRenderer->AddRuler(m_eRuler);
						
						int nCount = m_arPoints.GetSize();
						for (int nIndex = 0; nIndex < nCount; ++nIndex)
						{			
							double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
							double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

							pRenderer->AddPoint(lX, lY);

							if (nIndex % 4 > 1)
							{
								pGeomInfo.m_oCurPoint.dX = lX;
								pGeomInfo.m_oCurPoint.dY = lY;
							}
						}
						break;
					}
				case rtAngleEllipseTo:
				case rtAngleEllipse:
					{
						pRenderer->AddRuler(m_eRuler);
						
						int nCount = m_arPoints.GetSize();
						for (int nIndex = 0; nIndex < nCount; ++nIndex)
						{			
							double lX = 0;
							double lY = 0;
							if (nIndex % 3 == 0)
							{
								lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
								lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

								pRenderer->AddPoint(lX, lY);
								continue;
							}
							else if (nIndex % 3 == 1)
							{
								lX = (2 * dKoefX * m_arPoints[nIndex].x);
								lY = (2 * dKoefY * m_arPoints[nIndex].y);

								pRenderer->AddPoint(lX, lY);
								continue;
							}
							else 
							{
								double dCx = dLeft + (dKoefX * m_arPoints[nIndex - 2].x);
								double dCy = dTop  + (dKoefY * m_arPoints[nIndex - 2].y);

								double dRad = _hypot(dKoefX * m_arPoints[nIndex - 1].x, dKoefY * m_arPoints[nIndex - 1].y);
								double dAngle = (double)m_arPoints[nIndex].y / pow2_16;
								dAngle *= (3.14159265358979323846 / 180.0);

								//lX = m_arPoints[nIndex].x / pow2_16;
								//lY = m_arPoints[nIndex].y / pow2_16;
								int lx = m_arPoints[nIndex].x / pow2_16;
								int ly = m_arPoints[nIndex].y / pow2_16;

								if (lx < 0) lx += 360;
								if (ly < 0)     ly += 360;
								if (ly == lx) ly += 360;
								lX = lx;
								lY = ly;

								pGeomInfo.m_oCurPoint.dX = (dCx + dRad * cos(dAngle));//???
								pGeomInfo.m_oCurPoint.dY = (dCy - dRad * sin(dAngle));//???
							}

							pRenderer->AddPoint(lX, lY - lX);
						}
						break;
					}
				default:
					break;	
				};
				break;
			}
	#endif
			};

		}
		
	protected:
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
			fAngle *= (180 / M_PI);
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
			double& Left, double& Top, double& Width, double& Height, const CGeomShapeInfo::CPointD& pointCur)
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
					Left = pointCur.dX - Width / 2;
					Top = pointCur.dY - Height;
				}
				else if ((Width < 0) && (Height > 0))
				{
					angleStart = -90;
					angleSweet = -90;
					Width *= -1;
					Left = pointCur.dX - Width / 2;
					Top = pointCur.dY;
				}
				else if ((Width > 0) && (Height < 0))
				{
					angleStart = 90;
					angleSweet = -90;
					Height *= -1;
					Left = pointCur.dX - Width / 2;
					Top = pointCur.dY - Height;
				}
				else
				{
					Left = pointCur.dX - Width / 2;
					Top = pointCur.dY;
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
					Left = pointCur.dX - Width;
					Top = pointCur.dY - Height / 2;
				}
				else if ((Width < 0) && (Height > 0))
				{
					angleStart = 0;
					angleSweet = 90;
					Width *= -1;
					Left = pointCur.dX - Width;
					Top = pointCur.dY - Height / 2;
				}
				else if ((Width > 0) && (Height < 0))
				{
					angleStart = 180;
					angleSweet = 90;
					Height *= -1;
					Left = pointCur.dX;
					Top = pointCur.dY - Height / 2;
				}
				else
				{
					Left = pointCur.dX;
					Top = pointCur.dY - Height / 2;
				}
			}
			bIsX = !bIsX;
		}



		CSlice& operator =(const CSlice& oSrc)
		{
			m_eRuler = oSrc.m_eRuler;
			m_arPoints.RemoveAll();
			for (int nIndex = 0; nIndex < oSrc.m_arPoints.GetSize(); ++nIndex)
			{
				m_arPoints.Add(oSrc.m_arPoints[nIndex]);
			}
			return (*this);
		}

		void ApplyLimo(CGeomShapeInfo& pGeomInfo, double& lX, double& lY)
		{
			if ((0 == pGeomInfo.m_dLimoX) || (0 == pGeomInfo.m_dLimoY))
				return;

			double dAspect = (double)pGeomInfo.m_dLimoX / pGeomInfo.m_dLimoY;
			double lWidth  = (dAspect * pGeomInfo.m_dHeight);

			if (lWidth < pGeomInfo.m_dWidth)
			{
				// LimoX
				double lXc = pGeomInfo.m_dLeft + pGeomInfo.m_dWidth / 2;
				if ((lX > lXc) || ((lX == lXc) && (pGeomInfo.m_oCurPoint.dX >= lXc)))
				{
					double lXNew = pGeomInfo.m_dLeft + ((lWidth / pGeomInfo.m_dWidth) * (lX - pGeomInfo.m_dLeft));
					lXNew += (pGeomInfo.m_dWidth - lWidth);
					lX = lXNew;
				}
				//if (lX >= lXc)
				//{
				//	LONG lXNew = pGeomInfo->m_lLeft + (LONG)(((double)lWidth / pGeomInfo->m_lWidth) * (lX - pGeomInfo->m_lLeft));
				//	if (pGeomInfo->m_oCurPoint.x >= lXc)
				//	{
				//		lXNew += (pGeomInfo->m_lWidth - lWidth);
				//	}
				//	lX = lXNew;
				//}
			}
			else if (lWidth != pGeomInfo.m_dWidth)
			{
				// LimoY
				double lHeight = (pGeomInfo.m_dWidth / dAspect);
				double lYc = pGeomInfo.m_dTop + pGeomInfo.m_dHeight / 2;
				if ((lY > lYc) || ((lY == lYc) && (pGeomInfo.m_oCurPoint.dY >= lYc)))
				{
					double lYNew = pGeomInfo.m_dTop + ((lHeight / pGeomInfo.m_dHeight) * (lY - pGeomInfo.m_dTop));
					lYNew += (pGeomInfo.m_dHeight - lHeight);
					lY = lYNew;
				}
			}
		}

		void Bez2_3(CAtlArray<CGeomShapeInfo::CPointD>& oArray, RulesType& eType)
		{
			if (rtQuadrBesier == eType)
			{
				eType = rtCurveTo;
			}
			else if (rtOOXMLQuadBezTo == eType)
			{
				eType = rtOOXMLCubicBezTo;
			}
			else
			{
				return;
			}
			
			CAtlArray<CGeomShapeInfo::CPointD> arOld;
			arOld.Copy(oArray);

			oArray.RemoveAll();
			
			size_t nStart	= 0;
			size_t nEnd	= 2;

			size_t nCount = arOld.GetCount();
			while (nStart < (nCount - 1))
			{
				if (2 >= (nCount - nStart))
				{
					// по идее такого быть не может
					for (size_t i = nStart; i < nCount; ++i)
					{
						oArray.Add(arOld[i]);
					}
					
					nStart = nCount;
					break;
				}
				
				if (4 == (nCount - nStart))
				{
					// ничего не поделаешь... делаем кривую третьего порядка
					oArray.Add(arOld[nStart]);
					oArray.Add(arOld[nStart + 1]);
					oArray.Add(arOld[nStart + 2]);
					oArray.Add(arOld[nStart + 3]);

					nStart += 4;
					break;
				}

				// значит есть еще
				CGeomShapeInfo::CPointD mem1;
				mem1.dX = (arOld[nStart].dX + 2 * arOld[nStart + 1].dX) / 3.0;
				mem1.dY = (arOld[nStart].dY + 2 * arOld[nStart + 1].dY) / 3.0;

				CGeomShapeInfo::CPointD mem2;
				mem2.dX = (2 * arOld[nStart + 1].dX + arOld[nStart + 2].dX) / 3.0;
				mem2.dY = (2 * arOld[nStart + 1].dY + arOld[nStart + 2].dY) / 3.0;

				oArray.Add(mem1);
				oArray.Add(mem2);
				oArray.Add(arOld[nStart + 2]);

				nStart += 2;
			}
		}
	};

	class CPartPath
	{
	public:
		bool m_bFill;
		bool m_bStroke;
		long width;
		long height;
		CSimpleArray<CSlice> m_arSlices;

	public:
		CPartPath() : m_arSlices()
		{
			m_bFill = true;
			m_bStroke = true;
			width = 43200;
			height = 43200;
		}
	#if defined(PPTX_DEF)
		void FromXML(XmlUtils::CXmlNode& PathNode, NSGuidesOOXML::CFormulaManager& pManager)
		{
			m_bFill = PathNode.GetAttribute(_T("fill"), _T("norm")) != _T("none");
			CString stroke = PathNode.GetAttribute(_T("stroke"), _T("true"));
			m_bStroke = (stroke == _T("true")) || (stroke == _T("1"));
			width = (long)XmlUtils::GetInteger(PathNode.GetAttribute(_T("w"), _T("0")));
			height = (long)XmlUtils::GetInteger(PathNode.GetAttribute(_T("h"), _T("0")));
			if(width == 0) width = (long)pManager.GetWidth();
			if(height == 0) height = (long)pManager.GetHeight();

			XmlUtils::CXmlNodes list;
			PathNode.GetNodes(_T("*"), list);
			for(long i = 0; i < list.GetCount(); i++)
			{
				CSlice slice;
				XmlUtils::CXmlNode node;
				list.GetAt(i, node);
				slice.FromXML(node, pManager, pManager.GetWidth()/width, pManager.GetHeight()/height);
				m_arSlices.Add(slice);
			}

			//CSlice EndSlice;
			//EndSlice.m_eRuler = rtEnd;
			//m_arSlices.Add(EndSlice);
		}
	#endif
	#if defined(PPT_DEF)
		void FromXML(CString strPath, NSGuidesVML::CFormulasManager& pManager)
		{
			NSStringUtils::CheckPathOn_Fill_Stroke(strPath, m_bFill, m_bStroke);
			CSimpleArray<CString> oArray;

			NSStringUtils::ParsePath2(strPath, &oArray);

			ParamType eParamType = ptValue;
			RulesType eRuler = rtEnd; 
			LONG lValue;
			bool bRes = true;

			for (int nIndex = 0; nIndex < oArray.GetSize(); ++nIndex)
			{
				lValue = GetValue(oArray[nIndex], eParamType, bRes);
				if (bRes)
				{	
					switch (eParamType)
					{
					case ptFormula: { lValue = pManager.m_arResults[lValue]; break; }
					case ptAdjust:  { lValue = (*(pManager.m_pAdjustments))[lValue]; break; }
					default: break;
					};
					if (0 != m_arSlices.GetSize())
					{
						m_arSlices[m_arSlices.GetSize() - 1].AddParam(lValue);
					}
				}
				else
				{
					eRuler = GetRuler(oArray[nIndex], bRes);
					if (bRes)
					{
						if (rtNoFill == eRuler)
						{
							m_bFill = false;
						}
						else if (rtNoStroke == eRuler)
						{
							m_bStroke = false;
						}
						else
						{				
							CSlice oSlice(eRuler);
							m_arSlices.Add(oSlice);
						}
					}
				}
			}
		}
	#endif
	#if defined(ODP_DEF)
		void FromXML(CString strPath , NSGuidesOdp::CFormulaManager& pManager)
		{
			NSStringUtils::CheckPathOn_Fill_Stroke(strPath, m_bFill, m_bStroke);
			CSimpleArray<CString> oArray;
			//NSStringUtils::ParsePath(strPath, &oArray);
			NSStringUtils::ParseString(_T(" "), strPath, &oArray);

			RulesType eRuler = rtEnd; 
			LONG lValue;
			bool bRes = true;

			for (int nIndex = 0; nIndex < oArray.GetSize(); ++nIndex)
			{
				eRuler = GetRuler(oArray[nIndex], bRes);
				if (bRes)
				{
					if (rtNoFill == eRuler)
					{
						m_bFill = false;
					}
					else if (rtNoStroke == eRuler)
					{
						m_bStroke = false;
					}
					else
					{				
						CSlice oSlice(eRuler);
						m_arSlices.Add(oSlice);
					}
				}
				else
				{
					lValue = (long)pManager.GetValue(oArray[nIndex]);
					if (0 != m_arSlices.GetSize())
					{
						m_arSlices[m_arSlices.GetSize() - 1].AddParam(lValue);
					}
				}
			}
		}
	#endif

		CString ToXml(CGeomShapeInfo& pGeomInfo, double dStartTime, double dEndTime, CPen& pPen, CBrush& pFore, CMetricInfo& pInfo, NSBaseShape::ClassType ClassType)
		{
			CString strTimeLine = _T("");
			strTimeLine.Format(_T("<timeline type='1' begin='%lf' end='%lf' fadein='0' fadeout='0' completeness='1.0'/>"), dStartTime, dEndTime);

			CString strFill = _T("");
			strFill.Format(_T(" stroke='%s' fill='%s' angle='%lf' flags='%d' bounds-left='%lf' bounds-top='%lf' bounds-right='%lf' bounds-bottom='%lf' widthmm='%d' heightmm='%d'>"),
				NSPresentationEditor::BoolToString(m_bStroke), NSPresentationEditor::BoolToString(m_bFill), 
				pGeomInfo.m_dRotate, pGeomInfo.GetFlags(), 
				pGeomInfo.m_dLeft, pGeomInfo.m_dTop, pGeomInfo.m_dLeft + pGeomInfo.m_dWidth, pGeomInfo.m_dTop + pGeomInfo.m_dHeight,
				pInfo.m_lMillimetresHor, pInfo.m_lMillimetresVer);

			strFill += pPen.ToString();
			if (m_bFill)
				strFill += pFore.ToString();

			CString strResult = _T("<ImagePaint-DrawGraphicPath") + strFill;
			for (int nIndex = 0; nIndex < m_arSlices.GetSize(); ++nIndex)
			{
				strResult += m_arSlices[nIndex].ToXml(pGeomInfo, width, height, ClassType);
			}
			strResult += strTimeLine;
			strResult += _T("</ImagePaint-DrawGraphicPath>");

			return strResult;
		}

		void ToRenderer(CGraphicPath* pRenderer, CGeomShapeInfo& pGeomInfo, double dStartTime, double dEndTime, CPen& pPen, CBrush& pFore, CMetricInfo& pInfo, NSBaseShape::ClassType ClassType)
		{
			pRenderer->m_bStroke	= m_bStroke;
			pRenderer->m_bFill		= m_bFill;

			pRenderer->Pen			= pPen;
			pRenderer->Brush		= pFore;
			
			pRenderer->m_dWidthMM	= pInfo.m_lMillimetresHor;
			pRenderer->m_dHeightMM	= pInfo.m_lMillimetresVer;

			//pRenderer->SetCommandParams(pGeomInfo.m_dRotate, pGeomInfo.m_dLeft, pGeomInfo.m_dTop, 
			//	pGeomInfo.m_dWidth, pGeomInfo.m_dHeight, pGeomInfo.GetFlags());

			//pRenderer->BeginCommand(c_nPathType);

			int nSlises = m_arSlices.GetSize();
			for (int nIndex = 0; nIndex < nSlises; ++nIndex)
			{
				m_arSlices[nIndex].ToRenderer(pRenderer, pGeomInfo, width, height, ClassType);
			}
			
			//pRenderer->PathCommandEnd();
			//pRenderer->EndCommand(c_nPathType);
			//pRenderer->SetCommandParams(0, -1, -1, -1, -1, 0);
		}

		void ToRendererOOX(CGraphicPath* pRenderer, CGeomShapeInfo& pGeomInfo, CMetricInfo& pInfo, NSBaseShape::ClassType ClassType)
		{
			pRenderer->m_bStroke	= m_bStroke;
			pRenderer->m_bFill		= m_bFill;

			pRenderer->m_dWidthMM	= pInfo.m_lMillimetresHor;
			pRenderer->m_dHeightMM	= pInfo.m_lMillimetresVer;

			int nSlises = m_arSlices.GetSize();
			for (int nIndex = 0; nIndex < nSlises; ++nIndex)
			{
				m_arSlices[nIndex].ToRenderer(pRenderer, pGeomInfo, width, height, ClassType);
			}
		}

		CPartPath& operator =(const CPartPath& oSrc)
		{
			m_bFill = oSrc.m_bFill;
			m_bStroke = oSrc.m_bStroke;

			width = oSrc.width;
			height = oSrc.height;

			m_arSlices.RemoveAll();
			for (int nIndex = 0; nIndex < oSrc.m_arSlices.GetSize(); ++nIndex)
			{
				m_arSlices.Add(oSrc.m_arSlices[nIndex]);
			}
			return (*this);
		}
	};

	class CPath
	{
	public:
		CSimpleArray<CPartPath> m_arParts;
	public:
	#if defined(PPTX_DEF)
		void FromXML(XmlUtils::CXmlNodes& list, NSGuidesOOXML::CFormulaManager& pManager)
		{
			m_arParts.RemoveAll();
			for(long i = 0; i < list.GetCount(); i++)
			{
				XmlUtils::CXmlNode path;
				list.GetAt(i, path);
				CPartPath part;
				part.FromXML(path, pManager);
				m_arParts.Add(part);
			}
		}
	#endif
	#if defined(PPT_DEF)
		void FromXML(CString strPath, NSGuidesVML::CFormulasManager& pManager)
		{
			m_arParts.RemoveAll();
			CSimpleArray<CString> oArray;
			NSStringUtils::ParseString(_T("e"), strPath, &oArray);

			for (int nIndex = 0; nIndex < oArray.GetSize(); ++nIndex)
			{
				CPartPath oPath;
				m_arParts.Add(oPath);
				m_arParts[m_arParts.GetSize() - 1].FromXML(oArray[nIndex], pManager);
			}
		}
	#endif
	#if defined(ODP_DEF)
		void FromXML(CString strPath, NSGuidesOdp::CFormulaManager& pManager)
		{
			m_arParts.RemoveAll();
			CSimpleArray<CString> oArray;
			NSStringUtils::ParseString(_T("N"), strPath, &oArray);

			for (int nIndex = 0; nIndex < oArray.GetSize(); ++nIndex)
			{
				CPartPath oPath;
				m_arParts.Add(oPath);
				m_arParts[m_arParts.GetSize() - 1].FromXML(oArray[nIndex], pManager);
			}
		}
	#endif

		CString ToXml(CGeomShapeInfo& pGeomInfo, double dStartTime, double dEndTime, CPen& pPen, CBrush& pFore, CMetricInfo& pInfo, NSBaseShape::ClassType ClassType)
		{
			CString strResult = _T("");
			for (int nIndex = 0; nIndex < m_arParts.GetSize(); ++nIndex)
			{
				strResult += m_arParts[nIndex].ToXml(pGeomInfo, dStartTime, dEndTime, pPen, pFore, pInfo, ClassType);
			}
			return strResult;
		}

		void ToRenderer(IRenderer* pRenderer, CGeomShapeInfo& pGeomInfo, double dStartTime, 
			double dEndTime, CPen& pPen, CBrush& pFore, CMetricInfo& pInfo, NSBaseShape::ClassType ClassType)
		{
			CGraphicPath oPath;
			oPath.Pen	= pPen;
			oPath.Brush	= pFore;

			int nSize = m_arParts.GetSize();
			for (int nIndex = 0; nIndex < nSize; ++nIndex)
			{
				oPath.Clear();
				m_arParts[nIndex].ToRenderer(&oPath, pGeomInfo, dStartTime, dEndTime, pPen, pFore, pInfo, ClassType);

				oPath.Draw(pRenderer);
			}
		}

		CPath& operator =(const CPath& oSrc)
		{
			m_arParts.RemoveAll();
			for (int nIndex = 0; nIndex < oSrc.m_arParts.GetSize(); ++nIndex)
			{
				m_arParts.Add(oSrc.m_arParts[nIndex]);
			}
			return (*this);
		}

		void SetCoordsize(LONG lWidth, LONG lHeight)
		{
			for (int nIndex = 0; nIndex < m_arParts.GetSize(); ++nIndex)
			{
				m_arParts[nIndex].width		= lWidth;
				m_arParts[nIndex].height	= lHeight;
			}
		}
	};
}