#pragma once
#include "RtfDocument.h"
#include "XmlUtils.h"
#include "RtfProperty.h"
#include "OOXFontReader.h"
#include "OOXReaderBasic.h"
#include "OOXColorReader.h"
#include "OOXShadingReader.h"
#include "OOXBorderReader.h"

	class OOXrPrReader
	{
	public: bool m_bDefStyle;
	public: OOXrPrReader()
			{
				m_bDefStyle = true;
			}
	public: bool Parse( ReaderParameter oParam ,RtfCharProperty& oOutputProperty)
			{
				XmlUtils::CXmlReader oXmlReader;
				//сначала применяем стили
				oXmlReader.OpenFromXmlNode(oParam.oNode);
				//применяем стиль по-умолчанию,если не определен свой
				if( FALSE == oXmlReader.ReadNode(_T("w:rStyle")) && true == m_bDefStyle && NULL != oParam.oReader->m_oDefCharStyle )
				{
					oOutputProperty.Merge( oParam.oReader->m_oDefCharStyle->m_oCharProp );
				}

				oXmlReader.OpenFromXmlNode(oParam.oNode);
				oXmlReader.ReadNodeList( _T("*") );

				for(int i = 0; i < oXmlReader.GetLengthList(); i++ )
				{
					CString sNodeName = oXmlReader.ReadNodeName(i);
					if( _T("w:effect") == sNodeName )
					{
						CString sEffect = oXmlReader.ReadNodeAttribute( i, _T("w:val") );
						if( _T("none") == sEffect )
							oOutputProperty.m_nAnimated = 0;
						if( _T("lights") == sEffect )
							oOutputProperty.m_nAnimated = 1;
						if( _T("blinkBackground") == sEffect )
							oOutputProperty.m_nAnimated = 2;
						if( _T("sparkle") == sEffect )
							oOutputProperty.m_nAnimated = 3;
						if( _T("antsBlack") == sEffect )
							oOutputProperty.m_nAnimated = 4;
						if( _T("antsRed") == sEffect )
							oOutputProperty.m_nAnimated = 5;
						if( _T("shimmer") == sEffect )
							oOutputProperty.m_nAnimated = 6;
					}
					else if( _T("w:rStyle") == sNodeName )
					{
						CString sStyleID = oXmlReader.ReadNodeAttribute( i, _T("w:val"), _T("") );
						RtfStylePtr oCurStyle;
						if( true == oParam.oDocument->m_oStyleTable.GetStyle(sStyleID, oCurStyle)) 
						{
							if( oCurStyle->GetType() == TYPE_RTF_PROPERTY_STYLE_CHAR )
							{
								oCurStyle = oParam.oDocument->m_oStyleTable.GetStyleResulting( oCurStyle );
								RtfCharStylePtr oCharStyle = boost::shared_static_cast<RtfCharStyle, RtfStyle>( oCurStyle );
								oOutputProperty.m_nCharStyle = oCharStyle->m_nID;
								oOutputProperty.Merge( oCharStyle->m_oCharProp );
							}
						}
					}
					COMMAND_OOX_BOOL( _T("w:b"), oOutputProperty.m_bBold, sNodeName, oXmlReader, i )
					COMMAND_OOX_BOOL( _T("w:caps"), oOutputProperty.m_bCaps, sNodeName, oXmlReader, i )
					COMMAND_OOX_INT( _T("w:w"), oOutputProperty.m_nScalex, sNodeName, oXmlReader, i )
					else if( _T("w:position") == sNodeName )
					{
						CString sParam = oXmlReader.ReadNodeAttribute( i, _T("w:val"),_T("") );
						if( _T("") != sParam )
						{
							int nValue = Strings::ToInteger( sParam );
							if( nValue >= 0 )
								oOutputProperty.m_nUp = nValue;
							else
								oOutputProperty.m_nDown = -nValue;
						}
					}
					COMMAND_OOX_BOOL( _T("w:emboss"), oOutputProperty.m_bEmbo, sNodeName, oXmlReader, i )
					COMMAND_OOX_INT( _T("w:spacing"), oOutputProperty.m_nCharacterSpacing, sNodeName, oXmlReader, i )
					else if( _T("w:fitText") == sNodeName )
					{
						CString sParam = oXmlReader.ReadNodeAttribute( i, _T("w:val"),_T("") );
						CString sID = oXmlReader.ReadNodeAttribute( i, _T("w:id"),_T("") );
						if( _T("") != sParam )
						{
							int nValue = Strings::ToInteger( sParam );
							int nID = Strings::ToInteger( sID );
							if( _T("") != sID || oParam.oReader->m_nCurFittextId == nID )
							{
								oOutputProperty.m_nFitText = -1;
							}
							else
							{
								oParam.oReader->m_nCurFittextId = nID;
								oOutputProperty.m_nFitText = nValue;
							}
						}
					}
					else if( _T("w:rFonts") == sNodeName )
					{
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode( i , oNode );
						ReaderParameter oNewParam = oParam;
						oNewParam.oNode = oNode;
						OOXFontReader2 oFontReader2;
						oFontReader2.Parse( oNewParam, oOutputProperty.m_nFont);
					}
					COMMAND_OOX_INT( _T("w:sz"), oOutputProperty.m_nFontSize, sNodeName, oXmlReader, i )
					COMMAND_OOX_BOOL( _T("w:i"), oOutputProperty.m_bItalic, sNodeName, oXmlReader, i )
					COMMAND_OOX_BOOL( _T("w:imprint"), oOutputProperty.m_bImprint, sNodeName, oXmlReader, i )
					COMMAND_OOX_INT( _T("w:kern"), oOutputProperty.m_nKerning, sNodeName, oXmlReader, i )
					COMMAND_OOX_BOOL( _T("w:rtl"), oOutputProperty.m_bRightToLeft, sNodeName, oXmlReader, i )
					COMMAND_OOX_BOOL( _T("w:cs"), oOutputProperty.m_nComplexScript, sNodeName, oXmlReader, i )
					COMMAND_OOX_BOOL( _T("w:outline"), oOutputProperty.m_bOutline, sNodeName, oXmlReader, i )
					COMMAND_OOX_BOOL( _T("w:smallCaps"), oOutputProperty.m_bScaps, sNodeName, oXmlReader, i )
					COMMAND_OOX_BOOL( _T("w:shadow"), oOutputProperty.m_bShadow, sNodeName, oXmlReader, i )
					COMMAND_OOX_BOOL( _T("w:strike"), oOutputProperty.m_bStrike, sNodeName, oXmlReader, i )
					COMMAND_OOX_BOOL( _T("w:dstrike"), oOutputProperty.m_nStriked, sNodeName, oXmlReader, i )
					else if( _T("w:vertAlign") == sNodeName )
					{
						CString sAlign = oXmlReader.ReadNodeAttribute( i , _T("w:val") ,_T("") );
						if( _T("subscript") == sAlign )
							oOutputProperty.m_bSub = 1;
						else if( _T("superscript") == sAlign )
							oOutputProperty.m_bSuper = 1;
					}
					else if( _T("w:highlight") == sNodeName )
					{

						CString sHighLight = oXmlReader.ReadNodeAttribute( i , _T("w:val"), _T("") );
						if( _T("")!= sHighLight )
						{
							RtfColor oHighLight;
							if( sHighLight == _T("black") ) {oOutputProperty.m_nHightlited = oParam.oDocument->m_oColorTable.AddItem( RtfColor(0x000000) );}
							else if( sHighLight == _T("blue") ) {oOutputProperty.m_nHightlited = oParam.oDocument->m_oColorTable.AddItem( RtfColor(0x0000FF) );}
							else if( sHighLight == _T("cyan") ) {oOutputProperty.m_nHightlited = oParam.oDocument->m_oColorTable.AddItem( RtfColor(0x00FFFF) );}
							else if( sHighLight == _T("darkBlue") ) {oOutputProperty.m_nHightlited = oParam.oDocument->m_oColorTable.AddItem( RtfColor(0x00008B) );}
							else if( sHighLight == _T("darkCyan") ) {oOutputProperty.m_nHightlited = oParam.oDocument->m_oColorTable.AddItem( RtfColor(0x008B8B) );}
							else if( sHighLight == _T("darkGray") ) {oOutputProperty.m_nHightlited = oParam.oDocument->m_oColorTable.AddItem( RtfColor(0xA9A9A9) );}
							else if( sHighLight == _T("darkGreen") ) {oOutputProperty.m_nHightlited = oParam.oDocument->m_oColorTable.AddItem( RtfColor(0x006400) );}
							else if( sHighLight == _T("darkMagenta") ) {oOutputProperty.m_nHightlited = oParam.oDocument->m_oColorTable.AddItem( RtfColor(0x800080) );}
							else if( sHighLight == _T("darkRed") ) {oOutputProperty.m_nHightlited = oParam.oDocument->m_oColorTable.AddItem( RtfColor(0x8B0000) );}
							else if( sHighLight == _T("darkYellow") ) {oOutputProperty.m_nHightlited = oParam.oDocument->m_oColorTable.AddItem( RtfColor(0x808000) );}
							else if( sHighLight == _T("green") ) {oOutputProperty.m_nHightlited = oParam.oDocument->m_oColorTable.AddItem( RtfColor(0x00FF00) );}
							else if( sHighLight == _T("lightGray") ) {oOutputProperty.m_nHightlited = oParam.oDocument->m_oColorTable.AddItem( RtfColor(0xD3D3D3) );}
							else if( sHighLight == _T("magenta") ) {oOutputProperty.m_nHightlited = oParam.oDocument->m_oColorTable.AddItem( RtfColor(0xFF00FF) );}
							else if( sHighLight == _T("red") ) {oOutputProperty.m_nHightlited = oParam.oDocument->m_oColorTable.AddItem( RtfColor(0xFF0000) );}
							else if( sHighLight == _T("white") ) {oOutputProperty.m_nHightlited = oParam.oDocument->m_oColorTable.AddItem( RtfColor(0xFFFFFF) );}
							else if( sHighLight == _T("yellow") ) {oOutputProperty.m_nHightlited = oParam.oDocument->m_oColorTable.AddItem( RtfColor(0xFFFF00) );}
						}
					}
					else if( _T("w:color") == sNodeName )
					{
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode( i , oNode );
						ReaderParameter oNewParameter = oParam;
						oNewParameter.oNode = oNode;
						OOXColorReader oColorReader;
						RtfColor oColor;
						if( true == oColorReader.Parse( oNewParameter, oColor ) )
							oOutputProperty.m_nForeColor = oParam.oDocument->m_oColorTable.AddItem( oColor );
					}
					else if( _T("w:u") == sNodeName )
					{
						CString sStyle = oXmlReader.ReadNodeAttribute( i , _T("w:val") , _T("") );
						if( _T("single") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Single;
						else if( _T("dotted") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Dotted;
						else if( _T("dash") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Dashed;
						else if( _T("dotDash") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Dash_dotted;
						else if( _T("dotDotDash") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Dash_dot_dotted;
						else if( _T("double") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Double;
						else if( _T("wavyHeavy") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Heavy_wave;
						else if( _T("dashLong") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Long_dashe;
						//else if( _T("single") == sStyle )
						//	oOutputProperty.m_eUnderStyle = uls_Stops_all;
						else if( _T("thick") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Thick;
						else if( _T("dottedHeavy") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Thick_dotted;
						else if( _T("dashedHeavy") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Thick_dashed;
						else if( _T("dashDotHeavy") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Thick_dash_dotted;
						else if( _T("dashDotDotHeavy") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Thick_dash_dot_dotted;
						else if( _T("dashLongHeavy") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Thick_long_dashed;
						else if( _T("wavyDouble") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Double_wave;
						else if( _T("words") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Word;
						else if( _T("wave") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Wave;
						else if( _T("none") == sStyle )
							oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_none;
						CString sColor = oXmlReader.ReadNodeAttribute( i, _T("w:color") );
						if( _T("") != sColor && _T("auto") != sColor )
						{
							RtfColor oColor;	
							oColor.SetHEXString( sColor );
							oOutputProperty.m_nUnderlineColor =  oParam.oDocument->m_oColorTable.AddItem( oColor );
						}
					}
					else if( _T("w:bdr") == sNodeName )
					{
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode( i , oNode );
						ReaderParameter oNewParameter = oParam;
						oNewParameter.oNode = oNode;
						OOXBorderReader oBorderReader;
						oBorderReader.Parse( oNewParameter, oOutputProperty.m_poBorder );
					}
					else if( _T("w:shd") == sNodeName )
					{
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode( i , oNode );
						ReaderParameter oNewParameter = oParam;
						oNewParameter.oNode = oNode;
						OOXShadingReader oShadingReader;
						oShadingReader.Parse( oNewParameter, oOutputProperty.m_poShading );
					}
				}
				return true;
			}
	};
