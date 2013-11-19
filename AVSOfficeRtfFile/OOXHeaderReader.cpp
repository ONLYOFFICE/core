#include "stdafx.h"
#include "OOXHeaderReader.h"
#include "OOXTextItemReader.h"
#include "OOXRelsReader.h"

bool OOXHeaderReader::Parse( ReaderParameter oParam, CString sRID, TextItemContainerPtr& oOutput, bool bHeader )
		{
			//ищем файл в rels
			CString sRelativeFilename = oParam.oRels->GetByID( sRID );
			CString sFilename = oParam.oReader->m_sDocumentPath + _T("\\") + sRelativeFilename;
			//провер€ем наличие соответствующих rels
			CString sRelsPath = oParam.oReader->m_sDocumentPath + _T("\\_rels\\") + sRelativeFilename + _T(".rels");
			OOXRelsReaderPtr oRelsReader;
			if( ::GetFileAttributes( sRelsPath ) != DWORD(-1) )
			{
				oRelsReader = OOXRelsReaderPtr( new OOXRelsReader() );
				oRelsReader->Init( sRelsPath );
			}
			oParam.oRels = oRelsReader;

			XmlUtils::CXmlReader oXmlReader;
			if( TRUE == oXmlReader.OpenFromFile( sFilename ) )
			{
				if( true == bHeader )
				{	
					oXmlReader.ReadRootNode( _T("w:hdr") );
				}
				else
				{
					oXmlReader.ReadRootNode( _T("w:ftr") );
				}

				OOXTextItemReader oOOXTextItemReader;
				oOutput = TextItemContainerPtr( new TextItemContainer() );
				oOOXTextItemReader.m_oTextItems = oOutput;
				XML::IXMLDOMNodePtr oNode;
				oXmlReader.GetNode(oNode);
				ReaderParameter oNewParam = oParam;
				oNewParam.oNode = oNode;
				oOOXTextItemReader.Parse( oNewParam );
				return true;
			}
			return false;
		}
