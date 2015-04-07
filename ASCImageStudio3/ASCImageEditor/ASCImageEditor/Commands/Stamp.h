#ifndef __STAMP_COMM_
#define __STAMP_COMM_

#include "..//CommandManager.h"
/************************************************************************/
/* STAPM COMMAND   -----   ID = 100                                       */
/************************************************************************/

class CStampCommand : public CCommand
{
public:
	CStampCommand(IAppManager* app_manager):CCommand(app_manager)
		,m_size(30),m_type(0),m_soft(30),m_down(false), m_buf_image_data(NULL), m_buf_image(NULL),m_buff_without_back_command(NULL),m_buf_without_back_command_image(NULL)
	{
		DEV_LOG("Stamp start constructor");
		AddParameter(_T("Size"), _T("Stamp Size"), m_size, 10, 100);
		AddParameter(_T("Type"), _T("Type of Stamp"), m_type, 0, 2);
		AddParameter(_T("Soft"), _T("Soft"), m_soft, 0, 100);
		m_changes_after_finish = false;
		DEV_LOG("Stamp end constructor");
		DEV_LOG("---Stamp Command Begin");	
		m_mouseUp[0] = -10000;
		m_mouseUp[1] = -10000;
		m_mouseDown[0] = -10000;
		m_mouseDown[1] = -10000;
		m_redraw_after_mouse_up = false;
	}
	virtual ~CStampCommand() 
	{
		DEV_LOG("---Stamp Command End");
		if (m_buf_image_data)
			delete[] m_buf_image_data;
		if (m_buf_image)
			delete m_buf_image;
		if (m_buff_without_back_command)
			delete[] m_buff_without_back_command;
		if (m_buf_without_back_command_image)
			delete m_buf_without_back_command_image;
	}
public:

	virtual void OnMouseDown(LONG nX, LONG nY, LONG nButton)	
	{
		m_down = true;
		m_mouseDown[0] = nX;
		m_mouseDown[1] = nY;
		GetAppManager()->GetCoordinateTransformer()->ScreenPointToImagePoint(m_mouseDown[0],m_mouseDown[1]);
		m_mouseUp[0] = -10000;
		m_mouseUp[1] = -10000;

		if (m_buf_image_data)
			delete[] m_buf_image_data;
		if (m_buf_image)
			delete m_buf_image;
		if (m_buff_without_back_command)
			delete[] m_buff_without_back_command;
		if (m_buf_without_back_command_image)
			delete m_buf_without_back_command_image;
		m_buf_image_data = NULL;
		m_buf_image = NULL;
		m_buff_without_back_command = NULL;
		m_buf_without_back_command_image = NULL;

		long lWidth = 0; long lHeight = 0;
		const BYTE* pData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, lWidth, lHeight);

		long buffer_size = 2*m_size;

		m_buf_image_data = new BYTE[4*buffer_size*buffer_size];
		m_buff_without_back_command = new BYTE[4*buffer_size*buffer_size];

		double dKoef = 0.75; // вес, определяет притяжение параболы к прямой
		double dKoef2 = 0.4;

		double dVal = (m_soft >= 50) ? dKoef2 : (1 - dKoef2);
		dVal *= m_size;

		double dY = dVal + dKoef * m_size * (m_soft - 50.0) / 50.0;

		double dA = dY / (dVal * dVal - dVal * m_size);
		double dB = 1 - dA * m_size;
		
		for (int i = m_mouseDown[0]-m_size, buf_i=0; i<m_mouseDown[0]+m_size; i++, buf_i++)
			for (int j = m_mouseDown[1]-m_size, buf_j=0; j<m_mouseDown[1]+m_size; j++, buf_j++)
			{
				int  bufIndex = 4*(buf_i + buf_j*buffer_size);

				if (i<0 || i>(lWidth-1) ||	j<0 || j>(lHeight-1))
				{
					m_buf_image_data[bufIndex+0] = 0;
					m_buf_image_data[bufIndex+1] = 0;
					m_buf_image_data[bufIndex+2] = 0;
					m_buf_image_data[bufIndex+3] = 0;
					continue;
				}

				int imgX = max(0, min(lWidth-1,i));
				int imgY = max(0, min(lHeight-1,j));

				int  nIndex = 4*(imgX + imgY*lWidth);
				

				float transpCoef = GetDistanceBeetweenIndexes(i,j,m_mouseDown[0],m_mouseDown[1]);

				if (m_type!=1)
				{
					if (transpCoef>(float)m_size)
					{
						m_buf_image_data[bufIndex+3] = 0;
					}
					else
					{
						m_buf_image_data[bufIndex+0] = pData[nIndex+0];
						m_buf_image_data[bufIndex+1] = pData[nIndex+1];
						m_buf_image_data[bufIndex+2] = pData[nIndex+2];

						transpCoef = m_size - transpCoef;
						double dMem = 256 * (dA * transpCoef * transpCoef + dB * transpCoef) / m_size; 
						dMem = min(dMem,pData[nIndex+3] );
						m_buf_image_data[bufIndex+3] = (BYTE)max(0, min(255, dMem));
					}
				}
				else
				{
					m_buf_image_data[bufIndex+0] = pData[nIndex+0];
					m_buf_image_data[bufIndex+1] = pData[nIndex+1];
					m_buf_image_data[bufIndex+2] = pData[nIndex+2];
					m_buf_image_data[bufIndex+3] = pData[nIndex+3];
				}
			}

		memcpy(m_buff_without_back_command, m_buf_image_data, 4*buffer_size*buffer_size);
		GetAppManager()->ApplyBackgroundCommands(m_buf_image_data, buffer_size, buffer_size, false);

		m_buf_image = new Bitmap(buffer_size, buffer_size, 4*buffer_size, PixelFormat32bppARGB, m_buf_image_data);
		m_buf_without_back_command_image = new Bitmap(buffer_size, buffer_size, 4*buffer_size, PixelFormat32bppARGB, m_buff_without_back_command);
		
		SetInvalidateFlag(INVALID_GRAPHICS,false);
	}
	virtual void OnMouseUp(LONG nX, LONG nY, LONG nButton)	
	{
		m_down = false;
		m_mouseUp[0] = nX;
		m_mouseUp[1] = nY;
		GetAppManager()->GetCoordinateTransformer()->ScreenPointToImagePoint(m_mouseUp[0],m_mouseUp[1]);
		if (m_buf_image && m_buf_image_data)
		{
			long lWidth = 0; long lHeight = 0;
			const BYTE* pData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, lWidth, lHeight);

			long buffer_size = 2*m_size;
			BYTE*    tmp_buf_image_data = new BYTE[4*buffer_size*buffer_size];

			GetAppManager()->GetCoordinateTransformer()->ScreenPointToImagePoint(nX,nY);

			for (int i = nX-m_size, buf_i=0; i<nX+m_size; i++, buf_i++)
				for (int j = nY-m_size, buf_j=0; j<nY+m_size; j++, buf_j++)
				{
					int  bufIndex = 4*(buf_i + buf_j*buffer_size);

					if (i<0 || i>(lWidth-1) ||	j<0 || j>(lHeight-1))
					{
						tmp_buf_image_data[bufIndex+0] = 0;
						tmp_buf_image_data[bufIndex+1] = 0;
						tmp_buf_image_data[bufIndex+2] = 0;
						tmp_buf_image_data[bufIndex+3] = 0;
						continue;
					}

					int imgX = max(0, min(lWidth-1,i));
					int imgY = max(0, min(lHeight-1,j));

					int  nIndex = 4*(imgX + imgY*lWidth);
					
					tmp_buf_image_data[bufIndex+0] = pData[nIndex+0];
					tmp_buf_image_data[bufIndex+1] = pData[nIndex+1];
					tmp_buf_image_data[bufIndex+2] = pData[nIndex+2];
					tmp_buf_image_data[bufIndex+3] = pData[nIndex+3];
				}

			BYTE*    tmp_buf_image_data_2 = new BYTE[4*buffer_size*buffer_size];
			memcpy(tmp_buf_image_data_2, tmp_buf_image_data, 4*buffer_size*buffer_size);

			Gdiplus::Bitmap*  tmp_buf_image = new Bitmap(buffer_size, buffer_size, 4*buffer_size, PixelFormat32bppARGB, tmp_buf_image_data);
			Gdiplus::Bitmap*  tmp_buf_image_2 = new Bitmap(buffer_size, buffer_size, 4*buffer_size, PixelFormat32bppARGB, tmp_buf_image_data_2);
			Gdiplus::Graphics tmpGr(tmp_buf_image);
			tmpGr.DrawImage(m_buf_image,0,0);
			Gdiplus::Graphics tmpGr2(tmp_buf_image_2);
			tmpGr2.DrawImage(m_buf_without_back_command_image,0,0);

			delete[] m_buf_image_data;
			delete m_buf_image;
			delete[] m_buff_without_back_command;
			delete m_buf_without_back_command_image;

			m_buf_image = tmp_buf_image;
			m_buf_without_back_command_image = tmp_buf_image_2;
			m_buf_image_data = tmp_buf_image_data;
			m_buff_without_back_command = tmp_buf_image_data_2;
		}
		SetInvalidateFlag(INVALID_GRAPHICS,false);
		DrawOnLayer();
		m_redraw_after_mouse_up = true;
	}
	virtual void OnMouseMove(LONG nX, LONG nY, LONG nButton)	
	{
		m_mousePos[0] = nX;
		m_mousePos[1] = nY;
		
		SetInvalidateFlag(INVALID_GRAPHICS,m_redraw_after_mouse_up);
	}

	virtual void OnMouseLeave()
	{
		m_down = false;
		m_mouseUp[0] = -10000;
		m_mouseUp[1] = -10000;
		if (m_buf_image_data)
			delete[] m_buf_image_data;
		if (m_buf_image)
			delete m_buf_image;
		if (m_buff_without_back_command)
			delete[] m_buff_without_back_command;
		if (m_buf_without_back_command_image)
			delete m_buf_without_back_command_image;
		m_buf_image_data = NULL;
		m_buf_image = NULL;
		m_buff_without_back_command = NULL;
		m_buf_without_back_command_image = NULL;
	}
	
	virtual void OnChangeParameter(int paramInd) 
	{
		if (paramInd==0 && m_params.GetCount()>0)
		{
			m_size = m_params[0]->m_value.lVal;
			SetInvalidateFlag(INVALID_GRAPHICS,false);
		}
		if (paramInd==1 && m_params.GetCount()>0)
		{
			m_type = m_params[1]->m_value.lVal;
			if (m_type>2)
				m_type = 0;
			SetInvalidateFlag(INVALID_GRAPHICS,false);
		}
		if (paramInd==2)
		{
			m_soft = m_params[2]->m_value.lVal;
		}
	}

	virtual void DrawOnGraphics(Gdiplus::Graphics* pGr, const CRect& rect)
	{
		long realSz = m_size;
		GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(realSz);

		Gdiplus::SmoothingMode  savingSM  = pGr->GetSmoothingMode();
		pGr->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		Gdiplus::Pen  pn( Gdiplus::Color::White, 1.0f );
		Gdiplus::Pen  pnBlack( Gdiplus::Color::Black, 1.0f );
		long mDX = m_mouseDown[0];
		long mDY = m_mouseDown[1];
		long mPX = m_mousePos[0];
		long mPY = m_mousePos[1];
		if (m_down)
			GetAppManager()->GetCoordinateTransformer()->ImagePointToScreenPoint(mDX,mDY);

		long  VecLen = (long)GetDistanceBeetweenIndexes(mPX, mPY, mDX, mDY);
		if (m_type==0)
		{
			if (m_down && VecLen>1)
			{
				pGr->DrawEllipse(&pn, mDX-realSz, mDY-realSz, 2*realSz , 2*realSz);
				pGr->DrawEllipse(&pnBlack, mDX-realSz+1, mDY-realSz+1, 2*realSz-2 , 2*realSz-2);
				/*Gdiplus::AdjustableArrowCap cap(4, 6, false);
				pn.SetCustomEndCap(&cap);
				pGr->DrawLine(&pn, mDX+(mPX-mDX)*realSz/VecLen, mDY+(mPY-mDY)*realSz/VecLen, 
					mPX-(mPX-mDX)*realSz/VecLen, mPY-(mPY-mDY)*realSz/VecLen);*/
			}
			else
			{
				pGr->DrawEllipse(&pn, mPX-realSz, mPY-realSz, 2*realSz , 2*realSz);
				pGr->DrawEllipse(&pnBlack, mPX-realSz+1, mPY-realSz+1, 2*realSz-2 , 2*realSz-2);
			}
		}
		else if (m_type==1)
		{
			if (m_down && VecLen>1)
			{
				pGr->DrawRectangle(&pn, mDX-realSz, mDY-realSz, 2*realSz , 2*realSz);
				pGr->DrawRectangle(&pnBlack, mDX-realSz+1, mDY-realSz+1, 2*realSz-2 , 2*realSz-2);
				/*Gdiplus::AdjustableArrowCap cap(4, 6, false);
				pn.SetCustomEndCap(&cap);
				pGr->DrawLine(&pn, mDX+(mPX-mDX)*realSz/VecLen, mDY+(mPY-mDY)*realSz/VecLen, 
					mPX-(mPX-mDX)*realSz/VecLen, mPY-(mPY-mDY)*realSz/VecLen);*/
			}
			else
			{
				pGr->DrawRectangle(&pn, mPX-realSz, mPY-realSz, 2*realSz , 2*realSz);
				pGr->DrawRectangle(&pnBlack, mPX-realSz+1, mPY-realSz+1, 2*realSz-2 , 2*realSz-2);
			}
		}
		else if (m_type==2)
		{
			if (m_down && VecLen>1)
			{
				pGr->DrawEllipse(&pn, mDX-2*realSz, mDY-realSz, 4*realSz , 2*realSz);
				pGr->DrawEllipse(&pnBlack, mDX-2*realSz+1, mDY-realSz+1, 4*realSz-2 , 2*realSz-2);
				/*Gdiplus::AdjustableArrowCap cap(4, 6, false);
				pn.SetCustomEndCap(&cap);
				pGr->DrawLine(&pn, mDX+(mPX-mDX)*realSz/VecLen, mDY+(mPY-mDY)*realSz/VecLen, 
					mPX-(mPX-mDX)*realSz/VecLen, mPY-(mPY-mDY)*realSz/VecLen);*/
			}
			else
			{
				pGr->DrawEllipse(&pn, mPX-2*realSz, mPY-realSz, 4*realSz , 2*realSz);
				pGr->DrawEllipse(&pnBlack, mPX-2*realSz+1, mPY-realSz+1, 4*realSz-2 , 2*realSz-2);
			}
		}

		if (m_down && m_buf_image)
			pGr->DrawImage(m_buf_image,mPX-realSz, mPY-realSz, 2*realSz , 2*realSz);

		pGr->SetSmoothingMode(savingSM);

		m_redraw_after_mouse_up = false;
	}

	virtual bool DrawOnLayer()	
	{	
		long wdth, hght;
		const BYTE* imData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, wdth,hght);
		
		if (m_mouseUp[0]>-10000 && m_mouseUp[1]>-10000 &&m_buf_image && m_buf_image_data && m_buff_without_back_command)
		{
			long pY = m_mouseUp[1];
			long pX = m_mouseUp[0];

			long buffer_size = 2*m_size;

			GetAppManager()->GetLayersManager()->GetCurrentLayer()->StartHistoryBlock();
			GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangePartOfLayer(m_buff_without_back_command, buffer_size, buffer_size, pX-m_size, pY-m_size);

			CString tmpStr;
			tmpStr.Format(_T("Stamp: From-(%i, %i); Size-%i"),pX, pY, m_size);
			GetAppManager()->GetLayersManager()->GetCurrentLayer()->StopHistoryBlock(tmpStr);

			if (GetAppManager()->IsBackgroundCommandOnFullImage())
				GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangeImage(ILayer::SLOW,NULL,true);
			else
				GetAppManager()->GetLayersManager()->GetCurrentLayer()->InvalidateImagePart(pX-m_size, pY-m_size,pX+m_size, pY+m_size);
			RECT invR;
			invR.left = pX-m_size;
			invR.top = pY-m_size;
			GetAppManager()->GetCoordinateTransformer()->ImagePointToScreenPoint(invR.left,invR.top);
			invR.right = pX+m_size;
			invR.bottom = pY+m_size;
			GetAppManager()->GetCoordinateTransformer()->ImagePointToScreenPoint(invR.right,invR.bottom);
			GetAppManager()->GetView()->InvalidateView(true, &invR);
		}

		if (m_buf_image_data)
			delete[] m_buf_image_data;
		if (m_buf_image)
			delete m_buf_image;
		if (m_buff_without_back_command)
			delete[] m_buff_without_back_command;
		if (m_buf_without_back_command_image)
			delete m_buf_without_back_command_image;
		m_buf_image_data = NULL;
		m_buf_image = NULL;
		m_buff_without_back_command = NULL;
		m_buf_without_back_command_image = NULL;
		m_mouseUp[0] = -10000;
		m_mouseUp[1] = -10000;
		m_mouseDown[0] = -10000;
		m_mouseDown[1] = -10000;

		return true;
	}


private:
	LONG  m_mousePos[2];
	LONG  m_mouseDown[2];
	LONG  m_mouseUp[2];
	int   m_size;
	int   m_type;
	int   m_soft;
	bool  m_down;

	bool  m_redraw_after_mouse_up;

	BYTE*              m_buf_image_data;
	BYTE*              m_buff_without_back_command;
	Gdiplus::Bitmap*   m_buf_image;
	Gdiplus::Bitmap*   m_buf_without_back_command_image;

	float  GetDistanceBeetweenIndexes(long ind1_X, long ind1_Y, long ind2_X, long ind2_Y)
	{
		return sqrt((float)((ind2_X-ind1_X)*(ind2_X-ind1_X)+(ind2_Y-ind1_Y)*(ind2_Y-ind1_Y)));
	}
};

#endif