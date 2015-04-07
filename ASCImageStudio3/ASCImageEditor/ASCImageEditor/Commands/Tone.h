#ifndef __TONE_COMM_
#define __TONE_COMM_

#include "..//CommandManager.h"
/************************************************************************/
/* TONE COMMAND   -----   ID = 108                                    */
/************************************************************************/
#include <ImageTransformsCoreBGRA.h>

class CToneCommand : public CCommand
{
public:
	CToneCommand(IAppManager* app_manager):CCommand(app_manager)
		, m_changing_value(false)
		, m_hightlights(0)
		, m_shadows(0)
		, m_smooth(0)
		, m_explosure(0)
		, m_timer_type(TT_NONE)
		, m_last_thunbnail_changing(0)
		, m_fix_values(false)
	{
		AddParameter("Hue", "Hue Value", 0, -360, 360);
		AddParameter("Saturation", "Saturation Value", 0, -100, 100);
		AddParameter("Posterize", "Posterize Value", 100, 2, 255);
		AddParameter("Temperature", "Temperature Value", 0, -100, 100);

		AddParameter("Black", "Black Value", 0, 0, 255);
		AddParameter("Fill Light", "Fill Light Value", 0, 0, 255);
		AddParameter("Recovery", "Recovery Value", 0, 0, 255);
		AddParameter("Explosure", "Explosure Value", 0, 0, 255);
		
		long lWidth = 0; long lHeight = 0;
		const BYTE* pBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(lWidth, lHeight);

		m_backup_layer = new BYTE[4*lWidth*lHeight];
		memcpy(m_backup_layer, pBuffer, 4*lWidth*lHeight);

		Gdiplus::Bitmap* tmpImage = new Bitmap(lWidth, lHeight, 4*lWidth, PixelFormat32bppARGB, const_cast<BYTE*>(pBuffer));

		m_backup_thumbnail = (Gdiplus::Bitmap*)tmpImage->GetThumbnailImage(400, lHeight*400/lWidth, NULL, NULL);
		delete tmpImage;
		
		m_current_thumbnail = NULL;
	}
	virtual ~CToneCommand() 
	{
		delete m_backup_layer;
		delete m_backup_thumbnail;
		if (m_current_thumbnail)
			delete m_current_thumbnail;
		GetAppManager()->GetTimer()->StopTimer();
		GetAppManager()->GetView()->EnableImageDraw(true);
	}
public:

	virtual void OnTimer(void)	
	{
		if (m_timer_type==TT_FAST_CHANGINGS)
		{
			ChangeThumbnail();
			GetAppManager()->GetTimer()->StopTimer();
			GetAppManager()->GetView()->EnableImageDraw(false);
			SetInvalidateFlag(INVALID_GRAPHICS);
			GetAppManager()->GetTimer()->StartTimer(2000);
			m_timer_type = TT_SLOW_CHANGINGS;
			m_fix_values = true;
		}
		else
			if (m_timer_type==TT_SLOW_CHANGINGS)
			{
				m_changing_value = false;
				GetAppManager()->GetTimer()->StopTimer();
				GetAppManager()->GetView()->EnableImageDraw(true);
				SetInvalidateFlag(INVALID_GRAPHICS | INVALID_LAYER);
			}
	}

	virtual void OnChangeParameter(int paramInd) 
	{
		if (!m_backup_thumbnail)
			return;

		if (paramInd>=0 && paramInd<8 && m_params.GetCount()>paramInd)
		{
			GetAppManager()->GetTimer()->StopTimer();
			m_timer_type = TT_NONE;
			long newVal = m_params[paramInd]->m_value.lVal;

			if (paramInd==4)
				m_hightlights = newVal;
			if (paramInd==5)
				m_shadows = newVal;
			if (paramInd==6)
				m_smooth = newVal;
			if (paramInd==7)
				m_explosure = newVal;
			
			CHANGING_VALUE  tmpVal;
			if (paramInd==0)
				tmpVal.m_type = CP_HUE;
			else if (paramInd==1)
				tmpVal.m_type = CP_SATURATION;
			else if (paramInd==2)
				tmpVal.m_type = CP_POSTERIZE;
			else if (paramInd==3)
				tmpVal.m_type = CP_TEMPERATURE;
			else if (paramInd==4)
				tmpVal.m_type = CP_BLACK;
			else if (paramInd==5)
				tmpVal.m_type = CP_FILL_LIGHT;
			else if (paramInd==6)
				tmpVal.m_type = CP_RECOVERY;
			else if (paramInd==7)
				tmpVal.m_type = CP_EXPLOSURE;

			tmpVal.m_val = newVal;
			if (m_changing_stack.GetCount()>0)
			{
				if (m_changing_stack[m_changing_stack.GetCount()-1].m_type==tmpVal.m_type && !m_fix_values)
					m_changing_stack[m_changing_stack.GetCount()-1].m_val = tmpVal.m_val;
				else
					m_changing_stack.Add(tmpVal);
			}
			else
				m_changing_stack.Add(tmpVal);

			m_fix_values = false;

			GetAppManager()->GetTimer()->StartTimer(200);
			m_timer_type = TT_FAST_CHANGINGS;
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
			case CP_HUE:
				ImageStudio::Transforms::Core::BGRA::BGRA_AdjustHue(newImage, lWidth, lHeight,	1.0, m_changing_stack[i].m_val);
				break;
			case CP_SATURATION:
				ImageStudio::Transforms::Core::BGRA::BGRA_AdjustSaturation(newImage, lWidth, lHeight,1.0, m_changing_stack[i].m_val);
				break;
			case CP_POSTERIZE:
				ImageStudio::Transforms::Core::BGRA::BGRA_AdjustPosterize(newImage, lWidth, lHeight,1.0, m_changing_stack[i].m_val);
				break;
			case CP_TEMPERATURE:
				ImageStudio::Transforms::Core::BGRA::BGRA_AdjustTemperature(newImage, lWidth, lHeight,1.0, m_changing_stack[i].m_val);
				break;
			case CP_BLACK:
			case CP_FILL_LIGHT:
			case CP_RECOVERY:
			case CP_EXPLOSURE:
				//BGRA_AdjustTone(newImage, lWidth, lHeight,m_hightlights, m_shadows,m_smooth,m_explosure);
				break;
			default:
				break;
			}
		}

		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StartHistoryBlock();
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangePartOfLayer(newImage, lWidth, lHeight,0,0);
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StopHistoryBlock("Tone");

		delete[] newImage;

		m_changing_stack.RemoveAll();
		m_last_thunbnail_changing = 0;

		return true;
	}


private:
	Gdiplus::Bitmap*   m_backup_thumbnail;
	Gdiplus::Bitmap*   m_current_thumbnail;

	BYTE*              m_backup_layer;

	bool               m_changing_value;

	int                m_hightlights;
	int                m_shadows;
	int                m_smooth;
	int                m_explosure;

	typedef  enum
	{
		CP_HUE,
		CP_SATURATION,
		CP_POSTERIZE,
		CP_TEMPERATURE,
		CP_BLACK,
		CP_FILL_LIGHT,
		CP_RECOVERY,
		CP_EXPLOSURE
	} CHANGING_PARAMETER_DESC;

	struct  CHANGING_VALUE
	{
		CHANGING_PARAMETER_DESC   m_type;
		int                       m_val;
	};

	CAtlArray<CHANGING_VALUE>   m_changing_stack;
	int                         m_last_thunbnail_changing;
	bool                        m_fix_values;
	
	typedef enum
	{
		TT_NONE,
        TT_FAST_CHANGINGS,
		TT_SLOW_CHANGINGS
	} TIMER_TYPE;

	TIMER_TYPE m_timer_type;

	void ChangeThumbnail()
	{
		if (!m_backup_thumbnail)
			return;
		if (m_changing_stack.GetCount()==0)
			return;
		if (m_current_thumbnail)
			delete m_current_thumbnail;
		m_current_thumbnail = NULL;
		m_current_thumbnail = m_backup_thumbnail->Clone(0,0,m_backup_thumbnail->GetWidth(), m_backup_thumbnail->GetHeight(),PixelFormat32bppARGB);

		if (!m_current_thumbnail)
			return;

		BitmapData oBitmapData; 
		if ( Gdiplus::Ok != m_current_thumbnail->LockBits ( NULL, ImageLockModeRead, m_current_thumbnail->GetPixelFormat (), &oBitmapData ) )
			return;
		BYTE* imData = (BYTE*)oBitmapData.Scan0;

		int lWidth = m_current_thumbnail->GetWidth();
		int lHeight = m_current_thumbnail->GetHeight();

		for (size_t i=m_last_thunbnail_changing;i<m_changing_stack.GetCount();i++)
		{
			switch (m_changing_stack[i].m_type)
			{
			case CP_HUE:
				ImageStudio::Transforms::Core::BGRA::BGRA_AdjustHue(imData, lWidth, lHeight,	1.0, m_changing_stack[i].m_val);
				break;
			case CP_SATURATION:
				ImageStudio::Transforms::Core::BGRA::BGRA_AdjustSaturation(imData, lWidth, lHeight,1.0, m_changing_stack[i].m_val);
				break;
			case CP_POSTERIZE:
				ImageStudio::Transforms::Core::BGRA::BGRA_AdjustPosterize(imData, lWidth, lHeight,1.0, m_changing_stack[i].m_val);
				break;
			case CP_TEMPERATURE:
				ImageStudio::Transforms::Core::BGRA::BGRA_AdjustTemperature(imData, lWidth, lHeight,1.0, m_changing_stack[i].m_val);
				break;
			case CP_BLACK:
			case CP_FILL_LIGHT:
			case CP_RECOVERY:
			case CP_EXPLOSURE:
				//BGRA_AdjustTone(imData, lWidth, lHeight,m_hightlights, m_shadows,m_smooth,m_explosure);
				break;
			default:
				break;
			}
		}

		m_changing_value = true;
		m_last_thunbnail_changing = m_changing_stack.GetCount();

		m_current_thumbnail->UnlockBits ( &oBitmapData );
	}
};

#endif