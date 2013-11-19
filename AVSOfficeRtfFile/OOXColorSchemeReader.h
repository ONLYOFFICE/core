#pragma once
#include "OOXColorReader.h"
#include "RtfProperty.h"

class OOXColorSchemeReader
{
public: OOXColorSchemeReader()
		{
		}
public: bool Parse( ReaderParameter oParam )
		{
			XmlUtils::CXmlReader oXmlReader;
			if( TRUE ==  oXmlReader.OpenFromXmlNode(oParam.oNode) )
				if( TRUE == oXmlReader.ReadNodeList( _T("*") ) )
				{
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						CString sXml = oXmlReader.ReadNodeXml(i);
						CString sNodeName = oXmlReader.ReadNodeName(i);
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode(i,oNode);
						oParam.oNode = oNode;

						RtfColor oNewColor; 
						OOXColorReader oColorReader;

						oColorReader.Parse( oParam, oNewColor);
						if( _T("a:accent1") == sNodeName )
							oNewColor.m_eTheme = RtfColor::caccentone;
						else if( _T("a:accent2") == sNodeName )
							oNewColor.m_eTheme = RtfColor::caccenttwo;
						else if( _T("a:accent3") == sNodeName )
							oNewColor.m_eTheme = RtfColor::caccentthree;
						else if( _T("a:accent4") == sNodeName )
							oNewColor.m_eTheme = RtfColor::caccentfour;
						else if( _T("a:accent5") == sNodeName )
							oNewColor.m_eTheme = RtfColor::caccentfive;
						else if( _T("a:accent6") == sNodeName )
							oNewColor.m_eTheme = RtfColor::caccentsix;
						else if( _T("a:bg1") == sNodeName )
							oNewColor.m_eTheme = RtfColor::cbackgroundone;
						else if( _T("a:bg2") == sNodeName )
							oNewColor.m_eTheme = RtfColor::cbackgroundtwo;
						else if( _T("a:dk1") == sNodeName )
							oNewColor.m_eTheme = RtfColor::cmaindarkone;
						else if( _T("a:dk2") == sNodeName )
							oNewColor.m_eTheme = RtfColor::cmaindarktwo;
						else if( _T("a:folHlink") == sNodeName )
							oNewColor.m_eTheme = RtfColor::cfollowedhyperlink;
						else if( _T("a:hlink") == sNodeName )
							oNewColor.m_eTheme = RtfColor::chyperlink;
						else if( _T("a:lt1") == sNodeName )
							oNewColor.m_eTheme = RtfColor::cmainlightone;
						else if( _T("a:lt2") == sNodeName )
							oNewColor.m_eTheme = RtfColor::cmainlightone;
						else if( _T("a:phClr") == sNodeName )
							oNewColor.m_eTheme = RtfColor::cmainlighttwo;
						else if( _T("a:tx1") == sNodeName )
							oNewColor.m_eTheme = RtfColor::ctextone;
						else if( _T("a:tx2") == sNodeName )
							oNewColor.m_eTheme = RtfColor::ctexttwo;
						else
							continue;
						oParam.oDocument->m_oColorTable.AddItem( oNewColor );
					}
					return true;
				}
			return false;
		}
};