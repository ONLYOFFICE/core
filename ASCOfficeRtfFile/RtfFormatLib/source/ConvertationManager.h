#pragma once
#include "RtfReader.h"
#include "RtfDocument.h"
#include "RtfWriter.h"
#include "Writer/OOXWriter.h"
#include "Reader/OOXReader.h"


const double g_cdMaxReadRtfPercent = 0.70;
const double g_cdMaxWriteRtfPercent = 0.30;
const double g_cdMaxReadOoxPercent = 0.70;
const double g_cdMaxWriteOoxPercent = 0.30;

//#define MEMORY_SAFE_CONVERTATION


class ConvertationManager
{
public: 
	CString m_sTempFolder;

	ConvertationManager( )
	{
	}

	HRESULT ConvertRtfToOOX( CString sSrcFileName, CString sDstPath, CString sXMLOptions )
	{
		m_bParseFirstItem = true;

		RtfDocument oDocument;
		oDocument.m_oProperty.SetDefaultRtf();

		RtfReader oReader( oDocument, sSrcFileName );
		OOXWriter oWriter( oDocument, sDstPath );
		oReader.m_sTempFolder = sDstPath;
		oWriter.m_sTempFolder = sDstPath;
		m_poRtfReader = &oReader;
		m_poOOXWriter = &oWriter;

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
			m_poOOXWriter->SaveByItem();
			oWriter.SaveByItemEnd( );
		#else
			if( true == succes)
			{
				succes = oWriter.Save( );
				//ATLASSERT( true == succes );
			}
		#endif
		
		if( true == succes )
			return S_OK;
		else 
			return S_FALSE;
	}
	HRESULT ConvertOOXToRtf( CString sDstFileName, CString sSrcPath, CString sXMLOptions )
	{
		m_bParseFirstItem = true;

		RtfDocument oDocument;
		oDocument.m_oProperty.SetDefaultOOX();

		OOXReader oReader( oDocument, sSrcPath );
		RtfWriter oWriter( oDocument, sDstFileName, sSrcPath );
		
		oReader.m_sTempFolder = m_sTempFolder;
		oWriter.m_sTempFolder = m_sTempFolder;
		
		m_poOOXReader = &oReader;
		m_poRtfWriter = &oWriter;

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
			if( true == succes && true)
			{
				succes = oWriter.Save( );
				//ATLASSERT( true == succes );
				
				//succes = oWriter.Save( oDocument, CString("d:\\testOOX") );
				////ATLASSERT( true == succes );
			}
		#endif

		return S_OK;
	}

private:
	OOXWriter* m_poOOXWriter;
	OOXReader* m_poOOXReader;
	
	RtfWriter* m_poRtfWriter;
	RtfReader* m_poRtfReader;
	
	bool m_bParseFirstItem;

};