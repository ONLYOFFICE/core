#pragma once
#include "..\stdafx.h"

#define USE_ATL_CSTRING
#include <StringUtils.h>
//#include <ImageSerializeConstants.h>
//#include <ImageSerializeConstants2.h>
//#include <GraphicsRenderer.h>
#include "TimeMeasurer.h"

//#include "..\..\..\..\Common\OfficeDrawing\Shapes\Shape.h"
#include "..\..\..\..\Common\OfficeDrawing\Shapes\ShapeIncluder.h"

#ifdef _AVS_OFFICE_VIEWER_
#include "..\..\..\..\Common\OfficeDrawing\Shapes\ShapeCtrl.h"
#endif

#include "StructuresGDI.h"
#include "TextMeasurer/Structures.h"
#include "ASCRenderer.h"

#include "XmlConst.h"

#include "Array.h" 
#include <float.h>
#include <stack>
#include <map>
using namespace ArrayUtils;

#include <assert.h>
#include "FileDownloader.h"

namespace ImageStudio
{
	namespace Serialize
	{
		namespace Paint
		{
			namespace Common
			{
				class CDrawGraphicPath
				{
				public:

					virtual int InternalGetRequiredStreamSize()
					{
						return 0;
					}
					/*virtual void InternalToStream(Streams::IStream* pStream)
					{
					}*/
					/*virtual void InternalFromStream(Streams::IStream* pStream)
					{
					}*/
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						m_bStroke = (1 == Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("stroke"), _T("0"))));
						m_bFill = (1 == Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("fill"), _T("0"))));

						m_dWidthMM = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("widthmm"), _T("210")));
						m_dHeightMM = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("heightmm"), _T("190")));

						m_dAngle	= Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("angle"), _T("0")));
						m_lFlags	= Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("flags"), _T("0")));

						m_oBounds.left		= Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("bounds-left"),		_T("0")));
						m_oBounds.top		= Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("bounds-top"),		_T("0")));
						m_oBounds.right		= Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("bounds-right"),	_T("0")));
						m_oBounds.bottom	= Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("bounds-bottom"),	_T("0")));

						XmlUtils::CXmlNodes oNodes;
						oXmlNode.GetNodes(_T("part"), oNodes);
						for (int nIndex = 0; nIndex < oNodes.GetCount(); ++nIndex)
						{
							CPart oPart;
							XmlUtils::CXmlNode oNode;
							oNodes.GetAt(nIndex, oNode);
							oPart.FromXmlNode(oNode);
							m_arParts.Add(oPart);
						}

						XmlUtils::CXmlNode oPenNode;
						if (oXmlNode.GetNode(_T("pen"), oPenNode))
						{
							Pen.FromXmlNode(oPenNode);
						}
						XmlUtils::CXmlNode oBrushNode;
						if (oXmlNode.GetNode(_T("brush"), oBrushNode))
						{
							Brush.FromXmlNode(oBrushNode);
						}
					}					
					virtual void InternalClear()
					{
						m_bFill = false;
						m_bStroke = true;

						m_dWidthMM = 210;
						m_dHeightMM = 190;

						m_dAngle = 0;
						m_lFlags = 0;
					}

				public:

					CDrawGraphicPath()
					{
					}

					virtual void DrawOnGraphics(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, Gdiplus::Brush* pBrush)
					{
						PointF pointCur; pointCur.X = 0; pointCur.Y = 0;
						PointF pointMove; pointMove.X = 0; pointMove.Y = 0;
						Gdiplus::GraphicsPath oPath;
						for (int nIndex = 0; nIndex < m_arParts.GetSize(); ++nIndex)
						{
							m_arParts[nIndex].DrawOnGraphics(&oPath, pointCur, pointMove);
						}

						int nCount = oPath.GetPointCount();
						PointF* pPoints = new PointF[nCount];
						oPath.GetPathPoints(pPoints, nCount);

						if (m_bStroke)
							pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
						else
							pGraphics->SetSmoothingMode(SmoothingModeNone);

						pGraphics->DrawPath(pPen, &oPath);
						if (m_bFill)
						{
							pGraphics->FillPath(pBrush, &oPath);
						}
					}

					virtual void Draw(IAVSRenderer* pRenderer)
					{
						if (NULL == pRenderer || (0 == m_arParts.GetSize()))
							return;

						CFileDownloader* pDownloader = NULL;
						CString strOld = Brush.m_oBrush.TexturePath;
						if (c_BrushTypeTexture == Brush.m_oBrush.Type || c_BrushTypePattern == Brush.m_oBrush.Type)
						{
							if (CFileDownloader::IsNeedDownload(strOld))
							{
								pDownloader = new CFileDownloader(strOld, TRUE);
								pDownloader->StartWork( 1 );
								while ( pDownloader->IsRunned() )
								{
									::Sleep( 10 );
								}

								if ( pDownloader->IsFileDownloaded() )
								{
									Brush.m_oBrush.TexturePath = pDownloader->GetFilePath();
								}
							}
						}

						//    
						BSTR bsPen = Pen.m_oPen.ToXmlString().AllocSysString();
						BSTR bsBrush = Brush.m_oBrush.ToXmlString().AllocSysString();

						Brush.m_oBrush.TexturePath = strOld;

						pRenderer->SetPen(bsPen);
						pRenderer->SetBrush(bsBrush);

						pRenderer->put_Width((float)m_dWidthMM);
						pRenderer->put_Height((float)m_dHeightMM);

						SysFreeString(bsPen);
						SysFreeString(bsBrush);

						pRenderer->SetCommandParams(m_dAngle, m_oBounds.left, m_oBounds.top, m_oBounds.GetWidth(), m_oBounds.GetHeight(), m_lFlags);

						pRenderer->BeginCommand(c_nPathType);

						PointF pointCur; pointCur.X = 0; pointCur.Y = 0;
						for (int nIndex = 0; nIndex < m_arParts.GetSize(); ++nIndex)
						{
							m_arParts[nIndex].Draw(pRenderer, pointCur);
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

						pRenderer->DrawPath(lType);
						pRenderer->SetCommandParams(0, -1, -1, -1, -1, 0);

						pRenderer->EndCommand(c_nPathType);

						RELEASEOBJECT(pDownloader);
					}

				public:
					enum RulesType
					{
						rtMoveTo			= 0,	// 2
						rtLineTo			= 1,	// 2*
						rtCurveTo			= 2,	// 6*

						rtClose				= 3,	// 0

						rtRMoveTo			= 5,	// 2*
						rtRLineTo			= 6,	// 2*
						rtRCurveTo			= 7,	// 6*

						rtAngleEllipseTo	= 10,	// 6*
						rtAngleEllipse		= 11,	// 6*

						rtArc				= 12,	// 8* 
						rtArcTo				= 13,	// 8*

						rtClockwiseArcTo	= 14,	// 8*
						rtClockwiseArc		= 15,	// 8*

						rtEllipticalQuadrX	= 16,	// 2*
						rtEllipticalQuadrY	= 17,	// 2*

						rtQuadrBesier		= 18	// 2 + 2*
					};

					class CPart
					{
					public:
						RulesType m_eType;
						CSimpleArray<PointF> m_arPoints;

					public:
						CPart() : m_eType(rtMoveTo), m_arPoints()
						{
						}
						CPart& operator =(const CPart& oSrc)
						{
							m_eType	= oSrc.m_eType;
							m_arPoints.RemoveAll();
							for (int nIndex = 0; nIndex < oSrc.m_arPoints.GetSize(); ++nIndex)
							{
								m_arPoints.Add(oSrc.m_arPoints[nIndex]);
							}
							return (*this);
						}
						~CPart()
						{
							m_arPoints.RemoveAll();
						}
						void FromXmlNode(XmlUtils::CXmlNode& oNode)
						{
							CString strName = oNode.GetAttribute(_T("name"));
							if (_T("moveto") == strName) m_eType = rtMoveTo;
							else if (_T("lineto") == strName) m_eType = rtLineTo;
							else if (_T("curveto") == strName) m_eType = rtCurveTo;
							else if (_T("rmoveto") == strName) m_eType = rtRMoveTo;
							else if (_T("rlineto") == strName) m_eType = rtRLineTo;
							else if (_T("rcurveto") == strName) m_eType = rtRCurveTo;
							else if (_T("ellipseto") == strName) m_eType = rtAngleEllipseTo;
							else if (_T("ellipse") == strName) m_eType = rtAngleEllipse;
							else if (_T("arc") == strName) m_eType = rtArc;
							else if (_T("arcto") == strName) m_eType = rtArcTo;
							else if (_T("clockwisearcto") == strName) m_eType = rtClockwiseArcTo;
							else if (_T("clockwisearc") == strName) m_eType = rtClockwiseArc;
							else if (_T("ellipticalx") == strName) m_eType = rtEllipticalQuadrX;
							else if (_T("ellipticaly") == strName) m_eType = rtEllipticalQuadrY;
							else if (_T("qbesier") == strName) m_eType = rtQuadrBesier;
							else m_eType = rtClose;

							CString strPath = oNode.GetAttribute(_T("path"));
							if (_T("") == strPath)
								return;

							CSimpleArray<CString> arStrNums;
							ParseString(_T(" "), strPath, &arStrNums);

							bool bIsX = true;
							int nCurPoint = 0;
							for (int nIndex = 0; nIndex < arStrNums.GetSize(); ++nIndex)
							{
								if (bIsX)
								{
									++nCurPoint;
									PointF point;
									m_arPoints.Add(point);
									m_arPoints[nCurPoint - 1].X = (REAL)XmlUtils::GetDouble(arStrNums[nIndex]);
								}
								else
								{
									m_arPoints[nCurPoint - 1].Y = (REAL)XmlUtils::GetDouble(arStrNums[nIndex]);
								}
								bIsX = !bIsX;
							}
						}
						void ParseString(CString strDelimeters, CString strSource, 
							CSimpleArray<CString>* pArrayResults, bool bIsCleared = true)
						{
							if (NULL == pArrayResults)
								return;

							if (bIsCleared)
								pArrayResults->RemoveAll();

							CString resToken;
							int curPos= 0;

							resToken = strSource.Tokenize(strDelimeters, curPos);
							while (resToken != _T(""))
							{
								pArrayResults->Add(resToken);
								resToken = strSource.Tokenize(strDelimeters, curPos);
							};
						}

						void CheckLastPoint(Gdiplus::GraphicsPath* pPath, PointF& pointCur)
						{
							int nCount = pPath->GetPointCount();
							if (0 < nCount)
							{
								PointF* pPoints = new PointF[nCount];
								pPath->GetPathPoints(pPoints, nCount);

								pointCur.X = pPoints[nCount - 1].X;
								pointCur.Y = pPoints[nCount - 1].Y;

								delete []pPoints;
							}
						}
						void CheckLastPoint(IAVSRenderer* pRenderer, PointF& pointCur)
						{
							if (NULL == pRenderer)
								return;

							double fX = 0;
							double fY = 0;
							pRenderer->PathCommandGetCurrentPoint(&fX, &fY);

							pointCur.X = (float)fX;
							pointCur.Y = (float)fY;
						}
						void CheckFirstPoint(Gdiplus::GraphicsPath* pPath, PointF& pointCur)
						{
							int nCount = pPath->GetPointCount();
							if (0 < nCount)
							{
								PointF* pPoints = new PointF[nCount];
								pPath->GetPathPoints(pPoints, nCount);

								pointCur.X = pPoints[0].X;
								pointCur.Y = pPoints[0].Y;

								delete []pPoints;
							}
						}
						REAL GetAngle(REAL fCentreX, REAL fCentreY, REAL fX, REAL fY)
						{
							//  -   + (..  )
							REAL dX = fX - fCentreX;
							REAL dY = fY - fCentreY;

							REAL modDX = abs(dX);
							REAL modDY = abs(dY);

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

							REAL fAngle = atan(dY / dX);
							fAngle *= float(180 / M_PI);
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

						inline REAL GetSweepAngle(const REAL& angleStart, const REAL& angleEnd)
						{
							if (angleStart >= angleEnd)
								return angleEnd - angleStart;
							else
								return angleEnd - angleStart - 360;
						}

						void ApplyElliptical(bool& bIsX, REAL& angleStart, REAL& angleSweet, 
							REAL& Left, REAL& Top, REAL& Width, REAL& Height, const PointF& pointCur)
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
									Left = pointCur.X - Width / 2;
									Top = pointCur.Y - Height;
								}
								else if ((Width < 0) && (Height > 0))
								{
									angleStart = -90;
									angleSweet = -90;
									Width *= -1;
									Left = pointCur.X - Width / 2;
									Top = pointCur.Y;
								}
								else if ((Width > 0) && (Height < 0))
								{
									angleStart = 90;
									angleSweet = -90;
									Height *= -1;
									Left = pointCur.X - Width / 2;
									Top = pointCur.Y - Height;
								}
								else
								{
									Left = pointCur.X - Width / 2;
									Top = pointCur.Y;
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
									Left = pointCur.X - Width;
									Top = pointCur.Y - Height / 2;
								}
								else if ((Width < 0) && (Height > 0))
								{
									angleStart = 0;
									angleSweet = 90;
									Width *= -1;
									Left = pointCur.X - Width;
									Top = pointCur.Y - Height / 2;
								}
								else if ((Width > 0) && (Height < 0))
								{
									angleStart = 180;
									angleSweet = 90;
									Height *= -1;
									Left = pointCur.X;
									Top = pointCur.Y - Height / 2;
								}
								else
								{
									Left = pointCur.X;
									Top = pointCur.Y - Height / 2;
								}
							}
							bIsX = !bIsX;
						}


						void GetSafearrayPoints(IAVSRenderer* pRenderer, SAFEARRAY** ppArray, PointF& pointCur, BOOL bR = FALSE)
						{
							if (NULL == ppArray)
								return;

							int nCount = m_arPoints.GetSize();

							//    
							SAFEARRAYBOUND rgsab;
							rgsab.lLbound	= 0;
							rgsab.cElements	= 2 * (nCount + 1);

							SAFEARRAY* pArray = SafeArrayCreate(VT_R8, 1, &rgsab);
							DOUBLE* pBuffer = (DOUBLE*)(pArray->pvData);

							memset(pBuffer, 0, rgsab.cElements * sizeof(double));

							*pBuffer = pointCur.X; ++pBuffer;
							*pBuffer = pointCur.Y; ++pBuffer;

							if (bR)
							{
								for (int nIndex = 0; nIndex < nCount; ++nIndex)
								{
									*pBuffer = (m_arPoints[nIndex].X + pointCur.X); ++pBuffer;
									*pBuffer = (m_arPoints[nIndex].Y + pointCur.Y); ++pBuffer;

									if (nIndex == (nCount - 1))
									{
										pointCur.X += m_arPoints[nIndex].X;
										pointCur.Y += m_arPoints[nIndex].Y; 
									}
								}
							}
							else
							{
								for (int nIndex = 0; nIndex < nCount; ++nIndex)
								{
									*pBuffer = m_arPoints[nIndex].X; ++pBuffer;
									*pBuffer = m_arPoints[nIndex].Y; ++pBuffer;

									if (nIndex == (nCount - 1))
									{
										pointCur.X = m_arPoints[nIndex].X;
										pointCur.Y = m_arPoints[nIndex].Y;
									}
								}
							}

							*ppArray = pArray;
						}

						void DrawOnGraphics(Gdiplus::GraphicsPath* pPath, PointF& pointCur, PointF& pointMove)
						{
							switch (m_eType)
							{
							case rtMoveTo:
								{
									if (0 < m_arPoints.GetSize())
									{
										pointMove.X = m_arPoints[0].X;
										pointMove.Y = m_arPoints[0].Y;

										pointCur.X = pointMove.X;
										pointCur.Y = pointMove.Y;
										pPath->StartFigure();
									}
									break;
								}
							case rtLineTo:
								{
									CSimpleArray<PointF> arPoints;
									arPoints.Add(pointCur);
									for (int nIndex = 0; nIndex < m_arPoints.GetSize(); ++nIndex)
									{
										arPoints.Add(m_arPoints[nIndex]);
									}
									int nSize = arPoints.GetSize();
									if (1 < nSize)
									{
										pPath->AddLines(arPoints.m_aT, nSize);

										pointCur.X = arPoints[nSize - 1].X;
										pointCur.Y = arPoints[nSize - 1].Y;
									}
									arPoints.RemoveAll();
									break;
								}
							case rtCurveTo:
								{
									CSimpleArray<PointF> arPoints;
									arPoints.Add(pointCur);
									for (int nIndex = 0; nIndex < m_arPoints.GetSize(); ++nIndex)
									{
										arPoints.Add(m_arPoints[nIndex]);
									}
									int nSize = arPoints.GetSize();
									if (1 < nSize)
									{
										pPath->AddCurve(arPoints.m_aT, nSize);

										pointCur.X = arPoints[nSize - 1].X;
										pointCur.Y = arPoints[nSize - 1].Y;
									}
									arPoints.RemoveAll();
									break;
								}
							case rtClose:
								{
									/*CSimpleArray<PointF> arPoints;
									arPoints.Add(pointCur);
									arPoints.Add(pointMove);
									pPath->AddLines(arPoints.m_aT, 2);*/
									pPath->CloseFigure();
									break;
								}
							case rtRMoveTo:
								{
									if (0 < m_arPoints.GetSize())
									{
										pointMove.X = m_arPoints[0].X + pointCur.X;
										pointMove.Y = m_arPoints[0].Y + pointCur.Y;

										pointCur.X = pointMove.X;
										pointCur.Y = pointMove.Y;
										pPath->StartFigure();
									}
									break;
								}
							case rtRLineTo:
								{
									CSimpleArray<PointF> arPoints;
									arPoints.Add(pointCur);
									for (int nIndex = 0; nIndex < m_arPoints.GetSize(); ++nIndex)
									{
										arPoints.Add(m_arPoints[nIndex]);
									}
									for (int nIndex = 1; nIndex < arPoints.GetSize(); ++nIndex)
									{
										arPoints[nIndex].X += pointCur.X;
										arPoints[nIndex].Y += pointCur.Y;
									}
									int nSize = arPoints.GetSize();
									if (1 < nSize)
									{
										pPath->AddLines(arPoints.m_aT, nSize);

										pointCur.X = arPoints[nSize - 1].X;
										pointCur.Y = arPoints[nSize - 1].Y;
									}
									arPoints.RemoveAll();
									break;
								}
							case rtRCurveTo:
								{
									CSimpleArray<PointF> arPoints;
									arPoints.Add(pointCur);
									for (int nIndex = 0; nIndex < m_arPoints.GetSize(); ++nIndex)
									{
										arPoints.Add(m_arPoints[nIndex]);
									}
									for (int nIndex = 1; nIndex < arPoints.GetSize(); ++nIndex)
									{
										arPoints[nIndex].X += pointCur.X;
										arPoints[nIndex].Y += pointCur.Y;
									}
									int nSize = arPoints.GetSize();
									if (1 < nSize)
									{
										pPath->AddCurve(arPoints.m_aT, nSize);

										pointCur.X = arPoints[nSize - 1].X;
										pointCur.Y = arPoints[nSize - 1].Y;
									}
									arPoints.RemoveAll();
									break;
								}
							case rtAngleEllipseTo:
								{
									int nFigure = 0;
									while ((nFigure + 3) <= m_arPoints.GetSize())
									{
										REAL nLeft = m_arPoints[nFigure].X - m_arPoints[nFigure + 1].X / 2;
										REAL nTop = m_arPoints[nFigure].Y - m_arPoints[nFigure + 1].Y / 2;

										pPath->AddArc(nLeft, nTop,
											m_arPoints[nFigure + 1].X, m_arPoints[nFigure + 1].Y,
											m_arPoints[nFigure + 2].X, m_arPoints[nFigure + 2].Y);

										nFigure += 3;
									}
									CheckLastPoint(pPath, pointCur);
									break;
								}
							case rtAngleEllipse:
								{
									pPath->StartFigure();
									int nFigure = 0;
									while ((nFigure + 3) <= m_arPoints.GetSize())
									{
										REAL nLeft = m_arPoints[nFigure].X - m_arPoints[nFigure + 1].X / 2;
										REAL nTop = m_arPoints[nFigure].Y - m_arPoints[nFigure + 1].Y / 2;

										pPath->AddArc(nLeft, nTop,
											m_arPoints[nFigure + 1].X, m_arPoints[nFigure + 1].Y,
											m_arPoints[nFigure + 2].X, m_arPoints[nFigure + 2].Y);

										nFigure += 3;
									}
									CheckFirstPoint(pPath, pointMove);
									CheckLastPoint(pPath, pointCur);
									break;
								}
							case rtArc:
								{
									//pPath->CloseFigure();
									pPath->StartFigure();
									int nFigure = 0;
									while ((nFigure + 4) <= m_arPoints.GetSize())
									{
										REAL nCentreX = (m_arPoints[nFigure].X + m_arPoints[nFigure + 1].X) / 2;
										REAL nCentreY = (m_arPoints[nFigure].Y + m_arPoints[nFigure + 1].Y) / 2;

										REAL angleStart = GetAngle(nCentreX, nCentreY, 
											m_arPoints[nFigure + 2].X, m_arPoints[nFigure + 2].Y);

										REAL angleEnd = GetAngle(nCentreX, nCentreY, 
											m_arPoints[nFigure + 3].X, m_arPoints[nFigure + 3].Y);

										pPath->AddArc(m_arPoints[nFigure].X, m_arPoints[nFigure].Y,
											m_arPoints[nFigure + 1].X - m_arPoints[nFigure].X, 
											m_arPoints[nFigure + 1].Y - m_arPoints[nFigure].Y,
											angleStart, GetSweepAngle(angleStart, angleEnd));

										REAL r = GetSweepAngle(angleStart, angleEnd);
										if (abs(r) > 180)
										{
											int n = 5;
											n++;
										}

										nFigure += 4;
									}
									CheckFirstPoint(pPath, pointMove);
									CheckLastPoint(pPath, pointCur);
									break;
								}
							case rtArcTo:
								{
									int nFigure = 0;
									while ((nFigure + 4) <= m_arPoints.GetSize())
									{
										REAL nCentreX = (m_arPoints[nFigure].X + m_arPoints[nFigure + 1].X) / 2;
										REAL nCentreY = (m_arPoints[nFigure].Y + m_arPoints[nFigure + 1].Y) / 2;

										REAL angleStart = GetAngle(nCentreX, nCentreY, 
											m_arPoints[nFigure + 2].X, m_arPoints[nFigure + 2].Y);

										REAL angleEnd = GetAngle(nCentreX, nCentreY, 
											m_arPoints[nFigure + 3].X, m_arPoints[nFigure + 3].Y);

										pPath->AddArc(m_arPoints[nFigure].X, m_arPoints[nFigure].Y,
											m_arPoints[nFigure + 1].X - m_arPoints[nFigure].X, 
											m_arPoints[nFigure + 1].Y - m_arPoints[nFigure].Y,
											angleStart, GetSweepAngle(angleStart, angleEnd));

										nFigure += 4;
									}
									CheckLastPoint(pPath, pointCur);
									break;
								}
							case rtClockwiseArcTo:
								{
									int nFigure = 0;
									while ((nFigure + 4) <= m_arPoints.GetSize())
									{
										REAL nCentreX = (m_arPoints[nFigure].X + m_arPoints[nFigure + 1].X) / 2;
										REAL nCentreY = (m_arPoints[nFigure].Y + m_arPoints[nFigure + 1].Y) / 2;

										REAL angleStart = GetAngle(nCentreX, nCentreY, 
											m_arPoints[nFigure + 2].X, m_arPoints[nFigure + 2].Y);

										REAL angleEnd = GetAngle(nCentreX, nCentreY, 
											m_arPoints[nFigure + 3].X, m_arPoints[nFigure + 3].Y);

										pPath->AddArc(m_arPoints[nFigure].X, m_arPoints[nFigure].Y,
											m_arPoints[nFigure + 1].X - m_arPoints[nFigure].X, 
											m_arPoints[nFigure + 1].Y - m_arPoints[nFigure].Y,
											angleStart, 360 + GetSweepAngle(angleStart, angleEnd));

										nFigure += 4;
									}
									CheckLastPoint(pPath, pointCur);
									break;
								}
							case rtClockwiseArc:
								{
									//pPath->CloseFigure();
									pPath->StartFigure();
									int nFigure = 0;
									while ((nFigure + 4) <= m_arPoints.GetSize())
									{
										REAL nCentreX = (m_arPoints[nFigure].X + m_arPoints[nFigure + 1].X) / 2;
										REAL nCentreY = (m_arPoints[nFigure].Y + m_arPoints[nFigure + 1].Y) / 2;

										REAL angleStart = GetAngle(nCentreX, nCentreY, 
											m_arPoints[nFigure + 2].X, m_arPoints[nFigure + 2].Y);

										REAL angleEnd = GetAngle(nCentreX, nCentreY, 
											m_arPoints[nFigure + 3].X, m_arPoints[nFigure + 3].Y);

										pPath->AddArc(m_arPoints[nFigure].X, m_arPoints[nFigure].Y,
											m_arPoints[nFigure + 1].X - m_arPoints[nFigure].X, 
											m_arPoints[nFigure + 1].Y - m_arPoints[nFigure].Y,
											angleStart, 360 + GetSweepAngle(angleStart, angleEnd));

										nFigure += 4;
									}
									CheckFirstPoint(pPath, pointMove);
									CheckLastPoint(pPath, pointCur);
									break;
								}
							case rtEllipticalQuadrX:
								{
									bool bIsX = true;

									for (int nIndex = 0; nIndex < m_arPoints.GetSize(); ++nIndex)
									{
										REAL Width = 2 * (m_arPoints[nIndex].X - pointCur.X);
										REAL Height = 2 * (m_arPoints[nIndex].Y - pointCur.Y);

										REAL angleStart = -90;
										REAL angleSweet = 90;

										REAL Left = 0;
										REAL Top = 0;

										ApplyElliptical(bIsX, angleStart, angleSweet, Left, Top, Width, Height, pointCur);
										pPath->AddArc(Left, Top, Width, Height, angleStart, angleSweet);

										pointCur.X = m_arPoints[nIndex].X;
										pointCur.Y = m_arPoints[nIndex].Y;
									}
									break;
								}
							case rtEllipticalQuadrY:
								{
									bool bIsX = false;
									for (int nIndex = 0; nIndex < m_arPoints.GetSize(); ++nIndex)
									{
										REAL Width = 2 * (m_arPoints[nIndex].X - pointCur.X);
										REAL Height = 2 * (m_arPoints[nIndex].Y - pointCur.Y);

										REAL angleStart = 180;
										REAL angleSweet = -90;

										REAL Left = 0;
										REAL Top = 0;

										ApplyElliptical(bIsX, angleStart, angleSweet, Left, Top, Width, Height, pointCur);
										pPath->AddArc(Left, Top, Width, Height, angleStart, angleSweet);

										pointCur.X = m_arPoints[nIndex].X;
										pointCur.Y = m_arPoints[nIndex].Y;
									}
									break;
								}
							case rtQuadrBesier:
								{
									//     .
									//  ...
									CSimpleArray<PointF> arPoints;
									arPoints.Add(pointCur);
									for (int nIndex = 0; nIndex < m_arPoints.GetSize(); ++nIndex)
									{
										arPoints.Add(m_arPoints[nIndex]);
									}
									int nSize = arPoints.GetSize();
									if (1 < nSize)
									{
										pPath->AddLines(arPoints.m_aT, nSize);

										pointCur.X = arPoints[nSize - 1].X;
										pointCur.Y = arPoints[nSize - 1].Y;
									}
									arPoints.RemoveAll();
									break;
								}
							default: break;
							};
						}

						void Draw(IAVSRenderer* pRenderer, PointF& pointCur)
						{
							switch (m_eType)
							{
							case rtMoveTo:
								{
									if (0 < m_arPoints.GetSize())
									{
										pointCur.X = m_arPoints[0].X; pointCur.Y = m_arPoints[0].Y;
										pRenderer->PathCommandMoveTo(m_arPoints[0].X, m_arPoints[0].Y);
									}
									break;
								}
							case rtLineTo:
								{
									SAFEARRAY* pArray = NULL;
									GetSafearrayPoints(pRenderer, &pArray, pointCur);

									if (NULL != pArray)
									{
										pRenderer->PathCommandLinesTo(pArray);
									}

									RELEASEARRAY(pArray);
									break;
								}
							case rtCurveTo:
								{
									SAFEARRAY* pArray = NULL;
									GetSafearrayPoints(pRenderer, &pArray, pointCur);

									if (NULL != pArray)
									{
										pRenderer->PathCommandCurvesTo(pArray);
									}

									RELEASEARRAY(pArray);

									//CheckLastPoint(pRenderer, pointCur);
									break;
								}
							case rtClose:
								{
									pRenderer->PathCommandClose();
									break;
								}
							case rtRMoveTo:
								{
									if (0 < m_arPoints.GetSize())
									{
										pointCur.X = m_arPoints[0].X + pointCur.X;
										pointCur.Y = m_arPoints[0].Y + pointCur.Y;
										pRenderer->PathCommandMoveTo(pointCur.X, pointCur.Y);
									}
									break;
								}
							case rtRLineTo:
								{
									//  
									SAFEARRAY* pArray = NULL;
									GetSafearrayPoints(pRenderer, &pArray, pointCur, TRUE);

									if (NULL != pArray)
									{
										pRenderer->PathCommandLinesTo(pArray);
									}

									RELEASEARRAY(pArray);
									break;
								}
							case rtRCurveTo:
								{
									SAFEARRAY* pArray = NULL;
									GetSafearrayPoints(pRenderer, &pArray, pointCur, TRUE);

									PointF oPointNewCur;

									if (NULL != pArray)
									{
										pRenderer->PathCommandCurvesTo(pArray);
									}

									RELEASEARRAY(pArray);

									//CheckLastPoint(pRenderer, pointCur);
									break;
								}
							case rtAngleEllipseTo:
								{
									int nFigure = 0;
									while ((nFigure + 3) <= m_arPoints.GetSize())
									{
										REAL nLeft = m_arPoints[nFigure].X - m_arPoints[nFigure + 1].X / 2;
										REAL nTop = m_arPoints[nFigure].Y - m_arPoints[nFigure + 1].Y / 2;

										pRenderer->PathCommandArcTo(nLeft, nTop,
											m_arPoints[nFigure + 1].X, m_arPoints[nFigure + 1].Y,
											m_arPoints[nFigure + 2].X, m_arPoints[nFigure + 2].Y);

										nFigure += 3;
									}

									CheckLastPoint(pRenderer, pointCur);
									break;
								}
							case rtAngleEllipse:
								{
									pRenderer->PathCommandStart();
									int nFigure = 0;
									while ((nFigure + 3) <= m_arPoints.GetSize())
									{
										REAL nLeft = m_arPoints[nFigure].X - m_arPoints[nFigure + 1].X / 2;
										REAL nTop = m_arPoints[nFigure].Y - m_arPoints[nFigure + 1].Y / 2;

										pRenderer->PathCommandArcTo(nLeft, nTop,
											m_arPoints[nFigure + 1].X, m_arPoints[nFigure + 1].Y,
											m_arPoints[nFigure + 2].X, m_arPoints[nFigure + 2].Y);

										nFigure += 3;
									}

									CheckLastPoint(pRenderer, pointCur);
									break;
								}
							case rtArc:
								{
									pRenderer->PathCommandStart();
									int nFigure = 0;
									while ((nFigure + 4) <= m_arPoints.GetSize())
									{
										REAL nCentreX = (m_arPoints[nFigure].X + m_arPoints[nFigure + 1].X) / 2;
										REAL nCentreY = (m_arPoints[nFigure].Y + m_arPoints[nFigure + 1].Y) / 2;

										REAL angleStart = GetAngle(nCentreX, nCentreY, 
											m_arPoints[nFigure + 2].X, m_arPoints[nFigure + 2].Y);

										REAL angleEnd = GetAngle(nCentreX, nCentreY, 
											m_arPoints[nFigure + 3].X, m_arPoints[nFigure + 3].Y);

										pRenderer->PathCommandArcTo(m_arPoints[nFigure].X, m_arPoints[nFigure].Y,
											m_arPoints[nFigure + 1].X - m_arPoints[nFigure].X, 
											m_arPoints[nFigure + 1].Y - m_arPoints[nFigure].Y,
											angleStart, GetSweepAngle(angleStart, angleEnd));

										REAL r = GetSweepAngle(angleStart, angleEnd);
										nFigure += 4;
									}

									CheckLastPoint(pRenderer, pointCur);
									break;
								}
							case rtArcTo:
								{
									int nFigure = 0;
									while ((nFigure + 4) <= m_arPoints.GetSize())
									{
										REAL nCentreX = (m_arPoints[nFigure].X + m_arPoints[nFigure + 1].X) / 2;
										REAL nCentreY = (m_arPoints[nFigure].Y + m_arPoints[nFigure + 1].Y) / 2;

										REAL angleStart = GetAngle(nCentreX, nCentreY, 
											m_arPoints[nFigure + 2].X, m_arPoints[nFigure + 2].Y);

										REAL angleEnd = GetAngle(nCentreX, nCentreY, 
											m_arPoints[nFigure + 3].X, m_arPoints[nFigure + 3].Y);

										pRenderer->PathCommandArcTo(m_arPoints[nFigure].X, m_arPoints[nFigure].Y,
											m_arPoints[nFigure + 1].X - m_arPoints[nFigure].X, 
											m_arPoints[nFigure + 1].Y - m_arPoints[nFigure].Y,
											angleStart, GetSweepAngle(angleStart, angleEnd));

										nFigure += 4;
									}

									CheckLastPoint(pRenderer, pointCur);
									break;
								}
							case rtClockwiseArcTo:
								{
									int nFigure = 0;
									while ((nFigure + 4) <= m_arPoints.GetSize())
									{
										REAL nCentreX = (m_arPoints[nFigure].X + m_arPoints[nFigure + 1].X) / 2;
										REAL nCentreY = (m_arPoints[nFigure].Y + m_arPoints[nFigure + 1].Y) / 2;

										REAL angleStart = GetAngle(nCentreX, nCentreY, 
											m_arPoints[nFigure + 2].X, m_arPoints[nFigure + 2].Y);

										REAL angleEnd = GetAngle(nCentreX, nCentreY, 
											m_arPoints[nFigure + 3].X, m_arPoints[nFigure + 3].Y);

										pRenderer->PathCommandArcTo(m_arPoints[nFigure].X, m_arPoints[nFigure].Y,
											m_arPoints[nFigure + 1].X - m_arPoints[nFigure].X, 
											m_arPoints[nFigure + 1].Y - m_arPoints[nFigure].Y,
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
									while ((nFigure + 4) <= m_arPoints.GetSize())
									{
										REAL nCentreX = (m_arPoints[nFigure].X + m_arPoints[nFigure + 1].X) / 2;
										REAL nCentreY = (m_arPoints[nFigure].Y + m_arPoints[nFigure + 1].Y) / 2;

										REAL angleStart = GetAngle(nCentreX, nCentreY, 
											m_arPoints[nFigure + 2].X, m_arPoints[nFigure + 2].Y);

										REAL angleEnd = GetAngle(nCentreX, nCentreY, 
											m_arPoints[nFigure + 3].X, m_arPoints[nFigure + 3].Y);

										pRenderer->PathCommandArcTo(m_arPoints[nFigure].X, m_arPoints[nFigure].Y,
											m_arPoints[nFigure + 1].X - m_arPoints[nFigure].X, 
											m_arPoints[nFigure + 1].Y - m_arPoints[nFigure].Y,
											angleStart, 360 + GetSweepAngle(angleStart, angleEnd));

										nFigure += 4;
									}

									CheckLastPoint(pRenderer, pointCur);
									break;
								}
							case rtEllipticalQuadrX:
								{
									bool bIsX = true;

									for (int nIndex = 0; nIndex < m_arPoints.GetSize(); ++nIndex)
									{
										REAL Width = 2 * (m_arPoints[nIndex].X - pointCur.X);
										REAL Height = 2 * (m_arPoints[nIndex].Y - pointCur.Y);

										REAL angleStart = -90;
										REAL angleSweet = 90;

										REAL Left = 0;
										REAL Top = 0;

										ApplyElliptical(bIsX, angleStart, angleSweet, Left, Top, Width, Height, pointCur);
										pRenderer->PathCommandArcTo(Left, Top, Width, Height, angleStart, angleSweet);

										pointCur.X = m_arPoints[nIndex].X;
										pointCur.Y = m_arPoints[nIndex].Y;
									}
									break;
								}
							case rtEllipticalQuadrY:
								{
									bool bIsX = false;

									for (int nIndex = 0; nIndex < m_arPoints.GetSize(); ++nIndex)
									{
										REAL Width = 2 * (m_arPoints[nIndex].X - pointCur.X);
										REAL Height = 2 * (m_arPoints[nIndex].Y - pointCur.Y);

										REAL angleStart = 180;
										REAL angleSweet = -90;

										REAL Left = 0;
										REAL Top = 0;

										ApplyElliptical(bIsX, angleStart, angleSweet, Left, Top, Width, Height, pointCur);
										pRenderer->PathCommandArcTo(Left, Top, Width, Height, angleStart, angleSweet);

										pointCur.X = m_arPoints[nIndex].X;
										pointCur.Y = m_arPoints[nIndex].Y;
									}
									break;
								}
							case rtQuadrBesier:
								{
									//     .
									//  ...
									SAFEARRAY* pArray = NULL;
									GetSafearrayPoints(pRenderer, &pArray, pointCur);

									if (NULL != pArray)
									{
										pRenderer->PathCommandLinesTo(pArray);
									}

									RELEASEARRAY(pArray);

									CheckLastPoint(pRenderer, pointCur);
									break;
								}
							default: break;
							};
						}
					};


				public:

					CSimpleArray<CPart> m_arParts;
					int Metric;
					bool m_bFill;
					bool m_bStroke;

					double m_dWidthMM;
					double m_dHeightMM;

					double m_dAngle;
					CDoubleRect m_oBounds;
					LONG m_lFlags;

					CPen	Pen;
					CBrush	Brush;
				};

				class CDrawImageFromFile
				{
				public:

					virtual int InternalGetRequiredStreamSize()
					{
						return 5*sizeof(double) + 7*sizeof(int) + FilePath.GetLength() + 1;
					}
					/*virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(Left);
						pStream->WriteDouble(Top);
						pStream->WriteDouble(Right);
						pStream->WriteDouble(Bottom);
						pStream->WriteDouble(Angle);
						pStream->WriteLong(m_lFlags);
						pStream->WriteString(FilePath);
						pStream->WriteLong(BackColor);
						pStream->WriteLong(Alpha);
						pStream->WriteLong(nDrawImageScaleType);
						pStream->WriteLong(nDrawImageScaleBackColor);
					}*/
					/*virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Left = pStream->ReadDouble();
						Top = pStream->ReadDouble();
						Right = pStream->ReadDouble();
						Bottom = pStream->ReadDouble();
						Angle = pStream->ReadDouble();
						m_lFlags = pStream->ReadLong();
						FilePath = pStream->ReadString();
						BackColor = pStream->ReadLong();
						Alpha = pStream->ReadLong();
						nDrawImageScaleType = pStream->ReadLong();
						nDrawImageScaleBackColor = pStream->ReadLong();
					}
*/
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("left"), Left);
						oXmlWriter.WriteAttribute(_T("top"), Top);
						oXmlWriter.WriteAttribute(_T("angle"), Angle);
						oXmlWriter.WriteAttribute(_T("flags"), m_lFlags);
						oXmlWriter.WriteAttribute(_T("filepath"), FilePath);
						oXmlWriter.WriteAttribute(_T("backcolor"), BackColor);
						oXmlWriter.WriteAttribute(_T("alpha"), Alpha);
						oXmlWriter.WriteAttribute(_T("scaletype"), nDrawImageScaleType);
						oXmlWriter.WriteAttribute(_T("scalecolor"), nDrawImageScaleBackColor);

						if ( bAlign )
						{
							oXmlWriter.WriteAttribute(_T("aligntype"), nAlignType);
							oXmlWriter.WriteAttribute(_T("marginhorizontal"), nMarginX);
							oXmlWriter.WriteAttribute(_T("marginvertical"),   nMarginX);
						}
						switch(nSizeType)
						{
						case 0: 
							oXmlWriter.WriteAttribute(_T("bottom"), Bottom);
							oXmlWriter.WriteAttribute(_T("right"), Right);
							break;
						case 1:
							oXmlWriter.WriteAttribute(_T("width"), nWidth);
							oXmlWriter.WriteAttribute(_T("height"), nHeight);
							break;
						case 2:
							oXmlWriter.WriteAttribute(_T("width"), nWidth);
							break;
						case 3:
							oXmlWriter.WriteAttribute(_T("height"), nHeight);
							break;
						}

					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttribute(_T("metric"), _T("0")));
						Left = Strings::ToDouble(oXmlNode.GetAttribute(_T("left"), _T("0")));
						Top = Strings::ToDouble(oXmlNode.GetAttribute(_T("top"), _T("0")));
						Right = Strings::ToDouble(oXmlNode.GetAttribute(_T("right"), _T("0")));
						Bottom = Strings::ToDouble(oXmlNode.GetAttribute(_T("bottom"), _T("0")));
						Angle = Strings::ToDouble(oXmlNode.GetAttribute(_T("angle"), _T("0")));
						m_lFlags = Strings::ToInteger(oXmlNode.GetAttribute(_T("flags"), _T("0")));
						FilePath = oXmlNode.GetAttributeOrValue(_T("filepath"), _T(""));
						BackColor = Strings::ToInteger(oXmlNode.GetAttribute(_T("backcolor"), _T("-1")));
						Alpha = Strings::ToInteger(oXmlNode.GetAttribute(_T("alpha"), _T("255")));
						nDrawImageScaleType = Strings::ToInteger(oXmlNode.GetAttribute(_T("scaletype"), _T("-1")));
						nDrawImageScaleBackColor = Strings::ToInteger(oXmlNode.GetAttribute(_T("scalecolor"), _T("-1")));

						m_dWidthMM = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("widthmetric"), _T("210")));
						m_dHeightMM = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("heightmetric"), _T("190")));

						CString sAlign = oXmlNode.GetAttribute(_T("aligntype"));
						bAlign = FALSE;
						if ( sAlign.GetLength() > 0 )
							bAlign = TRUE;

						nAlignType = Strings::ToInteger(oXmlNode.GetAttribute(_T("aligntype"), _T("4")));
						nMarginX   = Strings::ToInteger(oXmlNode.GetAttribute(_T("marginhorizontal"), _T("10")));
						nMarginY   = Strings::ToInteger(oXmlNode.GetAttribute(_T("marginvertical"),   _T("10")));
						nWidth     = Strings::ToInteger(oXmlNode.GetAttribute(_T("width"),  _T("0")));
						nHeight    = Strings::ToInteger(oXmlNode.GetAttribute(_T("height"), _T("0")));

						CString sTemp = oXmlNode.GetAttribute(_T("right"));
						if ( sTemp.GetLength() < 1 )
							nSizeType = 1;

						sTemp = oXmlNode.GetAttribute(_T("bottom"));
						if ( sTemp.GetLength() < 1 )
							nSizeType = 1;

						if ( 1 == nSizeType )
						{
							sTemp = oXmlNode.GetAttribute(_T("height"));
							if ( sTemp.GetLength() < 1 )
								nSizeType = 2;

							sTemp = oXmlNode.GetAttribute(_T("width"));
							if ( 2!= nSizeType && sTemp.GetLength() < 1)
								nSizeType = 3;

							if ( 2 == nSizeType && sTemp.GetLength() < 1)
								nSizeType = 0;

						}

					}

					virtual void InternalClear()
					{
						//Metric = c_nMetricPixels;
						Metric = 0;

						Left = 0;
						Top = 0;
						Right = 0;
						Bottom = 0;
						Angle = 0;
						FilePath = _T("");

						m_lFlags = 0;

						nAlignType = 4;
						nSizeType = 0;
						bAlign = FALSE;
						nWidth = 0 ;
						nHeight = 0;

						nMarginX = 10;
						nMarginY = 10;

						BackColor = -1;
						Alpha = 255;

						nDrawImageScaleType = -1;
						nDrawImageScaleBackColor = -1;

						m_dWidthMM = 210;
						m_dHeightMM = 190;
					}

				public:

					CDrawImageFromFile()
					{
						InternalClear();
					}

					inline bool IsNeedDownload()
					{
						int n1 = FilePath.Find(_T("www"));
						int n2 = FilePath.Find(_T("http"));
						int n3 = FilePath.Find(_T("ftp"));

						if (((n1 >= 0) && (n1 < 10)) || ((n2 >= 0) && (n2 < 10)) || ((n3 >= 0) && (n3 < 10)))
							return true;
						return false;
					}

					BOOL Draw(IAVSRenderer* pRenderer, double dDPIX, double dDPIY)
					{
						if (NULL == pRenderer)
							return FALSE;

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

						VARIANT var;
						var.vt = VT_R8;
						var.dblVal = -1;
						pRenderer->GetAdditionalParam(L"DPIX", &var);

						if (-1 != var.dblVal)
						{
							DrawFromFile(pRenderer);
							RELEASEOBJECT(pDownloader);
							return TRUE;
						}

						pRenderer->put_Width((float)m_dWidthMM);
						pRenderer->put_Height((float)m_dHeightMM);

						LONG lType = this->GetFileType();

#ifdef _AVS_OFFICE_VIEWER_

						IUnknown* punkImage = NULL;

						if ((0 == lType) || (dDPIX < 0) || (dDPIY < 0))
						{
							ImageFile::IImageFile3* pImageFile = NULL;
							CoCreateInstance(ImageFile::CLSID_ImageFile3, NULL, CLSCTX_ALL, ImageFile::IID_IImageFile3, (void**)(&pImageFile));
							if (NULL == pImageFile)
							{
								return FALSE;
							}

							BSTR bstrFile = FilePath.AllocSysString();
							pImageFile->LoadImage2(bstrFile, &punkImage);
							SysFreeString(bstrFile);

							RELEASEINTERFACE(pImageFile);
						}
						else
						{
							LONG lPixX = (LONG)(dDPIX * (Right - Left) / 25.4);
							LONG lPixY = (LONG)(dDPIY * (Bottom - Top) / 25.4);

							Gdiplus::Bitmap oBitmap(lPixX, lPixY, PixelFormat32bppARGB);
							oBitmap.SetResolution((REAL)dDPIX, (REAL)dDPIY);

							Gdiplus::Graphics* pGraphics = Graphics::FromImage(&oBitmap);

							pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
							pGraphics->SetTextRenderingHint(TextRenderingHintAntiAlias);

							BSTR bstrFile = FilePath.AllocSysString();

							//if (1 == lType)
							if (TRUE)
							{
								Metafile oMeta(bstrFile);
								pGraphics->DrawImage(&oMeta, 0, 0, lPixX, lPixY);
							}
							else
							{
								HDC hDC = pGraphics->GetHDC();

								RECT oRectPlay;
								oRectPlay.left		= 0;
								oRectPlay.top		= 0;
								oRectPlay.right		= lPixX;
								oRectPlay.bottom	= lPixY;

								HENHMETAFILE hMeta = GetEnhMetaFile(bstrFile);

								PlayEnhMetaFile(hDC, hMeta, &oRectPlay);
								DeleteEnhMetaFile(hMeta);

								pGraphics->ReleaseHDC(hDC);
							}

							RELEASEOBJECT(pGraphics);
							SysFreeString(bstrFile);

							punkImage = CreateMediaDataFromBitmap(&oBitmap, lPixX, lPixY, FALSE);
						}						

						BOOL bRes = FALSE;
						if (NULL != punkImage)
						{
							pRenderer->SetCommandParams(Angle, Left, Top, Right - Left, Bottom - Top, m_lFlags);
							pRenderer->DrawImage(&punkImage, (float)Left, (float)Top, (float)(Right - Left), (float)(Bottom - Top));
							bRes = TRUE;
						}

						RELEASEINTERFACE(punkImage);

						FilePath = strOld;
						RELEASEOBJECT(pDownloader);
						return bRes;

#else
						FilePath = strOld;
						RELEASEOBJECT(pDownloader);
						return TRUE;
#endif
					}

					void DrawFromFile(IAVSRenderer* pRenderer)
					{
						BSTR bstrPath = FilePath.AllocSysString();

						pRenderer->DrawImageFromFile(bstrPath, (float)Left, (float)(Top), (float)(Right - Left), (float)(Bottom - Top));

						SysFreeString(bstrPath);
					}

				protected:

					LONG GetFileType()
					{						
						CFile oFile;
						if (S_OK != oFile.OpenFileRW(FilePath))
						{
							oFile.CloseFile();
							return 0;
						}

						DWORD dwSize = (DWORD)oFile.GetFileSize();

						if (44 > dwSize)
						{
							//    
							return 0;
						}

						DWORD dwKey = 0;
						oFile.ReadFile((BYTE*)(&dwKey), 4);

						if (0x9AC6CDD7 == dwKey)
						{
							// placeable meta
							oFile.CloseFile();
							return 1;
						}

						if (0x00000001 == dwKey)
						{
							oFile.SetPosition(40);
							oFile.ReadFile((BYTE*)(&dwKey), 4);

							if (0x464D4520 == dwKey)
							{
								// EMF/EMF+
								return 2;
							}
							else
							{
								// 
								return 0;
							}
						}

						if (0x00090001 == dwKey)
						{
							// standartmetafile
							oFile.SetPosition(0);

							BYTE* pHeader	= ToWMFHeader();
							BYTE* pData		= new BYTE[dwSize];

							oFile.ReadFile(pData, dwSize);

							oFile.SetPosition(0);
							oFile.WriteFile((void*)pHeader, 22);
							oFile.WriteFile((void*)pData, dwSize);

							oFile.CloseFile();

							RELEASEARRAYOBJECTS(pHeader);
							RELEASEARRAYOBJECTS(pData);

							return 1;
						}

						return 0;
					}

					BYTE* ToWMFHeader()
					{
						WmfPlaceableFileHeader oHeader;

						oHeader.Key = 0x9AC6CDD7;
						oHeader.Hmf = 0;

						double dKoef = 1440 / 25.4;

						oHeader.BoundingBox.Left = (short)(Left * dKoef);
						oHeader.BoundingBox.Top = (short)(Top * dKoef);
						oHeader.BoundingBox.Right = (short)(Right * dKoef);
						oHeader.BoundingBox.Bottom = (short)(Bottom * dKoef);

						oHeader.Inch = 1440; // 1:1
						oHeader.Reserved = 0;

						oHeader.Checksum = 0;
						oHeader.Checksum ^= (oHeader.Key & 0x0000FFFFL);
						oHeader.Checksum ^= ((oHeader.Key & 0xFFFF0000L) >> 16);

						oHeader.Checksum ^= oHeader.Hmf; 

						oHeader.Checksum ^= oHeader.BoundingBox.Left;
						oHeader.Checksum ^= oHeader.BoundingBox.Top; 
						oHeader.Checksum ^= oHeader.BoundingBox.Right;
						oHeader.Checksum ^= oHeader.BoundingBox.Bottom; 

						oHeader.Checksum ^= oHeader.Inch;
						oHeader.Checksum ^= (oHeader.Reserved & 0x0000FFFFL);
						oHeader.Checksum ^= ((oHeader.Reserved & 0xFFFF0000L) >> 16);

						LONG lLenHeader = 22;
						BYTE* pMetaHeader = new BYTE[lLenHeader];
						memcpy(pMetaHeader, (void*)(&oHeader), lLenHeader);

						return pMetaHeader;
					}

					IUnknown* CreateMediaDataFromBitmap(Bitmap* pImage, LONG nWidth, LONG nHeight, BOOL bFlip = FALSE)
					{
						if (NULL == pImage)
							return NULL;

						Gdiplus::Rect oRect(0, 0, nWidth, nHeight);
						Gdiplus::BitmapData oBitmapData;

						if (pImage->LockBits(&oRect, ImageLockModeRead, PixelFormat32bppARGB, &oBitmapData) != Ok)
							return NULL;

						MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;

						CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pMediaData));
						if (NULL == pMediaData)
							return FALSE;

						if (bFlip)
							pMediaData->put_ColorSpace(CSP_BGRA | CSP_VFLIP);
						else
							pMediaData->put_ColorSpace(CSP_BGRA);

						// specify settings
						pMediaData->put_Width(nWidth);
						pMediaData->put_Height(nHeight);
						pMediaData->put_AspectRatioX(nWidth);
						pMediaData->put_AspectRatioY(nHeight);
						pMediaData->put_Interlaced(VARIANT_FALSE);
						pMediaData->put_Stride(0, 4*nWidth);
						pMediaData->AllocateBuffer(4*nWidth*nHeight);

						// allocate necesasry buffer
						BYTE* pBufferPtr = 0;
						long nCreatedBufferSize = 0;
						pMediaData->get_Buffer(&pBufferPtr);
						pMediaData->get_BufferSize(&nCreatedBufferSize);
						pMediaData->put_Plane(0, pBufferPtr);

						// check for valid created buffer
						if (!pBufferPtr || nCreatedBufferSize != 4*nWidth*nHeight)
						{
							RELEASEINTERFACE(pMediaData);
							pImage->UnlockBits(&oBitmapData);
							return NULL;
						}

						// copy safearray's data to the buffer
						BYTE* pArray = (BYTE*)oBitmapData.Scan0;
						if (pArray != NULL)
							memcpy(pBufferPtr, pArray, nCreatedBufferSize);

						IUnknown* punkRes = NULL;
						pMediaData->QueryInterface(IID_IUnknown, (void**)&punkRes);
						RELEASEINTERFACE(pMediaData);

						pImage->UnlockBits(&oBitmapData);

						return punkRes;
					}

				public:

					int Metric;

					double Left, Top, Right, Bottom;
					double Angle;
					int BackColor;
					int Alpha;

					BOOL   bAlign;
					int    nAlignType;
					int    nSizeType;
					// 0 -  : left, top, right, bottom
					// 1 -  : left, top, width, height
					// 2 -  : left, top, width 
					// 3 -  : left, top, height 

					LONG m_lFlags;

					int    nMarginX;
					int    nMarginY;
					int    nWidth;
					int    nHeight;

					CString FilePath;

					int nDrawImageScaleType; 
					int nDrawImageScaleBackColor;

					double m_dWidthMM;
					double m_dHeightMM;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawImageFromFile (c_nSerializePaintDrawImageFromFile, c_strSerializePaintDrawImageFromFile)
					{
					int Metric = c_nMetricPixels;	// xml  - metri;  , . c_nMetric...
					CString FilePath = "";			// xml  - filepath;       

					// ,    (      - )
					double Left = 0;				// xml  - left
					double Top = 0;					// xml  - top
					double Right = 0;				// xml  - right
					double Bottom = 0;				// xml  - bottom

					// ,   ""   (..    - color key)
					int BackColor = -1;				// xml  - backcolor

					int nDrawImageScaleType;         // stretch, shrink, crop (. TransformConstants)
					int nDrawImageScaleBackColor;   //   BackColor  Resize
					}
					*/
				};
			}
		}
	}
}

#include "TextMeasurer/TextMeasurer.h"