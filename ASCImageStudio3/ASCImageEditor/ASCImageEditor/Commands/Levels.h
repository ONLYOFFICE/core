#ifndef __LEVELS_COMM_
#define __LEVELS_COMM_

#include "..//CommandManager.h"
/************************************************************************/
/* LEVELS COMMAND   -----   ID = 107                                    */
/************************************************************************/
#include <ImageTransformsCoreBGRA.h>

class CLevelsCommand : public CCommand
{
public:
	CLevelsCommand(IAppManager* app_manager):CCommand(app_manager)
		,m_changing_value(false), m_red(0), m_green(0), m_blue(0)
	{
		AddParameter("Red", "Red Level", m_red, -100, 100);
		AddParameter("Green", "Green Level", m_green, -100, 100);
		AddParameter("Blue", "Blue Level", m_blue, -100, 100);
		
		long lWidth = 0; long lHeight = 0;
		const BYTE* pBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(lWidth, lHeight);

		m_backup_layer = new BYTE[4*lWidth*lHeight];
		memcpy(m_backup_layer, pBuffer, 4*lWidth*lHeight);

		Gdiplus::Bitmap* tmpImage = new Bitmap(lWidth, lHeight, 4*lWidth, PixelFormat32bppARGB, const_cast<BYTE*>(pBuffer));

		m_backup_thumbnail = (Gdiplus::Bitmap*)tmpImage->GetThumbnailImage(400, lHeight*400/lWidth, NULL, NULL);
		delete tmpImage;
		
		m_current_thumbnail = NULL;
	}
	virtual ~CLevelsCommand() 
	{
		delete m_backup_layer;
		delete m_backup_thumbnail;
		if (m_current_thumbnail)
			delete m_current_thumbnail;
		
	}
public:

	virtual void OnTimer(void)	
	{
		m_changing_value = false;
		GetAppManager()->GetTimer()->StopTimer();
		GetAppManager()->GetView()->EnableImageDraw(true);
		SetInvalidateFlag(INVALID_GRAPHICS | INVALID_LAYER);
	}

	virtual void OnChangeParameter(int paramInd) 
	{
		if (!m_backup_thumbnail)
			return;

		if (paramInd==0 || paramInd==1 || paramInd==2)
		{
			if (m_current_thumbnail)
				delete m_current_thumbnail;

			long newVal = m_params[paramInd]->m_value.lVal;

			m_current_thumbnail = NULL;
			m_current_thumbnail = m_backup_thumbnail->Clone(0,0,m_backup_thumbnail->GetWidth(), m_backup_thumbnail->GetHeight(),PixelFormat32bppARGB);

			if (!m_current_thumbnail)
				return;

			GetAppManager()->GetTimer()->StartTimer(1000);//1000
			GetAppManager()->GetView()->EnableImageDraw(false);

			BitmapData oBitmapData; 
			if ( Gdiplus::Ok != m_current_thumbnail->LockBits ( NULL, ImageLockModeRead, m_current_thumbnail->GetPixelFormat (), &oBitmapData ) )
					return;
			BYTE* imData = (BYTE*)oBitmapData.Scan0;

			if (paramInd==0)
				m_red = newVal;
			if (paramInd==1)
				m_green = newVal;
			if (paramInd==2)
				m_blue = newVal;
			double levelVal = newVal;

			//if (AdjustLevels(imData,m_current_thumbnail->GetWidth(), m_current_thumbnail->GetHeight(),levelVal, paramInd)==ImageStudio::Transforms::Core::c_nErrorNone)
			AdjustAll(imData,m_current_thumbnail->GetWidth(), m_current_thumbnail->GetHeight());
			{
				m_changing_value = true;
				SetInvalidateFlag(INVALID_GRAPHICS);
			}
			

			m_current_thumbnail->UnlockBits ( &oBitmapData );
		}
	}

	virtual void DrawOnGraphics(Gdiplus::Graphics* pGr, const CRect& rect)
	{
		if (!m_changing_value)
			return;
		if (m_current_thumbnail)
		{
			pGr->DrawImage(m_current_thumbnail, GetAppManager()->GetView()->GetImageRect());
		}
	}

	virtual bool DrawOnLayer()	
	{	
		long lWidth = 0; long lHeight = 0;
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(lWidth, lHeight);

		BYTE* newImage = new BYTE[4*lWidth*lHeight];
		memcpy(newImage, m_backup_layer, 4*lWidth*lHeight);

		AdjustAll(newImage,lWidth, lHeight);
		{
			GetAppManager()->GetLayersManager()->GetCurrentLayer()->StartHistoryBlock();
			GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangePartOfLayer(newImage, lWidth, lHeight,0,0);
			GetAppManager()->GetLayersManager()->GetCurrentLayer()->StopHistoryBlock("Levels");
		}

		/*memcpy(m_backup_layer, newImage, 4*lWidth*lHeight);
		if (m_backup_thumbnail)
			delete m_backup_thumbnail;
		m_backup_thumbnail = m_current_thumbnail;
		m_current_thumbnail = NULL;*/
		delete[] newImage;

		return true;
	}


private:

	Gdiplus::Bitmap*   m_backup_thumbnail;
	Gdiplus::Bitmap*   m_current_thumbnail;

	BYTE*              m_backup_layer;

	bool               m_changing_value;

	int                m_red;
	int                m_green;
	int                m_blue;

	int AdjustLevels( BYTE* pPixels, int nWidth, int nHeight, double Level, unsigned short levelInd)
	{
		Level = (Level>0.0)?(4*Level/100.0+1.0):(0.8*Level/100.0+1.0);
		
		if (levelInd>2)
			return c_nErrorNone;
		
		// variables
		BYTE Table[256];
		
		for (int i = 0; i < 256; ++i)
		{
			Table[i] = (BYTE)min(255, (int)((255.0 * pow(((double)i) / 255.0, 1.0 / Level)) + 0.5));
		}


		// apply table
		for (int nCount = nWidth*nHeight; nCount > 0; --nCount, pPixels += 4)
		{
			pPixels[2-levelInd] = Table[pPixels[2-levelInd]];
		}
		return c_nErrorNone;
	}

	void   AdjustAll(BYTE* pPixels, int nWidth, int nHeight)
	{
		double LevelR = (m_red>0.0)?(4*m_red/100.0+1.0):(0.8*m_red/100.0+1.0);
		double LevelG = (m_green>0.0)?(4*m_green/100.0+1.0):(0.8*m_green/100.0+1.0);
		double LevelB = (m_blue>0.0)?(4*m_blue/100.0+1.0):(0.8*m_blue/100.0+1.0);
		
		// variables
		BYTE TableR[256];
		BYTE TableG[256];
		BYTE TableB[256];

		for (int i = 0; i < 256; ++i)
		{
			TableR[i] = (BYTE)min(255, (int)((255.0 * pow(((double)i) / 255.0, 1.0 / LevelR)) + 0.5));
			TableG[i] = (BYTE)min(255, (int)((255.0 * pow(((double)i) / 255.0, 1.0 / LevelG)) + 0.5));
			TableB[i] = (BYTE)min(255, (int)((255.0 * pow(((double)i) / 255.0, 1.0 / LevelB)) + 0.5));
		}


		// apply table
		for (int nCount = nWidth*nHeight; nCount > 0; --nCount, pPixels += 4)
		{
			pPixels[0] = TableB[pPixels[0]];
			pPixels[1] = TableG[pPixels[1]];
			pPixels[2] = TableR[pPixels[2]];
		}
	}
};

#endif