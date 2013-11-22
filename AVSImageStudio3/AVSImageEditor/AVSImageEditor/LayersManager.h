#ifndef __LAYERS_MANAGER__
#define __LAYERS_MANAGER__

#include "HistoryStack.h"
#include "CommonHeader.h"

namespace ImageEditor
{

	class  CWatermarks
	{
	public:
		CWatermarks(IAppManager* app_manager):m_app_manager(app_manager)
		{
			for (int i=0; i<2; i++)
				m_rects[i] = new TrackRect(m_app_manager->GetCoordinateTransformer());
			
			m_pBitmaps[0] = NULL;
			m_pBitmaps[1] = NULL;

			m_edit_regime = true;
		}
		~CWatermarks()
		{
			for (int i=0; i<2; i++)
			{
				delete m_pBitmaps[i];
				delete m_rects[i];
			}
		}

		void  Draw(Gdiplus::Graphics* pGr);

		void  DrawOnData(BYTE* imData, long wdth, long hght);

		void  SetImageSizes(int szX, int szY);

		bool OnMouseDown(LONG nX, LONG nY, LONG nButton);
	    bool OnMouseUp(LONG nX, LONG nY, LONG nButton);
	    bool OnMouseMove(LONG nX, LONG nY, LONG nButton);

		void  SetXML(const CString& paramName, BSTR* xmlStr);
		void  SetParam(const CString& paramName, double param);

		void  SetEditRegime(bool isEdit = true)  {m_edit_regime = isEdit;}

	private:
		IAppManager* m_app_manager;
		
		TrackRect*       m_rects[2];
		CString          m_xmls[2];
		Gdiplus::Bitmap* m_pBitmaps[2];

		bool             m_edit_regime;

	};

	namespace   Layers
	{

		class  CLayersManager;
		
		struct  histogramm
		{
			long m_values[256+256+256+256];
			histogramm()
			{
				clear();
			}
			void clear()
			{
				memset(m_values,0,(256+256+256+256)*sizeof(long));
			}
			long  get_red_shift()     {return 255+255;}
			long  get_green_shift()   {return 255;}
			long  get_blue_shift()    {return 0;}
			long  get_gray_shift()    {return 255+255+255;}
		};

		class  CLayer : public CHistoryBlockStorage, public ILayer
		{
			friend class CLayersManager;
		public:
			//CLayersManager* m_parent;
			CWatermarks* m_marks;
		private:
			CLayer():CHistoryBlockStorage(),
									m_show(true),m_name("Layer"),m_was_load(false), 
									m_app_manager(NULL),
									m_meta_data("Unknown"),
									m_actual_image(SLOW),
									m_layer_manager_changes_subscriber(NULL),
									m_slow_function_thread(NULL),
									m_freeze(false)
			{	
				for (int i=0;i<LAST_INDEX;i++)
				{
					m_image_data[i] = NULL;
					m_width[i] = 0;
					m_height[i] = 0;
				}

				m_marks = NULL;
			}
			~CLayer()
			{
				if (m_slow_function_thread)
					m_slow_function_thread->StopWork();
				ClearHistory();
				ClearImageData();
			}

			bool  Load(const CString& filePath, int& errCode);

			void  ClearImageData()
			{
				for (int i=0;i<LAST_INDEX;i++)
				{
					if (m_image_data[i])
					{
						delete[] m_image_data[i];
						m_image_data[i] = NULL;
						m_width[i] = m_height[i] = 0;
					}
				}
			}

			virtual  bool    Undo(bool asReset);
			virtual  bool    Redo();
		public:
			/************************************************************************/
			/* ILayer interface                                                     */
			/************************************************************************/
			virtual BYTE*  GetImage(IMAGE_TYPE imType, long& width, long& height)
			{
				BOOL bHuck = FALSE;
				if( imType == DUBLICATE )
				{
					imType = SLOW_COPY;
					bHuck = TRUE;
				}

				if (!m_was_load || (unsigned)imType >= LAST_INDEX)
				{
					width = 0;
					height = 0;
					return NULL;
				}	
				
				if (imType == SLOW_COPY && !m_image_data[SLOW_COPY])
				{
					UpdateCopy(SLOW);
				}
				else
				if (imType==FAST_COPY && !m_image_data[FAST_COPY])
				{
					UpdateCopy(FAST);
				}

				width = m_width[imType];
				height = m_height[imType];

				if( bHuck && m_image_data[SLOW_COPY] ) // huck!!!
				{
					MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;

					CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pMediaData));
					if (NULL == pMediaData)
						return NULL;

					// specify settings
					pMediaData->put_Width(width);
					pMediaData->put_Height(height);
					pMediaData->put_AspectRatioX(width);
					pMediaData->put_AspectRatioY(height);
					pMediaData->put_Interlaced(VARIANT_FALSE);
					pMediaData->put_Stride(0, 4*width);
					pMediaData->AllocateBuffer(4*width*height);

					// allocate necesasry buffer
					
					BYTE* pBufferPtr;
					pMediaData->get_Buffer(&pBufferPtr);
					if( !pBufferPtr )
					{
						pMediaData->Release();
						return NULL;
					}

					long nCreatedBufferSize = 0;
					pMediaData->get_BufferSize(&nCreatedBufferSize);
					pMediaData->put_Plane(0, pBufferPtr);

					//if (!flip)
					memcpy(pBufferPtr, m_image_data[SLOW_COPY], 4*width*height);

					if( m_marks )
					{
						m_marks->DrawOnData( pBufferPtr, width, height );
					}

					return (BYTE*)pMediaData;
				}

				return m_image_data[imType];
			}

			virtual void UpdateCopy(IMAGE_TYPE imType)
			{
				if (imType==SLOW || imType==SLOW_COPY)
				{
					if(!m_image_data[SLOW_COPY])
					{
						m_width[SLOW_COPY] = m_width[SLOW];	
						m_height[SLOW_COPY] = m_height[SLOW];
						m_image_data[SLOW_COPY] = new BYTE[4*m_width[SLOW]*m_height[SLOW]];
					}
					memcpy(m_image_data[SLOW_COPY],m_image_data[SLOW],4*m_width[SLOW]*m_height[SLOW]);
				}
				if (imType==FAST || imType==FAST_COPY)
				{
					if(!m_image_data[FAST_COPY])
					{
						m_width[FAST_COPY] = m_width[FAST];	
						m_height[FAST_COPY] = m_height[FAST];
						m_image_data[FAST_COPY] = new BYTE[4*m_width[FAST]*m_height[FAST]];
					}
					memcpy(m_image_data[FAST_COPY],m_image_data[FAST],4*m_width[FAST]*m_height[FAST]);
				}	
			}

			virtual  const BYTE*       GetActualImage(long& width, long& height, double& scaleCoef)
			{
				scaleCoef = ((double)m_width[SLOW])/((double)m_width[m_actual_image]);
				return GetImage(m_actual_image, width,height);
			}

			virtual  IMAGE_TYPE        GetActualImage()
			{
				return m_actual_image;
			}
			virtual  void              SetActualImage(IMAGE_TYPE imType)  
			{
				if ((unsigned)imType<LAST_INDEX)
				{
					m_actual_image = imType;
				}
			}

			virtual  void SetFreezeLayer(bool isFreeze)
			{
				m_freeze = isFreeze;
			}
			virtual  bool GetFreezeLayer()
			{
				return m_freeze;
			}

			virtual  void TryUsingSlowFunction( CBaseThread* slowThread )
			{
				if (m_slow_function_thread)
					m_slow_function_thread->StopWork();
				m_slow_function_thread = slowThread;
				if (slowThread)
					slowThread->StartWork(THREAD_PRIORITY_LOWEST);
			}

			virtual  void ChangeImage( IMAGE_TYPE imType, IChangesSubscriber* fromCommand = NULL, bool repaint=true, bool useOtherSubscribers=true );
			virtual  void InvalidateImagePart( int lft, int tp, int rght, int bttom );

			virtual   long*  GetHistogramm()    {return m_histogramm.m_values;};

			virtual  bool  StartHistoryBlock()			{			return StartBlock();		}
			virtual  bool  ChangePartOfLayer(BYTE* imPart, long wdth, long hght, long startX, long startY);
			virtual  bool  ChangePartOfLayer(BYTE* imPart, long shiftSrcX, long shiftSrcY, long scrBufferWidth, long wdth, long hght, long startX, long startY);
			virtual  bool  ChangeAllLayer(BYTE* imPart, long wdth, long hght);
			virtual  bool  ChangeManually(IUndoRedoCallback* hist_blocl);
			virtual  bool  StopHistoryBlock(const CString& block_description)	{ return StopBlock(block_description);	}

			virtual void          AddSubscriber(IChangesSubscriber* newSS)
			{
				m_changes_subscribes.AddTail(newSS);
			}
			virtual void          RemoveSubscriber(IChangesSubscriber* SS)
			{
				POSITION lPos = m_changes_subscribes.Find(SS);
				if (lPos)
					m_changes_subscribes.RemoveAt(lPos);
			}

		public:
			const CString&   GetMetaData() const {return m_meta_data;}
			void             SetMetaData(const CString& nMD) {m_meta_data = nMD;}

			const CAtlMap<CString, double>*   GetDoublesInfos() const {return &m_doubles_infos;};
			const CAtlMap<CString, CString>*  GetStringsInfos() const {return &m_strings_infos;};
		private:
			void             CalcHistogramm();

			IAppManager*        m_app_manager;
			bool     m_show;
			CString  m_name;
			bool     m_was_load;

			BYTE*    m_image_data[LAST_INDEX];
			long     m_width[LAST_INDEX];
			long     m_height[LAST_INDEX];

			histogramm          m_histogramm;

			CString             m_meta_data;

			IMAGE_TYPE          m_actual_image;

			CAtlList<IChangesSubscriber*>    m_changes_subscribes;
			IChangesSubscriber*              m_layer_manager_changes_subscriber;

			CBaseThread*        m_slow_function_thread;

			CAtlMap<CString, double>   m_doubles_infos;
			CAtlMap<CString, CString>  m_strings_infos;

			bool                m_freeze;


		};








		class  CLayersManager    :    public   ILayersManager, public  IChangesSubscriber
		{
		public:	
			CLayersManager():m_app_manager(NULL),m_regCheck(false)
			{
				if (!SUCCEEDED(m_undo_stack.CoCreateInstance(__uuidof(CHistoryStack), NULL, CLSCTX_INPROC)))
				{
					throw "Failed";
				}
				if (!SUCCEEDED(m_redo_stack.CoCreateInstance(__uuidof(CHistoryStack), NULL, CLSCTX_INPROC)))
				{
					throw "Failed";
				}

				CHistoryStack*  pUndoStack = static_cast<CHistoryStack*>((void*)m_undo_stack);
				CHistoryStack*  pRedoStack = static_cast<CHistoryStack*>((void*)m_redo_stack);

				pUndoStack->UseUndoHistory();
				pRedoStack->UseRedoHistory();

				for (int i=0;i<3;i++)
					m_pNonActivateBitmaps[i] = NULL;

				LoadNonActivateBitmaps();

				m_pWatermarks = NULL;
			}
			~CLayersManager() 
			{ 
				Clear();

				for (int i=0;i<3;i++)
					RELEASEOBJECT(m_pNonActivateBitmaps[i]);

			}

			void   ClearHistories()
			{
				POSITION myPos = m_layers.GetHeadPosition();

				while (myPos) 
				{
					CLayer* cyrL = m_layers.GetNext(myPos);
					cyrL->ClearHistory();
				};
			}

			void   Clear()
			{
				POSITION myPos = m_layers.GetHeadPosition();

				while (myPos) 
				{
					CLayer* cyrL = m_layers.GetNext(myPos);
					cyrL->ClearHistory();
					delete cyrL;
				};

				m_layers.RemoveAll();
			}

			bool   AddLayer(const CString filePath, int& errCode)
			{
				if (filePath.IsEmpty())
				{
					errCode = 1;
					return false;
				}

				CLayer* curL = new CLayer;
				curL->m_app_manager = m_app_manager;
				curL->m_layer_manager_changes_subscriber  = this;
				if (!curL->Load(filePath,errCode))
				{
					delete curL;
					return false;
				}
				
				m_layers.AddTail(curL);

				curL->ChangeImage(ILayer::SLOW);

				CHistoryStack*  pUndoStack = static_cast<CHistoryStack*>((void*)m_undo_stack);
				CHistoryStack*  pRedoStack = static_cast<CHistoryStack*>((void*)m_redo_stack);

				pUndoStack->SetStorage((CHistoryBlockStorage*)curL);
				pRedoStack->SetStorage((CHistoryBlockStorage*)curL);

				curL->SetUndoEventer((IHistoryEventer*)pUndoStack);
				curL->SetRedoEventer((IHistoryEventer*)pRedoStack);
				
				return true;
			}

			bool   Save(const CString& filePath, CWatermarks* watermarks = NULL);
			void   LoadNonActivateBitmaps();
			/************************************************************************/
			/* ILayersManagerInterface                                              */
			/************************************************************************/
			virtual ILayer*   GetCurrentLayer()
			{
				if (m_layers.GetCount()==0)
					return NULL;
				
				CLayer*  curLayer = m_layers.GetHead();
				if (!curLayer)
				{
					return NULL;
				}

				curLayer->m_marks = m_pWatermarks;

				return curLayer;
			}

			virtual  const BYTE*       GetResultImage(long& width, long& height, double& scaleCoef)
			{
				ILayer* curL = GetCurrentLayer();
				if (!curL)
				{
					width = 0;
					height = 0;
					scaleCoef = 1.0f;
					return NULL;
				}
				return curL->GetActualImage(width,height,scaleCoef);
			}

			virtual bool DrawLogo(MediaCore::IAVSUncompressedVideoFrame** pMediaData, bool bPrint );

			void GetUndoStackIDisp(IHistoryStack** pVal)	
			{	
				if (m_undo_stack)
					((IHistoryStack*)m_undo_stack)->AddRef();
				*pVal = m_undo_stack;	
			}
			void GetRedoStackIDisp(IHistoryStack** pVal)	
			{	
				if (m_redo_stack)
					((IHistoryStack*)m_redo_stack)->AddRef();
				*pVal = m_redo_stack;	
			}

			void SetUndoRedoCallbackPtr(IUndoRedoCallback* cbPtr)
			{
				CHistoryStack*  pUndoStack = static_cast<CHistoryStack*>((void*)m_undo_stack);
				CHistoryStack*  pRedoStack = static_cast<CHistoryStack*>((void*)m_redo_stack);

				pUndoStack->SetUndoRedoCallbackerPtr(cbPtr);
				pRedoStack->SetUndoRedoCallbackerPtr(cbPtr);
			}

			void  SetAppManager(IAppManager* am)   
			{
				m_app_manager=am;
				POSITION myPos = m_layers.GetHeadPosition();
				while (myPos) 
				{
					CLayer* cyrL = m_layers.GetNext(myPos);
					cyrL->m_app_manager = am;
				};

				CHistoryStack*  pUndoStack = static_cast<CHistoryStack*>((void*)m_undo_stack);
				CHistoryStack*  pRedoStack = static_cast<CHistoryStack*>((void*)m_redo_stack);

				pUndoStack->SetAppManager(am);
				pRedoStack->SetAppManager(am);
			};

			const CString   GetMetaData() const 
			{
				if (m_layers.GetCount()==0)
					return CString("Unknown");
				CLayer*  curLayer = m_layers.GetHead();
				if (!curLayer)
				{
					return CString("Unknown");
				}
				return curLayer->GetMetaData();
			}

			virtual void ChangesHandler(int changingObject, bool repaint) 
			{
				if (m_app_manager)
					m_app_manager->SendMsg(WM_IEM_UPDATE_THUMBNAIL, NULL);
			}

			void   SetRegCheck(bool rcheck)
			{
				m_regCheck = rcheck;
			}

			bool   GetRegCheck()
			{
				return m_regCheck ;
			}

			bool LoadState(const CString& filePath)
			{
				CHistoryStack*  pUndoStack = static_cast<CHistoryStack*>((void*)m_undo_stack);
				return pUndoStack->LoadState(filePath);
			}
			bool SaveState(const CString& filePath)
			{
				CHistoryStack*  pUndoStack = static_cast<CHistoryStack*>((void*)m_undo_stack);
				return pUndoStack->SaveState(filePath);
			}

			bool SaveData(const CString& filePath, BYTE* imData, long width, long hght, bool flip, CWatermarks* watermarks,CString &MetaDataXML);
			
			void SetWatermarks( CWatermarks* pWatermarks )
			{
				m_pWatermarks = pWatermarks;
			}

			CWatermarks* GetWatermarks() const
			{
				return m_pWatermarks;
			}

		private:
			CAtlList<CLayer*>   m_layers;

			CComPtr<IHistoryStack>   m_undo_stack;
			CComPtr<IHistoryStack>   m_redo_stack;

			IAppManager*             m_app_manager;

			bool                      m_regCheck;
			Gdiplus::Bitmap*          m_pNonActivateBitmaps[3];
			
			CWatermarks* m_pWatermarks;
		};

	};
};

#endif