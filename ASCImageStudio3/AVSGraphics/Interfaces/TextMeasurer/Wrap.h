#pragma once

namespace ImageStudio {
	namespace Serialize {
		namespace Paint {
			namespace Common {
				class CWrapPolygon
				{
				private:
					CArray<PointF> m_arPoints;

				public:
					CWrapPolygon() : m_arPoints()
					{
					}
					CWrapPolygon(const CWrapPolygon& oSrc)
					{
						*this = oSrc;
					}
					CWrapPolygon& operator=(const CWrapPolygon& oSrc)
					{
						m_arPoints.Copy(oSrc.m_arPoints);

						return *this;
					}
					~CWrapPolygon()
					{
					}

				public: 
					void LoadFromXml(XmlUtils::CXmlNode& oNode)
					{

					}
					void SetPoints(const CArray<PointF>& arrPoints)
					{
						m_arPoints.RemoveAll();
						m_arPoints.Copy(arrPoints);
					}

				public:
					RectF GetBounds()
					{
					}

					void Calc(const RectF& oTextRect, CArray<RectF>& oArray)
					{
						oArray.RemoveAll();

						REAL left	= oTextRect.X;
						REAL right	= oTextRect.GetRight();

						REAL top	= oTextRect.Y;
						REAL bottom = oTextRect.GetBottom();

						CArray<REAL> oArray1;
						CArray<REAL> oArray2;

						Calc(left, right, top, oArray1);
						Calc(left, right, bottom, oArray2);

						size_t lMin = 0;
						size_t lMax = 0;
						CArray<REAL>* pArray = NULL;

						if (oArray1.GetCount() >= oArray.GetCount())
						{
							lMin = oArray2.GetCount();
							lMax = oArray1.GetCount();

							pArray = &oArray1;
						}
						else
						{
							lMin = oArray.GetCount();
							lMax = oArray.GetCount();

							pArray = &oArray2;
						}

						BOOL bIsComplete = TRUE; // :  bIsComplete = !IsInPolygon(m_arPoints);

						REAL fStart = left;
						REAL fEnd	= right;

						for (size_t lIndex = 0; lIndex < lMin; ++lIndex)
						{
							if (bIsComplete)
							{
								REAL fCurEnd = min(oArray1[lIndex], oArray2[lIndex]);
								RectF oRect(fStart, top, fCurEnd - fStart, bottom - top);

								oArray.Add(oRect);
							}
							else
							{
								fStart = max(oArray1[lIndex], oArray2[lIndex]);
							}
							bIsComplete = !bIsComplete;
						}
						for (size_t lIndex = lMin; lIndex < lMax; ++lIndex)
						{
							if (bIsComplete)
							{
								RectF oRect(fStart, top, (*pArray)[lIndex] - fStart, bottom - top);

								oArray.Add(oRect);
							}
							else
							{
								fStart = (*pArray)[lIndex];
							}
							bIsComplete = !bIsComplete;
						}

						if (bIsComplete)
						{
							RectF oRect(fStart, top, right - fStart, bottom - top);

							oArray.Add(oRect);
						}	
					}

					void Calc(const REAL& x1, const REAL& x2, const REAL& y, CArray<REAL>& oArray)
					{
						oArray.RemoveAll();
						size_t lCount = m_arPoints.GetCount();

						for (size_t i = 0; i < lCount; ++i)
						{
							size_t j = i + 1;
							if (j == lCount)
								--j;

							REAL fCrossX = 0;
							if (Cross(x1, x2, y, m_arPoints[i], m_arPoints[j], fCrossX))
							{
								oArray.Add(fCrossX);
							}
						}

						oArray.Sort();				
					}

					void Calculate(const  RectF& oTextRect, CArray<RectF>& oArray)
					{
						oArray.RemoveAll();

						REAL xLeft1 = 0, xRight1 = 0;
						REAL xLeft2 = 0, xRight2 = 0;

						REAL fVerticesMax = oTextRect.X;
						REAL fVerticesMin = oTextRect.GetRight();

						REAL yMin = oTextRect.Y;
						REAL yMax = oTextRect.GetBottom();

						size_t lCount = m_arPoints.GetCount();
						for (size_t i = 0; i < lCount; ++i)
						{
							PointF& oPoint = m_arPoints[i];

							if ((yMin < oPoint.Y) && (yMax > oPoint.Y))
							{
								if (fVerticesMin > oPoint.X)
									fVerticesMin = oPoint.X;
								if (fVerticesMax < oPoint.X)
									fVerticesMax = oPoint.X;
							}
						}

						REAL left	= oTextRect.X;
						REAL right	= oTextRect.GetRight();

						CalcCrosses(left, right, yMin, xLeft1, xRight1);
						CalcCrosses(left, right, yMax, xLeft2, xRight2);

						REAL __fRight	= min(fVerticesMin, min(xLeft1, xLeft2));
						REAL __fLeft	= max(fVerticesMax, max(xRight1, xRight2));

						if (__fRight > left && (!IsInPolygon(left, yMin) && !IsInPolygon(left, yMax)))
						{
							RectF oRectLeft(left, yMin, __fRight - left, yMax - yMin);
							oArray.Add(oRectLeft);
						}
						if (__fLeft < right && (!IsInPolygon(right, yMin) && !IsInPolygon(right, yMax)))
						{
							RectF oRectRight(__fLeft, yMin, right - __fLeft, yMax - yMin);
							oArray.Add(oRectRight);
						}
					}

					void CalcCrosses(const REAL& left, const REAL& right, const REAL& y, REAL& fMin, REAL& fMax)
					{
						CArray<REAL> oArray;
						size_t lCount = m_arPoints.GetCount();

						for (size_t i = 0; i < lCount; ++i)
						{
							size_t j = i + 1;
							if (j == lCount)
								--j;

							REAL fCrossX = 0;
							if (Cross(left, right, y, m_arPoints[i], m_arPoints[j], fCrossX))
							{
								oArray.Add(fCrossX);
							}
						}

						fMin = right;
						fMax = left;

						lCount = oArray.GetCount();
						for (size_t i = 0; i < lCount; ++i)
						{
							if (oArray[i] < fMin)
								fMin = oArray[i];

							if (oArray[i] > fMax)
								fMax = oArray[i];
						}
					}

					BOOL IsInPolygon(const float& pointX, const float& pointY)
					{
						if (3 > m_arPoints.GetCount()) 
							return FALSE;

						int     yflag0, yflag1, inside_flag;
						float   vtx0, vty0, vtx1, vty1;

						int num_points = (int)m_arPoints.GetCount();

						vtx0 = m_arPoints[num_points - 1].X;
						vty0 = m_arPoints[num_points - 1].Y;

						yflag0 = (vty0 >= pointY);

						vtx1 = m_arPoints[0].X;
						vty1 = m_arPoints[0].Y;

						inside_flag = 0;
						for (int j = 1; j <= num_points; ++j) 
						{
							yflag1 = (vty1 >= pointY);
							if (yflag0 != yflag1) 
							{
								if ( ((vty1-pointY) * (vtx0-vtx1) >= (vtx1-pointX) * (vty0-vty1)) == yflag1 ) 
								{
									inside_flag ^= 1;
								}
							}

							yflag0 = yflag1;
							vtx0 = vtx1;
							vty0 = vty1;

							int k = (j >= num_points) ? j - num_points : j;
							vtx1 = m_arPoints[k].X;
							vty1 = m_arPoints[k].Y;
						}
						return inside_flag != 0;
					}

					BOOL Cross(const REAL& fX1, const REAL& fX2, const REAL& fY, const PointF& oPoint1, const PointF& oPoint2, REAL& fCrossX)
					{
						double x1 = (double)fX1;
						double x2 = (double)fX2;
						double y1  = (double)fY;

						double x3 = (double)oPoint1.X;
						double y3 = (double)oPoint1.Y;
						double x4 = (double)oPoint2.X;
						double y4 = (double)oPoint2.Y;

						double dZnam = (y4 - y3) * (x2 - x1);// - (x4 - x3) * (y2 - y1);

						if (0 == dZnam)
							return FALSE;

						double dCisl1 = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3);
						double dCisl2 = (x2 - x1) * (y1 - y3);// - (y2 - y1) * (x1 - x3);

						double dT1 = dCisl1 / dZnam;
						double dT2 = dCisl2 / dZnam;

						if ((0 >= dT1) || (dT1 >= 1) || (0 >= dT2) || (dT2 >= 1))
						{
							return FALSE;
						}

						//    ...
						fCrossX = (REAL)(x1 + dT1 * (x2 - x1));
						//double dY = y1 + dT1 * (y2 - y1);

						return TRUE;
					}
				};

				class CWrapRegion
				{
				public:
					class CLinePoint
					{
					public:
						REAL point;
						bool bLeft;

					public:
						CLinePoint()
						{
							point = 0;
							bLeft = true;
						}
						CLinePoint(REAL p, bool b)
						{
							point = p;
							bLeft = b;
						}
						CLinePoint(const CLinePoint& oSrc)
						{
							*this = oSrc;
						}
						CLinePoint& operator=(const CLinePoint& oSrc)
						{
							point = oSrc.point;
							bLeft = oSrc.bLeft;

							return *this;
						}
						friend bool operator>(const CLinePoint& a, const CLinePoint& b)
						{
							return (a.point > b.point) ? true : false;
						}
						friend bool operator>=(const CLinePoint& a, const CLinePoint& b)
						{
							return (a.point >= b.point) ? true : false;
						}
						friend bool operator<(const CLinePoint& a, const CLinePoint& b)
						{
							return (a.point < b.point) ? true : false;
						}
						friend bool operator<=(const CLinePoint& a, const CLinePoint& b)
						{
							return (a.point <= b.point) ? true : false;
						}
						~CLinePoint()
						{
						}
					};

				private:
					CArray<CWrapPolygon> m_arPolygons;
					CAtlArray<REAL> m_arDistanceLeft;
					CAtlArray<REAL> m_arDistanceRight;

				public:
					CWrapRegion() 
						: m_arPolygons(), 
						m_arDistanceLeft(), m_arDistanceRight()
					{
					}
					CWrapRegion(const CWrapRegion& oSrc)
					{
						*this = oSrc;
					}
					CWrapRegion& operator=(const CWrapRegion& oSrc)
					{
						m_arPolygons.Copy(oSrc.m_arPolygons);
						m_arDistanceLeft.Copy(oSrc.m_arDistanceLeft);
						m_arDistanceRight.Copy(oSrc.m_arDistanceRight);

						return *this;
					}
					~CWrapRegion()
					{
					}

				public:
					void Add(const CWrapPolygon& oSrc, const REAL& fDistanceLeft = 0.0f, const REAL& fDistanceRight = 0.0f)
					{
						m_arPolygons.Add(oSrc);
						m_arDistanceLeft.Add(fDistanceLeft);
						m_arDistanceRight.Add(fDistanceRight);
					}
					void RemoveAll()
					{
						m_arPolygons.RemoveAll();
					}
					void RemoveAt(size_t nIndex)
					{
						m_arPolygons.RemoveAt(nIndex);
					}

				public:
					void Calculate(const RectF oLineRect, CArray<RectF>& oArray)
					{
						oArray.RemoveAll();

						size_t lCount = m_arPolygons.GetCount();
						if (0 == lCount)
						{
							oArray.Add(oLineRect);
							return;
						}

						CArray<RectF>* pRects = new CArray<RectF>[lCount];

						for (size_t i = 0; i < lCount; ++i)
						{
							m_arPolygons[i].Calculate(oLineRect, pRects[i]);
							ApplyDistance(oLineRect, pRects[i], m_arDistanceLeft[i], m_arDistanceRight[i]);
						}

						CArray<CLinePoint> oLineBounds;
						for (size_t lIndexPolygon = 0; lIndexPolygon < lCount; ++lIndexPolygon)
						{
							if (pRects[lIndexPolygon].GetCount() == 0)
							{
								RELEASEARRAYOBJECTS(pRects);
								return;
							}
							for (size_t i = 0; i < pRects[lIndexPolygon].GetCount(); ++i)
							{
								CLinePoint oPointL(pRects[lIndexPolygon][i].X, true);
								oLineBounds.Add(oPointL);

								CLinePoint oPointR(pRects[lIndexPolygon][i].GetRight(), false);
								oLineBounds.Add(oPointR);
							}
						}

						oLineBounds.Sort();

						RELEASEARRAYOBJECTS(pRects);

						REAL fY			= oLineRect.Y;
						REAL fHeight	= oLineRect.Height;

						REAL fLeft	= oLineRect.X;
						REAL fRight	= left;

						size_t nCurIndex	= 0;
						size_t nCount		= oLineBounds.GetCount();

						bool bFoundLeft = false;

						while (nCurIndex < nCount)
						{
							while (!oLineBounds[nCurIndex].bLeft)
							{
								++nCurIndex;

								if (nCurIndex == nCount)
								{
									//    
									return;
								}

								continue;
							}

							while (oLineBounds[nCurIndex].bLeft)
							{
								++nCurIndex;
								continue;
							}

							fLeft  = oLineBounds[nCurIndex - 1].point;
							fRight = oLineBounds[nCurIndex].point;

							RectF oRect(fLeft, fY, fRight - fLeft, fHeight);
							oArray.Add(oRect);

							++nCurIndex;
						}
					}
					void ApplyDistance(const RectF& oLineRect, CArray<RectF>& arrRects, const REAL& fDistanceLeft, const REAL& fDistanceRight)
					{
						if (arrRects.GetCount() == 0)
						{
							return;
						}
						BYTE nFirst	= 0;
						if ((fDistanceLeft != 0) && (arrRects[nFirst].GetRight() != oLineRect.GetRight()))
						{
							arrRects[nFirst].Width -= fDistanceLeft;
							if (arrRects[nFirst].Width <= 0)
							{
								arrRects.RemoveAt(nFirst);
							}
						}

						if (arrRects.GetCount() == 0)
						{
							return;
						}

						size_t nLast	= arrRects.GetCount() - 1;
						if ((fDistanceRight != 0) && (arrRects[nLast].X != oLineRect.X))
						{
							arrRects[nLast].Width	-= fDistanceRight;
							arrRects[nLast].X		+= fDistanceRight;
							if (arrRects[nLast].Width <= 0)
							{
								arrRects.RemoveAt(nLast);
							}
						}
					}
				};
			}
		}
	}
}