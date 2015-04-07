#ifndef __BCKG_TONES_COMM_
#define __BCKG_TONES_COMM_

#include "..//CommandManager.h"
#include <vector>
/************************************************************************/
/* BACKGROUND COMMAND   ----   TONES
/************************************************************************/
//#include <ImageTransformsCoreBGRA.h>

//#include "performance.h"
//DECLARE_PERFORMANCE_FILE_STREAM(C:\\perf.log, perfFile);

static void AdjustAutoWB( const BYTE* srcPixels, int scrWidth, int scrHeight,  BYTE* pPixels, int nWidth, int nHeight );

class CBckgTonesCommand : public CCommand, public IUndoRedoCallback, public CBaseThread, public IChangesSubscriber
{
public:
	enum
	{
		PARAM_START           = 0,
		PARAM_RED             = PARAM_START,
		PARAM_GREEN           = 1,
		PARAM_BLUE            = 2,
		PARAM_HUE             = 3,
		PARAM_SATURATION      = 4,
		PARAM_POSTERIZE       = 5,
		PARAM_TEMPERATURE     = 6,
		PARAM_BLACK           = 7,
		PARAM_LIGHT           = 8,
		PARAM_RECOVERY        = 9,
		PARAM_EXPOSURE        = 10,
		PARAM_BRIGHTNESS      = 11,
		PARAM_CONTRAST        = 12,
		PARAM_DETAILING       = 13,
		PARAM_WB_PICK_REG     = 14,
		PARAM_TINT            = 15,
		PARAM_WB_SIZE         = 16,
		PARAM_WB_COLORS       = 17,
		PARAM_WB_PRESET       = 18,
		PARAM_VIGN_AMOUNT     = 19,
		PARAM_VIGN_MIDPOINT   = 20,
		PARAM_VIGN_ROUNDNESS  = 21,
		PARAM_VIGN_FEATHER    = 22,
		PARAM_SC_CLARITY      = 23,
		PARAM_SC_VIBRANCE     = 24,
		PARAM_SC_SATURATION   = 25,
		PARAM_AUTO_TONE_CORR  = 26,
		PARAM_AUTO_BR_CONTR   = 27,
		PARAM_AUTO_WB         = 28,

		PARAM_INSIDE_WB_R     = 29,
		PARAM_INSIDE_WB_G     = 30,
		PARAM_INSIDE_WB_B     = 31,

		PARAMS_COUNT          = 32,

		PICK_AREA_MAX_SIZE   = 9
	};

public:
	CBckgTonesCommand(IAppManager* app_manager):CCommand(app_manager),CBaseThread(0)
	{
		memset(m_parameters,0,PARAMS_COUNT*sizeof(int));
		
		m_parameters[PARAM_POSTERIZE]    = 100;
		m_parameters[PARAM_WB_SIZE]      = 5;

		m_parameters[PARAM_INSIDE_WB_R] =   m_parameters[PARAM_INSIDE_WB_G] =   m_parameters[PARAM_INSIDE_WB_B] =   -2;// -1 - Auto, >=0 - color

		memset(m_pick_colors, 0, 3*PICK_AREA_MAX_SIZE*PICK_AREA_MAX_SIZE*sizeof(long));
			
		AddParameter(_T("Red"), _T("Red Level"), m_parameters[PARAM_RED], -100, 100);
		AddParameter(_T("Green"), _T("Green Level"), m_parameters[PARAM_GREEN], -100, 100);
		AddParameter(_T("Blue"), _T("Blue Level"), m_parameters[PARAM_BLUE], -100, 100);

		AddParameter(_T("Hue"), _T("Hue Value"), m_parameters[PARAM_HUE], -360, 360);
		AddParameter(_T("Saturation"), _T("Saturation Value"), m_parameters[PARAM_SATURATION], -100, 100);
		AddParameter(_T("Posterize"), _T("Posterize Value"), m_parameters[PARAM_POSTERIZE], 2, 250);
		AddParameter(_T("Temperature"), _T("Temperature Value"), m_parameters[PARAM_TEMPERATURE], -100, 100);

		AddParameter(_T("Black"), _T("Black Value"), m_parameters[PARAM_BLACK], 0, 50);
		AddParameter(_T("Fill Light"), _T("Fill Light Value"), m_parameters[PARAM_LIGHT], 0, 255);
		AddParameter(_T("Recovery"), _T("Recovery Value"), m_parameters[PARAM_RECOVERY], 0, 400);
		AddParameter(_T("Explosure"), _T("Explosure Value"), m_parameters[PARAM_EXPOSURE], -100, 100);

		AddParameter(_T("Brightness"), _T("Brightness Value"), m_parameters[PARAM_BRIGHTNESS], -50, 50);
		AddParameter(_T("Contrast"), _T("Contrast Value"), m_parameters[PARAM_CONTRAST], -50, 50);

		AddParameter(_T("Detailing"), _T("Detailing"), m_parameters[PARAM_DETAILING], -100, 100);
		
		AddParameter(_T("PickColorRegime"), _T("PickColorRegime"), (m_pick_wthite_balance_point)?1:0, 0, 1);
		AddParameter(_T("White Balance Level"), _T("White Balance Level"), m_parameters[PARAM_TINT], -100, 100);
		AddParameter(_T("White Balance Pick Area Size"), _T("White Balance Level"), m_parameters[PARAM_WB_SIZE], 1, PICK_AREA_MAX_SIZE);
		AddParameter(_T("White Balance Pick colors"), _T("White Balance Pick colors"), m_pick_colors, 256,0,255);
		AddParameter(_T("White Balance Preset"), _T("Preset"), m_parameters[PARAM_WB_PRESET], 0,10);
		
		AddParameter(_T("Vignette Amount"), _T("Vignette Amount"), m_parameters[PARAM_VIGN_AMOUNT], -100, 100);
		AddParameter(_T("Vignette MidPoint"), _T("Vignette MidPoint"), m_parameters[PARAM_VIGN_MIDPOINT], 0, 100);
		AddParameter(_T("Vignette Roundness"), _T("Vignette Roundness"), m_parameters[PARAM_VIGN_ROUNDNESS], -100, 100);
		AddParameter(_T("Vignette Feather"), _T("Vignette Feather"), m_parameters[PARAM_VIGN_FEATHER], 0, 100);

		AddParameter(_T("Smart Clarity"), _T("Smart Clarity"), m_parameters[PARAM_SC_CLARITY], -100, 100);
		AddParameter(_T("Smart Vibrance"), _T("Smart Vibrance"), m_parameters[PARAM_SC_VIBRANCE], -100, 100);
		AddParameter(_T("Smart Saturation"), _T("Smart Saturation"), m_parameters[PARAM_SC_SATURATION], -100, 100);

		AddParameter(_T("Auto Tone"), _T("Auto Tone"), m_parameters[PARAM_AUTO_TONE_CORR], 0, 1);
		AddParameter(_T("Auto BrContr"), _T("Auto BrContr"), m_parameters[PARAM_AUTO_BR_CONTR], 0, 1);
		AddParameter(_T("Auto WhiteBalance"), _T("Auto WhiteBalance"), m_parameters[PARAM_AUTO_WB], 0, 1);

		memset(m_backup_parameters,0,PARAMS_COUNT*sizeof(int));
		m_backup_parameters[PARAM_INSIDE_WB_R] =   m_backup_parameters[PARAM_INSIDE_WB_G] =   m_backup_parameters[PARAM_INSIDE_WB_B] =   -2;// -1 - Auto, >=0 - color
		m_was_backuped = FALSE;

		//m_undo_cntr = 0;

		//m_exit_thread = false;

		//for (int j=-100; j<100 ; j++)
		//{
		//	for (int i = 0; i < 256; ++i)
		//	{
		//		float val = ((float)i)/255.0f;
		//		
		//		if (val<=0.04045) 
		//			val /= 12.92f; 
		//		else 
		//			val = pow((val+0.055f)/1.055f,2.4f);
		//		
		//		float expos = pow(2.0f, ((float)j)/25.0f);//pow(2.0f, ((float)j)/100.0f);
		//		/*if (j>0)
		//			expos*=2.0f;
		//		else
		//			expos/=2.0f;*/
		//		val *= expos;
		//		
		//		if (val<=0.0031308) 
		//			val *= 12.92f; 
		//		else 
		//			val = (float)(1.055*pow((double)val,1.0/2.4)-0.055);
		//		
		//		TableExposure[j+100][i] = ImageStudio::Transforms::Core::BGRA::Byte( 255.0f*val );
		//	}
		//}

		//for (int j=0; j<100 ; j++)
		//	for (int i = 0; i < 256; ++i)
		//		{
		//			double dBlack = (255.0 / 100.0) *((double)j);
		//			int offset = 0;
		//			if( i > dBlack )
		//			{
		//				offset = int(255 * (i - dBlack) / (256 - dBlack) + 0.5);
		//			}
		//			TableBlacks[j][i] = offset - i;
		//		}


		//for (int j=0; j<256 ; j++)
		//	for (int i = 0; i < 256; ++i)
		//	{
		//		double dFactor = (1024.0 / 255.0)* ((double)j);
		//		double dOffset = 1024 + dFactor;
		//		dFactor /= 255.0;
		//		TableFillLight[j][i] = ImageStudio::Round( dOffset - i * dFactor );
		//	}
 

		/*for (int j=0; j<100 ; j++)
			for (int i = 0; i < 256; ++i)
			{
				double dFactor = (4096.0*((double)j) / (1000.0 * 255.0)) ;
				TableRecovery[j][i] = int(4096.0 - ((double)i) * dFactor);
			}*/

		m_changing_group = FALSE;

		m_current_point.x = m_current_point.y  = -1;
		m_pick_wthite_balance_point = FALSE;
		m_onImage = FALSE;
		
	}
	virtual ~CBckgTonesCommand() 
	{
		//m_exit_thread = true;
		if (GetAppManager()->GetLayersManager()->GetCurrentLayer())
		{
			GetAppManager()->GetLayersManager()->GetCurrentLayer()->TryUsingSlowFunction(NULL);	
			GetAppManager()->GetLayersManager()->GetCurrentLayer()->RemoveSubscriber(this);
		}
	}
public:
	void         Reset()
	{
		memset(m_parameters,0,PARAMS_COUNT*sizeof(int));
		m_parameters[PARAM_POSTERIZE]    = 100;
		m_parameters[PARAM_INSIDE_WB_R] =   m_parameters[PARAM_INSIDE_WB_G] =   m_parameters[PARAM_INSIDE_WB_B] =   -2;// -1 - Auto, >=0 - color

		for (int i=0; i<PARAMS_COUNT;i++)
			FireChangeParameter(i, (int)m_parameters[i]);

		memset(m_backup_parameters,0,PARAMS_COUNT*sizeof(int));
		m_backup_parameters[PARAM_INSIDE_WB_R] =   m_backup_parameters[PARAM_INSIDE_WB_G] =   m_backup_parameters[PARAM_INSIDE_WB_B] =   -2;// -1 - Auto, >=0 - color

		m_parameters[PARAM_WB_SIZE]      = 5;

		memset(m_pick_colors, 0, 3*PICK_AREA_MAX_SIZE*PICK_AREA_MAX_SIZE*sizeof(long));
		
		m_was_backuped = FALSE;

		//m_exit_thread = false;

	}
	virtual void OnTimer(void)	
	{
		SetInvalidateFlag(INVALID_LAYER);
		GetAppManager()->GetTimer()->StopTimer(this);
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StartHistoryBlock();
		PushChagesToUndo();
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangeManually(this);
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StopHistoryBlock(_T("Tone"));
		
		//m_exit_thread = false;
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->TryUsingSlowFunction(this);
	}

	virtual void StartChangingParamsGroup() 
	{
		m_changing_group = TRUE;
		//m_exit_thread = true;
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->TryUsingSlowFunction(NULL);
		BackupParameters();

	}
	
	virtual void OnChangeParameter(int paramInd) 
	{
		if (paramInd == PARAM_WB_PICK_REG)   // pick color
		{
			m_pick_wthite_balance_point = (m_params[paramInd]->m_value.lVal>0);
			SetInvalidateFlag(INVALID_GRAPHICS);
			return;
		}
		if (paramInd == PARAM_WB_SIZE)   // WB area size
		{
			m_parameters[PARAM_WB_SIZE] = m_params[paramInd]->m_value.lVal;
			//SetInvalidateFlag(INVALID_GRAPHICS);
			return;
		}
		if( paramInd == PARAM_AUTO_TONE_CORR || 
			paramInd == PARAM_AUTO_BR_CONTR || 
			paramInd == PARAM_AUTO_WB ) 
		{
			if( m_params[paramInd]->m_value.lVal == 1 )
			{
				StartChangingParamsGroup();
				
				if (paramInd == PARAM_AUTO_WB)  // AutoWB
				{
					m_parameters[PARAM_INSIDE_WB_R] = -1;
					m_parameters[PARAM_INSIDE_WB_G] = -1;
					m_parameters[PARAM_INSIDE_WB_B] = -1;
				}
				if (paramInd == PARAM_AUTO_BR_CONTR)  // AutoBrContr
				{
					double autoBr = 0;
					double autoContr = 0;

					long wdth, hght;
					BYTE* imData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST, wdth,hght);
					ImageStudio::Transforms::Core::BGRA::BGRA_GetAutoBrightness(imData, wdth,hght, &autoBr);
					ImageStudio::Transforms::Core::BGRA::BGRA_GetAutoContrast(imData, wdth,hght, &autoContr);

					m_parameters[PARAM_BRIGHTNESS] = (int)autoBr;
					m_parameters[PARAM_CONTRAST] = (int)autoContr;

					FireChangeParameter(PARAM_BRIGHTNESS, (int)m_parameters[PARAM_BRIGHTNESS]);
					FireChangeParameter(PARAM_CONTRAST, (int)m_parameters[PARAM_CONTRAST]);
				}
				if (paramInd == PARAM_AUTO_TONE_CORR)  // AutoTone
				{
					double autoPost=0.0;
					double autoSatur=0.0;
					double autoExpl=0.0;

					long wdth, hght;
					BYTE* imData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST, wdth,hght);
					ImageStudio::Transforms::Core::BGRA::BGRA_GetAutoPosterize(imData, wdth,hght, &autoPost);
					ImageStudio::Transforms::Core::BGRA::BGRA_GetAutoSaturation(imData, wdth,hght, &autoSatur);
					ImageStudio::Transforms::Core::BGRA::BGRA_GetAutoHue(imData, wdth,hght, &autoExpl);

					autoExpl/=100.0;

					m_parameters[PARAM_POSTERIZE] = (int)autoPost;
					m_parameters[PARAM_SC_SATURATION] = (int)autoSatur;
					m_parameters[PARAM_EXPOSURE] = (int)autoExpl;

					FireChangeParameter(PARAM_POSTERIZE, (int)m_parameters[PARAM_POSTERIZE]);
					FireChangeParameter(PARAM_SC_SATURATION, (int)m_parameters[PARAM_SC_SATURATION]);
					FireChangeParameter(PARAM_EXPOSURE, (int)m_parameters[PARAM_EXPOSURE]);

				}
				FinishChangingParamsGroup();
			}
		}
		else
		if (paramInd >= PARAM_START && paramInd < PARAMS_COUNT)
		{
			if (!m_changing_group)
			{
				//m_exit_thread = true;
				GetAppManager()->GetLayersManager()->GetCurrentLayer()->TryUsingSlowFunction(NULL);
				BackupParameters();
				
				if (paramInd == PARAM_INSIDE_WB_R)
				{
					m_parameters[PARAM_INSIDE_WB_R] = 0;
					m_parameters[PARAM_INSIDE_WB_B] = 0;
					m_parameters[PARAM_INSIDE_WB_B] = 0;

					int n_min = -m_parameters[PARAM_WB_SIZE] / 2;
					int n_max =  m_parameters[PARAM_WB_SIZE] / 2;

					int ind2 = 0;
					int px_cnt = 0;
					
					for (int i = n_min; i <= n_max; i++)
					{
						for (int j = n_min; j <= n_max; j++, ind2 += 3, px_cnt++)
						{
							m_parameters[PARAM_INSIDE_WB_R + 0] += m_pick_colors[ind2 + 0];
							m_parameters[PARAM_INSIDE_WB_R + 1] += m_pick_colors[ind2 + 1];
							m_parameters[PARAM_INSIDE_WB_R + 2] += m_pick_colors[ind2 + 2];
						}
					}

					m_parameters[PARAM_INSIDE_WB_R + 0] /= px_cnt;
					m_parameters[PARAM_INSIDE_WB_R + 1] /= px_cnt;
					m_parameters[PARAM_INSIDE_WB_R + 2] /= px_cnt;
				 }
				else
				{
					long newVal = m_params[paramInd]->m_value.lVal;
					m_parameters[paramInd] = (int)newVal;
					if (!isParametersChanges())
					{
						m_parameters[PARAM_BLACK] = 1;
					}
				}

				GetAppManager()->GetTimer()->StartTimer(this, 1000); //1000
				
				GetAppManager()->GetLayersManager()->GetCurrentLayer()->UpdateCopy(ILayer::FAST);
				long wdth, hght;
				BYTE* imData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST_COPY, wdth,hght);
				//m_exit_thread = false;
				AdjustAll(imData, wdth, hght);
				//AdjustAll(imData, wdth/2, hght, (wdth/2)*4);
				//AdjustAll(imData+(wdth*50+80)*4, 100, 80, (wdth-100)*4);

				GetAppManager()->GetLayersManager()->GetCurrentLayer()->SetActualImage(ILayer::FAST_COPY);
				SwitchViewToFast();
				GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangeImage(ILayer::FAST_COPY,this);
				GetAppManager()->GetView()->InvalidateView();
			}
			else
			{
				long newVal = m_params[paramInd]->m_value.lVal;
				m_parameters[paramInd] = (int)newVal;
			}
		}
		/*if (m_undo_cntr>0)
			m_undo_cntr--;*/
	}

	virtual void FinishChangingParamsGroup() 
	{
		if (m_changing_group)
		{
			if (!isParametersChanges())
			{
				m_parameters[PARAM_BLACK] = 1;
			}
			GetAppManager()->GetTimer()->StartTimer(this, 1000);//1000

			GetAppManager()->GetLayersManager()->GetCurrentLayer()->UpdateCopy(ILayer::FAST);
			long wdth, hght;
			BYTE* imData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST_COPY, wdth,hght);
			//m_exit_thread = false;
			AdjustAll(imData, wdth, hght);

			GetAppManager()->GetLayersManager()->GetCurrentLayer()->SetActualImage(ILayer::FAST_COPY);
			SwitchViewToFast();
			GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangeImage(ILayer::FAST_COPY,this);
			GetAppManager()->GetView()->InvalidateView();
			m_changing_group = FALSE;
		}
	}


	virtual void OnMouseMove(LONG nX, LONG nY, LONG nButton)	
	{
		if (!m_pick_wthite_balance_point)
			return;

		m_current_point.x = nX;
		m_current_point.y = nY;

		GetAppManager()->GetCoordinateTransformer()->ScreenPointToImagePoint(nX, nY);

		long wdth, hght;
		BYTE* imData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, wdth,hght);

		if (nX<0 || nY<0 || nX>wdth || nY>hght)
			m_onImage = FALSE;
		else
			m_onImage = TRUE;

		if (nX>m_parameters[PARAM_WB_SIZE] && nY>m_parameters[PARAM_WB_SIZE] &&
			nX<wdth-m_parameters[PARAM_WB_SIZE] && nY<hght-m_parameters[PARAM_WB_SIZE])
		{
			int ind2 = 0;
			//m_parameters[PARAM_INSIDE_WB_R] = m_parameters[PARAM_INSIDE_WB_B] = m_parameters[PARAM_INSIDE_WB_B] = 0;
			int px_cnt = 0;
			for (int i=nX-m_parameters[PARAM_WB_SIZE]/2; i<=nX+m_parameters[PARAM_WB_SIZE]/2; i++)
				for (int j=nY-m_parameters[PARAM_WB_SIZE]/2; j<=nY+m_parameters[PARAM_WB_SIZE]/2; j++, ind2+=3, px_cnt++)
				{
					m_pick_colors[ind2] = imData[4*(i+j*wdth)+2];
					m_pick_colors[ind2+1] = imData[4*(i+j*wdth)+1];
					m_pick_colors[ind2+2] = imData[4*(i+j*wdth)+0];

					//for (int k=0;k<3;k++)
					//	m_parameters[PARAM_INSIDE_WB_R+k]+=m_pick_colors[ind2+k];
				}

				//for (int k=0;k<3;k++)
				//	m_parameters[PARAM_INSIDE_WB_R+k]/=px_cnt;

				FireChangeParameter(17, m_pick_colors, 3*PICK_AREA_MAX_SIZE*PICK_AREA_MAX_SIZE);

			SetInvalidateFlag(INVALID_GRAPHICS);
		}

	}

	virtual void OnMouseDown(LONG nX, LONG nY, LONG nButton)
	{
		if (!m_pick_wthite_balance_point || !m_onImage)
			return;
		
		OnChangeParameter(29) ;	

		//FireChangeParameter(PARAM_TEMPERATURE, (int)(((float)m_pick_mid_color[0])/255.0f*200.0f-100.0f));
		//FireChangeParameter(PARAM_WB_LEVEL, 15);

		m_onImage = FALSE;

	}
	
	virtual void DrawOnGraphics(Gdiplus::Graphics* pGr, const CRect& rect)
	{
		if (!m_pick_wthite_balance_point || !m_onImage)
			return;
		Gdiplus::Pen pen( Gdiplus::Color::Black );

        #define  RECT_SIZE  30
		#define   SHFT    10

		POINT startP;
		startP.x = m_current_point.x+SHFT;
		startP.y = m_current_point.y+SHFT;

		if ((startP.x+m_parameters[PARAM_WB_SIZE]*RECT_SIZE)>rect.Width())
			startP.x -= m_parameters[PARAM_WB_SIZE]*RECT_SIZE+2*SHFT;
		if ((startP.y+m_parameters[PARAM_WB_SIZE]*RECT_SIZE)>rect.Height())
			startP.y -= m_parameters[PARAM_WB_SIZE]*RECT_SIZE+2*SHFT;

		int ind2 = 0;
		for (int i=0;i<m_parameters[PARAM_WB_SIZE];i++)
			for (int j=0;j<m_parameters[PARAM_WB_SIZE];ind2+=3,j++)
			{
				Gdiplus::SolidBrush tmpBrush(Gdiplus::Color((BYTE)m_pick_colors[ind2], (BYTE)m_pick_colors[ind2+1], (BYTE)m_pick_colors[ind2+2]));
				pGr->FillRectangle(&tmpBrush, startP.x+RECT_SIZE*i, startP.y+RECT_SIZE*j, RECT_SIZE, RECT_SIZE);
				pGr->DrawRectangle(&pen, startP.x+RECT_SIZE*i, startP.y+RECT_SIZE*j, RECT_SIZE, RECT_SIZE);
			}

		POINT centerP;
		centerP.x = startP.x+RECT_SIZE*((m_parameters[PARAM_WB_SIZE])/2)+RECT_SIZE/2;
		centerP.y = startP.y+RECT_SIZE*((m_parameters[PARAM_WB_SIZE])/2)+RECT_SIZE/2;

		pGr->DrawLine(&pen, centerP.x, centerP.y-4, centerP.x, centerP.y+4);
		pGr->DrawLine(&pen, centerP.x-4, centerP.y, centerP.x+4, centerP.y);

			/*Gdiplus::Pen pen1( Gdiplus::Color::Green );
			pGr->DrawRectangle(&pen1, m_current_point.x+RECT_SIZE*(m_parameters[PARAM_WB_SIZE])/2, 
				m_current_point.y+RECT_SIZE*(m_parameters[PARAM_WB_SIZE])/2, 
				RECT_SIZE, RECT_SIZE);*/



		/*pGr->DrawRectangle(&pen, m_current_point.x+30, m_current_point.y+30, RECT_SIZE, RECT_SIZE);
		int xStep = RECT_SIZE/m_parameters[PARAM_WB_SIZE];
		int yStep = RECT_SIZE/m_parameters[PARAM_WB_SIZE];
		for (int i=m_current_point.x+30; i<m_current_point.x+30+RECT_SIZE;i+=xStep)
			pGr->DrawLine(&pen , i, m_current_point.y+30, i, m_current_point.y+30+RECT_SIZE);

		for (int j=m_current_point.y+30; j<m_current_point.y+30+RECT_SIZE;j+=yStep)
			pGr->DrawLine(&pen , m_current_point.x+30, j, m_current_point.x+30+RECT_SIZE, j);*/
			
	}

	virtual bool DrawOnLayer()	
	{	
		return true;
	}

	virtual void Undo(int stepsCount)
	{
	}

	virtual void Redo(int stepsCount)
	{
	}

	virtual  void  OnUndo(int stepsCount, bool Reset)
	{
		for (int i=0;i<stepsCount;i++)
		{
			if (!m_undo_parameters.IsEmpty())
			{
				PushParamsToRedo();
				for (size_t indx = 0;indx<m_undo_parameters.GetTail().size(); indx++)
				{
					m_parameters[m_undo_parameters.GetTail()[indx].m_index] = m_undo_parameters.GetTail()[indx].m_value;
					FireChangeParameter(m_undo_parameters.GetTail()[indx].m_index, m_undo_parameters.GetTail()[indx].m_value);
					//m_undo_cntr++;
				}
				m_undo_parameters.RemoveTail();
			}
		}
	}
	virtual  void  OnRedo(int stepsCount)
	{
		for (int i=0;i<stepsCount;i++)
		{
			if (!m_redo_parameters.IsEmpty())
			{
				PushParamsToUndo();
				for (size_t indx = 0;indx<m_redo_parameters.GetHead().size(); indx++)
				{
					m_parameters[m_redo_parameters.GetHead()[indx].m_index] = m_redo_parameters.GetHead()[indx].m_value;
					FireChangeParameter(m_redo_parameters.GetHead()[indx].m_index, m_redo_parameters.GetHead()[indx].m_value);
				}
				m_redo_parameters.RemoveHead();
			}
		}
	}

	virtual DWORD ThreadProc()
	{
		//DECLARE_PERFORMANCE_CHECKER(1, perfFile);
		//START_PERFORMANCE_CHECK(1);
		if (!isParametersChanges())
		{
			GetAppManager()->SendMsg(WM_IEM_THREAD_FINISHED,0);
			return 0;
		}
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->UpdateCopy(ILayer::SLOW);
		long wdth, hght;
		BYTE* imData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW_COPY, wdth,hght);
		
		AdjustAll(imData, wdth, hght);

		//m_tables_calculator.Calc(m_parameters);

		//int   val_A = wdth/2;
		//int   val_B = wdth-val_A;
		//int   val_C = hght/2;
		//int   val_D = hght-val_C;
		//
		//threadData tr_1(imData,                        val_A, val_C, val_B*4, &m_tables_calculator, this);
		//threadData tr_2(imData+val_A*4,                val_B, val_C, val_A*4, &m_tables_calculator, this);
		//threadData tr_3(imData+wdth*val_C*4,           val_A, val_D, val_B*4, &m_tables_calculator, this);
		//threadData tr_4(imData+(wdth*val_C+val_A)*4,   val_B, val_D, val_A*4, &m_tables_calculator, this);

		//DWORD dwTemp;

		//HANDLE     hThread_1  = ::CreateThread ( NULL, 0, &ThreadFunc, (void*)&tr_1, 0, &dwTemp ); ::SetThreadPriority(hThread_1, THREAD_PRIORITY_HIGHEST);
		//HANDLE     hThread_2  = ::CreateThread ( NULL, 0, &ThreadFunc, (void*)&tr_2, 0, &dwTemp ); ::SetThreadPriority(hThread_2, THREAD_PRIORITY_HIGHEST);
		//HANDLE     hThread_3  = ::CreateThread ( NULL, 0, &ThreadFunc, (void*)&tr_3, 0, &dwTemp ); ::SetThreadPriority(hThread_3, THREAD_PRIORITY_HIGHEST);
		//HANDLE     hThread_4  = ::CreateThread ( NULL, 0, &ThreadFunc, (void*)&tr_4, 0, &dwTemp ); ::SetThreadPriority(hThread_4, THREAD_PRIORITY_HIGHEST);

		//WaitForSingleObject(hThread_1, INFINITE);
		//WaitForSingleObject(hThread_2, INFINITE);
		//WaitForSingleObject(hThread_3, INFINITE);
		//WaitForSingleObject(hThread_4, INFINITE);

		if (m_parameters[PARAM_SC_CLARITY]!=0)
		{
			ImageStudio::Transforms::Core::BGRA::BGRA_AdjustClarity(imData, wdth,hght, 1.0,m_parameters[PARAM_SC_CLARITY], 30, 10);
		}
	
		if (m_parameters[PARAM_DETAILING]!=0 /*&& !m_exit_thread*/)
		{
			long wdth1, hght1;
			BYTE* tmp_imData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW_COPY, wdth1,hght1);
			float paramCoef = (float)wdth1/(float)wdth;
			if (m_parameters[PARAM_DETAILING]<0)
			{
				ImageStudio::Transforms::Core::BGRA::BGRA_IPPEffectGaussianBlur2(imData, wdth,hght, 1.0, (long)(-m_parameters[PARAM_DETAILING]/10.0/paramCoef));
			}
			else
				if (m_parameters[PARAM_DETAILING]>0)
				{
					ImageStudio::Transforms::Core::BGRA::BGRA_EffectSharpen(imData, wdth,hght, 1.0, m_parameters[PARAM_DETAILING]/paramCoef/paramCoef);
				}
		}

		if ((m_parameters[PARAM_VIGN_AMOUNT]!=0 || 
			m_parameters[PARAM_VIGN_MIDPOINT]!=0 ||
			m_parameters[PARAM_VIGN_ROUNDNESS]!=0 ||
			m_parameters[PARAM_VIGN_FEATHER]!=0 ) /*&& !m_exit_thread*/)
			{
				/*double dCenterX = wdth/2;
				double dCenterY = hght/2; 
				double dRadius1X = wdth/3;
				double dRadius1Y= hght/3;
				if (wdth>hght)
				{
					dRadius1Y = (double)m_parameters[PARAM_VIGN_MIDPOINT]/100.0*(double)hght/2.0;
					dRadius1X = dRadius1Y+(wdth/2.0-dRadius1Y)*(double)m_parameters[PARAM_VIGN_ROUNDNESS]/100.0;
				}
				else
				{
					dRadius1X = (double)m_parameters[PARAM_VIGN_MIDPOINT]/100.0*(double)wdth/2.0;
					dRadius1Y = dRadius1X+(hght/2.0-dRadius1X)*(double)m_parameters[PARAM_VIGN_ROUNDNESS]/100.0;
				}
				double dRadius2X= dRadius1X+(double)m_parameters[PARAM_VIGN_FEATHER]*(wdth/2.0-dRadius1X)/100.0;
				double dRadius2Y= dRadius1Y+(double)m_parameters[PARAM_VIGN_FEATHER]*(hght/2.0-dRadius1Y)/100.0;
				double dIntensity = ((double)m_parameters[PARAM_VIGN_AMOUNT])/100.0;
				
				ImageStudio::Transforms::Core::BGRA::BGRA_EffectVignette(imData, wdth,hght, 1.0,  dCenterX, dCenterY, 
					dRadius1X,dRadius1Y,dRadius2X,dRadius2Y,0,	dIntensity);*/
				ImageStudio::Transforms::Core::BGRA::BGRA_EffectPostCrop(imData, wdth,hght, 1.0,m_parameters[PARAM_VIGN_AMOUNT],m_parameters[PARAM_VIGN_MIDPOINT],
					m_parameters[PARAM_VIGN_ROUNDNESS],m_parameters[PARAM_VIGN_FEATHER]);
			}
		if((m_parameters[PARAM_INSIDE_WB_R]>-1 || m_parameters[PARAM_INSIDE_WB_G]>-1 || m_parameters[PARAM_INSIDE_WB_B]>-1) /*&& !m_exit_thread*/)
		{
			BGRA_AdjustWhiteBalance( imData, wdth,hght, 1.0, 
				m_parameters[PARAM_INSIDE_WB_R], m_parameters[PARAM_INSIDE_WB_G], m_parameters[PARAM_INSIDE_WB_B]);
		}
		if((m_parameters[PARAM_INSIDE_WB_R]==-1 && m_parameters[PARAM_INSIDE_WB_G]==-1 && m_parameters[PARAM_INSIDE_WB_B]==-1) /*&& !m_exit_thread*/)
		{
			long  fastW, fastH;
			BYTE* fastData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST, fastW,fastH);
			AdjustAutoWB(fastData, fastW, fastH, imData, wdth,hght);
		}

		//FINISH_PERFORMANCE_CHECK(1);

		
		//if (!m_exit_thread)
			GetAppManager()->SendMsg(WM_IEM_THREAD_FINISHED,0);
		return 0;
	}

	void  AfterThreadProc()
	{
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangeImage(ILayer::SLOW_COPY,this);
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->SetActualImage(ILayer::SLOW_COPY);
		GetAppManager()->GetView()->InvalidateView();
		SwitchViewToSlow();
	}

	virtual void ChangesHandler(int changingObject, bool repaint)
	{
		if (changingObject != ILayer::FAST_COPY)
		{
			//m_exit_thread = true;
			GetAppManager()->GetLayersManager()->GetCurrentLayer()->TryUsingSlowFunction(NULL);

			long wdth, hght;
			BYTE* imData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST_COPY, wdth,hght);
			//m_exit_thread = false;
			AdjustAll(imData, wdth, hght);

			if (repaint)
			{
				GetAppManager()->GetLayersManager()->GetCurrentLayer()->SetActualImage(ILayer::FAST_COPY);
				SwitchViewToFast();
				GetAppManager()->GetView()->InvalidateView();
			}

			//m_exit_thread = false;
			GetAppManager()->GetLayersManager()->GetCurrentLayer()->TryUsingSlowFunction(this);
		}
	}

public:	
	void ApplyToImage(BYTE* pPixels, int nWidth, int nHeight, bool scaleParams = true)
	{
		//bool  et = m_exit_thread;
		//m_exit_thread = false;
		AdjustAll(pPixels, nWidth, nHeight, scaleParams);
		//et = m_exit_thread;
	}
	bool IsBackgroundCommandOnFullImage() 
	{
		if ((m_parameters[PARAM_VIGN_AMOUNT]!=0 || 
			m_parameters[PARAM_VIGN_MIDPOINT]!=0 ||
			m_parameters[PARAM_VIGN_ROUNDNESS]!=0 ||
			m_parameters[PARAM_VIGN_FEATHER]!=0) ||
			m_parameters[PARAM_SC_CLARITY]<-30)
				return true;
		return false;
	}


	BOOL    isInteractive() const {return m_pick_wthite_balance_point;}
	BOOL    m_onImage;
private:

	int    m_parameters[PARAMS_COUNT];

	//TablesCalculator   m_tables_calculator;
	
	BYTE TableRGB[3][256];
	BYTE TableBright[256];
	BYTE TableContrast[256];
	BYTE TableTemperatureRGB[3][256];
	BYTE TableTintRGB[3][256];

	BYTE TablePosterize[256];
	
	BYTE TableExposure[256];
	int TableBlacks[256];
	int TableFillLight[256];
	//int  TableRecovery[100][256];

	double m_SaturationLevel;
	double m_RecoveryFactor;
	double m_VibranceFactor;

	POINT m_current_point;
	BOOL  m_pick_wthite_balance_point;
	long  m_pick_colors[3*PICK_AREA_MAX_SIZE*PICK_AREA_MAX_SIZE];

	void AdjustAll(BYTE* pPixels, int nWidth, int nHeight, bool scaleParams = true)
	{
		CalcColorTables();

		const int SMALL_CYCLE = 2048;

		BYTE* startPixels = pPixels;

		// apply tables
		for( int nCountPixels = nWidth * nHeight; nCountPixels > 0; nCountPixels -= SMALL_CYCLE, pPixels += 4 * SMALL_CYCLE )
		{
			//if (m_exit_thread)
			//	return;

			int nRowSize = min( SMALL_CYCLE, nCountPixels );

			if( m_parameters[PARAM_BRIGHTNESS]    ) ApplyBrightness_RowBGRX( pPixels, nRowSize );
			if( m_parameters[PARAM_CONTRAST]      ) ApplyContrast_RowBGRX( pPixels, nRowSize );
			if( m_parameters[PARAM_SC_SATURATION] ) ApplySaturation_RowBGRX( pPixels, nRowSize );
			if( m_parameters[PARAM_TEMPERATURE]   ) ApplyTemperature_RowBGRX( pPixels, nRowSize );
			if( m_parameters[PARAM_TINT]          ) ApplyTint_RowBGRX( pPixels, nRowSize );
			if( m_parameters[PARAM_BLACK]         ) ApplyBlack_RowBGRX( pPixels, nRowSize );
			if( m_parameters[PARAM_LIGHT]         ) ApplyLight_RowBGRX( pPixels, nRowSize );
			if( m_parameters[PARAM_RECOVERY]      ) ApplyRecovery_RowBGRX( pPixels, nRowSize );
			if( m_parameters[PARAM_EXPOSURE]      ) ApplyExposure_RowBGRX( pPixels, nRowSize );
			if( m_parameters[PARAM_RED] ||
				m_parameters[PARAM_GREEN] ||
				m_parameters[PARAM_BLUE]          ) ApplyRGB_RowBGRX( pPixels, nRowSize );
			if( m_parameters[PARAM_SC_VIBRANCE]   ) ApplyVibrance_RowBGRX( pPixels, nRowSize );
		}

		//if (m_exit_thread)
		//	return;

		if (m_parameters[PARAM_SC_CLARITY]!=0 )
		{
			ImageStudio::Transforms::Core::BGRA::BGRA_AdjustClarity(startPixels, nWidth,nHeight, 1.0,m_parameters[PARAM_SC_CLARITY], 30, 10);
		}

		if (m_parameters[PARAM_DETAILING]!=0 )
		{
		    pPixels = startPixels;
			long wdth, hght;
			BYTE* tmp_imData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW_COPY, wdth,hght);
			float paramCoef = (scaleParams)?((float)wdth/(float)nWidth):1.0f;
			if (m_parameters[PARAM_DETAILING]<0)
			{
			   ImageStudio::Transforms::Core::BGRA::BGRA_IPPEffectGaussianBlur2(pPixels, nWidth,nHeight, 1.0, (long)(-m_parameters[PARAM_DETAILING]/10.0/paramCoef));
			}
			else
				if (m_parameters[PARAM_DETAILING]>0)
				{
					ImageStudio::Transforms::Core::BGRA::BGRA_EffectSharpen(pPixels, nWidth,nHeight, 1.0, m_parameters[PARAM_DETAILING]/paramCoef/paramCoef);
				}
		}

		if( m_parameters[PARAM_VIGN_AMOUNT] || 
			m_parameters[PARAM_VIGN_MIDPOINT] ||
			m_parameters[PARAM_VIGN_ROUNDNESS] ||
			m_parameters[PARAM_VIGN_FEATHER] )
		{
			/*double dCenterX = nWidth/2;
			double dCenterY = nHeight/2; 
			double dRadius1X = nWidth/3;
			double dRadius1Y = nHeight/3;
			if (nWidth>nHeight)
			{
				dRadius1Y = (double)m_parameters[PARAM_VIGN_MIDPOINT]/100.0*(double)nHeight/2.0;
				dRadius1X = dRadius1Y+(nWidth/2.0-dRadius1Y)*(double)m_parameters[PARAM_VIGN_ROUNDNESS]/100.0;
			}
			else
			{
				dRadius1X = (double)m_parameters[PARAM_VIGN_MIDPOINT]/100.0*(double)nWidth/2.0;
				dRadius1Y = dRadius1X+(nHeight/2.0-dRadius1X)*(double)m_parameters[PARAM_VIGN_ROUNDNESS]/100.0;
			}
			double dRadius2X= dRadius1X+(double)m_parameters[PARAM_VIGN_FEATHER]*(nWidth/2.0-dRadius1X)/100.0;
			double dRadius2Y= dRadius1Y+(double)m_parameters[PARAM_VIGN_FEATHER]*(nHeight/2.0-dRadius1Y)/100.0;
			double dIntensity = ((double)m_parameters[PARAM_VIGN_AMOUNT])/100.0;
			
			ImageStudio::Transforms::Core::BGRA::BGRA_EffectVignette(startPixels, nWidth,nHeight, 1.0,  dCenterX, dCenterY, 
				dRadius1X,dRadius1Y,dRadius2X,dRadius2Y,0,	dIntensity);*/

			ImageStudio::Transforms::Core::BGRA::BGRA_EffectPostCrop(startPixels, nWidth,nHeight, 1.0,m_parameters[PARAM_VIGN_AMOUNT],m_parameters[PARAM_VIGN_MIDPOINT],
				m_parameters[PARAM_VIGN_ROUNDNESS],m_parameters[PARAM_VIGN_FEATHER]);
		}

		if( m_parameters[PARAM_INSIDE_WB_R]>-1 || m_parameters[PARAM_INSIDE_WB_G]>-1 || m_parameters[PARAM_INSIDE_WB_B]>-1 )
		{
			 BGRA_AdjustWhiteBalance( startPixels, nWidth,nHeight, 1.0, 
				 m_parameters[PARAM_INSIDE_WB_R], m_parameters[PARAM_INSIDE_WB_G], m_parameters[PARAM_INSIDE_WB_B]);
		}
		if( m_parameters[PARAM_INSIDE_WB_R]==-1 && m_parameters[PARAM_INSIDE_WB_G]==-1 && m_parameters[PARAM_INSIDE_WB_B]==-1 )
		{
			long  fastW, fastH;
			BYTE* fastData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST, fastW,fastH);
			AdjustAutoWB(fastData, fastW, fastH, startPixels, nWidth,nHeight);
		}


		/**/
		
	}

public:
	bool LoadState(const CString& filePath)
	{
		XmlUtils::CXmlReader oXmlReader;
		if (!oXmlReader.OpenFromFile(CString(filePath)))
			return false;

		if (!oXmlReader.ReadRootNode()) 
			return false;

		//StartChangingParamsGroup();
		
		m_parameters[PARAM_RED] = 			XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("Red"))); 
		m_parameters[PARAM_GREEN] = 		XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("Green")));    
		m_parameters[PARAM_BLUE] = 			XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("Blue")));               
		m_parameters[PARAM_HUE] = 			XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("Hue")));              
		m_parameters[PARAM_SATURATION] = 	XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("Saturation")));        
		m_parameters[PARAM_POSTERIZE] = 	XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("Posterize")));         
		m_parameters[PARAM_TEMPERATURE] = 	XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("Temperature")));       
		m_parameters[PARAM_TINT] = 			XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("Tint")));       
		m_parameters[PARAM_BLACK] = 		XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("Black")));			
		m_parameters[PARAM_LIGHT] = 		XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("FillLight")));		
		m_parameters[PARAM_RECOVERY] = 		XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("Recovery")));			
		m_parameters[PARAM_EXPOSURE] =    XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("Explosure")));			
		m_parameters[PARAM_BRIGHTNESS] =    XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("Brightness")));			
		m_parameters[PARAM_CONTRAST] = 		XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("Contrast")));	
		m_parameters[PARAM_DETAILING] = 	XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("Detailing")));			
		m_parameters[PARAM_WB_SIZE] = XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("WhiteBalancePickAreaSize"))); 
		m_parameters[PARAM_WB_PRESET] = 	XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("WhiteBalancePreset"))); 
		m_parameters[PARAM_VIGN_AMOUNT] = 	XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("VignetteAmount")));			
		m_parameters[PARAM_VIGN_MIDPOINT] = XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("VignetteMidPoint")));			
		m_parameters[PARAM_VIGN_ROUNDNESS] =  XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("VignetteRoundness")));				
		m_parameters[PARAM_VIGN_FEATHER] =  XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("VignetteFeather")));				
		m_parameters[PARAM_SC_CLARITY] = 	XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("SmartClarity")));			
		m_parameters[PARAM_SC_VIBRANCE] = 	XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("SmartVibrance")));			
		m_parameters[PARAM_SC_SATURATION] =  XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("SmartSaturation")));			
		m_parameters[PARAM_AUTO_TONE_CORR] =  XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("AutoTone")));			
		m_parameters[PARAM_AUTO_BR_CONTR] =  XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("AutoBrContr")));	

		m_parameters[PARAM_INSIDE_WB_R] =  XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("WhiteBalanceRed")));
		m_parameters[PARAM_INSIDE_WB_G] =  XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("WhiteBalanceGreen")));
		m_parameters[PARAM_INSIDE_WB_B] =  XmlUtils::GetInteger(oXmlReader.ReadNodeValue(_T("WhiteBalanceBlue")));


		for (int i=0; i<PARAMS_COUNT;i++)
			FireChangeParameter(i, (int)m_parameters[i]);

		if (m_parameters[PARAM_AUTO_TONE_CORR])
		{
			double autoPost=0.0;
			double autoSatur=0.0;
			double autoExpl=0.0;

			long wdth, hght;
			BYTE* imData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST, wdth,hght);
			ImageStudio::Transforms::Core::BGRA::BGRA_GetAutoPosterize(imData, wdth,hght, &autoPost);
			ImageStudio::Transforms::Core::BGRA::BGRA_GetAutoSaturation(imData, wdth,hght, &autoSatur);
			ImageStudio::Transforms::Core::BGRA::BGRA_GetAutoHue(imData, wdth,hght, &autoExpl);

			autoExpl/=100.0;

			m_parameters[PARAM_POSTERIZE] = (int)autoPost;
			m_parameters[PARAM_SC_SATURATION] = (int)autoSatur;
			m_parameters[PARAM_EXPOSURE] = (int)autoExpl;

			FireChangeParameter(PARAM_POSTERIZE, (int)m_parameters[PARAM_POSTERIZE]);
			FireChangeParameter(PARAM_SC_SATURATION, (int)m_parameters[PARAM_SC_SATURATION]);
			FireChangeParameter(PARAM_EXPOSURE, (int)m_parameters[PARAM_EXPOSURE]);

		}
		if (m_parameters[PARAM_AUTO_BR_CONTR])
		{
			double autoBr=0.0;
			double autoContr=0.0;

			long wdth, hght;
			BYTE* imData = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST, wdth,hght);
			ImageStudio::Transforms::Core::BGRA::BGRA_GetAutoBrightness(imData, wdth,hght, &autoBr);
			ImageStudio::Transforms::Core::BGRA::BGRA_GetAutoContrast(imData, wdth,hght, &autoContr);

			m_parameters[PARAM_BRIGHTNESS] = (int)autoBr;
			m_parameters[PARAM_CONTRAST] = (int)autoContr;

			FireChangeParameter(PARAM_BRIGHTNESS, (int)m_parameters[PARAM_BRIGHTNESS]);
			FireChangeParameter(PARAM_CONTRAST, (int)m_parameters[PARAM_CONTRAST]);
		}

		//FinishChangingParamsGroup();

		return true;
	}
	bool SaveState(const CString& filePath)
	{
		// save all presets to file
		XmlUtils::CXmlWriter xW;

		// open presets section
		xW.WriteNodeBegin(_T("EditorState"));

		xW.WriteNodeBegin(_T("Red"));                xW.WriteInteger(m_parameters[PARAM_RED]);			xW.WriteNodeEnd(_T("Red"));                xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("Green"));              xW.WriteInteger(m_parameters[PARAM_GREEN]);		xW.WriteNodeEnd(_T("Green"));              xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("Blue"));               xW.WriteInteger(m_parameters[PARAM_BLUE]);			xW.WriteNodeEnd(_T("Blue"));               xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("Hue"));                xW.WriteInteger(m_parameters[PARAM_HUE]);			xW.WriteNodeEnd(_T("Hue"));                xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("Saturation"));         xW.WriteInteger(m_parameters[PARAM_SATURATION]);	xW.WriteNodeEnd(_T("Saturation"));         xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("Posterize"));          xW.WriteInteger(m_parameters[PARAM_POSTERIZE]);	xW.WriteNodeEnd(_T("Posterize"));          xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("Temperature"));        xW.WriteInteger(m_parameters[PARAM_TEMPERATURE]);	xW.WriteNodeEnd(_T("Temperature"));        xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("Tint"));               xW.WriteInteger(m_parameters[PARAM_TINT]);	        xW.WriteNodeEnd(_T("Tint"));                xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("Black"));              xW.WriteInteger(m_parameters[PARAM_BLACK]);		xW.WriteNodeEnd(_T("Black"));				xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("FillLight"));			 xW.WriteInteger(m_parameters[PARAM_LIGHT]);		xW.WriteNodeEnd(_T("FillLight"));			xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("Recovery"));			 xW.WriteInteger(m_parameters[PARAM_RECOVERY]);		xW.WriteNodeEnd(_T("Recovery"));			xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("Explosure"));			 xW.WriteInteger(m_parameters[PARAM_EXPOSURE]);   xW.WriteNodeEnd(_T("Explosure"));			xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("Brightness"));		 xW.WriteInteger(m_parameters[PARAM_BRIGHTNESS]);   xW.WriteNodeEnd(_T("Brightness"));			xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("Contrast"));			 xW.WriteInteger(m_parameters[PARAM_CONTRAST]);		xW.WriteNodeEnd(_T("Contrast"));		xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("Detailing"));			 xW.WriteInteger(m_parameters[PARAM_DETAILING]);	xW.WriteNodeEnd(_T("Detailing"));			xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("WhiteBalancePickAreaSize")); xW.WriteInteger(m_parameters[PARAM_WB_SIZE]);xW.WriteNodeEnd(_T("WhiteBalancePickAreaSize")); xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("WhiteBalancePreset")); xW.WriteInteger(m_parameters[PARAM_WB_PRESET]);	xW.WriteNodeEnd(_T("WhiteBalancePreset")); xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("VignetteAmount"));	 xW.WriteInteger(m_parameters[PARAM_VIGN_AMOUNT]);	xW.WriteNodeEnd(_T("VignetteAmount"));			xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("VignetteMidPoint"));	 xW.WriteInteger(m_parameters[PARAM_VIGN_MIDPOINT]);xW.WriteNodeEnd(_T("VignetteMidPoint"));			xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("VignetteRoundness"));  xW.WriteInteger(m_parameters[PARAM_VIGN_ROUNDNESS]); xW.WriteNodeEnd(_T("VignetteRoundness"));				xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("VignetteFeather"));    xW.WriteInteger(m_parameters[PARAM_VIGN_FEATHER]); xW.WriteNodeEnd(_T("VignetteFeather"));				xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("SmartClarity"));	     xW.WriteInteger(m_parameters[PARAM_SC_CLARITY]);	xW.WriteNodeEnd(_T("SmartClarity"));			xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("SmartVibrance"));		 xW.WriteInteger(m_parameters[PARAM_SC_VIBRANCE]);	xW.WriteNodeEnd(_T("SmartVibrance"));			xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("SmartSaturation"));	 xW.WriteInteger(m_parameters[PARAM_SC_SATURATION]); xW.WriteNodeEnd(_T("SmartSaturation"));			xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("AutoTone"));			 xW.WriteInteger(m_parameters[PARAM_AUTO_TONE_CORR]); xW.WriteNodeEnd(_T("AutoTone"));			xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("AutoBrContr"));		 xW.WriteInteger(m_parameters[PARAM_AUTO_BR_CONTR]); xW.WriteNodeEnd(_T("AutoBrContr"));			xW.WriteString(_T("\n\n"));

		xW.WriteNodeBegin(_T("WhiteBalanceRed"));	 xW.WriteInteger(m_parameters[PARAM_INSIDE_WB_R]); xW.WriteNodeEnd(_T("WhiteBalanceRed"));			xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("WhiteBalanceGreen"));	 xW.WriteInteger(m_parameters[PARAM_INSIDE_WB_G]); xW.WriteNodeEnd(_T("WhiteBalanceGreen"));		xW.WriteString(_T("\n\n"));
		xW.WriteNodeBegin(_T("WhiteBalanceBlue"));	 xW.WriteInteger(m_parameters[PARAM_INSIDE_WB_B]); xW.WriteNodeEnd(_T("WhiteBalanceBlue"));			xW.WriteString(_T("\n\n"));
	
		xW.WriteNodeEnd(_T("EditorState"));

		// write composed xml string to the file
		xW.SaveToFile(filePath, TRUE);

		return true;
	}

private:
	//struct  threadData
	//{
	//	BYTE*              imStart;
	//	int                imW;
	//	int                imH;
	//	int                imRowsShift;
	//	TablesCalculator*  tblsCalc;
	//	CBckgTonesCommand* command;
	//	threadData(BYTE* imageStart, int imageW, int imageH, int imageRowsShift, TablesCalculator* tCalc, CBckgTonesCommand* cmnd)
	//	{
	//		imStart = imageStart;
	//		imW = imageW;
	//		imH = imageH;
	//		imRowsShift = imageRowsShift;
	//		tblsCalc = tCalc;
	//		command = cmnd;
	//	}
	//};

	//static DWORD WINAPI ThreadFunc ( void* pv )
	//{
	//	threadData *pTD = (threadData *)pv;
	//	pTD->command->AdjustAll(pTD->imStart, pTD->imW, pTD->imH, pTD->imRowsShift, pTD->tblsCalc);
	//	return 0;
	//}

	bool isParametersChanges()
	{
		if (m_parameters[PARAM_BRIGHTNESS]!=0 ||
			m_parameters[PARAM_CONTRAST]!=0 ||
			m_parameters[PARAM_SC_SATURATION]!=0 ||
			m_parameters[PARAM_TEMPERATURE]!=0 ||
			m_parameters[PARAM_BLACK]!=0 ||
			m_parameters[PARAM_LIGHT]!=0 ||
			m_parameters[PARAM_RECOVERY]!=0 ||
			m_parameters[PARAM_EXPOSURE]!=0 ||
			m_parameters[PARAM_BLUE]!=0 ||
			m_parameters[PARAM_GREEN]!=0 ||
			m_parameters[PARAM_RED]!=0 ||
			m_parameters[PARAM_DETAILING]!=0 ||
			m_parameters[PARAM_VIGN_AMOUNT]!=0 || 
			m_parameters[PARAM_VIGN_MIDPOINT]!=0 ||
			m_parameters[PARAM_VIGN_ROUNDNESS]!=0 ||
			m_parameters[PARAM_VIGN_FEATHER]!=0 ||
			(m_parameters[PARAM_INSIDE_WB_R]>-2 || m_parameters[PARAM_INSIDE_WB_G]>-2 || m_parameters[PARAM_INSIDE_WB_B]>-2) ||
			m_parameters[PARAM_SC_VIBRANCE]!=0 ||
			m_parameters[PARAM_SC_CLARITY]!=0 ||
			m_parameters[PARAM_TINT]!=0)
				return true;
		return false;

	}
	void CalcColorTables()
	{
		// calculate levels
		int LevelParam[3] = {m_parameters[PARAM_RED], m_parameters[PARAM_GREEN], m_parameters[PARAM_BLUE]};
		for( int n = 0; n < 3; n++ )
		{
			if( LevelParam[n] )
			{
				double level = 1 + ((LevelParam[n] > 0) ? (LevelParam[n] * 4 / 100.0) : (LevelParam[n] * 8 / 1000.0));
				level = 1 / level;

				for( int i = 0; i < 256; i++ )
				{
					int color = (int)((255.0 * pow( (double)ByteDiv255[i], level )) + 0.5);
					TableRGB[n][i] = (BYTE)min(255, color);
				}
			}
			else
			{
				for( int i = 0; i < 256; i++ )
				{
					TableRGB[n][i] = i;
				}
			}
		}
		
		// compute brightness levels
		if( m_parameters[PARAM_BRIGHTNESS] )
		{
			long BrightnessLevel = max( -255, min( 255, m_parameters[PARAM_BRIGHTNESS]));

			for( int i = 0; i < 256; i++ )
			{
				TableBright[i] = BYTE(max(0, min( 255, i + BrightnessLevel)));
			}
		}

		// compute contrast level
		if( m_parameters[PARAM_CONTRAST] )
		{
			double ContrastLevel = m_parameters[PARAM_CONTRAST] / 100.0;
			if( ContrastLevel + 1 > 0 )
				ContrastLevel = pow( 255, ContrastLevel );
			else
				ContrastLevel += 1;

			for (int i = 0; i < 256; ++i)
			{
				TableContrast[i] = BYTE(max(0, min(255, int(127.5 + (i - 127.5)*ContrastLevel))));
			}
		}
		
		if( m_parameters[PARAM_TEMPERATURE] )
		{
			double dDeltaTemperatureRed   = 0.3  * ((double)m_parameters[PARAM_TEMPERATURE]);
			double dDeltaTemperatureGreen = 0.05 * ((double)m_parameters[PARAM_TEMPERATURE]);
			double dDeltaTemperatureBlue  = -0.2 * ((double)m_parameters[PARAM_TEMPERATURE]);

			for (int i = 0; i < 256; ++i)
			{
				TableTemperatureRGB[0][i] = ImageStudio::Transforms::Core::BGRA::Byte(i + dDeltaTemperatureRed);
				TableTemperatureRGB[1][i] = ImageStudio::Transforms::Core::BGRA::Byte(i + dDeltaTemperatureGreen);
				TableTemperatureRGB[2][i] = ImageStudio::Transforms::Core::BGRA::Byte(i + dDeltaTemperatureBlue);
			}
		}

		if( m_parameters[PARAM_TINT] )
		{
			double dDeltaTintRed   = -0.1  * ((double)m_parameters[PARAM_TINT]);
			double dDeltaTintGreen = -0.3 * ((double)m_parameters[PARAM_TINT]);
			double dDeltaTintBlue  = 0.2 * ((double)m_parameters[PARAM_TINT]);

			for (int i = 0; i < 256; ++i)
			{
				TableTintRGB[0][i] = ImageStudio::Transforms::Core::BGRA::Byte(i + dDeltaTintRed);
				TableTintRGB[1][i] = ImageStudio::Transforms::Core::BGRA::Byte(i + dDeltaTintGreen);
				TableTintRGB[2][i] = ImageStudio::Transforms::Core::BGRA::Byte(i + dDeltaTintBlue);
			}
		}

		m_SaturationLevel = 1 + ((double)(min(max(-100, m_parameters[PARAM_SC_SATURATION]), 100)))/100.0;
		m_RecoveryFactor = ((double)m_parameters[PARAM_RECOVERY]) / 255000.0;
		m_VibranceFactor = m_parameters[PARAM_SC_VIBRANCE]/100.0;

		if( m_parameters[PARAM_EXPOSURE] )
		{
			int exposure = min( 99, max( -100, m_parameters[PARAM_EXPOSURE]));

			for (int i = 0; i < 256; ++i)
			{
				double val = ByteDiv255[i];
				
				if( val <= 0.04045 )
					val *= (1/12.92); 
				else 
					val = pow( (val + 0.055) * (1/1.055), 2.4);
				
				val *= pow(2, exposure * (1/25.0));
				
				if( val <= 0.0031308 ) 
					val *= 12.92; 
				else 
					val = 1.055 * pow( val, (1/2.4) ) - 0.055;
				
				TableExposure[i] = ImageStudio::Transforms::Core::BGRA::Byte( 255 * val );
			}
		}

		if( m_parameters[PARAM_BLACK] )
		{
			int black = max(0, min( 99, m_parameters[PARAM_BLACK] * 2 ) );

			for (int i = 0; i < 256; ++i)
			{
				double dBlack = (255.0/100.0) * black;
				int offset = 0;
				if( i > dBlack )
				{
					offset = int(255 * (i - dBlack) / (256 - dBlack) + 0.5);
				}
				TableBlacks[i] = offset - i;
			}
		}

		if( m_parameters[PARAM_LIGHT] )
		{
			int light = max(0, min( 256, m_parameters[PARAM_LIGHT] ) );

			for (int i = 0; i < 256; ++i)
			{
				double dFactor = (1024.0 / 255.0) * light;
				double dOffset = 1024 + dFactor;
				dFactor /= 255.0;
				TableFillLight[i] = ImageStudio::Round( dOffset - i * dFactor );
			}
		}


		//double PosterizeKoef = ((double)m_parameters[PARAM_POSTERIZE] - 1.0) / 255.0;
	}
	
	/************************************************************************/
	/* UNDO-REDO functionality                                              */
	/************************************************************************/
private:
	int    m_backup_parameters[PARAMS_COUNT];
	BOOL   m_was_backuped;

	//int    m_undo_cntr;

	//bool   m_exit_thread;

	BOOL   m_changing_group;

	struct   HISTORY_PAIR
	{
		int   m_value;
		int   m_index;
		HISTORY_PAIR()
		{
			m_value = -1;
			m_index  = -1;
		}
	};

	CAtlList< std::vector<HISTORY_PAIR> >     m_undo_parameters;
	CAtlList< std::vector<HISTORY_PAIR> >     m_redo_parameters;

	void  BackupParameters()
	{
		if (!m_was_backuped)
		{
			memcpy(m_backup_parameters, m_parameters, PARAMS_COUNT* sizeof(int));
			m_was_backuped = TRUE;
		}
	}

	void  PushChagesToUndo()
	{
		if (!m_was_backuped)
		{
			ATLASSERT(0);
			return;
		}
		std::vector<HISTORY_PAIR>   tmpArr;
		for (int i=0;i<PARAMS_COUNT;i++)
		{
			if (m_parameters[i]!=m_backup_parameters[i])
			{
				HISTORY_PAIR tmpPair;
				tmpPair.m_index = i;
				tmpPair.m_value = m_backup_parameters[i];
				tmpArr.push_back(tmpPair);
			}
		}
		if (tmpArr.size()>0)
		{
			m_undo_parameters.AddTail(tmpArr);
			m_redo_parameters.RemoveAll();
			m_was_backuped = FALSE;
		}

	}

	void  PushParamsToUndo()
	{
		std::vector<HISTORY_PAIR>   tmpArr;
		for (int i=0;i<PARAMS_COUNT;i++)
		{
			HISTORY_PAIR tmpPair;
			tmpPair.m_index = i;
			tmpPair.m_value = m_parameters[i];
			tmpArr.push_back(tmpPair);
		}
		m_undo_parameters.AddTail(tmpArr);
	}

	void  PushParamsToRedo()
	{
		std::vector<HISTORY_PAIR>   tmpArr;
		for (int i=0;i<PARAMS_COUNT;i++)
		{
			HISTORY_PAIR tmpPair;
			tmpPair.m_index = i;
			tmpPair.m_value = m_parameters[i];
			tmpArr.push_back(tmpPair);
		}
		m_redo_parameters.AddHead(tmpArr);
	}



	// apply operations
	void ApplyBrightness_RowBGRX( BYTE* pPixels, int nCountPixels ) const
	{
		for( int i = 0; i < nCountPixels; ++i, pPixels += 4 )
		{
			pPixels[0] = TableBright[pPixels[0]];
			pPixels[1] = TableBright[pPixels[1]];
			pPixels[2] = TableBright[pPixels[2]];
		}
	}
	void ApplyContrast_RowBGRX( BYTE* pPixels, int nCountPixels ) const
	{
		for( int i = 0; i < nCountPixels; ++i, pPixels += 4 )
		{
			pPixels[0] = TableContrast[pPixels[0]];
			pPixels[1] = TableContrast[pPixels[1]];
			pPixels[2] = TableContrast[pPixels[2]];
		}
	}

	void ApplySaturation_RowBGRX( BYTE* pPixels, int nCountPixels ) const
	{
		double dColor[3];

		for( int i = 0; i < nCountPixels; ++i, pPixels += 4 )
		{
			ImageStudio::Transforms::Core::BGRA::BgrToHls( pPixels, dColor );
			// compute new saturation value
			dColor[2] *= m_SaturationLevel;
			// clamp value
			if( dColor[2] > 1 ) dColor[2] = 1; 
			else
			if( dColor[2] < 0 ) dColor[2] = 0;
			// compute new color components
			ImageStudio::Transforms::Core::BGRA::HlsToBgr( dColor, pPixels );
		}
	}

	void ApplyTemperature_RowBGRX( BYTE* pPixels, int nCountPixels ) const
	{
		for( int i = 0; i < nCountPixels; ++i, pPixels += 4 )
		{
			pPixels[0] = TableTemperatureRGB[2][pPixels[0]];
			pPixels[1] = TableTemperatureRGB[1][pPixels[1]];
			pPixels[2] = TableTemperatureRGB[0][pPixels[2]];
		}
	}

	void ApplyTint_RowBGRX( BYTE* pPixels, int nCountPixels ) const
	{
		for( int i = 0; i < nCountPixels; ++i, pPixels += 4 )
		{
			pPixels[0] = TableTintRGB[2][pPixels[0]];
			pPixels[1] = TableTintRGB[1][pPixels[1]];
			pPixels[2] = TableTintRGB[0][pPixels[2]];
		}
	}

	void ApplyBlack_RowBGRX( BYTE* pPixels, int nCountPixels ) const
	{
		for( int i = 0; i < nCountPixels; ++i, pPixels += 4 )
		{
			int offset = TableBlacks[ImageStudio::Transforms::Core::BGRA::Intensity2( pPixels[2], pPixels[1], pPixels[0] )];

			pPixels[0] = max(pPixels[0] + offset, 0);
			pPixels[1] = max(pPixels[1] + offset, 0);
			pPixels[2] = max(pPixels[2] + offset, 0);
		}
	}

	void ApplyLight_RowBGRX(BYTE* pPixels, int nCountPixels ) const
	{
		for( int i = 0; i < nCountPixels; ++i, pPixels += 4 )
		{
			//ImageStudio::Transforms::Core::BGRA::BGRA_AdjustFillLight(pPixels, 1, 1, 1.0, m_parameters[PARAM_LIGHT]);

			int nB = pPixels[0];
			int nG = pPixels[1];
			int nR = pPixels[2];

			int nK = TableFillLight[ImageStudio::Transforms::Core::BGRA::Intensity2(nR, nG, nB)];

			nR = (nR * nK + 512) >> 10;
			nG = (nG * nK + 512) >> 10;
			nB = (nB * nK + 512) >> 10;

			pPixels[0] = ImageStudio::IntToByte(nB);
			pPixels[1] = ImageStudio::IntToByte(nG);
			pPixels[2] = ImageStudio::IntToByte(nR);
		}
	}

	void ApplyRecovery_RowBGRX( BYTE* pPixels, int nCountPixels ) const
	{
		for( int i = 0; i < nCountPixels; ++i, pPixels += 4 )
		{
			//ImageStudio::Transforms::Core::BGRA::BGRA_AdjustRecovery(pPixels, 1, 1, 1.0, m_parameters[PARAM_RECOVERY]);

			//double nB = pPixels[0];
			//double nG = pPixels[1];
			//double nR = pPixels[2];

			//int  curParam = max(0, min( 99, m_parameters[PARAM_RECOVERY]/5 ) );

			//double nK = (double)TableRecovery[curParam][ImageStudio::Transforms::Core::BGRA::Intensity2(nR, nG, nB)];

			//nR = (nR * nK + 2048.0)/4096.0;
			//nG = (nG * nK + 2048.0)/4096.0;
			//nB = (nB * nK + 2048.0)/4096.0;

			//pPixels[0] = ImageStudio::IntToByte((int)nB);
			//pPixels[1] = ImageStudio::IntToByte((int)nG);
			//pPixels[2] = ImageStudio::IntToByte((int)nR);

			int nB = pPixels[0];
			int nG = pPixels[1];
			int nR = pPixels[2];

			double dK = 1.0 - m_RecoveryFactor * ImageStudio::Transforms::Core::BGRA::Intensity2( nR, nG, nB );

			nR = int(nR * dK + 0.5);
			nG = int(nG * dK + 0.5);
			nB = int(nB * dK + 0.5);

			pPixels[0] = ImageStudio::IntToByte(nB);
			pPixels[1] = ImageStudio::IntToByte(nG);
			pPixels[2] = ImageStudio::IntToByte(nR);
		}
	}

	void ApplyExposure_RowBGRX( BYTE* pPixels, int nCountPixels ) const
	{
		for( int i = 0; i < nCountPixels; ++i, pPixels += 4 )
		{
			pPixels[0] = TableExposure[pPixels[0]];
			pPixels[1] = TableExposure[pPixels[1]];
			pPixels[2] = TableExposure[pPixels[2]];
		}
	}

	void ApplyRGB_RowBGRX( BYTE* pPixels, int nCountPixels ) const
	{
		for( int i = 0; i < nCountPixels; ++i, pPixels += 4 )
		{
			pPixels[0] = TableRGB[2][pPixels[0]];
			pPixels[1] = TableRGB[1][pPixels[1]];
			pPixels[2] = TableRGB[0][pPixels[2]];
		}
	}

	void ApplyVibrance_RowBGRX( BYTE* pPixels, int nCountPixels ) const
	{
		double dColor[3];

		for( int i = 0; i < nCountPixels; ++i, pPixels += 4 )
		{
			// original color -> Hue-Saturation-Lighteness
			ImageStudio::Transforms::Core::BGRA::BgrToHls( pPixels, dColor );

			// compute new saturation value
			dColor[2] *= m_VibranceFactor * (1 - dColor[2]) + 1;

			// clamp value
			if( dColor[2] > 1 ) dColor[2] = 1;
			if( dColor[2] < 0 ) dColor[2] = 0;

			// compute new color components
			ImageStudio::Transforms::Core::BGRA::HlsToBgr( dColor, pPixels );
		}
	}
};


static void AdjustAutoWB( const BYTE* srcPixels, int srcWidth, int srcHeight,  BYTE* pPixels, int nWidth, int nHeight )
{
	int nSize = nWidth * nHeight;
	int srcSize = srcWidth * srcHeight;

	BYTE table[3][256];

	double dFactorB = 0;
	double dFactorG = 0;
	double dFactorR = 0;

	double dOffsetB = 0;
	double dOffsetG = 0;
	double dOffsetR = 0;

	double dSumR = 0;
	double dSumG = 0;
	double dSumB = 0;
	double dMean = 0;
	double dCountPixels = 0;

	const BYTE* src = srcPixels;

	//for( int i = 0; i < srcSize; ++i, src += 4 )
	//{
	//	int b = src[0];
	//	int g = src[1];
	//	int r = src[2];
	//	int a = src[3];

	//	if( a > 0 )
	//	{
	//		double alpha = a * (1.0/255.0);

	//		dSumB += b * alpha;
	//		dSumG += g * alpha;
	//		dSumR += r * alpha;

	//		dCountPixels += alpha;
	//	}
	//}

	for( int count = srcSize; count > 0; count -= 0x8100 )
	{
		int nRowSize = min(count, 0x8100);

		int sumB = 0;
		int sumG = 0;
		int sumR = 0;
		int sumA = 0;

		for( int i = 0; i < nRowSize; ++i, src += 4 )
		{
			int a = src[3];
			if( a > 0 )
			{
				sumB += src[0] * a;
				sumG += src[1] * a;
				sumR += src[2] * a;
				sumA += a;
			}
		}

		dSumB += (double)sumB / 255;
		dSumG += (double)sumG / 255;
		dSumR += (double)sumR / 255;
		dCountPixels += (double)sumA / 255;
	}

	if( dCountPixels < 0.001 )
		return ;

	dSumB /= dCountPixels;
	dSumG /= dCountPixels;
	dSumR /= dCountPixels;

	dMean = /*(dSumR * 0.299 + dSumG * 0.587 + dSumB * 0.114);*/ (dSumB + dSumG + dSumR) / 3;

	//dFactorB = dSumB > 0.001 ? (dMean / dSumB ) : 1;
	//dFactorG = dSumG > 0.001 ? (dMean / dSumG ) : 1;
	//dFactorR = dSumR > 0.001 ? (dMean / dSumR ) : 1;

	//if( dMean < 127.5 )
	{
		dFactorB = dSumB > 0 ? (dMean / dSumB) : 1;
		dFactorG = dSumG > 0 ? (dMean / dSumG) : 1;
		dFactorR = dSumR > 0 ? (dMean / dSumR) : 1;

		dOffsetB = 0;
		dOffsetG = 0;
		dOffsetR = 0;
	}
	//else
	//{
	//	dFactorB = (255 - dMean) / (255 - dSumB);
	//	dFactorG = (255 - dMean) / (255 - dSumG);
	//	dFactorR = (255 - dMean) / (255 - dSumR);

	//	dOffsetB = 255 - 255 * dFactorB;
	//	dOffsetG = 255 - 255 * dFactorG;
	//	dOffsetR = 255 - 255 * dFactorR;
	//}

	for( int i = 0; i < 256; ++i )
	{
		table[0][i] = RealToByte( i * dFactorB + dOffsetB );
		table[1][i] = RealToByte( i * dFactorG + dOffsetG );
		table[2][i] = RealToByte( i * dFactorR + dOffsetR );
	}

	BGRA_ApplyColorTableEx( pPixels, nWidth, nHeight, table );


	double xHistogramm[256];
	int tmpHist[256];

	for( int i = 0; i < 256; ++i )
	{
		xHistogramm[i] = 0;
		tmpHist[i] = 0;
	}

	//dSumB = 0;
	//dSumG = 0;
	//dSumR = 0;

	dCountPixels = 0;
	src = srcPixels;

	//for( int i = 0; i < srcSize; ++i, src += 4 )
	//{
	//	int b = src[0];
	//	int g = src[1];
	//	int r = src[2];
	//	int a = src[3];

	//	if( a > 0 )
	//	{
	//		int index = Intensity3( r, g, b );

	//		double alpha = a * (1.0 / 255.0);

	//		dCountPixels += alpha;
	//		xHistogramm[index] += alpha;
	//	}
	//}

	for( int count = srcSize; count > 0; count -= 0x808080 )
	{
		int nRowSize = min(count, 0x808080);
		int sumA = 0;

		for( int i = 0; i < nRowSize; ++i, src += 4 )
		{
			int a = src[3];
			if( a > 0 )
			{
				int index = Intensity3( src[2], src[1], src[0] );
				tmpHist[index] += a;
				sumA += a;
			}
		}

		for( int i = 0; i < 256; i++ )
		{
			if( tmpHist[i] )
			{
				xHistogramm[i] += (double)tmpHist[i] / 255;
				tmpHist[i] = 0;
			}
		}
		dCountPixels += (double)sumA / 255;
	}

	double dLimit = dCountPixels * 0.001;
	double dSumma = 0;
	double dCount = 0;

	for( int i = 0; i < 256; ++i )
	{
		dSumma += xHistogramm[255 - i] * (255 - i);
		dCount += xHistogramm[255 - i];

		if( dCount >= dLimit )
			break;
	}

	if( dCount < 0.001 )
		return;

	double dFactor = 255 / (dSumma / dCount);

	for( int i = 0; i < 256; ++i )
	{
		table[0][i] = RealToByte( i * dFactor );
	}

	BGRA_ApplyColorTable( pPixels, nWidth, nHeight, table[0] );	
}

#endif