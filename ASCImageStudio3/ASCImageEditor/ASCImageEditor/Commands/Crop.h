#ifndef __CROP_COMM_
#define __CROP_COMM_

#include "..//CommandManager.h"
#include "..//ViewManager.h"
/************************************************************************/
/* Crop COMMAND   -----   ID = 102                                      */
/************************************************************************/

#include "AVSUtils.h"
#include <math.h>

using namespace Gdiplus;

#include "Commands/CommandsUtils.h"
class CCropCommand : public CCommand, public IChangesSubscriber, public ICommandButtonEvent
{
public:
	CCropCommand(IAppManager* app_manager) : 
		CCommand(app_manager), 
		m_oTrack(), 
		m_oApplyButton(0, IDB_APPLY_NORM_PNG, IDB_APPLY_OVER_PNG, IDB_APPLY_DOWN_PNG, IDB_SPLIT_PNG, 1, this),
		m_oCancelButton(1, IDB_CANCEL_NORM_PNG, IDB_CANCEL_OVER_PNG, IDB_CANCEL_DOWN_PNG, 0, -1, this)
	{
		DEV_LOG("Crop start constructor");
		
		// геометрическое место кнопок
		m_oApplyButton.SetBounds(20, 20, 50, 50);
		m_oCancelButton.SetBounds(90, 20, 50, 50);

		GetAppManager()->GetView()->FitImageToWindow();
		
		StartCommand();

		AddParameter("Width", "Width Value", (int)m_lImageWidth, 10, 2000);
		AddParameter("Height", "Height Value", (int)m_lImageHeight, 10, 2000);
		AddParameter("Angle", "Angle Value", (int)m_fAngle, 0, 360);
		AddParameter("OffsetX", "OffsetX Value", (float)m_dOffsetX, 0.0f, 1000.0f);
		AddParameter("OffsetY", "OffsetY Value", (float)m_dOffsetY, 0.0f, 1000.0f);
		AddParameter("AlwaysFullTrack", "AlwaysFullTrack Value", (int)m_bIsAlwaysFullTrack, 0, 1);
		AddParameter("AspectTrack", "AspectTrack Value", (float)m_oTrack.m_dAspectTrack, -1.0f, 10.0f);
		AddParameter("OnlyRotate", "OnlyRotate Value", (int)m_bIsOnlyRotate, 0, 1);

		SetInvalidateFlag(INVALID_GRAPHICS);

		GetAppManager()->GetView()->EnableDraw(false);
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->AddSubscriber(this);

		DEV_LOG("Crop end constructor");
		DEV_LOG("---Crop Command Begin");
	}
	virtual ~CCropCommand() 
	{
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->RemoveSubscriber(this);
		GetAppManager()->GetView()->EnableDraw(true);
		RELEASEOBJECT(m_pImage);
	}
public:

	bool IsButtonEnabled(LONG lFlag, LONG lX, LONG lY, LONG lButton)
	{
		m_bIsComplete = FALSE;
		
		bool b1 = m_oApplyButton.DoChange(lFlag, lX, lY, lButton);
		bool b2 = m_oCancelButton.DoChange(lFlag, lX, lY, lButton);

		if (!b1 && !b2)
			return false;

		return true;
	}

	virtual void OnMouseDown(LONG nX, LONG nY, LONG nButton)	
	{
		bool bIsButton = IsButtonEnabled(CCommandButton::btMouseDown, nX, nY, nButton);
		if (bIsButton)
		{
			SetInvalidateFlag(INVALID_GRAPHICS);
			return;
		}
		
		m_oCursorOld.x = nX;
		m_oCursorOld.y = nY;

		m_oTrack.OnMouseDown(nX, nY, nButton);

		m_bIsMoved = false;
		m_bIsRotate = false;
		m_bIsTracking = false;

		if (0 == m_oTrack.m_nFlagOld)
		{
			m_bIsMoved = true;
		}
		else if (-1 == m_oTrack.m_nFlagOld)
		{
			m_bIsRotate = true;
		}
		else
		{
			m_bIsTracking = true;
		}
	}
	virtual void OnMouseUp(LONG nX, LONG nY, LONG nButton)
	{
		bool bIsButton = IsButtonEnabled(CCommandButton::btMouseUp, nX, nY, nButton);
		if (bIsButton)
		{
			if (m_bIsRotate)
			{
				m_fAngleOld = m_fAngle;
			}
			if (m_bIsMoved)
			{
				m_dOffsetOldX = m_dOffsetX;
				m_dOffsetOldY = m_dOffsetY;
			}
			
			m_bIsRotate = false;
			m_bIsMoved = false;
			m_bIsTracking = false;

			m_oTrack.OnMouseUp(nX, nY, nButton);			
			SetInvalidateFlag(INVALID_GRAPHICS);
			return;
		}

		if (m_bIsRotate)
		{
			m_fAngleOld = m_fAngle;
		}
		if (m_bIsMoved)
		{
			m_dOffsetOldX = m_dOffsetX;
			m_dOffsetOldY = m_dOffsetY;
		}
		
		m_bIsRotate = false;
		m_bIsMoved = false;
		m_bIsTracking = false;

		m_oTrack.OnMouseUp(nX, nY, nButton);
		m_oTrack.GetChange(nX, nY);
	}
	virtual void OnMouseMove(LONG nX, LONG nY, LONG nButton)	
	{
		bool bIsButton = IsButtonEnabled(CCommandButton::btMouseMove, nX, nY, 0);
		if (bIsButton)
		{
			SetInvalidateFlag(INVALID_GRAPHICS);
			return;
		}
		
		if (!m_oTrack.m_bIsMove)
		{
			m_oTrack.OnMouseMove(nX, nY, m_bIsTracking);
			SetInvalidateFlag(INVALID_GRAPHICS);
			return;
		}
		
		if (m_bIsRotate)
		{
			SetCursor(LoadCursor(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDC_CURSOR_ROTATE)));

			double dCenterX = (double)(m_oTrack.m_oWndRect.right + m_oTrack.m_oWndRect.left) / 2.0;
			double dCenterY = (double)(m_oTrack.m_oWndRect.bottom + m_oTrack.m_oWndRect.top) / 2.0;
			
			double dAngleStart = atan((double)(m_oCursorOld.y - dCenterY) / (double)(m_oCursorOld.x - dCenterX));
			double dAngleCur = atan((double)(nY - dCenterY) / (double)(nX - dCenterX));

			if (m_oCursorOld.x < dCenterX)
			{
				dAngleStart += M_PI;
			}
			if (nX < dCenterX)
			{
				dAngleCur += M_PI;
			}

			double dAngle = m_fAngle;
				
			m_fAngle = (float)(180.0 * (dAngleCur - dAngleStart) / M_PI);
			m_fAngle += m_fAngleOld;

			long lOldWidth = m_oTrack.m_lWidth;
			long lOldHeight = m_oTrack.m_lHeight;

			if (!IsValidChanges())
			{
				m_fAngle = dAngle;
				return;
			}

			FireChangeParameter(2, (int)m_fAngle);			
		}
		else if (m_bIsMoved)
		{
			SetCursor(LoadCursor(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDC_CURSOR_CLOSEHAND)));

			double dOffsetX = m_dOffsetX;
			double dOffsetY = m_dOffsetY;
			
			m_dOffsetX = nX - m_oCursorOld.x + m_dOffsetOldX;
			m_dOffsetY = nY - m_oCursorOld.y + m_dOffsetOldY;

			long lOldWidth = m_oTrack.m_lWidth;
			long lOldHeight = m_oTrack.m_lHeight;
			
			if (!IsValidChanges())
			{
				m_dOffsetX = dOffsetX;
				m_dOffsetY = dOffsetY;

				m_oTrack.m_lWidth = lOldWidth;
				m_oTrack.m_lHeight = lOldHeight;

				return;
			}
		}
		else
		{
			long oldW = m_oTrack.m_lWidth;
			long oldH = m_oTrack.m_lHeight;

			LONG lOldMouseX = m_lMouseChangedMaxX;
			LONG lOldMouseY = m_lMouseChangedMaxY;

			m_oTrack.OnMouseMove(nX, nY, m_bIsTracking);

			if (!IsValidChanges())
			{
				m_oTrack.m_lWidth = oldW;
				m_oTrack.m_lHeight = oldH;

				m_lMouseChangedMaxX = lOldMouseX;
				m_lMouseChangedMaxY = lOldMouseY;

				return;
			}

			//if (-1.0 == m_oTrack.m_dAspectTrack)
			//{
			//	// забраковали мое дизайнерское решение))
			//	if ((1 == m_oTrack.m_nFlagOld) || (3 == m_oTrack.m_nFlagOld))
			//	{
			//		if (m_oTrack.m_lHeight != oldH)
			//		{
			//			m_oTrack.m_lWidth = oldW;				
			//			m_oTrack.m_lHeight = oldH;
			//			//return;
			//		}
			//	}
			//	else if ((2 == m_oTrack.m_nFlagOld) || (4 == m_oTrack.m_nFlagOld))
			//	{
			//		if (m_oTrack.m_lWidth != oldW)
			//		{
			//			m_oTrack.m_lWidth = oldW;				
			//			m_oTrack.m_lHeight = oldH;
			//			//return;
			//		}
			//	}
			//}
		}
		SetInvalidateFlag(INVALID_GRAPHICS);
	}

	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam)
	{
		DEV_LOG("OnKeyDown");

		if (m_oTrack.m_bIsMove)
			return;

		double dOffsetX_ = m_dOffsetX;
		double dOffsetY_ = m_dOffsetY;
		double dOffsetOldX_ = m_dOffsetOldX;
		double dOffsetOldY_ = m_dOffsetOldY;

		double dDelta = 1.0;

		switch (wParam)
		{
		case VK_LEFT:
			{
				m_dOffsetX -= dDelta;
				m_dOffsetOldX = m_dOffsetX;
				break;
			}
		case VK_RIGHT:
			{
				m_dOffsetX += dDelta;
				m_dOffsetOldX = m_dOffsetX;
				break;
			}
		case VK_UP:
			{
				m_dOffsetY -= dDelta;
				m_dOffsetOldY = m_dOffsetY;
				break;
			}
		case VK_DOWN:
			{
				m_dOffsetY += dDelta;
				m_dOffsetOldY = m_dOffsetY;
				break;
			}
		default:
			{
				return;
			}
		};

		CString str = _T("");
		str.Format(_T("%d, %lf"), lParam, dDelta);
		str += _T("\n");
		#ifdef _DEBUG
			ATLTRACE(str);
		#endif

		if (!IsValidChanges())
		{
			m_dOffsetX = dOffsetX_;
			m_dOffsetY = dOffsetY_;
			m_dOffsetOldX = dOffsetOldX_;
			m_dOffsetOldY = dOffsetOldY_;
		}

		SetInvalidateFlag(INVALID_GRAPHICS);
	}
	virtual void OnKeyUp(WPARAM wParam, LPARAM lParam)
	{
		DEV_LOG("OnKeyUp");
	}
	virtual void OnMouseLeave()
	{
		if (m_bIsRotate)
		{
			m_fAngleOld = m_fAngle;
		}
		if (m_bIsMoved)
		{
			m_dOffsetOldX = m_dOffsetX;
			m_dOffsetOldY = m_dOffsetY;
		}
		
		m_bIsRotate = false;
		m_bIsMoved = false;
		m_bIsTracking = false;

		m_oTrack.m_bIsMove = false;
	}

	virtual void Undo(int nStepCount)
	{		
		/*ILayer::IMAGE_TYPE lType = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetActualImage();
		UpdateImage(lType);
		
		SetInvalidateFlag(INVALID_GRAPHICS);*/
		if (0 >= nStepCount)
			return;

		m_bIsWaitDrawing = TRUE;
	}

	virtual void Redo(int nStepCount)
	{
		/*ILayer::IMAGE_TYPE lType = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetActualImage();
		UpdateImage(lType);

		SetInvalidateFlag(INVALID_GRAPHICS);*/
		if (0 >= nStepCount)
			return;

		m_bIsWaitDrawing = TRUE;
	}

	void StartCommand()
	{
		m_oTrack.Clear();
		
		m_bIsRotate = false;
		m_bIsMoved = false;
		m_bIsTracking = false;

		m_bIsAlwaysFullTrack = true;
		m_bIsFirstFullFlag = true;

		m_fAngle = 0.0;
		m_fAngleOld = 0.0;

		m_dOffsetX = 0.0;
		m_dOffsetY = 0.0;

		m_dOffsetOldX = 0.0;
		m_dOffsetOldY = 0.0;

		m_pImage = NULL;

		m_bIsComplete = FALSE;
		m_lOperationID = -1;
	
		m_dAspect = 0;
		UpdateAspect();

		m_bIsMaxTrack = TRUE;

		const BYTE* pBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, m_oTrack.m_lWidth, 
			m_oTrack.m_lHeight);

		m_lImageWidth = m_oTrack.m_lWidth;
		m_lImageHeight = m_oTrack.m_lHeight;
		
		GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(m_oTrack.m_lWidth);
		GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(m_oTrack.m_lHeight);

		m_bIsOnlyRotate = FALSE;

		//GetAppManager()->GetView()->FitImageToWindow();

		// вот тут заглушка под округление... нам нужно чтобы было ТОЧНОЕ равенство
		// ширины окна и ширины картинки -> ширина окна ----------------------------
		LONG lLeft_ = 0;
		LONG lTop_ = 0;
		GetAppManager()->GetCoordinateTransformer()->ImagePointToScreenPoint(lLeft_, lTop_);
		LONG lWidth_ = m_lImageWidth;
		LONG lHeight_ = m_lImageHeight;
		GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(lWidth_);
		GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(lHeight_);

		m_oTrack.m_oWndRect.left = lLeft_;
		m_oTrack.m_oWndRect.top = lTop_;
		m_oTrack.m_oWndRect.right = m_oTrack.m_oWndRect.left + lWidth_;
		m_oTrack.m_oWndRect.bottom = m_oTrack.m_oWndRect.top + lHeight_;
		// -------------------------------------------------------------------------

		m_dCurWidth = (double)m_oTrack.m_lWidth;
		m_dCurHeight = (double)m_oTrack.m_lHeight;

		m_bIsCorrectTrack = TRUE;
		m_bIsWaitDrawing = FALSE;
	}
	
	virtual void OnChangeParameter(int paramInd) 
	{
		LONG lOldWidth = m_oTrack.m_lWidth;
		LONG lOldHeight = m_oTrack.m_lHeight;

		double fOldAngle = m_fAngle;

		double dOldOffsetX = m_dOffsetX;
		double dOldOffsetY = m_dOffsetY;

		BOOL bIsChangeBounds = FALSE;
		
		if (paramInd == 0 && m_params.GetCount() > 0)
		{
			//m_oTrack.m_lWidth = max(m_params[0]->m_value.lVal, m_oTrack.m_nEpsilon);
			SetNewSizeX(m_params[0]->m_value.lVal);
			// вот здесь нужно выровнять по аспекту.
			if (-1 != m_oTrack.m_dAspectTrack)
			{
				m_oTrack.m_lHeight = (LONG)((double)m_oTrack.m_lWidth / m_oTrack.m_dAspectTrack);
			}
			//GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(m_oTrack.m_lWidth);
		}
		else if (paramInd == 1 && m_params.GetCount() > 1)
		{
			//m_oTrack.m_lHeight = max(m_params[1]->m_value.lVal, m_oTrack.m_nEpsilon);
			SetNewSizeY(m_params[1]->m_value.lVal);
			// вот здесь нужно выровнять по аспекту.
			if (-1 != m_oTrack.m_dAspectTrack)
			{
				m_oTrack.m_lWidth = (LONG)(m_oTrack.m_lHeight * m_oTrack.m_dAspectTrack);
			}
			//GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(m_oTrack.m_lHeight);
		}
		else if (paramInd == 2 && m_params.GetCount() > 2)
		{
			m_bIsRotate = true;
			m_fAngle = m_params[2]->m_value.lVal;
			m_fAngleOld = m_fAngle;
			bIsChangeBounds = TRUE;
		}
		else if (paramInd == 3 && m_params.GetCount() > 3)
		{
			m_dOffsetX = m_params[3]->m_value.lVal;
		}
		else if (paramInd == 4 && m_params.GetCount() > 4)
		{
			m_dOffsetY = m_params[4]->m_value.lVal;
		}
		else if (paramInd == 5 && m_params.GetCount() > 5)
		{
			m_bIsAlwaysFullTrack = (m_params[5]->m_value.lVal == 1);
		}
		else if (paramInd == 6 && m_params.GetCount() > 6)
		{
			m_dAspect = m_params[6]->m_value.dblVal;
			if (0.0 == m_dAspect)
			{
				m_dAspect = (double)m_oTrack.m_lWidth / m_oTrack.m_lHeight;
			}
			UpdateAspect();
		}
		else if (paramInd == 7 && m_params.GetCount() > 7)
		{
			m_bIsOnlyRotate = (m_params[7]->m_value.llVal == 1);
		}
		else
		{
			return;
		}
		
		if (!IsValidChanges())
		{
			m_oTrack.m_lWidth = lOldWidth;
			m_oTrack.m_lHeight = lOldHeight;

			m_fAngle = fOldAngle;
			m_fAngleOld = m_fAngle;

			m_dOffsetX = dOldOffsetX;
			m_dOffsetY = dOldOffsetY;

			m_bIsRotate = false;
			return;
		}

		m_bIsRotate = false;
		SetInvalidateFlag(INVALID_GRAPHICS);
	}

	virtual void ChangesHandler(int changingObject, bool repaint)
	{
		if (changingObject == ILayer::FAST_COPY || changingObject == ILayer::SLOW_COPY)
		{
			UpdateImage((ILayer::IMAGE_TYPE)changingObject);

			m_bIsWaitDrawing = FALSE;
			SetInvalidateFlag(INVALID_GRAPHICS);
		}
	}

	virtual void SendEvent(LONG lButtonID, LONG lType)
	{
		switch (lButtonID)
		{
		case 0:
			{
				//NeedRestartEvent(VARIANT_TRUE);
				GetAppManager()->SendMsg(WM_IEM_STOP_COMMAND_YES, 0);
				break;
			}
		case 1:
			{
				//NeedRestartEvent(VARIANT_FALSE);
				GetAppManager()->SendMsg(WM_IEM_STOP_COMMAND_NO, 0);
				break;
			}
		default:
			break;
		};
	}
	
	virtual void DrawOnGraphics(Gdiplus::Graphics* pGr, const CRect& rect)
	{
		if (m_bIsWaitDrawing)
			return;
		
		m_bIsMaxTrack = FALSE;
		
		Gdiplus::SolidBrush oBrushBcks(GetAppManager()->GetView()->GetBackgroundColor());
		pGr->FillRectangle(&oBrushBcks,GetAppManager()->GetView()->GetImageRect());

		if (NULL == m_pImage)
		{
			const BYTE* pBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW_COPY, m_lImageWidth, m_lImageHeight);

			m_pImage = new Bitmap(m_lImageWidth, m_lImageHeight, 4*m_lImageWidth, PixelFormat32bppARGB, const_cast<BYTE*>(pBuffer));

			UpdateAspect();
		}
		
		if (NULL != pGr && NULL != m_pImage)
		{
			// вот здесь вместо clear - нужно рисовать подложку
			//ImageRendering::CChessBrush oAlphaBrush;
			//oAlphaBrush.OnPaint(pGr, Rect(rect.left, rect.top, rect.Width(), rect.Height()));

			pGr->Clear(Color::Black);
			
			long lWidthImage = m_pImage->GetWidth();
			long lHeightImage = m_pImage->GetHeight();

			CheckOnSizeWindow(rect);
			
			PointF* pPoints = GetImageRect(rect);

			pGr->DrawImage(m_pImage, pPoints, 3);
			delete[] pPoints;

			// выделяем трек...
			Rect oTrackRect;
			GetTrackRect(&oTrackRect);

			Region oReg;
			pGr->GetClip(&oReg);
			
			pGr->ExcludeClip(oTrackRect);
			Color oColor;
			oColor.SetValue(0xA0000000);
			
			SolidBrush oBrush(oColor);
			pGr->FillRegion(&oBrush, &oReg);
			pGr->ResetClip();

			m_oTrack.DrawOnGraphics(pGr);
		}

		LONG lButtonWidth = 50;
		LONG lButtonHeight = 47;

		RectF oWndRect = GetAppManager()->GetView()->GetWindowRect();

		LONG lApplyLeft = (LONG)(oWndRect.X + oWndRect.Width / 2 - lButtonWidth);
		LONG lCancelLeft = (LONG)(oWndRect.X + oWndRect.Width / 2);
		
		LONG lTop = (LONG)(oWndRect.GetBottom() - lButtonHeight - 10);

		m_oApplyButton.SetBounds(lApplyLeft, lTop, lButtonWidth, lButtonHeight);
		m_oCancelButton.SetBounds(lCancelLeft, lTop, lButtonWidth, lButtonHeight);

		m_oApplyButton.DrawOnGraphics(pGr);
		m_oCancelButton.DrawOnGraphics(pGr);
	}

	virtual bool DrawOnLayer()
	{	
		if (NULL == m_pImage)
		{
			const BYTE* pBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(
				ILayer::SLOW, m_lImageWidth, m_lImageHeight);

			m_pImage = new Bitmap(m_lImageWidth, m_lImageHeight, 4 * m_lImageWidth, 
				PixelFormat32bppARGB, const_cast<BYTE*>(pBuffer));
		}
		if (NULL == m_pImage)
			return false;

		// вот проверка на максимум...
		if (m_bIsMaxTrack)
		{
			m_oTrack.m_lWidth = (m_oTrack.m_oWndRect.right - m_oTrack.m_oWndRect.left);
			m_oTrack.m_lHeight = (m_oTrack.m_oWndRect.bottom - m_oTrack.m_oWndRect.top);

			// а потом GetImageRect - сама  все разрулит
			m_dAspect = -1.0;
			UpdateAspect();
		}

		double dAngle = M_PI *  m_fAngle / 180.0;
		double dSin = abs(sin(dAngle));
		double dCos = abs(cos(dAngle));
		
		CRect oRect(m_oTrack.m_oWndRect.left, m_oTrack.m_oWndRect.top,
			m_oTrack.m_oWndRect.right, m_oTrack.m_oWndRect.bottom);

		LONG lNewWidth = m_lImageWidth;
		LONG lNewHeight = m_lImageHeight;

		double dXc = 0.0;
		double dYc = 0.0;

		if (CheckOnlyRotate(lNewWidth, lNewHeight))
		{
			dXc = (lNewWidth - 1) / 2.0;
			dYc = (lNewHeight - 1) / 2.0;
		}
		else
		{
			PointF* pPoints = GetImageRect(oRect);
			delete[] pPoints;

			double dX = (double)m_lImageWidth / m_dCurWidth;
			double dY = (double)m_lImageHeight / m_dCurHeight;

			double dTW = (double)m_oTrack.m_lWidth;
			double dTH = (double)m_oTrack.m_lHeight;

			lNewWidth = (long)(_hypot(dCos * m_lImageWidth / m_dCurWidth, dSin * m_lImageHeight / m_dCurHeight) * dTW);
			lNewHeight = (long)(_hypot(dSin * m_lImageWidth / m_dCurWidth, dCos * m_lImageHeight / m_dCurHeight) * dTH);

			double dOffsetX = m_dOffsetX * lNewWidth / dTW;
			double dOffsetY = m_dOffsetY * lNewHeight / dTH;
			
			dXc = (double)(lNewWidth - 1) / 2.0 + dOffsetX;
			dYc = (double)(lNewHeight - 1) / 2.0 + dOffsetY;
		}

		BYTE* pBuffer = new BYTE[4 * lNewWidth * lNewHeight];
		//memset(pBuffer, 0xFF, 4 * lNewWidth * lNewHeight);

		const BYTE* pSrcBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(
			ILayer::SLOW, m_lImageWidth, m_lImageHeight);

		BGRA_Rotate_(const_cast<BYTE*>(pSrcBuffer), m_lImageWidth, m_lImageHeight, pBuffer, lNewWidth, lNewHeight, 
						dXc, dYc, m_fAngle, 1, 1, TRUE);
			

		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StartHistoryBlock();
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangeAllLayer(pBuffer, lNewWidth, lNewHeight);
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StopHistoryBlock("Crop");

		GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangeImage(ILayer::SLOW,this);

		GetAppManager()->GetView()->FitImageToWindow();

		return true;
	}

private:
	
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
			
			Clear();			
		}

		void Clear()
		{
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

			HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
			
			if ((nX < (left - m_nEpsilon)) || (nX > (right + m_nEpsilon)) || 
						(nY < (top - m_nEpsilon)) || (nY > (bottom + m_nEpsilon)))
			{
				SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_ROTATE)));
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
			else 
			{
				int n = min(right - left, bottom - top);
				if ((n < 3 * m_nEpsilon) && ((nX >= left) && (nX <= right) && (nY >= top) && (nY <= bottom)))
				{
					nFlag = 0;
				}
				else
				{
					if (b1 && b2 && m_bIsTrackDoubleEnabled) nFlag = 5;
					else if (b2 && b3 && m_bIsTrackDoubleEnabled) nFlag = 6;
					else if (b3 && b4 && m_bIsTrackDoubleEnabled) nFlag = 7;
					else if (b4 && b1 && m_bIsTrackDoubleEnabled) nFlag = 8;
					
					else if (b1 && (nY > top) && (nY < bottom)) nFlag = 1;
					else if (b2 && (nX > left) && (nX < right)) nFlag = 2;
					else if (b3 && (nY > top) && (nY < bottom)) nFlag = 3; 
					else if (b4 && (nX > left) && (nX < right)) nFlag = 4;
				}
			}

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
				case 0: 
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_HAND)));
					break;
				default:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_ROTATE)));
					break;
			}

			return nFlag;
		}

		void FlagOldToCursor()
		{
			HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
			switch (m_nFlagOld)
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
				case 0: 
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_HAND)));
					break;
				default:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_ROTATE)));
					break;
			}
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

			m_lWidth = min(m_lWidth, m_oWndRect.right - m_oWndRect.left);
			m_lHeight = min(m_lHeight, m_oWndRect.bottom - m_oWndRect.top);

			m_lWidth = max(m_lWidth, m_nEpsilon);
			m_lHeight = max(m_lHeight, m_nEpsilon);

			//m_oRect.left = max(m_oRect.left, m_oWndRect.left);
			//m_oRect.top = max(m_oRect.top, m_oWndRect.top);
			//m_oRect.right = min(m_oRect.right, m_oWndRect.right);
			//m_oRect.bottom = min(m_oRect.bottom, m_oWndRect.bottom);
		}

		void DoChange2(int flag, int nXc, int nYc, int nX, int nY)
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
					break;
				case 1:
					m_lWidth = 2 * (nXc - nX);
					if (m_dAspectTrack > 0.01)
					{
						m_lHeight = (long)((double)m_lWidth / m_dAspectTrack);
					}
					break;
				case 2:
					m_lHeight = 2 * (nYc - nY);
					if (m_dAspectTrack > 0.01)
					{
						m_lWidth = (long)(m_dAspectTrack * m_lHeight);
					}
					break;
				case 3:
					m_lWidth = 2 * (nX - nXc);
					if (m_dAspectTrack > 0.01)
					{
						m_lHeight = (long)((double)m_lWidth / m_dAspectTrack);
					}
					break;
				case 4:
					m_lHeight = 2 * (nY - nYc);
					if (m_dAspectTrack > 0.01)
					{
						m_lWidth = (long)(m_dAspectTrack * m_lHeight);
					}
					break;
				case 5:
					if (m_dAspectTrack > 0.01)
					{
						break;
					}
					m_lWidth = 2 * (nXc - nX);
					m_lHeight = 2 * (nYc - nY);
					break;
				case 6:
					if (m_dAspectTrack > 0.01)
					{
						break;
					}
					m_lWidth = 2 * (nX - nXc);
					m_lHeight = 2 * (nYc - nY);
					break;
				case 7:
					if (m_dAspectTrack > 0.01)
					{
						break;
					}
					m_lWidth = 2 * (nX - nXc);
					m_lHeight = 2 * (nY - nYc);
					break;
				case 8:
					if (m_dAspectTrack > 0.01)
					{
						break;
					}
					m_lWidth = 2 * (nXc - nX);
					m_lHeight = 2 * (nY - nYc);
					break;
				default:
					break;
			}

			CheckMinBounds();
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
		void OnMouseMove(LONG nX, LONG nY, bool bIsTracking = false)	
		{
			if (!m_bIsMove)
			{
				int result = GetChange(nX, nY);
				m_bIsTrackEnabled = (result > 0) ? true : false;
				return;
			}
			int dX = nX - m_oCursor.x;
			int dY = nY - m_oCursor.y;
			
			int rez = m_nFlagOld;
			if (!bIsTracking)
			{
				rez = GetChange(m_oCursor.x, m_oCursor.y);
			}
			else
			{
				FlagOldToCursor();
			}

			m_bIsTrackEnabled = (rez > 0) ? true : false;

			if (rez != m_nFlagOld)
			{
				return;
			}

			if (rez == -1)
				return;

			m_oCursor.x += dX;
			m_oCursor.y += dY;

			//DoChange(rez, dX, dY);
			LONG nXc = (m_oWndRect.left + m_oWndRect.right) / 2;
			LONG nYc = (m_oWndRect.top + m_oWndRect.bottom) / 2;
			DoChange2(rez, nXc, nYc, nX, nY);
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

		void CheckMinBounds()
		{
			LONG lMinTrack = GetMinTrack();
			if (m_lWidth < lMinTrack || m_lHeight < lMinTrack)
			{
				SetMinBounds();
			}
		}

		void SetMinBounds()
		{
			long lMinTrack = 2 * m_nEpsilon;
			if (-1.0 == m_dAspectTrack)
			{
				if (m_lWidth < lMinTrack)
				{
					m_lWidth = lMinTrack;
				}
				if (m_lHeight < lMinTrack)
				{
					m_lHeight = lMinTrack;
				}
				return;
			}
			if (1.0 <= m_dAspectTrack)
			{
				m_lHeight = lMinTrack;
				m_lWidth = (LONG)(m_dAspectTrack * m_lHeight);
				return;
			}
			
			m_lWidth = lMinTrack;
			m_lHeight = (LONG)((double)m_lWidth / m_dAspectTrack);
		}
		LONG GetMinTrack()
		{
			return min(2 * m_nEpsilon, min(m_oWndRect.right - m_oWndRect.left, m_oWndRect.bottom - m_oWndRect.top));
		}
	};

public:

	POINT m_oCursorOld;
	CTrackRectangle m_oTrack;

	double m_fAngle;		// в градусах... (0...360)
	double m_fAngleOld;
	
	double m_dOffsetX;
	double m_dOffsetY;
	double m_dOffsetOldX;
	double m_dOffsetOldY;

	bool m_bIsRotate;
	bool m_bIsMoved;
	bool m_bIsTracking;

	bool m_bIsAlwaysFullTrack;
	bool m_bIsFirstFullFlag;

	Bitmap* m_pImage;
	ILayer::IMAGE_TYPE m_lImageType;

	double m_dAspect;

	CCommandButton m_oApplyButton;
	CCommandButton m_oCancelButton;

	BOOL m_bIsComplete;
	LONG m_lOperationID;

	// вот этот флаг - следит - была ли нажата кнопка ротэйт
	// короче если на DrawOnLayer - этот флаг выставлен - 
	// то мы раздвигаем трек на возможный максимум
	BOOL m_bIsMaxTrack;

	// размеры картинки в пикселах экрана (с учетом поворота)
	double m_dCurWidth;
	double m_dCurHeight;

	// размеры картинки реальные
	LONG m_lImageWidth;
	LONG m_lImageHeight;

	BOOL m_bIsCorrectTrack;

	// это для выставления максимального трэка
	long m_lMouseChangedMaxX;
	long m_lMouseChangedMaxY;

	BOOL m_bIsOnlyRotate;
	BOOL m_bIsWaitDrawing;

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

	PointF* GetImageRect(const CRect& oWndRect)
	{
		// устанавливаем рект окна для трека
		m_oTrack.m_oWndRect.left = oWndRect.left;
		m_oTrack.m_oWndRect.top = oWndRect.top;
		m_oTrack.m_oWndRect.right = oWndRect.right;
		m_oTrack.m_oWndRect.bottom = oWndRect.bottom;

		// вот тут заглушка под округление... нам нужно чтобы было ТОЧНОЕ равенство
		// ширины окна и ширины картинки -> ширина окна ----------------------------
		LONG lLeft_ = 0;
		LONG lTop_ = 0;
		GetAppManager()->GetCoordinateTransformer()->ImagePointToScreenPoint(lLeft_, lTop_);
		LONG lWidth_ = m_lImageWidth;
		LONG lHeight_ = m_lImageHeight;
		GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(lWidth_);
		GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(lHeight_);

		m_oTrack.m_oWndRect.left = lLeft_;
		m_oTrack.m_oWndRect.top = lTop_;
		m_oTrack.m_oWndRect.right = m_oTrack.m_oWndRect.left + lWidth_;
		m_oTrack.m_oWndRect.bottom = m_oTrack.m_oWndRect.top + lHeight_;
		// -------------------------------------------------------------------------

		if (m_bIsFirstFullFlag)
		{
			m_bIsFirstFullFlag = false;

			if (!m_bIsOnlyRotate)
			{
				m_lMouseChangedMaxX = m_oTrack.m_oWndRect.right - m_oTrack.m_oWndRect.left;
				m_lMouseChangedMaxY = m_oTrack.m_oWndRect.bottom - m_oTrack.m_oWndRect.top;
			}
		}

		CRect oImageRect(m_oTrack.m_oWndRect.left, m_oTrack.m_oWndRect.top, m_oTrack.m_oWndRect.right, m_oTrack.m_oWndRect.bottom);
		
		// теперь крутанем вокруг центра окна (т.к. у нас трек - в центре)
		PointF* pPoints = RotateRect(&oImageRect, (int)(oWndRect.right + oWndRect.left) / 2, (int)(oWndRect.bottom + oWndRect.top) / 2);
		
		// теперь сдвинем его...
		OffsetPoints(pPoints, 4);

		// а теперь проследим - может трэк - вылазит за картинку???
		NormalizeTrackRect(pPoints);

		m_dCurWidth = (double)max(1, (long)_hypot((double)(pPoints[0].X - pPoints[1].X), (double)(pPoints[0].Y - pPoints[1].Y)));
		m_dCurHeight = (double)max(1, (long)_hypot((double)(pPoints[0].X - pPoints[2].X), (double)(pPoints[0].Y - pPoints[2].Y)));

		return pPoints;
	}

	void GetTrackRect(Gdiplus::Rect* pRect)
	{
		return m_oTrack.GetTrackRect(pRect);
	}

	PointF* RotateRect(CRect* pRect, int nCx, int nCy)
	{
		pRect->OffsetRect(-nCx, -nCy);
		double dAngle = ((double)(M_PI * m_fAngle) / 180.0);
	
		PointF* pPoints = new PointF[4];

		pPoints[0].X = (REAL)(pRect->left * cos(dAngle) - pRect->top * sin(dAngle));
		pPoints[0].Y = (REAL)(pRect->left * sin(dAngle) + pRect->top * cos(dAngle));

		pPoints[1].X = (REAL)(pRect->right * cos(dAngle) - pRect->top * sin(dAngle));
		pPoints[1].Y = (REAL)(pRect->right * sin(dAngle) + pRect->top * cos(dAngle));

		pPoints[2].X = (REAL)(pRect->left * cos(dAngle) - pRect->bottom * sin(dAngle));
		pPoints[2].Y = (REAL)(pRect->left * sin(dAngle) + pRect->bottom * cos(dAngle));

		pPoints[3].X = (REAL)(pRect->right * cos(dAngle) - pRect->bottom * sin(dAngle));
		pPoints[3].Y = (REAL)(pRect->right * sin(dAngle) + pRect->bottom * cos(dAngle));

		pPoints[0].X += nCx; pPoints[0].Y += nCy;
		pPoints[1].X += nCx; pPoints[1].Y += nCy;
		pPoints[2].X += nCx; pPoints[2].Y += nCy;
		pPoints[3].X += nCx; pPoints[3].Y += nCy;

		pRect->OffsetRect(nCx, nCy);

		ShowAllImage(pPoints, pRect);
		
		// вот тут раздвигаем по максимально отредактированному
		// если он не влезет - то уменьшится потом
		if (m_oTrack.m_nFlagOld > 0)
		{
			m_lMouseChangedMaxX = m_oTrack.m_lWidth;
			m_lMouseChangedMaxY = m_oTrack.m_lHeight;
		}

		if (m_bIsAlwaysFullTrack)
		{
			m_oTrack.m_lWidth = m_lMouseChangedMaxX;
			m_oTrack.m_lHeight = m_lMouseChangedMaxY;
		}
		return pPoints;
	}
	
	void ShowAllImage(PointF* pPoints, CRect* pRect)
	{
		// делаем так, чтобы видна была ВСЯ картинка		
		double dMax = 0;
		double dDist = 0;

		float cX = ((float)(m_oTrack.m_oWndRect.right + m_oTrack.m_oWndRect.left)) / 2;
		float cY = ((float)(m_oTrack.m_oWndRect.bottom + m_oTrack.m_oWndRect.top)) / 2;

		double dDiag = _hypot((double)(cX - pPoints[0].X), (double)(cY - pPoints[0].Y));
		
		dDist = GetDistanseToBorder((float)pRect->left, (float)pRect->top, (float)pRect->right, (float)pRect->top, cX, cY, pPoints[0].X, pPoints[0].Y);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder((float)pRect->left, (float)pRect->top, (float)pRect->left, (float)pRect->bottom, cX, cY, pPoints[0].X, pPoints[0].Y);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder((float)pRect->left, (float)pRect->bottom, (float)pRect->right, (float)pRect->bottom, cX, cY, pPoints[0].X, pPoints[0].Y);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder((float)pRect->right, (float)pRect->top, (float)pRect->right, (float)pRect->bottom, cX, cY, pPoints[0].X, pPoints[0].Y);
		dMax = max(dMax, dDist);

		dDist = GetDistanseToBorder((float)pRect->left, (float)pRect->top, (float)pRect->right, (float)pRect->top, cX, cY, pPoints[1].X, pPoints[1].Y);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder((float)pRect->left, (float)pRect->top, (float)pRect->left, (float)pRect->bottom, cX, cY, pPoints[1].X, pPoints[1].Y);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder((float)pRect->left, (float)pRect->bottom, (float)pRect->right, (float)pRect->bottom, cX, cY, pPoints[1].X, pPoints[1].Y);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder((float)pRect->right, (float)pRect->top, (float)pRect->right, (float)pRect->bottom, cX, cY, pPoints[1].X, pPoints[1].Y);
		dMax = max(dMax, dDist);

		double dKoef = dMax / dDiag;

		pPoints[0].X = (float)max(pRect->left, min(pRect->right, pPoints[0].X + dKoef * (cX - pPoints[0].X)));
		pPoints[0].Y = (float)max(pRect->top, min(pRect->bottom, pPoints[0].Y + dKoef * (cY - pPoints[0].Y)));

		pPoints[1].X = (float)max(pRect->left, min(pRect->right, pPoints[1].X + dKoef * (cX - pPoints[1].X)));
		pPoints[1].Y = (float)max(pRect->top, min(pRect->bottom, pPoints[1].Y + dKoef * (cY - pPoints[1].Y)));
		
		pPoints[2].X = 2 * cX - pPoints[1].X;
		pPoints[2].Y = 2 * cY - pPoints[1].Y;

		pPoints[3].X = 2 * cX - pPoints[0].X;
		pPoints[3].Y = 2 * cY - pPoints[0].Y;
	}

	void NormalizeTrackRect(PointF* pPoints)
	{
		double dMax = 0;

		float cX = (float)(m_oTrack.m_oWndRect.right + m_oTrack.m_oWndRect.left) / 2;
		float cY = (float)(m_oTrack.m_oWndRect.bottom + m_oTrack.m_oWndRect.top) / 2;
		
		double dDist = 0.0;

		// вот здесь нужно выровнять по аспекту.
		if (-1 != m_oTrack.m_dAspectTrack)
		{
			LONG lAspW = (LONG)(m_oTrack.m_dAspectTrack * m_oTrack.m_lHeight);
			if (m_oTrack.m_lWidth > lAspW)
			{
				m_oTrack.m_lWidth = lAspW;
			}
			else if (m_oTrack.m_lWidth < lAspW)
			{
				m_oTrack.m_lHeight = (LONG)((double)m_oTrack.m_lWidth / m_oTrack.m_dAspectTrack);
			}
		}
		
		float eX = cX - (m_oTrack.m_lWidth) / 2;
		float eY = cY - (m_oTrack.m_lHeight) / 2;
		dDist = GetDistanseToBorder(pPoints[0].X, pPoints[0].Y, pPoints[1].X, pPoints[1].Y, cX, cY, eX, eY);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder(pPoints[1].X, pPoints[1].Y, pPoints[3].X, pPoints[3].Y, cX, cY, eX, eY);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder(pPoints[3].X, pPoints[3].Y, pPoints[2].X, pPoints[2].Y, cX, cY, eX, eY);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder(pPoints[2].X, pPoints[2].Y, pPoints[0].X, pPoints[0].Y, cX, cY, eX, eY);
		dMax = max(dMax, dDist);
		
		eX = cX + ((float)m_oTrack.m_lWidth) / 2;
		eY = cY - ((float)m_oTrack.m_lHeight) / 2;
		dDist = GetDistanseToBorder(pPoints[0].X, pPoints[0].Y, pPoints[1].X, pPoints[1].Y, cX, cY, eX, eY);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder(pPoints[1].X, pPoints[1].Y, pPoints[3].X, pPoints[3].Y, cX, cY, eX, eY);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder(pPoints[3].X, pPoints[3].Y, pPoints[2].X, pPoints[2].Y, cX, cY, eX, eY);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder(pPoints[2].X, pPoints[2].Y, pPoints[0].X, pPoints[0].Y, cX, cY, eX, eY);
		dMax = max(dMax, dDist);

		eX = cX + ((float)m_oTrack.m_lWidth) / 2;
		eY = cY + ((float)m_oTrack.m_lHeight) / 2;
		dDist = GetDistanseToBorder(pPoints[0].X, pPoints[0].Y, pPoints[1].X, pPoints[1].Y, cX, cY, eX, eY);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder(pPoints[1].X, pPoints[1].Y, pPoints[3].X, pPoints[3].Y, cX, cY, eX, eY);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder(pPoints[3].X, pPoints[3].Y, pPoints[2].X, pPoints[2].Y, cX, cY, eX, eY);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder(pPoints[2].X, pPoints[2].Y, pPoints[0].X, pPoints[0].Y, cX, cY, eX, eY);
		dMax = max(dMax, dDist);

		eX = cX - ((float)m_oTrack.m_lWidth) / 2;
		eY = cY + ((float)m_oTrack.m_lHeight) / 2;
		dDist = GetDistanseToBorder(pPoints[0].X, pPoints[0].Y, pPoints[1].X, pPoints[1].Y, cX, cY, eX, eY);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder(pPoints[1].X, pPoints[1].Y, pPoints[3].X, pPoints[3].Y, cX, cY, eX, eY);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder(pPoints[3].X, pPoints[3].Y, pPoints[2].X, pPoints[2].Y, cX, cY, eX, eY);
		dMax = max(dMax, dDist);
		dDist = GetDistanseToBorder(pPoints[2].X, pPoints[2].Y, pPoints[0].X, pPoints[0].Y, cX, cY, eX, eY);
		dMax = max(dMax, dDist);
		
		if (dMax == 0)
		{
			return;
		}
		
		/*if (m_oTrack.m_nFlagOld > 0)
		{
			m_bIsCorrectTrack = FALSE;
			return;
		}*/

		// заглушка под нулевой угол - и когда аспект выключен и когда двигаем мышкой за края
		// тогда - нормализуем только одну сторону
		if ((-1 == m_oTrack.m_dAspectTrack) && (0 == m_fAngle) && (0 < m_oTrack.m_nFlagOld))
		{
			return;
		}

		double dCoef = 1 - 2* dMax / (_hypot((double)m_oTrack.m_lWidth, (double)m_oTrack.m_lHeight));
		m_oTrack.m_lWidth = (long)(dCoef * m_oTrack.m_lWidth);
		m_oTrack.m_lHeight = (long)(dCoef * m_oTrack.m_lHeight);
	}

private:
	BOOL IsValidChanges()
	{
		if (m_bIsOnlyRotate)
			return TRUE;
		
		BOOL bRes = TRUE;		

		long lWidth = 0; long lHeight = 0;
		const BYTE* pBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, lWidth, lHeight);

		GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(lWidth);
		GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(lHeight);

		CRect oRect(m_oTrack.m_oWndRect.left, m_oTrack.m_oWndRect.top, 
			m_oTrack.m_oWndRect.right, m_oTrack.m_oWndRect.bottom);

		PointF* pPoints = GetImageRect(oRect);

		long lXc = (m_oTrack.m_oWndRect.left + m_oTrack.m_oWndRect.right) / 2;
		long lYc = (m_oTrack.m_oWndRect.top + m_oTrack.m_oWndRect.bottom) / 2;

		long lMinTrack = m_oTrack.GetMinTrack();
		if ((m_oTrack.m_lWidth < lMinTrack) || (m_oTrack.m_lHeight < lMinTrack)
			|| /*(!IsInRect(pPoints, lXc, lYc))*/ !CheckTrackInRect(pPoints))
		{	
			m_oTrack.SetMinBounds();
			bRes = FALSE;
		}

		if (m_oTrack.m_lWidth > lWidth)
		{
			m_oTrack.m_lWidth = lWidth;
			bRes = FALSE;
		}
		if (m_oTrack.m_lHeight > lHeight)
		{
			m_oTrack.m_lHeight = lHeight;
			bRes = FALSE;
		}

		/*if (bRes)
		{
			bRes = m_bIsCorrectTrack;
		}
		m_bIsCorrectTrack = TRUE;*/
		
		if (bRes)
		{
			SendChangeSize(pPoints);
		}

		if (bRes && m_oTrack.m_nFlagOld > 0)
		{
			m_lMouseChangedMaxX = m_oTrack.m_lWidth;
			m_lMouseChangedMaxY = m_oTrack.m_lHeight;
		}

		delete[] pPoints;

		return bRes;
	}


private:
	// geometric
	double GetDistance(double dX1, double dY1, double dX2, double dY2, float fAngle)
	{
		double dAngleRad = (M_PI * fAngle) / 180.0;
		double dA = cos(dAngleRad);
		double dB = sin(dAngleRad);
		double dC = -(dX2 * dA + dY2 * dB);

		return abs(dA * dX1 + dB * dY1 + dC) / _hypot(dA, dB);
	}
	BOOL IsInRect(PointF* pPoints, long x, long y)
	{
		HRGN hRgn;
		POINT pAr[4];
		
		pAr[0].x = (LONG)pPoints[0].X;
		pAr[0].y = (LONG)pPoints[0].Y;

		pAr[1].x = (LONG)pPoints[1].X;
		pAr[1].y = (LONG)pPoints[1].Y;

		pAr[2].x = (LONG)pPoints[3].X;
		pAr[2].y = (LONG)pPoints[3].Y;

		pAr[3].x = (LONG)pPoints[2].X;
		pAr[3].y = (LONG)pPoints[2].Y;

		hRgn = CreatePolygonRgn(pAr, 4, WINDING);
		BOOL bRes = (0 != PtInRegion(hRgn, int(x), int(y)));
		DeleteObject(hRgn);

		return bRes;
	}

	BOOL CheckTrackInRect(PointF* pPoints)
	{
		LONG lCx = (m_oTrack.m_oWndRect.left + m_oTrack.m_oWndRect.right) / 2;
		LONG lCy = (m_oTrack.m_oWndRect.top + m_oTrack.m_oWndRect.bottom) / 2;

		LONG lDeltaX = (m_oTrack.m_lWidth - 1) / 2;
		LONG lDeltaY = (m_oTrack.m_lHeight - 1) / 2;

		HRGN hRgn;
		POINT pAr[4];
		
		pAr[0].x = (LONG)pPoints[0].X;
		pAr[0].y = (LONG)pPoints[0].Y;

		pAr[1].x = (LONG)pPoints[1].X;
		pAr[1].y = (LONG)pPoints[1].Y;

		pAr[2].x = (LONG)pPoints[3].X;
		pAr[2].y = (LONG)pPoints[3].Y;

		pAr[3].x = (LONG)pPoints[2].X;
		pAr[3].y = (LONG)pPoints[2].Y;

		hRgn = CreatePolygonRgn(pAr, 4, WINDING);

		/*BOOL b1 = PtInRegion(hRgn, int(lCx - lDeltaX), int(lCy - lDeltaY));
		b1 = PtInRegion(hRgn, int(lCx - lDeltaX), int(lCy + lDeltaY));
		b1 = PtInRegion(hRgn, int(lCx + lDeltaX), int(lCy - lDeltaY));
		b1 = PtInRegion(hRgn, int(lCx + lDeltaX), int(lCy + lDeltaY));*/
		
		BOOL bRes =	(PtInRegion(hRgn, int(lCx - lDeltaX), int(lCy - lDeltaY)) &&
					 PtInRegion(hRgn, int(lCx - lDeltaX), int(lCy + lDeltaY)) &&
					 PtInRegion(hRgn, int(lCx + lDeltaX), int(lCy - lDeltaY)) &&
					 PtInRegion(hRgn, int(lCx + lDeltaX), int(lCy + lDeltaY)));
		
		DeleteObject(hRgn);

		return bRes;
	}

	BOOL IsInWndRect(long x, long y)
	{
		RectF oWindowRect = GetAppManager()->GetView()->GetWindowRect();
		
		REAL lLeft = oWindowRect.X;
		REAL lTop = oWindowRect.Y;
		REAL lRight = oWindowRect.X + oWindowRect.Width;
		REAL lBottom = oWindowRect.Y + oWindowRect.Height;
		
		if ((x >= lLeft) && (x <= lRight) && (y >= lTop) && (y <= lBottom))
		{
			return TRUE;
		}
		return FALSE;
	}	

	double GetDistanseToBorder(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
	{
		// расстояние между (x4, y4) и точкой пересечения прямых [(x1, y1)-(x2, y2)], [(x3, y3)-(x4, y4)]
		
		// возвращаем 0, если ничего пересчитывать не надо, иначе > 0
		double dZnam = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
		
		if (0 == dZnam)
			return 0;

		double dCisl1 = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3);
		double dCisl2 = (x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3);

		double dT1 = dCisl1 / dZnam;
		double dT2 = dCisl2 / dZnam;

		if ((0 >= dT1) || (dT1 >= 1) || (0 >= dT2) || (dT2 >= 1))
		{
			return 0;
		}

		// теперь берем точку пересечения...
		double dX = x1 + dT1 * (x2 - x1);
		double dY = y1 + dT1 * (y2 - y1);

		return _hypot(x4 - dX, y4 - dY);
	}

	void OffsetPoints(PointF* pPoints, int nCount)
	{
		for (int nIndex = 0; nIndex < nCount; ++nIndex)	
		{
			pPoints[nIndex].X += (float)m_dOffsetX;
			pPoints[nIndex].Y += (float)m_dOffsetY;
		}
	}

	void SendChangeSize(PointF* pPoints)
	{
		double dAngle = M_PI *  m_fAngle / 180.0;
		double dSin = abs(sin(dAngle));
		double dCos = abs(cos(dAngle));

		double dX = (double)m_lImageWidth / m_dCurWidth;
		double dY = (double)m_lImageHeight / m_dCurHeight;

		double dTW = (double)m_oTrack.m_lWidth;
		double dTH = (double)m_oTrack.m_lHeight;

		long lNewWidth = (long)(_hypot(dCos * m_lImageWidth / m_dCurWidth, dSin * m_lImageHeight / m_dCurHeight) * dTW);
		long lNewHeight = (long)(_hypot(dSin * m_lImageWidth / m_dCurWidth, dCos * m_lImageHeight / m_dCurHeight) * dTH);

		FireChangeParameter(0, (int)lNewWidth);
		FireChangeParameter(1, (int)lNewHeight);
	}

	void SetNewSizeX(LONG lWidth)
	{
		double dAngle = M_PI *  m_fAngle / 180.0;
		double dSin = abs(sin(dAngle));
		double dCos = abs(cos(dAngle));

		m_oTrack.m_lWidth = (LONG)ceil(lWidth / _hypot(dCos * m_lImageWidth / m_dCurWidth, dSin * m_lImageHeight / m_dCurHeight));

		m_lMouseChangedMaxX = m_oTrack.m_lWidth;
	}
	void SetNewSizeY(LONG lHeight)
	{
		double dAngle = M_PI *  m_fAngle / 180.0;
		double dSin = abs(sin(dAngle));
		double dCos = abs(cos(dAngle));

		m_oTrack.m_lHeight = (LONG)ceil(lHeight/ _hypot(dSin * m_lImageWidth / m_dCurWidth, dCos * m_lImageHeight / m_dCurHeight));

		m_lMouseChangedMaxY = m_oTrack.m_lHeight;
	}

	void CheckOnlyRotate()
	{
		if (!m_bIsOnlyRotate)
			return;

		LONG lAngle = (LONG)m_fAngle;
		if (0 == (lAngle % 180))
		{
			SetNewSizeX(m_lImageWidth);
			SetNewSizeY(m_lImageHeight);
		}
		else if (0 == (lAngle % 90))
		{
			SetNewSizeX(m_lImageHeight);
			SetNewSizeY(m_lImageWidth);
		}
	}

	BOOL CheckOnlyRotate(LONG& lNewWidth, LONG& lNewHeight)
	{
		if (!m_bIsOnlyRotate)
			return FALSE;

		LONG lAngle = (LONG)m_fAngle;
		if (0 == (lAngle % 180))
		{
			lNewWidth = m_lImageWidth;
			lNewHeight = m_lImageHeight;
		}
		else if (0 == (lAngle % 90))
		{
			lNewWidth = m_lImageHeight;
			lNewHeight = m_lImageWidth;
		}

		return TRUE;
	}

	void CheckOnSizeWindow(const CRect& oWndRect)
	{
		LONG lOldWidth = m_oTrack.m_oWndRect.right - m_oTrack.m_oWndRect.left;
		LONG lOldHeight = m_oTrack.m_oWndRect.bottom - m_oTrack.m_oWndRect.top;

		LONG lNewWidth = oWndRect.Width();
		LONG lNewHeight = oWndRect.Height();

		if ((lOldWidth == lNewWidth) || (lOldHeight == lNewHeight))
			return;
		
		double dKoefX = (double)lNewWidth / lOldWidth;
		double dKoefY = (double)lNewHeight / lOldHeight;

		m_lMouseChangedMaxX = (LONG)(dKoefX * m_lMouseChangedMaxX);
		m_lMouseChangedMaxY = (LONG)(dKoefY * m_lMouseChangedMaxY);

		m_oTrack.m_lWidth = (LONG)(dKoefX * m_oTrack.m_lWidth);
		m_oTrack.m_lHeight = (LONG)(dKoefY * m_oTrack.m_lHeight);

		IsValidChanges();
	}

	void UpdateImage(ILayer::IMAGE_TYPE lType)
	{
		//if ((NULL == m_pImage) || (lType != m_lImageType))
		{
			m_lImageType = lType;

			LONG lImageWidth	= 0;
			LONG lImageHeight	= 0;
			const BYTE* pBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(m_lImageType, lImageWidth, lImageHeight);

			RELEASEOBJECT(m_pImage);
			m_pImage = new Bitmap(lImageWidth, lImageHeight, 4 * lImageWidth, PixelFormat32bppARGB, const_cast<BYTE*>(pBuffer));

			// теперь посмотрим, изменились ли размеры - тогда нужно перестартовать команду
			if (ILayer::SLOW_COPY == lType)
			{
				if ((m_lImageWidth != lImageWidth) || (m_lImageHeight != lImageHeight))
				{
					StartCommand();
				}
			}
		}
	}
};

#endif