#ifndef __COMMON_HEADER__
#define __COMMON_HEADER__

/************************************************************************/
/* COMMON INTERFACES                                                    */
/************************************************************************/

struct ICoordinatSystem;

struct IView
{
	virtual void           InvalidateView(bool updateData=true, LPRECT invRect = NULL)   = 0;
	virtual void           FitImageToWindow()        = 0;
	virtual Gdiplus::RectF GetImageRect()            = 0 ;
	virtual Gdiplus::Color GetBackgroundColor()      = 0 ;
	virtual Gdiplus::RectF GetWindowRect()           = 0 ;
	virtual void           EnableDraw(bool)          = 0 ;
};

struct IUndoRedoCallback
{
	virtual  void  OnUndo(int stepsCount, bool Reset)     =0;
	virtual  void  OnRedo(int stepsCount)     =0;
	virtual  void  ClearUndo()   {};
	virtual  void  ClearRedo()   {};
};

struct IChangesSubscriber
{
	virtual bool BeforeChangesHandler(int changingObject)            {return true;}   // переопредел€ть и возвращать false если не надо пересчета картинки от других команд
	virtual void ChangesHandler(int changingObject, bool repaint=true)      = 0;
};

struct ILayer
{
	typedef enum
	{
		SLOW       = 0,
		SLOW_COPY  = 1,
		FAST       = 2,
		FAST_COPY  = 3,
		THUMBNAIL  = 4,
		LAST_INDEX = 5,
		DUBLICATE  = 6
	} IMAGE_TYPE;
	// Ќе используйте эту функцию дл€ смены картинки - любые изменени€ необратимы, так как не попадут в Undo лист
	virtual        BYTE*       GetImage(IMAGE_TYPE imType, long& width, long& height)        =0; 
	virtual        void        UpdateCopy(IMAGE_TYPE imType)                                 =0;

	virtual  const BYTE*       GetActualImage(long& width, long& height, double& scaleCoef)   =0;
	virtual  IMAGE_TYPE        GetActualImage()                                              =0;
	virtual  void              SetActualImage(IMAGE_TYPE imType)                             =0;

	virtual  void              SetFreezeLayer(bool isFreeze)                                 =0;  // не будут примен€тьс€ автоматически расчитываемые измеенин€
	virtual  bool              GetFreezeLayer()								                 =0;  // не будут примен€тьс€ автоматически расчитываемые измеенин€

	virtual        void        ChangeImage(IMAGE_TYPE imType, IChangesSubscriber* fromCommand = NULL, bool repaint=true, bool useOtherSubscribers=true) = 0;
	virtual        void        InvalidateImagePart(int lft, int tp, int rght, int bttom) = 0;

	virtual        void        TryUsingSlowFunction(CBaseThread* slowThread)                          = 0;

	virtual  long*             GetHistogramm()                            =0;

	virtual  const CAtlMap<CString, double>*   GetDoublesInfos()   const         =0;
	virtual  const CAtlMap<CString, CString>*  GetStringsInfos()   const         =0;

	// Ћюбые изменени€ картинки делаютс€ через эти функции:
	
	// Ќачать блок изменений, которые будут отмен€тс€ при одном нажатии на Undo
	virtual  bool  StartHistoryBlock()   = 0;
	
	// »зменить часть картинки - 
	// imPart - буфер с новым участком
	// wdth, hgth - его размеры
	// startX, startY - положение левого верхнего угла нового куска на слое
	virtual  bool  ChangePartOfLayer(BYTE* imPart, long wdth, long hght, long startX, long startY)   = 0;  // Dont use this function without starting history block

	// »зменить часть картинки - 
	// imPart - буфер с новым участком
	// shiftX, shiftY , wdth, hgth - смещение и размеры нового учаска на большем массиве
	// scrBufferWidth - ширина большого буфера
	// startX, startY - положение левого верхнего угла нового куска на слое
	virtual  bool  ChangePartOfLayer(BYTE* imPart, long shiftSrcX, long shiftSrcY, long scrBufferWidth, long wdth, long hght, long startX, long startY)   = 0;  // Dont use this function without starting history block
	
	// »зменить полностью слой - с новыми размерами
	// imPart - буфер новым слоем
	// wdth, hgth - его размеры
	// !!!! ≈сли надо изменить слой полностью, но без изменени€ размеров - то лчше использовать предыдущую функцию!!!!
	virtual  bool  ChangeAllLayer(BYTE* imPart, long wdth, long hght)      = 0;  // Dont use this function without starting history block

	// ƒл€ отката параметров фоновой команды
	virtual  bool  ChangeManually(IUndoRedoCallback* hist_blocl)      = 0; 
	
	// «акрывает текущий блок изменений, заносит его в Undo историю, очищает Redo историю
	// block_description - это описание блока измений. Ќапример - "Line - (10,20)-(30,40)"
	virtual  bool  StopHistoryBlock(const CString& block_description)   = 0;


	/************************************************************************/
	/* ѕодписчики на ловлю изменений в слое                                 */
	/************************************************************************/
	virtual void          AddSubscriber(IChangesSubscriber* newSS)     =0;
	virtual void          RemoveSubscriber(IChangesSubscriber* SS)     =0;
};

struct ILayersManager
{
	virtual ILayer*   GetCurrentLayer()    = 0;
	virtual bool      DrawLogo(MediaCore::IAVSUncompressedVideoFrame** pMediaData, bool bPrint)   =0;

};


struct ITimerHandler
{
	virtual void OnTimer() = 0;
};
struct ITimer
{
	virtual void StartTimer(ITimerHandler* timerHdlr, int timeStepInMillisec) = 0;
	virtual void StopTimer(ITimerHandler* timerHdlr) = 0;
};

struct IProgresser
{
	virtual void StartProgress()             =0;
	virtual void Progress(LONG percent)      =0;
	virtual void FinishProgress()            = 0;
};


#define   WM_IEM_UPDATE_THUMBNAIL   WM_USER+100
#define   WM_IEM_THREAD_FINISHED    WM_USER+101
#define   WM_IEM_STOP_COMMAND_YES   WM_USER+102
#define   WM_IEM_STOP_COMMAND_NO    WM_USER+103

struct IMessageReceiver
{
	virtual void SendMsg(int msgID, void* messageData)  = 0;
};

struct IAppManager : public IMessageReceiver
{
	virtual IView*		       GetView()                  =  0;
	virtual ICoordinatSystem*  GetCoordinateTransformer() =  0;
	virtual ILayersManager*    GetLayersManager()         =  0;
	virtual ITimer*            GetTimer()                 =  0;
	virtual IProgresser*       GetProgresser()            =  0;

	virtual ImageStudio3::IImageTransformsPtr   GetImageStudio()                =  0;

	virtual void                ApplyBackgroundCommands(BYTE* pPixels, int nWidth, int nHeight, bool scaleParams = true)  =  0;
	virtual bool                IsBackgroundCommandOnFullImage()  = 0;       

	virtual void                ResizeImage(int nWidth, int nHeight)  = 0;       

	virtual void                EnableInterfaceControls(bool)                    = 0;
};

struct IParameterChangeCallback
{
	virtual void OnChangeParameter(int paramInd)  = 0;
};

#include "ViewManager.h"
#include "resource.h"

class   TrackRect
{
public:
	TrackRect(ICoordinatSystem*   cs_transform):
	   m_cs_transform(cs_transform),
		   m_container_width(0), 
		   m_container_heigth(0), 
		   m_x(0), m_y(0), m_w(0), m_h(0) , m_isVisible(false), m_track_type (eTT_None),
		   m_cursor_shift_x(0), m_cursor_shift_y(0), m_container_limits(true), m_proportions(0.0f){}

	   void  Draw(Gdiplus::Graphics* pGr)
	   {
		   if (!m_isVisible)
			   return;
		   long scrPX = m_x;
		   long scrPY = m_y;
		   long scrW = m_w;
		   long scrH = m_h;
		   m_cs_transform->ImagePointToScreenPoint(scrPX, scrPY);
		   m_cs_transform->ImageDistanceToScreenDistance(scrW);
		   m_cs_transform->ImageDistanceToScreenDistance(scrH);

		   Gdiplus::Pen	oPen ( Gdiplus::Color::White );
		   if (m_track_type!=eTT_None)
			   oPen.SetColor(Color(0xFF, 0x80, 0x80));
		   else
			   oPen.SetColor(Color(0x80, 0x80, 0x80));
		   pGr->DrawRectangle ( &oPen, scrPX,scrPY, scrW, scrH );

#define  GREP_SIZE    8

		   oPen.SetWidth(2);

		   pGr->DrawLine( &oPen, scrPX , scrPY, scrPX + GREP_SIZE, scrPY);                              // --
		   pGr->DrawLine( &oPen, scrPX , scrPY-1, scrPX, scrPY + GREP_SIZE);                            // |

		   pGr->DrawLine( &oPen, scrPX+scrW-GREP_SIZE , scrPY, scrPX+scrW+1, scrPY);                    // --
		   pGr->DrawLine( &oPen, scrPX+scrW+1 , scrPY-1, scrPX+scrW+1, scrPY + GREP_SIZE);              //   |

		   pGr->DrawLine( &oPen, scrPX+scrW-GREP_SIZE , scrPY+scrH+1, scrPX+scrW+1, scrPY+scrH+1);      //   |
		   pGr->DrawLine( &oPen, scrPX+scrW+1 , scrPY+scrH+2, scrPX+scrW+1, scrPY + scrH - GREP_SIZE);  //  __

		   pGr->DrawLine( &oPen, scrPX , scrPY+scrH+1, scrPX+ GREP_SIZE, scrPY+scrH+1);                  // |
		   pGr->DrawLine( &oPen, scrPX , scrPY+scrH+2, scrPX, scrPY + scrH - GREP_SIZE);                 //  __

		   pGr->DrawLine( &oPen, scrPX + scrW/2 - GREP_SIZE/2, scrPY, scrPX + scrW/2 + GREP_SIZE/2, scrPY);                             
		   pGr->DrawLine( &oPen, scrPX + scrW + 1, scrPY + scrH/2 - GREP_SIZE/2, scrPX + scrW + 1, scrPY + scrH/2 + GREP_SIZE/2);                             
		   pGr->DrawLine( &oPen, scrPX + scrW/2 - GREP_SIZE/2, scrPY+scrH+1, scrPX + scrW/2 + GREP_SIZE/2, scrPY+scrH+1);                             
		   pGr->DrawLine( &oPen, scrPX , scrPY + scrH/2 - GREP_SIZE/2, scrPX , scrPY + scrH/2 + GREP_SIZE/2);                             
	   }


	   void  RecalcSizes(int newContainerW, int newContainerH)
		   {
				if (m_container_width==0 || m_container_heigth==0)
				{
					m_container_width = newContainerW;
					m_container_heigth = newContainerH;
					return;
				}

				double oldDiagon = sqrt(double(m_container_width*m_container_width+m_container_heigth*m_container_heigth));
				double prop_x = (double)m_x/oldDiagon;
				double prop_y = (double)m_y/oldDiagon;
				double prop_w = (double)m_w/oldDiagon;
				double prop_h = (double)m_h/oldDiagon;

				double newDiagon = sqrt(double(newContainerW*newContainerW+newContainerH*newContainerH));

				m_x = (int)((double)newDiagon*prop_x);
				m_y = (int)((double)newDiagon*prop_y);
				m_w = (int)((double)newDiagon*prop_w);
				m_h = (int)((double)newDiagon*prop_h);

				m_container_width = newContainerW;
				m_container_heigth = newContainerH;
			}

	   void  SetPos(int newX, int newY, int newW, int newH)
	   {
		   m_x = newX;
		   m_y = newY;
		   m_w = newW;
		   m_h = newH;
	   }

	   bool OnMouseDown(LONG nX, LONG nY, LONG nButton)
	   {
		   if (!m_isVisible)
			   return false;
			if (m_track_type!=eTT_None)
			{
				long imX = nX;
				long imY = nY;

				m_cs_transform->ScreenPointToImagePoint(imX, imY);

				m_cursor_shift_x = imX-m_x;
				m_cursor_shift_y = imY-m_y;

				return true;
			}

		return false;
	   }

	   bool OnMouseUp(LONG nX, LONG nY, LONG nButton)
	   {
		   if (!m_isVisible)
			   return false;
		   if (m_track_type!=eTT_None)
		   {
			   m_cursor_shift_x = 0;
			   m_cursor_shift_y = 0;
			   return true;
		   }
		   return false;
	   }
	   bool OnMouseMove(LONG nX, LONG nY, LONG nButton)
	   {
		   if (!m_isVisible)
			   return false;

		   long imX = nX;
		   long imY = nY;

		   m_cs_transform->ScreenPointToImagePoint(imX, imY);

		   if (nButton==0)
		   {
#define  EPSILON    10

			   TRACK_TYPE newTT = eTT_None;
			   if (PointInRect(imX, imY, m_x+EPSILON, m_y+EPSILON, m_w-2*EPSILON, m_h-2*EPSILON))
				   newTT = eTT_Drag;
			   else if (PointInRect(imX, imY, m_x-EPSILON, m_y-EPSILON, m_w/3, 2*EPSILON) ||
				   PointInRect(imX, imY, m_x-EPSILON, m_y-EPSILON, 2*EPSILON, m_h/3))
				   newTT = eTT_LeftTopResize;
			   else if (PointInRect(imX, imY, m_x+m_w/3, m_y-EPSILON, m_w/3, 2*EPSILON))
				   newTT = eTT_TopResize;
			   else if (PointInRect(imX, imY, m_x+m_w-EPSILON, m_y-EPSILON, 2*EPSILON, m_h/3) ||
				   PointInRect(imX, imY, m_x+m_w-m_w/3, m_y-EPSILON, m_w/3, 2*EPSILON))
				   newTT = eTT_RigthTopResize;
			   else if (PointInRect(imX, imY, m_x+m_w-EPSILON, m_y+m_h/3, 2*EPSILON, m_h/3))
				   newTT = eTT_RigthResize;
			   else if (PointInRect(imX, imY, m_x+m_w-EPSILON, m_y+m_h-m_h/3, 2*EPSILON, m_h/3) ||
				   PointInRect(imX, imY, m_x+m_w-m_w/3, m_y+m_h-EPSILON, m_w/3, 2*EPSILON))
				   newTT = eTT_BottomRigthResize;
			   else if (PointInRect(imX, imY, m_x+m_w/3, m_y+m_h-EPSILON, m_w/3, 2*EPSILON))
				   newTT = eTT_BottomResize;
			   else if (PointInRect(imX, imY, m_x-EPSILON, m_y+m_h-m_h/3, 2*EPSILON, m_h/3) ||
				   PointInRect(imX, imY, m_x-EPSILON, m_y+m_h-EPSILON, m_w/3, 2*EPSILON))
				   newTT = eTT_LeftBottomResize;
			   else if (PointInRect(imX, imY, m_x-EPSILON, m_y+m_h/3, 2*EPSILON, m_h/3))
				   newTT = eTT_LeftResize;

			   //if (newTT!=m_track_type)
			   {
				   HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
				   switch (newTT)
				   {
				   case eTT_None:
					   break;
				   case eTT_Drag:
					   SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_HAND)));
					   break;
				   case eTT_LeftTopResize:
				   case eTT_BottomRigthResize:
					   SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE2)));
					   break;
				   case eTT_TopResize:
				   case eTT_BottomResize:
					   SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE_V)));
					   break;
				   case eTT_RigthTopResize:
				   case eTT_LeftBottomResize:
					   SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE3)));
					   break;
				   case eTT_RigthResize:
				   case eTT_LeftResize:
					   SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_MOVE_H)));
					   break;
				   }
				   m_track_type = newTT;
			   }

		   }
		   else
			   if (nButton & MK_LBUTTON)
			   {
				   bool  ret = false;
				   bool  wPropPrior = true;
				   switch (m_track_type)
				   {
				   case eTT_None:
					   break;
				   case eTT_Drag:
					   {
						   m_x = imX-m_cursor_shift_x;
						   m_y = imY-m_cursor_shift_y;
						   ret = true;
					   }
					   break;
				   case eTT_LeftTopResize:
					   {
						   int save_x = m_x;
						   int save_y = m_y;
						   m_x = imX-m_cursor_shift_x;
						   m_y = imY-m_cursor_shift_y;
						   m_w+=save_x-m_x;
						   m_h+=save_y-m_y;
						   ret = true;
					   }
					   break;
				   case eTT_TopResize:
					   {
						   int save_y = m_y;
						   m_y = imY-m_cursor_shift_y;
						   m_h+=save_y-m_y;
						   ret = true;
						   wPropPrior = false;
					   }
					   break;
				   case eTT_RigthTopResize:
					   {
						   int save_y = m_y;
						   m_y = imY-m_cursor_shift_y;
						   m_h+=save_y-m_y;
						   m_w=imX-m_x;
						   ret = true;
					   }
					   break;
				   case eTT_RigthResize:
					   {
						   m_w=imX-m_x;
						   ret = true;
					   }
					   break;
				   case eTT_BottomRigthResize:
					   {
						   m_w=imX-m_x;
						   m_h=imY-m_y;
						   ret = true;
					   }
					   break;
				   case eTT_BottomResize:
					   {
						   m_h=imY-m_y;
						   ret = true;
						   wPropPrior = false;
					   }
					   break;
				   case eTT_LeftBottomResize:
					   {
						   int save_x = m_x;
						   m_x = imX-m_cursor_shift_x;
						   m_w+=save_x-m_x;
						   m_h=imY-m_y;
						   ret = true;
					   }
					   break;
				   case eTT_LeftResize:
					   {
						   int save_x = m_x;
						   m_x = imX-m_cursor_shift_x;
						   m_w+=save_x-m_x;
						   ret = true;
					   }
					   break;
				   }

				   Recalc(wPropPrior);
				   return ret;
			   }	

			   return false;
	   }

	   void   Recalc(bool wPropPrior = true)
	   {
		   if (m_proportions>0.01)
		   {
			   if (wPropPrior)
				   m_h = (int)(m_w*m_proportions);
			   else
				   m_w = (int)(m_h/m_proportions);
		   }
		   if (m_container_limits)
		   {
			   if (m_x < 0) m_x = 0;
			   if (m_y < 0) m_y = 0;
			   if ((m_x+m_w) > m_container_width) m_x = max(0, m_container_width-m_w);
			   if ((m_y+m_h) > m_container_heigth) m_y = max(0, m_container_heigth-m_h);

			   if (m_w > m_container_width) 
			   {
				   m_w = m_container_width;
				   if ((m_proportions>0.01) && ((m_w*m_proportions))<m_container_heigth)
					   m_h = (int)(m_w*m_proportions);
			   }
			   if (m_h > m_container_heigth) 
			   {
				   m_h = m_container_heigth;
				   if ((m_proportions>0.01) &&((m_h/m_proportions))<m_container_width)
					   m_w = (int)(m_h/m_proportions);
			   }

		   }

		   if (m_w <10) m_w = 10;
		   if (m_h <10) m_h = 10;

		 /*  if (m_proportions>0.01)
		   {
			   if (wPropPrior)
				   m_h = m_w*m_proportions;
			   else
				   m_w = m_h/m_proportions;
		   }

		   if (m_container_limits)
		   {
			   if (m_w > m_container_width) 
			   {
				   m_w = m_container_width;
				   if ((m_proportions>0.01) && ((m_w*m_proportions))<m_container_heigth)
					   m_h = m_w*m_proportions;
			   }
			   if (m_h > m_container_heigth)
			   {
				   m_h = m_container_heigth;
				   if ((m_proportions>0.01) &&((m_h/m_proportions))<m_container_width)
					    m_w = m_h/m_proportions;
			   }

		   }

		   if (m_w <10) m_w = 10;
		   if (m_h <10) m_h = 10;*/
	   }

	   Gdiplus::RectF  GetScreenRect()
	   {
		   long tmp_x = m_x;
		   long tmp_y = m_y;
		   long tmp_w = m_w;
		   long tmp_h = m_h;

		   m_cs_transform->ImagePointToScreenPoint(tmp_x, tmp_y);
		   m_cs_transform->ImageDistanceToScreenDistance(tmp_w);
		   m_cs_transform->ImageDistanceToScreenDistance(tmp_h);

		   return Gdiplus::RectF((float)tmp_x, (float)tmp_y, (float)tmp_w, (float)tmp_h);
	   }

	   bool  GetIntersection(const Gdiplus::RectF& inputScreenRect, const Gdiplus::RectF& inputSourceRect, 
								   Gdiplus::RectF& outputScreenRect,      Gdiplus::RectF& outputSourceRect)
	   {
		   Gdiplus::RectF scrRct = GetScreenRect();
			outputScreenRect = scrRct;
			outputSourceRect = inputSourceRect;

			if (scrRct.GetLeft()>inputScreenRect.GetLeft() && scrRct.GetRight()<inputScreenRect.GetRight() &&
				scrRct.GetTop()>inputScreenRect.GetTop() && scrRct.GetBottom()<inputScreenRect.GetBottom())
					return true;

			if (scrRct.GetRight()<inputScreenRect.GetLeft() || scrRct.GetLeft()>inputScreenRect.GetRight() ||
				scrRct.GetBottom()<inputScreenRect.GetTop() || scrRct.GetTop()>inputScreenRect.GetBottom())
					return false;

			if (scrRct.GetLeft()<inputScreenRect.GetLeft())
			{
				REAL delta_val = inputScreenRect.GetLeft()-scrRct.GetLeft();
				outputScreenRect.X = inputScreenRect.X;
				outputScreenRect.Width -= delta_val;

				REAL prop_delta = delta_val/scrRct.Width;
				outputSourceRect.X+=outputSourceRect.Width*prop_delta;
				outputSourceRect.Width*=(1.0f-prop_delta);
			}
			if (scrRct.GetRight()>inputScreenRect.GetRight())
			{
				REAL delta_val = scrRct.GetRight()-inputScreenRect.GetRight();
				outputScreenRect.Width -= delta_val;

				REAL prop_delta = delta_val/scrRct.Width;
				outputSourceRect.Width*=(1.0f-prop_delta);
			}

			if (scrRct.GetTop()<inputScreenRect.GetTop())
			{
				REAL delta_val = inputScreenRect.GetTop()-scrRct.GetTop();
				outputScreenRect.Y = inputScreenRect.Y;
				outputScreenRect.Height -= delta_val;

				REAL prop_delta = delta_val/scrRct.Height;
				outputSourceRect.Y+=outputSourceRect.Height*prop_delta;
				outputSourceRect.Height*=(1.0f-prop_delta);
			}
			if (scrRct.GetBottom()>inputScreenRect.GetBottom())
			{
				REAL delta_val = scrRct.GetBottom()-inputScreenRect.GetBottom();
				outputScreenRect.Height -= delta_val;

				REAL prop_delta = delta_val/scrRct.Height;
				outputSourceRect.Height*=(1.0f-prop_delta);
			}



			return true;


	   }
	int  m_x;
	int  m_y;
	int  m_w;
	int  m_h;

	bool  m_isVisible;

public:
	inline int GetContainerWidth()
	{
		return m_container_width;
	}
	inline int GetContainerHeight()
	{
		return m_container_heigth;
	}

	void  SetContainerLimitFlag(bool cl) {m_container_limits = cl;}

	void  SetProportions(float prop) {m_proportions = prop;}

private:
	
	bool  PointInRect(long px, long py, long rx, long ry, long rw, long rh)
	{
		return (px>=rx && py>=ry && px<=(rx+rw) && py<=(ry+rh));
	}

	ICoordinatSystem*   m_cs_transform;
	int  m_container_width;
	int  m_container_heigth;

	bool  m_container_limits;

	float  m_proportions;


	typedef enum
	{
		eTT_None  = 0,
		eTT_Drag,
		eTT_LeftTopResize,
		eTT_TopResize,
		eTT_RigthTopResize,
		eTT_RigthResize,
		eTT_BottomRigthResize,
		eTT_BottomResize,
		eTT_LeftBottomResize,
		eTT_LeftResize
	} TRACK_TYPE;

	TRACK_TYPE m_track_type;

	int  m_cursor_shift_x;
	int  m_cursor_shift_y;
};

#endif