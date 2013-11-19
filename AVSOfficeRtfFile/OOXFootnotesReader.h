#pragma once
#include "OOXReader.h"
#include "RtfDocument.h"
#include <limits.h>

class OOXFootnotesReader
{
public: OOXFootnotesReader()
		{
		}
public: bool Parse( ReaderParameter oParam, CString sFileName, bool bFootnote )
		{
			//выделяем только имя файла
			int nSlashIndex = sFileName.ReverseFind( '\\' );
			CString sName = sFileName.Right( sFileName.GetLength() - nSlashIndex  - 1) ;


			//проверяем наличие соответствующих rels
			CString sRelsPath = oParam.oReader->m_sDocumentPath + _T("\\_rels\\") + sName + _T(".rels");
			OOXRelsReaderPtr oRelsReader;
			if( ::GetFileAttributes( sRelsPath ) != DWORD(-1) )
			{
				oRelsReader = OOXRelsReaderPtr( new OOXRelsReader() );
				oRelsReader->Init( sRelsPath );
			}
			oParam.oRels = oRelsReader;

			int nSeparatorId = 0;
			int nContinueId = 1;


			XmlUtils::CXmlReader oXmlReader;
			oXmlReader.OpenFromFile( sFileName );
			if( true == bFootnote )
			{
				oXmlReader.ReadRootNode( _T("w:footnotes") );
				oXmlReader.ReadNodeList( _T("w:footnote") );

				int nMin1 = INT_MAX;
				if( oParam.oDocument->m_oProperty.m_aSpecialFootnotes.GetCount() > 0 )
				{
					int nMin1 = INT_MAX;
					int nMin2 = INT_MAX;
					for( int i = 0; i < (int)oParam.oDocument->m_oProperty.m_aSpecialFootnotes.GetCount(); i++ )
						if( nMin1 > oParam.oDocument->m_oProperty.m_aSpecialFootnotes[i] )
						{
							nMin2 = nMin1;
							nMin1 = oParam.oDocument->m_oProperty.m_aSpecialFootnotes[i];
						}
					if( INT_MAX != nMin1 )
						nSeparatorId = nMin1;
					if( INT_MAX != nMin2 )
						nContinueId = nMin1;
				}
			}
			else
			{
				oXmlReader.ReadRootNode( _T("w:endnotes") );
				oXmlReader.ReadNodeList( _T("w:endnote") );

				int nMin1 = INT_MAX;
				if( oParam.oDocument->m_oProperty.m_aSpecialEndnotes.GetCount() > 0 )
				{
					int nMin1 = INT_MAX;
					int nMin2 = INT_MAX;
					for( int i = 0; i < (int)oParam.oDocument->m_oProperty.m_aSpecialEndnotes.GetCount(); i++ )
						if( nMin1 > oParam.oDocument->m_oProperty.m_aSpecialEndnotes[i] )
						{
							nMin2 = nMin1;
							nMin1 = oParam.oDocument->m_oProperty.m_aSpecialEndnotes[i];
						}
					if( INT_MAX != nMin1 )
						nSeparatorId = nMin1;
					if( INT_MAX != nMin2 )
						nContinueId = nMin1;
				}
			}


			OOXTextItemReader oTextItemReader;
			for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
			{
				XML::IXMLDOMNodePtr oNode;
				oXmlReader.GetNode( i, oNode );
				ReaderParameter oNewParam = oParam;
				oNewParam.oNode = oNode;
				CString sId = oXmlReader.ReadNodeAttribute( i, _T("w:id") );
				if( _T("") != sId )
				{
					int nId = Strings::ToInteger( sId );
					if( nSeparatorId == nId )
					{
						TextItemContainerPtr oNewTextItem( new TextItemContainer() );
						oTextItemReader.m_oTextItems = oNewTextItem;
						if( true == oTextItemReader.Parse( oNewParam ) )
						{
							if( true == bFootnote )
								oParam.oDocument->m_oFootnoteSep = oNewTextItem;
							else
								oParam.oDocument->m_oEndnoteSep = oNewTextItem;
						}
					}
					else if( nContinueId == nId )
					{
						TextItemContainerPtr oNewTextItem( new TextItemContainer() );
						oTextItemReader.m_oTextItems = oNewTextItem;
						if( true == oTextItemReader.Parse( oNewParam ) )
						{
							if( true == bFootnote )
								oParam.oDocument->m_oFootnoteCon = oNewTextItem;
							else
								oParam.oDocument->m_oEndnoteCon = oNewTextItem;
						}
					}
					else
					{
						TextItemContainerPtr oNewTextItem( new TextItemContainer() );
						oTextItemReader.m_oTextItems = oNewTextItem;
						if( true == oTextItemReader.Parse( oNewParam ) )
						{
							if( true == bFootnote )
								oParam.oReader->m_mapFootnotes.SetAt( nId, oNewTextItem );
							else
								oParam.oReader->m_mapEndnotes.SetAt( nId, oNewTextItem );
						}
					}
				}
			}
			return true;
		}
};
