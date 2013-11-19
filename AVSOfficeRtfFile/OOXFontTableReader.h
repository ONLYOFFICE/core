#pragma once
#include "OOXFontReader.h"


	class OOXFontTableReader
	{
	private: CString m_sFileName;

	public: OOXFontTableReader(CString sFilename)
			{
				m_sFileName = sFilename;
			}
	public: bool Parse( ReaderParameter oParam )
			{
				XmlUtils::CXmlReader oXmlReader;
				oXmlReader.OpenFromFile( m_sFileName);
				oXmlReader.ReadRootNode( _T("w:fonts") );
				oXmlReader.ReadNodeList( _T("w:font") );
				for(int i=0;i<oXmlReader.GetLengthList();i++)
				{
					XML::IXMLDOMNodePtr oNode;
					oXmlReader.GetNode(i,oNode);
					oParam.oNode = oNode;
					OOXFontReader oFontReader;
					RtfFont oNewFont;
					if( true == oFontReader.Parse( oParam, oNewFont ) )
						oParam.oDocument->m_oFontTable.AddItem(oNewFont);
					
				}	
				return true;
			}
	};
