#pragma once
#include "RtfReader.h"
#include "RtfDocument.h"
#include "RtfWriter.h"
#include "OOXWriter.h"
#include "OOXReader.h"


const double g_cdMaxReadRtfPercent = 0.70;
const double g_cdMaxWriteRtfPercent = 0.30;
const double g_cdMaxReadOoxPercent = 0.70;
const double g_cdMaxWriteOoxPercent = 0.30;

#define MEMORY_SAFE_CONVERTATION

[event_receiver(native)]
class ConvertationManager{
private: IEventSource* m_poEventSource;
private: long m_nCurrentProgress;
private: RtfReader* m_poRtfReader;
private: OOXWriter* m_poOOXWriter;
private: OOXReader* m_poOOXReader;
private: RtfWriter* m_poRtfWriter;
private: bool m_bStop;
private: bool m_bParseFirstItem;
public: ConvertationManager( IEventSource* poEventSource ):m_poEventSource(poEventSource)
		{
			m_nCurrentProgress = -1;
		}

public: HRESULT ConvertRtfToOOX( CString sSrcFileName, CString sDstPath, CString sXMLOptions )
		{
			m_bStop = false;
			m_bParseFirstItem = true;
			m_nCurrentProgress = -1;

			RtfDocument oDocument;
			oDocument.m_oProperty.SetDefaultRtf();

			RtfReader oReader( oDocument, sSrcFileName );
			OOXWriter oWriter( oDocument, sDstPath );
			oReader.m_sTempFolder = sDstPath;
			oWriter.m_sTempFolder = sDstPath;
			m_poRtfReader = &oReader;
			m_poOOXWriter = &oWriter;

			hookEventsRtfReader( &oReader );
			hookEventsOOXWriter( &oWriter );

			bool succes = oReader.Load( );
			//ATLASSERT( true == succes );
			#ifdef MEMORY_SAFE_CONVERTATION
			//сохранение будет поэлементое в обработчике OnCompleteItemRtfReader
			//надо только завершить
				if( true == m_bParseFirstItem )
				{
					m_bParseFirstItem = false;
					oWriter.SaveByItemStart( );
				}
				oWriter.SaveByItemEnd( );
			#else
				if( true == succes && true != m_bStop )
				{
					succes = oWriter.Save( );
					//ATLASSERT( true == succes );
				}
			#endif
			unhookEventsRtfReader( &oReader );
			unhookEventsOOXWriter( &oWriter );

			if( true == succes )
				return S_OK;
			else 
				return S_FALSE;
		}
public: HRESULT ConvertOOXToRtf( CString sDstFileName, CString sSrcPath, CString sXMLOptions )
		{
			m_bStop = false;
			m_bParseFirstItem = true;
			m_nCurrentProgress = -1;

			RtfDocument oDocument;
			oDocument.m_oProperty.SetDefaultOOX();

			OOXReader oReader( oDocument, sSrcPath );
			RtfWriter oWriter( oDocument, sDstFileName, sSrcPath );
			oReader.m_sTempFolder = sSrcPath;
			oWriter.m_sTempFolder = sSrcPath;
			m_poOOXReader = &oReader;
			m_poRtfWriter = &oWriter;
			hookEventsOOXReader( &oReader );
			hookEventsRtfWriter( &oWriter );

			bool succes = oReader.Parse( );
			//ATLASSERT( true == succes );

			#ifdef MEMORY_SAFE_CONVERTATION
				//сохранение будет поэлементое в обработчике OnCompleteItemRtfReader
				//надо только завершить
				if( true == m_bParseFirstItem )
				{
					m_bParseFirstItem = false;
					oWriter.SaveByItemStart( );
				}
				oWriter.SaveByItemEnd( );
			#else
				if( true == succes && true != m_bStop )
				{
					succes = oWriter.Save( );
					//ATLASSERT( true == succes );
					
					//succes = oWriter.Save( oDocument, CString("d:\\testOOX") );
					////ATLASSERT( true == succes );
				}
			#endif
			unhookEventsOOXReader( &oReader );
			unhookEventsRtfWriter( &oWriter );
			return S_OK;
		}
private : void OnCompleteItemRtfReader()
		  {
			#ifdef MEMORY_SAFE_CONVERTATION
			if( true == m_bParseFirstItem )
			{
				m_bParseFirstItem = false;
				m_poOOXWriter->SaveByItemStart( );
			}
			m_poOOXWriter->SaveByItem();

			short shCancel = c_shProgressContinue;
			m_poEventSource->RaiseOnProcess( m_poRtfReader->GetProgress(), &shCancel );	
			if( c_shProgressCancel == shCancel )
			{
				m_bStop = true;
				m_poRtfReader->Stop();
			}
			#endif
		  }
private : void OnCompleteItemOOXReader()
		  {
			#ifdef MEMORY_SAFE_CONVERTATION
  			if( true == m_bParseFirstItem )
			{
				m_bParseFirstItem = false;
				m_poRtfWriter->SaveByItemStart( );
			}
			m_poRtfWriter->SaveByItem( );
			#endif
		  }
private : void OnProgressRtfReader( long nProgress, short* shCancel )
{
	#ifdef MEMORY_SAFE_CONVERTATION
		if( m_nCurrentProgress != nProgress )
		{
			m_nCurrentProgress = nProgress;
			short nCancel = c_shProgressContinue;
			m_poEventSource->RaiseOnProcess( m_nCurrentProgress, &nCancel );	
			if( c_shProgressCancel == nCancel )
			{
				m_bStop = true;
				(*shCancel) = c_shProgressCancel;
			}
		}
	#else

		long nCurrentProgress = (long)(g_cdMaxReadRtfPercent * nProgress);
		if( m_nCurrentProgress != nCurrentProgress )
		{
			m_nCurrentProgress = nCurrentProgress;
			short nCancel = c_shProgressContinue;
			m_poEventSource->RaiseOnProcess( m_nCurrentProgress, &nCancel );	
			if( c_shProgressCancel == nCancel )
			{
				m_bStop = true;
				(*shCancel) = c_shProgressCancel;
			}
		}
	#endif
}
private : void OnProgressOOXWriter( long nProgress, short* shCancel )
{
		#ifdef MEMORY_SAFE_CONVERTATION	
			if( m_nCurrentProgress != nProgress )
			{
				m_nCurrentProgress = nProgress;
				short nCancel = c_shProgressContinue;
				m_poEventSource->RaiseOnProcess( m_nCurrentProgress, &nCancel );	
				if( c_shProgressCancel == nCancel )
				{
					m_bStop = true;
					(*shCancel) = c_shProgressCancel;
				}
			}
		#else
			long nCurrentProgress = (long)(g_cdMaxReadRtfPercent * g_cdMaxPercent + g_cdMaxWriteOoxPercent * nProgress);
			if( m_nCurrentProgress != nCurrentProgress )
			{
				m_nCurrentProgress = nCurrentProgress;
				short nCancel = c_shProgressContinue;
				m_poEventSource->RaiseOnProcess( m_nCurrentProgress, &nCancel );	
				if( c_shProgressCancel == nCancel )
				{
					m_bStop = true;
					(*shCancel) = c_shProgressCancel;
				}
			}
		#endif
}
private : void OnProgressOOXReader( long nProgress, short* shCancel )
{
		#ifdef MEMORY_SAFE_CONVERTATION	
			if( m_nCurrentProgress != nProgress )
			{
				m_nCurrentProgress = nProgress;
				short nCancel = c_shProgressContinue;
				m_poEventSource->RaiseOnProcess( m_nCurrentProgress, &nCancel );	
				if( c_shProgressCancel == nCancel )
				{
					m_bStop = true;
					(*shCancel) = c_shProgressCancel;
				}
			}
		#else
			long nCurrentProgress = (long)(g_cdMaxReadOoxPercent * nProgress);
			if( m_nCurrentProgress != nCurrentProgress )
			{
				m_nCurrentProgress = nCurrentProgress;
				short nCancel = c_shProgressContinue;
				m_poEventSource->RaiseOnProcess( m_nCurrentProgress, &nCancel );
				if( c_shProgressCancel == nCancel )
				{
					m_bStop = true;
					(*shCancel) = c_shProgressCancel;
				}
			}
		#endif
}
private : void OnProgressRtfWriter( long nProgress, short* shCancel )
{
		#ifdef MEMORY_SAFE_CONVERTATION	
			if( m_nCurrentProgress != nProgress )
			{
				m_nCurrentProgress = nProgress;
				short nCancel = c_shProgressContinue;
				m_poEventSource->RaiseOnProcess( m_nCurrentProgress, &nCancel );	
				if( c_shProgressCancel == nCancel )
				{
					m_bStop = true;
					(*shCancel) = c_shProgressCancel;
				}
			}
		#else
			long nCurrentProgress = (long)(g_cdMaxReadOoxPercent * g_cdMaxPercent + g_cdMaxWriteRtfPercent * nProgress);
			if( m_nCurrentProgress != nCurrentProgress )
			{
				m_nCurrentProgress = nCurrentProgress;
				short nCancel = c_shProgressContinue;
				m_poEventSource->RaiseOnProcess( m_nCurrentProgress, &nCancel );	
				if( c_shProgressCancel == nCancel )
				{
					m_bStop = true;
					(*shCancel) = c_shProgressCancel;
				}
			}
		#endif
}
private: void hookEventsRtfReader(RtfReader* pSource)
		{
			__hook( &RtfReader::OnCompleteItem,	pSource, &ConvertationManager::OnCompleteItemRtfReader);
		}

private: void unhookEventsRtfReader(RtfReader* pSource) 
		{
			__unhook( &RtfReader::OnCompleteItem,	pSource, &ConvertationManager::OnCompleteItemRtfReader);
		 }
 private: void hookEventsOOXWriter(OOXWriter* pSource)
		{
			__hook( &OOXWriter::OnProgress,		pSource, &ConvertationManager::OnProgressOOXWriter);
		}
 private: void unhookEventsOOXWriter(OOXWriter* pSource)
		{
			__unhook( &OOXWriter::OnProgress,		pSource, &ConvertationManager::OnProgressOOXWriter);
		}

 private: void hookEventsOOXReader(OOXReader* pSource)
		{
			__hook( &OOXReader::OnCompleteItem,	pSource, &ConvertationManager::OnCompleteItemOOXReader);
			__hook( &OOXReader::OnProgress,		pSource, &ConvertationManager::OnProgressOOXReader);
		}
 private: void unhookEventsOOXReader(OOXReader* pSource)
		{
			__unhook( &OOXReader::OnCompleteItem,	pSource, &ConvertationManager::OnCompleteItemOOXReader);
			__unhook( &OOXReader::OnProgress,		pSource, &ConvertationManager::OnProgressOOXReader);
		}
 private: void hookEventsRtfWriter(RtfWriter* pSource)
		{
			__hook( &RtfWriter::OnProgress,		pSource, &ConvertationManager::OnProgressRtfWriter);
		}
 private: void unhookEventsRtfWriter(RtfWriter* pSource)
		{
			__unhook( &RtfWriter::OnProgress,		pSource, &ConvertationManager::OnProgressRtfWriter);
		}
};