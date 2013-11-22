#ifndef __RESIZE_COMM_
#define __RESIZE_COMM_

#include "..//CommandManager.h"
#include "..//ViewManager.h"
/************************************************************************/
/* Resize COMMAND   -----   ID = 103                                    */
/************************************************************************/

#include "AVSUtils.h"
#include <math.h>
#include "Commands/CommandsUtils.h"

/*#include "ImageTransformsCoreBGRA.h"
using namespace ImageStudio;
using namespace ImageStudio::Transforms;
using namespace ImageStudio::Transforms::Core;
using namespace ImageStudio::Transforms::Core::BGRA;*/

class CResizeCommand : public CCommand, public IChangesSubscriber
{
public:
	CResizeCommand(IAppManager* app_manager):CCommand(app_manager)
	{
		m_lType = c_nResizeStretchBillinear;
		//m_lType = c_nResizeShrinkBillinear;
		m_lColor = 0x000000;

		m_pImage = NULL;

		m_dAspect = 0;
		UpdateAspect();

		GetAppManager()->GetLayersManager()->GetCurrentLayer()->AddSubscriber(this);
		
		const BYTE* pBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, m_oTrack.m_lWidth, 
			m_oTrack.m_lHeight);

		m_lWidth = m_oTrack.m_lWidth;
		m_lHeight = m_oTrack.m_lHeight;
		
		GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(m_oTrack.m_lWidth);
		GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(m_oTrack.m_lHeight);

		AddParameter(_T("Width"), _T("Width Value"), (int)m_lWidth, 10, 2000);
		AddParameter(_T("Height"), _T("Height Value"), (int)m_lHeight, 10, 2000);
		AddParameter(_T("Type"), _T("Type Value"), (int)m_lType, 10, 2000);
		AddParameter(_T("Color"), _T("Color Value"), (int)m_lColor, 0, 0xFFFFFF);
		AddParameter(_T("AspectTrack"), _T("AspectTrack Value"), (float)m_oTrack.m_dAspectTrack, -1.0f, 10.0f);
		
		
		//GetAppManager()->GetView()->FitImageToWindow();
		SetInvalidateFlag(INVALID_GRAPHICS);
	}
	virtual ~CResizeCommand() 
	{
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->RemoveSubscriber(this);
		RELEASEOBJECT(m_pImage);
	}
public:
	
	virtual void OnMouseDown(LONG nX, LONG nY, LONG nButton)	
	{
		m_oTrack.OnMouseDown(nX, nY, nButton);
	}
	virtual void OnMouseUp(LONG nX, LONG nY, LONG nButton)
	{
		m_oTrack.OnMouseUp(nX, nY, nButton);
	}
	virtual void OnMouseMove(LONG nX, LONG nY, LONG nButton)	
	{
		long oldW = m_oTrack.m_lWidth;
		long oldH = m_oTrack.m_lHeight;

		m_oTrack.OnMouseMove(nX, nY);

		m_lWidth = m_oTrack.m_lWidth;
		m_lHeight = m_oTrack.m_lHeight;
		GetAppManager()->GetCoordinateTransformer()->ScreenDistanceToImageDistance(m_lWidth);
		GetAppManager()->GetCoordinateTransformer()->ScreenDistanceToImageDistance(m_lHeight);
		if (oldW != m_oTrack.m_lWidth)
			FireChangeParameter(0, (int)m_lWidth);
		if (oldH != m_oTrack.m_lHeight)
			FireChangeParameter(1, (int)m_lHeight);
		
		SetInvalidateFlag(INVALID_GRAPHICS);
	}
	virtual void OnChangeParameter(int paramInd) 
	{
		if (paramInd == 0 && m_params.GetCount() > 0)
		{
			m_oTrack.m_lWidth = m_params[0]->m_value.lVal;
			m_lWidth = m_oTrack.m_lWidth;
			GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(m_oTrack.m_lWidth);
		}
		else if (paramInd == 1 && m_params.GetCount() > 1)
		{
			m_oTrack.m_lHeight = m_params[1]->m_value.lVal;
			m_lHeight = m_oTrack.m_lHeight;
			GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(m_oTrack.m_lHeight);
		}
		else if (paramInd == 2 && m_params.GetCount() > 2)
		{
			m_lType = m_params[2]->m_value.lVal;
		}
		else if (paramInd == 3 && m_params.GetCount() > 3)
		{
			m_lColor = m_params[3]->m_value.lVal;
		}
		else if (paramInd == 4 && m_params.GetCount() > 4)
		{
			m_dAspect = m_params[4]->m_value.dblVal;
			if (0.0 == m_dAspect)
			{
				m_dAspect = (double)m_oTrack.m_lWidth / m_oTrack.m_lHeight;
			}
			UpdateAspect();
		}
		else
		{
			return;
		}

		//SetInvalidateFlag(INVALID_LAYER);
	}

	virtual void ChangesHandler(int changingObject, bool repaint)
	{
		if (changingObject==ILayer::FAST_COPY || changingObject==ILayer::SLOW_COPY)
		{
			delete m_pImage;
			m_pImage = NULL;
			SetInvalidateFlag(INVALID_GRAPHICS);
		}
	}

	virtual void DrawOnGraphics(Gdiplus::Graphics* pGr, const CRect& rect)
	{
		Gdiplus::SolidBrush oBrushBcks(GetAppManager()->GetView()->GetBackgroundColor());
		pGr->FillRectangle(&oBrushBcks,GetAppManager()->GetView()->GetImageRect());
		if (NULL == m_pImage)
		{
			long lWidth = 0; long lHeight = 0;
			double scaleCoef;
			const BYTE* pBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetActualImage(lWidth, lHeight,scaleCoef);

			m_pImage = new Bitmap(lWidth, lHeight, 4*lWidth, PixelFormat32bppARGB, const_cast<BYTE*>(pBuffer));

			/*m_oTrack.m_lWidth = rect.right - rect.left;
			m_oTrack.m_lHeight = rect.bottom - rect.top;*/

			UpdateAspect();
		}
		
		if (NULL != pGr && NULL != m_pImage)
		{
			m_oTrack.m_oWndRect.left = rect.left;
			m_oTrack.m_oWndRect.top = rect.top;
			m_oTrack.m_oWndRect.right = rect.right;
			m_oTrack.m_oWndRect.bottom = rect.bottom;
			
			// вот здесь вместо clear - нужно рисовать подложку
			//ImageRendering::CChessBrush oAlphaBrush;
			//oAlphaBrush.OnPaint(pGr, Rect(rect.left, rect.top, rect.Width(), rect.Height()));
			pGr->Clear(Color::Black);

			// выделяем трек...
			Rect oTrackRect;
			m_oTrack.GetTrackRect(&oTrackRect);

			Color oBackColor((BYTE)(m_lColor >> 16), (BYTE)(m_lColor >> 8), (BYTE)(m_lColor)); 
			SolidBrush oBackBrush(oBackColor);
			pGr->FillRectangle(&oBackBrush, oTrackRect);

			if (c_nResizeShrinkBillinear == m_lType)
			{
				long lWidth = 0; long lHeight = 0;
				const BYTE* pBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, lWidth, lHeight);
				
				double dScaleX = (double)m_oTrack.m_lWidth / lWidth;
				double dScaleY = (double)m_oTrack.m_lHeight / lHeight;

				Rect oShrinkRect(oTrackRect.X, oTrackRect.Y, oTrackRect.Width, oTrackRect.Height);

				double dZoom = min(1.0, min(dScaleX, dScaleY));

				dScaleX = dZoom;
				dScaleY = dZoom;

				double dAspect = (double)lWidth / lHeight;

				double dW = dAspect * lHeight;
				dScaleX *= (dW / lWidth);

				int nNewWidthAsp = int(dScaleX * lWidth);
				if (nNewWidthAsp > m_oTrack.m_lWidth)
				{
					double dCorrect = (double)m_oTrack.m_lWidth / nNewWidthAsp;
					dScaleX *= dCorrect;
					dScaleY *= dCorrect;
				}

				if( m_oTrack.m_lHeight > dScaleY * lHeight )
				{
					int nOffset = (int)((m_oTrack.m_lHeight - dScaleY * lHeight) / 2.0);
					oShrinkRect.Y += nOffset;
					oShrinkRect.Height -= 2 * nOffset;
				}
				if( m_oTrack.m_lWidth > dScaleX * lWidth )
				{
					int nOffset = (int)((m_oTrack.m_lWidth - dScaleX * lWidth) / 2.0);
					oShrinkRect.X += nOffset;
					oShrinkRect.Width -= 2 * nOffset;
				}
				
				pGr->DrawImage(m_pImage, oShrinkRect);
			}
			else
			{
				pGr->DrawImage(m_pImage, oTrackRect);
			}

			m_oTrack.DrawOnGraphics(pGr);
		}
	}

	virtual bool DrawOnLayer()
	{
		long lSrcWidth = 0; long lSrcHeight = 0;
		const BYTE* pSrcBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, lSrcWidth, lSrcHeight);
		
		double dAspect = (double)lSrcWidth / lSrcHeight;

		double dScaleX = (double)m_lWidth / lSrcWidth;
		double dScaleY = (double)m_lHeight / lSrcHeight;

		BYTE* pDstBuffer = NULL;

		m_lWidth = max(1, m_lWidth);
		m_lHeight = max(1, m_lHeight);

		try
		{
			pDstBuffer = new BYTE[m_lWidth * m_lHeight * 4];
			BGRA_TransformResize_(const_cast<BYTE*>(pSrcBuffer), lSrcWidth, lSrcHeight, pDstBuffer, m_lWidth, m_lHeight, m_lColor, m_lType, dAspect, 0);
		}
		catch (...)
		{ 
			//::MessageBox(NULL,"Not enough memory","Error", MB_OK|MB_ICONERROR);
			return false;
		}

		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StartHistoryBlock();
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangeAllLayer(pDstBuffer, m_lWidth, m_lHeight);
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StopHistoryBlock(_T("Resize"));

		try
		{
			GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangeImage(ILayer::SLOW,this);
		}
		catch (...)
		{ 
			//::MessageBox(NULL,"Not enough memory","Error", MB_OK|MB_ICONERROR);
			return false;
		}
		

		//GetAppManager()->GetView()->FitImageToWindow();

		return true;
	}

public:

	void UpdateAspect()
	{
		m_oTrack.m_bIsTrackDoubleEnabled = TRUE;
		m_oTrack.m_dAspectTrack = m_dAspect;
		if (0 == m_oTrack.m_dAspectTrack)
		{
			if (NULL == m_pImage)
			{
				m_oTrack.m_dAspectTrack = -1.0;
				return;
			}

			double dW = (double)m_pImage->GetWidth();
			double dH = (double)m_pImage->GetHeight();
			m_oTrack.m_dAspectTrack = dW / dH;
			m_oTrack.m_bIsTrackDoubleEnabled = FALSE;
		}
		else if (0 < m_oTrack.m_dAspectTrack)
		{
			if (NULL == m_pImage)
			{
				m_oTrack.m_dAspectTrack = -1.0;
				return;
			}

			m_oTrack.m_bIsTrackDoubleEnabled = FALSE;
		}
	}

	class CTrackRectangle
	{
	public:
		RECT m_oWndRect; 
		long m_lWidth;
		long m_lHeight;

		Gdiplus::Pen m_oPen;
		int m_nEpsilon;

		POINT m_oCursor;

		bool m_bIsMove;
		int m_nFlagOld;

		bool m_bIsTrackEnabled;

		// аспект для трэка
		double m_dAspectTrack;
		BOOL m_bIsTrackDoubleEnabled;

	public:
		CTrackRectangle() : m_oPen(Gdiplus::Color::White, 1)
		{
			m_oPen.SetDashStyle(Gdiplus::DashStyleSolid);
			
			m_oWndRect.left = 0; m_oWndRect.right = 500;
			m_oWndRect.top = 0; m_oWndRect.bottom = 500;

			m_lWidth = 100;
			m_lHeight = 100;

			m_nEpsilon = 5;

			m_oCursor.x = 0;
			m_oCursor.y = 0;

			m_bIsMove = false;
			m_bIsTrackEnabled = false;
			m_nFlagOld = -1;

			m_dAspectTrack = -1;
			m_bIsTrackDoubleEnabled = TRUE;
		}
		void DrawOnGraphics(Gdiplus::Graphics* pGr)
		{
			Gdiplus::Rect oRect;
			
			oRect.X = m_oWndRect.left + (INT)(m_oWndRect.right - m_oWndRect.left - m_lWidth) / 2;
			oRect.Y = m_oWndRect.top + (INT)(m_oWndRect.bottom - m_oWndRect.top - m_lHeight) / 2;
			oRect.Width = (INT)m_lWidth;
			oRect.Height = (INT)m_lHeight;
			
			if (!m_bIsTrackEnabled)
			{
				m_oPen.SetColor(Color(0x80, 0x80, 0x80));
				pGr->DrawRectangle(&m_oPen, oRect);
			}
			else
			{
				m_oPen.SetColor(Color(0xFF, 0xFF, 0xFF));
				pGr->DrawRectangle(&m_oPen, oRect);
			}

			long lLen = 14; long lOffset = 2;
			Gdiplus::Rect oRect2;
			oRect2.X = 0;
			oRect2.Y = 0;
			oRect2.Width = lLen;
			oRect2.Height = lLen;

			SolidBrush oBrush(Color(0x80, 0x80, 0x80));
			if (m_bIsTrackEnabled)
			{
				oBrush.SetColor(Color::White);
			}	
			
			pGr->ExcludeClip(oRect);		

			if ((m_lWidth > 3 * lLen) && (m_lHeight > 3 * lLen))
			{
				oRect2.X = oRect.X - lOffset; oRect2.Y = oRect.Y - lOffset;
				pGr->FillRectangle(&oBrush, oRect2);

				oRect2.X = oRect.X + oRect.Width + lOffset - lLen + 1; oRect2.Y = oRect.Y - lOffset;
				pGr->FillRectangle(&oBrush, oRect2);

				oRect2.X = oRect.X - lOffset; oRect2.Y = oRect.Y + oRect.Height + lOffset - lLen + 1;
				pGr->FillRectangle(&oBrush, oRect2);

				oRect2.X = oRect.X + oRect.Width + lOffset - lLen + 1; oRect2.Y = oRect.Y + oRect.Height + lOffset - lLen + 1;
				pGr->FillRectangle(&oBrush, oRect2);
			}

			if ((m_lWidth > 5 * lLen) && (m_lHeight > 5 * lLen))
			{
				long lX2 = (m_oWndRect.right + m_oWndRect.left - lLen) / 2; 
				long lY2 = (m_oWndRect.bottom + m_oWndRect.top - lLen) / 2;
				
				oRect2.X = lX2; oRect2.Y = oRect.Y - lOffset;
				pGr->FillRectangle(&oBrush, oRect2);

				oRect2.X = oRect.X + oRect.Width + lOffset - lLen + 1; oRect2.Y = lY2;
				pGr->FillRectangle(&oBrush, oRect2);

				oRect2.X = oRect.X - lOffset; oRect2.Y = lY2;
				pGr->FillRectangle(&oBrush, oRect2);

				oRect2.X = lX2; oRect2.Y = oRect.Y + oRect.Height + lOffset - lLen + 1;
				pGr->FillRectangle(&oBrush, oRect2);
			}

			pGr->ResetClip();
		}
		// 1 - left, 2 - top, 3 - right, 4 - bottom, 0 - In, -1 - nothing
		// 5 - left-top, 6 - top-right, 7 - right-bottom, 8 - bottom-left
		int GetChange(int nX, int nY)
		{
			int left = m_oWndRect.left + (INT)(m_oWndRect.right - m_oWndRect.left - m_lWidth) / 2;
			int top = m_oWndRect.top + (INT)(m_oWndRect.bottom - m_oWndRect.top - m_lHeight) / 2;
			int right = left + m_lWidth;
			int bottom = top + m_lHeight;
			
			if ((nX < (left - m_nEpsilon)) || (nX > (right + m_nEpsilon)) || 
						(nY < (top - m_nEpsilon)) || (nY > (bottom + m_nEpsilon)))
			{
				//SetCursor(LoadCursor(NULL, IDC_ARROW));
				return -1;
			}
			
			BOOL b1 = PointOnSegment(left, top, left, bottom, nX, nY, m_nEpsilon);
			BOOL b2 = PointOnSegment(left, top, right, top, nX, nY, m_nEpsilon);
			BOOL b3 = PointOnSegment(right, top, right, bottom, nX, nY, m_nEpsilon);
			BOOL b4 = PointOnSegment(left, bottom, right, bottom, nX, nY, m_nEpsilon);

			int nFlag = -1;
			if ((nX >= (left + m_nEpsilon)) && (nX <= (right - m_nEpsilon)) && 
						(nY >= (top + m_nEpsilon)) && (nY <= (bottom - m_nEpsilon)))
			{
				nFlag = 0;
			}

			if (b1 && b2 && m_bIsTrackDoubleEnabled) nFlag = 5;
			else if (b2 && b3 && m_bIsTrackDoubleEnabled) nFlag = 6;
			else if (b3 && b4 && m_bIsTrackDoubleEnabled) nFlag = 7;
			else if (b4 && b1 && m_bIsTrackDoubleEnabled) nFlag = 8;
			
			else if (b1 && (nY > top) && (nY < bottom)) nFlag = 1;
			else if (b2 && (nX > left) && (nX < right)) nFlag = 2;
			else if (b3 && (nY > top) && (nY < bottom)) nFlag = 3; 
			else if (b4 && (nX > left) && (nX < right)) nFlag = 4;

			HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();			
			switch (nFlag)
			{
				case 1:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE_H)));
					break;
				case 2:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE_V)));
					break;
				case 3:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE_H)));
					break;
				case 4:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE_V)));
					break;
				case 5:
					if (m_dAspectTrack > 0.01)
					{
						break;
					}
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE2)));
					break;
				case 6:
					if (m_dAspectTrack > 0.01)
					{
						break;
					}
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE3)));
					break;
				case 7:
					if (m_dAspectTrack > 0.01)
					{
						break;
					}
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE2)));
					break;
				case 8:
					if (m_dAspectTrack > 0.01)
					{
						break;
					}
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE3)));
					break;
				default:
					break;
			}

			return nFlag;
		}

		void DoChange(int flag, int dX, int dY)
		{
			if (m_lWidth < m_nEpsilon)
			{
				m_lWidth = m_nEpsilon;
			}
			if (m_lHeight < m_nEpsilon)
			{
				m_lHeight = m_nEpsilon;
			}
			
			switch (flag)
			{
				case 0: 
					/// 
					//SetCursor(LoadCursor(NULL, IDC_SIZEALL));
					break;
				case 1:
					m_lWidth -= 2 * dX;
					if (m_dAspectTrack > 0.01)
					{
						m_lHeight = (long)((double)m_lWidth / m_dAspectTrack);
					}
					//m_oRect.left -= dX;
					//m_oRect.right -= dX;
					//SetCursor(LoadCursor(NULL, IDC_SIZEWE));
					break;
				case 2:
					m_lHeight -= 2 * dY;
					if (m_dAspectTrack > 0.01)
					{
						m_lWidth = (long)(m_dAspectTrack * m_lHeight);
					}
					//m_oRect.top -= dX;
					//m_oRect.bottom -= dX;
					//SetCursor(LoadCursor(NULL, IDC_SIZENS));
					break;
				case 3:
					m_lWidth += 2 * dX;
					if (m_dAspectTrack > 0.01)
					{
						m_lHeight = (long)((double)m_lWidth / m_dAspectTrack);
					}
					//m_oRect.left += dX;
					//m_oRect.right += dX;
					//SetCursor(LoadCursor(NULL, IDC_SIZEWE));
					break;
				case 4:
					m_lHeight += 2 * dY;
					if (m_dAspectTrack > 0.01)
					{
						m_lWidth = (long)(m_dAspectTrack * m_lHeight);
					}
					//m_oRect.top += dX;
					//m_oRect.bottom += dX;
					//SetCursor(LoadCursor(NULL, IDC_SIZENS));
					break;
				case 5:
					if (m_dAspectTrack > 0.01)
					{
						break;
					}
					m_lWidth -= 2 * dX;
					m_lHeight -= 2 * dY;
					//m_oRect.left -= dX;
					//m_oRect.right -= dX;
					//m_oRect.top -= dX;
					//m_oRect.bottom -= dX;
					//SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
					break;
				case 6:
					if (m_dAspectTrack > 0.01)
					{
						break;
					}
					m_lWidth += 2 * dX;
					m_lHeight -= 2 * dY;
					//m_oRect.left += dX;
					//m_oRect.right += dX;
					//m_oRect.top -= dX;
					//m_oRect.bottom -= dX;
					//SetCursor(LoadCursor(NULL, IDC_SIZENESW));
					break;
				case 7:
					if (m_dAspectTrack > 0.01)
					{
						break;
					}
					m_lWidth += 2 * dX;
					m_lHeight += 2 * dY;
					//m_oRect.left += dX;
					//m_oRect.right += dX;
					//m_oRect.top += dX;
					//m_oRect.bottom += dX;
					//SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
					break;
				case 8:
					if (m_dAspectTrack > 0.01)
					{
						break;
					}
					m_lWidth -= 2 * dX;
					m_lHeight += 2 * dY;
					//m_oRect.left -= dX;
					//m_oRect.right -= dX;
					//m_oRect.top += dX;
					//m_oRect.bottom += dX;
					//SetCursor(LoadCursor(NULL, IDC_SIZENESW));
					break;
				default:
					break;
			}

			//m_lWidth = min(m_lWidth, m_oWndRect.right - m_oWndRect.left);
			//m_lHeight = min(m_lHeight, m_oWndRect.bottom - m_oWndRect.top);

			m_lWidth = max(m_lWidth, m_nEpsilon);
			m_lHeight = max(m_lHeight, m_nEpsilon);

			//m_oRect.left = max(m_oRect.left, m_oWndRect.left);
			//m_oRect.top = max(m_oRect.top, m_oWndRect.top);
			//m_oRect.right = min(m_oRect.right, m_oWndRect.right);
			//m_oRect.bottom = min(m_oRect.bottom, m_oWndRect.bottom);
		}

		bool PointOnSegment(int x1, int y1, int x2, int y2, int ptx, int pty, int epsilon)
		{
			/*
			int scalar1 = (x2-x1)*(ptx-x1) + (y2-y1)*(pty-y1);
			if (scalar1 < 0)
				return false;

			int scalar2 = (x1-x2)*(ptx-x2) + (y1-y2)*(pty-y2);
			if (scalar2 < 0)
				return false;
			*/
			/*
			double dLen = _hypot(x1 - x2, y1 - y2) + epsilon;
			double dLen1 = _hypot(x1 - ptx, y1 - pty);
			if (dLen1 > dLen) 
				return false;
			double dLen2 = _hypot(x2 - ptx, y2 - pty); 
			if (dLen2 > dLen)
				return false;
			*/

			// point is inside the bundle - detect distance
			double dA = (y2-y1);
			double dB = (x1-x2);
			double dC = y1*(x2-x1)-x1*(y2-y1);

			double dDist = (dA*ptx + dB*pty + dC)/_hypot(dA, dB);

			if (fabs(dDist) < epsilon)
				return true;

			return false;
		}
		void OnMouseDown(LONG nX, LONG nY, LONG nButton)	
		{
			m_oCursor.x = nX;
			m_oCursor.y = nY;

			m_bIsMove = true;
			m_nFlagOld = GetChange(nX, nY);
		}
		void OnMouseUp(LONG nX, LONG nY, LONG nButton)
		{
			m_bIsMove = false;
		}
		void OnMouseMove(LONG nX, LONG nY)	
		{
			if (!m_bIsMove)
			{
				int result = GetChange(nX, nY);
				m_bIsTrackEnabled = (result > 0) ? true : false;
				return;
			}
			int dX = nX - m_oCursor.x;
			int dY = nY - m_oCursor.y;
			
			int rez = GetChange(m_oCursor.x, m_oCursor.y);

			m_bIsTrackEnabled = (rez > 0) ? true : false;

			m_oCursor.x += dX;
			m_oCursor.y += dY;

			if (rez != m_nFlagOld)
			{
				return;
			}

			if (rez == -1)
				return;
			DoChange(rez, dX, dY);
		}

		void GetTrackRect(Gdiplus::Rect* pRect)
		{
			if (NULL == pRect)
				return;

			pRect->X = m_oWndRect.left + (INT)(m_oWndRect.right - m_oWndRect.left - m_lWidth) / 2;
			pRect->Y = m_oWndRect.top + (INT)(m_oWndRect.bottom - m_oWndRect.top - m_lHeight) / 2;
			pRect->Width = (INT)m_lWidth;
			pRect->Height = (INT)m_lHeight;
		}
	};


private:
	LONG m_lWidth;
	LONG m_lHeight;

	LONG m_lType;
	LONG m_lColor;

	Bitmap* m_pImage;
	CTrackRectangle m_oTrack;

	double m_dAspect;
};

#endif