/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "RtfWriter.h"
#include "RtfDocument.h"

RtfWriter::RtfWriter( RtfDocument& oDocument , std::wstring sFilename, std::wstring sFolder ):m_oDocument(oDocument)
{
	m_sFilename = sFilename;
	m_sTempFolder = sFolder;
	m_bFirst = true;
	m_oCurTempFileWriter = NULL;
	m_oCurTempFileSectWriter = NULL;
}
RtfWriter::~RtfWriter()
{
	RELEASEOBJECT( m_oCurTempFileWriter );
	RELEASEOBJECT( m_oCurTempFileSectWriter );
	for( int i = 0; i < (int)m_aTempFiles.size(); i++ )
		Utils::RemoveDirOrFile( m_aTempFiles[i] );
	for( int i = 0; i < (int)m_aTempFilesSectPr.size(); i++ )
		Utils::RemoveDirOrFile( m_aTempFilesSectPr[i] );
	m_aTempFiles.clear();
}
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
    //std::string sRtf;
	//sRtf = CreateRtfStart();
    //m_oFileWriter->Write( (BYTE*)(LPCSTR)sRtf, sRtf.length() );
	m_bFirst = true;
	return true;
}
bool RtfWriter::SaveByItem()
{
	if( NULL == m_oCurTempFileWriter )
	{
		try
		{
            std::wstring sNewTempFile = Utils::CreateTempFile( m_sTempFolder );
			m_aTempFiles.push_back( sNewTempFile );

			RELEASEOBJECT( m_oCurTempFileWriter );
		
			m_oCurTempFileWriter = new NFileWriter::CBufferedFileWriter( sNewTempFile );

            std::wstring sNewTempFileSect = Utils::CreateTempFile( m_sTempFolder );
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

	if( m_oDocument.GetCount() > 1 && m_oDocument[0].props->GetCount() == 0 )
	{
		//пишем конец секции
        std::string sRtfExt = "\\sect";
        m_oCurTempFileWriter->Write( (BYTE*)sRtfExt.c_str(), sRtfExt.length() );
		//окончательно дописываем темповый файл
		RELEASEOBJECT( m_oCurTempFileWriter )
		try
		{
            std::wstring sNewTempFile = Utils::CreateTempFile( m_sTempFolder );
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
            std::wstring sRtf;
			if( true == m_bFirst )
			{
				//первый свойства секции пишем как свойства документа
				m_bFirst = false;
				oNewParam.nType = RENDER_TO_OOX_PARAM_FIRST_SECTION;
			}
			sRtf = m_oDocument[0].props->m_oProperty.RenderToRtf(oNewParam);
            RtfUtility::RtfInternalEncoder::Decode( sRtf, *m_oCurTempFileSectWriter );
			//дописываем в файл
			RELEASEOBJECT( m_oCurTempFileSectWriter );
			//создаем новый
            std::wstring sNewTempFileSect = Utils::CreateTempFile( m_sTempFolder );
			m_aTempFilesSectPr.push_back( sNewTempFileSect );

			m_oCurTempFileSectWriter = new NFileWriter::CBufferedFileWriter( sNewTempFileSect );

			//m_aTempFilesSectPr.push_back( sRtf );
			//RtfInternalEncoder::Decode( sRtf, *m_oFileWriter );
            //m_oFileWriter->Write( (BYTE*)(LPCSTR)sRtf, sRtf.length() );
		}
		//удаляем секцию
		m_oDocument.RemoveItem( 0 );
	}
	//пишем параграф
	if( m_oDocument.GetCount() > 0 && m_oDocument[0].props->GetCount() > 0 )
	{
        std::wstring sRtf;
		sRtf = m_oDocument[0].props->operator[](0)->RenderToRtf(oNewParam);
		
		if( TYPE_RTF_PARAGRAPH ==		m_oDocument[0].props->operator[](0)->GetType() 
								&&	!( m_oDocument[0].props->GetCount() == 0 
									&& m_oDocument.GetCount() > 1) )//для последнего параграфа секции не пишем \par
		{
			sRtf += L"\\par";
			//oNewParam.nValue = RENDER_TO_RTF_PARAM_NO_PAR;
		}
        RtfUtility::RtfInternalEncoder::Decode( sRtf, *m_oCurTempFileWriter );
        //m_oTempFileWriter->Write( (BYTE*)(LPCSTR)sRtf, sRtf.length() );

		//удаляем элемент который только что написали
		m_oDocument[0].props->RemoveItem( 0 );
	}
	return true;
}
bool RtfWriter::SaveByItemEnd()
{
	//окончательно дописываем темповый файл
	RELEASEOBJECT( m_oCurTempFileWriter );

    std::wstring sRtf;
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
			sRtf = m_oDocument[0].props->m_oProperty.RenderToRtf(oNewParam);
            RtfUtility::RtfInternalEncoder::Decode( sRtf, *m_oCurTempFileSectWriter );
			//дописываем в файл
			RELEASEOBJECT( m_oCurTempFileSectWriter );
		}
		//RtfInternalEncoder::Decode( sRtf, *m_oCurTempFileWriter );
        //m_oFileWriter->Write( (BYTE*)(LPCSTR)sRtf, sRtf.length() );

		//удаляем секцию
		m_oDocument.RemoveItem( 0 );
	}

	//формируем выходной файл
	try
	{
		NFileWriter::CBufferedFileWriter oTargetFileWriter(m_sFilename );

		//пишем заголовок потом все содежимое
		sRtf = CreateRtfStart();
		DWORD dwBytesWrite = 0;
        RtfUtility::RtfInternalEncoder::Decode( sRtf, oTargetFileWriter );
        //WriteFile ( hTargetFile, sRtf, ( DWORD ) sRtf.length(), &dwBytesWrite, NULL );

		//копируем заголовки из массива и параграфы из темповых файлов
		for (size_t i = 0 ; i < m_aTempFiles.size() && i < m_aTempFilesSectPr.size(); i++ )
		{
			//свойства секции

			NSFile::CFileBinary file;
			if (true == file.OpenFile(m_aTempFilesSectPr[i]))
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
			if (true == file.OpenFile(m_aTempFiles[i]))
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
        RtfUtility::RtfInternalEncoder::Decode( sRtf, oTargetFileWriter );
        BYTE nEndFile = 0;
		oTargetFileWriter.Write( &nEndFile, 1);
	}
	catch(...)
	{}

	for (size_t i = 0; i < m_aTempFiles.size(); i++ )
		Utils::RemoveDirOrFile( m_aTempFiles[i] );

	m_aTempFiles.clear();

	for (size_t i = 0; i < m_aTempFilesSectPr.size(); i++ )
		Utils::RemoveDirOrFile( m_aTempFilesSectPr[i] );

	m_aTempFilesSectPr.clear();
	return true;
}
int RtfWriter::GetCount()
{
	int nCount = 0;
	for (int i = 0; i < m_oDocument.GetCount(); i++ )
	{
		nCount += m_oDocument[i].props->GetCount();
	}
	return nCount;
}
std::wstring RtfWriter::CreateRtfStart()
{
	RenderParameter oRenderParameter;
	
	oRenderParameter.poDocument = &m_oDocument;
	oRenderParameter.poWriter	= this;
	oRenderParameter.nType		= RENDER_TO_RTF_PARAM_UNKNOWN;

    std::wstring sResult;
	sResult += L"{\\rtf1\\ulc1";
	sResult += m_oDocument.m_oProperty.RenderToRtf( oRenderParameter );
	sResult += m_oDocument.m_oFontTable.RenderToRtf( oRenderParameter );
	sResult += m_oDocument.m_oColorTable.RenderToRtf( oRenderParameter );
	
    std::wstring sDefCharProp = m_oDocument.m_oDefaultCharProp.RenderToRtf( oRenderParameter );
    
	if( false == sDefCharProp.empty() )
		sResult += L"{\\*\\defchp " + sDefCharProp + L"}";
    std::wstring sDefParProp = m_oDocument.m_oDefaultParagraphProp.RenderToRtf( oRenderParameter );
    
	if( false == sDefParProp.empty() )
		sResult += L"{\\*\\defpap " + sDefParProp+ L"}";
	sResult += m_oDocument.m_oStyleTable.RenderToRtf( oRenderParameter );
 	
	sResult += m_oDocument.m_oListTable.RenderToRtf			( oRenderParameter );
	sResult += m_oDocument.m_oListOverrideTable.RenderToRtf	( oRenderParameter );
	sResult += m_oDocument.m_oRevisionTable.RenderToRtf		( oRenderParameter );
	sResult += m_oDocument.m_oInformation.RenderToRtf		( oRenderParameter );

	if (m_oDocument.m_pBackground)
	{
		sResult += m_oDocument.m_pBackground->RenderToRtf ( oRenderParameter );
	}
	
	sResult += L"\\fet2";//0	Footnotes only or nothing at all (the default). 1 Endnotes only. 2	Both footnotes and endnotes
	
    std::wstring sFootnote;
	if( NULL != m_oDocument.m_oFootnoteSep )
	{
		sFootnote = m_oDocument.m_oFootnoteSep->RenderToRtf( oRenderParameter );
        if( !sFootnote.empty() )
            sResult += L"{\\*\\ftnsep " + sFootnote + L"}";
	}
	if( NULL != m_oDocument.m_oFootnoteCon )
	{
		sFootnote = m_oDocument.m_oFootnoteCon->RenderToRtf( oRenderParameter );
        if( !sFootnote.empty() )
            sResult += L"{\\*\\ftnsepc " + sFootnote + L"}";
	}
	if( NULL != m_oDocument.m_oEndnoteSep )
	{
		sFootnote = m_oDocument.m_oEndnoteSep->RenderToRtf( oRenderParameter );
        if( !sFootnote.empty() )
            sResult += L"{\\*\\aftnsep " + sFootnote + L"}";
	}
	if( NULL != m_oDocument.m_oEndnoteCon )
	{
		sFootnote = m_oDocument.m_oEndnoteCon->RenderToRtf( oRenderParameter );
        if( !sFootnote.empty() )
            sResult += L"{\\*\\aftnsepc " + sFootnote + L"}";
	}
	sResult += L"\n\n";
	return sResult;
}
std::wstring RtfWriter::CreateRtfEnd( )
{
	return L"\n}\n";
}
