#pragma once
#include "OOXLevelReader.h"


class OOXAbstractNumReader
{
public: OOXAbstractNumReader()
		{
		}
public: bool Parse( ReaderParameter oParam )
		{
			XmlUtils::CXmlReader oXmlReader;
			if( TRUE ==  oXmlReader.OpenFromXmlNode(oParam.oNode) )
			{
				RtfListProperty oNewList;

				COMMAND_OOX_INT_ATTRIBUTE_2( _T("w:abstractNumId"), oNewList.m_nID, oXmlReader )

				oXmlReader.ReadNodeList( _T("*") );
				for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
				{
					CString sNodeName = oXmlReader.ReadNodeName( i );
					if( _T("w:multiLevelType") == sNodeName )
					{
						CString sLevelType = oXmlReader.ReadNodeAttribute( i, _T("w:val") );
						if( _T("hybridMultilevel") == sLevelType )
							oNewList.m_bListHybrid = 1;
						else if( _T("singleLevel") == sLevelType )
							oNewList.m_nListSimple = 1;
						//else if( _T("multilevel") == sLevelType )
						//	oNewList.m_nListSimple = 0;
					}
					else if( _T("w:name") == sNodeName )
						oNewList.m_sName = oXmlReader.ReadNodeAttribute(i, _T("w:val") );
					COMMAND_OOX_INT( _T("w:tmpl"), oNewList.m_nTemplateId, sNodeName, oXmlReader, i )
					else if( _T("w:lvl") == sNodeName )
					{
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode(i,oNode);
						oParam.oNode = oNode;
						OOXLevelReader oLevelReader;
						RtfListLevelProperty oNewLevel;
						if( true == oLevelReader.Parse( oParam, oNewLevel) )
							oNewList.AddItem( oNewLevel );
					}
				}
				oParam.oDocument->m_oListTabel.AddItem( oNewList );
			}
			return true;
		}
};