#pragma once
#include "DocumentConverter.h"
#include "Errors.h"
#include "TimeMeasurer.h"
#include "StringUtils.h"


struct FileAndProcess
{
	int nId;
	LONG lProcess;
};
[event_receiver(native)]
class  DocumentConverterManager
{

//----------------------------- Public Members --------------------------------

//----------------------------- Public Methods --------------------------------
public: DocumentConverterManager()
		{
			InitializeCriticalSection(&m_csThread);
			m_nState = DOCUMENTCONVERTER_STATE_IDLE ;
			m_nThreadPriority = THREAD_PRIORITY_NORMAL;
			m_nConvertersCount = -1;
			Clear();
		}
public: ~DocumentConverterManager()
		{
			DeleteCriticalSection(&m_csThread);
			for( INT32 i=0; i < m_nConvertersCount; i++ )
				unhookEvent(&m_aConverters[i]);
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
			return m_nConvertersCount;
		}
public: VOID SetTreadCount(INT32 nCount)
		{
			if(  m_nConvertersCount != nCount )
			{
				for( INT32 i=0; i < m_nConvertersCount; i++ )
				{
					unhookEvent(&m_aConverters[i]);
				}

				m_aConverters.RemoveAll();
				m_aCurConvertIndexes.RemoveAll();

				m_nConvertersCount = nCount;
				m_aConverters.SetCount( nCount );
				m_aCurConvertIndexes.SetCount( nCount );

				for( INT32 i=0; i < m_nConvertersCount; i++ )
				{
					m_aConverters[i].SetThreadPriority( m_nThreadPriority );
					hookEvent(&m_aConverters[i]);
					m_aCurConvertIndexes[i] = -1;
				}
			}
		}
public: VOID SetThreadPriority(LONG NewVal)
		{
			m_nThreadPriority = NewVal;
			for(INT32 i=0; i < (INT32)m_nConvertersCount; i++)
			{
				m_aConverters[i].SetThreadPriority( NewVal );
			}
		}
public: DOUBLE GetEstimatedTime()
		{
			if( m_lCurProcess <= 0 || m_lCurProcess > c_nMaxProgressPercent )
				return -1;
			else
				return GetElapsedTime() * (c_nMaxProgressPercent - m_lCurProcess) / m_lCurProcess;
		}
public: DOUBLE GetElapsedTime()
		{
			return (DOUBLE)m_oWorkedTimer.GetTimeInterval();
		}
public: long GetProcess()
		{
			return m_lCurProcess;
		}
public: BOOL Suspend()
		{
			CTemporaryCS oCS(&m_csThread);
			if( DOCUMENTCONVERTER_STATE_RUN == m_nState )
			{
				BOOL bIsSuspended = FALSE;
				for( INT32 i=0; i < (INT32)m_aConverters.GetCount(); i++)
				{
					if( FALSE == m_aConverters[i].IsSuspended() )
					{
						m_aConverters[i].Suspend();
						bIsSuspended = TRUE;
					}
				}
				// считаем успешной, если удалось остановить хотябы 1 поток
				if( TRUE == bIsSuspended )
				{
					m_oWorkedTimer.Suspend();
					m_nState = DOCUMENTCONVERTER_STATE_SUSPENDED;
					return TRUE;
				}
			}
			return FALSE;
		}
public: BOOL Resume()
		{	
			CTemporaryCS oCS(&m_csThread);
			if( DOCUMENTCONVERTER_STATE_SUSPENDED == m_nState )
			{
				BOOL bIsResumed = FALSE;
				for(INT32 i=0; i < (INT32)m_aConverters.GetCount(); i++)
				{
					if( TRUE == m_aConverters[i].IsSuspended() )
					{
						m_aConverters[i].Resume();
						bIsResumed = TRUE;
					}
				}
				// считаем успешной, если удалось возобновить хотябы 1 поток
				if( TRUE == bIsResumed )
				{
					m_nState = DOCUMENTCONVERTER_STATE_RUN;
					m_oWorkedTimer.Resume();
					return TRUE;
				}
			}
			return FALSE;
		}
public: VOID Start()
		{
			BOOL bResult = TRUE;
			m_oWorkedTimer.Reset();
			//стартуем конвертеры
			for(INT32 i=0; i < m_nActiveTheads; i++)
				m_aConverters[ i ].Start( m_nThreadPriority );
			m_nState = DOCUMENTCONVERTER_STATE_RUN;
		}
public: BOOL Stop( bool bWait = false )
		{
			if( DOCUMENTCONVERTER_STATE_IDLE != m_nState )
			{
				Resume();

				for(INT32 i=0; i < (INT32)m_aConverters.GetCount(); i++)
				{
					m_aConverters[i].Stop( bWait );
				}

				m_nState = DOCUMENTCONVERTER_STATE_IDLE;
			}
			return TRUE;
		}
public: VOID SetEventSource( HANDLE& hEventSource )
		{
			m_hEventSource = hEventSource;
		}
public: VOID SetTempDirectory( CString sTempDir )
		{
			for(INT32 i=0; i < (INT32)m_aConverters.GetCount(); i++)
				m_aConverters[ i ].SetTempDirectory( sTempDir );
		}
public: VOID SetCache( IAVSOfficeFilesCache* piFilesCache )
		{
			for(INT32 i=0; i < (INT32)m_aConverters.GetCount(); i++)
				m_aConverters[ i ].SetCache( piFilesCache );
		}
public: VOID Clear()
		{
			m_nActiveTheads = m_nConvertersCount;
			m_lCurProcess = 0;
			m_aFilesStarted.RemoveAll();
			m_aFilesComplete.RemoveAll();
			m_aTasks.RemoveAll();
			m_nCurTaskIndex = 0;
			m_nCompleteCount = 0;
			ResetEvent( m_hEventSource );
			for( INT32 i=0; i < m_nConvertersCount; i++ )
			{
				m_aConverters[i].Clear();
				m_aCurConvertIndexes[i] = -1;
			}
		}
public: bool GetErrors(CAtlArray<CError>* aInputArray)
		{
			CTemporaryCS oCS(&m_csThread);
			aInputArray->RemoveAll();
			INT32 nCount = (INT32)m_aErrors.GetCount();
			if( nCount > 0 )
			{
				aInputArray->Append(m_aErrors);
				m_aErrors.RemoveAll();
			}
			return nCount > 0;
		}
public: bool GetErrorsNoCritical(CAtlArray<CError>* aInputArray)
		{
			CTemporaryCS oCS(&m_csThread);
			aInputArray->RemoveAll();
			INT32 nCount = (INT32)m_aErrorsNoCritical.GetCount();
			if( nCount > 0 )
			{
				aInputArray->Append(m_aErrorsNoCritical);
				m_aErrorsNoCritical.RemoveAll();
			}
			return nCount > 0;
		}
public: bool GetFilesStarted(CAtlArray<int>* aInputArray)
		{
			CTemporaryCS oCS(&m_csThread);
			aInputArray->RemoveAll();
			INT32 nCount = (INT32)m_aFilesStarted.GetCount();
			if( nCount > 0 )
			{
				aInputArray->Append(m_aFilesStarted);
				m_aFilesStarted.RemoveAll();
			}
			return nCount > 0;
		}
public: bool GetFilesComplete(CAtlArray<int>* aInputArray)
		{
			CTemporaryCS oCS(&m_csThread);
			aInputArray->RemoveAll();
			INT32 nCount = (INT32)m_aFilesComplete.GetCount();
			if( nCount > 0 )
			{
				aInputArray->Append(m_aFilesComplete);
				m_aFilesComplete.RemoveAll();
			}
			return nCount > 0;
		}
public: bool GetFilesProcess(CAtlArray<FileAndProcess>* aInputArray)
		{
			CTemporaryCS oCS(&m_csThread);
			aInputArray->RemoveAll();
			INT32 nCount = (INT32)m_aFilesProcess.GetCount();
			if( nCount > 0 )
			{
				aInputArray->Append(m_aFilesProcess);
				m_aFilesProcess.RemoveAll();
			}
			return nCount > 0;
		}
public: LONG AddDocuments(XmlUtils::CXmlReader& oReader)
		{
			LONG nError = 0;
			if ( m_nConvertersCount > oReader.GetLengthList() )
				m_nActiveTheads = oReader.GetLengthList();
			else
				m_nActiveTheads = m_nConvertersCount;

			for(INT32 i=0; i < oReader.GetLengthList(); i++)
			{
				XML::IXMLDOMNodePtr oNode;
				oReader.GetNode(i,oNode);

				DocumentItem oNewDocument;
				XmlUtils::CXmlReader oTaskReader;
				oTaskReader.OpenFromXmlNode( oNode );

				if( TRUE == oTaskReader.ReadNode( CString(_T("Sources")) ) )
				{
					if( TRUE == oTaskReader.ReadNodeList( CString(_T("Source"))) )
					{
						for(INT32 i = 0, length = oTaskReader.GetLengthList(); i < length; i++)
						{
							XML::IXMLDOMNodePtr oNode;
							if( TRUE == oTaskReader.GetNode(i,oNode))
							{
								XmlUtils::CXmlReader oSubReader;
								if( TRUE == oSubReader.OpenFromXmlNode(oNode) )
								{
									Source oNewSource;
									oNewSource.sFilename = oSubReader.ReadNodeAttribute( CString(_T("filename")),CString(_T("")));
									oNewSource.nId = Strings::ToInteger( oSubReader.ReadNodeAttribute( CString(_T("id")),CString(_T("0")) ) );
									oNewSource.nFileType = Strings::ToInteger( oSubReader.ReadNodeAttribute( CString(_T("filetype")),CString(_T("-1")) ) );
									oNewSource.nStartPage = Strings::ToInteger( oSubReader.ReadNodeAttribute( CString(_T("startpage")),CString(_T("-1")) ) );
									oNewSource.nEndPage = Strings::ToInteger( oSubReader.ReadNodeAttribute( CString(_T("endpage")),CString(_T("-1")) ) );
									
									if( TRUE == oSubReader.ReadNode( CString(_T("Options")) ))
										if( TRUE == oSubReader.ReadNodeList( CString(_T("*")) ))
										{
											for( int i = 0; i < oSubReader.GetLengthList(); i++ )
												oNewSource.sOptions.Append( oSubReader.ReadNodeXml( i ) );
										}
									oNewDocument.aSources.Add(oNewSource);
								}
							}
						}
					}
				}
				else
				{
					oTaskReader.OpenFromXmlNode( oNode );
					if( TRUE == oTaskReader.ReadNode( CString(_T("Source"))) )
					{
						Source oNewSource;
						oNewSource.sFilename = oTaskReader.ReadNodeAttribute( CString(_T("filename")),CString(_T("")));
						oNewSource.nId = Strings::ToInteger( oTaskReader.ReadNodeAttribute( CString(_T("id")),CString(_T("0")) ) );
						oNewSource.nFileType = Strings::ToInteger( oTaskReader.ReadNodeAttribute( CString(_T("filetype")),CString(_T("-1")) ) );
						oNewSource.nStartPage = Strings::ToInteger( oTaskReader.ReadNodeAttribute( CString(_T("startpage")),CString(_T("-1")) ) );
						oNewSource.nEndPage = Strings::ToInteger( oTaskReader.ReadNodeAttribute( CString(_T("endpage")),CString(_T("-1")) ) );

						if( TRUE == oTaskReader.ReadNode( CString(_T("Options")) ))
							if( TRUE == oTaskReader.ReadNodeList( CString(_T("*")) ))
							{
								for( int i = 0; i < oTaskReader.GetLengthList(); i++ )
									oNewSource.sOptions.Append( oTaskReader.ReadNodeXml( i ) );
							}
						oNewDocument.aSources.Add(oNewSource);
					}
				}
				oTaskReader.OpenFromXmlNode( oNode );
				if( TRUE == oTaskReader.ReadNode( CString(_T("Destinations")) ) )
				{
					if( TRUE == oTaskReader.ReadNodeList( CString(_T("Destination")) ))
					{
						for(INT32 i = 0; i < oTaskReader.GetLengthList(); i++)
						{
							XML::IXMLDOMNodePtr oNode;
							if( TRUE == oTaskReader.GetNode(i,oNode))
							{
								XmlUtils::CXmlReader oSubReader;
								if( TRUE == oSubReader.OpenFromXmlNode(oNode) )
								{
									Destination oNewDest;
									oNewDest.sFilename =  oSubReader.ReadNodeAttribute( CString(_T("filename")),CString(_T("")));
									oNewDest.nFileType = Strings::ToInteger( oSubReader.ReadNodeAttribute( CString(_T("filetype")),CString(_T("-1")) ) );

									if( TRUE == oSubReader.ReadNode( CString(_T("Options")) ))
										if( TRUE == oSubReader.ReadNodeList( CString(_T("*")) ))
										{
											for(INT32 i = 0; i < oSubReader.GetLengthList(); i++)
												oNewDest.sOptions.Append( oSubReader.ReadNodeXml(i) );
										}
									oNewDocument.aDestinations.Add( oNewDest );
								}
							}
						}
					}
				}

				if( FALSE == oNewDocument.IsValid() )
					return AVS_UNIVERSALDOCUMENTCONVERTER_ERROR_XML_FORMAT;
				m_aTasks.Add(oNewDocument);
				
			}
			return nError;
		}
public: bool GetIsComplete()
		{
			return m_nCompleteCount == m_nActiveTheads;
		}
public: void Lock()
		{
			EnterCriticalSection( &m_csThread );
		}
public: void unLock()
		{
			LeaveCriticalSection( &m_csThread );
		}
//----------------------------- Private Methods --------------------------------
private: void hookEvent(DocumentConverter* pSource)
		{
			__hook( &DocumentConverter::OnError,					pSource, &DocumentConverterManager::OnError);
			__hook( &DocumentConverter::OnDestinationFileStart,		pSource, &DocumentConverterManager::OnDestinationFileStart);
			__hook( &DocumentConverter::OnDestinationFileComplete,	pSource, &DocumentConverterManager::OnDestinationFileComplete);
			__hook( &DocumentConverter::OnGetTask,					pSource, &DocumentConverterManager::OnGetTask);
			__hook( &DocumentConverter::OnProcessFile,				pSource, &DocumentConverterManager::OnProcessFile);
			__hook( &DocumentConverter::OnComplete,					pSource, &DocumentConverterManager::OnComplete );
			__hook( &DocumentConverter::OnErrorNoCritical,			pSource, &DocumentConverterManager::OnErrorNoCritical );

		}

private: void unhookEvent(DocumentConverter* pSource) 
		{
			__unhook( &DocumentConverter::OnError,					pSource, &DocumentConverterManager::OnError);
			__unhook( &DocumentConverter::OnDestinationFileStart,	pSource, &DocumentConverterManager::OnDestinationFileStart);
			__unhook( &DocumentConverter::OnDestinationFileComplete,pSource, &DocumentConverterManager::OnDestinationFileComplete);
			__unhook( &DocumentConverter::OnGetTask,				pSource, &DocumentConverterManager::OnGetTask);
			__unhook( &DocumentConverter::OnProcessFile,			pSource, &DocumentConverterManager::OnProcessFile);
			__unhook( &DocumentConverter::OnComplete,					pSource, &DocumentConverterManager::OnComplete );
			__unhook( &DocumentConverter::OnErrorNoCritical,		pSource, &DocumentConverterManager::OnErrorNoCritical );

		}
private:void OnProcess()
		{
			long nTempSum = 0;
			for(INT32 i=0; i < m_nActiveTheads; i++)
				nTempSum += m_aConverters[i].GetProcess();

			if( m_nCurTaskIndex < m_nActiveTheads || m_aTasks.GetCount() == 0 )
				m_lCurProcess = 0;
			else
				m_lCurProcess = (LONG)(1.0*( c_nMaxProgressPercent * (m_nCurTaskIndex - m_nActiveTheads) + nTempSum  ) /  m_aTasks.GetCount());
			SetEvent( m_hEventSource );
			return;
		}
private:void OnProcessFile( int nId, LONG nProcess )
		{
			CTemporaryCS oCS(&m_csThread);
			bool bExist = false;
			for( int j = 0; j < (INT32)m_aFilesProcess.GetCount(); j++ )
				if( m_aFilesProcess[j].nId == nId )
				{
					m_aFilesProcess[j].lProcess = (LONG)( nProcess );
					bExist = true;
					break;
				}
			if( false == bExist )
			{
				FileAndProcess oNewItem;
				oNewItem.nId = nId;
				oNewItem.lProcess = (LONG)( nProcess );
				m_aFilesProcess.Add( oNewItem );
			}
			OnProcess();
			SetEvent( m_hEventSource );
		}
private:void OnErrorNoCritical(LONG dValue, int nId)
		{
			CTemporaryCS oCS( &m_csThread );
			CError oNewError;
			oNewError.SetID( dValue );
			oNewError.SetSeverity( 0 );
			//oNewError.SetFilename( CString( sFilename ) );
			oNewError.SetFileID( nId );
			m_aErrorsNoCritical.Add( oNewError );
			SetEvent( m_hEventSource );
		}
private:void OnError(LONG dValue, int nId)
		{
			CTemporaryCS oCS( &m_csThread );
			CError oNewError;
			oNewError.SetID( dValue );
			oNewError.SetFileID( nId );
			oNewError.SetSeverity( 1 );
			//oNewError.SetFilename( CString(sFilename) );

			m_aErrors.Add( oNewError );
			Suspend();
			SetEvent( m_hEventSource );
		}
private:void OnDestinationFileStart(int nId)
		{
			CTemporaryCS oCS(&m_csThread);
			m_aFilesStarted.Add( nId );
			SetEvent( m_hEventSource );
		}
private:void OnDestinationFileComplete(int nId)
		{
			CTemporaryCS oCS(&m_csThread);
			m_aFilesComplete.Add( nId );
			SetEvent( m_hEventSource );
		}
private:bool OnGetTask( DocumentItem& oNewTask )
		{
			CTemporaryCS oCS(&m_csThread);
			SetEvent( m_hEventSource );
			m_nCurTaskIndex++;
			if( m_nCurTaskIndex - 1 < (INT32)m_aTasks.GetCount() )
			{
				oNewTask = m_aTasks[ m_nCurTaskIndex - 1 ];
				return true;
			}
			return false;
		}
private:void OnComplete()
		{
			CTemporaryCS oCS(&m_csThread);
			SetEvent( m_hEventSource );
			m_nCompleteCount++;
			if( m_nCompleteCount == m_nActiveTheads )
			{
				m_nState = DOCUMENTCONVERTER_STATE_IDLE;
			}
		}
//----------------------------- Private Members -------------------------------
private: CAtlArray< DocumentConverter > m_aConverters;
private: CAtlArray< INT32 > m_aCurConvertIndexes;
private: INT32 m_nState;
private: INT32 m_nConvertersCount;
private: CTimeMeasurerExt m_oWorkedTimer;
private: BOOL m_bProcessChange;	
private: INT32 m_nThreadPriority;
private: INT32 m_nActiveTheads;
private: LONG m_lCurProcess;
private: CAtlArray< int > m_aFilesStarted;
private: CAtlArray< int > m_aFilesComplete;
private: CAtlArray< FileAndProcess > m_aFilesProcess;
private: CAtlArray< CError > m_aErrors;
private: CAtlArray< CError > m_aErrorsNoCritical;
private: CAtlArray< DocumentItem > m_aTasks;
private: INT32 m_nCurTaskIndex;
private: INT32 m_nCompleteCount;
private: HANDLE m_hEventSource;
private: CRITICAL_SECTION m_csThread;
};