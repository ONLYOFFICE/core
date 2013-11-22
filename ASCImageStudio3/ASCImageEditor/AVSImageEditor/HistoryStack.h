// HistoryStack.h : Declaration of the CHistoryStack

#pragma once
#include "resource.h"       // main symbols

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


/************************************************************************/
/* HISTORY BLOCK ELEMENTS                                               */
/************************************************************************/
class  CHistoryBlockElementBase
{
public:
	typedef enum
	{
		ET_PART_OF_IMAGE,
		ET_FULL_IMAGE,
		ET_EXTERNAL_HISTORY_BLOCK
	} ELEMENT_TYPE;

	CHistoryBlockElementBase(ELEMENT_TYPE et):m_type(et)	{	}
	virtual  ~CHistoryBlockElementBase()	{	}

	ELEMENT_TYPE   GetType() const {return m_type;}
private:
	ELEMENT_TYPE   m_type;
};

class  CHistoryBlockElementImagePart : public CHistoryBlockElementBase
{
public:
	CHistoryBlockElementImagePart(BYTE* data, long wdth, long hght, long startX, long startY):
									CHistoryBlockElementBase(CHistoryBlockElementBase::ET_PART_OF_IMAGE)
								  , m_data(data)
								  , m_width(wdth), m_height(hght)
								  , m_startX(startX), m_startY(startY)

	{

	}
	virtual ~CHistoryBlockElementImagePart()
	{
		if (m_data)
			delete[] m_data;	
	}

	       BYTE*    GetData() const         {return m_data;}
	const  long     GetWidth() const        {return m_width;}
	const  long     GetHeight() const       {return m_height;}
	const  long     GetStartX() const       {return m_startX;}
	const  long     GetStartY() const       {return m_startY;}
private:
	BYTE*          m_data;
	long           m_width;
	long           m_height;
	long           m_startX;
	long           m_startY;
};

class  CHistoryBlockElementImage : public CHistoryBlockElementBase
{
public:
	CHistoryBlockElementImage(BYTE* data, long wdth, long hght):
	  CHistoryBlockElementBase(CHistoryBlockElementBase::ET_FULL_IMAGE)
		  , m_data(data)
		  , m_width(wdth), m_height(hght)
	  {

	  }
	  virtual ~CHistoryBlockElementImage()
	  {
		  if (m_data)
			  delete[] m_data;	
	  }

	  void     ResetData(BYTE* data, long wdth, long hght)
	  {
		  m_data = data;
		  m_width = wdth;
		  m_height = hght;
	  }

	  BYTE*    GetData() const                {return m_data;}
	  const  long     GetWidth() const        {return m_width;}
	  const  long     GetHeight() const       {return m_height;}
private:
	BYTE*          m_data;
	long           m_width;
	long           m_height;
};

class  CExternalHistoryBlock : public CHistoryBlockElementBase
{
public:
	CExternalHistoryBlock(IUndoRedoCallback* external_history):CHistoryBlockElementBase(CHistoryBlockElementBase::ET_EXTERNAL_HISTORY_BLOCK)
		  , m_external_history_block(external_history)
	  {

	  }
	  virtual ~CExternalHistoryBlock()
	  {
	  }

	  IUndoRedoCallback*  GetCallbacker() const         {return m_external_history_block;}
private:
	IUndoRedoCallback* m_external_history_block;
};

class  CHistoryBlock
{
public:
	CHistoryBlock() {}
	~CHistoryBlock() 
	{
		POSITION myPos = m_elements.GetHeadPosition();
		while (myPos) 
		{
			CHistoryBlockElementBase* curEl = m_elements.GetNext(myPos);
			delete curEl;
		};
		m_elements.RemoveAll();
	}

	void     ClearSubHistories(bool isUndoCear)
	{
		POSITION myPos = m_elements.GetHeadPosition();
		while (myPos) 
		{
			CHistoryBlockElementBase* curEl = m_elements.GetNext(myPos);
			if (curEl->GetType()==CHistoryBlockElementBase::ET_EXTERNAL_HISTORY_BLOCK)
			{
				CExternalHistoryBlock* elem = reinterpret_cast<CExternalHistoryBlock*>(curEl);
				if (isUndoCear)
					elem->GetCallbacker()->ClearUndo();
				else
					elem->GetCallbacker()->ClearRedo();
			}
		};		
	}



	CString   m_description;

	void      AddImagePart(BYTE* data, long wdth, long hght, long startX, long startY)
	{
		m_elements.AddTail(new CHistoryBlockElementImagePart(data, wdth, hght,startX, startY));
	}

	void      AddImage(BYTE* data, long wdth, long hght)
	{
		m_elements.AddTail(new CHistoryBlockElementImage(data, wdth, hght));
	}

	void      AddExternalBlock(IUndoRedoCallback* hist_blocl)
	{
		m_elements.AddTail(new CExternalHistoryBlock(hist_blocl));
	}

	CAtlList<CHistoryBlockElementBase*>    m_elements;
};


struct  IHistoryEventer
{
	virtual  void   IncreaseHistory(int  incVal)   =0;
	virtual  void   DecreaseHistory(int  decVal)   =0;
};

class CHistoryBlockStorage
{
public:
	CHistoryBlockStorage():m_current_history_block(NULL),m_undo_Eventer(NULL),m_redo_Eventer(NULL)
	{

	}

	virtual ~CHistoryBlockStorage()
	{
		if (m_current_history_block)
			delete m_current_history_block;
		ClearRedo();
		ClearUndo();
	}

	void     ClearUndo()
	{
		POSITION myPos = m_undo_blocks.GetHeadPosition();
		int stCnt = 0;
		while (myPos) 
		{
			CHistoryBlock* cyrBlock = m_undo_blocks.GetNext(myPos);
			cyrBlock->ClearSubHistories(true);
			delete cyrBlock;
			stCnt++;
		};
		m_undo_blocks.RemoveAll();
		if (stCnt && m_undo_Eventer)
			m_undo_Eventer->DecreaseHistory(stCnt);

	}

	void     ClearRedo()
	{
		POSITION myPos = m_redo_blocks.GetHeadPosition();
		int stCnt = 0;
		while (myPos) 
		{
			CHistoryBlock* cyrBlock = m_redo_blocks.GetNext(myPos);
			cyrBlock->ClearSubHistories(false);
			delete cyrBlock;
			stCnt++;
		};
		m_redo_blocks.RemoveAll();
		if (stCnt && m_undo_Eventer)
			m_redo_Eventer->DecreaseHistory(stCnt);
	}

	void  ClearHistory()
	{
		if (m_current_history_block)
			delete m_current_history_block;
		ClearRedo();
		ClearUndo();
	}
protected:
	virtual  bool    Undo(bool asReset)    =0;
	virtual  bool    Redo()                =0;
public:

	int     Undo(int steps_count)
	{
		long realUndoCount = 0;
		for (int i=0;i<steps_count;i++)
		{
			if (Undo(steps_count>1))
			  realUndoCount++;
		}
		if (realUndoCount)
		{
			if (m_undo_Eventer)
				m_undo_Eventer->DecreaseHistory(realUndoCount);
			if (m_redo_Eventer)
				m_redo_Eventer->IncreaseHistory(realUndoCount);
		}
		return realUndoCount;
	}
	int     Redo(int steps_count)
	{
		long realRedoCount = 0;
		for (int i=0;i<steps_count;i++)
		{
			if (Redo())
				realRedoCount++;
		}
		if (realRedoCount)
		{
			if (m_undo_Eventer)
				m_undo_Eventer->IncreaseHistory(realRedoCount);
			if (m_redo_Eventer)
				m_redo_Eventer->DecreaseHistory(realRedoCount);
		}
		return realRedoCount;
	}

	size_t     GetUndoDepth()   {return m_undo_blocks.GetCount();}
	size_t     GetRedoDepth()   {return m_redo_blocks.GetCount();}

	CString    GetUndoElementDescription(long indx)   
	{
		POSITION  elPos = m_undo_blocks.FindIndex(indx);
		if (!elPos)
			return _T("Error");
		return  m_undo_blocks.GetAt(elPos)->m_description;
	}

	CString    GetRedoElementDescription(long indx)   
	{
		POSITION  elPos = m_redo_blocks.FindIndex(indx);
		if (!elPos)
			return _T("Error");
		return  m_redo_blocks.GetAt(elPos)->m_description;
	}

	bool    StartBlock()
	{
		if (m_current_history_block)
			delete m_current_history_block;
		m_current_history_block = new CHistoryBlock();
		return true;
	}
	bool    StopBlock(const CString& desc)
	{
		if (!m_current_history_block)
			return false;
		m_current_history_block->m_description = desc;
		m_undo_blocks.AddTail(m_current_history_block);

		if (m_undo_Eventer)
			m_undo_Eventer->IncreaseHistory(1);

		size_t wasItems = m_redo_blocks.GetCount();
		ClearRedo();
		if (m_redo_Eventer)
			m_redo_Eventer->DecreaseHistory((int)wasItems);
		m_current_history_block = NULL;
		return true;
	}

	void   SetUndoEventer(IHistoryEventer* ue) {m_undo_Eventer = ue;}
	void   SetRedoEventer(IHistoryEventer* re) {m_redo_Eventer = re;}

protected:
	CHistoryBlock*  GetCurrentBlock() {return m_current_history_block;}

	CHistoryBlock*             m_current_history_block;
	CAtlList<CHistoryBlock*>   m_undo_blocks;
	CAtlList<CHistoryBlock*>   m_redo_blocks;

	IHistoryEventer*           m_undo_Eventer;
	IHistoryEventer*           m_redo_Eventer;
};

// IHistoryStack
[
	object,
	uuid("204E7FF2-A704-4AA3-A3D8-4A00CE85A6F0"),
	dual,	helpstring("IHistoryStack Interface"),
	pointer_default(unique)
]
__interface IHistoryStack : IDispatch
{
	[id(100)] HRESULT GetDepth([out,retval] LONG* nCount);
	[id(101)] HRESULT GetElementDescription([in] LONG nParam, [out,retval] BSTR* Descr);

	[id(200)] HRESULT Pop([in] LONG nCount);
};

// _IHistoryStackEvents
[
	dispinterface,
	uuid("FAD7CD6A-9B42-4280-BC7D-088827F4FB67"),
	helpstring("_IHistoryStackEvents Interface")
]
__interface _IHistoryStackEvents
{
	[id(100)] HRESULT Increase([in] LONG nCount);
	[id(101)] HRESULT Decrease([in] LONG nCount);
};

// CHistoryStack

[
	coclass,
	default(IHistoryStack,_IHistoryStackEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSImageEditor.HistoryStack"),
	progid("AVSImageEditor.HistoryStack.1"),
	version(1.0),
	uuid("91465E88-2E3B-4F41-9856-524607D11B4C"),
	helpstring("HistoryStack Class")
]
class ATL_NO_VTABLE CHistoryStack :
	public IHistoryStack, public IHistoryEventer
{
public:
	CHistoryStack():m_history_storage(NULL),m_undo_history_using(true),m_app_manager(NULL),m_undo_redo_callback(NULL)
	{
	}

	__event __interface _IHistoryStackEvents;


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
private:
	CHistoryBlockStorage*   m_history_storage;
	bool                    m_undo_history_using;
	IAppManager*            m_app_manager;
	IUndoRedoCallback*      m_undo_redo_callback;
public:
	void     UseUndoHistory()    {m_undo_history_using = true;}
	void     UseRedoHistory()    {m_undo_history_using = false;}

	void     SetStorage(CHistoryBlockStorage* hbs)   {m_history_storage = hbs;}

	void SetAppManager(IAppManager* appPtr)	                        {		m_app_manager = appPtr;	}
	void SetUndoRedoCallbackerPtr(IUndoRedoCallback* cbPtr)	{		m_undo_redo_callback = cbPtr;	}

	virtual  void   IncreaseHistory(int  incVal)
	{
		Increase((LONG)incVal);
	}
	virtual  void   DecreaseHistory(int  decVal)
	{
		Decrease((LONG)decVal);
	}

	bool LoadState(const CString& filePath)
	{
		return true;
	}
	bool SaveState(const CString& filePath)
	{
		return true;
	}

public:

	STDMETHOD(GetDepth)( LONG* nCount)
	{
		if (!m_history_storage)
			*nCount = 0;
		else
		{
			if (m_undo_history_using)
				*nCount = m_history_storage->GetUndoDepth();
			else
				*nCount = m_history_storage->GetRedoDepth();
		}
		return S_OK;
	}
	STDMETHOD(GetElementDescription)( LONG nParam, BSTR* Descr)
	{
		CString tmpS;
		if (m_history_storage)
		{
			if (m_undo_history_using)
				tmpS = m_history_storage->GetUndoElementDescription(nParam);
			else
				tmpS = m_history_storage->GetRedoElementDescription(nParam);
		}
		*Descr = tmpS.AllocSysString();
		return S_OK;
	}

	STDMETHOD(Pop)( LONG nCount)
	{
		if (m_history_storage)
		{
			m_app_manager->EnableInterfaceControls(false);
			if (m_undo_history_using)
			{
				long realUndoCount = m_history_storage->Undo(nCount);
				
				if (realUndoCount)
				{
					if (m_undo_redo_callback)
						m_undo_redo_callback->OnUndo(realUndoCount, realUndoCount>1);
					if (m_app_manager)
						m_app_manager->GetLayersManager()->GetCurrentLayer()->ChangeImage(ILayer::SLOW);
				}

			}
			else
			{
				long realRedoCount = m_history_storage->Redo(nCount);
				if (realRedoCount)
				{
					if (m_undo_redo_callback)
						m_undo_redo_callback->OnRedo(realRedoCount);
					if (m_app_manager)
						m_app_manager->GetLayersManager()->GetCurrentLayer()->ChangeImage(ILayer::SLOW);
				}
			}
			m_app_manager->EnableInterfaceControls(true);
		}
		return S_OK;
	}

};

