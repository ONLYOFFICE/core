#pragma once
#include "RtfDocument.h"
#include "../Common/XmlUtils.h"
#include "OOXLevelReader.h"

	class OOXNumberingMapReader
	{
	public: OOXNumberingMapReader()
			{
			}
	public: bool Parse( ReaderParameter oParam )
			{
				XmlUtils::CXmlReader oXmlReader;
				if( TRUE ==  oXmlReader.OpenFromXmlNode(oParam.oNode) )
				{
					CString sXml = oXmlReader.ReadNodeXml();
					RtfListOverrideProperty oNewOverride;
					COMMAND_OOX_INT_ATTRIBUTE_2( _T("w:numId"), oNewOverride.m_nIndex, oXmlReader )


					oXmlReader.ReadNodeList( _T("*") );
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						CString sNodeName = oXmlReader.ReadNodeName( i );
						if( sNodeName == _T("w:abstractNumId") )
							COMMAND_OOX_INT_ATTRIBUTE( _T("w:val"), oNewOverride.m_nListID, oXmlReader, i )
						else if( sNodeName == _T("w:lvlOverride") )
						{
							CString sOverrideLevel = oXmlReader.ReadNodeAttribute( i, _T("w:ilvl") );

							RtfListOverrideProperty::ListOverrideLevels::ListOverrideLevel oOverrideLevel;
							if( _T("") != sOverrideLevel )
								oOverrideLevel.m_nLevelIndex = Strings::ToInteger( sOverrideLevel );
							else
								oOverrideLevel.m_nLevelIndex = 0;
							
							XML::IXMLDOMNodePtr oSubNode;
							oXmlReader.GetNode( i, oSubNode );

							XmlUtils::CXmlReader oXmlSubReader;
							if( TRUE == oXmlSubReader.OpenFromXmlNode( oSubNode ) )
								if( TRUE == oXmlSubReader.ReadNodeList( _T("*") ) )
								{
									for( int j = 0; j < oXmlSubReader.GetLengthList(); j++ )
									{
										CString sSubNodeName = oXmlSubReader.ReadNodeName( j );
										if( _T("w:startOverride") == sSubNodeName )
										{
											CString sStartOverride = oXmlSubReader.ReadNodeAttribute( j, _T("w:val") );
											if( _T("") != sStartOverride )
												oOverrideLevel.m_nStart = Strings::ToInteger( sStartOverride );
										}
										else if( _T("w:lvl") == sSubNodeName )
										{
											XML::IXMLDOMNodePtr oSubSubNode;
											oXmlSubReader.GetNode( j, oSubSubNode );

											ReaderParameter oNewParam = oParam;
											oNewParam.oNode = oSubSubNode;

											OOXLevelReader oLevelReader;
											oLevelReader.Parse( oNewParam, oOverrideLevel.m_oLevel );
										}
									}
								}
								oNewOverride.m_oOverrideLevels.m_aOverrideLevels.Add( oOverrideLevel );
						}
					}

					oParam.oDocument->m_oListOverrideTabel.AddItem( oNewOverride );
					return true;
				}
				return false;
			}
	};
