#pragma once
#include "OOXStyleReader.h"
#include "OOXDocDefaultsReader.h"


	class OOXStyleTableReader
	{
	private: CString m_sFileName;

	public: OOXStyleTableReader(CString sFilename)
			{
				m_sFileName = sFilename;
			}
	public: bool Parse( ReaderParameter oParam )
			{
				XmlUtils::CXmlReader oXmlReader;
				oXmlReader.OpenFromFile( m_sFileName);
				oXmlReader.ReadRootNode( _T("w:styles") );
				oXmlReader.ReadNodeList( _T("*") );
				for(int i=0;i<oXmlReader.GetLengthList();i++)
				{
					CString sNodeName = oXmlReader.ReadNodeName(i);
					XML::IXMLDOMNodePtr oNode;
					oXmlReader.GetNode(i,oNode);
					oParam.oNode = oNode;
					if( _T("w:docDefaults") == sNodeName )
					{
						OOXDocDefaultsReader oDocDefaultsReader;
						oDocDefaultsReader.Parse( oParam );
					}
					//if( _T("w:latentStyles") == sNodeName )
					//{
					//	OOXLatentStyleReader oLatentStyleReader;
					//	oLatentStyleReader.Parse( oParam );
					//}
					if( _T("w:style") == sNodeName )
					{
						RtfStylePtr oNewStyle;
						OOXStyleReader oStyleReader;
						oStyleReader.ParseStyle( oParam, oNewStyle);
						if( NULL != oNewStyle )
							oParam.oDocument->m_oStyleTable.AddItem( oNewStyle );
					}
				}
				//только чтобы добавить связи между стилями
				for(int i=0;i<oXmlReader.GetLengthList();i++)
				{
					CString sNodeName = oXmlReader.ReadNodeName(i);
					XML::IXMLDOMNodePtr oNode;
					oXmlReader.GetNode(i,oNode);
					oParam.oNode = oNode;
					if( _T("w:style") == sNodeName )
					{
						OOXStyleReader oStyleReader;
						oStyleReader.ParseRelations( oParam );
					}
				}	
				return true;
			}
	};
