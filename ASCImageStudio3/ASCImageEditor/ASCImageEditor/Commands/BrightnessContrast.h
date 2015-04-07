#ifndef __BRIGHTNESS_CONTRAST_COMM_
#define __BRIGHTNESS_CONTRAST_COMM_

#include "..//CommandManager.h"
/************************************************************************/
/* BRIGHTNESS-CONTRAST COMMAND   -----   ID = 106                                    */
/************************************************************************/
#include <ImageTransformsCoreBGRA.h>

class CBrightnessContrastCommand : public CCommand
{
public:
	CBrightnessContrastCommand(IAppManager* app_manager):CCommand(app_manager)
		,m_changing_value(false)
	{
		AddParameter("Brightness", "Brightness Value", 0, -255, 255);
		AddParameter("Contrast", "Contrast Value", 0, -100, 100);
		
		long lWidth = 0; long lHeight = 0;
		const BYTE* pBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(lWidth, lHeight);

		m_backup_layer = new BYTE[4*lWidth*lHeight];
		memcpy(m_backup_layer, pBuffer, 4*lWidth*lHeight);

		Gdiplus::Bitmap* tmpImage = new Bitmap(lWidth, lHeight, 4*lWidth, PixelFormat32bppARGB, const_cast<BYTE*>(pBuffer));

		/*if (lWidth<400)
			m_backup_thumbnail = tmpImage;
		else*/
		{
			m_backup_thumbnail = (Gdiplus::Bitmap*)tmpImage->GetThumbnailImage(400, lHeight*400/lWidth, NULL, NULL);
			delete tmpImage;
		}
		m_current_thumbnail = NULL;

		DEV_LOG("---Brightness Contrast Command Begin");
	}
	virtual ~CBrightnessContrastCommand() 
	{
		DEV_LOG("---Brightness Contrast Command End");
		
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

		if ((paramInd==0 && m_params.GetCount()>0) ||
			(paramInd==1 && m_params.GetCount()>1))
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
			{
				if (ImageStudio::Transforms::Core::BGRA::BGRA_AdjustBrightnessEx(imData,
																				m_current_thumbnail->GetWidth(), 
																				m_current_thumbnail->GetHeight(),
																				1.0,newVal,
																				newVal,	newVal)==ImageStudio::Transforms::Core::c_nErrorNone)
				{
					//m_brightness = newVal;
					m_changing_value = true;
					SetInvalidateFlag(INVALID_GRAPHICS);
				}
			}
			else
				if (paramInd==1)
				{
					if (ImageStudio::Transforms::Core::BGRA::BGRA_AdjustContrast(imData,
															m_current_thumbnail->GetWidth(), 
															m_current_thumbnail->GetHeight(),
															1.0,newVal)==ImageStudio::Transforms::Core::c_nErrorNone)
					{
						//m_contrast = newVal;
						m_changing_value = true;
						SetInvalidateFlag(INVALID_GRAPHICS);
					}
				}

			CHANGING_VALUE  tmpVal;
			if (paramInd==0)
				tmpVal.m_type = CP_BRIGHTNESS;
			else
				tmpVal.m_type = CP_CONTRAST;
			tmpVal.m_val = newVal;
			if (m_changing_stack.GetCount()>0)
			{
				if (m_changing_stack[m_changing_stack.GetCount()-1].m_type==tmpVal.m_type)
					m_changing_stack[m_changing_stack.GetCount()-1].m_val = tmpVal.m_val;
				else
					m_changing_stack.Add(tmpVal);
			}
			else
				m_changing_stack.Add(tmpVal);

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
		if (m_changing_stack.GetCount()==0)
			return false;
		long lWidth = 0; long lHeight = 0;
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(lWidth, lHeight);

		BYTE* newImage = new BYTE[4*lWidth*lHeight];
		memcpy(newImage, m_backup_layer, 4*lWidth*lHeight);

		for (size_t i=0;i<m_changing_stack.GetCount();i++)
		{
			switch (m_changing_stack[i].m_type)
			{
			case CP_BRIGHTNESS:
				ImageStudio::Transforms::Core::BGRA::BGRA_AdjustBrightnessEx(newImage, lWidth, lHeight,	1.0, m_changing_stack[i].m_val, m_changing_stack[i].m_val, m_changing_stack[i].m_val);
				break;
			case CP_CONTRAST:
				ImageStudio::Transforms::Core::BGRA::BGRA_AdjustContrast(newImage, lWidth, lHeight,	1.0, m_changing_stack[i].m_val);
				break;
			default:
				break;
			}
		}

		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StartHistoryBlock();
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangePartOfLayer(newImage, lWidth, lHeight,0,0);
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StopHistoryBlock("Brightness-Contrast");

		delete[] newImage;

		m_changing_stack.RemoveAll();
		return true;
	}


private:
	Gdiplus::Bitmap*   m_backup_thumbnail;
	Gdiplus::Bitmap*   m_current_thumbnail;

	BYTE*              m_backup_layer;

	bool               m_changing_value;

	typedef  enum
	{
		CP_BRIGHTNESS,
		CP_CONTRAST
	} CHANGING_PARAMETER_DESC;

	struct  CHANGING_VALUE
	{
		CHANGING_PARAMETER_DESC   m_type;
		int                       m_val;
	};

	CAtlArray<CHANGING_VALUE>   m_changing_stack;
};

#endif