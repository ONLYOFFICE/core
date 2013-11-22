#pragma once

#include <ImagePaintState.h>
#include <ImageCaching.h>
#include <Geometry.h>

namespace ImageStudio
{
	namespace Paint
	{
		namespace Text
		{
			class CWaypointAction
			{
			public:
				
				static const int c_nActionNone = 0;
				static const int c_nActionTranslate = 1;
				static const int c_nActionScale = 2;
				static const int c_nActionRotate = 3;
				static const int c_nActionShear = 4;
				
			public:
				
				CWaypointAction()
				{
					m_nType = c_nActionNone;
				}
				virtual ~CWaypointAction()
				{
				}
				
				CWaypointAction& operator= (const CWaypointAction& oAction)
				{
					m_nType = oAction.m_nType;

					memcpy(m_pValues, oAction.m_pValues, sizeof(float)*3);

					return *this;
				}
				
				int GetType() const
				{
					return m_nType;
				}
				float GetValue(int nIndex) const
				{
					if (nIndex < 0 || nIndex >= 3)
						return 0.0f;

					return m_pValues[nIndex];
				}
				
				void SetActionNone()
				{
					m_nType = c_nActionNone;
				}
				void SetActionTranslate(float fX, float fY)
				{
					m_nType = c_nActionTranslate;
					m_pValues[0] = fX;
					m_pValues[1] = fY;
				}
				void SetActionScale(float fX, float fY)
				{
					m_nType = c_nActionScale;
					m_pValues[0] = fX;
					m_pValues[1] = fY;
				}
				void SetActionRotate(float fX, float fY, float fAngle) 
				{
					m_nType = c_nActionRotate;
					m_pValues[0] = fX;
					m_pValues[1] = fY;
					m_pValues[2] = fAngle;
				}
				void SetActionShear(float fX, float fY)
				{
					m_nType = c_nActionShear;
					m_pValues[0] = fX;
					m_pValues[1] = fY;
				}
				
				void AddTransformToMatrix(Gdiplus::Matrix& oMatrix)
				{
					if (m_nType == c_nActionTranslate)
						oMatrix.Translate(m_pValues[0], m_pValues[1]);
					else if (m_nType == c_nActionScale)
						oMatrix.Scale(m_pValues[0], m_pValues[1]);
					else if (m_nType == c_nActionRotate)
					{
						Gdiplus::PointF oPointCenter(m_pValues[0], m_pValues[1]);

						if (fabs(oPointCenter.X) < 0.001 || fabs(oPointCenter.Y) < 0.001)
							oMatrix.Rotate(m_pValues[2]);
						else
							oMatrix.RotateAt(m_pValues[2], oPointCenter);
					}
					else if (m_nType == c_nActionShear)
						oMatrix.Shear(m_pValues[0], m_pValues[1]);
				}
				void AddTransformToMatrix(Gdiplus::Matrix& oMatrix, float fAdvancedRotateAngle)
				{
					if (m_nType == c_nActionTranslate)
						oMatrix.Translate(m_pValues[0], m_pValues[1]);
					else if (m_nType == c_nActionScale)
						oMatrix.Scale(m_pValues[0], m_pValues[1]);
					else if (m_nType == c_nActionRotate)
					{
						Gdiplus::PointF oPointCenter(m_pValues[0], m_pValues[1]);

						if (fabs(oPointCenter.X) < 0.001 || fabs(oPointCenter.Y) < 0.001)
							oMatrix.Rotate(m_pValues[2] + fAdvancedRotateAngle);
						else
							oMatrix.RotateAt(m_pValues[2] + fAdvancedRotateAngle, oPointCenter);
					}
					else if (m_nType == c_nActionShear)
						oMatrix.Shear(m_pValues[0], m_pValues[1]);
				}

				void Interpolate(CWaypointAction& oPoint1, CWaypointAction& oPoint2, double dDelta)
				{
					for (int index = 0; index < 3; ++index)
						m_pValues[index] = (float)(oPoint1.m_pValues[index] + (oPoint2.m_pValues[index] - oPoint1.m_pValues[index])*dDelta);
				}
				
			protected:
				
				int m_nType;
				float m_pValues[3];
			};
			class CWaypoint
			{
			public:

				CWaypoint()
				{ 
					m_bVisible = TRUE; 
					m_strName = ' ';
					m_nAlpha = 0;
				}
				virtual ~CWaypoint()
				{
					m_arrActions.RemoveAll();
				}
				
				CWaypoint& operator= (const CWaypoint& oWaypoint)
				{ 
					m_arrActions.RemoveAll(); 

					for (int index = 0; index < oWaypoint.m_arrActions.GetSize(); ++index)
						m_arrActions.Add(oWaypoint.m_arrActions[index]);

					m_bVisible = oWaypoint.m_bVisible;
					m_nAlpha = oWaypoint.m_nAlpha;
					m_strName = oWaypoint.m_strName;

					return *this;
				}
				CWaypointAction& operator[] (int nIndex)
				{ 
					return m_arrActions[nIndex];
				}
				int GetCount() const
				{
					return m_arrActions.GetSize();
				}
				
				WCHAR GetName() const
				{
					return m_strName;
				}
				WCHAR* GetNameBSTR()
				{
					return &m_strName;
				}
				void SetName(WCHAR strName)
				{
					m_strName = strName;
				}
				BOOL GetVisible() const
				{
					return m_bVisible;
				}
				void SetVisible(BOOL bVisible)
				{
					m_bVisible = bVisible;
				}
				int GetAlpha() const
				{
					return m_nAlpha;
				}
				void SetAlpha(int nAlpha)
				{
					m_nAlpha = nAlpha;
				}
				
				void Clear()
				{
					m_arrActions.RemoveAll();
				}
				void AppendActions(CWaypoint& oPoint)
				{
					for (int index = 0; index < oPoint.m_arrActions.GetSize(); ++index)
						m_arrActions.Add(oPoint.m_arrActions[index]);
				}
				void Translate(float fX, float fY) 
				{
					CWaypointAction oAction;

					oAction.SetActionTranslate(fX, fY);

					m_arrActions.Add(oAction);
				}
				void Scale(float fX, float fY) 
				{
					CWaypointAction oAction;

					oAction.SetActionScale(fX, fY);

					m_arrActions.Add(oAction);
				}
				void Rotate(float fX, float fY, float fAngle) 
				{
					CWaypointAction oAction;

					oAction.SetActionRotate(fX, fY, fAngle);

					m_arrActions.Add(oAction);
				}
				void Shear(float fX, float fY) 
				{
					CWaypointAction oAction;

					oAction.SetActionShear(fX, fY);

					m_arrActions.Add(oAction);
				}
				
				void AddTransformToGraphics(Graphics* pGraphics)
				{
					if (NULL == pGraphics)
						return;

					Gdiplus::Matrix oMatrix;

					for (int index = 0; index < m_arrActions.GetSize(); ++index)
						m_arrActions[index].AddTransformToMatrix(oMatrix);

					pGraphics->MultiplyTransform(&oMatrix);
				}
				void SetTransformToGraphics(Graphics* pGraphics, float fAdvancedRotateAngle)
				{
					if (NULL == pGraphics)
						return;

					Gdiplus::Matrix oMatrix;

					for (int index = 0; index < m_arrActions.GetSize(); ++index)
						m_arrActions[index].AddTransformToMatrix(oMatrix, fAdvancedRotateAngle);

					pGraphics->SetTransform(&oMatrix);
				}
				void Interpolate(CWaypoint& oPoint1, CWaypoint& oPoint2, double dDelta)
				{
					m_nAlpha = (BYTE)(oPoint1.m_nAlpha + (oPoint2.m_nAlpha - oPoint1.m_nAlpha)*dDelta);
					m_bVisible = (dDelta < 0.5) ? oPoint1.m_bVisible : oPoint2.m_bVisible;
					m_strName = (dDelta < 0.5) ? oPoint1.m_strName : oPoint2.m_strName;

					m_arrActions.RemoveAll();

					if (oPoint1.m_arrActions.GetSize() == oPoint2.m_arrActions.GetSize())
					{
						for (int index = 0; index < oPoint1.m_arrActions.GetSize(); ++index)
						{
							CWaypointAction oAction;

							oAction.Interpolate(oPoint1.m_arrActions[index], oPoint2.m_arrActions[index], dDelta);

							m_arrActions.Add(oAction);
						}
					}
				}
				
			protected:
				
				WCHAR m_strName;
				BOOL m_bVisible;
				BYTE m_nAlpha;
				CSimpleArray<CWaypointAction> m_arrActions;
			};
			class CWaypoints
			{
			public:
				
				CWaypoints()
				{
				}
				virtual ~CWaypoints()
				{
				}
				
				CWaypoints& operator= (const CWaypoints& oWaypoints)
				{
					m_arrWaypoints.RemoveAll(); 

					for (int index = 0; index < oWaypoints.m_arrWaypoints.GetSize(); ++index)
						m_arrWaypoints.Add(oWaypoints.m_arrWaypoints[index]);
				}
				CWaypoint& operator[] (int nIndex)
				{
					return m_arrWaypoints[nIndex];
				}
				int GetCount()
				{
					return m_arrWaypoints.GetSize();
				}
				
				void Add(const CWaypoint& oWaypoint)
				{
					m_arrWaypoints.Add(oWaypoint);
				}
				void Remove(int nIndex)
				{
					if (nIndex < 0 || nIndex >= m_arrWaypoints.GetSize())
						return;

					m_arrWaypoints.RemoveAt(nIndex);
				}
				void Clear()
				{
					m_arrWaypoints.RemoveAll();
				}
				
				BOOL GetWaypoint(double dFrame, CWaypoint& oPoint)
				{
					if (dFrame < 0 || dFrame > 1.0 || m_arrWaypoints.GetSize() < 1)
						return FALSE;
					
					// если точка только одна - ее координаты и возвращаем
					if (m_arrWaypoints.GetSize() < 2)
					{
						oPoint = m_arrWaypoints[0];
						return TRUE;
					}

					// find index of the nearest waypoint
					int nWaypointIndex = (int)floor((m_arrWaypoints.GetSize() - 1)*dFrame);

					// check that the waypoint index is correct
					if (nWaypointIndex < 0 || nWaypointIndex >= m_arrWaypoints.GetSize())
						return FALSE;
					if (nWaypointIndex == m_arrWaypoints.GetSize() - 1)
					{
						oPoint = m_arrWaypoints[nWaypointIndex];
						return TRUE;
					}

					// compute delta
					double dWaypointStep = 1.0/(m_arrWaypoints.GetSize() - 1);
					double dWaypointDelta = dFrame - nWaypointIndex*dWaypointStep;

					// interpolate waypoint
					oPoint.Interpolate(m_arrWaypoints[nWaypointIndex], m_arrWaypoints[nWaypointIndex + 1], dWaypointDelta/dWaypointStep);

					return TRUE;
				}
				
			protected:
				
				CSimpleArray<CWaypoint> m_arrWaypoints;
			};
			class CLetter
			{
			public:
				
				static const int c_nTypeIn = 0;
				static const int c_nTypeState = 1;
				static const int c_nTypeOut = 2;
				
			public:

				CLetter()
				{
				}
				virtual ~CLetter()
				{
				}
				
				CLetter& operator= (const CLetter& oLetter)
				{ 
					m_strName = oLetter.m_strName;
					m_nColor = oLetter.m_nColor;
					m_dFontSize = oLetter.m_dFontSize;
					m_oWaypoints[0] = oLetter.m_oWaypoints[0]; 
					m_oWaypoints[1] = oLetter.m_oWaypoints[1]; 
					m_oWaypoints[2] = oLetter.m_oWaypoints[2]; 

					return *this;
				}
				
				WCHAR GetName() const
				{
					return m_strName;
				}
				WCHAR* GetNameBSTR()
				{
					return &m_strName;
				}
				void SetName(WCHAR strName)
				{
					m_strName = strName;
				}
				int GetColor() const
				{
					return m_nColor;
				}
				void SetColor(int nColor)
				{
					m_nColor = nColor;
				}
				double GetFontSize() const
				{
					return m_dFontSize;
				}
				void SetFontSize(double dFontSize)
				{
					m_dFontSize = dFontSize;
				}
				CWaypoints& GetWaypoints(int nType)
				{
					return m_oWaypoints[nType];
				}
				Gdiplus::RectF GetRect()
				{
					return m_oRectLetter;
				}
				void SetRect(Gdiplus::RectF oRect)
				{
					m_oRectLetter = oRect;
				}
				
				BOOL GetWaypoint(int nType, double dFrame, CWaypoint& oPoint)
				{
					return m_oWaypoints[nType].GetWaypoint(dFrame, oPoint);
				}
				void SetColorAndSize(int nColor, double dFontSize)
				{
					m_nColor = nColor;
					m_dFontSize = dFontSize;
				}
				
			protected:
				
				WCHAR m_strName; // буква
				int m_nColor; // цвет буквы
				double m_dFontSize; // размер шрифта буквы
				CWaypoints m_oWaypoints[3]; // траектория полета буквы (0-прилет, 1-стояние, 2-улет)

				Gdiplus::RectF m_oRectLetter; // прямоугольник, описанный около буквы (при отрисовке в точке 0)
			};
			
			class CTextPainterOld : public ImageStudio::Caching::CCacheItemPointer
			{
			public:
				
				static const int c_nTypeDisplaceRandomMovements = 1;
				
			public:

				CTextPainterOld()
				{
					m_pPaintState = NULL;
					m_pTextureManager = NULL;
				}
				virtual ~CTextPainterOld()
				{
				}
				
				CPaintState* GetPaintState() const
				{
					return m_pPaintState;
				}
				void SetPaintState(CPaintState* pPaintState)
				{
					m_pPaintState = pPaintState;
				}
				Painter::CTextureManager* GetTextureManager() const
				{
					return m_pTextureManager;
				}
				void SetTextureManager(Painter::CTextureManager* pTextureManager)
				{
					m_pTextureManager = pTextureManager;
				}
				CString GetText() const
				{
					return m_strText;
				}
				Gdiplus::RectF GetTextRect() const
				{
					return m_oTextRect;
				}
				void SetTextRect(Gdiplus::RectF oRect)
				{
					m_oTextRect = oRect;
				}
				
				BOOL CreateText_StraightLine(const CString& strText, double dLeft, double dTop, double dStep)
				{
					if (NULL == m_pPaintState)
						return FALSE;

					CreateText_Empty(strText);

					for (int index = 0; index < m_strText.GetLength(); ++index)
					{
						CWaypoint oPoint;

						oPoint.SetName(m_arrLetters[index].GetName());
						oPoint.SetVisible(TRUE);
						oPoint.SetAlpha(m_pPaintState->GetBrush().GetAlpha1());

						float fLetterX = (float)(dLeft + dStep*index + m_pPaintState->GetFont().GetFontSize()/2);
						float fLetterY = (float)(dTop + m_pPaintState->GetFont().GetFontSize()/2);

						oPoint.Translate(fLetterX, fLetterY);
						oPoint.Rotate(0.0f, 0.0f, 0.0f); 
						oPoint.Scale(1.0f, 1.0f); 

						m_arrLetters[index].GetWaypoints(CLetter::c_nTypeState).Add(oPoint);
					}

					return TRUE;
				}
				
				BOOL CreateAnimation(BOOL bAnimationIn, int nType, int nAlpha)
				{
					if (NULL == m_pPaintState || m_arrLetters.GetSize() < 1)
						return FALSE;

					if (nType == c_nTypeDisplaceRandomMovements)
					{
						if (bAnimationIn)
							return CreateAnimation_RandomMovements_In(m_arrLetters, nAlpha);
					}

					return FALSE;
				}
				
				BOOL DrawText(Gdiplus::Graphics* pGraphics, int nWidth, int nHeight, double dFrame)
				{
					// dFrame изменяется в диапазоне [0..1..2..3]
					if (NULL == m_pPaintState)
						return FALSE;

					// подсчитываем, какого типа данные нам нужны...
					int nWaypointsType = (int)floor(dFrame);
					double dWaypointsFrame = dFrame - nWaypointsType;

					CWaypoint oPoint;
					
					// инициализируем формат для отрисовки букв
					Gdiplus::StringFormat oFormat; 
					oFormat.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap);
					oFormat.SetAlignment(Gdiplus::StringAlignmentCenter); 
					oFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
					oFormat.SetTrimming(Gdiplus::StringTrimmingNone);

					Gdiplus::Font* pFont = m_pPaintState->GetFont().GetFont();
				
					// отрисовываем тень
					if (m_pPaintState->GetShadow().GetVisible()) 
					{   		
						for (int index = 0; index < m_arrLetters.GetSize(); ++index)
						{
							if (!m_arrLetters[index].GetWaypoints(nWaypointsType).GetWaypoint(dWaypointsFrame, oPoint))
								continue;
							if (!oPoint.GetVisible())
								continue;

							pGraphics->ResetTransform();	
											
							oPoint.AddTransformToGraphics(pGraphics);
							
							// подсчитываем размеры буквы с текущими установками
							Gdiplus::RectF oRectLetter;
							pGraphics->MeasureString(m_arrLetters[index].GetNameBSTR(), 1, pFont, Gdiplus::PointF(0, 0), &oRectLetter);
							m_arrLetters[index].SetRect(oRectLetter);

							m_pPaintState->GetShadow().Draw(pGraphics, pFont, &oFormat, oPoint.GetNameBSTR(), 1, m_arrLetters[index].GetRect(), oPoint.GetAlpha());
						}
					}	

					// отрисовываем сами буквы
					for (int index = 0; index < m_arrLetters.GetSize(); ++index)
					{
						if (!m_arrLetters[index].GetWaypoints(nWaypointsType).GetWaypoint(dWaypointsFrame, oPoint))
							continue;
						if (!oPoint.GetVisible())
							continue;

						pGraphics->ResetTransform();	
										
						oPoint.AddTransformToGraphics(pGraphics);

						// отрисовываем границу буквы
						m_pPaintState->GetEdge().Draw(pGraphics, pFont, &oFormat, oPoint.GetNameBSTR(), 1, m_arrLetters[index].GetRect(), oPoint.GetAlpha());

						// отрисовываем букву в точке (0,0)
						if ((Painter::c_BrushTypePathGradient1 != m_pPaintState->GetBrush().GetType()) && 
							(Painter::c_BrushTypePathGradient2 != m_pPaintState->GetBrush().GetType()))
						{
							Gdiplus::Brush* pBrush = CreateLetterBrush(m_pTextureManager, &m_arrLetters[index].GetRect(), oPoint.GetAlpha());

							if (NULL != pBrush)
							{
								pGraphics->DrawString(oPoint.GetNameBSTR(), 1, pFont, m_arrLetters[index].GetRect(), &oFormat, pBrush);

								delete pBrush;
							}
						}

						pGraphics->Flush();
					}
					
					pGraphics->ResetTransform();

					return TRUE;
				}
				
			protected:
				
				void CreateText_Empty(const CString& strText)
				{
					m_strText = strText;

					m_arrLetters.RemoveAll();

					if (m_strText.GetLength() < 1)
						return;

					BSTR bstrText = m_strText.AllocSysString();

					for (int index = 0; index < m_strText.GetLength(); ++index)
					{
						CLetter oLetter;

						oLetter.SetName(bstrText[index]);
						oLetter.SetColor(m_pPaintState->GetBrush().GetColor1());
						oLetter.SetFontSize(m_pPaintState->GetFont().GetFontSize());

						m_arrLetters.Add(oLetter);
					}

					SysFreeString(bstrText);
				}
				
				BOOL CreateAnimation_RandomMovements_In(CSimpleArray<CLetter>& arrLetters, int nAlpha)
				{
					for (int index = 0; index < arrLetters.GetSize(); ++index)
					{
						CLetter* pLetter = &(arrLetters[index]);

						if (pLetter->GetWaypoints(CLetter::c_nTypeState).GetCount() < 1 ||
							pLetter->GetWaypoints(CLetter::c_nTypeState)[0].GetCount() < 1)
							return FALSE;

						CWaypoint oPointOld = pLetter->GetWaypoints(CLetter::c_nTypeState)[0];
						CWaypoint oPoint; 

						oPoint.SetName(pLetter->GetName());
						oPoint.SetVisible(TRUE);
						oPoint.SetAlpha(0);
						
						oPoint.Clear();
						oPoint.AppendActions(oPointOld);
						oPoint.Translate((float)(rand() % 300), (float)(rand() % 300));
						oPoint.Rotate(0.0f, 0.0f, (float)(rand() % 720));
						oPoint.Scale(1 + 0.1f*(rand() % 100), 1 + 0.1f*(rand() % 100));
						pLetter->GetWaypoints(CLetter::c_nTypeIn).Add(oPoint);

						float fResultX = pLetter->GetWaypoints(CLetter::c_nTypeState)[0][0].GetValue(0);
						float fResultY = pLetter->GetWaypoints(CLetter::c_nTypeState)[0][0].GetValue(1);

						oPoint.Clear();
						oPoint.AppendActions(oPointOld);
						oPoint.Translate(fResultX, fResultY);
						oPoint.Rotate(0.0f, 0.0f, 0.0f);
						oPoint.Scale(1.0f, 1.0f);
						oPoint.SetAlpha((BYTE)nAlpha);
						pLetter->GetWaypoints(CLetter::c_nTypeIn).Add(oPoint);
					}

					return TRUE;
				}
			
				Gdiplus::Brush* CreateLetterBrush(Painter::CTextureManager* pManager, Gdiplus::RectF* pRect, BYTE nAlpha)
				{
					if (NULL == m_pPaintState)
						return NULL;

					int nBrushType = m_pPaintState->GetBrush().GetType();

					// в случае текстурного браша - создаем его в кэше
					if (nBrushType == Painter::c_BrushTypeTexture && pManager)
					{
						// TODO:
						// int nTextureIndex = pManager->Find(m_pPaintState->GetBrush().GetTexturePath());
						// if (nTextureIndex >= 0)
						//	return GetTexureBrush(nTextureIndex, pManager, pRect, (BYTE)(m_brush.TextureAlpha*Alpha/255), m_brush.TextureMode);
					}

					// считаем цвета для других типов брашей с учетом указанного nAlpha
					Gdiplus::Color oColor1((BYTE)(m_pPaintState->GetBrush().GetAlpha1()*nAlpha/255), GetRValue(m_pPaintState->GetBrush().GetColor1()), GetGValue(m_pPaintState->GetBrush().GetColor1()), GetBValue(m_pPaintState->GetBrush().GetColor1()));
					Gdiplus::Color oColor2((BYTE)(m_pPaintState->GetBrush().GetAlpha2()*nAlpha/255), GetRValue(m_pPaintState->GetBrush().GetColor2()), GetGValue(m_pPaintState->GetBrush().GetColor2()), GetBValue(m_pPaintState->GetBrush().GetColor2()));

					// create hatch brush
					if (nBrushType >= Painter::c_BrushTypeHatch1 && nBrushType <= Painter::c_BrushTypeHatch53)
						return new HatchBrush((HatchStyle)(nBrushType - Painter::c_BrushTypeHatch1), oColor1, oColor2);

					// create gradient brush (Horizontal, Vertical, Diagonals)
					else if (nBrushType >= Painter::c_BrushTypeHorizontal && nBrushType <= Painter::c_BrushTypeDiagonal2 && pRect)
						return new Gdiplus::LinearGradientBrush(*pRect, oColor1, oColor2, (LinearGradientMode)(nBrushType - Painter::c_BrushTypeHorizontal));

					// create gradient brush (rect with center)
					else if (nBrushType == Painter::c_BrushTypeCenter && pRect)
					{
						Gdiplus::GraphicsPath oPath; oPath.AddRectangle(*pRect);

						Gdiplus::PathGradientBrush* pBrush = new Gdiplus::PathGradientBrush(&oPath);
						
						int nColorsCount = 1;

						pBrush->SetSurroundColors(&oColor1, &nColorsCount);
						pBrush->SetCenterColor(oColor2);

						return pBrush;
					}

					// create gradient brush (Cylinder Horizontal)
					else if (nBrushType == Painter::c_BrushTypeCylinderHor && pRect)
					{
						Gdiplus::LinearGradientBrush* pBrush = new Gdiplus::LinearGradientBrush(*pRect, oColor1, oColor2, -90.0f);
						
						pBrush->SetBlendBellShape(0.70f);

						return pBrush;
					}
					
					// create gradient brush (Cylinder Vertical)
					else if (nBrushType == Painter::c_BrushTypeCylinderVer && pRect)
					{
						Gdiplus::LinearGradientBrush* pBrush = new Gdiplus::LinearGradientBrush(*pRect, oColor1, oColor2, -180.0f);
						
						pBrush->SetBlendBellShape(0.70f);

						return pBrush;
					}

					// create solid brush
					return new Gdiplus::SolidBrush(oColor1);
				}
			
			protected:
				
				CPaintState* m_pPaintState; // настройки отрисовщика текста
				Painter::CTextureManager* m_pTextureManager; // настройки текстур для brush'ей

				CString m_strText; // текст для отрисовки
				Gdiplus::RectF m_oTextRect; // прямоугольник, в котором будет отрисовываться текст

				CSimpleArray<CLetter> m_arrLetters; // текст, по буквам
			};
			
		
			
			// типы анимаций
			const int c_nAnimationNone = 0;
			
			const int c_nAnimationAlphaBlendSectionBegin = 1;
			const int c_nAnimationAlphaBlendSimple = 1;         // простой fade на все буквы одновременно
			const int c_nAnimationAlphaBlendLetterAZ = 2;       // побуквенный fade слева на право
			const int c_nAnimationAlphaBlendLetterRandom = 3;   // плавный побуквенный fade в случайном порядке
			const int c_nAnimationAlphaBlendLetterAZSmooth = 4; // плавный побуквенный fade слева на право
			const int c_nAnimationAlphaBlendLetterRandomGlass = 5;   // плавный побуквенный fade в случайном порядке c эффектом Лупа
			const int c_nAnimationAlphaBlendLetterAZSmoothGlass = 6; // плавный побуквенный fade слева на право с эффектом Лупа
			const int c_nAnimationAlphaBlendLetterRandomScale = 7;   // плавный побуквенный fade в случайном порядке c увеличением
			const int c_nAnimationAlphaBlendLetterAZSmoothScale = 8; // плавный побуквенный fade слева на право с увеличением
			const int c_nAnimationAlphaBlendSectionEnd = 9;

			const int c_nAnimationAlphaSmoothSectionBegin = 80;
			const int c_nAnimationAlphaSmoothDualToCenter = 80;  // плавный побуквенный fade от краёв к центру
			const int c_nAnimationAlphaSmoothDualFromCenter = 81;// плавный побуквенный fade от центра к краям
			const int c_nAnimationAlphaSmoothSectionEnd = 89;

			const int c_nAnimationDropSectionBegin = 10;
			const int c_nAnimationDropLeft = 10;                // побуквенный drop слева с изменением прозрачности букв
			const int c_nAnimationDropTop = 11;                 // побуквенный drop сверху с изменением прозрачности букв
			const int c_nAnimationDropRight = 12;               // побуквенный drop справа с изменением прозрачности букв
			const int c_nAnimationDropBottom = 13;              // побуквенный drop снизу с изменением прозрачности букв
			const int c_nAnimationDropRotate1 = 20;             // побуквенный rotate против часовой стрелки с изменением прозрачности букв
			const int c_nAnimationDropRotate2 = 21;             // побуквенный rotate по часовой стрелке с изменением прозрачности букв
			const int c_nAnimationDropSectionEnd = 29;

			const int c_nAnimationMoveSectionBegin = 30;
			const int c_nAnimationMoveLeft = 30;                // движение текста влево
			const int c_nAnimationMoveTop = 31;                 // движение текста вверх
			const int c_nAnimationMoveRight = 32;               // движение текста вправо
			const int c_nAnimationMoveBottom = 33;              // движение текста вниз
			const int c_nAnimationMoveTopLeft = 34;             // движение текста из верхнего левого угла
			const int c_nAnimationMoveTopRight = 35;            // движение текста из верхнего правого угла
			const int c_nAnimationMoveBottomLeft = 36;          // движение текста из нижнего левого угла
			const int c_nAnimationMoveBottomRight = 37;         // движение текста из нижнего правого угла
			const int c_nAnimationMoveSectionEnd = 39;

			const int c_nAnimationScaleSectionBegin = 40;
			const int c_nAnimationScaleFromSmall = 40;               // увеличение до нормального размера (с альфой)
			const int c_nAnimationScaleFromLarge = 41;               // уменьшение до нормального размера (с альфой)
			const int c_nAnimationScaleFromXLargeYSmall = 42;        // уменьшение по Х и увеличение по Y (с альфой)
			const int c_nAnimationScaleFromXSmallYLarge = 43;        // увеличение по Х и уменьшение по Y (с альфой)
			const int c_nAnimationScaleFromSmallDropFromTop = 44;    // увеличение и падение сверху (с альфой)
			const int c_nAnimationScaleFromSmallDropFromBottom = 45; // увеличение и прилёт снизу (с альфой)
			const int c_nAnimationScaleSectionEnd = 49;

			const int c_nAnimationRotateSectionBegin = 50;
			const int c_nAnimationRotate1 = 50;              // вращение по часовой стрелке
			const int c_nAnimationRotate2 = 51;              // вращение против часовой стрелки
			const int c_nAnimationRotateFromSmall1 = 52;     // вращение по часовой стрелке с увеличением
			const int c_nAnimationRotateFromSmall2 = 53;     // вращение против часовой стрелки с увеличением
			const int c_nAnimationRotateFromLarge1 = 54;     // вращение по часовой стрелке с уменьшением
			const int c_nAnimationRotateFromLarge2 = 55;     // вращение против часовой стрелки с уменьшением
			const int c_nAnimationRotateSectionEnd = 59;

			const int c_nAnimationStretchSectionBegin = 60;
			const int c_nAnimationStretchScale = 60;           // побуквенное расширение
			const int c_nAnimationStretchHorScale = 61;        // побуквенное расширение по горизонтали
			const int c_nAnimationStretchVerScale = 62;        // побуквенное расширение по вертикали
			const int c_nAnimationStretchLVerScale = 63;       // побуквенное сжатие по вертикали
			const int c_nAnimationStretchLVerHorScale = 64;    // побуквенное растяжение по горизонтали и сжатие по вертикали
			
			const int c_nAnimationStretchSmall = 65;           // растяжение по горизонтали с альфой
			const int c_nAnimationStretchSmallScale = 66;      // растяжение по горизнотали с расширением букв по горизонтали и вертикали
			const int c_nAnimationStretchSmallHorScale = 67;   // растяжение по горизнотали с расширением по горизонтали
			const int c_nAnimationStretchSmallVerScale = 68;   // растяжение по горизнотали с расширением букв по вертикали
			const int c_nAnimationStretchSmallLargeVerScale = 69; // растяжение по горизнотали с сжатием букв по вертикали
			const int c_nAnimationStretchSmallLargeVerHorScale = 70; // растяжение по горизнотали с сжатием букв по вертикали и расширением по горизонтали
			const int c_nAnimationStretchLarge = 71;           // сжатие по горизонтали с альфой
			const int c_nAnimationStretchLargeScale = 72;      // сжатие по горизнотали с масштабированием
			const int c_nAnimationStretchLargeHorScale = 73;   // сжатие по горизнотали с расширением букв по горизонтали
			const int c_nAnimationStretchLargeVerScale = 74;   // сжатие по горизнотали с расширением букв по вертикали
			const int c_nAnimationStretchLargeLargeVerScale = 75; // сжатие по горизнотали с жатием букв по вертикали
			const int c_nAnimationStretchLargeLargeVerHorScale = 76; // сжатие по горизнотали с сжатием букв по вертикали и расширением по горизонтали
			
			const int c_nAnimationStretchSectionEnd = 79;

			class CLetterPermutation
			{
				int* m_pValues;
				int m_nCount;
			
			public:
				
				CLetterPermutation()
				{
					m_pValues = NULL;
					m_nCount = 0;
				}
				virtual ~CLetterPermutation()
				{
					Destroy();
				}
				
				void Destroy()
				{
					if (NULL != m_pValues)
					{
						delete[] m_pValues;
						m_pValues = NULL;
					}

					m_nCount = 0;
				}
				void Create(int nCount, BOOL bPermutate)
				{
					Destroy();

					if (nCount < 1)
						return;

					m_nCount = nCount;

					m_pValues = new int[nCount];

					for (int index = 0; index < nCount; ++index)
						m_pValues[index] = index;

					if (!bPermutate || nCount < 2)
						return;

					srand(GetTickCount());
					for (int index = 0; index < nCount; ++index)
					{
						int nIndexFrom = index;
						int nIndexTo = rand() % nCount;

						int nValue1 = m_pValues[nIndexFrom];
						int nValue2 = m_pValues[nIndexTo];

						m_pValues[nIndexFrom] = nValue2;
						m_pValues[nIndexTo] = nValue1;
					}						
				}
				
				int GetCount()
				{
					return m_nCount;
				}
				int operator[](int nIndex)
				{
					if (nIndex < 0 || nIndex >= m_nCount)
						return -1;

					return m_pValues[nIndex];
				}
			};
			class CTextTransform
			{
			public:
				
				static const int c_nTransformNone = 0;
				static const int c_nTransformTranslate = 1;
				static const int c_nTransformScale = 2;
				static const int c_nTransformRotate = 3;
				static const int c_nTransformShear = 4;
				static const int c_nTransformShadowColor = 5;
				static const int c_nTransformShadowPosition = 6;
				static const int c_nTransformShadowSize = 7;
				static const int c_nTransformEdgeColor = 8;
				static const int c_nTransformEdgeSize = 9;
				static const int c_nTransformTextColor1 = 10;
				static const int c_nTransformTextColor2 = 11;
				static const int c_nTransformMultiplyAlpha = 12;
				
			public:
				
				CTextTransform()
				{
					m_nType = c_nTransformNone;
				}
				CTextTransform(int nType, float fValue1 = 0.0f, float fValue2 = 0.0f, float fValue3 = 0.0f)
				{
					m_nType = nType;
					m_pValues[0] = fValue1;
					m_pValues[1] = fValue2;
					m_pValues[2] = fValue3;
				}
				virtual ~CTextTransform()
				{
				}
				
				CTextTransform& operator= (const CTextTransform& oTransform)
				{
					m_nType = oTransform.m_nType;

					memcpy(m_pValues, oTransform.m_pValues, sizeof(float)*3);

					return *this;
				}
				
				int GetType() const
				{
					return m_nType;
				}
				float GetValue(int nIndex) const
				{
					if (nIndex < 0 || nIndex >= 3)
						return 0.0f;

					return m_pValues[nIndex];
				}
				
				void SetTransformNone()
				{
					m_nType = c_nTransformNone;
				}
				void SetTransformTranslate(float fX, float fY)
				{
					m_nType = c_nTransformTranslate;
					m_pValues[0] = fX;
					m_pValues[1] = fY;
				}
				void SetTransformScale(float fX, float fY)
				{
					m_nType = c_nTransformScale;
					m_pValues[0] = fX;
					m_pValues[1] = fY;
				}
				void SetTransformRotate(float fX, float fY, float fAngle) 
				{
					m_nType = c_nTransformRotate;
					m_pValues[0] = fX;
					m_pValues[1] = fY;
					m_pValues[2] = fAngle;
				}
				void SetTransformShear(float fX, float fY)
				{
					m_nType = c_nTransformShear;
					m_pValues[0] = fX;
					m_pValues[1] = fY;
				}
				void SetTransformShadowColor(int nRGB, int nAlpha)
				{
					m_nType = c_nTransformShadowColor;
					m_pValues[0] = (float)nRGB;
					m_pValues[1] = (float)nAlpha;
				}
				void SetTransformShadowPosition(int nDistanceX, int nDistanceY)
				{
					m_nType = c_nTransformShadowPosition;
					m_pValues[0] = (float)nDistanceX;
					m_pValues[1] = (float)nDistanceY;
				}
				void SetTransformShadowSize(BOOL bVisible, int nBlurSize)
				{
					m_nType = c_nTransformShadowSize;
					m_pValues[0] = (float)(int)bVisible;
					m_pValues[1] = (float)nBlurSize;
				}
				void SetTransformEdgeColor(int nRGB, int nAlpha)
				{
					m_nType = c_nTransformEdgeColor;
					m_pValues[0] = (float)nRGB;
					m_pValues[1] = (float)nAlpha;
				}
				void SetTransformEdgeSize(BOOL bVisible, int nSize)
				{
					m_nType = c_nTransformEdgeSize;
					m_pValues[0] = (float)(int)bVisible;
					m_pValues[1] = (float)nSize;
				}
				void SetTransformTextColor1(int nRGB, int nAlpha)
				{
					m_nType = c_nTransformTextColor1;
					m_pValues[0] = (float)nRGB;
					m_pValues[1] = (float)nAlpha;
				}
				void SetTransformTextColor2(int nRGB, int nAlpha)
				{
					m_nType = c_nTransformTextColor2;
					m_pValues[0] = (float)nRGB;
					m_pValues[1] = (float)nAlpha;
				}
				void SetTransformMultiplyAlpha(float fAlphaKoef)
				{
					m_nType = c_nTransformMultiplyAlpha;
					m_pValues[0] = fAlphaKoef;
				}
				
				void AddTransformToMatrix(Gdiplus::Matrix& oMatrix)
				{
					if (m_nType == c_nTransformTranslate)
						oMatrix.Translate(m_pValues[0], m_pValues[1], Gdiplus::MatrixOrderAppend);
					else if (m_nType == c_nTransformScale)
						oMatrix.Scale(m_pValues[0], m_pValues[1], Gdiplus::MatrixOrderAppend);
					else if (m_nType == c_nTransformRotate)
					{
						Gdiplus::PointF oPointCenter(m_pValues[0], m_pValues[1]);

						if (fabs(oPointCenter.X) < 0.001 && fabs(oPointCenter.Y) < 0.001)
							oMatrix.Rotate(m_pValues[2], Gdiplus::MatrixOrderAppend);
						else
							oMatrix.RotateAt(m_pValues[2], oPointCenter, Gdiplus::MatrixOrderAppend);
					}
					else if (m_nType == c_nTransformShear)
						oMatrix.Shear(m_pValues[0], m_pValues[1], Gdiplus::MatrixOrderAppend);
				}
				void Interpolate(CTextTransform* pPoint1, CTextTransform* pPoint2, double dDelta)
				{
					if (NULL == pPoint1 || NULL == pPoint2 || pPoint1->m_nType != pPoint2->m_nType)
					{
						m_nType = c_nTransformNone;
						return;
					}

					m_nType = pPoint1->m_nType;

					if (m_nType == c_nTransformShadowColor ||
						m_nType == c_nTransformEdgeColor ||
						m_nType == c_nTransformTextColor1 ||
						m_nType == c_nTransformTextColor2)
					{
						if (pPoint1->m_pValues[0]>= 0 && pPoint2->m_pValues[0] >= 0)
						{
							int nRGB1 = (int)(pPoint1->m_pValues[0]);
							int nRGB2 = (int)(pPoint2->m_pValues[0]);

							int nR1 = GetRValue(nRGB1);
							int nG1 = GetGValue(nRGB1);
							int nB1 = GetBValue(nRGB1);

							int nR2 = GetRValue(nRGB2);
							int nG2 = GetGValue(nRGB2);
							int nB2 = GetBValue(nRGB2);

							int nR = (int)(nR1 + (nR2 - nR1)*dDelta);
							int nG = (int)(nG1 + (nG2 - nG1)*dDelta);
							int nB = (int)(nB1 + (nB2 - nB1)*dDelta);

							m_pValues[0] = (float)RGB(nR, nG, nB);
						}
						else
							m_pValues[0] = (dDelta < 0.5) ? pPoint1->m_pValues[0] : pPoint2->m_pValues[0];

						if (pPoint1->m_pValues[1] >= 0 && pPoint2->m_pValues[1] >= 0)
							m_pValues[1] = (float)(pPoint1->m_pValues[1] + (pPoint2->m_pValues[1] - pPoint1->m_pValues[1])*dDelta);
						else
							m_pValues[1] = (dDelta < 0.5) ? pPoint1->m_pValues[1] : pPoint2->m_pValues[1];
					}
					else
					{
						for (int index = 0; index < 3; ++index)
						{
							m_pValues[index] = (float)(pPoint1->m_pValues[index] + (pPoint2->m_pValues[index] - pPoint1->m_pValues[index])*dDelta);
						}
					}
				}
				
			protected:
				
				int m_nType;
				float m_pValues[3];
			};
			class CTextTransforms
			{
			public:
				
				CTextTransforms()
				{
				}
				virtual ~CTextTransforms()
				{
					RemoveAll();
				}
				
				CTextTransform* operator[] (int nIndex)
				{
					if (nIndex < 0 || nIndex >= GetCount())
						return NULL;

					return m_arrTransforms[nIndex];
				}
				CTextTransforms* operator= (CTextTransforms& oTransforms)
				{
					RemoveAll();

					for (int index = 0; index < oTransforms.GetCount(); ++index)
					{
						CTextTransform* pTransform = new CTextTransform();

						if (NULL == pTransform)
							return NULL;

						pTransform->operator =(*oTransforms[index]);

						Add(pTransform);
					}

					return this;
				}
				
				int GetCount()
				{
					return m_arrTransforms.GetSize();
				}
				
				void Add(CTextTransform* pTransform)
				{
					m_arrTransforms.Add(pTransform);
				}
				void Remove(int nIndex)
				{
					if (nIndex < 0 || nIndex >= GetCount())
						return;

					if (NULL != m_arrTransforms[nIndex])
					{
						delete m_arrTransforms[nIndex];
						m_arrTransforms[nIndex] = NULL;
					}

					m_arrTransforms.RemoveAt(nIndex);
				}
				void RemoveAll()
				{
					for (int index = 0; index < GetCount(); ++index)
					{
						if (NULL != m_arrTransforms[index])
						{
							delete m_arrTransforms[index];
							m_arrTransforms[index] = NULL;
						}
					}

					m_arrTransforms.RemoveAll();
				}
				
				void AddTransformToMatrix(Gdiplus::Matrix& oMatrix)
				{
					for (int index = 0; index < GetCount(); ++index)
						m_arrTransforms[index]->AddTransformToMatrix(oMatrix);
				}
				void Interpolate(CTextTransforms* pTransforms1, CTextTransforms* pTransforms2, double dDelta)
				{
					RemoveAll();

					if (NULL == pTransforms1 || NULL == pTransforms2)
						return;

					if (pTransforms1->GetCount() != pTransforms2->GetCount())
						return;

					for (int index = 0; index < pTransforms1->GetCount(); ++index)
					{
						CTextTransform* pTransform1 = pTransforms1->operator[](index);
						CTextTransform* pTransform2 = pTransforms2->operator[](index);

						CTextTransform* pTransform = new CTextTransform();

						pTransform->Interpolate(pTransform1, pTransform2, dDelta);

						m_arrTransforms.Add(pTransform);
					}
				}
				
				int FindTransform(int nType)
				{
					for (int index = GetCount() - 1; index >= 0; --index)
					{
						if (m_arrTransforms[index]->GetType() == nType)
							return index;
					}

					return -1;
				}
				
			protected:
				
				CSimpleArray<CTextTransform*> m_arrTransforms;
			};
			class CTextTransformsList
			{
			public:
				
				CTextTransformsList()
				{
				}
				virtual ~CTextTransformsList()
				{
					RemoveAll();
				}
				
				CTextTransforms* operator[] (int nIndex)
				{
					if (nIndex < 0 || nIndex >= GetCount())
						return NULL;

					return m_arrTransforms[nIndex];
				}
				
				int GetCount()
				{
					return m_arrTransforms.GetSize();
				}
				
				void Add(CTextTransforms* pTransform)
				{
					m_arrTransforms.Add(pTransform);
				}
				void Remove(int nIndex)
				{
					if (nIndex < 0 || nIndex >= GetCount())
						return;

					if (NULL != m_arrTransforms[nIndex])
					{
						delete m_arrTransforms[nIndex];
						m_arrTransforms[nIndex] = NULL;
					}

					m_arrTransforms.RemoveAt(nIndex);
				}
				void RemoveAll()
				{
					for (int index = 0; index < GetCount(); ++index)
					{
						if (NULL != m_arrTransforms[index])
						{
							delete m_arrTransforms[index];
							m_arrTransforms[index] = NULL;
						}
					}

					m_arrTransforms.RemoveAll();
				}
				
				void Interpolate(double dDelta, CTextTransforms& oTransforms)
				{
					oTransforms.RemoveAll();

					if (GetCount() < 1)
						return;

					int nIndexLeft = (int)floor((GetCount() - 1)*dDelta);

					if (nIndexLeft == GetCount() - 1)
					{
						CTextTransforms* pTransforms = m_arrTransforms[nIndexLeft];
						if (NULL != pTransforms)
							oTransforms = *pTransforms;
					}
					else
					{
						double dStep = 1.0/(GetCount() - 1);
						double dDeltaLocal = dDelta - nIndexLeft*dStep;

						oTransforms.Interpolate(m_arrTransforms[nIndexLeft], m_arrTransforms[nIndexLeft + 1], dDeltaLocal);
					}
				}
				void AddTransformToMatrix(Gdiplus::Matrix& oMatrix, double dDelta, CTextTransforms& oTransforms)
				{
					Interpolate(dDelta, oTransforms);

					for (int index = 0; index < oTransforms.GetCount(); ++index)
						oTransforms.AddTransformToMatrix(oMatrix);
				}
				
			protected:
				
				CSimpleArray<CTextTransforms*> m_arrTransforms;
			};
			class CTextPainterLetterTransform
			{
			public:
				
				CTextPainterLetterTransform()
				{
				}
				virtual ~CTextPainterLetterTransform()
				{
					m_oBefore.RemoveAll();
					m_oAfter.RemoveAll();
					m_oTransformsFadeIn.RemoveAll();
					m_oTransformsStatic.RemoveAll();
					m_oTransformsFadeOut.RemoveAll();
				}
				
				CTextTransforms& GetBefore()
				{
					return m_oBefore;
				}
				CTextTransforms& GetAfter()
				{
					return m_oAfter;
				}
				CTextTransformsList& GetTransformsFadeIn()
				{
					return m_oTransformsFadeIn;
				}
				CTextTransformsList& GetTransformsStatic()
				{
					return m_oTransformsStatic;
				}
				CTextTransformsList& GetTransformsFadeOut()
				{
					return m_oTransformsFadeOut;
				}
				
				void SetTransformToGraphics(Gdiplus::Graphics& oGraphics, double dAnimation, CTextTransforms& oTransforms)
				{
					// dAnimation принимает значения из промежутка [...0..1..2..3...]

					Gdiplus::Matrix oMatrix;

					// вначале выполняем все пред-треансформы (сдвиг в центр буквы и поворот на нужный градус)
					m_oBefore.AddTransformToMatrix(oMatrix);

					// далее выполняем трансформы, соответствующие определенной анимации
					if (dAnimation < 0)
						m_oTransformsFadeIn.AddTransformToMatrix(oMatrix, 0, oTransforms);
					else if (dAnimation < 1)
						m_oTransformsFadeIn.AddTransformToMatrix(oMatrix, dAnimation - 0.0, oTransforms);
					else if (dAnimation < 2)
						m_oTransformsStatic.AddTransformToMatrix(oMatrix, dAnimation - 1.0, oTransforms);
					else if (dAnimation < 3)
						m_oTransformsFadeOut.AddTransformToMatrix(oMatrix, dAnimation - 2.0, oTransforms);
					else // if (dAnimation >= 3)
						m_oTransformsFadeOut.AddTransformToMatrix(oMatrix, 1.0, oTransforms);

					// затем возвращаем букву на прежнее место
					m_oAfter.AddTransformToMatrix(oMatrix);
					
					oGraphics.ResetTransform();
					oGraphics.SetTransform(&oMatrix);
				}
				
			protected:

				CTextTransforms m_oBefore;
				CTextTransforms m_oAfter;
				CTextTransformsList m_oTransformsFadeIn;
				CTextTransformsList m_oTransformsStatic;
				CTextTransformsList m_oTransformsFadeOut;
			};
			class CTextPainterLetter
			{
			public:
				
				CTextPainterLetter()
				{
					m_pGraphicsPath = NULL;

					m_oRectBounds.X = -65001.0f;
					m_oRectBounds.Y = -65001.0f;
					m_oRectBounds.Width = 0.0f;
					m_oRectBounds.Height = 0.0f;
				}
				virtual ~CTextPainterLetter()
				{
					Destroy();
				}
				
				void Destroy()
				{
					if (NULL != m_pGraphicsPath)
					{
						delete m_pGraphicsPath;
						m_pGraphicsPath = NULL;
					}

					m_oRectBounds.X = -65001.0f;
					m_oRectBounds.Y = -65001.0f;
					m_oRectBounds.Width = 0.0f;
					m_oRectBounds.Height = 0.0f;
				}
				void Create(Gdiplus::GraphicsPath* pGraphicsPath, const Gdiplus::RectF& oRectBounds)
				{
					m_pGraphicsPath = pGraphicsPath;
					m_oRectBounds = oRectBounds;
				}
				
				Gdiplus::GraphicsPath* GetGraphicsPath()
				{
					return m_pGraphicsPath;
				}
				Gdiplus::RectF GetRectBounds()
				{
					return m_oRectBounds;
				}
				Gdiplus::PointF GetBoundsCenter()
				{
					Gdiplus::PointF oPoint( m_oRectBounds.X + m_oRectBounds.Width * 0.5f, m_oRectBounds.Y + m_oRectBounds.Height * 0.5f );
					return oPoint;
				}
				CTextPainterLetterTransform& GetTransform()
				{
					return m_oTransform;
				}
				
			protected:
				
				CTextPainterLetterTransform m_oTransform;

				Gdiplus::GraphicsPath* m_pGraphicsPath; // путь с содержимым буквы
				Gdiplus::RectF m_oRectBounds; // прямоугольник, описанный около буквы
			};
			class CTextPainterLetters
			{
			public:
				
				CTextPainterLetters()
				{
				}
				virtual ~CTextPainterLetters()
				{
					RemoveAll();
				}
				
				CTextPainterLetter* operator[] (int nIndex)
				{
					if (nIndex < 0 || nIndex >= GetCount())
						return NULL;

					return m_arrLetters[nIndex];
				}
				
				int GetCount()
				{
					return m_arrLetters.GetSize();
				}
				
				void Add(CTextPainterLetter* pLetter)
				{
					m_arrLetters.Add(pLetter);
				}
				void Remove(int nIndex)
				{
					if (nIndex < 0 || nIndex >= GetCount())
						return;

					if (NULL != m_arrLetters[nIndex])
					{
						delete m_arrLetters[nIndex];
						m_arrLetters[nIndex] = NULL;
					}

					m_arrLetters.RemoveAt(nIndex);
				}
				void RemoveAll()
				{
					for (int index = 0; index < GetCount(); ++index)
					{
						if (NULL != m_arrLetters[index])
						{
							delete m_arrLetters[index];
							m_arrLetters[index] = NULL;
						}
					}

					m_arrLetters.RemoveAll();
				}
				
				BOOL GetRectBounds(double& dLeft, double& dTop, double& dRight, double& dBottom)
				{
					if (GetCount() < 1)
						return FALSE;

					dLeft = m_arrLetters[0]->GetRectBounds().GetLeft();
					dTop = m_arrLetters[0]->GetRectBounds().GetTop();
					dRight = m_arrLetters[0]->GetRectBounds().GetRight();
					dBottom = m_arrLetters[0]->GetRectBounds().GetBottom();

					for (int index = 1; index < GetCount(); ++index)
					{
						if (NULL == m_arrLetters[index])
							continue;

						dLeft = min(dLeft, m_arrLetters[index]->GetRectBounds().GetLeft());
						dTop = min(dTop, m_arrLetters[index]->GetRectBounds().GetTop());
						dRight = max(dRight, m_arrLetters[index]->GetRectBounds().GetRight());
						dBottom = max(dBottom, m_arrLetters[index]->GetRectBounds().GetBottom());
					}

					return TRUE;
				}
				
			protected:
				
				CSimpleArray<CTextPainterLetter*> m_arrLetters;
			};
			class CTextPainterLine
			{
			public:

				CTextPainterLine()
				{
					m_nIndexStart = 0;
					m_nIndexEnd = 0;

					m_oRectBounds.X = -65001.0f;
					m_oRectBounds.Y = -65001.0f;
					m_oRectBounds.Width = 0.0f;
					m_oRectBounds.Height = 0.0f;
				}
				virtual ~CTextPainterLine()
				{
					Destroy();
				}
				
				void Destroy()
				{
					m_nIndexStart = 0;
					m_nIndexEnd = 0;

					m_oRectBounds.X = -65001.0f;
					m_oRectBounds.Y = -65001.0f;
					m_oRectBounds.Width = 0.0f;
					m_oRectBounds.Height = 0.0f;
				}
				
				int GetIndexStart() const
				{
					return m_nIndexStart;
				}
				void SetIndexStart(int nValue)
				{
					m_nIndexStart = nValue;
				}
				int GetIndexEnd() const
				{
					return m_nIndexEnd;
				}
				void SetIndexEnd(int nValue)
				{
					m_nIndexEnd = nValue;
				}
				Gdiplus::RectF& GetRectBounds()
				{
					return m_oRectBounds;
				}
				
			protected:
			
				int m_nIndexStart; // индекс буквы-пути, с которой начинается строка
				int m_nIndexEnd; // индекс буквы-пути, на которой закачивается строка
			
				Gdiplus::RectF m_oRectBounds; // прямоугольник, описанный около строки
			};
			class CTextPainterLines
			{
			public:
				
				CTextPainterLines()
				{
				}
				virtual ~CTextPainterLines()
				{
					RemoveAll();
				}
				
				CTextPainterLine* operator[] (int nIndex)
				{
					if (nIndex < 0 || nIndex >= GetCount())
						return NULL;

					return m_arrLines[nIndex];
				}
				
				int GetCount()
				{
					return m_arrLines.GetSize();
				}
				
				void Add(CTextPainterLine* pLine)
				{
					m_arrLines.Add(pLine);
				}
				void Remove(int nIndex)
				{
					if (nIndex < 0 || nIndex >= GetCount())
						return;

					if (NULL != m_arrLines[nIndex])
					{
						delete m_arrLines[nIndex];
						m_arrLines[nIndex] = NULL;
					}

					m_arrLines.RemoveAt(nIndex);
				}
				void RemoveAll()
				{
					for (int index = 0; index < GetCount(); ++index)
					{
						if (NULL != m_arrLines[index])
						{
							delete m_arrLines[index];
							m_arrLines[index] = NULL;
						}
					}

					m_arrLines.RemoveAll();
				}
				
				int FindLineByLetterIndex(int nIndex)
				{
					for (int index = 0; index < GetCount(); ++index)
					{
						if (NULL == m_arrLines[index])
							continue;

						if (nIndex < m_arrLines[index]->GetIndexStart() || nIndex > m_arrLines[index]->GetIndexEnd())
							continue;
						
						return index;
					}

					return -1;
				}
				BOOL GetRectBounds(double& dLeft, double& dTop, double& dRight, double& dBottom)
				{
					if (GetCount() < 1)
						return FALSE;

					dLeft = m_arrLines[0]->GetRectBounds().GetLeft();
					dTop = m_arrLines[0]->GetRectBounds().GetTop();
					dRight = m_arrLines[0]->GetRectBounds().GetRight();
					dBottom = m_arrLines[0]->GetRectBounds().GetBottom();

					for (int index = 1; index < GetCount(); ++index)
					{
						if (NULL == m_arrLines[index])
							continue;

						dLeft = min(dLeft, m_arrLines[index]->GetRectBounds().GetLeft());
						dTop = min(dTop, m_arrLines[index]->GetRectBounds().GetTop());
						dRight = max(dRight, m_arrLines[index]->GetRectBounds().GetRight());
						dBottom = max(dBottom, m_arrLines[index]->GetRectBounds().GetBottom());
					}

					return TRUE;
				}
				
			protected:
				
				CSimpleArray<CTextPainterLine*> m_arrLines;
			};
			class CTextPainter : public ImageStudio::Caching::CCacheItemPointer
			{
			public:
				
				CTextPainter()
				{
					m_pPaintState      = NULL;
					m_pTextureManager  = NULL;
					m_dTextLineSpacing = 0;
					m_bClipping        = FALSE;
					m_nHorAlign        = 0;
					m_nVerAlign        = 0;
				}
				virtual ~CTextPainter()
				{
					Destroy();
				}
				
				CTextPainterLetters& GetLetters()
				{
					return m_oLetters;
				}
				CPaintState* GetPaintState() const
				{
					return m_pPaintState;
				}
				void SetPaintState(CPaintState* pPaintState)
				{
					m_pPaintState = pPaintState;
				}
				Painter::CTextureManager* GetTextureManager() const
				{
					return m_pTextureManager;
				}
				void SetTextureManager(Painter::CTextureManager* pTextureManager)
				{
					m_pTextureManager = pTextureManager;
				}
								
				BOOL TextCreate(BSTR bstrText, int nTextLength, Gdiplus::Font& oFont)
				{
					return TextCreate(bstrText, nTextLength, oFont, 0, 0, 65000, 65000, Gdiplus::StringAlignmentNear, Gdiplus::StringAlignmentNear, FALSE); 
				}
				BOOL TextCreate(BSTR bstrText, int nTextLength, Gdiplus::Font& oFont, double dLeft, double dTop, double dRight, double dBottom, int nAlignVertical, int nAlignHorizontal, BOOL bClipping)
				{
					Destroy();

					// вначале просто создаем текст (подсчитываем все необходимые graphics paths)
					CSimpleArray<Gdiplus::GraphicsPath*> arrPaths;
					if (!GetTextPaths(bstrText, nTextLength, &oFont, dLeft, dTop, dRight, dBottom, nAlignVertical, nAlignHorizontal, arrPaths))
						return FALSE;

					// для каждого graphics path - создаем необходимые структуры для анимаций и трансформов
					// в arrPaths каждый graphics path описывает одну букву исходного текста
					for (int index = 0; index < arrPaths.GetSize(); ++index)
					{
						if (NULL == arrPaths[index])
							continue;

						Gdiplus::RectF oRectBounds;
						if (!GetPathBoundingRect(arrPaths[index], oRectBounds))
						{
							Gdiplus::GraphicsPath* pPath = arrPaths[index];
							delete pPath;
							arrPaths[index] = NULL;
							continue;
						}

						CTextPainterLetter* pLetter = new CTextPainterLetter();

						if (NULL == pLetter)
						{
							Gdiplus::GraphicsPath* pPath = arrPaths[index];
							delete pPath;
							arrPaths[index] = NULL;
							continue;
						}

						pLetter->Create(arrPaths[index], oRectBounds);

						m_oLetters.Add(pLetter);
					}

					// считаем межстрочный интервал для текста - чтобы правильно расположить его по вертикали
					if (TRUE)
					{
						Gdiplus::FontFamily oFontFamily; oFont.GetFamily(&oFontFamily);
						int nFontStyle = oFont.GetStyle();

						float fFontSize     = oFont.GetSize();
						int   nFontEmHeight = oFontFamily.GetEmHeight(nFontStyle);
						int   nLineSpacing  = oFontFamily.GetLineSpacing(nFontStyle);
						int   nCellAscent   = oFontFamily.GetCellAscent(nFontStyle);
						int   nCellDescent  = oFontFamily.GetCellDescent(nFontStyle);

						m_dTextLineSpacing = ((nFontEmHeight - (nCellAscent + nCellDescent)*0.5f)*0.34f) * fFontSize / nFontEmHeight;
						m_fTextFactorY     = (nLineSpacing - nCellAscent - nCellDescent)/float(nLineSpacing);
					}

					m_bClipping = bClipping;
					m_oClipRect.X = float(dLeft);
					m_oClipRect.Y = float(dTop);
					m_oClipRect.Width = float(dRight - dLeft);
					m_oClipRect.Height = float(dBottom - dTop);

					m_nHorAlign = nAlignHorizontal;
					m_nVerAlign = nAlignVertical;

					return TRUE;
				}
				
				void TextPlaceLine(double dLeft, double dTop)
				{
					// теперь реальные координаты каждого graphics path трансформируем в локальные (относительно центра этого path'а)
					// и результат подсчетов выставляем в position transform'ы (before и after трансформы) у каждой буквы
					for (int index = 0; index < m_oLetters.GetCount(); ++index)
					{
						CTextPainterLetter* pLetter = m_oLetters[index];
						if (NULL == pLetter)
							continue;

						Gdiplus::RectF oRectBounds = pLetter->GetRectBounds();

						// откат буквы в точку (0; 0) для дальнейших трансформов
						CTextTransforms& oTransformsBefore = pLetter->GetTransform().GetBefore();
						oTransformsBefore.RemoveAll();
						oTransformsBefore.Add(new CTextTransform(CTextTransform::c_nTransformTranslate, -(oRectBounds.X + 0.5f*oRectBounds.Width), -(oRectBounds.Y + 0.5f*oRectBounds.Height)));

						// возврат буквы в исходную позицию (после промежуточных трансформов)
						CTextTransforms& oTransformsAfter = pLetter->GetTransform().GetAfter();
						oTransformsAfter.RemoveAll();
						oTransformsAfter.Add(new CTextTransform(CTextTransform::c_nTransformTranslate, (float)dLeft + oRectBounds.X + 0.5f*oRectBounds.Width, (float)dTop + oRectBounds.Y + 0.5f*oRectBounds.Height));
					}

					TextUpdateBoundingBox();
				}
				void TextPlaceRectangle(double dLeft, double dTop, double dRight, double dBottom, int nAlignVertical, int nAlignHorizontal, double dRotateDegrees)
				{
					if( m_bClipping )
					{
						m_oClipRect.X = float(dLeft);
						m_oClipRect.Y = float(dTop);
					}

					if (m_oLetters.GetCount() < 1)
						return;

					// вначале надо задетектить все строки в тексте - чтобы потом можно было спокойно располагать все буквы как нам надо
					CTextPainterLines oLines;
					if (!GetTextLines(m_oLetters, oLines))
						return;

					// считаем bounding box для всего текста - нужно для выраснивания по высоте
					double dAllTextLeft = 0;
					double dAllTextTop = 0;
					double dAllTextRight = 0;
					double dAllTextBottom = 0;
					if (!oLines.GetRectBounds(dAllTextLeft, dAllTextTop, dAllTextRight, dAllTextBottom))
						return;

					double dRotateRadians = 3.1415926*dRotateDegrees/180.0;

					// теперь реальные координаты каждого graphics path трансформируем в локальные (относительно центра этого path'а)
					// и результат подсчетов выставляем в position transform'ы (before и after трансформы) у каждой буквы
					for (int index = 0; index < m_oLetters.GetCount(); ++index)
					{
						CTextPainterLetter* pLetter = m_oLetters[index];
						if (NULL == pLetter)
							continue;

						Gdiplus::RectF oRectBounds = pLetter->GetRectBounds();

						// откат буквы в точку (0; 0) для дальнейших трансформов
						CTextTransforms& oTransformsBefore = pLetter->GetTransform().GetBefore();
						oTransformsBefore.RemoveAll();
						oTransformsBefore.Add(new CTextTransform(CTextTransform::c_nTransformTranslate, -(oRectBounds.X + 0.5f*oRectBounds.Width), -(oRectBounds.Y + 0.5f*oRectBounds.Height)));

						// возврат буквы в исходную позицию (после промежуточных трансформов)
						CTextTransforms& oTransformsAfter = pLetter->GetTransform().GetAfter();
						oTransformsAfter.RemoveAll();

						int nLineIndex = oLines.FindLineByLetterIndex(index);
						if (nLineIndex < 0)
						{
							// этого случая не должно быть, т.к. все буквы должны быть распределены по своим линиям
							oTransformsAfter.Add(new CTextTransform(CTextTransform::c_nTransformTranslate, (float)dLeft + oRectBounds.X + 0.5f*oRectBounds.Width, (float)dTop + oRectBounds.Y + 0.5f*oRectBounds.Height));
							continue;
						}

						// считаем смещения текста с учетом alignment'а
						double dLetterX = 0; //oRectBounds.X + 0.5f*oRectBounds.Width;
						double dLetterY = 0; //oRectBounds.Y + 0.5f*oRectBounds.Height;
						double dHeight  = dBottom - dTop;

						//if (nAlignHorizontal == Gdiplus::StringAlignmentNear)
						//{
							dLetterX = dLeft + 0.5f*oRectBounds.Width + oRectBounds.X;
						//}
						//else if (nAlignHorizontal == Gdiplus::StringAlignmentFar)
						//{
						//	double dOffsetX = dRight - oLines[nLineIndex]->GetRectBounds().Width;

						//	dLetterX = dOffsetX + 0.5f*oRectBounds.Width + oRectBounds.X - oLines[nLineIndex]->GetRectBounds().GetLeft();
						//}
						//else // if (nAlignHorizontal == Gdiplus::StringAlignmentCenter)
						//{
						//	double dOffsetX = 0.5*(dRight + dLeft) - 0.5*oLines[nLineIndex]->GetRectBounds().Width;

						//	dLetterX = dOffsetX + 0.5f*oRectBounds.Width + oRectBounds.X - oLines[nLineIndex]->GetRectBounds().GetLeft();
						//}

						if (nAlignVertical == Gdiplus::StringAlignmentNear)
						{
							//dLetterY = dTop + 0.5f*oRectBounds.Height + oRectBounds.Y;
							double dOffsetY = 0.5f*oRectBounds.Height + oRectBounds.Y; //+ m_dTextLineSpacing;
							dLetterY = dTop + dOffsetY - dOffsetY * m_fTextFactorY;
						}
						else if (nAlignVertical == Gdiplus::StringAlignmentFar)
						{
							double dOffsetY = 0.5f*oRectBounds.Height + oRectBounds.Y + m_dTextLineSpacing;
							dLetterY = dTop + dOffsetY + (dHeight - dOffsetY) * m_fTextFactorY;
						}
						else // if (nAlignVertical == Gdiplus::StringAlignmentCenter)
						{
							double dOffsetY = 0.5f*oRectBounds.Height + oRectBounds.Y + m_dTextLineSpacing;
							dLetterY = dTop + dOffsetY - dOffsetY * m_fTextFactorY;
						}

						// теперь надо расчитать поворот... мдя..
						if (fabs(dRotateDegrees) < 0.001)
							oTransformsAfter.Add(new CTextTransform(CTextTransform::c_nTransformTranslate, (float)dLetterX, (float)dLetterY));
						else
						{
							Geometry::RotatePoint(dLetterX, dLetterY, 0.5*(dLeft + dRight), 0.5*(dTop + dBottom), dRotateRadians);

							oTransformsAfter.Add(new CTextTransform(CTextTransform::c_nTransformRotate, 0, 0, (float)dRotateDegrees));
							oTransformsAfter.Add(new CTextTransform(CTextTransform::c_nTransformTranslate, (float)dLetterX, (float)dLetterY));
						}
					}

					TextUpdateBoundingBox();
				}
				void TextPlaceEllipse(double dLeft, double dTop, double dRight, double dBottom, double dAngleStart)
				{
					double dEllipseRadiusX = 0.5*(dRight - dLeft);
					double dEllipseRadiusY = 0.5*(dBottom - dTop);

					int nPointsCount = (int)(dEllipseRadiusX + dEllipseRadiusY);
					
					if (nPointsCount < 2)
						return;

					double dEllipseCenterX = 0.5*(dLeft + dRight);
					double dEllipseCenterY = 0.5*(dTop + dBottom);

					CSimpleArray<double> arrPoints;

					for (int index = 0; index < nPointsCount; ++index)
					{
						double dPointAngle = 6.2831852*(index/(double)(nPointsCount - 1) + dAngleStart/360);

						arrPoints.Add(dEllipseCenterX + dEllipseRadiusX*cos(dPointAngle));
						arrPoints.Add(dEllipseCenterY + dEllipseRadiusY*sin(dPointAngle));
					}

					TextPlacePolyline(arrPoints);
				}
				void TextPlacePolyline(CSimpleArray<double>& arrPoints)
				{
					Geometry::CPolylineManager oPolyline;

					if (!oPolyline.Create(arrPoints))
						return;

					// вначале считаем bounding box для всех букв - это нам надо для того, чтобы правильно расположить буквы на эллепсе
					double dRectBoundingBoxLeft = 0.0f;
					double dRectBoundingBoxTop = 0.0f;
					double dRectBoundingBoxRight = 0.0f;
					double dRectBoundingBoxBottom = 0.0f;
					for (int index = 0; index < m_oLetters.GetCount(); ++index)
					{
						CTextPainterLetter* pLetter = m_oLetters[index];
						if (NULL == pLetter)
							continue;

						if ((dRectBoundingBoxRight - dRectBoundingBoxLeft) < 1.0f || (dRectBoundingBoxBottom - dRectBoundingBoxTop) < 1.0f)
						{
							dRectBoundingBoxLeft = pLetter->GetRectBounds().GetLeft();
							dRectBoundingBoxTop = pLetter->GetRectBounds().GetTop();
							dRectBoundingBoxRight = pLetter->GetRectBounds().GetRight();
							dRectBoundingBoxBottom = pLetter->GetRectBounds().GetBottom();
						}
						else
						{
							dRectBoundingBoxLeft = min(dRectBoundingBoxLeft, pLetter->GetRectBounds().GetLeft());
							dRectBoundingBoxTop = min(dRectBoundingBoxTop, pLetter->GetRectBounds().GetTop());
							dRectBoundingBoxRight = max(dRectBoundingBoxRight, pLetter->GetRectBounds().GetRight());
							dRectBoundingBoxBottom = max(dRectBoundingBoxBottom, pLetter->GetRectBounds().GetBottom());
						}
					}
					Gdiplus::RectF oRectBoundingBox((float)dRectBoundingBoxLeft, (float)dRectBoundingBoxTop, (float)(dRectBoundingBoxRight - dRectBoundingBoxLeft), (float)(dRectBoundingBoxBottom - dRectBoundingBoxTop));

					// теперь реальные координаты каждого graphics path трансформируем в локальные (относительно центра этого path'а)
					// и результат подсчетов выставляем в position transform'ы (before и after трансформы) у каждой буквы
					for (int index = 0; index < m_oLetters.GetCount(); ++index)
					{
						CTextPainterLetter* pLetter = m_oLetters[index];
						if (NULL == pLetter)
							continue;

						Gdiplus::RectF oRectBounds = pLetter->GetRectBounds();

						// откат буквы в точку (0; 0) для дальнейших трансформов
						CTextTransforms& oTransformsBefore = pLetter->GetTransform().GetBefore();
						oTransformsBefore.RemoveAll();
						oTransformsBefore.Add(new CTextTransform(CTextTransform::c_nTransformTranslate, -(oRectBounds.X + 0.5f*oRectBounds.Width), -(oRectBounds.Y + 0.5f*oRectBounds.Height)));

						// возврат буквы в исходную позицию (после промежуточных трансформов)
						CTextTransforms& oTransformsAfter = pLetter->GetTransform().GetAfter();
						oTransformsAfter.RemoveAll();

						// считаем свиг буквы относительно общего bounding box'а
						double dLetterOffsetX = oRectBounds.X + 0.5*oRectBounds.Width;
						double dLetterOffsetY = oRectBounds.Y + 0.5*oRectBounds.Height;

						// а теперь в длинах polyline относительно первой точки polylin'а
						double dLetterPolyOffset1 = oRectBounds.X;
						double dLetterPolyOffset2 = oRectBounds.X + oRectBounds.Width;
						if (dLetterPolyOffset1 > oPolyline.GetTotalLength() || dLetterPolyOffset2 > oPolyline.GetTotalLength())
						{
							// буквы не уместившиеся на полилайне отрисовываются в его хвосте всем скопом...
							dLetterPolyOffset1 = max(0, oPolyline.GetTotalLength() - oRectBounds.Width);
							dLetterPolyOffset2 = oPolyline.GetTotalLength();
						}

						// находим отрезок "под" буквой (на полилайне) - пару точек, соответствующих x-границам буквы
						double dLetterPoint1X, dLetterPoint1Y, dLetterPoint2X, dLetterPoint2Y;
						if (!oPolyline.GetPointByLength(dLetterPolyOffset1, dLetterPoint1X, dLetterPoint1Y) || !oPolyline.GetPointByLength(dLetterPolyOffset2, dLetterPoint2X, dLetterPoint2Y))
						{
							// сюда вообще не должны попасть... но если что,
							// берем крайние точки полилайна в качестве образующего отрезка
							// надо же где-то буквы отрисовать
							dLetterPoint1X = oPolyline.GetPointsData()[0];
							dLetterPoint1Y = oPolyline.GetPointsData()[1];
							dLetterPoint2X = oPolyline.GetPointsData()[2*(oPolyline.GetPointsCount() - 1) + 0];
							dLetterPoint2Y = oPolyline.GetPointsData()[2*(oPolyline.GetPointsCount() - 1) + 1];
						}

						double dLetterPointsAngle = Geometry::GetAngle(dLetterPoint1X, dLetterPoint1Y, dLetterPoint2X, dLetterPoint2Y);

						// находим нормаль к отрезку под буквой (чтобы сдвинуть центр буквы вдоль этой нормали)
						double dNormalX =  (dLetterPoint2Y - dLetterPoint1Y);
						double dNormalY = -(dLetterPoint2X - dLetterPoint1X);
						double dNormalLength = max(0.001, _hypot(dNormalX, dNormalY));
						dNormalX /= dNormalLength;
						dNormalY /= dNormalLength;

						// считаем новые координаты центра буквы (с учетом нормали)
						double dLetterPointX = 0.5*(dLetterPoint1X + dLetterPoint2X) + dNormalX*(oRectBoundingBox.GetBottom() - dLetterOffsetY);
						double dLetterPointY = 0.5*(dLetterPoint1Y + dLetterPoint2Y) + dNormalY*(oRectBoundingBox.GetBottom() - dLetterOffsetY);

						oTransformsAfter.Add(new CTextTransform(CTextTransform::c_nTransformRotate, 0, 0, (float)(180*dLetterPointsAngle/3.1415)));
						oTransformsAfter.Add(new CTextTransform(CTextTransform::c_nTransformTranslate, (float)dLetterPointX, (float)dLetterPointY));
					}

					TextUpdateBoundingBox();
				}
				
				void TextTransform(int nAnimationFadeIn, int nAnimationState, int nAnimationFadeOut, float fAlphaFadeFactorMin, float fAlphaFadeFactorMax)
				{
					m_fAlphaFadeFactorMin = fAlphaFadeFactorMin;
					m_fAlphaFadeFactorMax = fAlphaFadeFactorMax;

					if (nAnimationFadeIn >= c_nAnimationAlphaBlendSectionBegin && 
						nAnimationFadeIn <= c_nAnimationAlphaBlendSectionEnd )
						SetAnimationAlphaBlendFadeIn( nAnimationFadeIn );

					if (nAnimationFadeOut >= c_nAnimationAlphaBlendSectionBegin && 
						nAnimationFadeOut <= c_nAnimationAlphaBlendSectionEnd)
						SetAnimationAlphaBlendFadeOut( nAnimationFadeOut );

					if (nAnimationFadeIn >= c_nAnimationAlphaSmoothSectionBegin && 
						nAnimationFadeIn <= c_nAnimationAlphaSmoothSectionEnd )
						SetAnimationAlphaSmoothFadeIn( nAnimationFadeIn );

					if (nAnimationFadeOut >= c_nAnimationAlphaSmoothSectionBegin && 
						nAnimationFadeOut <= c_nAnimationAlphaSmoothSectionEnd)
						SetAnimationAlphaSmoothFadeOut( nAnimationFadeOut );
					
					if (nAnimationFadeIn >= c_nAnimationDropSectionBegin &&
						nAnimationFadeIn <= c_nAnimationDropSectionEnd)
						SetAnimationAlphaBlendDropFadeIn( nAnimationFadeIn );

					if (nAnimationFadeOut >= c_nAnimationDropSectionBegin &&
						nAnimationFadeOut <= c_nAnimationDropSectionEnd)
						SetAnimationAlphaBlendDropFadeOut( nAnimationFadeOut );

					if (nAnimationFadeIn >= c_nAnimationMoveSectionBegin &&
						nAnimationFadeIn <= c_nAnimationMoveSectionEnd)
						SetAnimationMoveToFadeIn( nAnimationFadeIn );

					if (nAnimationFadeOut >= c_nAnimationMoveSectionBegin &&
						nAnimationFadeOut <= c_nAnimationMoveSectionEnd)
						SetAnimationMoveToFadeOut( nAnimationFadeOut );

					if (nAnimationFadeIn >= c_nAnimationScaleSectionBegin &&
						nAnimationFadeIn <= c_nAnimationScaleSectionEnd)
						SetAnimationScaleFadeIn( nAnimationFadeIn );

					if (nAnimationFadeOut >= c_nAnimationScaleSectionBegin &&
						nAnimationFadeOut <= c_nAnimationScaleSectionEnd)
						SetAnimationScaleFadeOut( nAnimationFadeOut );

					if (nAnimationFadeIn >= c_nAnimationRotateSectionBegin &&
						nAnimationFadeIn <= c_nAnimationRotateSectionEnd)
						SetAnimationRotateFadeIn( nAnimationFadeIn );

					if (nAnimationFadeOut >= c_nAnimationRotateSectionBegin &&
						nAnimationFadeOut <= c_nAnimationRotateSectionEnd)
						SetAnimationRotateFadeOut( nAnimationFadeOut );

					if( nAnimationFadeIn >= c_nAnimationStretchSectionBegin &&
						nAnimationFadeIn <= c_nAnimationStretchSectionEnd)
						SetAnimationStretchFadeIn( nAnimationFadeIn );

					if( nAnimationFadeOut >= c_nAnimationStretchSectionBegin &&
						nAnimationFadeOut <= c_nAnimationStretchSectionEnd)
						SetAnimationStretchFadeOut( nAnimationFadeOut );

					// использование в статике прозрачности с коэффициентом fAlphaFadeFactorMax
					for (int index = 0; index < m_oLetters.GetCount(); ++index)
					{
						CTextPainterLetter* pLetter = m_oLetters[index];
						if (NULL == pLetter)
							continue;

						CTextTransformsList& oTransformsStatic = pLetter->GetTransform().GetTransformsStatic();
						CTextTransforms* pLocalTransforms = new CTextTransforms();
						pLocalTransforms->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax));
						oTransformsStatic.Add(pLocalTransforms);
					}
				}
				
				void TextDraw(double dAnimation, Gdiplus::Graphics& oGraphics)
				{
					if (NULL == m_pPaintState)
						return;

					oGraphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
					oGraphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
					oGraphics.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
					
					if( m_bClipping )
						oGraphics.SetClip(m_oClipRect /*, CombineModeIntersect*/); 

					// вначале надо отрисовать тень от всех букв
					TextDrawShadow(dAnimation, oGraphics);

					// затем отрисовываем сами буквы
					for (int index = 0; index < m_oLetters.GetCount(); ++index)
					{
						Gdiplus::GraphicsPath* pPath = m_oLetters[index]->GetGraphicsPath();

						CTextPainterLetterTransform& oTransform = m_oLetters[index]->GetTransform();

						Gdiplus::GraphicsState oStateInitial = oGraphics.Save();

						// применяем нужные трансформы для grpahics'а, чтобы отрисовать букву там, где надо
						CTextTransforms oAnimationTransforms;
						oTransform.SetTransformToGraphics(oGraphics, dAnimation, oAnimationTransforms);

						// отрисовываем границу буквы
						TextDrawEdge(oGraphics, pPath, oAnimationTransforms);

						// отрисовываем саму букву
						TextDrawLetter(oGraphics, pPath, m_oLetters[index]->GetRectBounds(), oAnimationTransforms);
						
						oGraphics.Restore(oStateInitial);
					}
				}

				
				static void GDIPLUS_GaussianBlur(Gdiplus::Bitmap& oBitmap, int nBlurSize)
				{
					if( nBlurSize < 1 )
					{
						return;
					}

					Gdiplus::BitmapData oBitmapData;
					
					if( Gdiplus::Ok == oBitmap.LockBits(NULL, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &oBitmapData) )
					{
						BGRA_GaussianBlur((BYTE*)oBitmapData.Scan0, oBitmapData.Width, oBitmapData.Height, oBitmapData.Stride, nBlurSize);

						oBitmap.UnlockBits(&oBitmapData);
					}
				}
				//static BOOL BGRA_GaussianBlurHorizontal(BYTE* pSrcData, BYTE* pDstData, int nWidth, int nHeight, int nSrcStride, int nDstStride, int nBlurSize )
				//{
				//	if( !pSrcData || !pDstData || nWidth < 1 || nHeight < 1 || nSrcStride < 1 || nDstStride < 1 || nBlurSize < 1 )
				//	{
				//		return FALSE;
				//	}

				//	int nLength;
				//	int nA, nR, nG, nB;
				//	int nClr;
				//	int	nFactor;

				//	for( ; nHeight > 0; --nHeight, pSrcData += nSrcStride, pDstData += nDstStride )
				//	{
				//		DWORD* pSrc = (DWORD*)pSrcData;
				//		DWORD* pDst = (DWORD*)pDstData;

				//		nB = 0;
				//		nG = 0;
				//		nR = 0;
				//		nA = 0;

				//		// подсчёт начальной суммы
				//		nLength = nBlurSize < nWidth ? nBlurSize : nWidth - 1;
				//		nFactor = nLength + 1;
				//		for( ; nLength >= 0; --nLength )
				//		{
				//			nClr = pSrc[nLength];
				//			
				//			nB += nClr       & 0xff;
				//			nG += nClr >> 8  & 0xff;
				//			nR += nClr >> 16 & 0xff;
				//			nA += nClr >> 24 & 0xff;
				//		}

				//		// расчёт новых пикселов в строке
				//		nLength = nWidth;
				//		for( ;; )
				//		{
				//			nClr  = nB/nFactor;
				//			nClr |= nG/nFactor << 8;
				//			nClr |= nR/nFactor << 16;
				//			nClr |= nA/nFactor << 24;

				//			*pDst = nClr;

				//			// проверка на выход из цикла
				//			if( nLength <= 1 )
				//			{
				//				break;
				//			}

				//			// вычитаем крайнее левое значение
				//			if( nLength <= nWidth - nBlurSize )
				//			{
				//				nClr = pSrc[-nBlurSize];
				//			
				//				nB -= nClr       & 0xff;
				//				nG -= nClr >> 8  & 0xff;
				//				nR -= nClr >> 16 & 0xff;
				//				nA -= nClr >> 24 & 0xff;
				//			}
				//			else
				//			{
				//				++nFactor;
				//			}

				//			// переходим к следующему пикселу
				//			--nLength;
				//			++pSrc;
				//			++pDst;
				//			
				//			// прибавляем следующее крайнее правое значение
				//			if( nLength > nBlurSize )
				//			{
				//				nClr = pSrc[nBlurSize];

				//				nB += nClr       & 0xff;
				//				nG += nClr >> 8  & 0xff;
				//				nR += nClr >> 16 & 0xff;
				//				nA += nClr >> 24 & 0xff;
				//			}
				//			else
				//			{
				//				--nFactor;
				//			}
				//		}
				//	}

				//	return TRUE;
				//}
				//static BOOL BGRA_GaussianBlurVertical(BYTE* pSrcData, BYTE* pDstData, int nWidth, int nHeight, int nSrcStride, int nDstStride, int nBlurSize )
				//{
				//	if( !pSrcData || !pDstData || nWidth < 1 || nHeight < 1 || nSrcStride < 1 || nDstStride < 1 || nBlurSize < 1 )
				//	{
				//		return FALSE;
				//	}

				//	int nLength;
				//	int nA, nR, nG, nB;
				//	int nClr;
				//	int nFactor;

				//	for( ; nWidth > 0; --nWidth, pSrcData += 4, pDstData += 4 )
				//	{
				//		BYTE* pSrc = pSrcData;
				//		BYTE* pDst = pDstData;

				//		nB = 0;
				//		nG = 0;
				//		nR = 0;
				//		nA = 0;

				//		// подсчёт начальной суммы
				//		nLength = nBlurSize < nHeight ? nBlurSize : nHeight - 1;
				//		nFactor = nLength + 1;
				//		for( nLength *= nSrcStride; nLength >= 0; nLength -= nSrcStride )
				//		{
				//			nClr = *(DWORD*)(pSrc + nLength);
				//			
				//			nB += nClr       & 0xff;
				//			nG += nClr >> 8  & 0xff;
				//			nR += nClr >> 16 & 0xff;
				//			nA += nClr >> 24 & 0xff;
				//		}

				//		// расчёт новых пикселов в строке
				//		nLength = nHeight;
				//		for( ;; )
				//		{
				//			nClr  = nB/nFactor;
				//			nClr |= nG/nFactor << 8;
				//			nClr |= nR/nFactor << 16;
				//			nClr |= nA/nFactor << 24;

				//			*(DWORD*)(pDst) = nClr;

				//			// проверка на выход из цикла
				//			if( nLength <= 1 )
				//			{
				//				break;
				//			}

				//			// вычитаем крайнее верхнее значение
				//			if( nLength <= nHeight - nBlurSize )
				//			{
				//				nClr = *(DWORD*)(pSrc - nBlurSize*nSrcStride);
				//			
				//				nB -= nClr       & 0xff;
				//				nG -= nClr >> 8  & 0xff;
				//				nR -= nClr >> 16 & 0xff;
				//				nA -= nClr >> 24 & 0xff;
				//			}
				//			else
				//			{
				//				++nFactor;
				//			}

				//			// переходим к следующему пикселу
				//			--nLength;
				//			pSrc += nSrcStride;
				//			pDst += nDstStride;

				//			// прибавляем следующее крайнее нижнее значение
				//			if( nLength > nBlurSize )
				//			{
				//				nClr = *(DWORD*)(pSrc + nBlurSize*nSrcStride);

				//				nB += nClr       & 0xff;
				//				nG += nClr >> 8  & 0xff;
				//				nR += nClr >> 16 & 0xff;
				//				nA += nClr >> 24 & 0xff;
				//			}
				//			else
				//			{
				//				--nFactor;
				//			}
				//		}
				//	}

				//	return TRUE;
				//}
				static BOOL BGRA_GaussianBlurHorizontal(BYTE* pSrcData, BYTE* pDstData, int nWidth, int nHeight, int nSrcStride, int nDstStride, int nBlurSize )
				{
					if( !pSrcData || !pDstData || nWidth < 1 || nHeight < 1 || nSrcStride < 1 || nDstStride < 1 || nBlurSize < 1 )
					{
						return FALSE;
					}

					if( nBlurSize >= nWidth )
						nBlurSize = nWidth - 1;

					int nLength;
					int nA, nR, nG, nB;
					int nClr;
					int	nFactor;

					for( ; nHeight > 0; --nHeight, pSrcData += nSrcStride, pDstData += nDstStride )
					{
						DWORD* pSrc = (DWORD*)pSrcData;
						DWORD* pDst = (DWORD*)pDstData;

						nFactor = nBlurSize + 1;
						nClr = pSrc[0];
						nB = (nClr       & 0xff)*nFactor;
						nG = (nClr >> 8  & 0xff)*nFactor;
						nR = (nClr >> 16 & 0xff)*nFactor;
						nA = (nClr >> 24 & 0xff)*nFactor;

						// подсчёт начальной суммы
						for( nLength = nBlurSize - 1; nLength > 0; --nLength )
						{
							nClr = pSrc[nLength];
							
							nB += nClr       & 0xff;
							nG += nClr >> 8  & 0xff;
							nR += nClr >> 16 & 0xff;
							nA += nClr >> 24 & 0xff;
						}

						// расчёт новых пикселов в строке
						nFactor = (nBlurSize << 1) + 1;
						nLength = nWidth;
						for( ;; )
						{
							// прибавляем следующее крайнее правое значение
							if( nLength > nBlurSize )
							{
								nClr = pSrc[nBlurSize];
							}
							else
							{
								//nClr = pSrc[(nLength << 1) - nBlurSize - 1];
								nClr = pSrc[nLength - 1];
							}
							nB += nClr       & 0xff;
							nG += nClr >> 8  & 0xff;
							nR += nClr >> 16 & 0xff;
							nA += nClr >> 24 & 0xff;


							nClr  = nB/nFactor;
							nClr |= nG/nFactor << 8;
							nClr |= nR/nFactor << 16;
							nClr |= nA/nFactor << 24;

							*pDst = nClr;

							// проверка на выход из цикла
							if( nLength <= 1 )
							{
								break;
							}

							// вычитаем крайнее левое значение
							if( nLength <= nWidth - nBlurSize )
							{
								nClr = pSrc[-nBlurSize];
							}
							else
							{
								//nClr = pSrc[nBlurSize - ((nWidth - nLength)<<1) - 1];
								nClr = pSrc[nLength - nWidth];
							}

							nB -= nClr       & 0xff;
							nG -= nClr >> 8  & 0xff;
							nR -= nClr >> 16 & 0xff;
							nA -= nClr >> 24 & 0xff;

							// переходим к следующему пикселу
							--nLength;
							++pSrc;
							++pDst;
						}
					}

					return TRUE;
				}
				static BOOL BGRA_GaussianBlurVertical(BYTE* pSrcData, BYTE* pDstData, int nWidth, int nHeight, int nSrcStride, int nDstStride, int nBlurSize )
				{
					if( !pSrcData || !pDstData || nWidth < 1 || nHeight < 1 || nSrcStride < 1 || nDstStride < 1 || nBlurSize < 1 )
					{
						return FALSE;
					}

					if( nBlurSize >= nHeight )
						nBlurSize = nHeight - 1;

					int nLength;
					int nA, nR, nG, nB;
					int nClr;
					int nFactor;

					for( ; nWidth > 0; --nWidth, pSrcData += 4, pDstData += 4 )
					{
						BYTE* pSrc = pSrcData;
						BYTE* pDst = pDstData;

						nFactor = nBlurSize + 1;
						nClr = *(DWORD*)(pSrc);
						nB = (nClr       & 0xff)*nFactor;
						nG = (nClr >> 8  & 0xff)*nFactor;
						nR = (nClr >> 16 & 0xff)*nFactor;
						nA = (nClr >> 24 & 0xff)*nFactor;

						// подсчёт начальной суммы
						for( nLength = nBlurSize - 1; nLength > 0; --nLength )
						{
							nClr = *(DWORD*)(pSrc + nLength*nSrcStride);
							
							nB += nClr       & 0xff;
							nG += nClr >> 8  & 0xff;
							nR += nClr >> 16 & 0xff;
							nA += nClr >> 24 & 0xff;
						}

						// расчёт новых пикселов в строке
						nFactor = (nBlurSize << 1) + 1;
						nLength = nHeight;
						for( ;; )
						{
							// прибавляем следующее крайнее нижнее значение
							if( nLength > nBlurSize )
							{
								nClr = *(DWORD*)(pSrc + nBlurSize*nSrcStride);
							}
							else
							{
								//nClr = *(DWORD*)(pSrc + ((nLength << 1) - nBlurSize - 1)*nSrcStride);
								nClr = *(DWORD*)(pSrc + (nLength - 1)*nSrcStride);
							}

							nB += nClr       & 0xff;
							nG += nClr >> 8  & 0xff;
							nR += nClr >> 16 & 0xff;
							nA += nClr >> 24 & 0xff;

							nClr  = nB/nFactor;
							nClr |= nG/nFactor << 8;
							nClr |= nR/nFactor << 16;
							nClr |= nA/nFactor << 24;

							*(DWORD*)(pDst) = nClr;

							// проверка на выход из цикла
							if( nLength <= 1 )
							{
								break;
							}

							// вычитаем крайнее верхнее значение
							if( nLength <= nHeight - nBlurSize )
							{
								nClr = *(DWORD*)(pSrc - nBlurSize*nSrcStride);
							}
							else
							{
								//nClr = *(DWORD*)(pSrc + (nBlurSize - ((nHeight - nLength)<<1) - 1)*nSrcStride);
								nClr = *(DWORD*)(pSrc + (nLength - nHeight)*nSrcStride);
							}

							nB -= nClr       & 0xff;
							nG -= nClr >> 8  & 0xff;
							nR -= nClr >> 16 & 0xff;
							nA -= nClr >> 24 & 0xff;

							// переходим к следующему пикселу
							--nLength;
							pSrc += nSrcStride;
							pDst += nDstStride;
						}
					}

					return TRUE;
				}

				static void BGRA_GaussianBlur(BYTE* pBGRA, int nWidth, int nHeight, int nStride, int nBlurSize)
				{
					if( !pBGRA || nWidth < 1 || nHeight < 1 || nStride < nWidth*4 || nBlurSize < 1 )
					{
						return;
					}

					int nBufStride = nWidth * 4;
					BYTE* pBuffer  = new BYTE[nBufStride * nHeight];
					if( !pBuffer )
					{
						return;
					}

					for( int i = 0; i < 2; i++ )
					{
						BGRA_GaussianBlurVertical(pBGRA, pBuffer, nWidth, nHeight, nStride, nBufStride, nBlurSize);
						BGRA_GaussianBlurHorizontal(pBuffer, pBGRA, nWidth, nHeight, nBufStride, nStride, nBlurSize);
					}

					delete[] pBuffer;
				}
			protected:
				
				void Destroy()
				{
					m_oLetters.RemoveAll();
					m_dTextLineSpacing = 0.0;
					m_bClipping = FALSE;
					m_nHorAlign = 0;
					m_nVerAlign = 0;
					m_fTextFactorY = 0;
				}
				
				void SetAnimationAlphaBlendFadeIn( int nAnimationFadeIn )

				{
					float fAlphaFadeFactorMin = m_fAlphaFadeFactorMin;
					float fAlphaFadeFactorMax = m_fAlphaFadeFactorMax;

					int nNumLetters = m_oLetters.GetCount();

					if( c_nAnimationAlphaBlendSimple == nAnimationFadeIn )
					{
						for (int nLetter = 0; nLetter < nNumLetters; ++nLetter)
						{
							CTextPainterLetter* pLetter = m_oLetters[nLetter];
							if (NULL == pLetter)
								continue;

							CTextTransformsList& oTransformsFadeIn = pLetter->GetTransform().GetTransformsFadeIn();

							CTextTransforms* pLocalTransforms1 = new CTextTransforms();
							CTextTransforms* pLocalTransforms2 = new CTextTransforms();

							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMin));
							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax));

							oTransformsFadeIn.Add(pLocalTransforms1);
							oTransformsFadeIn.Add(pLocalTransforms2);
						}
						return;
					}

					CLetterPermutation oPermutation;
					float fLetterAlpha = 0;
					int nFactor = 4;
					int nStep = 4;
					BOOL bGlassScale = FALSE;
					BOOL bSimpleScale = FALSE;

					switch( nAnimationFadeIn )
					{
					case c_nAnimationAlphaBlendLetterAZ: 
						nFactor = 1; nStep = 1;
						break;
					case c_nAnimationAlphaBlendLetterRandom: 
						oPermutation.Create(m_oLetters.GetCount(), TRUE);
						break;
					case c_nAnimationAlphaBlendLetterAZSmooth: 
						break;
					case c_nAnimationAlphaBlendLetterRandomGlass: 
						oPermutation.Create(m_oLetters.GetCount(), TRUE);
						nFactor = 7; nStep = 3;
						bGlassScale = TRUE;						
						break;
					case c_nAnimationAlphaBlendLetterAZSmoothGlass: 
						nFactor = 5; nStep = 4;
						bGlassScale = TRUE;						
						break;
					case c_nAnimationAlphaBlendLetterRandomScale: 
						oPermutation.Create(m_oLetters.GetCount(), TRUE);
						bSimpleScale = TRUE;						
						break;
					case c_nAnimationAlphaBlendLetterAZSmoothScale: 
						bSimpleScale = TRUE;						
						break;
					default: return;
					}

					int nNumCycles = (nNumLetters + nStep)*nFactor;

					for (int nLetter = 0; nLetter < nNumLetters; nLetter++ )
					{
						CTextPainterLetter* pLetter = m_oLetters[nLetter];
						if (NULL == pLetter)
							continue;

						CTextTransformsList& oTransformsFadeIn = pLetter->GetTransform().GetTransformsFadeIn();

						int nIndex = oPermutation[nLetter];
						if( nIndex < 0 ) nIndex = nLetter;

						for( int i = 0; i < nNumCycles; i++ )
						{
							CTextTransforms* pLocalTransforms = new CTextTransforms();

							if( i < nIndex * nFactor )
								fLetterAlpha = 0.0f;
							else if( i >= (nIndex + nStep)*nFactor )
								fLetterAlpha = 1.0f;
							else
								fLetterAlpha =(i - nIndex*nFactor + 1) / float(nStep*nFactor);

							// корректировка альфы
							fLetterAlpha = fLetterAlpha * (fAlphaFadeFactorMax - fAlphaFadeFactorMin) + fAlphaFadeFactorMin;

							if( bGlassScale )
							{
								float scale = (fLetterAlpha > 0.5f) ? (-fLetterAlpha + 2) : (2.5f*fLetterAlpha + 0.75f);
								pLocalTransforms->Add(new CTextTransform(CTextTransform::c_nTransformScale, scale, scale));

								fLetterAlpha *=1.5f;
								if(fLetterAlpha > 1) fLetterAlpha = 1;
							}
							if( bSimpleScale )
							{
								pLocalTransforms->Add(new CTextTransform(CTextTransform::c_nTransformScale, fLetterAlpha, fLetterAlpha));
							}

							pLocalTransforms->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fLetterAlpha));
							oTransformsFadeIn.Add(pLocalTransforms);
						}
					}
				}

				void SetAnimationAlphaBlendFadeOut( int nAnimationFadeOut )
				{
					float fAlphaFadeFactorMin = m_fAlphaFadeFactorMin;
					float fAlphaFadeFactorMax = m_fAlphaFadeFactorMax;

					int nNumLetters = m_oLetters.GetCount();

					if( c_nAnimationAlphaBlendSimple == nAnimationFadeOut )
					{
						for (int nLetter = 0; nLetter < nNumLetters; ++nLetter)
						{
							CTextPainterLetter* pLetter = m_oLetters[nLetter];
							if (NULL == pLetter)
								continue;

							CTextTransformsList& oTransformsFadeOut = pLetter->GetTransform().GetTransformsFadeOut();

							CTextTransforms* pLocalTransforms1 = new CTextTransforms();
							CTextTransforms* pLocalTransforms2 = new CTextTransforms();

							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax));
							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMin));

							oTransformsFadeOut.Add(pLocalTransforms1);
							oTransformsFadeOut.Add(pLocalTransforms2);
						}
						return;
					}

					CLetterPermutation oPermutation;
					float fLetterAlpha = 0;
					int nFactor = 4;
					int nStep = 4;
					BOOL bGlassScale = FALSE;
					BOOL bSimpleScale = FALSE;

					switch( nAnimationFadeOut )
					{
					case c_nAnimationAlphaBlendLetterAZ: 
						nFactor = 1; nStep = 1;
						break;
					case c_nAnimationAlphaBlendLetterRandom: 
						oPermutation.Create(m_oLetters.GetCount(), TRUE);
						break;
					case c_nAnimationAlphaBlendLetterAZSmooth: 
						break;
					case c_nAnimationAlphaBlendLetterRandomGlass: 
						oPermutation.Create(m_oLetters.GetCount(), TRUE);
						nFactor = 6; nStep = 3;
						bGlassScale = TRUE;						
						break;
					case c_nAnimationAlphaBlendLetterAZSmoothGlass: 
						nFactor = 6; nStep = 3;
						bGlassScale = TRUE;						
						break;
					case c_nAnimationAlphaBlendLetterRandomScale: 
						oPermutation.Create(m_oLetters.GetCount(), TRUE);
						bSimpleScale = TRUE;						
						break;
					case c_nAnimationAlphaBlendLetterAZSmoothScale: 
						bSimpleScale = TRUE;						
						break;
					default: return;
					}

					int nNumCycles = (nNumLetters + nStep)*nFactor;

					for (int nLetter = 0; nLetter < nNumLetters; nLetter++ )
					{
						CTextPainterLetter* pLetter = m_oLetters[nLetter];
						if (NULL == pLetter)
							continue;

						CTextTransformsList& oTransformsFadeOut = pLetter->GetTransform().GetTransformsFadeOut();

						int nIndex = oPermutation[nLetter];
						if( nIndex < 0 ) nIndex = nLetter;

						for( int i = 0; i < nNumCycles; i++ )
						{
							CTextTransforms* pLocalTransforms = new CTextTransforms();

							if( i < nIndex * nFactor )
								fLetterAlpha = 1;
							else if( i >= (nIndex + nStep)*nFactor )
								fLetterAlpha = 0;
							else
								fLetterAlpha = 1 - (i - nIndex*nFactor + 1) / float(nStep*nFactor);

							// корректировка альфы
							fLetterAlpha = fLetterAlpha * (fAlphaFadeFactorMax - fAlphaFadeFactorMin) + fAlphaFadeFactorMin;

							if( bGlassScale )
							{
								float scale = (fLetterAlpha > 0.5) ? (-0.5f*fLetterAlpha + 1.5f) : (0.5f*fLetterAlpha + 1);
								pLocalTransforms->Add(new CTextTransform(CTextTransform::c_nTransformScale, scale, scale));
							}
							if( bSimpleScale )
							{
								pLocalTransforms->Add(new CTextTransform(CTextTransform::c_nTransformScale, fLetterAlpha, fLetterAlpha));
							}

							pLocalTransforms->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fLetterAlpha));
							oTransformsFadeOut.Add(pLocalTransforms);
						}
					}
				}
				void SetAnimationAlphaBlendDropFadeIn( int nAnimationFadeIn )
				{
					float fAlphaFadeFactorMin = m_fAlphaFadeFactorMin;
					float fAlphaFadeFactorMax = m_fAlphaFadeFactorMax;

					float fAnimationOffset = 50.0f;

					for (int index = 0; index < m_oLetters.GetCount(); ++index)
					{
						CTextPainterLetter* pLetter = m_oLetters[index];
						if (NULL == pLetter)
							continue;
					
						CTextTransformsList& oTransformsFadeIn = pLetter->GetTransform().GetTransformsFadeIn();
						if (nAnimationFadeIn == c_nAnimationDropLeft)
						{
							CTextTransforms* pLocalTransforms1 = new CTextTransforms();
							CTextTransforms* pLocalTransforms2 = new CTextTransforms();

							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMin/*0.0f*/));
							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, -fAnimationOffset, 0.0f));

							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax/*1.0f*/));
							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0.0f, 0.0f));

							oTransformsFadeIn.Add(pLocalTransforms1);
							oTransformsFadeIn.Add(pLocalTransforms2);
						}
						else if (nAnimationFadeIn == c_nAnimationDropTop)
						{
							CTextTransforms* pLocalTransforms1 = new CTextTransforms();
							CTextTransforms* pLocalTransforms2 = new CTextTransforms();

							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMin/*0.0f*/));
							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0.0f, -fAnimationOffset));

							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax/*1.0f*/));
							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0.0f, 0.0f));

							oTransformsFadeIn.Add(pLocalTransforms1);
							oTransformsFadeIn.Add(pLocalTransforms2);
						}
						else if (nAnimationFadeIn == c_nAnimationDropRight)
						{
							CTextTransforms* pLocalTransforms1 = new CTextTransforms();
							CTextTransforms* pLocalTransforms2 = new CTextTransforms();

							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMin/*0.0f*/));
							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, fAnimationOffset, 0.0f));

							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax/*1.0f*/));
							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0.0f, 0.0f));

							oTransformsFadeIn.Add(pLocalTransforms1);
							oTransformsFadeIn.Add(pLocalTransforms2);
						}
						else if (nAnimationFadeIn == c_nAnimationDropBottom)
						{
							CTextTransforms* pLocalTransforms1 = new CTextTransforms();
							CTextTransforms* pLocalTransforms2 = new CTextTransforms();

							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMin/*0.0f*/));
							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0.0f, fAnimationOffset));

							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax/*1.0f*/));
							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0.0f, 0.0f));

							oTransformsFadeIn.Add(pLocalTransforms1);
							oTransformsFadeIn.Add(pLocalTransforms2);
						}
						else if (nAnimationFadeIn == c_nAnimationDropRotate1)
						{
							CTextTransforms* pLocalTransforms1 = new CTextTransforms();
							CTextTransforms* pLocalTransforms2 = new CTextTransforms();

							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMin/*0.0f*/));
							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformRotate, fAnimationOffset, 0, 360.0f));

							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax/*1.0f*/));
							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformRotate, 0, 0, 0.0f));

							oTransformsFadeIn.Add(pLocalTransforms1);
							oTransformsFadeIn.Add(pLocalTransforms2);
						}
						else if (nAnimationFadeIn == c_nAnimationDropRotate2)
						{
							CTextTransforms* pLocalTransforms1 = new CTextTransforms();
							CTextTransforms* pLocalTransforms2 = new CTextTransforms();

							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMin/*0.0f*/));
							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformRotate, fAnimationOffset, 0, -360.0f));

							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax/*1.0f*/));
							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformRotate, 0, 0, 0.0f));

							oTransformsFadeIn.Add(pLocalTransforms1);
							oTransformsFadeIn.Add(pLocalTransforms2);
						}
					}
				}

				void SetAnimationAlphaBlendDropFadeOut( int nAnimationFadeOut )
				{
					float fAlphaFadeFactorMin = m_fAlphaFadeFactorMin;
					float fAlphaFadeFactorMax = m_fAlphaFadeFactorMax;

					float fAnimationOffset = 50.0f;

					for (int index = 0; index < m_oLetters.GetCount(); ++index)
					{
						CTextPainterLetter* pLetter = m_oLetters[index];
						if (NULL == pLetter)
							continue;
					
						CTextTransformsList& oTransformsFadeOut = pLetter->GetTransform().GetTransformsFadeOut();
						if (nAnimationFadeOut == c_nAnimationDropLeft)
						{
							CTextTransforms* pLocalTransforms1 = new CTextTransforms();
							CTextTransforms* pLocalTransforms2 = new CTextTransforms();

							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax/*1.0f*/));
							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0.0f, 0.0f));

							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMin/*0.0f*/));
							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, fAnimationOffset, 0.0f));

							oTransformsFadeOut.Add(pLocalTransforms1);
							oTransformsFadeOut.Add(pLocalTransforms2);
						}
						else if (nAnimationFadeOut == c_nAnimationDropTop)
						{
							CTextTransforms* pLocalTransforms1 = new CTextTransforms();
							CTextTransforms* pLocalTransforms2 = new CTextTransforms();

							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax/*1.0f*/));
							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0.0f, 0.0f));

							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMin/*0.0f*/));
							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0.0f, fAnimationOffset));

							oTransformsFadeOut.Add(pLocalTransforms1);
							oTransformsFadeOut.Add(pLocalTransforms2);
						}
						else if (nAnimationFadeOut == c_nAnimationDropRight)
						{
							CTextTransforms* pLocalTransforms1 = new CTextTransforms();
							CTextTransforms* pLocalTransforms2 = new CTextTransforms();

							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax/*1.0f*/));
							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0.0f, 0.0f));

							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMin/*0.0f*/));
							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, -fAnimationOffset, 0.0f));

							oTransformsFadeOut.Add(pLocalTransforms1);
							oTransformsFadeOut.Add(pLocalTransforms2);
						}
						else if (nAnimationFadeOut == c_nAnimationDropBottom)
						{
							CTextTransforms* pLocalTransforms1 = new CTextTransforms();
							CTextTransforms* pLocalTransforms2 = new CTextTransforms();

							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax/*1.0f*/));
							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0.0f, 0.0f));

							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMin/*0.0f*/));
							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0.0f, -fAnimationOffset));

							oTransformsFadeOut.Add(pLocalTransforms1);
							oTransformsFadeOut.Add(pLocalTransforms2);
						}
						else if (nAnimationFadeOut == c_nAnimationDropRotate1)
						{
							CTextTransforms* pLocalTransforms1 = new CTextTransforms();
							CTextTransforms* pLocalTransforms2 = new CTextTransforms();

							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax/*1.0f*/));
							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformRotate, 0.0f, 0, 0.0f));

							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMin/*0.0f*/));
							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformRotate, fAnimationOffset, 0, 360.0f));

							oTransformsFadeOut.Add(pLocalTransforms1);
							oTransformsFadeOut.Add(pLocalTransforms2);
						}
						else if (nAnimationFadeOut == c_nAnimationDropRotate2)
						{
							CTextTransforms* pLocalTransforms1 = new CTextTransforms();
							CTextTransforms* pLocalTransforms2 = new CTextTransforms();

							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax/*1.0f*/));
							pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformRotate, 0.0f, 0, 0.0f));

							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMin/*0.0f*/));
							pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformRotate, fAnimationOffset, 0, -360.0f));

							oTransformsFadeOut.Add(pLocalTransforms1);
							oTransformsFadeOut.Add(pLocalTransforms2);
						}
					}
				}

				void SetAnimationMoveToFadeIn( int nAnimationFadeIn )
				{
					float fTextWidth  = m_oRectBounds.Width;
					float fTextHeight = m_oRectBounds.Height;
					float fInOffsetXLeft = 0;
					float fInOffsetXRight = 0;
					float fInOffsetYTop = 0;
					float fInOffsetYBottom = 0;

					switch( m_nHorAlign )
					{
					case Gdiplus::StringAlignmentNear:
						fInOffsetXLeft = -fTextWidth;
						fInOffsetXRight = m_oClipRect.Width;
						break;
					case Gdiplus::StringAlignmentCenter:
						fInOffsetXLeft = -(0.5f * (m_oClipRect.Width + fTextWidth));
						fInOffsetXRight = -fInOffsetXLeft;
						break;
					case Gdiplus::StringAlignmentFar:
						fInOffsetXLeft = -m_oClipRect.Width;
						fInOffsetXRight = fTextWidth;
						break;
					}

					switch( m_nVerAlign )
					{
					case Gdiplus::StringAlignmentNear:
						fInOffsetYTop = -fTextHeight;
						fInOffsetYBottom = m_oClipRect.Height;
						break;
					case Gdiplus::StringAlignmentCenter:
						fInOffsetYTop = -(0.5f * (m_oClipRect.Height + fTextHeight));
						fInOffsetYBottom = -fInOffsetYTop;
						break;
					case Gdiplus::StringAlignmentFar:
						fInOffsetYTop = -m_oClipRect.Height;
						fInOffsetYBottom = fTextHeight;
						break;
					}

					float fX, fY;

					switch( nAnimationFadeIn )
					{
					case c_nAnimationMoveLeft:        fX = fInOffsetXLeft;  fY = 0;                break;
					case c_nAnimationMoveTop:         fX = 0;               fY = fInOffsetYTop;    break;
					case c_nAnimationMoveRight:       fX = fInOffsetXRight; fY = 0;                break;
					case c_nAnimationMoveBottom:      fX = 0;               fY = fInOffsetYBottom; break;
					case c_nAnimationMoveTopLeft:     fX = fInOffsetXLeft;  fY = fInOffsetYTop;    break;
					case c_nAnimationMoveTopRight:    fX = fInOffsetXRight; fY = fInOffsetYTop;    break;
					case c_nAnimationMoveBottomLeft:  fX = fInOffsetXLeft;  fY = fInOffsetYBottom; break;
					case c_nAnimationMoveBottomRight: fX = fInOffsetXRight; fY = fInOffsetYBottom; break;
					default: return;
					}

					CTextTransforms* pLocalTransforms1 = NULL;
					CTextTransforms* pLocalTransforms2 = NULL;

					for (int index = 0; index < m_oLetters.GetCount(); ++index)
					{
						CTextPainterLetter* pLetter = m_oLetters[index];
						if (NULL == pLetter)
							continue;
					
						CTextTransformsList& oTransformsFadeIn  = pLetter->GetTransform().GetTransformsFadeIn();

						pLocalTransforms1 = new CTextTransforms();
						pLocalTransforms2 = new CTextTransforms();
						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, fX, fY));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0, 0));
						oTransformsFadeIn.Add(pLocalTransforms1);
						oTransformsFadeIn.Add(pLocalTransforms2);
					}
				}
				void SetAnimationMoveToFadeOut( int nAnimationFadeOut )
				{
					float fTextWidth  = m_oRectBounds.Width;
					float fTextHeight = m_oRectBounds.Height;
					float fInOffsetXLeft = 0;
					float fInOffsetXRight = 0;
					float fInOffsetYTop = 0;
					float fInOffsetYBottom = 0;

					switch( m_nHorAlign )
					{
					case Gdiplus::StringAlignmentNear:
						fInOffsetXLeft = -fTextWidth;
						fInOffsetXRight = m_oClipRect.Width;
						break;
					case Gdiplus::StringAlignmentCenter:
						fInOffsetXLeft = -(0.5f * (m_oClipRect.Width + fTextWidth));
						fInOffsetXRight = -fInOffsetXLeft;
						break;
					case Gdiplus::StringAlignmentFar:
						fInOffsetXLeft = -m_oClipRect.Width;
						fInOffsetXRight = fTextWidth;
						break;
					}

					switch( m_nVerAlign )
					{
					case Gdiplus::StringAlignmentNear:
						fInOffsetYTop = -fTextHeight;
						fInOffsetYBottom = m_oClipRect.Height;
						break;
					case Gdiplus::StringAlignmentCenter:
						fInOffsetYTop = -(0.5f * (m_oClipRect.Height + fTextHeight));
						fInOffsetYBottom = -fInOffsetYTop;
						break;
					case Gdiplus::StringAlignmentFar:
						fInOffsetYTop = -m_oClipRect.Height;
						fInOffsetYBottom = fTextHeight;
						break;
					}

					float fX, fY;

					switch( nAnimationFadeOut )
					{
					case c_nAnimationMoveLeft:        fX = fInOffsetXLeft;  fY = 0;                break;
					case c_nAnimationMoveTop:         fX = 0;               fY = fInOffsetYTop;    break;
					case c_nAnimationMoveRight:       fX = fInOffsetXRight; fY = 0;                break;
					case c_nAnimationMoveBottom:      fX = 0;               fY = fInOffsetYBottom; break;
					case c_nAnimationMoveTopLeft:     fX = fInOffsetXLeft;  fY = fInOffsetYTop;    break;
					case c_nAnimationMoveTopRight:    fX = fInOffsetXRight; fY = fInOffsetYTop;    break;
					case c_nAnimationMoveBottomLeft:  fX = fInOffsetXLeft;  fY = fInOffsetYBottom; break;
					case c_nAnimationMoveBottomRight: fX = fInOffsetXRight; fY = fInOffsetYBottom; break;
					default: return;
					}

					CTextTransforms* pLocalTransforms1 = NULL;
					CTextTransforms* pLocalTransforms2 = NULL;

					for (int index = 0; index < m_oLetters.GetCount(); ++index)
					{
						CTextPainterLetter* pLetter = m_oLetters[index];
						if (NULL == pLetter)
							continue;
					
						CTextTransformsList& oTransformsFadeOut = pLetter->GetTransform().GetTransformsFadeOut();
						pLocalTransforms1 = new CTextTransforms();
						pLocalTransforms2 = new CTextTransforms();
						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0, 0));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, fX, fY));
						oTransformsFadeOut.Add(pLocalTransforms1);
						oTransformsFadeOut.Add(pLocalTransforms2);
					}
				}

				void SetAnimationScaleFadeIn( int nAnimationFadeIn )
				{
					float fAlphaFadeFactorMin = m_fAlphaFadeFactorMin;
					float fAlphaFadeFactorMax = m_fAlphaFadeFactorMax;

					Gdiplus::PointF oTextCenter(m_oRectBounds.X + m_oRectBounds.Width * 0.5f, m_oRectBounds.Y + m_oRectBounds.Height * 0.5f);

					float factorX = 1, factorY = 1;
					float offsetX = 0, offsetY = 0;
					
					switch( nAnimationFadeIn )
					{
					case c_nAnimationScaleFromSmall: 
						factorX = 0.5f; factorY = 0.5f; 	
						break;
					case c_nAnimationScaleFromLarge: 
						factorX = 1.5f; factorY = 1.5f;
						break;
					case c_nAnimationScaleFromXLargeYSmall:
						factorX = 1.5f; factorY = 0.5f;
						break;
					case c_nAnimationScaleFromXSmallYLarge:
						factorX = 0.5f; factorY = 1.5f;
						break;
					case c_nAnimationScaleFromSmallDropFromTop:
						factorX = 0.5f; factorY = 0.5f; 	
						switch( m_nVerAlign )
						{
						case Gdiplus::StringAlignmentNear:   offsetY = -0.5f*(m_oRectBounds.Height*(1 - factorY) + 0); break;
						case Gdiplus::StringAlignmentCenter: offsetY = -0.5f*(m_oRectBounds.Height*(1 - factorY) + m_oClipRect.Height*0.5f); break;
						case Gdiplus::StringAlignmentFar:    offsetY = -0.5f*(m_oRectBounds.Height*(1 - factorY) + m_oClipRect.Height); break;
						}
						break;
					case c_nAnimationScaleFromSmallDropFromBottom:
						factorX = 0.5f; factorY = 0.5f; 	
						switch( m_nVerAlign )
						{
						case Gdiplus::StringAlignmentNear:   offsetY = 0.5f*(m_oRectBounds.Height*(1 - factorY) + m_oClipRect.Height); break;
						case Gdiplus::StringAlignmentCenter: offsetY = 0.5f*(m_oRectBounds.Height*(1 - factorY) + m_oClipRect.Height*0.5f); break;
						case Gdiplus::StringAlignmentFar:    offsetY = 0.5f*(m_oRectBounds.Height*(1 - factorY) + 0); break;
						}
						break;
					default: return;
					}

					for (int index = 0; index < m_oLetters.GetCount(); ++index)
					{
						CTextPainterLetter* pLetter = m_oLetters[index];
						if (NULL == pLetter)
							continue;
					
						CTextTransforms* pLocalTransforms1 = NULL;
						CTextTransforms* pLocalTransforms2 = NULL;

						CTextTransformsList& oTransformsFadeIn = pLetter->GetTransform().GetTransformsFadeIn();
						pLocalTransforms1 = new CTextTransforms();
						pLocalTransforms2 = new CTextTransforms();
						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformScale, factorX, factorY));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformScale, 1, 1));
						
						Gdiplus::PointF oLetterCenter( pLetter->GetBoundsCenter() - oTextCenter );
						oLetterCenter.X *= factorX - 1;
						oLetterCenter.Y *= factorY - 1;
						oLetterCenter.X += offsetX;
						oLetterCenter.Y += offsetY;
						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, oLetterCenter.X, oLetterCenter.Y));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0, 0));

						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMin));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax));

						oTransformsFadeIn.Add(pLocalTransforms1);
						oTransformsFadeIn.Add(pLocalTransforms2);
					}
				}
				void SetAnimationScaleFadeOut( int nAnimationFadeOut )
				{
					float fAlphaFadeFactorMin = m_fAlphaFadeFactorMin;
					float fAlphaFadeFactorMax = m_fAlphaFadeFactorMax;

					Gdiplus::PointF oTextCenter(m_oRectBounds.X + m_oRectBounds.Width * 0.5f, m_oRectBounds.Y + m_oRectBounds.Height * 0.5f);

					float factorX = 1, factorY = 1;
					float offsetX = 0, offsetY = 0;
					
					switch( nAnimationFadeOut )
					{
					case c_nAnimationScaleFromSmall: 
						factorX = 1.5f; factorY = 1.5f; 	
						break;
					case c_nAnimationScaleFromLarge: 
						factorX = 0.5f; factorY = 0.5f;
						break;
					case c_nAnimationScaleFromXLargeYSmall:
						factorX = 0.5f; factorY = 1.5f;
						break;
					case c_nAnimationScaleFromXSmallYLarge:
						factorX = 1.5f; factorY = 0.5f;
						break;
					case c_nAnimationScaleFromSmallDropFromTop:
						factorX = 0.5f; factorY = 0.5f; 	
						switch( m_nVerAlign )
						{
						case Gdiplus::StringAlignmentNear:   offsetY = -0.5f*(m_oRectBounds.Height*(1 - factorY) + 0); break;
						case Gdiplus::StringAlignmentCenter: offsetY = -0.5f*(m_oRectBounds.Height*(1 - factorY) + m_oClipRect.Height*0.5f); break;
						case Gdiplus::StringAlignmentFar:    offsetY = -0.5f*(m_oRectBounds.Height*(1 - factorY) + m_oClipRect.Height); break;
						}
						break;
					case c_nAnimationScaleFromSmallDropFromBottom:
						factorX = 0.5f; factorY = 0.5f; 	
						switch( m_nVerAlign )
						{
						case Gdiplus::StringAlignmentNear:   offsetY = 0.5f*(m_oRectBounds.Height*(1 - factorY) + m_oClipRect.Height); break;
						case Gdiplus::StringAlignmentCenter: offsetY = 0.5f*(m_oRectBounds.Height*(1 - factorY) + m_oClipRect.Height*0.5f); break;
						case Gdiplus::StringAlignmentFar:    offsetY = 0.5f*(m_oRectBounds.Height*(1 - factorY) + 0); break;
						}
						break;
					default: return;
					}

					for (int index = 0; index < m_oLetters.GetCount(); ++index)
					{
						CTextPainterLetter* pLetter = m_oLetters[index];
						if (NULL == pLetter)
							continue;
					
						CTextTransforms* pLocalTransforms1 = NULL;
						CTextTransforms* pLocalTransforms2 = NULL;

						CTextTransformsList& oTransformsFadeOut = pLetter->GetTransform().GetTransformsFadeOut();
						pLocalTransforms1 = new CTextTransforms();
						pLocalTransforms2 = new CTextTransforms();
						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformScale, 1, 1));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformScale, factorX, factorY));
						
						Gdiplus::PointF oLetterCenter( pLetter->GetBoundsCenter() - oTextCenter );
						oLetterCenter.X *= factorX - 1;
						oLetterCenter.Y *= factorY - 1;
						oLetterCenter.X += offsetX;
						oLetterCenter.Y += offsetY;
						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0, 0));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, oLetterCenter.X, oLetterCenter.Y));

						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMax));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fAlphaFadeFactorMin));

						oTransformsFadeOut.Add(pLocalTransforms1);
						oTransformsFadeOut.Add(pLocalTransforms2);
					}
				};

				void SetAnimationRotateFadeIn( int nAnimationFadeIn )
				{
					Gdiplus::PointF oTextCenter(m_oRectBounds.X + m_oRectBounds.Width * 0.5f, m_oRectBounds.Y + m_oRectBounds.Height * 0.5f);

					float factor, angle;

					switch( nAnimationFadeIn )
					{
					case c_nAnimationRotate1:          factor = 1.0f; angle = 360;  break;
					case c_nAnimationRotate2:          factor = 1.0f; angle = -360; break;
					case c_nAnimationRotateFromSmall1: factor = 0.5f; angle = 360;  break;
					case c_nAnimationRotateFromSmall2: factor = 0.5f; angle = -360; break;
					case c_nAnimationRotateFromLarge1: factor = 1.5f; angle = 360;  break;
					case c_nAnimationRotateFromLarge2: factor = 1.5f; angle = -360; break;
					default: return;
					}

					for (int index = 0; index < m_oLetters.GetCount(); ++index)
					{
						CTextPainterLetter* pLetter = m_oLetters[index];
						if (NULL == pLetter)
							continue;
					
						CTextTransforms* pLocalTransforms1 = NULL;
						CTextTransforms* pLocalTransforms2 = NULL;

						CTextTransformsList& oTransformsFadeIn = pLetter->GetTransform().GetTransformsFadeIn();
						pLocalTransforms1 = new CTextTransforms();
						pLocalTransforms2 = new CTextTransforms();

						Gdiplus::PointF oLetterCenter( pLetter->GetBoundsCenter() - oTextCenter );

						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformRotate, -oLetterCenter.X, -oLetterCenter.Y, 0));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformRotate, -oLetterCenter.X, -oLetterCenter.Y, angle));

						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformScale, factor, factor));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformScale, 1, 1));

						oLetterCenter.X *= factor - 1;
						oLetterCenter.Y *= factor - 1;
						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, oLetterCenter.X, oLetterCenter.Y));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0, 0));

						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, m_fAlphaFadeFactorMin));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, m_fAlphaFadeFactorMax));

						oTransformsFadeIn.Add(pLocalTransforms1);
						oTransformsFadeIn.Add(pLocalTransforms2);
					}
				}
				void SetAnimationRotateFadeOut( int nAnimationFadeOut )
				{
					Gdiplus::PointF oTextCenter(m_oRectBounds.X + m_oRectBounds.Width * 0.5f, m_oRectBounds.Y + m_oRectBounds.Height * 0.5f);

					float factor, angle;

					switch( nAnimationFadeOut )
					{
					case c_nAnimationRotate1:          factor = 1.0f; angle = 360;  break;
					case c_nAnimationRotate2:          factor = 1.0f; angle = -360; break;
					case c_nAnimationRotateFromSmall1: factor = 1.5f; angle = 360;  break;
					case c_nAnimationRotateFromSmall2: factor = 1.5f; angle = -360; break;
					case c_nAnimationRotateFromLarge1: factor = 0.5f; angle = 360;  break;
					case c_nAnimationRotateFromLarge2: factor = 0.5f; angle = -360; break;
					default: return;
					}

					for (int index = 0; index < m_oLetters.GetCount(); ++index)
					{
						CTextPainterLetter* pLetter = m_oLetters[index];
						if (NULL == pLetter)
							continue;
					
						CTextTransforms* pLocalTransforms1 = NULL;
						CTextTransforms* pLocalTransforms2 = NULL;

						CTextTransformsList& oTransformsFadeOut = pLetter->GetTransform().GetTransformsFadeOut();
						pLocalTransforms1 = new CTextTransforms();
						pLocalTransforms2 = new CTextTransforms();

						Gdiplus::PointF oLetterCenter( pLetter->GetBoundsCenter() - oTextCenter );

						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformRotate, -oLetterCenter.X, -oLetterCenter.Y, 0));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformRotate, -oLetterCenter.X, -oLetterCenter.Y, angle));

						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformScale, 1, 1));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformScale, factor, factor));

						oLetterCenter.X *= factor - 1;
						oLetterCenter.Y *= factor - 1;
						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0, 0));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, oLetterCenter.X, oLetterCenter.Y));

						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, m_fAlphaFadeFactorMax));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, m_fAlphaFadeFactorMin));

						oTransformsFadeOut.Add(pLocalTransforms1);
						oTransformsFadeOut.Add(pLocalTransforms2);
					}
				}

				void SetAnimationAlphaSmoothFadeIn( int nAnimationFadeIn )
				{
					float fAlphaFadeFactorMin = m_fAlphaFadeFactorMin;
					float fAlphaFadeFactorMax = m_fAlphaFadeFactorMax;

					int nNumLetters = m_oLetters.GetCount();
					float fLetterAlpha = 0;
					int nFactor = 4;
					int nStep = 4;
					int nEnd = (nNumLetters + 1) / 2;
					int nOffset1, nOffset2;

					switch( nAnimationFadeIn )
					{
					case c_nAnimationAlphaSmoothDualToCenter: 
						nOffset1 = nNumLetters - 1;
						nOffset2 = 0;
						break;
					case c_nAnimationAlphaSmoothDualFromCenter:
						nOffset1 = nEnd - 1;
						nOffset2 = nEnd + ((nNumLetters % 2) ? -1 : 0);
						break;
					default: return;
					}

					int nNumCycles = (nEnd + nStep)*nFactor;
					
					for ( int nLetter = 0; nLetter < nEnd; nLetter++ )
					{
						CTextPainterLetter* pLetter1 = m_oLetters[nOffset1 - nLetter];
						CTextPainterLetter* pLetter2 = m_oLetters[nOffset2 + nLetter];
						if( pLetter1 == pLetter2 )
							pLetter2 = NULL;

						CTextTransformsList* oTransformsFadeIn1 = NULL;
						CTextTransformsList* oTransformsFadeIn2 = NULL;
						
						if( pLetter1 ) oTransformsFadeIn1 = &(pLetter1->GetTransform().GetTransformsFadeIn());
						if( pLetter2 ) oTransformsFadeIn2 = &(pLetter2->GetTransform().GetTransformsFadeIn());

						int nIndex = nLetter;

						for( int i = 0; i < nNumCycles; i++ )
						{
							if( i < nIndex * nFactor )
								fLetterAlpha = 0;
							else if( i >= (nIndex + nStep)*nFactor )
								fLetterAlpha = 1;
							else
								fLetterAlpha = (i - nIndex*nFactor + 1) / float(nStep*nFactor);

							// корректировка альфы
							fLetterAlpha = fLetterAlpha * (fAlphaFadeFactorMax - fAlphaFadeFactorMin) + fAlphaFadeFactorMin;

							if( oTransformsFadeIn1 )
							{
								CTextTransforms* pLocalTransforms = new CTextTransforms();
								pLocalTransforms->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fLetterAlpha));
								oTransformsFadeIn1->Add(pLocalTransforms);
							}
							if( oTransformsFadeIn2 )
							{
								CTextTransforms* pLocalTransforms = new CTextTransforms();
								pLocalTransforms->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fLetterAlpha));
								oTransformsFadeIn2->Add(pLocalTransforms);
							}
						}
					}
				}
				void SetAnimationAlphaSmoothFadeOut( int nAnimationFadeOut )
				{
					float fAlphaFadeFactorMin = m_fAlphaFadeFactorMin;
					float fAlphaFadeFactorMax = m_fAlphaFadeFactorMax;

					int nNumLetters = m_oLetters.GetCount();
					float fLetterAlpha = 0;
					int nFactor = 4;
					int nStep = 4;
					int nEnd = (nNumLetters + 1) / 2;
					int nOffset1, nOffset2;

					switch( nAnimationFadeOut )
					{
					case c_nAnimationAlphaSmoothDualToCenter: 
						nOffset1 = nNumLetters - 1;
						nOffset2 = 0;
						break;
					case c_nAnimationAlphaSmoothDualFromCenter:
						nOffset1 = nEnd - 1;
						nOffset2 = nEnd + ((nNumLetters % 2) ? -1 : 0);
						break;
					default: return;
					}

					int nNumCycles = (nEnd + nStep)*nFactor;
					
					for ( int nLetter = 0; nLetter < nEnd; nLetter++ )
					{
						CTextPainterLetter* pLetter1 = m_oLetters[nOffset1 - nLetter];
						CTextPainterLetter* pLetter2 = m_oLetters[nOffset2 + nLetter];
						if( pLetter1 == pLetter2 )
							pLetter2 = NULL;

						CTextTransformsList* oTransformsFadeOut1 = NULL;
						CTextTransformsList* oTransformsFadeOut2 = NULL;
						
						if( pLetter1 ) oTransformsFadeOut1 = &(pLetter1->GetTransform().GetTransformsFadeOut());
						if( pLetter2 ) oTransformsFadeOut2 = &(pLetter2->GetTransform().GetTransformsFadeOut());

						int nIndex = nLetter;

						for( int i = 0; i < nNumCycles; i++ )
						{
							if( i < nIndex * nFactor )
								fLetterAlpha = 1;
							else if( i >= (nIndex + nStep)*nFactor )
								fLetterAlpha = 0;
							else
								fLetterAlpha = 1 - (i - nIndex*nFactor + 1) / float(nStep*nFactor);

							// корректировка альфы
							fLetterAlpha = fLetterAlpha * (fAlphaFadeFactorMax - fAlphaFadeFactorMin) + fAlphaFadeFactorMin;

							if( oTransformsFadeOut1 )
							{
								CTextTransforms* pLocalTransforms = new CTextTransforms();
								pLocalTransforms->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fLetterAlpha));
								oTransformsFadeOut1->Add(pLocalTransforms);
							}
							if( oTransformsFadeOut2 )
							{
								CTextTransforms* pLocalTransforms = new CTextTransforms();
								pLocalTransforms->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, fLetterAlpha));
								oTransformsFadeOut2->Add(pLocalTransforms);
							}
						}
					}
				}
				void SetAnimationStretchFadeIn( int nAnimationFadeIn )
				{
					float fScaleX, fScaleY, fFactorX, fFactorY;

					switch( nAnimationFadeIn )
					{
					case c_nAnimationStretchScale:
						fScaleX = 0.5f; fScaleY = 0.5f; fFactorX = 0.0f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchHorScale:
						fScaleX = 0.5f; fScaleY = 1.0f; fFactorX = 0.0f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchVerScale:
						fScaleX = 1.0f; fScaleY = 0.5f; fFactorX = 0.0f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchLVerScale:
						fScaleX = 1.0f; fScaleY = 1.5f; fFactorX = 0.0f; fFactorY = -0.5f;
						break;
					case c_nAnimationStretchLVerHorScale:
						fScaleX = 0.5f; fScaleY = 1.5f; fFactorX = 0.0f; fFactorY = -0.5f;
						break;
					
					case c_nAnimationStretchSmall:
						fScaleX = 1.0f; fScaleY = 1.0f; fFactorX = 0.3f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchSmallScale:
						fScaleX = 0.5f; fScaleY = 0.5f; fFactorX = 0.3f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchSmallHorScale:
						fScaleX = 0.5f; fScaleY = 1.0f; fFactorX = 0.3f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchSmallVerScale:
						fScaleX = 1.0f; fScaleY = 0.5f; fFactorX = 0.3f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchSmallLargeVerScale:
						fScaleX = 1.0f; fScaleY = 1.5f; fFactorX = 0.3f; fFactorY = -0.5f;
						break;
					case c_nAnimationStretchSmallLargeVerHorScale:
						fScaleX = 0.5f; fScaleY = 1.5f; fFactorX = 0.3f; fFactorY = -0.5f;
						break;
					
					case c_nAnimationStretchLarge:
						fScaleX = 1.0f; fScaleY = 1.0f; fFactorX = -1.0f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchLargeScale:
						fScaleX = 0.5f; fScaleY = 0.5f; fFactorX = -1.0f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchLargeHorScale:
						fScaleX = 0.5f; fScaleY = 1.0f; fFactorX = -1.0f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchLargeVerScale:
						fScaleX = 1.0f; fScaleY = 0.5f; fFactorX = -1.0f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchLargeLargeVerScale:
						fScaleX = 1.0f; fScaleY = 1.5f; fFactorX = -1.0f; fFactorY = -0.5f;
						break;
					case c_nAnimationStretchLargeLargeVerHorScale:
						fScaleX = 0.5f; fScaleY = 1.5f; fFactorX = -1.0f; fFactorY = -0.5f;
						break;
					default: return;
					};

					int nNumLetters = m_oLetters.GetCount();

					Gdiplus::PointF ptTextCenter( m_oRectBounds.X + m_oRectBounds.Width * 0.5f, m_oRectBounds.Y + m_oRectBounds.Height * 0.5f );

					for (int nLetter = 0; nLetter < nNumLetters; ++nLetter)
					{
						CTextPainterLetter* pLetter = m_oLetters[nLetter];
						if (NULL == pLetter)
							continue;

						CTextTransformsList& oTransformsFadeIn = pLetter->GetTransform().GetTransformsFadeIn();

						CTextTransforms* pLocalTransforms1 = new CTextTransforms();
						CTextTransforms* pLocalTransforms2 = new CTextTransforms();

						Gdiplus::PointF ptLetterCenter = pLetter->GetBoundsCenter();
						float dx = (ptTextCenter.X - ptLetterCenter.X)*fFactorX;
						float dy = (ptTextCenter.Y - ptLetterCenter.Y)*fFactorY;

						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformScale, fScaleX, fScaleY));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformScale, 1, 1));
						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, dx, dy));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0, 0));
						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, m_fAlphaFadeFactorMin));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, m_fAlphaFadeFactorMax));

						oTransformsFadeIn.Add(pLocalTransforms1);
						oTransformsFadeIn.Add(pLocalTransforms2);
					}
				}
				void SetAnimationStretchFadeOut( int nAnimationFadeOut )
				{
					float fScaleX, fScaleY, fFactorX, fFactorY;

					switch( nAnimationFadeOut )
					{
					case c_nAnimationStretchScale:
						fScaleX = 1.25f; fScaleY = 1.25f; fFactorX = -0.2f; fFactorY = -0.2f;
						break;
					case c_nAnimationStretchHorScale:
						fScaleX = 1.25f; fScaleY = 1.0f; fFactorX = -0.2f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchVerScale:
						fScaleX = 1.0f; fScaleY = 1.25f; fFactorX = 0.0f; fFactorY = -0.2f;
						break;
					case c_nAnimationStretchLVerScale:
						fScaleX = 1.0f; fScaleY = 0.5f; fFactorX = 0.0f; fFactorY = 0.5f;
						break;
					case c_nAnimationStretchLVerHorScale:
						fScaleX = 1.25f; fScaleY = 0.5f; fFactorX = -0.2f; fFactorY = 0.5f;
						break;
					
					case c_nAnimationStretchSmall:
						fScaleX = 1.0f; fScaleY = 1.0f; fFactorX = -0.3f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchSmallScale:
						fScaleX = 0.5f; fScaleY = 0.5f; fFactorX = -0.3f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchSmallHorScale:
						fScaleX = 0.5f; fScaleY = 1.0f; fFactorX = -0.3f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchSmallVerScale:
						fScaleX = 1.0f; fScaleY = 0.5f; fFactorX = -0.3f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchSmallLargeVerScale:
						fScaleX = 1.0f; fScaleY = 1.5f; fFactorX = -0.3f; fFactorY = -0.5f;
						break;
					case c_nAnimationStretchSmallLargeVerHorScale:
						fScaleX = 0.5f; fScaleY = 1.5f; fFactorX = -0.3f; fFactorY = -0.5f;
						break;
					
					case c_nAnimationStretchLarge:
						fScaleX = 1.0f; fScaleY = 1.0f; fFactorX = 1.0f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchLargeScale:
						fScaleX = 0.5f; fScaleY = 0.5f; fFactorX = 1.0f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchLargeHorScale:
						fScaleX = 0.5f; fScaleY = 1.0f; fFactorX = 1.0f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchLargeVerScale:
						fScaleX = 1.0f; fScaleY = 0.5f; fFactorX = 1.0f; fFactorY = 0.0f;
						break;
					case c_nAnimationStretchLargeLargeVerScale:
						fScaleX = 1.0f; fScaleY = 1.5f; fFactorX = 1.0f; fFactorY = -0.5f;
						break;
					case c_nAnimationStretchLargeLargeVerHorScale:
						fScaleX = 0.5f; fScaleY = 1.5f; fFactorX = 1.0f; fFactorY = -0.5f;
						break;
					default: return;
					};

					int nNumLetters = m_oLetters.GetCount();

					Gdiplus::PointF ptTextCenter( m_oRectBounds.X + m_oRectBounds.Width * 0.5f, m_oRectBounds.Y + m_oRectBounds.Height * 0.5f );

					for (int nLetter = 0; nLetter < nNumLetters; ++nLetter)
					{
						CTextPainterLetter* pLetter = m_oLetters[nLetter];
						if (NULL == pLetter)
							continue;

						CTextTransformsList& oTransformsFadeOut = pLetter->GetTransform().GetTransformsFadeOut();

						CTextTransforms* pLocalTransforms1 = new CTextTransforms();
						CTextTransforms* pLocalTransforms2 = new CTextTransforms();

						Gdiplus::PointF ptLetterCenter = pLetter->GetBoundsCenter();
						float dx = (ptTextCenter.X - ptLetterCenter.X)*fFactorX;
						float dy = (ptTextCenter.Y - ptLetterCenter.Y)*fFactorY;

						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformScale, 1, 1));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformScale, fScaleX, fScaleY));
						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, 0, 0));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformTranslate, dx, dy));
						pLocalTransforms1->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, m_fAlphaFadeFactorMax));
						pLocalTransforms2->Add(new CTextTransform(CTextTransform::c_nTransformMultiplyAlpha, m_fAlphaFadeFactorMin));

						oTransformsFadeOut.Add(pLocalTransforms1);
						oTransformsFadeOut.Add(pLocalTransforms2);
					}
				}

				
				void TextDrawShadow(double dAnimation, Gdiplus::Graphics& oGraphics)
				{
					// создаем brush, которым будем отрисовывать текст
					Gdiplus::SolidBrush oShadowBrush(Gdiplus::Color::Black);

					for (int index = 0; index < m_oLetters.GetCount(); ++index)
					{
						Gdiplus::GraphicsPath* pPath = m_oLetters[index]->GetGraphicsPath();

						CTextPainterLetterTransform& oTransform = m_oLetters[index]->GetTransform();

						Gdiplus::GraphicsState oStateInitial = oGraphics.Save();
						
						// применяем нужные трансформы для grpahics'а, чтобы отрисовать букву там, где надо
						CTextTransforms oAnimationTransforms;
						oTransform.SetTransformToGraphics(oGraphics, dAnimation, oAnimationTransforms);

						BOOL bShadowVisible = m_pPaintState->GetShadow().GetVisible();
						int nShadowDistanceX = int(m_pPaintState->GetShadow().GetDistanceX());
						int nShadowDistanceY = int(m_pPaintState->GetShadow().GetDistanceY());
						int nShadowBlurSize = int(m_pPaintState->GetShadow().GetBlurSize());
						int nShadowColor = m_pPaintState->GetShadow().GetColor();
						int nShadowAlpha = m_pPaintState->GetShadow().GetAlpha();

						// если в анимации есть трансформы, соответствующие изменениям объекта shadow, то их надо применить
						if (TRUE)
						{
							int nIndex = -1;

							nIndex = oAnimationTransforms.FindTransform(CTextTransform::c_nTransformShadowColor);
							if (nIndex >= 0)
							{
								int nNewColor = (int)oAnimationTransforms[nIndex]->GetValue(0);
								if (nNewColor > -1)
									nShadowColor = nNewColor;

								int nNewAlpha = (BYTE)oAnimationTransforms[nIndex]->GetValue(1);
								if (nNewAlpha > -1)
									nShadowAlpha = nNewAlpha;
							}

							nIndex = oAnimationTransforms.FindTransform(CTextTransform::c_nTransformShadowPosition);
							if (nIndex >= 0)
							{
								nShadowDistanceX = (int)oAnimationTransforms[nIndex]->GetValue(0);
								nShadowDistanceY = (int)oAnimationTransforms[nIndex]->GetValue(1);
							}

							nIndex = oAnimationTransforms.FindTransform(CTextTransform::c_nTransformShadowSize);
							if (nIndex >= 0)
							{
								int nNewVisible = (int)oAnimationTransforms[nIndex]->GetValue(0);
								if (nNewVisible > -1)
									bShadowVisible = (BOOL)nNewVisible;

								int nNewBlurSize = (int)oAnimationTransforms[nIndex]->GetValue(1);
								if (nNewBlurSize > -1)
									nShadowBlurSize = nNewBlurSize;
							}

							nIndex = oAnimationTransforms.FindTransform(CTextTransform::c_nTransformMultiplyAlpha);
							if (nIndex >= 0)
								nShadowAlpha = (int)(nShadowAlpha*oAnimationTransforms[nIndex]->GetValue(0));
						}

						// отрисовываем только видимую тень
						if (bShadowVisible && nShadowAlpha > 0)
						{
							DWORD dwShadowColor = Gdiplus::Color::MakeARGB( nShadowAlpha, GetRValue(nShadowColor), GetGValue(nShadowColor), GetBValue(nShadowColor) );

							// сдвигаемся на отступ, указанный в настройках тени
							oGraphics.TranslateTransform((float)nShadowDistanceX, (float)nShadowDistanceY, Gdiplus::MatrixOrderAppend);

							int nMargin = 5;
							
							Gdiplus::RectF oRectLetter = m_oLetters[index]->GetRectBounds();

							int w = int(oRectLetter.Width  + 0.5f) + 2*(nMargin + nShadowBlurSize);
							int h = int(oRectLetter.Height + 0.5f) + 2*(nMargin + nShadowBlurSize);

							Gdiplus::Bitmap oTempBitmap( w, h);

							if( true )
							{
								Gdiplus::Graphics pGraphicsTemp(&oTempBitmap);

								pGraphicsTemp.TranslateTransform(nShadowBlurSize + nMargin - oRectLetter.X, nShadowBlurSize + nMargin - oRectLetter.Y, MatrixOrderAppend);
								pGraphicsTemp.Clear(Color(0, 0, 0, 0));
								pGraphicsTemp.SetInterpolationMode(InterpolationModeBilinear);
								pGraphicsTemp.SetSmoothingMode(SmoothingModeAntiAlias);
								
								if( nShadowBlurSize > 0 )
									oShadowBrush.SetColor( Gdiplus::Color::Blue ); // синий цвет менять нельзя!!!
								else
									oShadowBrush.SetColor( Gdiplus::Color( dwShadowColor ) );

								pGraphicsTemp.FillPath(&oShadowBrush, pPath);
							}
							
							GDIPLUS_ShadowBlur(oTempBitmap, nShadowBlurSize, dwShadowColor);

							oGraphics.SetInterpolationMode(InterpolationModeBilinear);
							oGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
							
							float dstX = oRectLetter.X - nShadowBlurSize - nMargin;
							float dstY = oRectLetter.Y - nShadowBlurSize - nMargin;
							float srcW = float(w);
							float srcH = float(h);
							oGraphics.DrawImage(&oTempBitmap, dstX, dstY, srcW, srcH );
						}

						oGraphics.Restore(oStateInitial);
					}
				}

				void TextDrawEdge(Gdiplus::Graphics& oGraphics, Gdiplus::GraphicsPath* pLetterPath, CTextTransforms& oAnimationTransforms)
				{
					BOOL bEdgeVisible = m_pPaintState->GetEdge().Visible;
					int nEdgeSize = int(m_pPaintState->GetEdge().Dist);
					int nEdgeColor = m_pPaintState->GetEdge().Color;
					int nEdgeAlpha = m_pPaintState->GetEdge().Alpha;

					// если в анимации есть трансформы, соответствующие изменениям объекта edge, то их надо применить
					if (TRUE)
					{
						int nIndex = -1;

						nIndex = oAnimationTransforms.FindTransform(CTextTransform::c_nTransformEdgeColor);
						if (nIndex >= 0)
						{
							int nNewEdgeColor = (int)oAnimationTransforms[nIndex]->GetValue(0);
							if (nNewEdgeColor > -1)
								nEdgeColor = nNewEdgeColor;

							int nNewEdgeAlpha = (BYTE)oAnimationTransforms[nIndex]->GetValue(1);
							if (nNewEdgeAlpha > - 1)
								nEdgeAlpha = nNewEdgeAlpha;
						}

						nIndex = oAnimationTransforms.FindTransform(CTextTransform::c_nTransformEdgeSize);
						if (nIndex >= 0)
						{
							int nNewEdgeVisible = (int)oAnimationTransforms[nIndex]->GetValue(0);
							if (nNewEdgeVisible > -1)
								bEdgeVisible = (BOOL)nNewEdgeVisible;

							int nNewEdgeSize = (int)oAnimationTransforms[nIndex]->GetValue(1);
							if (nNewEdgeSize > -1)
								nEdgeSize = nNewEdgeSize;
						}

						nIndex = oAnimationTransforms.FindTransform(CTextTransform::c_nTransformMultiplyAlpha);
						if (nIndex >= 0)
							nEdgeAlpha = (int)(nEdgeAlpha*oAnimationTransforms[nIndex]->GetValue(0));
					}

					// если указан edge, то отрисовываем границу текста указанным edge'ем
					if (bEdgeVisible && nEdgeSize > 0)
					{
						Gdiplus::Color oEdgeColor(nEdgeAlpha, GetRValue(nEdgeColor), GetGValue(nEdgeColor), GetBValue(nEdgeColor));
						
						Gdiplus::Pen oEdgePen(oEdgeColor, (float)nEdgeSize);

						oEdgePen.SetLineJoin(Gdiplus::LineJoinRound);
										
						oGraphics.DrawPath(&oEdgePen, pLetterPath);
					}
				}
				void TextDrawLetter(Gdiplus::Graphics& oGraphics, Gdiplus::GraphicsPath* pLetterPath, Gdiplus::RectF& oLetterRect, CTextTransforms& oAnimationTransforms)
				{
					Painter::CBrush oBrush = m_pPaintState->GetBrush();

					// если в анимации есть трансформы, соответствующие изменениям объекта letter, то их надо применить
					if (TRUE)
					{
						int nIndex = -1;

						nIndex = oAnimationTransforms.FindTransform(CTextTransform::c_nTransformTextColor1);
						if (nIndex >= 0)
						{
							int nTextColor1 = (int)oAnimationTransforms[nIndex]->GetValue(0);
							if (nTextColor1 > -1)
								oBrush.SetColor1(nTextColor1);

							int nTextAlpha1 = (int)oAnimationTransforms[nIndex]->GetValue(1);
							if (nTextAlpha1 > -1)
							{
								if( nTextAlpha1 > 255 ) nTextAlpha1 = 255;

								oBrush.SetAlpha1(nTextAlpha1);
							}
						}

						nIndex = oAnimationTransforms.FindTransform(CTextTransform::c_nTransformTextColor2);
						if (nIndex >= 0)
						{
							int nTextColor2 = (int)oAnimationTransforms[nIndex]->GetValue(0);
							if (nTextColor2 > -1)
								oBrush.SetColor2(nTextColor2);

							int nTextAlpha2 = (int)oAnimationTransforms[nIndex]->GetValue(1);
							if (nTextAlpha2 > -1)
							{
								if( nTextAlpha2 > 255 ) nTextAlpha2 = 255;
								
								oBrush.SetAlpha2(nTextAlpha2);
							}
						}

						nIndex = oAnimationTransforms.FindTransform(CTextTransform::c_nTransformMultiplyAlpha);
						if (nIndex >= 0)
						{
							float fAlphaCoef = oAnimationTransforms[nIndex]->GetValue(0);
							if( fAlphaCoef < 0 ) fAlphaCoef = 0;
							else if( fAlphaCoef > 1 ) fAlphaCoef = 1;

							oBrush.SetTextureAlpha(int(oBrush.GetTextureAlpha()*fAlphaCoef));
							oBrush.SetAlpha1(int(oBrush.GetAlpha1()*fAlphaCoef));
							oBrush.SetAlpha2(int(oBrush.GetAlpha2()*fAlphaCoef));
						}
					}

					// подсчитываем размер текста на экране для того, чтобы сделать корректную градиентную заливку
					Gdiplus::Brush* pBrush = NULL;
					
					int nBrushType = Painter::ConstantCompatible(oBrush.GetType());
					if (nBrushType == Painter::c_BrushTypePathGradient1 ||
					    nBrushType == Painter::c_BrushTypePathGradient2)
					{
						// TODO: рассмотреть дополнительные случаи с path gradient brush
						pBrush = oBrush.GetBrush(m_pTextureManager, &oLetterRect);
					}
					else
						pBrush = oBrush.GetBrush(m_pTextureManager, &m_oRectBounds);

					// отрисовываем букву...
					if( pBrush )
						oGraphics.FillPath(pBrush, pLetterPath);
				}
				
				BOOL GetTextPaths(BSTR bstrText, int nTextLength, Gdiplus::Font* pFont, CSimpleArray<Gdiplus::GraphicsPath*>& arrPaths)
				{
					return GetTextPaths(bstrText, nTextLength, pFont, 0, 0, 65000, 65000, Gdiplus::StringAlignmentNear, Gdiplus::StringAlignmentNear, arrPaths);

					/*
					// функция возвращает массив GraphicsPath - путей, описывающих буквы
					arrPaths.RemoveAll();

					if (nTextLength < 1 || NULL == pFont)
						return FALSE;

					Gdiplus::GraphicsPath oTextPath;

					// вначале отрисовываем текст в path
					if (TRUE)
					{
						Gdiplus::FontFamily oFontFamily;
						pFont->GetFamily(&oFontFamily);
						
						Gdiplus::StringFormat oStringFormat;
						oStringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
						oStringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);

						oTextPath.AddString(bstrText, nTextLength, &oFontFamily, pFont->GetStyle(), pFont->GetSize(), PointF(0, 0), &oStringFormat);
					}

					int nTotalPoints = oTextPath.GetPointCount();
					BYTE* pPointsTypes = new BYTE[nTotalPoints];
					if (NULL == pPointsTypes)
						return FALSE;
					oTextPath.GetPathTypes(pPointsTypes, nTotalPoints);

					Gdiplus::PointF* pPointsData = new Gdiplus::PointF[nTotalPoints];
					if (NULL == pPointsData)
					{
						delete[] pPointsTypes;
						return FALSE;
					}
					oTextPath.GetPathPoints(pPointsData, nTotalPoints);

					// считаем последовательности точек и типов от nMarkerType до nMarkerType
					int nIndexStart = 0;
					int nIndexEnd = 1;

					int nMarkerType = (int)(Gdiplus::PathPointTypeCloseSubpath | Gdiplus::PathPointTypePathMarker);

					while (nIndexStart < nTotalPoints)
					{
						BOOL bCreatePath = FALSE;

						if (nIndexEnd >= nTotalPoints)
						{
							nIndexEnd = nTotalPoints - 1;
							bCreatePath = TRUE;
						}
						else if ((pPointsTypes[nIndexEnd] & nMarkerType) == nMarkerType)
							bCreatePath = TRUE;

						if (bCreatePath && nIndexEnd > nIndexStart)
						{
							Gdiplus::GraphicsPath* pPath = new Gdiplus::GraphicsPath(pPointsData + nIndexStart, pPointsTypes + nIndexStart, nIndexEnd - nIndexStart + 1, oTextPath.GetFillMode());

							if (NULL != pPath)
								arrPaths.Add(pPath);
						}

						if (bCreatePath)
						{
							nIndexStart = nIndexEnd + 1;
							nIndexEnd = nIndexStart + 1;
							bCreatePath = FALSE;
						}

						nIndexEnd++;
					}

					delete[] pPointsTypes;
					delete[] pPointsData;

					return TRUE;
					*/
				}
				BOOL GetTextPaths(BSTR bstrText, int nTextLength, Gdiplus::Font* pFont, double dLeft, double dTop, double dRight, double dBottom, int nAlignVertical, int nAlignHorizontal, CSimpleArray<Gdiplus::GraphicsPath*>& arrPaths)
				{
					// функция возвращает массив GraphicsPath - путей, описывающих буквы
					arrPaths.RemoveAll();

					if (nTextLength < 1 || NULL == pFont)
						return FALSE;

					Gdiplus::GraphicsPath oTextPath;

					// вначале отрисовываем текст в path
					if (TRUE)
					{
						Gdiplus::FontFamily oFontFamily;
						pFont->GetFamily(&oFontFamily);
						
						Gdiplus::StringFormat oStringFormat;
						oStringFormat.SetAlignment((Gdiplus::StringAlignment)nAlignHorizontal);
						oStringFormat.SetLineAlignment((Gdiplus::StringAlignment)nAlignVertical);

						Gdiplus::RectF oRectText;
						oRectText.X = (float)min(dLeft, dRight);
						oRectText.Y = (float)min(dTop, dBottom);
						oRectText.Width = (float)fabs(dRight - dLeft);
						oRectText.Height = (float)fabs(dBottom - dTop);

						oTextPath.AddString(bstrText, nTextLength, &oFontFamily, pFont->GetStyle(), pFont->GetSize(), oRectText, &oStringFormat);
					}

					int nTotalPoints = oTextPath.GetPointCount();
					BYTE* pPointsTypes = new BYTE[nTotalPoints];
					if (NULL == pPointsTypes)
						return FALSE;
					oTextPath.GetPathTypes(pPointsTypes, nTotalPoints);

					Gdiplus::PointF* pPointsData = new Gdiplus::PointF[nTotalPoints];
					if (NULL == pPointsData)
					{
						delete[] pPointsTypes;
						return FALSE;
					}
					oTextPath.GetPathPoints(pPointsData, nTotalPoints);

					// считаем последовательности точек и типов от nMarkerType до nMarkerType
					int nIndexStart = 0;
					int nIndexEnd = 1;

					int nMarkerType = (int)(Gdiplus::PathPointTypeCloseSubpath | Gdiplus::PathPointTypePathMarker);

					while (nIndexStart < nTotalPoints)
					{
						BOOL bCreatePath = FALSE;

						if (nIndexEnd >= nTotalPoints)
						{
							nIndexEnd = nTotalPoints - 1;
							bCreatePath = TRUE;
						}
						else if ((pPointsTypes[nIndexEnd] & nMarkerType) == nMarkerType)
							bCreatePath = TRUE;

						if (bCreatePath && nIndexEnd > nIndexStart)
						{
							Gdiplus::GraphicsPath* pPath = new Gdiplus::GraphicsPath(pPointsData + nIndexStart, pPointsTypes + nIndexStart, nIndexEnd - nIndexStart + 1, oTextPath.GetFillMode());

							if (NULL != pPath)
								arrPaths.Add(pPath);
						}

						if (bCreatePath)
						{
							nIndexStart = nIndexEnd + 1;
							nIndexEnd = nIndexStart + 1;
							bCreatePath = FALSE;
						}

						nIndexEnd++;
					}

					delete[] pPointsTypes;
					delete[] pPointsData;

					return TRUE;
				}
				BOOL GetTextLines(CTextPainterLetters& oLetters, CTextPainterLines& oLines)
				{
					oLines.RemoveAll();

					if (oLetters.GetCount() < 1)
						return TRUE;

					CTextPainterLine* pLastLine = new CTextPainterLine();
					if (NULL == pLastLine)
						return FALSE;

					pLastLine->SetIndexStart(0);
					pLastLine->SetIndexEnd(0);
					oLines.Add(pLastLine);

					double dLeft = oLetters[0]->GetRectBounds().GetLeft();
					double dTop = oLetters[0]->GetRectBounds().GetTop();
					double dRight = oLetters[0]->GetRectBounds().GetRight();
					double dBottom = oLetters[0]->GetRectBounds().GetBottom();

					for (int index = 1; index < oLetters.GetCount(); ++index)
					{
						double dLetterCenterY = 0.5*(oLetters[index]->GetRectBounds().GetTop() + oLetters[index]->GetRectBounds().GetBottom());

						// проверяем, попадает ли центр текущей буквы по высоте в bounding box от текущей линии
						if (dLetterCenterY >= dTop && dLetterCenterY <= dBottom)
						{
							pLastLine->SetIndexEnd(index);

							dLeft = min(dLeft, oLetters[index]->GetRectBounds().GetLeft());
							dTop = min(dTop, oLetters[index]->GetRectBounds().GetTop());
							dRight = max(dRight, oLetters[index]->GetRectBounds().GetRight());
							dBottom = max(dBottom, oLetters[index]->GetRectBounds().GetBottom());

							continue;
						}

						// если не попадает - значит буква на "новой линии"
						pLastLine->GetRectBounds().X = (float)dLeft;
						pLastLine->GetRectBounds().Y = (float)dTop;
						pLastLine->GetRectBounds().Width = (float)(dRight - dLeft);
						pLastLine->GetRectBounds().Height = (float)(dBottom - dTop);

						pLastLine = new CTextPainterLine(); 
						if (NULL == pLastLine)
							return FALSE;

						dLeft = oLetters[index]->GetRectBounds().GetLeft();
						dTop = oLetters[index]->GetRectBounds().GetTop();
						dRight = oLetters[index]->GetRectBounds().GetRight();
						dBottom = oLetters[index]->GetRectBounds().GetBottom();

						pLastLine->SetIndexStart(index);
						pLastLine->SetIndexEnd(index);
						oLines.Add(pLastLine);
					}

					pLastLine->SetIndexEnd(oLetters.GetCount() - 1);

					pLastLine->GetRectBounds().X = (float)dLeft;
					pLastLine->GetRectBounds().Y = (float)dTop;
					pLastLine->GetRectBounds().Width = (float)(dRight - dLeft);
					pLastLine->GetRectBounds().Height = (float)(dBottom - dTop);

					return TRUE;
				}
				BOOL GetPathBoundingRect(Gdiplus::GraphicsPath* pPath, Gdiplus::RectF& oBounds)
				{
					// прямоугольник инвалидный
					oBounds.X = -65001.0f;
					oBounds.Y = -65001.0f;
					oBounds.Width = 0.0f;
					oBounds.Height = 0.0f;

					if (NULL == pPath)
						return FALSE;

					int nTotalPoints = pPath->GetPointCount();
					if (nTotalPoints < 1)
						return FALSE;

					Gdiplus::PointF* pPointsData = new Gdiplus::PointF[nTotalPoints];
					if (NULL == pPointsData)
						return FALSE;
					pPath->GetPathPoints(pPointsData, nTotalPoints);

					double dLeft =    100000;
					double dRight =  -100000;
					double dTop =     100000;
					double dBottom = -100000;

					for (int index = 0; index < nTotalPoints; ++index)
					{
						dLeft = min(dLeft, pPointsData[index].X);
						dTop = min(dTop, pPointsData[index].Y);
						dRight = max(dRight, pPointsData[index].X);
						dBottom = max(dBottom, pPointsData[index].Y);
					}

					if (dRight < dLeft || dBottom < dTop)
					{
						delete[] pPointsData;
						return FALSE;
					}

					oBounds.X = (float)dLeft;
					oBounds.Y = (float)dTop;
					oBounds.Width = (float)(dRight - dLeft);
					oBounds.Height = (float)(dBottom - dTop);

					delete[] pPointsData;

					return TRUE;
				}
				BOOL TextUpdateBoundingBox()
				{
					double dLeft, dTop, dRight, dBottom;

					if (!m_oLetters.GetRectBounds(dLeft, dTop, dRight, dBottom))
						return FALSE;

					m_oRectBounds.X = (float)dLeft;
					m_oRectBounds.Y = (float)dTop;
					m_oRectBounds.Width = (float)(dRight - dLeft);
					m_oRectBounds.Height = (float)(dBottom - dTop);

					return TRUE;
				}
				
				// функции ShadowBlur заточены для прорисовки тени и не могут быть 
				// использованы в других целях в отличие от функций GaussianBlur
				static void GDIPLUS_ShadowBlur(Gdiplus::Bitmap& oBitmap, int nBlurSize, DWORD dwShadowColor)
				{
					if( nBlurSize < 1 )
					{
						return;
					}

					Gdiplus::BitmapData oBitmapData;
					
					if( Gdiplus::Ok == oBitmap.LockBits(NULL, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &oBitmapData) )
					{
						BGRA_ShadowBlur((BYTE*)oBitmapData.Scan0, oBitmapData.Width, oBitmapData.Height, oBitmapData.Stride, nBlurSize, dwShadowColor);

						oBitmap.UnlockBits(&oBitmapData);
					}
				}
				static void BGRA_ShadowBlur( BYTE* pBGRA, int nWidth, int nHeight, int nStride, int nBlurSize, DWORD dwShadowColor )
				{
					if( !pBGRA || nWidth < 1 || nHeight < 1 || nStride < nWidth*4 || nBlurSize < 1 )
					{
						return;
					}

					int nBufStride = nWidth * 4;
					BYTE* pBuffer  = new BYTE[nBufStride * nHeight];
					if( !pBuffer )
					{
						return;
					}

					FillZeroLeftRightEdge( pBuffer, nBlurSize, nHeight, nBufStride, nWidth - nBlurSize );
					BGRA_ShadowBlurVertical(pBGRA, pBuffer, nWidth, nHeight, nStride, nBufStride, nBlurSize);
					BGRA_ShadowBlurHorizontal(dwShadowColor, pBuffer, pBGRA, nWidth, nHeight, nBufStride, nStride, nBlurSize);

					delete[] pBuffer;
				}
				static BOOL BGRA_ShadowBlurHorizontal( DWORD dwColor, BYTE* pSrcData, BYTE* pDstData, int nWidth, int nHeight, int nSrcStride, int nDstStride, int nBlurSize )
				{
					if( !pSrcData || !pDstData || nWidth < 1 || nHeight < 1 || nSrcStride < 1 || nDstStride < 1 || nBlurSize < 1 )
					{
						return FALSE;
					}

					int nLength;
					int nA, nR, nG, nB;
					int nClr;
					DWORD nFactor;
					DWORD nClrFactor;
					DWORD *pSrc, *pDst;

					nA = (dwColor >> 24 & 0xff);
					nR = (dwColor >> 16 & 0xff);
					nG = (dwColor >> 8  & 0xff);
					nB = (dwColor >> 0  & 0xff);
					nFactor = (nBlurSize << 1) + 1;
					nFactor *= nFactor;

					for( ; nHeight > 0; --nHeight, pSrcData += nSrcStride, pDstData += nDstStride )
					{
						pSrc = (DWORD*)pSrcData;
						pDst = (DWORD*)pDstData;
						nClr = 0;

						// подсчёт начальной суммы
						nLength = nBlurSize < nWidth ? nBlurSize : nWidth - 1;
						for( ; nLength >= 0; --nLength )
						{
							nClr += pSrc[nLength];
						}

						// расчёт новых пикселов в строке
						nLength = nWidth;
						for( ;; )
						{
							nClrFactor = nClr / nFactor + 1;
							dwColor  = (nB * nClrFactor >> 8) << 0;
							dwColor |= (nG * nClrFactor >> 8) << 8;
							dwColor |= (nR * nClrFactor >> 8) << 16;
							dwColor |= (nA * nClrFactor >> 8) << 24;

							*pDst = dwColor;

							// проверка на выход из цикла
							if( nLength <= 1 )
							{
								break;
							}

							// вычитаем крайнее левое значение
							if( nLength <= nWidth - nBlurSize )
							{
								nClr -= pSrc[-nBlurSize];
							}

							// переходим на следующий пиксел
							--nLength;
							++pSrc; 
							++pDst;
							
							// прибавляем следующее крайнее правое значение
							if( nLength > nBlurSize )
							{
								nClr += pSrc[nBlurSize];
							}
						}
					}

					return TRUE;
				}
				static BOOL BGRA_ShadowBlurVertical(BYTE* pSrcData, BYTE* pDstData, int nWidth, int nHeight, int nSrcStride, int nDstStride, int nBlurSize )
				{
					if( !pSrcData || !pDstData || nWidth < 1 || nHeight < 1 || nSrcStride < 1 || nDstStride < 1 || nBlurSize < 1 )
					{
						return FALSE;
					}

					int nLength;
					DWORD nClr;
					BYTE *pSrc, *pDst;
					
					pSrcData += nBlurSize*4;
					pDstData += nBlurSize*4;
					for( nWidth -= nBlurSize*2; nWidth > 0; --nWidth, pSrcData += 4, pDstData += 4 )
					{
						pSrc = pSrcData;
						pDst = pDstData;
						nClr = 0;

						// подсчёт начальной суммы
						nLength = nBlurSize < nHeight ? nBlurSize : nHeight - 1;
						for( nLength *= nSrcStride; nLength >= 0; nLength -= nSrcStride )
						{
							nClr += *(DWORD*)(pSrc + nLength)&0xff;
						}

						// расчёт новых пикселов в строке
						nLength = nHeight;
						for( ;; )
						{
							*(DWORD*)(pDst) = nClr;

							// проверка на выход из цикла
							if( nLength <= 1 )
							{
								break;
							}

							// вычитаем крайнее верхнее значение
							if( nLength <= nHeight - nBlurSize )
							{
								nClr -= *(DWORD*)(pSrc - nBlurSize*nSrcStride)&0xff;
							}

							// переходим на следующий пиксел
							--nLength;
							pSrc += nSrcStride;
							pDst += nDstStride;
							
							// прибавляем следующее крайнее нижнее значение
							if( nLength > nBlurSize )
							{
								nClr += *(DWORD*)(pSrc + nBlurSize*nSrcStride)&0xff;
							}
						}
					}

					return TRUE;
				}
				static void FillZeroLeftRightEdge( BYTE* pARGBData, int nEdgeWidth, int nEdgeHeight, int nStride, int nPixelOffset )
				{
					DWORD* pDst;
					int nWidth;

					for( nEdgeHeight; nEdgeHeight > 0; --nEdgeHeight, pARGBData += nStride )
					{
						pDst = (DWORD*)pARGBData;
						for( nWidth = nEdgeWidth; nWidth > 0; --nWidth, ++pDst )
						{
							pDst[0] = 0;
							pDst[nPixelOffset] = 0;
						}
					}
				}
				

			protected:
				
				CTextPainterLetters m_oLetters; // массив букв текста для отрисовки
				
				CPaintState* m_pPaintState; // настройки отрисовщика текста
				Painter::CTextureManager* m_pTextureManager; // настройки текстур для brush'ей

				Gdiplus::RectF m_oRectBounds; // описанный около текста прямоугольник
				double m_dTextLineSpacing; // межстрочный интервал для корректной отрисовки текста по вертикали

				Gdiplus::RectF m_oClipRect; // прямоугольник для отсечения
				BOOL m_bClipping;
				int  m_nHorAlign;
				int  m_nVerAlign;

				float m_fAlphaFadeFactorMin;
				float m_fAlphaFadeFactorMax;

				float m_fTextFactorY;
			};
		}
	}
}