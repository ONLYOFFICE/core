#ifndef __ERASER_COMM_
#define __ERASER_COMM_

#include "..//CommandManager.h"
/************************************************************************/
/* ERASER COMMAND   -----   ID = 101                                    */
/************************************************************************/

class CEraserCommand : public CCommand
{
public:
	CEraserCommand(IAppManager* app_manager):CCommand(app_manager)
		,m_size(30),m_down(false), m_tmp_buffer(NULL)
	{
		AddParameter(_T("Size"), _T("Eraser Size"), m_size, 10, 300);
		AllocTmpBuffer(m_size);
	}
	virtual ~CEraserCommand() 
	{
		ClearTmpBuffer();
	}
public:

	virtual void OnMouseDown(LONG nX, LONG nY, LONG nButton)	
	{
		m_down = true;
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StartHistoryBlock();
	}
	virtual void OnMouseUp(LONG nX, LONG nY, LONG nButton)	
	{
		m_down = false;
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StopHistoryBlock(_T("Eraser Command"));
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangeImage(ILayer::SLOW);
	}
	virtual void OnMouseMove(LONG nX, LONG nY, LONG nButton)	
	{
		m_mousePos[0] = nX;
		m_mousePos[1] = nY;
		if (m_down)
		{
			GetAppManager()->GetCoordinateTransformer()->ScreenPointToImagePoint(nX,nY);
			m_mousePosOnImage[0] = nX;
			m_mousePosOnImage[1] = nY;
		}
		SetInvalidateFlag(INVALID_GRAPHICS);
		DrawOnLayer();
	}
	
	virtual void OnChangeParameter(int paramInd) 
	{
		if (paramInd==0 && m_params.GetCount()>0)
		{
			m_size = m_params[0]->m_value.lVal;
			AllocTmpBuffer(m_size);
			SetInvalidateFlag(INVALID_GRAPHICS);
		}
	}

	virtual void DrawOnGraphics(Gdiplus::Graphics* pGr, const CRect& rect)
	{
		long realSz = m_size;
		GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(realSz);

		Gdiplus::SmoothingMode  savingSM  = pGr->GetSmoothingMode();
		pGr->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		Gdiplus::Pen  pn( Gdiplus::Color::Green, 1.0f );

		pGr->DrawEllipse(&pn, m_mousePos[0]-realSz, m_mousePos[1]-realSz, 2*realSz , 2*realSz);

		pGr->SetSmoothingMode(savingSM);
	}

	virtual bool DrawOnLayer()	
	{	
		if(!m_down)
			return false;
		long wdth, hght;
		const BYTE*  imData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, wdth,hght);

		if (!m_tmp_buffer)
			AllocTmpBuffer(m_size);

		if (imData)
		{
			long pX = m_mousePosOnImage[0];
			long pY = m_mousePosOnImage[1];

			for (int i = pX-m_size, bufI = 0; i<pX+m_size; i++, bufI++)
				for (int j = pY-m_size, bufJ=0; j<pY+m_size; j++, bufJ++)
				{
					int imgX = max(0, min(wdth-1,i));
					int imgY = max(0, min(hght-1,j));

					int  nIndex = 4*(imgX + imgY*wdth);
					int  bufIndex = 4*(bufI + bufJ*2*m_size);

					float transpCoef = (GetDistanceBeetweenIndexes(i,j,pX,pY))/((float)m_size);

					memcpy(m_tmp_buffer+bufIndex, imData+nIndex, 4);

					if (transpCoef<1.0f)
					{
						BYTE newTr = (BYTE)(transpCoef*255.0f);
						if (newTr<m_tmp_buffer[bufIndex+3])
							m_tmp_buffer[bufIndex+3] = newTr;
					}
				}
			GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangePartOfLayer(m_tmp_buffer, 2*m_size, 2*m_size, pX-m_size, pY-m_size);
		}

		return true;
	}


private:
	LONG  m_mousePos[2];
	LONG  m_mousePosOnImage[2];
	int   m_size;
	bool  m_down;

	BYTE*   m_tmp_buffer;

	void  ClearTmpBuffer()
	{
		if (m_tmp_buffer)
			delete[]  m_tmp_buffer;
		m_tmp_buffer = NULL;
	}

	void  AllocTmpBuffer(int sz)
	{
		ClearTmpBuffer();
		m_size = sz;
		int bufSz = 4*(2*m_size)*(2*m_size);
		m_tmp_buffer = new BYTE[bufSz];
	}

	float  GetDistanceBeetweenIndexes(long ind1_X, long ind1_Y, long ind2_X, long ind2_Y)
	{
		return sqrt((float)((ind2_X-ind1_X)*(ind2_X-ind1_X)+(ind2_Y-ind1_Y)*(ind2_Y-ind1_Y)));
	}
};

#endif