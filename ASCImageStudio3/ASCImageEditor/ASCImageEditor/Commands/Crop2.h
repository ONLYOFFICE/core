#ifndef __CROP_COMM_
#define __CROP_COMM_

#include "..//CommandManager.h"
#include "..//ViewManager.h"
/************************************************************************/
/* Crop COMMAND   -----   ID = 102                                      */
/************************************************************************/

#include "../../../Common/ASCUtils.h"
#include <math.h>

using namespace Gdiplus;
#include "GeometryHdr.h"
using namespace NSGeometry;

const double c_dBorder = 50;

class CCropCommand : public CCommand, public IChangesSubscriber, public ICommandButtonEvent, public ICommandTrackEvent
{
public:
	class CCropInfo
	{
	public:
		LONG m_lWidth;
		LONG m_lHeight;

		RectF m_oTrackRect;
		COffsetInfo m_oInfo;

		bool m_bIsMaxTrackUpdate;

	public:
		CCropInfo() : m_oInfo()
		{
			m_lWidth	= 0;
			m_lHeight	= 0;

			m_oTrackRect.X = 0;
			m_oTrackRect.Y = 0;
			m_oTrackRect.Width	= 0;
			m_oTrackRect.Height	= 0;

			m_bIsMaxTrackUpdate;
		}
	};
	class CNet
	{
	private:
		GraphicsPath	m_oPath;
		Pen				m_oPen;
	public:
		CNet() : m_oPath(), m_oPen(Color(50, 0xFF, 0xFF, 0xFF), 1)
		{
		}
		~CNet()
		{
		}

	public:
		void Init(const CDoubleRect& oRegion)
		{
			m_oPath.Reset();

			double dCx = (oRegion.left + oRegion.right) / 2.0;
			double dCy = (oRegion.top + oRegion.bottom) / 2.0;

			const double dInterval = 40.0;

			LONG lCountX = (LONG)(oRegion.GetWidth() / (2 * dInterval));
			LONG lCountY = (LONG)(oRegion.GetHeight() / (2 * dInterval));

			REAL left	= (REAL)oRegion.left;
			REAL top	= (REAL)oRegion.top;
			REAL right	= (REAL)oRegion.right;
			REAL bottom	= (REAL)oRegion.bottom;

			m_oPath.AddLine((REAL)dCx, top, (REAL)dCx, bottom); m_oPath.CloseFigure();
			m_oPath.AddLine(left, (REAL)dCy, right, (REAL)dCy); m_oPath.CloseFigure();

			LONG lIndex = 1;			
			while (lIndex <= lCountX)
			{
				REAL x1 = (REAL)(dCx + dInterval * lIndex);
				REAL x2 = (REAL)(dCx - dInterval * lIndex);
				m_oPath.AddLine(x1, top, x1, bottom); m_oPath.CloseFigure();
				m_oPath.AddLine(x2, top, x2, bottom); m_oPath.CloseFigure();

				++lIndex;
			}

			lIndex = 1;			
			while (lIndex <= lCountY)
			{
				REAL y1 = (REAL)(dCy + dInterval * lIndex);
				REAL y2 = (REAL)(dCy - dInterval * lIndex);
				m_oPath.AddLine(left, y1, right, y1); m_oPath.CloseFigure();
				m_oPath.AddLine(left, y2, right, y2); m_oPath.CloseFigure();

				++lIndex;
			}
		}

		void DrawOnGraphics(Graphics* pGr, const RectF& oRect)
		{
			pGr->SetClip(oRect);
			pGr->DrawPath(&m_oPen, &m_oPath);
			pGr->ResetClip();
		}
	};

	
public:
	CCropCommand(IAppManager* app_manager) : 
		CCommand(app_manager), 
		m_oTrack(this), 
		m_oApplyButton	(0, IDB_APPLY_NORM_PNG,	 IDB_APPLY_OVER_PNG,  IDB_APPLY_DOWN_PNG,  IDB_SPLIT_PNG, 1, this),
		m_oCancelButton	(1, IDB_CANCEL_NORM_PNG, IDB_CANCEL_OVER_PNG, IDB_CANCEL_DOWN_PNG, 0,			 -1, this)
	{
		DEV_LOG("Crop start constructor");
		// геометрическое место кнопок
		m_oApplyButton.SetBounds(20, 20, 50, 50);
		m_oCancelButton.SetBounds(90, 20, 50, 50);

		GetAppManager()->GetView()->FitImageToWindow();
		
		StartCommand();

		AddParameter(_T("Width"), _T("Width Value"),				(int)m_lImageWidth, 10, 2000);
		AddParameter(_T("Height"), _T("Height Value"),				(int)m_lImageHeight, 10, 2000);
		AddParameter(_T("Angle"), _T("Angle Value"),				(int)(180.0 * m_dAngle / M_PI), 0, 360);
		AddParameter(_T("OffsetX"), _T("OffsetX Value"),			(float)0, 0.0f, 1000.0f);
		AddParameter(_T("OffsetY"), _T("OffsetY Value"),			(float)0, 0.0f, 1000.0f);
		AddParameter(_T("AlwaysFullTrack"), _T("AlwaysFullTrack Value"), 0, 0, 1);
		AddParameter(_T("AspectTrack"), _T("AspectTrack Value"),	(float)m_dAspectTrack, -1.0f, 10.0f);
		AddParameter(_T("OnlyRotate"), _T("OnlyRotate Value"),		(int)m_bRotateCommand, 0, 1);

		GetAppManager()->GetView()->EnableDraw(false);
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->AddSubscriber(this);

		SetInvalidateFlag(INVALID_GRAPHICS);

		DEV_LOG("Crop end constructor");
		DEV_LOG("---Crop Command Begin");
	}
	
	void StartCommand()
	{
		m_oTrack.Clear();
		
		m_eType = ttNone;

		m_dAngle = 0.0;
		m_dAngleOld = 0.0;

		m_pImage = NULL;

		m_dAspectTrack = 0.0;

		const BYTE* pBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, m_lImageWidth, m_lImageHeight);

		m_lWidth	= m_lImageWidth;
		m_lHeight	= m_lImageHeight;

		m_oLastInfo.m_lWidth  = m_lWidth;
		m_oLastInfo.m_lHeight = m_lHeight;

		m_oCentreImage.x = (double)m_lImageWidth	/ 2.0;
		m_oCentreImage.y = (double)m_lImageHeight	/ 2.0;

		m_dWindowWidth	= 0;
		m_dWindowHeight	= 0;

		m_dWindowPixToImageDistanceX = -1.0;
		m_dWindowPixToImageDistanceY = -1.0;
		
		CalculateRegionImage();

		m_oCentreImageOld = m_oCentreImage;

		m_bRotateCommand = FALSE;
		m_bIsWaitDrawing = FALSE;

		UpdateImage(GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetActualImage(), false);
	}
	virtual ~CCropCommand() 
	{
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->RemoveSubscriber(this);
		GetAppManager()->GetView()->EnableDraw(true);
		RELEASEOBJECT(m_pImage);
	}

public:
	// кнопки
	bool IsButtonEnabled(LONG lFlag, LONG lX, LONG lY, LONG lButton)
	{
		bool b1 = m_oApplyButton.DoChange(lFlag, lX, lY, lButton);
		bool b2 = m_oCancelButton.DoChange(lFlag, lX, lY, lButton);

		if (!b1 && !b2)
			return false;

		return true;
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

	// мышь
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

		m_eType = ttNone;

		if (tteRotate == m_oTrack.m_nFlagOld)
		{
			if (ttRotate != m_eType)
			{
				m_oLastInfo.m_lWidth	= m_lWidth;
				m_oLastInfo.m_lHeight	= m_lHeight;
			}
			m_eType = ttRotate;
		}
		else
		{		
			m_oLastInfo.m_lWidth	= m_lWidth;
			m_oLastInfo.m_lHeight	= m_lHeight;

			m_oLastInfo.m_oTrackRect = m_oTrack.m_oRect;

			if (tteMove == m_oTrack.m_nFlagOld)
			{
				m_oCentreImageOld = m_oCentreImage;
				m_eType = ttMove;
			}
			else
			{
				m_eType = ttBound;
			}
		}
	}
	virtual void OnMouseUp(LONG nX, LONG nY, LONG nButton)
	{
		BOOL bInvalidate = FALSE;
		m_oCentreImageOld = m_oCentreImage;

		if (m_oLastInfo.m_bIsMaxTrackUpdate)
		{
			m_oLastInfo.m_oTrackRect = m_oTrack.m_oRect;
			m_oLastInfo.m_bIsMaxTrackUpdate = false;
		}
		CalculateCriticalParams();
		
		if (ttRotate == m_eType)
		{
			m_dAngleOld = m_dAngle;
			bInvalidate = TRUE;
		}
		
		m_eType = ttNone;
		m_oTrack.OnMouseUp(nX, nY, nButton);
		
		bool bIsButton = IsButtonEnabled(CCommandButton::btMouseUp, nX, nY, nButton);
		if (bIsButton || bInvalidate)
		{
			SetInvalidateFlag(INVALID_GRAPHICS);
			return;
		}

		m_oTrack.GetChange(nX, nY);
	}
	virtual void OnMouseMove(LONG nX, LONG nY, LONG nButton)	
	{
		if (m_oTrack.m_bIsMove && (nButton != MK_LBUTTON))
		{
			// это значит мы вышли за окно - и отжали за его перделами мышку
			m_oCentreImageOld = m_oCentreImage;

			if (m_oLastInfo.m_bIsMaxTrackUpdate)
			{
				m_oLastInfo.m_oTrackRect = m_oTrack.m_oRect;
				m_oLastInfo.m_bIsMaxTrackUpdate = false;
			}
			CalculateCriticalParams();
			
			if (ttRotate == m_eType)
			{
				m_dAngleOld = m_dAngle;
			}
			
			m_eType = ttNone;
			m_oTrack.OnMouseUp(nX, nY, nButton);
		}
		
		bool bIsButton = (ttNone == m_eType) ? IsButtonEnabled(CCommandButton::btMouseMove, nX, nY, 0) : false;
		if (bIsButton)
		{
			SetInvalidateFlag(INVALID_GRAPHICS);
			return;
		}

		if (!m_oTrack.m_bIsMove)
		{
			m_oTrack.OnMouseMove(nX, nY, (ttBound == m_eType));
			SetInvalidateFlag(INVALID_GRAPHICS);
			return;
		}
		
		if (ttRotate == m_eType)
		{
			SetCursor(LoadCursor(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDC_CURSOR_ROTATE)));

			double dCenterX = (m_oRegionImage.left + m_oRegionImage.right) / 2;
			double dCenterY = (m_oRegionImage.top + m_oRegionImage.bottom) / 2;

			double dZnam1 = (double)(m_oCursorOld.x - dCenterX);
			double dZnam2 = (double)(nX - dCenterX);

			double dCisl1 = (double)(m_oCursorOld.y - dCenterY);
			double dCisl2 = (double)(nY - dCenterY);

			double dAngleStart	= (dCisl1 >= 0) ? M_PI / 2 : -M_PI / 2;
			double dAngleCur	= (dCisl2 >= 0) ? M_PI / 2 : -M_PI / 2;
			
			if (dZnam1 != 0.0)
				dAngleStart = atan(dCisl1 / dZnam1);
			if (dZnam2 != 0.0)
				dAngleCur = atan(dCisl2 / dZnam2);

			if (m_oCursorOld.x < dCenterX)
			{
				dAngleStart += M_PI;
			}
			if (nX < dCenterX)
			{
				dAngleCur += M_PI;
			}

			m_dAngle = dAngleCur - dAngleStart;
			m_dAngle += m_dAngleOld;

			CheckRotate(tteRotate);

			FireChangeParameter(2, (int)(180.0 * m_dAngle / M_PI));			
		}
		else if (ttMove == m_eType)
		{
			SetCursor(LoadCursor(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDC_CURSOR_CLOSEHAND)));

			double dOffsetX = nX - m_oCursorOld.x;
			double dOffsetY = nY - m_oCursorOld.y;

			OffsetImage(dOffsetX, dOffsetY);
		}
		else
		{
			m_oTrack.OnMouseMove(nX, nY, ttBound == m_eType);


		}
		SetInvalidateFlag(INVALID_GRAPHICS);
	}
	virtual void OnMouseLeave()
	{
		/*if (ttRotate == m_eType)
		{
			m_dAngleOld = m_dAngle;
		}
		if (ttMove == m_eType)
		{
			m_oCentreImageOld = m_oCentreImage;
		}
		
		m_eType = ttNone;
		m_oTrack.m_bIsMove = false;*/
	}

	// клавиатура
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam)
	{
		DEV_LOG("OnKeyDown");

		if (m_oTrack.m_bIsMove)
			return;

		double dDelta = 1.0;

		switch (wParam)
		{
		case VK_LEFT:
			{
				OffsetImage(-dDelta, 0);
				break;
			}
		case VK_RIGHT:
			{
				OffsetImage(dDelta, 0);
				break;
			}
		case VK_UP:
			{
				OffsetImage(0, -dDelta);
				break;
			}
		case VK_DOWN:
			{
				OffsetImage(0, dDelta);
				break;
			}
		default:
			{
				return;
			}
		};

		m_oCentreImageOld = m_oCentreImage;

		SetInvalidateFlag(INVALID_GRAPHICS);
	}
	virtual void OnKeyUp(WPARAM wParam, LPARAM lParam)
	{
		DEV_LOG("OnKeyUp");
	}
	
	// events
	virtual void Undo(int nStepCount)
	{		
		if (0 >= nStepCount)
			return;

		m_bIsWaitDrawing = TRUE;
	}

	virtual void Redo(int nStepCount)
	{
		if (0 >= nStepCount)
			return;

		m_bIsWaitDrawing = TRUE;
	}

	virtual void OnChangeParameter(int paramInd) 
	{
		if (paramInd == 0 && m_params.GetCount() > 0)
		{
			m_lWidth = max(1, m_params[0]->m_value.lVal);

			if (m_oTrack.IsFixAspect())
			{
				double dHeight = (double)m_lWidth / m_oTrack.m_dAspectTrack;
				m_lHeight = max(1, (LONG)dHeight);
			}

			m_oLastInfo.m_lWidth  = m_lWidth;
			m_oLastInfo.m_lHeight = m_lHeight;

			CheckRotate();

			m_oLastInfo.m_lWidth  = m_lWidth;
			m_oLastInfo.m_lHeight = m_lHeight;

			CalculateCriticalParams();
		}
		else if (paramInd == 1 && m_params.GetCount() > 1)
		{
			m_lHeight = max(1, m_params[1]->m_value.lVal);

			if (m_oTrack.IsFixAspect())
			{
				double dWidth = (double)m_lHeight * m_oTrack.m_dAspectTrack;
				m_lWidth = max(1, (LONG)m_lWidth);
			}

			m_oLastInfo.m_lWidth  = m_lWidth;
			m_oLastInfo.m_lHeight = m_lHeight;

			CheckRotate();

			m_oLastInfo.m_lWidth  = m_lWidth;
			m_oLastInfo.m_lHeight = m_lHeight;

			CalculateCriticalParams();
		}
		else if (paramInd == 2 && m_params.GetCount() > 2)
		{
			LONG lOldX = m_lWidth;
			LONG lOldY = m_lHeight;
			
			m_dAngle = m_params[2]->m_value.lVal * M_PI / 180.0;
			m_dAngleOld = m_dAngle;

			CheckRotate();
			CalculateCriticalParams();

			if ((lOldX != m_lWidth) || (lOldY != m_lHeight))
			{
				SentChangedSizes();
			}
		}
		else if (paramInd == 6 && m_params.GetCount() > 6)
		{
			m_dAspectTrack = m_params[6]->m_value.dblVal;
			if (0.0 == m_dAspectTrack)
			{
				m_dAspectTrack = (double)m_oTrack.m_oRect.Width / m_oTrack.m_oRect.Height;
			}
			m_oTrack.m_dAspectTrack = m_dAspectTrack;

			CalculateCriticalParams();
		}
		else if (paramInd == 7 && m_params.GetCount() > 7)
		{
			m_bRotateCommand = (m_params[7]->m_value.lVal == 1);
		}

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

	virtual void DrawOnGraphics(Gdiplus::Graphics* pGr, const CRect& rect)
	{
		if (m_bIsWaitDrawing)
			return;
		
		if (NULL == pGr || NULL == m_pImage)
			return;

		pGr->Clear(GetAppManager()->GetView()->GetBackgroundColor());
		
		SmoothingMode mode = pGr->GetSmoothingMode();
		pGr->SetSmoothingMode(SmoothingModeAntiAlias);
				
		CalculateRegionImage();
		
		// вот здесь вместо clear - нужно рисовать подложку
		//ImageRendering::CChessBrush oAlphaBrush;
		//oAlphaBrush.OnPaint(pGr, Rect(rect.left, rect.top, rect.Width(), rect.Height()));
	
		PointF* pPoints = GetImageRegion();
		pGr->DrawImage(m_pImage, pPoints, 3);

		Region oReg;
		pGr->GetClip(&oReg);
		
		pGr->ExcludeClip(m_oTrack.m_oRect);
			
		SolidBrush oBrush(Color(0xA0, 0x00, 0x00, 0x00));
		pGr->FillRegion(&oBrush, &oReg);
		
		pGr->ResetClip();
		m_oTrack.DrawOnGraphics(pGr, pPoints);

		if (ttRotate == m_eType)
			m_oNet.DrawOnGraphics(pGr, m_oTrack.m_oRect);

		LONG lButtonWidth = 50;
		LONG lButtonHeight = 47;

		RectF oWndRect = m_oRegionImage.GetRectF();

		LONG lApplyLeft = (LONG)(oWndRect.X + oWndRect.Width / 2 - lButtonWidth);
		LONG lCancelLeft = (LONG)(oWndRect.X + oWndRect.Width / 2);
		
		//LONG lTop = (LONG)(oWndRect.GetBottom() - lButtonHeight - 10);
		LONG lTop = (LONG)(oWndRect.GetBottom() + 10);

		m_oApplyButton.SetBounds(lApplyLeft, lTop, lButtonWidth, lButtonHeight);
		m_oCancelButton.SetBounds(lCancelLeft, lTop, lButtonWidth, lButtonHeight);

		m_oApplyButton.DrawOnGraphics(pGr);
		m_oCancelButton.DrawOnGraphics(pGr);

		delete[] pPoints;

		pGr->SetSmoothingMode(mode);
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
		
		double dDegAngle = 180.0 * m_dAngle / M_PI;

		if (m_bRotateCommand)
		{
			if (0 != ((LONG)dDegAngle % 180))
			{
				m_lWidth	= m_lImageHeight;
				m_lHeight	= m_lImageWidth;
			}
			else
			{
				m_lWidth	= m_lImageWidth;
				m_lHeight	= m_lImageHeight;
			}
		}

		BYTE* pBuffer = new BYTE[4 * m_lWidth * m_lHeight];
		//memset(pBuffer, 0xFF, 4 * m_lWidth * m_lHeight);

		const BYTE* pSrcBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, m_lImageWidth, m_lImageHeight);

		double dCentreX = (double)m_lImageWidth  / 2.0;
		double dCentreY = (double)m_lImageHeight / 2.0;

		double dDeltaX  = m_oCentreImage.x - dCentreX;
		double dDeltaY  = m_oCentreImage.y - dCentreY;

		double dRad		= _hypot(dDeltaX, dDeltaY);

		double dBetta   = 0.0;

		if (dDeltaX == 0)
		{
			dBetta = (0 == dDeltaY) ? 0.0 : (dDeltaY > 0) ? M_PI / 2.0 : -M_PI / 2.0;
		}
		else
		{
			dBetta = atan(dDeltaY / dDeltaX);
			if (dDeltaX < 0)
				dBetta += M_PI;
		}

		dBetta += m_dAngle;
	
		double dResCentreX = (double)(m_lWidth  - 1) / 2.0 - dRad * cos(dBetta);
		double dResCentreY = (double)(m_lHeight - 1) / 2.0 - dRad * sin(dBetta);

		BGRA_Rotate_(const_cast<BYTE*>(pSrcBuffer), m_lImageWidth, m_lImageHeight, pBuffer, m_lWidth, m_lHeight, 
						dResCentreX, dResCentreY, dDegAngle, 1, 1, TRUE);			

		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StartHistoryBlock();
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangeAllLayer(pBuffer, m_lWidth, m_lHeight);
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StopHistoryBlock(_T("Crop"));

		GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangeImage(ILayer::SLOW, this);
		GetAppManager()->GetView()->FitImageToWindow();

		return true;
	}


private:
	class CTrackRectangle
	{
	public:
		RectF			m_oRect;

		Gdiplus::Pen	m_oPen;
		REAL			m_fEpsilon;

		POINT m_oCursor;

		bool m_bIsTrackEnabled;
		bool m_bIsMove;
		
		TrackTypeEx m_nFlagOld;
		TrackTypeEx m_eCurrentType;

		// аспект для трэка
		double m_dAspectTrack;

		ICommandTrackEvent* m_pCommand;

	public:
		CTrackRectangle(ICommandTrackEvent* pBase) : m_oPen(Gdiplus::Color::White, 1), m_pCommand(pBase)
		{
			m_oPen.SetDashStyle(Gdiplus::DashStyleSolid);
			Clear();			
		}

		void Clear()
		{
			m_oRect.X		= 0;
			m_oRect.Y		= 0;
			m_oRect.Width	= 0;
			m_oRect.Height	= 0;

			m_fEpsilon		= 5;

			m_oCursor.x = 0;
			m_oCursor.y = 0;

			m_bIsTrackEnabled	= false;
			m_bIsMove			= false;
			m_nFlagOld			= tteNone;

			m_dAspectTrack = -1;
		}

		inline bool IsFixAspect()
		{
			return (-1 != m_dAspectTrack);
		}

		void DrawOnGraphics(Gdiplus::Graphics* pGr, PointF* pointsBorder)
		{
			m_oPen.SetColor(Color(0x80, 0x80, 0x80));

			if (!m_bIsTrackEnabled)
			{
				m_oPen.SetColor(Color(0x80, 0x80, 0x80));
			}
			else
			{
				m_oPen.SetColor(Color(0xFF, 0xFF, 0xFF));
			}

			pGr->DrawRectangle(&m_oPen, m_oRect);

			REAL lLen = 14; REAL lOffset = 2;
			Gdiplus::RectF oRect;
			oRect.X			= 0;
			oRect.Y			= 0;
			oRect.Width		= lLen;
			oRect.Height	= lLen;

			SolidBrush oBrush(Color(0x80, 0x80, 0x80));
			if (m_bIsTrackEnabled)
			{
				oBrush.SetColor(Color::White);
			}	
			
			pGr->ExcludeClip(m_oRect);		

			if ((m_oRect.Width > 3 * lLen) && (m_oRect.Height > 3 * lLen))
			{
				oRect.X = m_oRect.X - lOffset; oRect.Y = m_oRect.Y - lOffset;
				pGr->FillRectangle(&oBrush, oRect);

				oRect.X = m_oRect.X + m_oRect.Width + lOffset - lLen + 1; oRect.Y = m_oRect.Y - lOffset;
				pGr->FillRectangle(&oBrush, oRect);

				oRect.X = m_oRect.X - lOffset; oRect.Y = m_oRect.Y + m_oRect.Height + lOffset - lLen + 1;
				pGr->FillRectangle(&oBrush, oRect);

				oRect.X = m_oRect.X + m_oRect.Width + lOffset - lLen + 1; oRect.Y = m_oRect.Y + m_oRect.Height + lOffset - lLen + 1;
				pGr->FillRectangle(&oBrush, oRect);
			}

			if ((m_oRect.Width > 5 * lLen) && (m_oRect.Height > 5 * lLen))
			{
				REAL lX2 = m_oRect.X + (m_oRect.Width - lLen) / 2; 
				REAL lY2 = m_oRect.Y + (m_oRect.Height - lLen) / 2; 
				
				oRect.X = lX2; oRect.Y = m_oRect.Y - lOffset;
				pGr->FillRectangle(&oBrush, oRect);

				oRect.X = m_oRect.X + m_oRect.Width + lOffset - lLen + 1; oRect.Y = lY2;
				pGr->FillRectangle(&oBrush, oRect);

				oRect.X = m_oRect.X - lOffset; oRect.Y = lY2;
				pGr->FillRectangle(&oBrush, oRect);

				oRect.X = lX2; oRect.Y = m_oRect.Y + m_oRect.Height + lOffset - lLen + 1;
				pGr->FillRectangle(&oBrush, oRect);
			}

			pGr->ResetClip();

			// теперь рисуем границу картинки
			GraphicsPath oPath;
			oPath.AddLine(pointsBorder[0], pointsBorder[1]);
			oPath.AddLine(pointsBorder[3], pointsBorder[2]);
			oPath.CloseFigure();

			if (tteMove == m_eCurrentType)
			{
				m_oPen.SetColor(Color(0xFF, 0xFF, 0xFF));
			}
			else
			{
				m_oPen.SetColor(Color(0x80, 0x80, 0x80));
			}

			pGr->DrawPath(&m_oPen, &oPath);
		}
		// 1 - left, 2 - top, 3 - right, 4 - bottom, 0 - In, -1 - nothing
		// 5 - left-top, 6 - top-right, 7 - right-bottom, 8 - bottom-left
		TrackTypeEx GetChange(int nX, int nY)
		{
			REAL left	= m_oRect.X;
			REAL top	= m_oRect.Y;
			REAL right	= m_oRect.GetRight();
			REAL bottom = m_oRect.GetBottom();

			REAL x = (REAL)nX;
			REAL y = (REAL)nY;

			HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
			
			if ((x < (left - m_fEpsilon)) || (x > (right + m_fEpsilon)) || 
						(y < (top - m_fEpsilon)) || (y > (bottom + m_fEpsilon)))
			{
				SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_ROTATE)));
				m_eCurrentType = tteRotate;
				return tteRotate;
			}
			
			BOOL b1 = PointOnSegment(left,  top,    left,  bottom, x, y, m_fEpsilon);
			BOOL b2 = PointOnSegment(left,  top,    right, top,    x, y, m_fEpsilon);
			BOOL b3 = PointOnSegment(right, top,    right, bottom, x, y, m_fEpsilon);
			BOOL b4 = PointOnSegment(left,  bottom, right, bottom, x, y, m_fEpsilon);

			BOOL bNoFixAspect = (-1 == m_dAspectTrack);

			TrackTypeEx flag = tteRotate;
			if ((x >= (left + m_fEpsilon)) && (x <= (right - m_fEpsilon)) && 
						(y >= (top + m_fEpsilon)) && (y <= (bottom - m_fEpsilon)))
			{
				flag = tteMove;
			}
			else 
			{
				REAL n = min(right - left, bottom - top);
				if ((n < 3 * m_fEpsilon) && ((x >= left) && (x <= right) && (y >= top) && (y <= bottom)))
				{
					flag = tteMove;
				}
				else
				{
					if		(b1 && b2 && bNoFixAspect) flag = tteLeftTop;
					else if (b2 && b3 && bNoFixAspect) flag = tteRightTop;
					else if (b3 && b4 && bNoFixAspect) flag = tteRightBottom;
					else if (b4 && b1 && bNoFixAspect) flag = tteLeftBottom;
					
					else if (b1 && (y > top ) && (y < bottom)) flag = tteLeft;
					else if (b2 && (x > left) && (x < right )) flag = tteTop;
					else if (b3 && (y > top ) && (y < bottom)) flag = tteRight; 
					else if (b4 && (x > left) && (x < right )) flag = tteBottom;
				}
			}

			switch (flag)
			{
				case tteLeft:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE_H)));
					break;
				case tteTop:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE_V)));
					break;
				case tteRight:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE_H)));
					break;
				case tteBottom:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE_V)));
					break;
				case tteLeftTop:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE2)));
					break;
				case tteRightTop:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE3)));
					break;
				case tteRightBottom:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE2)));
					break;
				case tteLeftBottom:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE3)));
					break;
				case tteMove: 
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_HAND)));
					break;
				default:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_ROTATE)));
					break;
			}

			m_eCurrentType = flag;
			return flag;
		}

		void FlagOldToCursor()
		{
			HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
			switch (m_nFlagOld)
			{
				case tteLeft:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE_H)));
					break;
				case tteTop:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE_V)));
					break;
				case tteRight:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE_H)));
					break;
				case tteBottom:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE_V)));
					break;
				case tteLeftTop:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE2)));
					break;
				case tteRightTop:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE3)));
					break;
				case tteRightBottom:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE2)));
					break;
				case tteLeftBottom:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE3)));
					break;
				case tteMove: 
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_HAND)));
					break;
				default:
					SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_ROTATE)));
					break;
			}
		}

		void DoChange(int flag, REAL fX, REAL fY)
		{
			REAL fCx = m_oRect.X + m_oRect.Width  / 2;
			REAL fCy = m_oRect.Y + m_oRect.Height / 2;

			bool bFixAspect = IsFixAspect();

			double dWidthOld	= (double)m_oRect.Width;
			double dHeigthOld	= (double)m_oRect.Height;

			switch (flag)
			{
				case tteMove: 
					break;
				case tteLeft:
					{
						m_oRect.Width = max(1, 2 * (fCx - fX));
						if (bFixAspect)
						{
							m_oRect.Height = (REAL)(max((double)m_oRect.Width / m_dAspectTrack, 1));
						}
						break;
					}
				case tteRight:
					{
						m_oRect.Width = max(1, 2 * (fX - fCx));
						if (bFixAspect)
						{
							m_oRect.Height = (REAL)(max((double)m_oRect.Width / m_dAspectTrack, 1));
						}
						break;
					}
				case tteTop:
					{
						m_oRect.Height = max(1, 2 * (fCy - fY));
						if (bFixAspect)
						{
							m_oRect.Width = (REAL)(max(m_dAspectTrack * m_oRect.Height, 1));
						}
						break;
					}
				case tteBottom:
					{
						m_oRect.Height = max(1, 2 * (fY - fCy));
						if (bFixAspect)
						{
							m_oRect.Width = (REAL)(max(m_dAspectTrack * m_oRect.Height, 1));
						}
						break;
					}
				case tteLeftTop:
				case tteRightTop:
				case tteLeftBottom:
				case tteRightBottom:
					{
						if (bFixAspect)
						{
							break;
						}
						REAL width  = ((tteLeftTop == flag) || (tteLeftBottom == flag)) ? (2 * (fCx - fX)) : (2 * (fX - fCx));
						REAL height = ((tteLeftTop == flag) || (tteRightTop   == flag)) ? (2 * (fCy - fY)) : (2 * (fY - fCy));
						m_oRect.Width	= max(1, width);
						m_oRect.Height	= max(1, height);
						break;
					}
				default:
					break;
			};

			if (NULL != m_pCommand)
				m_pCommand->TrackEvent(flag, (double)(m_oRect.Width - dWidthOld), (double)(m_oRect.Height - dHeigthOld));
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
				TrackTypeEx result = GetChange(nX, nY);
				m_bIsTrackEnabled = (result != tteRotate && result != tteMove) ? true : false;
				return;
			}
			int dX = nX - m_oCursor.x;
			int dY = nY - m_oCursor.y;
			
			TrackTypeEx result = m_nFlagOld;
			if (!bIsTracking)
			{
				result = GetChange(m_oCursor.x, m_oCursor.y);
			}
			else
			{
				FlagOldToCursor();
			}

			m_bIsTrackEnabled = (result != tteRotate && result != tteMove) ? true : false;

			if (result != m_nFlagOld)
			{
				return;
			}

			if (result == tteRotate)
				return;

			m_oCursor.x += dX;
			m_oCursor.y += dY;

			DoChange(result, (REAL)nX, (REAL)nY);
		}
	};

protected:
	virtual void TrackEvent(LONG lType, double dParamX, double dParamY)
	{
		if (0 == dParamX && 0 == dParamY)
			return;

		CDoubleRect oSourceRect;
		oSourceRect.left	= 0;
		oSourceRect.top		= 0;
		oSourceRect.right	= m_lImageWidth;
		oSourceRect.bottom	= m_lImageHeight;

		oSourceRect.Offset(-m_oCentreImageOld.x, -m_oCentreImageOld.y);

		CDoublePoint oPoint;
		oPoint.x = 0;
		oPoint.y = 0;
		
		CDoubleRectRotate oSourceRotate(oSourceRect, oPoint, m_dAngle);

		oSourceRect.left	= -(double)m_oLastInfo.m_lWidth  / 2.0;
		oSourceRect.top		= -(double)m_oLastInfo.m_lHeight / 2.0;
		oSourceRect.right	=  (double)m_oLastInfo.m_lWidth  / 2.0;
		oSourceRect.bottom	=  (double)m_oLastInfo.m_lHeight / 2.0;

		double dParam1 = (double)(m_oTrack.m_oRect.Width  - m_oLastInfo.m_oTrackRect.Width);
		double dParam2 = (double)(m_oTrack.m_oRect.Height - m_oLastInfo.m_oTrackRect.Height);

		// заглушка под ошибку округления
		if ((tteLeft == m_oTrack.m_eCurrentType) || (tteRight == m_oTrack.m_eCurrentType))
			dParam2 = 0.0;
		else if ((tteTop == m_oTrack.m_eCurrentType) || (tteBottom == m_oTrack.m_eCurrentType))
			dParam1 = 0.0;

		double dParamMem1 = dParam1 / m_dWindowPixToImageDistanceX;
		double dParamMem2 = dParam2 / m_dWindowPixToImageDistanceY;

		double dWidth  = dParamMem1 + m_oLastInfo.m_lWidth;
		double dHeight = dParamMem2 + m_oLastInfo.m_lHeight;

		oSourceRotate.Normalize(m_oTrack.m_eCurrentType, oSourceRect, dWidth, dHeight, m_oTrack.m_dAspectTrack, m_oLastInfo.m_oInfo);

		dParamMem1 = dWidth  - m_oLastInfo.m_lWidth;
		dParamMem2 = dHeight - m_oLastInfo.m_lHeight;
		
		dParam1 = dParamMem1 * m_dWindowPixToImageDistanceX / 2.0;
		dParam2 = dParamMem2 * m_dWindowPixToImageDistanceY / 2.0;

		m_lWidth	= (LONG)dWidth;
		m_lHeight	= (LONG)dHeight;

		CDoubleRectRotate oRectRotate = GetImageRegion2();

		double dCosA = cos(m_dAngle);
		double dSinA = sin(m_dAngle);

		double dKoefX = ((double)m_lImageWidth  / oRectRotate.GetWidth());
		double dKoefY = ((double)m_lImageHeight / oRectRotate.GetHeight());

		double dOffsetX_hor = dKoefX * dParam1 * dCosA;
		double dOffsetY_hor = dKoefY * dParam2 * dSinA;

		double dOffsetX_ver = dKoefX * dParam1 * dSinA;
		double dOffsetY_ver = dKoefY * dParam2 * dCosA;

		CDoublePoint oPointNewCentre = m_oCentreImageOld;

		switch (lType)
		{
		case tteLeft:
			{
				oPointNewCentre.x += (-dOffsetX_hor);	// cos(180 - a) = -cos(a)
				oPointNewCentre.y += (dOffsetX_ver);		// sin(180 - a) = sin(a)

				break;
			}
		case tteRight:
			{
				oPointNewCentre.x += (dOffsetX_hor);  
				oPointNewCentre.y += (-dOffsetX_ver);	// sin(-a) = sin(a)

				break;
			}
		case tteTop:
			{
				oPointNewCentre.x += -(dOffsetY_hor);	// cos(-a) = cos(a)
				oPointNewCentre.y += (-dOffsetY_ver);	// sin(-a) = -sin(a)

				break;
			}
		case tteBottom:
			{
				oPointNewCentre.x += -(-dOffsetY_hor);	// cos(180 - a) = -cos(a)
				oPointNewCentre.y += (dOffsetY_ver);		// sin(180 - a) = sin(a)

				break;
			}
		case tteLeftTop:
			{
				oPointNewCentre.x += (-dOffsetX_hor);	// cos(180 - a) = -cos(a)
				oPointNewCentre.y += (dOffsetX_ver);		// sin(180 - a) = sin(a)

				oPointNewCentre.x += -(dOffsetY_hor);	// cos(-a) = cos(a)
				oPointNewCentre.y += (-dOffsetY_ver);	// sin(-a) = -sin(a)

				break;
			}
		case tteLeftBottom:
			{
				oPointNewCentre.x += (-dOffsetX_hor);	// cos(180 - a) = -cos(a)
				oPointNewCentre.y += (dOffsetX_ver);		// sin(180 - a) = sin(a)

				oPointNewCentre.x += -(-dOffsetY_hor);	// cos(180 - a) = -cos(a)
				oPointNewCentre.y += (dOffsetY_ver);		// sin(180 - a) = sin(a)

				break;
			}
		case tteRightTop:
			{
				oPointNewCentre.x += (dOffsetX_hor);  
				oPointNewCentre.y += (-dOffsetX_ver);	// sin(-a) = sin(a)

				oPointNewCentre.x += -(dOffsetY_hor);	// cos(-a) = cos(a)
				oPointNewCentre.y += (-dOffsetY_ver);	// sin(-a) = -sin(a)

				break;
			}
		case tteRightBottom:
			{
				oPointNewCentre.x += (dOffsetX_hor);  
				oPointNewCentre.y += (-dOffsetX_ver);	// sin(-a) = sin(a)

				oPointNewCentre.x += -(-dOffsetY_hor);	// cos(180 - a) = -cos(a)
				oPointNewCentre.y += (dOffsetY_ver);		// sin(180 - a) = sin(a)

				break;
			}
		};

		m_lWidth  = max(1, m_lWidth);
		m_lHeight = max(1, m_lHeight);

		CalculateTrackRect();
		m_oCentreImage = oPointNewCentre;

		m_oLastInfo.m_bIsMaxTrackUpdate = true;

		SentChangedSizes();
	}

private:
	CDoubleRect			m_oRegionImage;					// в координатах экрана
	double				m_dWindowWidth;					// ширина экрана
	double				m_dWindowHeight;				// высота экрана

	double				m_dWindowPixToImageDistanceX;	// сдвинули курсор на 1 pix - на сколько пикселов сдвинулись в картинке (по горизонтали)
	double				m_dWindowPixToImageDistanceY;	// сдвинули курсор на 1 pix - на сколько пикселов сдвинулись в картинке (по вертикали)

	CDoublePoint		m_oCentreImage;					// точка, относительно которой вращается картинка (в координатах картинки)
	CDoublePoint		m_oCentreImageOld;				// точка, относительно которой вращалась картинка ранее

	double				m_dAngle;						// угол (в радианах)
	double				m_dAngleOld;					// угол (старый) в радианах

	LONG				m_lWidth;						// ширина кропа (в координатах картинки)
	LONG				m_lHeight;						// высота кропа (в координатах картинки)	

	double				m_dAspectTrack;					// соотношения сторон кропа
	CDoubleRect			m_oMaxRext;						// максимальный выставленный кроп (чтоб при повороте не только уменьшать, а и увеличивать)

	LONG				m_lImageWidth;					// ширина картинки
	LONG				m_lImageHeight;					// высота картинки

	CCommandButton		m_oApplyButton;					// кнопка применения команды
	CCommandButton		m_oCancelButton;				// кнопка отмены команды

	Bitmap*				m_pImage;						// битмап (пикселы - текущая картинка)
	ILayer::IMAGE_TYPE	m_lImageType;					// текущая картинка (FAST_COPY || SLOW_COPY)

	TrackType			m_eType;						// какое действие происходит

	POINT				m_oCursorOld;					// положение курсора
	CTrackRectangle		m_oTrack;						// трек

	BOOL				m_bRotateCommand;				// надо было отдельную команду для ротейта
	BOOL				m_bIsWaitDrawing;				// повременить с отрисовкой

	CNet				m_oNet;							// при повороте показываем сетку
	CCropInfo			m_oLastInfo;					// последнее состояние

protected:
	void UpdateWindowRect(double dWidth, double dHeight)
	{
		if (m_dWindowWidth == dWidth && m_dWindowHeight == dHeight)
			return;

		m_dWindowWidth	= dWidth;
		m_dWindowHeight	= dHeight;

		m_oRegionImage.left		= c_dBorder;
		m_oRegionImage.top		= c_dBorder / 2;
		m_oRegionImage.right	= dWidth	- c_dBorder;
		m_oRegionImage.bottom	= dHeight	- 3 * c_dBorder / 2.0;

		GetImageRegion2();

		CalculateTrackRect();
		m_oLastInfo.m_oTrackRect = m_oTrack.m_oRect;

		// здесь нужно пересчитать сетку
		m_oNet.Init(m_oRegionImage);
	}
	
	CDoubleRectRotate GetImageRegion2()
	{
		// здесь: расчитываем точки с учетом поворота (максимальный влезающий рект по аспекту в m_oRegionImage)
		// а потом сдвигаем его так, чтобы центр m_oCentreImage - оказался в центре экрана

		// 1) расчитываем рект, без поворота
		CDoubleRect oRect;
		double dAspectImage		= (double)m_lImageWidth / m_lImageHeight;
		double dAspectWindow	= m_oRegionImage.GetWidth() / m_oRegionImage.GetHeight();
		
		if (dAspectImage >= dAspectWindow)
		{
			oRect.left		= m_oRegionImage.left;
			oRect.right		= m_oRegionImage.right;

			double dHeight	= m_oRegionImage.GetWidth() / dAspectImage;
			double dDelta	= (m_oRegionImage.GetHeight() - dHeight) / 2.0;
			oRect.top		= m_oRegionImage.top + dDelta;
			oRect.bottom	= m_oRegionImage.bottom - dDelta;
		}
		else
		{
			oRect.top		= m_oRegionImage.top;
			oRect.bottom	= m_oRegionImage.bottom;

			double dWidth	= m_oRegionImage.GetHeight() * dAspectImage;
			double dDelta	= (m_oRegionImage.GetWidth() - dWidth) / 2.0;
			oRect.left		= m_oRegionImage.left + dDelta;
			oRect.right		= m_oRegionImage.right - dDelta;
		}

		// теперь повернем его
		CDoubleRectRotate oRectRotate(oRect, m_dAngle);

		// сделаем чтобы он влез в m_oRegionImage
		oRectRotate.Normalize(m_oRegionImage);

		if (0 == m_oTrack.m_oRect.Width)
		{
			m_oTrack.m_oRect = oRect.GetRectF();
			m_oLastInfo.m_oTrackRect = m_oTrack.m_oRect;

			CalculateCriticalParams();
		}
		
		double dSin = abs(sin(m_dAngle));
		double dCos	= abs(cos(m_dAngle));

		double dWidthInImage	= m_lImageWidth * dCos + m_lImageHeight * dSin;
		double dHeightInImage	= m_lImageWidth * dSin + m_lImageHeight * dCos;

		//m_dWindowPixToImageDistanceX = oRectRotate.GetMaxDistanceHor() / dWidthInImage;
		//m_dWindowPixToImageDistanceY = oRectRotate.GetMaxDistanceVer() / dHeightInImage;

		m_dWindowPixToImageDistanceX = oRectRotate.GetWidth()  / m_lImageWidth;
		m_dWindowPixToImageDistanceY = oRectRotate.GetHeight() / m_lImageHeight;

		// теперь нужно сдвинуть oRectRotate так, чтобы
		// m_oCentreImage - попала в центр окна
		
		double dNewWidth	= oRectRotate.GetWidth();
		double dNewHeight	= oRectRotate.GetHeight();

		double dCentreX		= oRect.GetCentreX();
		double dCentreY		= oRect.GetCentreY();

		double dImageCentreX = (double)m_lImageWidth  / 2.0;
		double dImageCentreY = (double)m_lImageHeight / 2.0;

		double dOffsetX = m_oCentreImage.x - dImageCentreX;
		double dOffsetY	= m_oCentreImage.y - dImageCentreY;

		double dRadX		= dNewWidth  * dOffsetX / m_lImageWidth;
		double dRadY		= dNewHeight * dOffsetY / m_lImageHeight;

		double dBetta = 0;

		if (dOffsetX == 0)
		{
			dBetta = (0 == dOffsetY) ? 0.0 : (dOffsetY > 0) ? -M_PI / 2.0 : M_PI / 2.0;
		}
		else
		{
			dBetta = atan(dOffsetY / dOffsetX);
			if (dOffsetX > 0)
				dBetta += M_PI;
		}

		dBetta += m_dAngle;

		double dDist = _hypot(dRadX, dRadY);

		/*double dCos_	= (dOffsetX >= 0) ? -abs(cos(dBetta)) : abs(cos(dBetta));
		double dSin_	= (dOffsetY >= 0) ? -abs(sin(dBetta)) : abs(sin(dBetta));*/

		double dCos_	= cos(dBetta);
		double dSin_	= sin(dBetta);

		double dX = dDist * dCos_;
		double dY = dDist * dSin_;

		oRectRotate.Offset(dX, dY);
		return oRectRotate;
	}
	
	PointF* GetImageRegion()
	{
		CDoubleRectRotate oRectRotate = GetImageRegion2();
		return oRectRotate.GetPoints();
	}

	void UpdateImage(ILayer::IMAGE_TYPE lType, bool bAttack = true)
	{
		m_lImageType = lType;

		LONG lImageWidth	= 0;
		LONG lImageHeight	= 0;
		const BYTE* pBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(m_lImageType, lImageWidth, lImageHeight);

		RELEASEOBJECT(m_pImage);
		m_pImage = new Bitmap(lImageWidth, lImageHeight, 4 * lImageWidth, PixelFormat32bppARGB, const_cast<BYTE*>(pBuffer));

		if (bAttack)
		{
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

	void CalculateTrackRect()
	{		
		// здесь по всем "картиночным" параметрам - расчитаем рект для трека
		m_oTrack.m_oRect.Width	= (REAL)((double)m_lWidth  * m_dWindowPixToImageDistanceX);
		m_oTrack.m_oRect.Height = (REAL)((double)m_lHeight * m_dWindowPixToImageDistanceY);
		m_oTrack.m_oRect.X		= (REAL)(m_oRegionImage.GetCentreX() - m_oTrack.m_oRect.Width / 2);
		m_oTrack.m_oRect.Y		= (REAL)(m_oRegionImage.GetCentreY() - m_oTrack.m_oRect.Height / 2);
	}

	void CheckRotate(LONG type = tteRotate)
	{
		// здесь оставляем ширину / высоту как m_oLastInfo.m_lWidth, m_oLastInfo.m_lHeight, 
		// если влезает. А если нет - то просто масштабируем размеры до максимально
		// влезающего
		CDoubleRect oRect;
		oRect.left		= 0.0;
		oRect.top		= 0.0;
		oRect.right		= (double)m_lImageWidth;
		oRect.bottom	= (double)m_lImageHeight;

		CDoubleRectRotate oRectRotate(oRect, m_oCentreImage, m_dAngle);

		m_lWidth	= m_oLastInfo.m_lWidth;
		m_lHeight	= m_oLastInfo.m_lHeight;

		CDoubleRect	oRectCrop;
		oRectCrop.left		= -(double)m_lWidth  / 2.0;
		oRectCrop.right		=  (double)m_lWidth  / 2.0;
		oRectCrop.top		= -(double)m_lHeight / 2.0;
		oRectCrop.bottom	=  (double)m_lHeight / 2.0;

		oRectCrop.Offset(m_oCentreImage.x, m_oCentreImage.y);

		oRectRotate.Normalize2(oRectCrop);

		m_lWidth  = (LONG)max(oRectCrop.GetWidth(), 1.0);
		m_lHeight = (LONG)max(oRectCrop.GetHeight(), 1.0);

		if ((m_lWidth != m_oLastInfo.m_lWidth) || (m_lHeight != m_oLastInfo.m_lHeight))
		{
			SentChangedSizes();
		}

		// угол мог измениться - надо параметры пересчитать
		CDoubleRectRotate oRectRotatePix = GetImageRegion2();
		m_dWindowPixToImageDistanceX = oRectRotatePix.GetWidth()  / m_lImageWidth;
		m_dWindowPixToImageDistanceY = oRectRotatePix.GetHeight() / m_lImageHeight;

		CalculateTrackRect();
	}

	void CalculateRegionImage()
	{
		RectF oBackRect = GetAppManager()->GetView()->GetImageRect();

		double dWidthWindow		= oBackRect.X * 2 + oBackRect.Width;
		double dHeightWindow	= oBackRect.Y * 2 + oBackRect.Height;

		UpdateWindowRect(dWidthWindow, dHeightWindow);
	}

	void CalculateCriticalParams()
	{
		// эта функция высчитывает критические значения для смещений
		CDoubleRect oSourceRect;
		oSourceRect.left	= 0;
		oSourceRect.top		= 0;
		oSourceRect.right	= m_lImageWidth;
		oSourceRect.bottom	= m_lImageHeight;

		oSourceRect.Offset(-m_oCentreImage.x, -m_oCentreImage.y);

		CDoublePoint oPoint;
		oPoint.x = 0;
		oPoint.y = 0;
		
		CDoubleRectRotate oSourceRotate(oSourceRect, oPoint, m_dAngle);

		oSourceRect.left	= -(double)m_lWidth  / 2.0;
		oSourceRect.top		= -(double)m_lHeight / 2.0;
		oSourceRect.right	= (double)m_lWidth  / 2.0;
		oSourceRect.bottom	= (double)m_lHeight / 2.0;

		m_oLastInfo.m_oInfo = oSourceRotate.GetCritical(oSourceRect);
	}

	void SentChangedSizes()
	{
		FireChangeParameter(0, m_lWidth);
		FireChangeParameter(1, m_lHeight);
	}

	void OffsetImage(double dOffsetX, double dOffsetY)
	{
		double dDist = _hypot(dOffsetX, dOffsetY);

		double dBetta = 0;

		if (dOffsetX == 0)
		{
			dBetta = (dOffsetY >= 0) ? M_PI / 2.0 : -M_PI / 2.0;
		}
		else
		{
			dBetta = atan(dOffsetY / dOffsetX);
			if (dOffsetX < 0)
				dBetta += M_PI;
		}
		dBetta -= m_dAngle;

		CDoubleRectRotate oRectRotate = GetImageRegion2();
		double dW = oRectRotate.GetWidth();
		double dH = oRectRotate.GetHeight();

		double dRadX = dDist * cos(dBetta);
		double dRadY = dDist * sin(dBetta);

		double dX = dRadX * m_lImageWidth  / dW;
		double dY = dRadY * m_lImageHeight / dH;

		CDoublePoint oPointCentre;
		oPointCentre.x = m_oCentreImageOld.x - dX;
		oPointCentre.y = m_oCentreImageOld.y - dY;

		m_oLastInfo.m_oInfo.m_oCentresRect.CorrectCentre(oPointCentre);

		m_oCentreImage = oPointCentre;
	}
};

#endif