#ifndef __REDEYE_COMM_
#define __REDEYE_COMM_

#include "..//CommandManager.h"

/************************************************************************/
/* REDEYE COMMAND   -----   ID = 105                                    */
/************************************************************************/
#include <ImageTransformsCoreBGRA.h>

class CRedEyeCommand : public CCommand
{
public:
	CRedEyeCommand(IAppManager* app_manager):CCommand(app_manager)
		,m_size(30)
	{
		AddParameter(_T("Size"), _T("Size"), m_size, 10, 200);
		m_changes_after_finish = false;
	}
	virtual ~CRedEyeCommand() {}
public:

	virtual void OnMouseDown(LONG nX, LONG nY, LONG nButton)	
	{
		DrawOnLayer();
	}
	virtual void OnMouseUp(LONG nX, LONG nY, LONG nButton)	
	{
	}
	virtual void OnMouseMove(LONG nX, LONG nY, LONG nButton)	
	{
		m_mousePos[0] = nX;
		m_mousePos[1] = nY;
		SetInvalidateFlag(INVALID_GRAPHICS);
	}
	
	virtual void OnChangeParameter(int paramInd) 
	{
		if (paramInd==0 && m_params.GetCount()>0)
		{
			m_size = m_params[0]->m_value.lVal;
			SetInvalidateFlag(INVALID_GRAPHICS);
		}
	}

	virtual void DrawOnGraphics(Gdiplus::Graphics* pGr, const CRect& rect)
	{
		long realSz = m_size;
		GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(realSz);

		Gdiplus::SmoothingMode  savingSM  = pGr->GetSmoothingMode();
		pGr->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		Gdiplus::Pen  pn( Gdiplus::Color::White, 1.0f );

		pGr->DrawEllipse(&pn, m_mousePos[0]-realSz, m_mousePos[1]-realSz, 2*realSz , 2*realSz);

		Gdiplus::Pen  pnBlack( Gdiplus::Color::Black, 1.0f );
		pGr->DrawEllipse(&pnBlack, m_mousePos[0]-realSz+1, m_mousePos[1]-realSz+1, 2*realSz-2 , 2*realSz-2);

		pGr->SetSmoothingMode(savingSM);
	}

	virtual bool DrawOnLayer()	
	{	
		long lWidth, lHeight;
		const BYTE*  imData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, lWidth,lHeight);

		if (!imData)
			return false;

		
		long pX = m_mousePos[0];
		long pY = m_mousePos[1];

		GetAppManager()->GetCoordinateTransformer()->ScreenPointToImagePoint(pX, pY);

		long buffer_size = 2*m_size;

		int Amount1 = 0; // [-30,30] Threshold
		int Amount2 = 0; // removed Hue Range control from preliminary version
		// Fine-tune input parameters, we may drop this block in the future by writing the "good" values directly into the formulas
		int Amount1x = Amount1;
		Amount1x += -10;
		int Amount2x = -Amount2;
		Amount2x += 2;

		BYTE* tmpBuf = new BYTE[4*buffer_size*buffer_size];

		for (int i = pX-m_size, buf_i=0; i<pX+m_size; i++, buf_i++)
			for (int j = pY-m_size, buf_j=0; j<pY+m_size; j++, buf_j++)
			{
				int imgX = max(0, min(lWidth-1,i));
				int imgY = max(0, min(lHeight-1,j));

				float transpCoef = GetDistanceBeetweenIndexes(imgX,imgY,pX,pY);

				int  nIndex = 4*(imgX + imgY*lWidth);
				int  bufIndex = 4*(buf_i + buf_j*buffer_size);

				if (transpCoef>(float)m_size)					
				{
					tmpBuf[bufIndex+2] = imData[nIndex+2];
					tmpBuf[bufIndex+1] = imData[nIndex+1];
					tmpBuf[bufIndex+0] = imData[nIndex+0];
					tmpBuf[bufIndex+3] = imData[nIndex+3];
					continue;
				}

				BYTE r = imData[nIndex+2];
				BYTE g = imData[nIndex+1];
				BYTE b = imData[nIndex+0];
				double ai = (double)imData[nIndex+3];
				
				int hue , sat, lig;
				ConvertToHSV(r,g,b, hue, sat, lig);

				hue += Amount2x;
				if(hue > 360)
					hue -= 360;
				if(hue < 0)
					hue += 360;

				double axh = 1.0;
				if(hue > 259 && hue < 270)
					axh = ((double)hue - 259.0) / 10.0;

				double axs = 1.0;
				if (hue > 259)
				{
					if (sat < Amount1x + 40)
						axs = 0.0;
					if (sat > Amount1x + 39 && sat < Amount1x + 45)
						axs = ((double)sat - ((double)Amount1x + 39.0)) / 5.0;
				}                                       

				if (hue < 260)
				{
					if (sat < hue * 2 + Amount1x + 40)
						axs = 0.0;
					if (sat > hue * 2 + Amount1x + 39 && sat < hue * 2 + Amount1x + 50)
						axs = ((double)sat - ((double)hue * 2.0 + (double)Amount1x + 39.0)) / 10.0;
				}

				axh *= axs;           

				// Calculate final alpha based on original and multiplier:
				ai *= axh;
				BYTE a = (BYTE)((ai>255.0)?255.0:((ai<0.0)?0.0:ai));

				BYTE src_r = imData[nIndex+2];
				BYTE src_g = imData[nIndex+1];
				BYTE src_b = imData[nIndex+0];
				BYTE src_a = imData[nIndex+3];

				BYTE tmpR = g;
				BYTE tmpG = g;
				BYTE tmpB = b;
				BYTE tmpA = a;
				
				BlendPixels(src_r, src_g, src_b, src_a, 
							tmpR, tmpG, tmpB, tmpA, 
							tmpBuf[bufIndex+2], tmpBuf[bufIndex+1], tmpBuf[bufIndex+0], tmpBuf[bufIndex+3]);

				if (tmpBuf[bufIndex+2]>(tmpBuf[bufIndex+0]+100) &&
					tmpBuf[bufIndex+2]>(tmpBuf[bufIndex+1]+100))
						tmpBuf[bufIndex+2] = tmpBuf[bufIndex+1];
			
				
				}

		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StartHistoryBlock();
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangePartOfLayer(tmpBuf, 2*m_size, 2*m_size, pX-m_size, pY-m_size);
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StopHistoryBlock(_T("Red Eye Remove"));

		if (GetAppManager()->IsBackgroundCommandOnFullImage())
		  GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangeImage(ILayer::SLOW,NULL,true);
		else
			GetAppManager()->GetLayersManager()->GetCurrentLayer()->InvalidateImagePart(pX-m_size, pY-m_size,pX+m_size, pY+m_size);
		GetAppManager()->GetView()->InvalidateView();
		
		delete[] tmpBuf;

		return true;
	}


private:
	LONG  m_mousePos[2];
	int   m_size;

	float  GetDistanceBeetweenIndexes(long ind1_X, long ind1_Y, long ind2_X, long ind2_Y)
	{
		return sqrt((float)((ind2_X-ind1_X)*(ind2_X-ind1_X)+(ind2_Y-ind1_Y)*(ind2_Y-ind1_Y)));
	}

	void  ConvertToHSV(const BYTE& Red, const BYTE& Green, const BYTE& Blue, 
						int& hVal, int& sVal, int& vVal)
	{
		double minVal;
		double maxVal;
		double delta;

		double r = (double)Red / 255.0;
		double g = (double)Green / 255.0;
		double b = (double)Blue / 255.0;

		double h;
		double s;
		double v;

		minVal = min(min(r, g), b);
		maxVal = max(max(r, g), b);
		v = maxVal;
		delta = maxVal - minVal;

		if (fabs(maxVal) < 0.00001 || fabs(delta) < 0.00001) 
		{
			s = 0.0;
			h = 0.0;
		} 
		else 
		{
			s = delta / maxVal;
			if (r == maxVal) 
			{
				// Between Yellow and Magenta
				h = (g - b) / delta;
			} 
			else if (g == maxVal) 
			{
				// Between Cyan and Yellow
				h = 2.0 + (b - r) / delta;
			} 
			else 
			{
				// Between Magenta and Cyan
				h = 4.0 + (r - g) / delta;
			}

		}
		
		h *= 60.0;

		if (h < 0.0)
		{
			h += 360.0;
		}

		hVal = (int)h;
		sVal = (int)(s * 100);
		vVal = (int)(v * 100);
	}

	void     BlendPixels(const BYTE& srcR, const BYTE& srcG,const BYTE& srcB,const BYTE& srcA,
						 const BYTE& tmpR, const BYTE& tmpG,const BYTE& tmpB,const BYTE& tmpA,
						 BYTE& resR, BYTE& resG , BYTE& resB, BYTE& resA)
	{
		int lhsA = srcA; 

		int rhsA = tmpA;  

		int y = ((lhsA) * (255 - rhsA) + 0x80); 
		y = ((((y) >> 8) + (y)) >> 8); 

		int totalA = y + rhsA; 

		if (totalA == 0) { resR = resG = resB = resA = 0; } 
		else 
		{ 
			int fB; int fG; int fR; 
			fB = tmpB; 
			fG = tmpG; 
			fR = tmpR; 

			int x = ((lhsA) * (rhsA) + 0x80); 
			x = ((((x) >> 8) + (x)) >> 8);

			int z = rhsA - x; 
						
			resB = (BYTE)((float)(((srcB * y) + (tmpB * z) + (fB * x)))/(float)(totalA)); 
			resG = (BYTE)((float)(((srcG * y) + (tmpG * z) + (fG * x)))/(float)(totalA)); 
			resR = (BYTE)((float)(((srcR * y) + (tmpR * z) + (fR * x)))/(float)(totalA)); 
			int tempA = ((lhsA) * (255 - (rhsA)) + 0x80); 
			tempA = ((((tempA) >> 8) + (tempA)) >> 8);  
			tempA += (rhsA);  

			resA = (BYTE)tempA;


		} 

	}
};

#endif