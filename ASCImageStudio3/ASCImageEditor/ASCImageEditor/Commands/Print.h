#ifndef __PRINT_COMM_
#define __PRINT_COMM_

#include "..//CommandManager.h"
#include "..//ViewManager.h"
/************************************************************************/
/* PRINT COMMAND   -----   ID = 110                                    */
/************************************************************************/

#include "ASCUtils.h"
#include <math.h>
#include "Commands/CommandsUtils.h"
//#include "atlprint.h"


class CPrintCommand : public CCommand, public IChangesSubscriber, public ICoordinatSystem
{
	static const int INDENT = 100;

public:
	CPrintCommand(IAppManager* app_manager):CCommand(app_manager)
	{
		AddParameter(_T("Page Sizes Proportion"), _T("Proportion"), 1.4f, 1.0f, 10.0f);	
		AddParameter(_T("Start"), _T("Start print process"), 0, 0, 1);	
		AddParameter(_T("Printer"), _T("Current printer contex"), 0, 0, 1);

		AddParameter(_T("Center"), _T("Center image"), 0, 0, 1);
		AddParameter(_T("X"), _T("Image left"), 0, 0, 1000);
		AddParameter(_T("Y"), _T("Image Top"), 0, 0, 1000);

		AddParameter(_T("Page Width"), _T("Width"), 1000, 10, 10000);	
		AddParameter(_T("Page Height"), _T("Height"), 1000, 10, 10000);	

		AddParameter(_T("Image Width"), _T("ImageWidth"), 1000, 10, 10000);	
		AddParameter(_T("Image Height"), _T("ImageHeight"), 1000, 10, 10000);	
		AddParameter(_T("ScaledToFitMedia"), _T("ScaledToFitMedia"), 0, 0, 1);

		long imW, imH;
		BYTE* pBufferTmp = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::DUBLICATE, imW, imH);
		if (!pBufferTmp)
			return;

		MediaCore::IAVSUncompressedVideoFrame* pMediaData = (MediaCore::IAVSUncompressedVideoFrame*)pBufferTmp;

		BYTE* pBufferPtr = NULL;
		pMediaData->get_Buffer(&pBufferPtr);


		bool reg = GetAppManager()->GetLayersManager()->DrawLogo(&pMediaData, true);

		pMediaData->get_Buffer(&pBufferPtr);

		//m_pImage = new Bitmap(imW, imH, 4*imW, PixelFormat32bppARGB, const_cast<BYTE*>(pBufferPtr));
		m_pImage = new Bitmap( imW, imH, PixelFormat32bppARGB );
		if( m_pImage && m_pImage->GetLastStatus() == Gdiplus::Ok )
		{
			Gdiplus::BitmapData oData;
			oData.Width = imW;
			oData.Height = imH;
			oData.PixelFormat = PixelFormat32bppARGB;
			oData.Stride = imW * 4;
			oData.Scan0 = pBufferPtr;

			m_pImage->LockBits( NULL, ImageLockModeUserInputBuf | ImageLockModeWrite, PixelFormat32bppARGB, &oData );
			m_pImage->UnlockBits( &oData );
		}
		pMediaData->Release();

		//GetAppManager()->GetView()->FitImageToWindow();
		GetAppManager()->GetView()->EnableDraw(false);

		int   scrW = (int)GetAppManager()->GetView()->GetWindowRect().Width;
		int   scrH = (int)GetAppManager()->GetView()->GetWindowRect().Height;

		m_current_sizes[0] = scrW;
		m_current_sizes[1] = scrH;

	    m_page_rect = CalcPageRect(m_current_sizes[0], m_current_sizes[1]);

		m_rect = new TrackRect(this);
		m_rect->RecalcSizes((int)m_page_rect.Width, (int)m_page_rect.Height);
		m_rect->SetPos(20, 50, 200, (int)(200*(float)imH/(float)imW));
		m_rect->m_isVisible = true;
		m_rect->SetContainerLimitFlag(false);
		m_rect->SetProportions((float)imH/(float)imW);

		 m_page_W	=	m_page_H	=	0;
		 m_zoom		=	0.0;

		 m_coeff = 1.0f;

		 m_indraw_flag = IDF_NONE;

		SetInvalidateFlag(INVALID_GRAPHICS);
	}
	virtual ~CPrintCommand() 
	{
		delete m_rect;
		delete m_pImage;
		GetAppManager()->GetView()->EnableDraw(true);
//		GetAppManager()->GetLayersManager()->GetCurrentLayer()->RemoveSubscriber(this);
	}
public:
	
	virtual void OnMouseDown(LONG nX, LONG nY, LONG nButton)	
	{
		if (m_rect && m_rect->OnMouseDown(nX,nY,nButton))
			SetInvalidateFlag(INVALID_GRAPHICS);
	}
	virtual void OnMouseUp(LONG nX, LONG nY, LONG nButton)
	{
		if (m_rect && m_rect->OnMouseUp(nX,nY,nButton))
			SetInvalidateFlag(INVALID_GRAPHICS);
	}
	virtual void OnMouseMove(LONG nX, LONG nY, LONG nButton)	
	{
		if (m_rect && m_rect->OnMouseMove(nX,nY,nButton))
		{
			FireChangeParameter(4, (int)((float)m_rect->m_x*m_coeff));
			FireChangeParameter(5, (int)((float)m_rect->m_y*m_coeff));

			FireChangeParameter(8, (int)((float)m_rect->m_w*m_coeff));
			FireChangeParameter(9, (int)((float)m_rect->m_h*m_coeff));

			SetInvalidateFlag(INVALID_GRAPHICS);
		}
	}

	typedef enum
	{
		IDF_NONE,
		IDF_CENTER,
		IDF_SCALE
	}  INDRAW_FLAG;

	INDRAW_FLAG   m_indraw_flag;

	virtual void OnChangeParameter(int paramInd) 
	{
		
		if (paramInd == 6)
		{
			m_page_W = m_params[6]->m_value.lVal;
			m_current_sizes[0] = 0;
			if (m_params[3]->m_value.lVal)
			{
				m_indraw_flag = IDF_CENTER;
			}
			if (m_params[10]->m_value.lVal)
			{
				m_indraw_flag = IDF_SCALE;
			}
			SetInvalidateFlag(INVALID_GRAPHICS);
		}
		if (paramInd == 7)
		{
			m_page_H = m_params[7]->m_value.lVal;
			m_current_sizes[0] = 0;
			if (m_params[3]->m_value.lVal)
			{
				m_indraw_flag = IDF_CENTER;
			}
			if (m_params[10]->m_value.lVal)
			{
				m_indraw_flag = IDF_SCALE;
			}
			SetInvalidateFlag(INVALID_GRAPHICS);
		}
		if (paramInd == 4 && m_rect)
		{
			m_rect->m_x = (int)(m_params[4]->m_value.lVal/m_coeff);
			SetInvalidateFlag(INVALID_GRAPHICS);
		}
		if (paramInd == 5 && m_rect)
		{
			m_rect->m_y = (int)(m_params[5]->m_value.lVal/m_coeff);
			SetInvalidateFlag(INVALID_GRAPHICS);
		}
		if (paramInd == 8 && m_rect)
		{
			m_rect->m_w = (int)(m_params[8]->m_value.lVal/m_coeff);
			SetInvalidateFlag(INVALID_GRAPHICS);
		}
		if (paramInd == 9 && m_rect)
		{
			m_rect->m_h = (int)(m_params[9]->m_value.lVal/m_coeff);
			SetInvalidateFlag(INVALID_GRAPHICS);
		}
		if (paramInd == 3 && m_rect)
		{
			if (m_params[3]->m_value.lVal)
			{
				m_indraw_flag = IDF_CENTER;
				SetInvalidateFlag(INVALID_GRAPHICS);

				//FireChangeParameter(4, (int)((float)m_rect->m_x*m_coeff));
				//FireChangeParameter(5, (int)((float)m_rect->m_y*m_coeff));
			}
		}
		if (paramInd == 10 && m_rect)
		{
			if (m_params[10]->m_value.lVal)
			{
				m_indraw_flag = IDF_SCALE;
				SetInvalidateFlag(INVALID_GRAPHICS);

				/*FireChangeParameter(4, (int)((float)m_rect->m_x*m_coeff));
				FireChangeParameter(5, (int)((float)m_rect->m_y*m_coeff));

				FireChangeParameter(8, (int)((float)m_rect->m_w*m_coeff));
				FireChangeParameter(9, (int)((float)m_rect->m_h*m_coeff));*/
			}
		}
		if (paramInd ==2)
		{
			HDC pHdc = (HDC)((INT_PTR)m_params[2]->m_value.lVal);

			LONG nLOGPIXELSX = GetDeviceCaps(pHdc, HORZRES);
			LONG nLOGPIXELSY = GetDeviceCaps(pHdc, VERTRES);

			double dLeft	= (double)m_rect->m_x * nLOGPIXELSX / m_rect->GetContainerWidth();
			double dTop		= (double)m_rect->m_y * nLOGPIXELSY / m_rect->GetContainerHeight();
			double dWidth	= (double)m_rect->m_w * nLOGPIXELSX / m_rect->GetContainerWidth();
			double dHeight	= (double)m_rect->m_h * nLOGPIXELSY / m_rect->GetContainerHeight();

			Graphics oGraphics(pHdc);

			oGraphics.SetPageUnit(UnitPixel);

			Gdiplus::RectF sourceRct, screenRct;

			if (m_rect->GetIntersection(/*Gdiplus::RectF((REAL)dLeft, (REAL)dTop, (REAL)dWidth, (REAL)dHeight)*/m_page_rect,
										Gdiplus::RectF(0, 0, (float)m_pImage->GetWidth(), (float)m_pImage->GetHeight()), screenRct, sourceRct))
			{
				/*screenRct.X *= (float)nLOGPIXELSX / (float)m_rect->GetContainerWidth();
				screenRct.Width *= (float)nLOGPIXELSX / (float)m_rect->GetContainerWidth();
				screenRct.Y *= (float)nLOGPIXELSY / (float)m_rect->GetContainerHeight();
				screenRct.Height *= (float)nLOGPIXELSY / (float)m_rect->GetContainerHeight();

				sourceRct.X *= nLOGPIXELSX / m_rect->GetContainerWidth();
				sourceRct.Width *= nLOGPIXELSX / m_rect->GetContainerWidth();
				sourceRct.Y *= nLOGPIXELSY / m_rect->GetContainerHeight();
				sourceRct.Height *= nLOGPIXELSY / m_rect->GetContainerHeight();*/

				/*oGraphics.DrawImage(m_pImage, screenRct, sourceRct.GetLeft(), sourceRct.GetTop(),
								sourceRct.Width, sourceRct.Height, UnitPixel);*/

				if (dLeft<0)            {dWidth+=dLeft; dLeft=0;}
				if (dTop<0)             {dHeight+=dTop; dTop=0;}
				if (dWidth>nLOGPIXELSX) {dWidth=nLOGPIXELSX;}
				if (dHeight>nLOGPIXELSY){dHeight=nLOGPIXELSY; }

				oGraphics.DrawImage(m_pImage, 
					Gdiplus::RectF((REAL)dLeft, (REAL)dTop, (REAL)dWidth, (REAL)dHeight), 
					sourceRct.GetLeft(), sourceRct.GetTop(), sourceRct.Width, sourceRct.Height, UnitPixel);
			}

			//oGraphics.DrawImage(m_pImage, (REAL)dLeft, (REAL)dTop, (REAL)dWidth, (REAL)dHeight);
		}

		//SetInvalidateFlag(INVALID_LAYER);
	}

	virtual void ChangesHandler(int changingObject, bool repaint)
	{
		if (changingObject==ILayer::FAST_COPY || changingObject==ILayer::SLOW_COPY)
		{
			SetInvalidateFlag(INVALID_GRAPHICS);
		}
	}

	Gdiplus::RectF    CalcPageRect(int viewW, int viewH)
	{
		if (m_page_H==0 || m_page_W==0)
			return Gdiplus::RectF(0,0,0,0);

			float tmpW = 0.0f;
			float tmpH = 0.0f;

			float scCoef = (float)m_page_H/(float)m_page_W;

			tmpW = 100;
			tmpH = tmpW*scCoef;

			float coeff = min((viewW-INDENT)/tmpW, (viewH-INDENT)/tmpH);

			tmpW *= coeff;
			tmpH *= coeff;
		
			m_zoom	= min(tmpW/m_page_W, tmpH/m_page_H);

			Gdiplus::RectF result(viewW/2-tmpW/2, viewH/2-tmpH/2, tmpW, tmpH);

			return result;
	}

	virtual void DrawOnGraphics(Gdiplus::Graphics* pGr, const CRect& rect)
	{
		Gdiplus::SolidBrush		white_brush ( Gdiplus::Color::White );
		Gdiplus::SolidBrush     bckg_brash(GetAppManager()->GetView()->GetBackgroundColor());
		
		pGr->FillRectangle ( &bckg_brash, GetAppManager()->GetView()->GetWindowRect() );

		if (m_page_H==0 || m_page_W==0)
			return;

		
		int   scrW = (int)GetAppManager()->GetView()->GetWindowRect().Width;
		int   scrH = (int)GetAppManager()->GetView()->GetWindowRect().Height;

		if (scrW!=m_current_sizes[0] || scrH!=m_current_sizes[1])
		{
			m_page_rect = CalcPageRect(scrW, scrH);
			m_current_sizes[0] = scrW;
			m_current_sizes[1] = scrH;
			if (m_rect)
			{
				m_rect->RecalcSizes((int)m_page_rect.Width, (int)m_page_rect.Height);
			}
		}

		if (m_page_rect.Width>0.00001)
			m_coeff = m_page_W/m_page_rect.Width;
		
		pGr->FillRectangle ( &white_brush, m_page_rect );

		if (m_indraw_flag!=IDF_NONE)
		{
			if (m_indraw_flag==IDF_CENTER)
			{
				m_rect->m_x = ((int)m_page_rect.Width-m_rect->m_w)/2;
				m_rect->m_y = ((int)m_page_rect.Height-m_rect->m_h)/2;
				FireChangeParameter(4, (int)((float)m_rect->m_x*m_coeff));
				FireChangeParameter(5, (int)((float)m_rect->m_y*m_coeff));
			}
			else
				if (m_indraw_flag==IDF_SCALE)
				{
					/*m_rect->m_x = 10;
					m_rect->m_w = m_page_rect.Width-20;
					m_rect->m_y = 10;
					m_rect->m_h = m_page_rect.Height-20;*/

					float scCoef = (float)m_pImage->GetHeight()/(float)m_pImage->GetWidth();

					float tmpW = (float)m_pImage->GetWidth();
					float tmpH = tmpW*scCoef;

					float coeff = min((m_page_rect.Width-SHFT)/tmpW, (m_page_rect.Height-SHFT)/tmpH);

					tmpW *= coeff;
					tmpH *= coeff;

					m_rect->m_x = (int)(m_page_rect.Width/2-tmpW/2);
					m_rect->m_y = (int)(m_page_rect.Height/2-tmpH/2); 
					m_rect->m_w = (int)(tmpW);
					m_rect->m_h = (int)(tmpH);

					FireChangeParameter(4, (int)((float)m_rect->m_x*m_coeff));
					FireChangeParameter(5, (int)((float)m_rect->m_y*m_coeff));
					FireChangeParameter(8, (int)((float)m_rect->m_w*m_coeff));
					FireChangeParameter(9, (int)((float)m_rect->m_h*m_coeff));
				}
			m_indraw_flag = IDF_NONE;
		}

		if (m_rect)
		{
			Gdiplus::RectF sourceRct, screenRct;
			bool bIntersect = m_rect->GetIntersection(m_page_rect, Gdiplus::RectF(0, 0, (float)m_pImage->GetWidth(), (float)m_pImage->GetHeight()), screenRct, sourceRct);
			if( bIntersect )
			{
				pGr->DrawImage(m_pImage, screenRct, sourceRct.GetLeft(), sourceRct.GetTop(), sourceRct.Width, sourceRct.Height, UnitPixel);
			}
			
			m_rect->Draw(pGr);
		}
		
		/*Gdiplus::SolidBrush blackBrush(Gdiplus::Color(155, 150, 150));
		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);
		Gdiplus::Font myFont2(L"Arial", 48 ,Gdiplus::FontStyleBold);
		Gdiplus::SolidBrush blackBrush2(Gdiplus::Color(255, 61, 61, 61));
		
		pGr->DrawString(L"Тут типа команда Print",22,	&myFont2,GetAppManager()->GetView()->GetWindowRect(), &format, &blackBrush);*/
	}

	virtual bool DrawOnLayer()
	{

		return true;
	}

/*ICoordinatSystem methods*/
	void ScreenPointToImagePoint			( long& X, long& Y )
	{
		X = long(X - m_page_rect.GetLeft());
		Y = long(Y - m_page_rect.GetTop());
	}
	void ImagePointToScreenPoint			( long& X, long& Y )
	{
		X = long(X + m_page_rect.GetLeft());
		Y = long(Y + m_page_rect.GetTop());
	}
	void ScreenDistanceToImageDistance		( long& Distance )
	{
	}
	void ImageDistanceToScreenDistance		( long& Distance )
	{
	}


	Gdiplus::RectF	GetPageRect ()	{	return	m_page_rect;}
	double			GetZoom		()	{	return	m_zoom;		}
	long			GetPageW	()	{	return	m_page_W;	}
	long			GetPageH	()	{	return	m_page_H;	}
	long			GetIndent	()	{	return	INDENT;		}

private:
	Bitmap* m_pImage;

	TrackRect*   m_rect;

	int m_current_sizes[2];

	Gdiplus::RectF  m_page_rect;

	long			m_page_W;
	long			m_page_H;

	double			m_zoom;

	float           m_coeff;

};

#endif