
#include "RtfWriter.h"
#include "RtfDocument.h"

bool RtfWriter::Save()
{
	int nItemsCount = GetCount();
	SaveByItemStart();
	for( int i = 0; i < nItemsCount; i++ )
	{
		SaveByItem();
	}
	return SaveByItemEnd();

}
bool RtfWriter::SaveByItemStart()
{
	//try{
	//	m_sTempFileResult = Utils::CreateTempFile( m_sTempFolder );
	//	BSTR bstrFileName = m_sTempFileResult.AllocSysString();
	//	m_oFileWriter = new NFileWriter::CBufferedFileWriter( bstrFileName );
	//	SysFreeString( bstrFileName );
	//	//создаем темповый файл куда пишем параграфы( потом копируем свойства секции и содержимое файла )
	//	m_sTempFile = Utils::CreateTempFile( m_sTempFolder );
	//	BSTR bstrTempFileName = m_sTempFile.AllocSysString();
	//	m_oTempFileWriter = new NFileWriter::CBufferedFileWriter( bstrTempFileName );
	//	SysFreeString( bstrTempFileName );
	//}
	//catch(...)
	//{
	//	return false;
	//}
	//CStringA sRtf;
	//sRtf = CreateRtfStart();
	//m_oFileWriter->Write( (BYTE*)(LPCSTR)sRtf, sRtf.GetLength() );
	m_bFirst = true;
	return true;
}
bool RtfWriter::SaveByItem()
{
	if( NULL == m_oCurTempFileWriter )
	{
		try
		{
			CString sNewTempFile = Utils::CreateTempFile( m_sTempFolder );
			m_aTempFiles.push_back( sNewTempFile );

			RELEASEOBJECT( m_oCurTempFileWriter );
		
			m_oCurTempFileWriter = new NFileWriter::CBufferedFileWriter( sNewTempFile );

			CString sNewTempFileSect = Utils::CreateTempFile( m_sTempFolder );
			m_aTempFilesSectPr.push_back( sNewTempFileSect );

			RELEASEOBJECT( m_oCurTempFileSectWriter );

			m_oCurTempFileSectWriter = new NFileWriter::CBufferedFileWriter( sNewTempFileSect );
		}
		catch(...)
		{
			return false;
		}
	}
	RenderParameter oNewParam;
	oNewParam.poDocument = &m_oDocument;
	oNewParam.poWriter = this;
	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

	if( m_oDocument.GetCount() > 1 && m_oDocument[0]->GetCount() == 0 )
	{
		//пишем конец секции
		CStringA sRtfExt = "\\sect";
		m_oCurTempFileWriter->Write( (BYTE*)(LPCSTR)sRtfExt, sRtfExt.GetLength() );
		//окончательно дописываем темповый файл
		RELEASEOBJECT( m_oCurTempFileWriter )
		try
		{
			CString sNewTempFile = Utils::CreateTempFile( m_sTempFolder );
			m_aTempFiles.push_back( sNewTempFile );

			m_oCurTempFileWriter = new NFileWriter::CBufferedFileWriter( sNewTempFile );
		}
		catch(...)
		{
			return false;
		}
		if( NULL != m_oCurTempFileSectWriter )
		{
			//пишем свойства секции
			CString sRtf;
			if( true == m_bFirst )
			{
				//первый свойства секции пишем как свойства документа
				m_bFirst = false;
				oNewParam.nType = RENDER_TO_OOX_PARAM_FIRST_SECTION;
			}
			sRtf = m_oDocument[0]->m_oProperty.RenderToRtf(oNewParam);
			RtfInternalEncoder::Decode( sRtf, *m_oCurTempFileSectWriter );
			//дописываем в файл
			RELEASEOBJECT( m_oCurTempFileSectWriter );
			//создаем новый
			CString sNewTempFileSect = Utils::CreateTempFile( m_sTempFolder );
			m_aTempFilesSectPr.push_back( sNewTempFileSect );

			m_oCurTempFileSectWriter = new NFileWriter::CBufferedFileWriter( sNewTempFileSect );

			//m_aTempFilesSectPr.push_back( sRtf );
			//RtfInternalEncoder::Decode( sRtf, *m_oFileWriter );
			//m_oFileWriter->Write( (BYTE*)(LPCSTR)sRtf, sRtf.GetLength() );
		}
		//удаляем секцию
		m_oDocument.RemoveItem( 0 );
	}
	//пишем параграф
	if( m_oDocument.GetCount() > 0 && m_oDocument[0]->GetCount() > 0 )
	{
		CString sRtf;
		sRtf = m_oDocument[0]->operator[](0)->RenderToRtf(oNewParam);
		if( TYPE_RTF_PARAGRAPH == m_oDocument[0]->operator[](0)->GetType() && !(m_oDocument[0]->GetCount() == 0 && m_oDocument.GetCount() > 1) )//для последнего параграфа секции не пишем \par
		{
			sRtf.Append( _T("\\par") );
			//oNewParam.nValue = RENDER_TO_RTF_PARAM_NO_PAR;
		}
		RtfInternalEncoder::Decode( sRtf, *m_oCurTempFileWriter );
		//m_oTempFileWriter->Write( (BYTE*)(LPCSTR)sRtf, sRtf.GetLength() );

		//удаляем элемент который только что написали
		m_oDocument[0]->RemoveItem( 0 );
	}
	return true;
}
bool RtfWriter::SaveByItemEnd()
{
	//окончательно дописываем темповый файл
	RELEASEOBJECT( m_oCurTempFileWriter );

	CString sRtf;
	if( m_oDocument.GetCount() > 0 )
	{
		RenderParameter oNewParam;
		oNewParam.poDocument = &m_oDocument;
		oNewParam.poWriter = this;
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

		if( NULL != m_oCurTempFileSectWriter )
		{
			//пишем последнюю секцию
			if( true == m_bFirst )
			{
				//первый свойства секции пишем как свойства документа
				m_bFirst = false;
				oNewParam.nType = RENDER_TO_OOX_PARAM_FIRST_SECTION;
			}
			sRtf = m_oDocument[0]->m_oProperty.RenderToRtf(oNewParam);
			RtfInternalEncoder::Decode( sRtf, *m_oCurTempFileSectWriter );
			//дописываем в файл
			RELEASEOBJECT( m_oCurTempFileSectWriter );
		}
		//RtfInternalEncoder::Decode( sRtf, *m_oCurTempFileWriter );
		//m_oFileWriter->Write( (BYTE*)(LPCSTR)sRtf, sRtf.GetLength() );

		//удаляем секцию
		m_oDocument.RemoveItem( 0 );
	}

	//формируем выходной файл
	try
	{
		NFileWriter::CBufferedFileWriter oTargetFileWriter( m_sFilename );

		//пишем заголовок потом все содежимое
		sRtf = CreateRtfStart();
		DWORD dwBytesWrite = 0;
		RtfInternalEncoder::Decode( sRtf, oTargetFileWriter );
		//WriteFile ( hTargetFile, sRtf, ( DWORD ) sRtf.GetLength(), &dwBytesWrite, NULL );

		//копируем заголовки из массива и параграфы из темповых файлов
		for( int i = 0 ; i < (int)m_aTempFiles.size() && i < (int)m_aTempFilesSectPr.size(); i++ )
		{
			//свойства секции

			CFile file;
			if (file.OpenFile(m_aTempFilesSectPr[i]) == S_OK)
			{
				DWORD dwBytesRead = 1;
				BYTE byteBuffer[ 4096 ];

				while( 0 != dwBytesRead )
				{
					dwBytesRead  = file.GetPosition();
					file.ReadFile( byteBuffer, 4096);
					dwBytesRead  = file.GetPosition() - dwBytesRead;

					oTargetFileWriter.Write( byteBuffer,  dwBytesRead );
				}
				file.CloseFile();
			}
			//параграфы
			if (file.OpenFile(m_aTempFiles[i]) == S_OK)
			{
				DWORD dwBytesRead = 1;
				BYTE byteBuffer[ 4096 ];

				while( 0 != dwBytesRead )
				{
					dwBytesRead  = file.GetPosition();
					file.ReadFile(byteBuffer, 4096);
					dwBytesRead  = file.GetPosition() - dwBytesRead;
					
					oTargetFileWriter.Write( byteBuffer,  dwBytesRead );
				}
				file.CloseFile();
			}
		}

		//завершаем документ
		sRtf = CreateRtfEnd();
		RtfInternalEncoder::Decode( sRtf, oTargetFileWriter );
		byte nEndFile = 0;
		oTargetFileWriter.Write( &nEndFile, 1);
	}
	catch(...)
	{}

	for( int i = 0; i < (int)m_aTempFiles.size(); i++ )
		Utils::RemoveDirOrFile( m_aTempFiles[i] );
	m_aTempFiles.clear();
	for( int i = 0; i < (int)m_aTempFilesSectPr.size(); i++ )
		Utils::RemoveDirOrFile( m_aTempFilesSectPr[i] );
	m_aTempFilesSectPr.clear();
	return true;
}
int RtfWriter::GetCount()
{
	int nCount = 0;
	for( int i = 0; i < m_oDocument.GetCount(); i++ )
		nCount += m_oDocument[i]->GetCount();
	return nCount;
}
CString RtfWriter::CreateRtfStart()
{
	RenderParameter oRenderParameter;
	oRenderParameter.poDocument = &m_oDocument;
	oRenderParameter.poWriter = this;
	oRenderParameter.nType = RENDER_TO_RTF_PARAM_UNKNOWN;

	CString sResult;
	sResult.Append( _T("{\\rtf1\\ulc1") );
	sResult.Append( m_oDocument.m_oProperty.RenderToRtf( oRenderParameter ) );
	sResult.Append( m_oDocument.m_oFontTable.RenderToRtf( oRenderParameter ) );
	sResult.Append( m_oDocument.m_oColorTable.RenderToRtf( oRenderParameter ) );
	//CString sDefCharProp = m_oDocument.m_oDefaultCharProp.RenderToRtf( oRenderParameter );
	//if( false == sDefCharProp.IsEmpty() )
	//	sResult.AppendFormat( _T("{\\*\\defchp %ls}"), sDefCharProp);
	//CString sDefParProp = m_oDocument.m_oDefaultParagraphProp.RenderToRtf( oRenderParameter );
	//if( false == sDefParProp.IsEmpty() )
	//	sResult.AppendFormat( _T("{\\*\\defpap %ls}"),sDefParProp );
	//sResult.Append( m_oDocument.m_oStyleTable.RenderToRtf( oRenderParameter ) );
	sResult.Append( m_oDocument.m_oListTabel.RenderToRtf( oRenderParameter ) );
	sResult.Append( m_oDocument.m_oListOverrideTabel.RenderToRtf( oRenderParameter ) );
	sResult.Append( m_oDocument.m_oInformation.RenderToRtf( oRenderParameter ) );
	sResult.Append( _T("\\fet2") );//0	Footnotes only or nothing at all (the default). 1 Endnotes only. 2	Both footnotes and endnotes
	CString sFootnote;
	if( NULL != m_oDocument.m_oFootnoteSep )
	{
		sFootnote = m_oDocument.m_oFootnoteSep->RenderToRtf( oRenderParameter );
		if( _T("") != sFootnote )
			sResult.AppendFormat( _T("{\\*\\ftnsep %ls}"), sFootnote );
	}
	if( NULL != m_oDocument.m_oFootnoteCon )
	{
		sFootnote = m_oDocument.m_oFootnoteCon->RenderToRtf( oRenderParameter );
		if( _T("") != sFootnote )
			sResult.AppendFormat( _T("{\\*\\ftnsepc %ls}"), sFootnote );
	}
	if( NULL != m_oDocument.m_oEndnoteSep )
	{
		sFootnote = m_oDocument.m_oEndnoteSep->RenderToRtf( oRenderParameter );
		if( _T("") != sFootnote )
			sResult.AppendFormat( _T("{\\*\\aftnsep %ls}"), sFootnote );
	}
	if( NULL != m_oDocument.m_oEndnoteCon )
	{
		sFootnote = m_oDocument.m_oEndnoteCon->RenderToRtf( oRenderParameter );
		if( _T("") != sFootnote )
			sResult.AppendFormat( _T("{\\*\\aftnsepc %ls}"), sFootnote );
	}

	sResult.Append(_T("\n\n"));
	return sResult;
}

CString RtfWriter::CreateRtfEnd( )
{
	return _T("\n}\n");
}
