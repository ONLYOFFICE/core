#include "stdafx.h"
#include "Page.h"
#include <stdio.h>
#include "../../../Common/OfficeDrawing/File.h"
#include "XPSConst.h"


namespace XPS
{
	const long cbsSolid = 1000;		//const brush style

	const double cdk = 25.4 / 96;	//const double koef
//#define xpsUnitToMM(x) (x)
#define xpsUnitToMM(x) ((x)*cdk)

	Page::Page(const CString& File, const CString& Path, NSCommon::smart_ptr<CAtlMap<CString, bool>> fontMap, CRITICAL_SECTION* fontCS)
		:	m_strFilename(File)
		,	m_strPath(Path)
		,	m_fontMap(fontMap)
	{
		m_pFontCS = fontCS;
	}


	Page::~Page()
	{
	}


	void Page::GetDataFromNode(CString& str, XmlUtils::CXmlNode& node)
	{
		str = g_cstrEmpty;
		XmlUtils::CXmlNode data;
		node.GetNode(_T("Path.Data"), data);

		XmlUtils::CXmlNode geometry;
		data.GetNode(_T("PathGeometry"), geometry);
		CString FillMode = geometry.GetAttribute(_T("FillRule"), _T("EvenOdd"));
		if(FillMode == _T("EvenOdd"))
			str += _T("F 0 ");
		else
			str += _T("F 1 ");

		XmlUtils::CXmlNodes figures;
		XmlUtils::CXmlNode	figure;
		XmlUtils::CXmlNodes segments;
		XmlUtils::CXmlNode	segment;
		geometry.GetNodes(_T("PathFigure"), figures);
		for(int i = 0; i < figures.GetCount(); i++)
		{
			figures.GetAt(i, figure);
			CString StartPoint = figure.GetAttribute(_T("StartPoint"));
			str += _T(" M ");
			str += StartPoint;

			figure.GetNodes(_T("*"), segments);
			for(int j = 0; j < segments.GetCount(); j++)
			{
				segments.GetAt(j, segment);
				CString name = segment.GetName();
				if(name == _T("PolyLineSegment"))
				{
					str += _T(" L ");
					str += segment.GetAttribute(_T("Points"));
				}
				else if(name == _T("PolyBezierSegment"))
				{
					str += _T(" C ");
					str += segment.GetAttribute(_T("Points"));
				}
				else if(name == _T("PolyQuadraticBezierSegment"))
				{
					str += _T(" Q ");
					str += segment.GetAttribute(_T("Points"));
				}
				else if(name == _T("ArcSegment"))
				{
					str += _T(" A ");
					str += segment.GetAttribute(_T("Size"));
					str += _T(" ");
					str += segment.GetAttribute(_T("RotationAngle"));
					str += _T(" ");
					if(GetBool(segment.GetAttribute(_T("IsLargeArc"))))
						str += _T("1 ");
					else
						str += _T("0 ");
					if(segment.GetAttribute(_T("SweepDirection")) == _T("Counterclockwise"))
						str += _T("0 ");
					else
						str += _T("1 ");
					str += segment.GetAttribute(_T("Point"));
				}
			}
			bool close = GetBool(figure.GetAttribute(_T("IsClosed")));
			if(close)
				str += _T(" Z ");
		}
	}


	bool Page::VmlToRenderer(CString& str, ASCGraphics::IASCRenderer* pRenderer)
	{
		bool result = false;
		double x_cur = 0.0, y_cur = 0.0;
		double x_cp = 0.0, y_cp = 0.0;
		bool l_bPrevCommandIsC = false;

		std::vector<CString> dest;
		PrepareVML(str);
		DelimString(str, dest, _T(" ,"));
		int elems = (int)dest.size();
		for(int i = 0; i < elems; i++)
		{
			if(dest[i] == _T("F"))
			{
				result = GetBool(dest[i + 1]);
				l_bPrevCommandIsC = false;
			}
			if(dest[i] == _T("M"))
			{
				x_cur = GetDouble(dest[i + 1]);
				y_cur = GetDouble(dest[i + 2]);
				pRenderer->PathCommandMoveTo(xpsUnitToMM(x_cur), xpsUnitToMM(y_cur));
				i += 2;
				l_bPrevCommandIsC = false;
			}
			else if(dest[i] == _T("L"))
			{
				x_cur = GetDouble(dest[i + 1]);
				y_cur = GetDouble(dest[i + 2]);
				pRenderer->PathCommandLineTo(xpsUnitToMM(x_cur), xpsUnitToMM(y_cur));
				i += 2;
				if(i + 2 < elems)
				{
					while((i + 2 < elems) && (!IsAlpha(dest[i + 1][0])))
					{
						x_cur = GetDouble(dest[i + 1]);
						y_cur = GetDouble(dest[i + 2]);
						pRenderer->PathCommandLineTo(xpsUnitToMM(x_cur), xpsUnitToMM(y_cur));
						i += 2;
					}
				}
				l_bPrevCommandIsC = false;
			}
			else if(dest[i] == _T("A"))
			{
				Aggplus::CMatrix tmatx, rmatx, smatx, itmatx, irmatx, ismatx;
				double x_cur_temp = x_cur, y_cur_temp = y_cur;
				double x_end = GetDouble(dest[i + 6]), y_end = GetDouble(dest[i + 7]);

				tmatx.Translate(-x_cur, -y_cur);
				tmatx.TransformPoint(x_cur_temp, y_cur_temp);
				tmatx.TransformPoint(x_end, y_end);

				double angle = GetDouble(dest[i + 3]);
				rmatx.Rotate(-angle);
				rmatx.TransformPoint(x_cur_temp, y_cur_temp);
				rmatx.TransformPoint(x_end, y_end);

				double rx = GetDouble(dest[i + 1]), ry = GetDouble(dest[i + 2]);
				smatx.Scale(ry / rx, 1);
				smatx.TransformPoint(x_cur_temp, y_cur_temp);
				smatx.TransformPoint(x_end, y_end);

				double mid_x = (x_cur_temp + x_end) / 2.0, mid_y = (y_cur_temp + y_end) / 2.0;
				double vect_x = x_end - x_cur_temp, vect_y = y_end - y_cur_temp;
				double length = sqrt(vect_x * vect_x + vect_y * vect_y);
				double halfChord = length / 2.0;
				bool isLarge	= GetBool(dest[i + 4]);
				bool isCCW		= !GetBool(dest[i + 5]);
				double rotated_x, rotated_y;

				if (isLarge == isCCW)
				{
					rotated_x = -vect_y;
					rotated_y = vect_x;
				}
				else
				{
					rotated_x = vect_y;
					rotated_y = -vect_x;
				}

				rotated_x = rotated_x / length;
				rotated_y = rotated_y / length;

				double centerDistance = sqrt(max(ry * ry - halfChord * halfChord, 0));
				double center_x = mid_x + centerDistance * rotated_x, center_y = mid_y + centerDistance * rotated_y;
			    
				double angle1 = atan2(y_cur_temp - center_y, x_cur_temp - center_x);
				double angle2 = atan2(y_end - center_y, x_end - center_x);

				if((!isCCW) && (abs(y_end - center_y) < 0.000001) && (x_end - center_x < 0))
					angle2 = - M_PI;
				if((isCCW) && (abs(y_cur_temp - center_y) < 0.000001) && (x_cur_temp - center_x < 0))
					angle1 = - M_PI;

				if (isLarge == (abs(angle2 - angle1) < M_PI ))
				{
					if (angle1 < angle2)
						angle1 += 2 * M_PI;
					else
						angle2 += 2 * M_PI;
				}
				if (abs(angle2 - angle1) == M_PI)
				{
					if ((angle1 < angle2) && (isCCW) && (angle1 != - M_PI) && (angle1 != 0.0) && (angle1 != M_PI))
						angle1 += 2 * M_PI;
				//	else if ((angle1 < angle2) && (isCCW))
				//		angle2 += 2 * M_PI;
				}

				//matx.Invert();
				//Aggplus::CMatrix imatx;
				itmatx.Translate(x_cur, y_cur);
				irmatx.Rotate(angle);
				ismatx.Scale(rx / ry, 1);

				int max = (int)((4 * (rx + ry) * abs(angle2 - angle1) / (2 * M_PI)) / 1/*tolerance*/);

				double x,y;
				for (int counter = 0; counter <= max; counter++)
				{
					double angle_cur = ((max - counter) * angle1 + counter * angle2) / max;
					x = center_x + ry * cos(angle_cur);
					y = center_y + ry * sin(angle_cur);

					// Transform the point back
					ismatx.TransformPoint(x, y);
					irmatx.TransformPoint(x, y);
					itmatx.TransformPoint(x, y);
					//points.Add(pt);
					pRenderer->PathCommandLineTo(xpsUnitToMM(x), xpsUnitToMM(y));
					//x_cur = x;
					//y_cur = y;
				}
				x_cur = x;
				y_cur = y;
				i += 7;

				if(i + 7 < elems)
				{
					while((i + 7 < elems) && (!IsAlpha(dest[i + 1][0])))
					{
						Aggplus::CMatrix tmatx, rmatx, smatx, itmatx, irmatx, ismatx;
						double x_cur_temp = x_cur, y_cur_temp = y_cur;
						double x_end = GetDouble(dest[i + 6]), y_end = GetDouble(dest[i + 7]);

						tmatx.Translate(-x_cur, -y_cur);
						tmatx.TransformPoint(x_cur_temp, y_cur_temp);
						tmatx.TransformPoint(x_end, y_end);

						double angle = GetDouble(dest[i + 3]);
						rmatx.Rotate(-angle);
						rmatx.TransformPoint(x_cur_temp, y_cur_temp);
						rmatx.TransformPoint(x_end, y_end);

						double rx = GetDouble(dest[i + 1]), ry = GetDouble(dest[i + 2]);
						smatx.Scale(ry / rx, 1);
						smatx.TransformPoint(x_cur_temp, y_cur_temp);
						smatx.TransformPoint(x_end, y_end);

						double mid_x = (x_cur_temp + x_end) / 2.0, mid_y = (y_cur_temp + y_end) / 2.0;
						double vect_x = x_end - x_cur_temp, vect_y = y_end - y_cur_temp;
						double length = sqrt(vect_x * vect_x + vect_y * vect_y);
						double halfChord = length / 2.0;
						bool isLarge	= GetBool(dest[i + 4]);
						bool isCCW		= !GetBool(dest[i + 5]);
						double rotated_x, rotated_y;

						if (isLarge == isCCW)
						{
							rotated_x = -vect_y;
							rotated_y = vect_x;
						}
						else
						{
							rotated_x = vect_y;
							rotated_y = -vect_x;
						}

						rotated_x = rotated_x / length;
						rotated_y = rotated_y / length;

						double centerDistance = sqrt(max(ry * ry - halfChord * halfChord, 0));
						double center_x = mid_x + centerDistance * rotated_x, center_y = mid_y + centerDistance * rotated_y;
					    
						double angle1 = atan2(y_cur_temp - center_y, x_cur_temp - center_x);
						double angle2 = atan2(y_end - center_y, x_end - center_x);

						if((!isCCW) && (abs(y_end - center_y) < 0.000001) && (x_end - center_x < 0))
							angle2 = - M_PI;
						if((isCCW) && (abs(y_cur_temp - center_y) < 0.000001) && (x_cur_temp - center_x < 0))
							angle1 = - M_PI;

						if (isLarge == (abs(angle2 - angle1) < M_PI ))
						{
							if (angle1 < angle2)
								angle1 += 2 * M_PI;
							else
								angle2 += 2 * M_PI;
						}
						if (abs(angle2 - angle1) == M_PI)
						{
							if ((angle1 < angle2) && (isCCW) && (angle1 != - M_PI) && (angle1 != 0.0) && (angle1 != M_PI))
								angle1 += 2 * M_PI;
						//	else
						//		angle2 += 2 * M_PI;
						}

						//matx.Invert();
						//Aggplus::CMatrix imatx;
						itmatx.Translate(x_cur, y_cur);
						irmatx.Rotate(angle);
						ismatx.Scale(rx / ry, 1);

						int max = (int)((4 * (rx + ry) * abs(angle2 - angle1) / (2 * M_PI)) / 1/*tolerance*/);

						double x,y;
						for (int counter = 0; counter <= max; counter++)
						{
							double angle_cur = ((max - counter) * angle1 + counter * angle2) / max;
							x = center_x + ry * cos(angle_cur);
							y = center_y + ry * sin(angle_cur);

							// Transform the point back
							ismatx.TransformPoint(x, y);
							irmatx.TransformPoint(x, y);
							itmatx.TransformPoint(x, y);
							//points.Add(pt);
							pRenderer->PathCommandLineTo(xpsUnitToMM(x), xpsUnitToMM(y));
							//x_cur = x;
							//y_cur = y;
						}
						x_cur = x;
						y_cur = y;
						i += 7;
					}
				}

				l_bPrevCommandIsC = false;
			}
			else if(dest[i] == _T("H"))
			{
				//double x, y;
				//pRenderer->PathCommandGetCurrentPoint(&x, &y);
				x_cur = GetDouble(dest[i + 1]);
				pRenderer->PathCommandLineTo(xpsUnitToMM(x_cur), xpsUnitToMM(y_cur));
				i += 1;
				l_bPrevCommandIsC = false;
			}
			else if(dest[i] == _T("V"))
			{
				//double x, y;
				//pRenderer->PathCommandGetCurrentPoint(&x, &y);
				y_cur = GetDouble(dest[i + 1]);
				pRenderer->PathCommandLineTo(xpsUnitToMM(x_cur), xpsUnitToMM(y_cur));
				i += 1;
				l_bPrevCommandIsC = false;
			}
			else if(dest[i] == _T("C"))
			{
				x_cp = GetDouble(dest[i + 3]);
				y_cp = GetDouble(dest[i + 4]);
				x_cur = GetDouble(dest[i + 5]);
				y_cur = GetDouble(dest[i + 6]);

				pRenderer->PathCommandCurveTo(xpsUnitToMM(GetDouble(dest[i + 1])), xpsUnitToMM(GetDouble(dest[i + 2])),
					xpsUnitToMM(x_cp), xpsUnitToMM(y_cp), xpsUnitToMM(x_cur), xpsUnitToMM(y_cur));

				i += 6;
				if(i + 6 < elems)
				{
					while((i + 6 < elems) && (!IsAlpha(dest[i + 1][0])))
					{
						x_cp = GetDouble(dest[i + 3]);
						y_cp = GetDouble(dest[i + 4]);
						x_cur = GetDouble(dest[i + 5]);
						y_cur = GetDouble(dest[i + 6]);

						pRenderer->PathCommandCurveTo(xpsUnitToMM(GetDouble(dest[i + 1])), xpsUnitToMM(GetDouble(dest[i + 2])),
							xpsUnitToMM(x_cp), xpsUnitToMM(y_cp), xpsUnitToMM(x_cur), xpsUnitToMM(y_cur));

						i += 6;
					}
				}
				l_bPrevCommandIsC = true;
			}
			else if(dest[i] == _T("Q"))
			{
				//double x = 0, y = 0;
				//pRenderer->PathCommandGetCurrentPoint(&x, &y);
				double x1 = 2.0 * GetDouble(dest[i + 1]);
				double y1 = 2.0 * GetDouble(dest[i + 2]);
				double x2 = GetDouble(dest[i + 3]);
				double y2 = GetDouble(dest[i + 4]);

				pRenderer->PathCommandCurveTo(	xpsUnitToMM((x_cur + x1) / 3.0), xpsUnitToMM((y_cur + y1) / 3.0),
												xpsUnitToMM((x1 + x2) / 3.0), xpsUnitToMM((y1 + y2) / 3.0),
												xpsUnitToMM(x2), xpsUnitToMM(y2));
				x_cur = x2;
				y_cur = y2;
				i += 4;
				if(i + 4 < elems)
				{
					while((i + 4 < elems) && (!IsAlpha(dest[i + 1][0])))
					{
						x1 = 2.0 * GetDouble(dest[i + 1]);
						y1 = 2.0 * GetDouble(dest[i + 2]);
						x2 = GetDouble(dest[i + 3]);
						y2 = GetDouble(dest[i + 4]);

						pRenderer->PathCommandCurveTo(	xpsUnitToMM((x_cur + x1) / 3.0), xpsUnitToMM((y_cur + y1) / 3.0),
														xpsUnitToMM((x1 + x2) / 3.0), xpsUnitToMM((y1 + y2) / 3.0),
														xpsUnitToMM(x2), xpsUnitToMM(y2));
						x_cur = x2;
						y_cur = y2;

						i += 4;
					}
				}

				l_bPrevCommandIsC = false;
			}
			else if(dest[i] == _T("S"))
			{
				if((!l_bPrevCommandIsC) || (i == 0))
				{
					x_cp = x_cur;
					y_cp = y_cur;
				}
				else
				{
					x_cp = 2 * x_cur - x_cp;
					y_cp = 2 * y_cur - y_cp;
				}

				x_cur = GetDouble(dest[i + 3]);
				y_cur = GetDouble(dest[i + 4]);
				pRenderer->PathCommandCurveTo(xpsUnitToMM(x_cp), xpsUnitToMM(y_cp),
					xpsUnitToMM(GetDouble(dest[i + 1])), xpsUnitToMM(GetDouble(dest[i + 2])),
					xpsUnitToMM(x_cur), xpsUnitToMM(y_cur));
				x_cp = GetDouble(dest[i + 1]);
				y_cp = GetDouble(dest[i + 2]);
				i += 4;

				if(i + 4 < elems)
				{
					while((i + 4 < elems) && (!IsAlpha(dest[i + 1][0])))
					{
						x_cp = 2 * x_cur - x_cp;
						y_cp = 2 * y_cur - y_cp;

						x_cur = GetDouble(dest[i + 3]);
						y_cur = GetDouble(dest[i + 4]);

						pRenderer->PathCommandCurveTo(xpsUnitToMM(x_cp), xpsUnitToMM(y_cp),
							xpsUnitToMM(GetDouble(dest[i + 1])), xpsUnitToMM(GetDouble(dest[i + 2])),
							xpsUnitToMM(x_cur), xpsUnitToMM(y_cur));

						x_cp = GetDouble(dest[i + 1]);
						y_cp = GetDouble(dest[i + 2]);

						i += 4;
					}
				}
				l_bPrevCommandIsC = true;
			}
			else if(dest[i] == _T("Z"))
			{
				pRenderer->PathCommandClose();
				l_bPrevCommandIsC = false;
			}
			else if(dest[i] == _T("m"))
			{
				//double x, y;
				//pRenderer->PathCommandGetCurrentPoint(&x, &y);
				x_cur += GetDouble(dest[i + 1]);
				y_cur += GetDouble(dest[i + 2]);
				pRenderer->PathCommandMoveTo(xpsUnitToMM(x_cur), xpsUnitToMM(y_cur));
				i += 2;
				l_bPrevCommandIsC = false;
			}
			else if(dest[i] == _T("l"))
			{
				//double x, y;
				//pRenderer->PathCommandGetCurrentPoint(&x, &y);
				x_cur += GetDouble(dest[i + 1]);
				y_cur += GetDouble(dest[i + 2]);
				pRenderer->PathCommandLineTo(xpsUnitToMM(x_cur), xpsUnitToMM(y_cur));
				i += 2;
				if(i + 2 < elems)
				{
					while((i + 2 < elems) && (!IsAlpha(dest[i + 1][0])))
					{
						x_cur += GetDouble(dest[i + 1]);
						y_cur += GetDouble(dest[i + 2]);
						pRenderer->PathCommandLineTo(xpsUnitToMM(x_cur), xpsUnitToMM(y_cur));
						i += 2;
					}
				}
				l_bPrevCommandIsC = false;
			}
			else if(dest[i] == _T("a"))
			{
				Aggplus::CMatrix tmatx, rmatx, smatx, itmatx, irmatx, ismatx;
				double x_cur_temp = x_cur, y_cur_temp = y_cur;
				double x_end = GetDouble(dest[i + 6]) + x_cur, y_end = GetDouble(dest[i + 7]) + y_cur;

				tmatx.Translate(-x_cur, -y_cur);
				tmatx.TransformPoint(x_cur_temp, y_cur_temp);
				tmatx.TransformPoint(x_end, y_end);

				double angle = GetDouble(dest[i + 3]);
				rmatx.Rotate(-angle);
				rmatx.TransformPoint(x_cur_temp, y_cur_temp);
				rmatx.TransformPoint(x_end, y_end);

				double rx = GetDouble(dest[i + 1]), ry = GetDouble(dest[i + 2]);
				smatx.Scale(ry / rx, 1);
				smatx.TransformPoint(x_cur_temp, y_cur_temp);
				smatx.TransformPoint(x_end, y_end);

				double mid_x = (x_cur_temp + x_end) / 2.0, mid_y = (y_cur_temp + y_end) / 2.0;
				double vect_x = x_end - x_cur_temp, vect_y = y_end - y_cur_temp;
				double length = sqrt(vect_x * vect_x + vect_y * vect_y);
				double halfChord = length / 2.0;
				bool isLarge	= GetBool(dest[i + 4]);
				bool isCCW		= !GetBool(dest[i + 5]);
				double rotated_x, rotated_y;

				if (isLarge == isCCW)
				{
					rotated_x = -vect_y;
					rotated_y = vect_x;
				}
				else
				{
					rotated_x = vect_y;
					rotated_y = -vect_x;
				}

				rotated_x = rotated_x / length;
				rotated_y = rotated_y / length;

				double centerDistance = sqrt(max(ry * ry - halfChord * halfChord, 0));
				double center_x = mid_x + centerDistance * rotated_x, center_y = mid_y + centerDistance * rotated_y;
			    
				double angle1 = atan2(y_cur_temp - center_y, x_cur_temp - center_x);
				double angle2 = atan2(y_end - center_y, x_end - center_x);

				if((!isCCW) && (abs(y_end - center_y) < 0.000001) && (x_end - center_x < 0))
					angle2 = - M_PI;
				if((isCCW) && (abs(y_cur_temp - center_y) < 0.000001) && (x_cur_temp - center_x < 0))
					angle1 = - M_PI;

				if (isLarge == (abs(angle2 - angle1) < M_PI ))
				{
					if (angle1 < angle2)
						angle1 += 2 * M_PI;
					else
						angle2 += 2 * M_PI;
				}
				if (abs(angle2 - angle1) == M_PI)
				{
					if ((angle1 < angle2) && (isCCW) && (angle1 != - M_PI) && (angle1 != 0.0) && (angle1 != M_PI))
						angle1 += 2 * M_PI;
				//	else
				//		angle2 += 2 * M_PI;
				}

				//matx.Invert();
				//Aggplus::CMatrix imatx;
				itmatx.Translate(x_cur, y_cur);
				irmatx.Rotate(angle);
				ismatx.Scale(rx / ry, 1);

				int max = (int)((4 * (rx + ry) * abs(angle2 - angle1) / (2 * M_PI)) / 1/*tolerance*/);

				double x,y;
				for (int counter = 0; counter <= max; counter++)
				{
					double angle_cur = ((max - counter) * angle1 + counter * angle2) / max;
					x = center_x + ry * cos(angle_cur);
					y = center_y + ry * sin(angle_cur);

					// Transform the point back
					ismatx.TransformPoint(x, y);
					irmatx.TransformPoint(x, y);
					itmatx.TransformPoint(x, y);
					//points.Add(pt);
					pRenderer->PathCommandLineTo(xpsUnitToMM(x), xpsUnitToMM(y));
					//x_cur = x;
					//y_cur = y;
				}
				x_cur = x;
				y_cur = y;
				i += 7;

				if(i + 7 < elems)
				{
					while((i + 7 < elems) && (!IsAlpha(dest[i + 1][0])))
					{
						Aggplus::CMatrix tmatx, rmatx, smatx, itmatx, irmatx, ismatx;
						double x_cur_temp = x_cur, y_cur_temp = y_cur;
						double x_end = GetDouble(dest[i + 6]) + x_cur, y_end = GetDouble(dest[i + 7]) + y_cur;

						tmatx.Translate(-x_cur, -y_cur);
						tmatx.TransformPoint(x_cur_temp, y_cur_temp);
						tmatx.TransformPoint(x_end, y_end);

						double angle = GetDouble(dest[i + 3]);
						rmatx.Rotate(-angle);
						rmatx.TransformPoint(x_cur_temp, y_cur_temp);
						rmatx.TransformPoint(x_end, y_end);

						double rx = GetDouble(dest[i + 1]), ry = GetDouble(dest[i + 2]);
						smatx.Scale(ry / rx, 1);
						smatx.TransformPoint(x_cur_temp, y_cur_temp);
						smatx.TransformPoint(x_end, y_end);

						double mid_x = (x_cur_temp + x_end) / 2.0, mid_y = (y_cur_temp + y_end) / 2.0;
						double vect_x = x_end - x_cur_temp, vect_y = y_end - y_cur_temp;
						double length = sqrt(vect_x * vect_x + vect_y * vect_y);
						double halfChord = length / 2.0;
						bool isLarge	= GetBool(dest[i + 4]);
						bool isCCW		= !GetBool(dest[i + 5]);
						double rotated_x, rotated_y;

						if (isLarge == isCCW)
						{
							rotated_x = -vect_y;
							rotated_y = vect_x;
						}
						else
						{
							rotated_x = vect_y;
							rotated_y = -vect_x;
						}

						rotated_x = rotated_x / length;
						rotated_y = rotated_y / length;

						double centerDistance = sqrt(max(ry * ry - halfChord * halfChord, 0));
						double center_x = mid_x + centerDistance * rotated_x, center_y = mid_y + centerDistance * rotated_y;
					    
						double angle1 = atan2(y_cur_temp - center_y, x_cur_temp - center_x);
						double angle2 = atan2(y_end - center_y, x_end - center_x);

						if((!isCCW) && (abs(y_end - center_y) < 0.000001) && (x_end - center_x < 0))
							angle2 = - M_PI;
						if((isCCW) && (abs(y_cur_temp - center_y) < 0.000001) && (x_cur_temp - center_x < 0))
							angle1 = - M_PI;

						if (isLarge == (abs(angle2 - angle1) < M_PI ))
						{
							if (angle1 < angle2)
								angle1 += 2 * M_PI;
							else
								angle2 += 2 * M_PI;
						}
						if (abs(angle2 - angle1) == M_PI)
						{
							if ((angle1 < angle2) && (isCCW) && (angle1 != - M_PI) && (angle1 != 0.0) && (angle1 != M_PI))
								angle1 += 2 * M_PI;
						//	else
						//		angle2 += 2 * M_PI;
						}

						//matx.Invert();
						//Aggplus::CMatrix imatx;
						itmatx.Translate(x_cur, y_cur);
						irmatx.Rotate(angle);
						ismatx.Scale(rx / ry, 1);

						int max = (int)((4 * (rx + ry) * abs(angle2 - angle1) / (2 * M_PI)) / 1/*tolerance*/);

						double x,y;
						for (int counter = 0; counter <= max; counter++)
						{
							double angle_cur = ((max - counter) * angle1 + counter * angle2) / max;
							x = center_x + ry * cos(angle_cur);
							y = center_y + ry * sin(angle_cur);

							// Transform the point back
							ismatx.TransformPoint(x, y);
							irmatx.TransformPoint(x, y);
							itmatx.TransformPoint(x, y);
							//points.Add(pt);
							pRenderer->PathCommandLineTo(xpsUnitToMM(x), xpsUnitToMM(y));
							//x_cur = x;
							//y_cur = y;
						}
						x_cur = x;
						y_cur = y;
						i += 7;
					}
				}

				l_bPrevCommandIsC = false;
			}
			else if(dest[i] == _T("h"))
			{
				//double x, y;
				//pRenderer->PathCommandGetCurrentPoint(&x, &y);
				x_cur += GetDouble(dest[i + 1]);
				pRenderer->PathCommandLineTo(xpsUnitToMM(x_cur), xpsUnitToMM(y_cur));
				i += 1;
				l_bPrevCommandIsC = false;
			}
			else if(dest[i] == _T("v"))
			{
				//double x, y;
				//pRenderer->PathCommandGetCurrentPoint(&x, &y);
				y_cur += GetDouble(dest[i + 1]);
				pRenderer->PathCommandLineTo(xpsUnitToMM(x_cur), xpsUnitToMM(y_cur));
				i += 1;
				l_bPrevCommandIsC = false;
			}
			else if(dest[i] == _T("c"))
			{
				x_cp = x_cur + GetDouble(dest[i + 3]);
				y_cp = y_cur + GetDouble(dest[i + 4]);

				pRenderer->PathCommandCurveTo(xpsUnitToMM(x_cur + GetDouble(dest[i + 1])), xpsUnitToMM(y_cur + GetDouble(dest[i + 2])),
					xpsUnitToMM(x_cp), xpsUnitToMM(y_cp), xpsUnitToMM(x_cur + GetDouble(dest[i + 5])), xpsUnitToMM(y_cur + GetDouble(dest[i + 6])));
				x_cur += GetDouble(dest[i + 5]);
				y_cur += GetDouble(dest[i + 6]);

				i += 6;
				if(i + 6 < elems)
				{
					while((i + 6 < elems) && (!IsAlpha(dest[i + 1][0])))
					{
						x_cp = x_cur + GetDouble(dest[i + 3]);
						y_cp = y_cur + GetDouble(dest[i + 4]);

						pRenderer->PathCommandCurveTo(xpsUnitToMM(x_cur + GetDouble(dest[i + 1])), xpsUnitToMM(y_cur + GetDouble(dest[i + 2])),
							xpsUnitToMM(x_cp), xpsUnitToMM(y_cp), xpsUnitToMM(x_cur + GetDouble(dest[i + 5])), xpsUnitToMM(y_cur + GetDouble(dest[i + 6])));
						x_cur += GetDouble(dest[i + 5]);
						y_cur += GetDouble(dest[i + 6]);

						i += 6;
					}
				}
				l_bPrevCommandIsC = true;
			}
			else if(dest[i] == _T("q"))
			{
				double x1 = 2.0 * (x_cur + GetDouble(dest[i + 1]));
				double y1 = 2.0 * (y_cur + GetDouble(dest[i + 2]));
				double x2 = x_cur + GetDouble(dest[i + 3]);
				double y2 = y_cur + GetDouble(dest[i + 4]);

				pRenderer->PathCommandCurveTo(	xpsUnitToMM((x_cur + x1) / 3.0), xpsUnitToMM((y_cur + y1) / 3.0),
												xpsUnitToMM((x1 + x2) / 3.0), xpsUnitToMM((y1 + y2) / 3.0),
												xpsUnitToMM(x2), xpsUnitToMM(y2));
				x_cur = x2;
				y_cur = y2;
				i += 4;
				if(i + 4 < elems)
				{
					while((i + 4 < elems) && (!IsAlpha(dest[i + 1][0])))
					{
						x1 = 2.0 * (x_cur + GetDouble(dest[i + 1]));
						y1 = 2.0 * (y_cur + GetDouble(dest[i + 2]));
						x2 = x_cur + GetDouble(dest[i + 3]);
						y2 = y_cur + GetDouble(dest[i + 4]);

						pRenderer->PathCommandCurveTo(	xpsUnitToMM((x_cur + x1) / 3.0), xpsUnitToMM((y_cur + y1) / 3.0),
														xpsUnitToMM((x1 + x2) / 3.0), xpsUnitToMM((y1 + y2) / 3.0),
														xpsUnitToMM(x2), xpsUnitToMM(y2));
						x_cur = x2;
						y_cur = y2;

						i += 4;
					}
				}

				l_bPrevCommandIsC = false;
			}
			else if(dest[i] == _T("s"))
			{
				if((!l_bPrevCommandIsC) || (i == 0))
				{
					x_cp = x_cur;
					y_cp = y_cur;
				}
				else
				{
					x_cp = 2 * x_cur - x_cp;
					y_cp = 2 * y_cur - y_cp;
				}
				pRenderer->PathCommandCurveTo(xpsUnitToMM(x_cp), xpsUnitToMM(y_cp),
					xpsUnitToMM(x_cur + GetDouble(dest[i + 1])), xpsUnitToMM(y_cur + GetDouble(dest[i + 2])),
					xpsUnitToMM(x_cur + GetDouble(dest[i + 3])), xpsUnitToMM(y_cur + GetDouble(dest[i + 4])));
				x_cp = x_cur + GetDouble(dest[i + 1]);
				y_cp = y_cur + GetDouble(dest[i + 2]);
				x_cur = x_cur + GetDouble(dest[i + 3]);
				y_cur = y_cur + GetDouble(dest[i + 4]);
				i += 4;

				if(i + 4 < elems)
				{
					while((i + 4 < elems) && (!IsAlpha(dest[i + 1][0])))
					{
						x_cp = 2 * x_cur - x_cp;
						y_cp = 2 * y_cur - y_cp;

						pRenderer->PathCommandCurveTo(xpsUnitToMM(x_cp), xpsUnitToMM(y_cp),
							xpsUnitToMM(x_cur + GetDouble(dest[i + 1])), xpsUnitToMM(y_cur + GetDouble(dest[i + 2])),
							xpsUnitToMM(x_cur + GetDouble(dest[i + 3])), xpsUnitToMM(y_cur + GetDouble(dest[i + 4])));

						x_cp = x_cur + GetDouble(dest[i + 1]);
						y_cp = y_cur + GetDouble(dest[i + 2]);
						x_cur = x_cur + GetDouble(dest[i + 3]);
						y_cur = y_cur + GetDouble(dest[i + 4]);

						i += 4;
					}
				}
				l_bPrevCommandIsC = true;
			}
			else if(dest[i] == _T("z"))
			{
				pRenderer->PathCommandClose();
				l_bPrevCommandIsC = false;
			}
		}
		//
		//SAFEARRAY* psaData = SafeArrayCreateVector(VT_R8, lLen);
		//double* pData = (double*)psaData->pvData;

		//RELEASEARRAY(psaData);
		//
		return result;
	}


	void Page::TransformToRenderer(CString& str, ASCGraphics::IASCRenderer* pRenderer, DrawPageProperties* pProps)const
	{
		std::vector<CString> dest;
		DelimString(str, dest, _T(','));
		double res[6] = {1.0, 0.0, 0.0, 1.0, 0.0, 0.0};
		for(int i = 0; i < min(6, dest.size()); i++)
			res[i] = GetDouble(dest[i]);
		//pRenderer->SetTransform(res[0], res[1], res[2], res[3], xpsUnitToMM(res[4]), xpsUnitToMM(res[5]));
		Aggplus::CMatrix transform(res[0], res[1], res[2], res[3], res[4], res[5]);
		pProps->m_CurrentTransform.Multiply(&transform);
		pProps->m_TransformStack.push_back(pProps->m_CurrentTransform);
		pRenderer->SetTransform(pProps->m_CurrentTransform.m_agg_mtx.sx, pProps->m_CurrentTransform.m_agg_mtx.shy,
								pProps->m_CurrentTransform.m_agg_mtx.shx, pProps->m_CurrentTransform.m_agg_mtx.sy,
								xpsUnitToMM(pProps->m_CurrentTransform.m_agg_mtx.tx), xpsUnitToMM(pProps->m_CurrentTransform.m_agg_mtx.ty));
	}


	void Page::ResetTransform(ASCGraphics::IASCRenderer* pRenderer, DrawPageProperties* pProps)const
	{
		pProps->m_TransformStack.pop_back();
		pProps->m_CurrentTransform = pProps->m_TransformStack.back();
		pRenderer->SetTransform(pProps->m_CurrentTransform.m_agg_mtx.sx, pProps->m_CurrentTransform.m_agg_mtx.shy,
								pProps->m_CurrentTransform.m_agg_mtx.shx, pProps->m_CurrentTransform.m_agg_mtx.sy,
								xpsUnitToMM(pProps->m_CurrentTransform.m_agg_mtx.tx), xpsUnitToMM(pProps->m_CurrentTransform.m_agg_mtx.ty));
	}


	void Page::GetSize(int& w, int& h)const
	{
		XmlUtils::CXmlNode	lNode;
		lNode.FromXmlFile(m_strFilename);
		lNode.ReadRootNode(_T("FixedPage"));

		w = XmlUtils::GetInteger(lNode.GetAttribute(_T("Width")));
		h = XmlUtils::GetInteger(lNode.GetAttribute(_T("Height")));
	}


	void Page::Draw(ASCGraphics::IASCRenderer* pRenderer, BOOL* pBreak)const
	{
		XmlUtils::CXmlNode	lNode;

		lNode.FromXmlFile(m_strFilename);
		lNode.ReadRootNode(_T("FixedPage"));

		DrawPageProperties props;

		XmlUtils::CXmlNode oNodeResources;
		if (lNode.GetNode(_T("FixedPage.Resources"), oNodeResources))
		{
			XmlUtils::CXmlNode oNodeDictionary;
			if (oNodeResources.GetNode(_T("ResourceDictionary"), oNodeDictionary))
			{
				CString strXmlSource = oNodeDictionary.GetAttribute(_T("Source"));
				if (strXmlSource != _T(""))
				{
					CString strPath = m_strPath + strXmlSource;
					XmlUtils::CXmlNode oNodeSource;
					oNodeSource.FromXmlFile(strPath);

					if (oNodeSource.IsValid())
					{
						CString s = oNodeSource.GetName();

						XmlUtils::CXmlNodes oNodes;
						if (oNodeSource.GetNodes(_T("PathGeometry"), oNodes))
						{
							int nCount = oNodes.GetCount();
							for (int i = 0; i < nCount; ++i)
							{
								XmlUtils::CXmlNode node;
								oNodes.GetAt(i, node);
								CString strKey = node.GetAttribute(_T("x:Key"));
								CString strValue = node.GetAttribute(_T("Figures"));

								strKey = (_T("{StaticResource ") + strKey + _T("}"));
								props.m_mapFigures.SetAt(strKey, strValue);
							}
						}
					}
				}
				else
				{
					XmlUtils::CXmlNodes oNodes;
					if (oNodeDictionary.GetNodes(_T("PathGeometry"), oNodes))
					{
						int nCount = oNodes.GetCount();
						for (int i = 0; i < nCount; ++i)
						{
							XmlUtils::CXmlNode node;
							oNodes.GetAt(i, node);
							CString strKey = node.GetAttribute(_T("x:Key"));
							CString strValue = node.GetAttribute(_T("Figures"));

							strKey = (_T("{StaticResource ") + strKey + _T("}"));
							props.m_mapFigures.SetAt(strKey, strValue);
						}
					}
				}
			}
		}

		DrawCanvas(lNode, pRenderer, &props, pBreak);
		return;
	}


	void Page::DrawCanvas(XmlUtils::CXmlNode& canvas, ASCGraphics::IASCRenderer* pRenderer, DrawPageProperties* pProps, BOOL* pBreak)const
	{
		bool isClipped = false;
		bool isTransformed = false;
		XmlUtils::CXmlNode	node;
		XmlUtils::CXmlNodes	nodesList;
		CString				nodeName;

		CString clip = canvas.GetAttribute(_T("Clip"), g_cstrEmpty);
		if(clip != g_cstrEmpty)
		{
			CAtlMap<CString, CString>::CPair* pPair = pProps->m_mapFigures.Lookup(clip);
			if (NULL != pPair)
				clip = pPair->m_value;

			isClipped = true;
			pRenderer->PathCommandStart();
			pRenderer->BeginCommand(0x0020);
			pRenderer->put_ClipMode(0);
			pRenderer->BeginCommand( 0x0004 );
			VmlToRenderer(clip, pRenderer);

			pRenderer->EndCommand( 0x0004 );
			pRenderer->EndCommand(0x0020);
			pRenderer->PathCommandEnd();
		}
		CString transform;
		transform = canvas.GetAttribute(_T("RenderTransform"), g_cstrEmpty);
		if(transform != g_cstrEmpty)
		{
			isTransformed = true;
			TransformToRenderer(transform, pRenderer, pProps);
		}

		canvas.GetNodes(_T("*"), nodesList);
		int nodes = nodesList.GetCount();
		for(int i = 0; i < nodes; i++)
		{
			nodesList.GetAt(i, node);
			nodeName = node.GetName();
			nodeName = RemoveNamespace(nodeName);
			if(nodeName == g_cstrGlyphs)
				DrawGlyph(node, pRenderer, pProps);
			else if(nodeName == g_cstrCanvas)
				DrawCanvas(node, pRenderer, pProps, pBreak);
			else if(nodeName == g_cstrCanvasTransform)
			{
				if(!isTransformed)
				{
					CanvasTransform(node, pRenderer, pProps);
					isTransformed = true;
				}
			}
			else if(nodeName == g_cstrPath)
				DrawPath(node, pRenderer, pProps);

			if(pBreak != NULL)
			{
				if(*pBreak)
					return;
			}
		}

		if(isClipped)
		{
			pRenderer->BeginCommand(0x0040);
			pRenderer->EndCommand(0x0040);
		}
		if(isTransformed)
			ResetTransform(pRenderer, pProps);
	}


	void Page::DrawGlyph(XmlUtils::CXmlNode& node, ASCGraphics::IASCRenderer* pRenderer, DrawPageProperties* pProps)const
	{
		//CString font = m_Path + node.GetAttribute(_T("FontUri"));
		//font.Replace(_T("\\/"), _T("\\"));
		//font.Replace(_T("/"), _T("\\"));

		//_bstr_t bs;
		//bs = font;
		//pRenderer->put_FontPath(bs.GetBSTR());
		CString font = node.GetAttribute(g_cstrFont, g_cstrEmpty);
		if(font != g_cstrEmpty)
		{
			CString fname = font.Left(font.ReverseFind(_T('.')));
			fname = fname.Right(fname.GetLength() - fname.ReverseFind(_T('/')) - 1);
			font = m_strPath + _T("/") + font;

			CString ext = GetExtension(font);
			ext.MakeLower();
			if(ext == _T("odttf"))
			{
				//CTemporaryCS* cs = new CTemporaryCS(m_pFontCS);
				//BOOL lcs = TryEnterCriticalSection(m_pFontCS);
				//while(lcs == 0)
				//{
				//	lcs = TryEnterCriticalSection(m_pFontCS);
				//}
				EnterCriticalSection(m_pFontCS);
				
				fname.MakeLower();
				if (NULL == m_fontMap->Lookup(fname))
				{
					if (m_fontMap->SetAt(fname, true))
					{
						unsigned char key[16];
						GetFontKey(fname, key);
						CFile fontFile;
						HRESULT res = fontFile.OpenFileRW(font);
						//assert(res == S_OK);
						unsigned char fontData[32];
						fontFile.ReadFile(fontData, 32);
						for(int i = 0; i < 32; i++)
							fontData[i] ^= key[i % 16];
						fontFile.SetPosition(0);
						fontFile.WriteFile(fontData, 32);
						fontFile.CloseFile();
					}
				}

				//delete cs;
				LeaveCriticalSection(m_pFontCS);
			}

			font.Replace(_T("/"), _T("\\"));
			int nFind = font.Find(_T("\\\\"));
			while (-1 != nFind)
			{
				font.Replace(_T("\\\\"), _T("\\"));
				nFind = font.Find(_T("\\\\"));
			}

			_bstr_t bsFont(font);
			pRenderer->put_FontPath(bsFont.GetBSTR());
		}

		int bgr, alpha;
		ARGB2BGRA(node.GetAttribute(g_cstrFill, _T("#FF000000")), bgr, alpha);
		pRenderer->put_BrushColor1(bgr & 0x00FFFFFF);
		pRenderer->put_BrushAlpha1(alpha);
		pRenderer->put_BrushType(cbsSolid);

		CString fontStyle = node.GetAttribute(g_cstrFontStyle, g_cstrNone);
		if(fontStyle == g_cstrItalic)
			pRenderer->put_FontStyle(0x02);
		else if(fontStyle == g_cstrBold)
			pRenderer->put_FontStyle(0x01);
		else if(fontStyle == g_cstrBoldItalic)
			pRenderer->put_FontStyle(0x03);

		double fontSize = GetDouble(node.GetAttribute(g_cstrFontSize));

		CString transform = node.GetAttribute(_T("RenderTransform"), _T(""));
		bool isTransformed = false;
		if(transform != g_cstrEmpty)
		{
			TransformToRenderer(transform, pRenderer, pProps);
			isTransformed = true;
			if(fontSize < 5)
			{
				double det = pProps->m_CurrentTransform.m_agg_mtx.sx * pProps->m_CurrentTransform.m_agg_mtx.sy
					- pProps->m_CurrentTransform.m_agg_mtx.shx * pProps->m_CurrentTransform.m_agg_mtx.shy;
				double rtdet = sqrt(det);
				fontSize *= rtdet;
				pProps->m_CurrentTransform.m_agg_mtx.sx		= pProps->m_CurrentTransform.m_agg_mtx.sx / rtdet;
				pProps->m_CurrentTransform.m_agg_mtx.shx	= pProps->m_CurrentTransform.m_agg_mtx.shx / rtdet;
				pProps->m_CurrentTransform.m_agg_mtx.sy		= pProps->m_CurrentTransform.m_agg_mtx.sy / rtdet;
				pProps->m_CurrentTransform.m_agg_mtx.shy	= pProps->m_CurrentTransform.m_agg_mtx.shy / rtdet;
				pRenderer->SetTransform(pProps->m_CurrentTransform.m_agg_mtx.sx, pProps->m_CurrentTransform.m_agg_mtx.shy,
										pProps->m_CurrentTransform.m_agg_mtx.shx, pProps->m_CurrentTransform.m_agg_mtx.sy,
										xpsUnitToMM(pProps->m_CurrentTransform.m_agg_mtx.tx), xpsUnitToMM(pProps->m_CurrentTransform.m_agg_mtx.ty));
			}
		}
		pRenderer->put_FontSize(fontSize * 0.75);

		CString text = node.GetAttributeExt(g_cstrUnicodeString, g_cstrEmpty);
		int textLen = text.GetLength();
		//_bstr_t bs (node.GetAttribute(_T("UnicodeString"), _T("")));
		double x = GetDouble(node.GetAttribute(g_cstrOriginX));
		double y = GetDouble(node.GetAttribute(g_cstrOriginY));

		CString tempStr(text[0]);
		_bstr_t bs = tempStr;
		std::vector<std::vector<CString> > dest;
		DelimString(node.GetAttribute(g_cstrIndicies, g_cstrEmpty), dest, _T(';'), _T(','));

		_bstr_t bsFont(font);
		//HRESULT res = m_FontManager->LoadFontFromFile(bsFont.GetBSTR(), (float)(fontSize * 0.75), 96, 96, 0);
		pProps->m_FontManager->LoadFontFromFile(bsFont.GetBSTR(), (float)(fontSize * 0.75), 96, 96, 0);

		for(int i = 0; i < textLen - 1; i++)
		{
			if(i >= dest.size())
				dest.push_back(std::vector<CString>());
			if(dest.at(i).size() >= 2)
			{
				pRenderer->CommandDrawText(bs.GetBSTR(), xpsUnitToMM(x), xpsUnitToMM(y), 0, 0, 0);
				//pRenderer->CommandDrawTextEx(bs.GetBSTR(), NULL, NULL, xpsUnitToMM(x), xpsUnitToMM(y), 0, 0, 0, 0);
				x += GetDouble(dest.at(i).at(1)) * fontSize / 100.0;
				tempStr = text[i + 1];
				bs = tempStr;
			}
			else
			{
				pRenderer->CommandDrawText(bs.GetBSTR(), xpsUnitToMM(x), xpsUnitToMM(y), 0, 0, 0);
				//pRenderer->CommandDrawTextEx(bs.GetBSTR(), NULL, NULL, xpsUnitToMM(x), xpsUnitToMM(y), 0, 0, 0, 0);

				//_bstr_t bsFont(font);
				//HRESULT res = m_FontManager->LoadFontFromFile(bsFont.GetBSTR(), (float)(fontSize * 0.75), 96, 96, 0);
				pProps->m_FontManager->LoadStringW(bs.GetBSTR(), 0, 0);
				float str_x, str_y, str_w, str_h;
				pProps->m_FontManager->MeasureString2(&str_x, &str_y, &str_w, &str_h);
				x += str_w;

				tempStr = text[i + 1];
				bs = tempStr;
			}
		}
		if(tempStr != g_cstrEmpty)
		{
			bs = tempStr;
			pRenderer->CommandDrawText(bs.GetBSTR(), xpsUnitToMM(x), xpsUnitToMM(y), 0, 0, 0);
			//pRenderer->CommandDrawTextEx(bs.GetBSTR(), NULL, NULL, xpsUnitToMM(x), xpsUnitToMM(y), 0, 0, 0, 0);
		}
		if(isTransformed)
			ResetTransform(pRenderer, pProps);
	}


	void Page::DrawPath(XmlUtils::CXmlNode& node, ASCGraphics::IASCRenderer* pRenderer, DrawPageProperties* pProps)const
	{
		CString transform = node.GetAttribute(_T("RenderTransform"), _T(""));
		bool isTransformed = false;
		if(transform != g_cstrEmpty)
		{
			TransformToRenderer(transform, pRenderer, pProps);
			isTransformed = true;
		}

		int alpha, bgr;
		ARGB2BGRA(node.GetAttribute(_T("Stroke"), _T("#00FFFFFF")), bgr, alpha);
		pRenderer->put_PenColor(bgr & 0x00FFFFFF);
		pRenderer->put_PenAlpha(alpha);
		pRenderer->put_PenSize(xpsUnitToMM(GetDouble(node.GetAttribute(_T("StrokeThickness"), _T("1.0")))));

		bool isFilled = (node.GetAttribute(_T("Fill")) != g_cstrEmpty);
		if(isFilled)
		{
			ARGB2BGRA(node.GetAttribute(_T("Fill"), _T("#00FFFFFF")), bgr, alpha);
			pRenderer->put_BrushType(cbsSolid);
			pRenderer->put_BrushColor1(bgr & 0x00FFFFFF);
			pRenderer->put_BrushAlpha1(alpha);
		}
		else
		{
			XmlUtils::CXmlNode fill;
			if(node.GetNode(_T("Path.Fill"), fill))
				FillToRenderer(fill, pRenderer);
			else
			{
				pRenderer->put_BrushAlpha1(0);
				pRenderer->put_BrushAlpha2(0);
			}
		}

		CString data = node.GetAttribute(_T("Data"));
		if(data == g_cstrEmpty)
		{
			GetDataFromNode(data, node);
		}
		if (data[0] == _T('{'))
		{
			CAtlMap<CString, CString>::CPair* pPair = pProps->m_mapFigures.Lookup(data);
			if (NULL != pPair)
				data = pPair->m_value;
			else
			{
				if(isTransformed)
					ResetTransform(pRenderer, pProps);
				return;
			}
		}

		pRenderer->BeginCommand(0x0004);

		LONG lBrushType = 0;
		pRenderer->get_BrushType(&lBrushType);
		LONG lRendererType = 0x00;
		pRenderer->get_Type(&lRendererType);

		// заглушка под ПДФ
		if ((0x01 == lRendererType))
		{
			//pRenderer->PathCommandEnd();
		}
		else
		{
			pRenderer->PathCommandStart();
		}

		bool l_bFillType = false;
		int l_iFillMode;
		if(data[0] != _T('{'))
			l_bFillType = VmlToRenderer(data, pRenderer);
		if(l_bFillType)
			l_iFillMode = 0x101;
		else
			l_iFillMode = 0x201;

		//pRenderer->DrawPath(l_iFillMode);
		HRESULT hr = pRenderer->DrawPath(l_iFillMode);

		pRenderer->EndCommand(0x0004);
		pRenderer->PathCommandEnd();

		if(isTransformed)
			ResetTransform(pRenderer, pProps);
	}


	void Page::CanvasTransform(XmlUtils::CXmlNode& node, ASCGraphics::IASCRenderer* pRenderer, DrawPageProperties* pProps)const
	{
		XmlUtils::CXmlNode transform;
		node.GetNode(_T("MatrixTransform"), transform);
		CString data = transform.GetAttribute(_T("Matrix"));
		TransformToRenderer(data, pRenderer, pProps);
	}


	void Page::FillToRenderer(XmlUtils::CXmlNode& node, ASCGraphics::IASCRenderer* pRenderer)const
	{
		XmlUtils::CXmlNode brush;
		if(node.GetNode(_T("SolidColorBrush"), brush))
		{
			pRenderer->put_BrushType(cbsSolid);
			int bgr, alpha;
			ARGB2BGRA(brush.GetAttribute(_T("Color"), _T("#00FFFFFF")), bgr, alpha);
			pRenderer->put_BrushColor1(bgr & 0x00FFFFFF);
			pRenderer->put_BrushAlpha1(alpha);
		}
		else if(node.GetNode(_T("ImageBrush"), brush))
		{
			pRenderer->put_BrushType(3008);
			CString str = m_strPath;
			str += brush.GetAttribute(_T("ImageSource"));
			_bstr_t bs = str;
			pRenderer->put_BrushTexturePath(bs.GetBSTR());
		}
		else
		{
			pRenderer->put_BrushAlpha1(0);
			pRenderer->put_BrushAlpha2(0);
		}
	}
} // namespace XPS