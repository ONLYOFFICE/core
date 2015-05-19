#ifndef __HISTOGRAMM_COMM_
#define __HISTOGRAMM_COMM_

#include "..//CommandManager.h"
/************************************************************************/
/* HISTOGRAMM COMMAND   -----   ID = 103                                    */
/************************************************************************/

class CHistogrammCommand : public CCommand
{
public:
	CHistogrammCommand(IAppManager* app_manager):CCommand(app_manager)
	{
		memset(m_hr,0,256*sizeof(long));
		memset(m_hg,0,256*sizeof(long));
		memset(m_hb,0,256*sizeof(long));
		memset(m_hgray,0,256*sizeof(long));

		long xmin,xmax,ymin,ymax;

		long lWidth = 0; long lHeight = 0;
		const BYTE* pBuffer = app_manager->GetLayersManager()->GetCurrentLayer()->GetImage(lWidth, lHeight);
		
		xmin = ymin = 0;
		xmax = lWidth; ymax=lHeight;
		

		for(long y=ymin; y<ymax; y++)
		{
			for(long x=xmin; x<xmax; x++)
			{
				int  nIndex = 4*(x + y*lWidth);
				
				m_hr[pBuffer[nIndex+0]]++;
				m_hg[pBuffer[nIndex+1]]++;
				m_hb[pBuffer[nIndex+2]]++;
				m_hgray[(BYTE)((pBuffer[nIndex+0]+pBuffer[nIndex+1]+pBuffer[nIndex+2])/3)]++;
				
			}
		}

		long n=0;
		for (int i=0; i<256; i++)
		{
			if (m_hr[i]>n) n=m_hr[i];
			if (m_hg[i]>n) n=m_hg[i];
			if (m_hb[i]>n) n=m_hb[i];
			if (m_hgray[i]>n) n=m_hgray[i];
		}

		AddParameter("RedHistogramm","Red Histogramm",m_hr,256,0,255);
		AddParameter("GreenHistogramm","Green Histogramm",m_hg,256,0,255);
		AddParameter("BlueHistogramm","Blue Histogramm",m_hb,256,0,255);
		AddParameter("GrayHistogramm","Gray Histogramm",m_hgray,256,0,255);

	}
	virtual ~CHistogrammCommand() {}
public:
	virtual void OnChangeParameter(int paramInd) 
	{
		
	}

	virtual void DrawOnGraphics(Gdiplus::Graphics* pGr, const CRect& rect)
	{
	}

	virtual bool DrawOnLayer()	
	{	

		return true;
	}


private:
	long m_hr[256]; //histogram
	long m_hg[256];
	long m_hb[256];
	long m_hgray[256];
};

#endif