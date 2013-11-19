#pragma once
#include "RtfDocument.h"
#include "XmlUtils.h"
#include "StringUtils.h"
#include "RtfProperty.h"
#include "Utils.h"

	class OOXLevelReader
	{
	public: OOXLevelReader()
			{
			}
	public: bool Parse( ReaderParameter oParam, RtfListLevelProperty& oLevelProperty )
			{
				XmlUtils::CXmlReader oXmlReader;
				if( TRUE == oXmlReader.OpenFromXmlNode(oParam.oNode) )
				{
					COMMAND_OOX_INT_ATTRIBUTE_2( _T("w:ilvl"), oLevelProperty.m_nLevel, oXmlReader )
					COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("w:tentative"), oLevelProperty.m_bTentative, oXmlReader )

					oXmlReader.ReadNodeList( _T("*") );
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						CString sNodeName = oXmlReader.ReadNodeName( i );

						if( _T("w:lvlJc") == sNodeName )
						{
							CString sJust =  oXmlReader.ReadNodeAttribute( i, _T("w:val") );
							if( _T("start") == sJust )
								oLevelProperty.m_nJustification = 0;
							else if( _T("center") == sJust )
								oLevelProperty.m_nJustification = 1;
							else if( _T("end") == sJust )
								oLevelProperty.m_nJustification = 2;
							else if( _T("right") == sJust )
								oLevelProperty.m_nJustification = 2;
							else if( _T("left") == sJust )
								oLevelProperty.m_nJustification = 0;
						}
						else if( _T("w:lvlRestart") == sNodeName )
						{
							CString sVal = oXmlReader.ReadNodeAttribute(i, _T("w:val") );
							if(  _T("") != sVal )
							{
								int nVal = Strings::ToInteger( sVal );
								if( 0 == nVal )
									oLevelProperty.m_nNoRestart = 0;
							}
						}
						COMMAND_OOX_BOOL( _T("w:isLgl"), oLevelProperty.m_nLegal, sNodeName, oXmlReader, i )
						else if( _T("w:lvlText") == sNodeName )
						{
							CString sText ;
							sText= oXmlReader.ReadNodeAttribute(i, _T("w:val"), _T("") );
							oLevelProperty.SetLevelTextOOX( sText );
						}
						else if( _T("w:numFmt") == sNodeName )
						{
							CString sText ;
							sText = oXmlReader.ReadNodeAttribute(i, _T("w:val"), _T("") );
							oLevelProperty.m_nNumberType = oLevelProperty.GetFormat( sText );
						}
						else if( _T("w:lvlPicBulletId") == sNodeName )
						{
							int nID = Strings::ToInteger( oXmlReader.ReadNodeAttribute(i, _T("w:val"), _T("-1") ) );
							CAtlMap<int, int>::CPair* poPair =  oParam.oReader->m_mapPictureBullet.Lookup( nID );
							if( NULL != poPair )
							{
								oLevelProperty.m_nPictureIndex = poPair->m_value;
							}
						}
						COMMAND_OOX_INT( _T("w:start"), oLevelProperty.m_nStart, sNodeName, oXmlReader, i )
						else if( _T("w:suff") == sNodeName )
						{
							CString sVal = oXmlReader.ReadNodeAttribute( i, _T("w:val") );
							if( _T("tab") == sVal )
								oLevelProperty.m_nFollow = 0;
							else if( _T("space") == sVal )
								oLevelProperty.m_nFollow = 1;
							else if( _T("nothing") == sVal )
								oLevelProperty.m_nFollow = 2;

						}
						else if( sNodeName == _T("w:ind") )
						{
							CString nLeft = oXmlReader.ReadNodeAttribute(i, _T("w:left") );
							CString nFirst = oXmlReader.ReadNodeAttribute(i, _T("w:firstLine") );
							if( _T("") != nFirst )
								oLevelProperty.m_nFirstIndent = Strings::ToInteger( nFirst );			
							if( _T("") != nLeft )
								oLevelProperty.m_nIndent = Strings::ToInteger( nLeft );		
						}
						else if( sNodeName == _T("w:rPr") )
						{
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode( i, oNode );
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oNode;
							RtfCharProperty oNewProperty;
							oNewProperty.SetDefaultOOX();
							OOXrPrReader orPrReader;
							orPrReader.m_bDefStyle = false;
							if( true == orPrReader.Parse( oNewParam, oNewProperty ) )
								oLevelProperty.m_oCharProp = oNewProperty;
						}
						else if(sNodeName == _T("w:pPr") )
						{
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode(i,oNode);
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oNode;
							RtfParagraphProperty oNewProperty;
							OOXpPrReader opPrReader;
							bool bStartNewSectoion;
							if( true == opPrReader.Parse( oNewParam, oNewProperty, CcnfStyle(), bStartNewSectoion ) )
							{
								oLevelProperty.m_nFirstIndent = oNewProperty.m_nIndFirstLine;
								oLevelProperty.m_nIndent = oNewProperty.m_nIndLeft;
								oLevelProperty.m_nIndentStart = oNewProperty.m_nIndStart;
								oLevelProperty.m_oTabs = oNewProperty.m_oTabs;
							}
						}
					}
					return true;
				}
				return false;
			}
	};
