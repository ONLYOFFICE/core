#pragma once
#include "OOXReader.h"
#include "RtfDocument.h"
#include "OOXReaderBasic.h"
#include "OOXcnfStyleReader.h"

	class OOXtrPrReader
	{
	public: OOXtrPrReader()
			{
			}
	public: bool Parse( ReaderParameter oParam , RtfRowProperty& oOutputProperty, CcnfStyle& oConditionalTableStyle)
			{
				XmlUtils::CXmlReader oXmlReader;
				//ищем cnfStyle  и применяем внешний стиль
				oXmlReader.OpenFromXmlNode(oParam.oNode);
				if( NULL != oParam.poTableStyle )
				{
					if( TRUE == oXmlReader.ReadNode( _T("w:cnfStyle") ) )
					{
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode( oNode );
						ReaderParameter oNewParam = oParam;
						oNewParam.oNode = oNode;
						CcnfStyle ocnfStyle;
						OOXcnfStyleReader ocnfStyleReader;
						ocnfStyleReader.Parse( oNewParam, ocnfStyle );

						oConditionalTableStyle.Merge( ocnfStyle );
					}
				}
				RtfTableStylePtr oResultStyle = oConditionalTableStyle.ApplyTableStyle( oParam.poTableStyle );
				if( NULL != oResultStyle )
					oOutputProperty.Merge( oResultStyle->m_oRowProp );


				if( TRUE == oXmlReader.OpenFromXmlNode(oParam.oNode) )
				{
					oXmlReader.ReadNodeList( _T("*") );
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						CString sNodeName = oXmlReader.ReadNodeName(i);
						if( _T("123456789") == sNodeName )
							;
						COMMAND_OOX_BOOL( _T("w:tblHeader"), oOutputProperty.m_bIsHeader, sNodeName, oXmlReader, i )
						COMMAND_OOX_BOOL( _T("w:cantSplit"), oOutputProperty.m_bIsHeader, sNodeName, oXmlReader, i )
						else if( _T("w:jc") == sNodeName )
						{
							CString sVal = oXmlReader.ReadNodeAttribute(i, _T("w:val"), _T("") );
							if( _T("left") == sVal )
								oOutputProperty.m_eJust = RtfRowProperty::rj_trql;
							else if( _T("right") == sVal )
								oOutputProperty.m_eJust = RtfRowProperty::rj_trql;
							else if( _T("center") == sVal )
								oOutputProperty.m_eJust = RtfRowProperty::rj_trqc;
						}
						else if( _T("w:trHeight") == sNodeName )
						{
							CString sType = oXmlReader.ReadNodeAttribute(i, _T("w:val"), _T("") );
							CString sValue = oXmlReader.ReadNodeAttribute(i, _T("w:val"), _T("") );
							if( _T("") != sValue )
							{
								if( _T("exact") == sType )
									oOutputProperty.m_nHeight = - Strings::ToInteger( sValue );
								else
									oOutputProperty.m_nHeight = Strings::ToInteger( sValue );
							}
						}
						else if( _T("w:wBefore") == sNodeName )
							COMMAND_READ_MARGIN( oXmlReader, i, oOutputProperty.m_eMUStartInvCell, oOutputProperty.m_nWidthStartInvCell )
						else if( _T("w:wAfter") == sNodeName )
							COMMAND_READ_MARGIN( oXmlReader, i, oOutputProperty.m_eMUEndInvCell, oOutputProperty.m_nWidthEndInvCell )
						COMMAND_OOX_INT( _T("w:gridBefore"), oOutputProperty.m_nGridBefore, sNodeName, oXmlReader, i )
						COMMAND_OOX_INT( _T("w:gridAfter"), oOutputProperty.m_nGridAfter, sNodeName, oXmlReader, i )
					}
				}
				return false;
			}

	};
