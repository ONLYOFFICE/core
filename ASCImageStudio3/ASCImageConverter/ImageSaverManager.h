#pragma once
#include "TimeMeasurer.h"
#include "TemporaryCS.h"
#include "ImageSaver.h"

[event_receiver(native)]
class  ImageSaverManager
{
//----------------------------- Public Members --------------------------------
public: static const LONG g_cdMaxPercent = 1000000;
//----------------------------- Public Methods --------------------------------
public: ImageSaverManager()
		{
			InitializeCriticalSection(&m_csThread);
			m_nState = IMAGECONVERTER_STATE_IDLE ;
			m_nThreadPriority = THREAD_PRIORITY_NORMAL;
			Clear();
		}
public: ~ImageSaverManager()
		{
			DeleteCriticalSection(&m_csThread);
			for( INT32 i=0; i < (INT32)m_aConverters.GetCount(); i++ )
				unhookEvent(&m_aConverters[i]);
		}
public: VOID SetLogo(MediaCore::IAVSUncompressedVideoFrame* piLogo)
		{	
			for( int i = 0; i < (INT32)m_aConverters.GetCount(); i++ )
				m_aConverters[i].SetLogo( piLogo );
		}
public: VOID SetLogoParam( INT32 nParam )
		{	
			for( int i = 0; i < (INT32)m_aConverters.GetCount(); i++ )
				m_aConverters[i].SetLogoParam( nParam );
		}
public: VOID SetWM( bool bEnable )
		{	
			for( int i = 0; i < (INT32)m_aConverters.GetCount(); i++ )
				m_aConverters[i].SetWM( bEnable );
		}
public: VOID SetCache( ImageStudio::IAVSImageCache* piCache )
		{	
			for( int i = 0; i < (INT32)m_aConverters.GetCount(); i++ )
				m_aConverters[i].SetCache( piCache );
		}
public: INT32 GetStatus()
		{
			return m_nState;
		}
public: void SetStatus(INT32 nNewStatus)
		{
			m_nState = nNewStatus;
		}
public: INT32 GetTreadCount()
		{
			return (INT32)m_aConverters.GetCount();
		}
public: VOID SetTreadCount(INT32 nCount)
		{
			if(  m_aConverters.GetCount() != nCount )
			{
				for( INT32 i=0; i < (INT32)m_aConverters.GetCount(); i++ )
					unhookEvent(&m_aConverters[i]);
				m_aConverters.RemoveAll();

				m_aConverters.SetCount( nCount );

				for( INT32 i=0; i < (INT32)m_aConverters.GetCount(); i++ )
				{
					hookEvent(&m_aConverters[i]);
					if (!m_old_photo_masks_folder.IsEmpty())
					{
						CString sParamName = _T("OldPhotoMasksFolder");
						BSTR  tmpBSTRStr1 = sParamName.AllocSysString();
						BSTR  tmpBSTRStr2 = m_old_photo_masks_folder.AllocSysString();
						m_aConverters[i].SetAdditionalParam(tmpBSTRStr1, tmpBSTRStr2);
						SysFreeString( tmpBSTRStr1 );
						SysFreeString( tmpBSTRStr2 );
					}
				}
			}
		}
public: VOID SetThreadPriority(LONG NewVal)
		{
			m_nThreadPriority = NewVal;
			for(INT32 i=0; i < (INT32)m_aConverters.GetCount(); i++)
			{
				m_aConverters[i].SetThreadPriority( NewVal );
			}
		}
public: DOUBLE GetEstimatedTime()
		{
			if( m_lCurProcess <= 0 || m_lCurProcess > g_cdMaxPercent )
				return -1;
			else
				return GetElapsedTime() * (g_cdMaxPercent - m_lCurProcess) / m_lCurProcess;
		}
public: DOUBLE GetElapsedTime()
		{
			return (DOUBLE)m_oWorkedTimer.GetTimeInterval();
		}
public: CString GetCurImageName()
		{
			return m_sCurImagename;
		}
public: LONG GetProcess()
		{
			return m_lCurProcess;
		}
public: bool GetErrors(CAtlArray<LONG>& aInputArray)
		{
			aInputArray.RemoveAll();
			aInputArray.Append(m_aErrors);
			m_aErrors.RemoveAll();
			return aInputArray.GetCount() > 0;
		}
public: bool GetErrors2(CAtlArray<CErrorItem>& aInputArray)
		{
			aInputArray.RemoveAll();
			aInputArray.Append(m_aErrors2);
			m_aErrors2.RemoveAll();
			return aInputArray.GetCount() > 0;
		}
public: BOOL Suspend()
		{
			CTemporaryCS oCS( &m_csThread );
			if( IMAGECONVERTER_STATE_RUN == m_nState )
			{
				BOOL bIsSuspended = FALSE;
				for( INT32 i = 0; i < (INT32)m_aConverters.GetCount(); i++)
					if( TRUE == m_aConverters[i].IsRunned() )
					{
						m_aConverters[i].Suspend();
						bIsSuspended = TRUE;
					}

				// считаем успешной, если удалось остановить хотябы 1 поток
				if( TRUE == bIsSuspended )
				{
					m_oWorkedTimer.Suspend();
					m_nState = IMAGECONVERTER_STATE_SUSPENDED;
					return TRUE;
				}
			}
			return FALSE;
		}
public: BOOL Resume()
		{	
			CTemporaryCS oCS( &m_csThread );
			if( IMAGECONVERTER_STATE_SUSPENDED == m_nState )
			{
				BOOL bIsSuspended = FALSE;
				for(INT32 i=0; i < (INT32)m_aConverters.GetCount(); i++)
					if( TRUE == m_aConverters[i].IsSuspended() )
					{
						m_aConverters[i].Resume();
						bIsSuspended = TRUE;
					}
				// считаем успешной, если удалось восстановить хотябы 1 поток
				if( TRUE == bIsSuspended )
				{
					m_oWorkedTimer.Resume();
					m_nState = IMAGECONVERTER_STATE_RUN;
					return TRUE;
				}
			}
			return FALSE;
		}
public: INT32 Start()
		{
			if( m_nActiveConvertersCount > 0 )
			{
				m_oWorkedTimer.Reset();
				m_nThreadComplete = 0;
				//стартуем конвертеры
				for(INT32 i=0; i < m_nActiveConvertersCount; i++)
					m_aConverters[ i ].Start( m_nThreadPriority );
				m_nState = IMAGECONVERTER_STATE_RUN;
				return 0;
			}
			return 1;
		}
public: BOOL Stop()
		{
			if( IMAGECONVERTER_STATE_IDLE != m_nState )
			{
				Resume();

				for(INT32 i=0; i < (INT32)m_aConverters.GetCount(); i++)
				{
					m_aConverters[i].Stop();
				}

				m_nState = IMAGECONVERTER_STATE_IDLE;
			}
			return TRUE;
		}
public: VOID Clear()
		{
			m_sCurImagename = _T("");
			m_aTasks.RemoveAll();
			m_nActiveConvertersCount = -1;
			m_nThreadComplete = 0;
			m_lCurProcess = 0;
			m_nCurTaskIndex = 0;
			m_bIsSomeChange = false;
		}
public: LONG AddTasks(XmlUtils::CXmlReader& oReader)
		{
			LONG nError = 0;
			if ( (INT32)m_aConverters.GetCount() > oReader.GetLengthList() )
				m_nActiveConvertersCount = oReader.GetLengthList();
			else
				m_nActiveConvertersCount = (INT32)m_aConverters.GetCount();

			for(INT32 i=0; i < oReader.GetLengthList(); i++)
			{
				ConvertItem oNewItem;
				XML::IXMLDOMNodePtr oSubNode;
				oReader.GetNode(i,oSubNode);

				XmlUtils::CXmlReader oSubNodeReader;
				oSubNodeReader.OpenFromXmlNode( oSubNode );

				if( TRUE == oSubNodeReader.ReadNodeList( CString(_T("Source")) ))
				{
					for( int j = 0; j < oSubNodeReader.GetLengthList(); j++ )
					{
						Source oNewSource;
						oNewSource.sFilename = oSubNodeReader.ReadNodeAttribute( j, _T("filepath"),_T(""));
						oNewSource.nPage = Strings::ToInteger( oSubNodeReader.ReadNodeAttribute( j, _T("page"),_T("-1")) );
						XML::IXMLDOMNodePtr oSubSubNode;
						oSubNodeReader.GetNode(j, oSubSubNode);
						XmlUtils::CXmlReader oSubSubNodeReader;
						oSubSubNodeReader.OpenFromXmlNode( oSubSubNode );
						if( TRUE == oSubSubNodeReader.ReadNode( _T("Transforms") ) )
							if( TRUE == oSubSubNodeReader.ReadNodeList( _T("*") ) )
								for( int k = 0; k < oSubSubNodeReader.GetLengthList(); k++ )
									oNewSource.sTransforms.Append( oSubSubNodeReader.ReadNodeXml(k) );
						if( false == oNewSource.sFilename.IsEmpty() )
							oNewItem.aSources.Add( oNewSource );
					}
				}
				oSubNodeReader.OpenFromXmlNode( oSubNode );
				if( TRUE == oSubNodeReader.ReadNode( CString(_T("Destinations")) ) )
				{
					if( TRUE == oSubNodeReader.ReadNodeList( CString(_T("*")) ))
					{
						for(INT32 i = 0; i < oSubNodeReader.GetLengthList(); i++)
						{
							oNewItem.aDestinations.Add( oSubNodeReader.ReadNodeXml(i) );
						}
					}
				}
				if( oNewItem.IsValid() == false )
					return IMAGECONVERTER_ERROR_XML_FORMAT;

				m_aTasks.Add( oNewItem );
			}
			return 0;
		}
public: bool GetIsComplete()
		{
			return m_nThreadComplete == m_nActiveConvertersCount;
		}
public: bool GetCompleteFiles( CAtlArray<CString>& aInputArray )
		{
			aInputArray.RemoveAll();
			aInputArray.Append(m_aCompleteFiles);
			m_aCompleteFiles.RemoveAll();
			return aInputArray.GetCount() > 0;
		}

public: void Lock()
		{
			EnterCriticalSection( &m_csThread );
		}
public: void Free()
		{
			m_bIsSomeChange = false;
			LeaveCriticalSection( &m_csThread );
		}
public: bool IsSomeChange()
		{
			return m_bIsSomeChange;
		}
public: void  SetAdditionalParam(BSTR ParamName, BSTR ParamValue)
		{
			if( ParamName == CString(_T("OldPhotoMasksFolder")) )
			{
				m_old_photo_masks_folder = CString(ParamValue);
				for(INT32 i=0; i < (INT32)m_aConverters.GetCount(); i++)
				{
					m_aConverters[i].SetAdditionalParam(ParamName, ParamValue);
				}
			}
		}
//----------------------------- Private Methods --------------------------------
private: void hookEvent(ImageSaver* pSource)
		{
			__hook( &ImageSaver::OnComplete,				pSource, &ImageSaverManager::OnComplete);
			__hook( &ImageSaver::OnError,					pSource, &ImageSaverManager::OnError);
			__hook( &ImageSaver::OnGetTask,					pSource, &ImageSaverManager::OnGetTask);
			__hook( &ImageSaver::OnProgress,				pSource, &ImageSaverManager::OnProgress);
			__hook( &ImageSaver::OnError2,					pSource, &ImageSaverManager::OnError2);
			__hook( &ImageSaver::OnCompleteFile,			pSource, &ImageSaverManager::OnCompleteFile);
		}

private: void unhookEvent(ImageSaver* pSource) 
		{
			__unhook( &ImageSaver::OnComplete,				pSource, &ImageSaverManager::OnComplete);
			__unhook( &ImageSaver::OnError,					pSource, &ImageSaverManager::OnError);
			__unhook( &ImageSaver::OnGetTask,				pSource, &ImageSaverManager::OnGetTask);
			__unhook( &ImageSaver::OnProgress,				pSource, &ImageSaverManager::OnProgress);
			__unhook( &ImageSaver::OnError2,				pSource, &ImageSaverManager::OnError2);
			__unhook( &ImageSaver::OnCompleteFile,			pSource, &ImageSaverManager::OnCompleteFile);
		}
private: void OnProgress()
		{
			CTemporaryCS oCS(&m_csThread);
			m_bIsSomeChange = true;
			CalculateProgress();
		 }
private: void OnComplete()
		{
			CTemporaryCS oCS(&m_csThread);
			m_bIsSomeChange = true;
			m_nThreadComplete++;
			if( m_nThreadComplete == m_nActiveConvertersCount )
			{
				m_lCurProcess = g_cdMaxPercent;
				m_nState = IMAGECONVERTER_STATE_IDLE;
			}

		 }
private: void OnError(LONG dValue)
		 {
			CTemporaryCS oCS(&m_csThread);
			Suspend();
			m_bIsSomeChange = true;
			m_aErrors.Add( dValue );
		}
private: void OnError2(CErrorItem& oError)
		 {
			CTemporaryCS oCS(&m_csThread);
			Suspend();
			m_bIsSomeChange = true;
			m_aErrors2.Add( oError );
		}
private: void CalculateProgress()
		 {
			double dTempSum = 0;
			for(INT32 i=0; i < m_nActiveConvertersCount; i++)
				dTempSum += m_aConverters[i].GetProcess();

			if( m_nCurTaskIndex < m_nActiveConvertersCount || m_aTasks.GetCount() == 0 )
				m_lCurProcess = 0;
			else
				m_lCurProcess = (LONG)(g_cdMaxPercent * ( m_nCurTaskIndex - m_nActiveConvertersCount + dTempSum ) /  m_aTasks.GetCount()   );
		 }
private: bool OnGetTask(ConvertItem& oNewTask)
		{
			CTemporaryCS oCS(&m_csThread);
			m_bIsSomeChange = true;
			m_nCurTaskIndex++;
	
			CalculateProgress();
			if( m_nCurTaskIndex - 1< (INT32)m_aTasks.GetCount() )
			{
				oNewTask = m_aTasks[ m_nCurTaskIndex - 1 ];
				m_sCurImagename = _T("");
				if( oNewTask.aSources.GetCount() > 0 )
				{
					if( false == oNewTask.aSources[0].sFilename.IsEmpty() )
					{
						m_sCurImagename = _T("");
						m_sCurImagename.AppendFormat( _T("<Source filepath=\"%s\""), ImageSaver::ModifyToValidXml( oNewTask.aSources[0].sFilename ));
						m_sCurImagename.AppendFormat( _T(" page=\"%d\">"), oNewTask.aSources[0].nPage);
						m_sCurImagename.AppendFormat( _T("<Transforms>%s</Transforms>"), oNewTask.aSources[0].sTransforms);
						m_sCurImagename.Append( _T("</Source>") );
					}
				}
				return true;
			}
			m_sCurImagename = _T("");
			return false;
		}

private: void OnCompleteFile(CString& sCompleteFile)
		 {
			CTemporaryCS oCS(&m_csThread);
			m_bIsSomeChange = true;
			m_aCompleteFiles.Add( sCompleteFile );
		 }
//----------------------------- Private Members -------------------------------

private: CAtlArray<ImageSaver> m_aConverters;
private: CAtlArray<LONG> m_aErrors;
private: CAtlArray<CErrorItem> m_aErrors2;
private: CAtlArray<ConvertItem> m_aTasks;
private: CAtlArray<CString> m_aCompleteFiles;
private: INT32 m_nCurTaskIndex;
private: INT32 m_nState;
private: INT32 m_nActiveConvertersCount;
private: CTimeMeasurerExt m_oWorkedTimer;
private: INT32 m_nThreadPriority;
private: CRITICAL_SECTION m_csThread;
private: INT32 m_nThreadComplete;
private: LONG m_lCurProcess;
private: CString m_sCurImagename;
private: bool m_bIsSomeChange;
private: CString  m_old_photo_masks_folder;
};