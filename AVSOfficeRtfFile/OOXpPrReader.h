#pragma once
#include "OOXShadingReader.h"
#include "OOXBorderReader.h"
#include "OOXReaderBasic.h"
#include "OOXpPrFrameReader.h"
#include "OOXpPrTabReader.h"
#include "OOXcnfStyleReader.h"
#include "OOXpPrFrameReader.h"
#include "OOXrPrReader.h"
#include "OOXSectionPropertyReader.h"

#include "OOXcnfStyleReader.h"


	class OOXpPrReader
	{
	public: bool m_bDefStyle;
	public: OOXpPrReader()
			{
				m_bDefStyle = true;
			}
	public: bool Parse( ReaderParameter oParam ,RtfParagraphProperty& oOutputProperty, CcnfStyle& oConditionalTableStyle, bool& bStartNewSection )
			{
				XmlUtils::CXmlReader oXmlReader;
				//применяем внешний стиль таблицы
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
				{
					oOutputProperty.Merge( oResultStyle->m_oParProp );
					oOutputProperty.m_oCharProperty.Merge( oResultStyle->m_oCharProp );
				}
				//применяем стиль по-умолчанию,если не определен свой
				oXmlReader.OpenFromXmlNode(oParam.oNode);
				if( FALSE == oXmlReader.ReadNode(_T("w:pStyle")) && true == m_bDefStyle && NULL != oParam.oReader->m_oDefParStyle )
				{
					oOutputProperty.Merge( oParam.oReader->m_oDefParStyle->m_oParProp );
					oOutputProperty.m_nStyle = oParam.oReader->m_oDefParStyle->m_nID;
				}
				//применяем все остальные свойчтва direct formating
				oXmlReader.OpenFromXmlNode(oParam.oNode);
				oXmlReader.ReadNodeList(_T("*"));
				for(int i=0;i< oXmlReader.GetLengthList();i++)
				{
					CString sNodeName = oXmlReader.ReadNodeName(i);
					if( _T("1234567890") == sNodeName )
						;
					else if( _T("w:pStyle") == sNodeName )
					{
						CString sText = oXmlReader.ReadNodeXml();
						CString sStyleName = oXmlReader.ReadNodeAttribute( i, _T("w:val"), _T("") );
						RtfStylePtr oCurStyle;
						if( true == oParam.oDocument->m_oStyleTable.GetStyle(sStyleName, oCurStyle) )
							if( oCurStyle->GetType() == TYPE_RTF_PROPERTY_STYLE_PARAGRAPH )
							{
								RtfStylePtr oResultStyle = oParam.oDocument->m_oStyleTable.GetStyleResulting( oCurStyle );
								RtfParagraphStylePtr oParagraphStyle = boost::shared_static_cast<RtfParagraphStyle, RtfStyle>( oResultStyle );
								oOutputProperty.Merge( oParagraphStyle->m_oParProp );
								oOutputProperty.m_oCharProperty.Merge( oParagraphStyle->m_oCharProp );
								oOutputProperty.m_nStyle = oCurStyle->m_nID;
							}
					}
					else if( _T("w:suppressAutoHyphens") == sNodeName )
					{
						CString sParam = oXmlReader.ReadNodeAttribute( i, _T("w:val"), _T("") );
						if( _T("false") == sParam || _T("0") == sParam || _T("off") == sParam )
							oOutputProperty.m_bAutoHyphenation = 1;
						else
							oOutputProperty.m_bAutoHyphenation = 0;
					}
					COMMAND_OOX_BOOL( _T("w:keepLines"), oOutputProperty.m_bKeep, sNodeName, oXmlReader, i)
					COMMAND_OOX_BOOL( _T("w:keepNext"), oOutputProperty.m_bKeepNext, sNodeName, oXmlReader, i)
					COMMAND_OOX_INT( _T("w:outlineLvl"), oOutputProperty.m_nOutlinelevel, sNodeName, oXmlReader, i)
					COMMAND_OOX_BOOL( _T("w:pageBreakBefore"), oOutputProperty.m_bPageBB, sNodeName, oXmlReader, i)
					else if( _T("w:jc") == sNodeName )
					{
						CString sAlign = oXmlReader.ReadNodeAttribute(i, _T("w:val"),_T(""));
						if( _T("center") == sAlign ) { oOutputProperty.m_eAlign = RtfParagraphProperty::pa_qc; }
						else if( _T("both") == sAlign ) { oOutputProperty.m_eAlign = RtfParagraphProperty::pa_qj; }
						else if( _T("left") == sAlign ) { oOutputProperty.m_eAlign = RtfParagraphProperty::pa_ql; }
						else if( _T("right") == sAlign ) { oOutputProperty.m_eAlign = RtfParagraphProperty::pa_qr; }
						else if( _T("distribute") == sAlign ) { oOutputProperty.m_eAlign = RtfParagraphProperty::pa_qd; }
						else if( _T("lowKashida") == sAlign ) { oOutputProperty.m_eAlign = RtfParagraphProperty::pa_qk0; }
						else if( _T("mediumKashida") == sAlign ) { oOutputProperty.m_eAlign = RtfParagraphProperty::pa_qk10; }
						else if( _T("highKashida") == sAlign ) { oOutputProperty.m_eAlign = RtfParagraphProperty::pa_qk20; }
					}
					else if( _T("w:textAlignment") == sNodeName )
					{
						CString sAlign = oXmlReader.ReadNodeAttribute(i, _T("w:val"),_T(""));
						if( _T("auto") == sAlign ) { oOutputProperty.m_eFontAlign = RtfParagraphProperty::fa_faauto; }
						else if( _T("top") == sAlign ) { oOutputProperty.m_eFontAlign = RtfParagraphProperty::fa_fahang; }
						else if( _T("center") == sAlign ) { oOutputProperty.m_eFontAlign = RtfParagraphProperty::fa_facenter; }
						else if( _T("baseline") == sAlign ) { oOutputProperty.m_eFontAlign = RtfParagraphProperty::fa_faroman; }
						else if( _T("bottom") == sAlign ) { oOutputProperty.m_eFontAlign = RtfParagraphProperty::fa_favar; }
					}
					else if( _T("w:ind") == sNodeName )
					{
						int nFirstLine = PROP_DEF;
						COMMAND_OOX_INT_ATTRIBUTE( _T("w:hanging"), nFirstLine, oXmlReader, i )
						if( PROP_DEF != nFirstLine )
							oOutputProperty.m_nIndFirstLine = -nFirstLine;
						COMMAND_OOX_INT_ATTRIBUTE( _T("w:firstLine"), oOutputProperty.m_nIndFirstLine, oXmlReader, i )
						COMMAND_OOX_INT_ATTRIBUTE( _T("w:left"), oOutputProperty.m_nIndLeft,oXmlReader, i )
						COMMAND_OOX_INT_ATTRIBUTE( _T("w:right"), oOutputProperty.m_nIndRight,oXmlReader, i )
						COMMAND_OOX_INT_ATTRIBUTE( _T("w:start"), oOutputProperty.m_nIndStart,oXmlReader, i )
						COMMAND_OOX_INT_ATTRIBUTE( _T("w:end"), oOutputProperty.m_nIndEnd,oXmlReader, i )
					}
					COMMAND_OOX_BOOL( _T("w:adjustRightInd"), oOutputProperty.m_bIndRightAuto, sNodeName, oXmlReader, i)
					COMMAND_OOX_BOOL( _T("w:mirrorIndents"), oOutputProperty.m_bIndMirror, sNodeName, oXmlReader, i)
					else if( _T("w:spacing") == sNodeName )
					{
						CString sXml = oXmlReader.ReadNodeXml(i);
						COMMAND_OOX_INT_ATTRIBUTE( _T("w:before"), oOutputProperty.m_nSpaceBefore,oXmlReader, i )
						COMMAND_OOX_INT_ATTRIBUTE( _T("w:after"), oOutputProperty.m_nSpaceAfter,oXmlReader, i )
						COMMAND_OOX_BOOL_ATTRIBUTE( _T("w:beforeAutospacing"), oOutputProperty.m_nSpaceBeforeAuto,oXmlReader, i )
						COMMAND_OOX_BOOL_ATTRIBUTE( _T("w:afterAutospacing"), oOutputProperty.m_nSpaceAfterAuto,oXmlReader, i )
						COMMAND_OOX_INT_ATTRIBUTE( _T("w:beforeLines"), oOutputProperty.m_nSpaceBeforeLine,oXmlReader, i )
						COMMAND_OOX_INT_ATTRIBUTE( _T("w:afterLines"), oOutputProperty.m_nSpaceAfterLine,oXmlReader, i )
						CString sBetween = oXmlReader.ReadNodeAttribute( i, _T("w:line"),_T("") );
						CString sRule = oXmlReader.ReadNodeAttribute( i, _T("w:lineRule"),_T("") );
						if( _T("") != sBetween )
						{
							if( _T("exact") == sRule ) //exact
							{
								oOutputProperty.m_nSpaceBetween = - Strings::ToInteger( sBetween );
								oOutputProperty.m_nSpaceMultiLine = 0;
							}
							else if( _T("atLeast") == sRule )
							{
								oOutputProperty.m_nSpaceBetween = Strings::ToInteger( sBetween );
								oOutputProperty.m_nSpaceMultiLine = 0;
							}
						}
						else
						{
							oOutputProperty.m_nSpaceBetween = 240;
							oOutputProperty.m_nSpaceMultiLine = 1;
						}
					}
					COMMAND_OOX_BOOL( _T("w:snapToGrid"), oOutputProperty.m_bSnapToGrid, sNodeName, oXmlReader, i)
					COMMAND_OOX_BOOL( _T("w:contextualSpacing"), oOutputProperty.m_bContextualSpacing, sNodeName, oXmlReader, i)
					COMMAND_OOX_BOOL( _T("w:bidi"), oOutputProperty.m_bRtl, sNodeName, oXmlReader, i)
					if( _T("w:wordWrap") == sNodeName )
					{
						CString swordWrap= oXmlReader.ReadNodeAttribute( i, _T("w:val") );
						if( _T("off") != swordWrap && _T("false") != swordWrap && _T("0") != swordWrap )
							oOutputProperty.m_bNoWordWrap = 0;
						else
							oOutputProperty.m_bNoWordWrap = 1;
					}
					else if( _T("w:textboxTightWrap") == sNodeName )
					{
						CString sType = oXmlReader.ReadNodeAttribute( i, _T("w:val"), _T("") );
						if( _T("none") == sType )
							oOutputProperty.m_eTextBoxWrap = RtfParagraphProperty::tbw_txbxtwno;
						else if( _T("allLines") == sType )
							oOutputProperty.m_eTextBoxWrap = RtfParagraphProperty::tbw_txbxtwalways;
						else if( _T("firstAndLastLine") == sType )
							oOutputProperty.m_eTextBoxWrap = RtfParagraphProperty::tbw_txbxtwfirstlast;
						else if( _T("firstLineOnly") == sType )
							oOutputProperty.m_eTextBoxWrap = RtfParagraphProperty::tbw_txbxtwfirst;
						else if( _T("lastLineOnly") == sType )
							oOutputProperty.m_eTextBoxWrap = RtfParagraphProperty::tbw_txbxtwlast;
					}
					else if( _T("w:numPr") == sNodeName )
					{
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode( i , oNode );
						XmlUtils::CXmlReader oXmlSubReader;
						if( TRUE == oXmlSubReader.OpenFromXmlNode(oNode) )
							if( TRUE == oXmlSubReader.ReadNodeList(_T("*")) )
								for( int j = 0 ; j < oXmlSubReader.GetLengthList(); j++ )
								{
									CString sSubNodeName = oXmlSubReader.ReadNodeName(j);
									if( _T("w:ilvl") == sSubNodeName )
									{
										CString sValue = oXmlSubReader.ReadNodeAttribute( j, _T("w:val") );
										if( _T("") != sValue )
											oOutputProperty.m_nListLevel = Strings::ToInteger( sValue );
									}
									else if( _T("w:numId") == sSubNodeName )
									{
										CString sValue = oXmlSubReader.ReadNodeAttribute( j, _T("w:val") );
										if( _T("") != sValue )
										{
											oOutputProperty.m_nListId = Strings::ToInteger( sValue );
											if( PROP_DEF == oOutputProperty.m_nListLevel )
												oOutputProperty.m_nListLevel = 0;
										}
									}
								}
						if( PROP_DEF != oOutputProperty.m_nListLevel && PROP_DEF != oOutputProperty.m_nListId )
						{
							//применяем direct свойства списка к параграфу
							RtfListOverrideProperty oListOverrideProperty;
							//ищем по override table
							if( true == oParam.oDocument->m_oListOverrideTabel.GetList( oOutputProperty.m_nListId, oListOverrideProperty ) )
							{
								RtfListProperty oListProperty;
								//Ищем по List Table
								if( true == oParam.oDocument->m_oListTabel.GetList( oListOverrideProperty.m_nListID, oListProperty) )
								{
									//дописываем свойства параграфа firstIndent Indent
									RtfListLevelProperty poLevelProp ;
									if( true == oListProperty.GetItem( poLevelProp , oOutputProperty.m_nListLevel ) )
									{
										if( PROP_DEF != poLevelProp.m_nFirstIndent )
											oOutputProperty.m_nIndFirstLine = poLevelProp.m_nFirstIndent;
										if( PROP_DEF != poLevelProp.m_nIndent )
											oOutputProperty.m_nIndLeft = poLevelProp.m_nIndent;
										if( PROP_DEF != poLevelProp.m_nIndentStart )
											oOutputProperty.m_nIndStart = poLevelProp.m_nIndentStart;
										oOutputProperty.m_oTabs = poLevelProp.m_oTabs;
									}
								}
							}
						}
					}
					else if( _T("w:shd") == sNodeName )
					{
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode( i , oNode );
						ReaderParameter oNewParameter = oParam;
						oNewParameter.oNode = oNode;
						OOXShadingReader oShadingReader;
						oShadingReader.Parse( oNewParameter, oOutputProperty.m_oShading  );
					}
					else if( _T("w:pBdr") == sNodeName ) //todo
					{
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode( i , oNode );
						XmlUtils::CXmlReader oXmlSubReader;

						OOXBorderReader oBorderReader;
						if( TRUE == oXmlSubReader.OpenFromXmlNode(oNode) )
							if( TRUE == oXmlSubReader.ReadNodeList(_T("*")) )
								for( int j = 0 ; j < oXmlSubReader.GetLengthList(); j++ )
								{
									CString sSubNodeName = oXmlSubReader.ReadNodeName(j);
									XML::IXMLDOMNodePtr oSubNode;
									oXmlSubReader.GetNode( j , oSubNode );
									ReaderParameter oNewParam = oParam;
									oNewParam.oNode = oSubNode;
									RtfBorder oNewBorder;
									if( _T("w:top") == sSubNodeName )
										if( true ==  oBorderReader.Parse( oNewParam, oNewBorder) )
											oOutputProperty.m_oBorderTop = oNewBorder;
									if( _T("w:left") == sSubNodeName )
										if( true ==  oBorderReader.Parse( oNewParam, oNewBorder) )
											oOutputProperty.m_oBorderLeft = oNewBorder;	
									if( _T("w:bottom") == sSubNodeName )
										if( true ==  oBorderReader.Parse( oNewParam, oNewBorder) )
											oOutputProperty.m_oBorderBottom = oNewBorder;	
									if( _T("w:right") == sSubNodeName )
										if( true ==  oBorderReader.Parse( oNewParam, oNewBorder) )
											oOutputProperty.m_oBorderRight = oNewBorder;	
									if( _T("w:between") == sSubNodeName )
										if( true ==  oBorderReader.Parse( oNewParam, oNewBorder) )
											oOutputProperty.m_oBorderBox = oNewBorder;	
									if( _T("w:bar") == sSubNodeName )
										if( true ==  oBorderReader.Parse( oNewParam, oNewBorder) )
											oOutputProperty.m_oBorderBar = oNewBorder;	
								}
					}
					else if( _T("w:framePr") == sNodeName )
					{
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode( i , oNode );
						ReaderParameter oNewParam = oParam;
						oNewParam.oNode = oNode;
						OOXpPrFrameReader oFrameReader;
						oFrameReader.Parse( oNewParam, oOutputProperty.m_oFrame );
					}
					COMMAND_OOX_BOOL( _T("w:suppressOverlap"), oOutputProperty.m_bOverlap, sNodeName, oXmlReader, i)
					else if( _T("w:textFlow") == sNodeName )
					{
						CString sType = oXmlReader.ReadNodeAttribute( i, _T("w:val"), _T("") );
						CharLower( sType.GetBuffer() );
						sType.ReleaseBuffer();
						if( _T("lrtb") == sType )
							oOutputProperty.m_eTextFollow = RtfParagraphProperty::tf_frmtxlrtb;
						else if( _T("tbrl") == sType )
							oOutputProperty.m_eTextFollow = RtfParagraphProperty::tf_frmtxtbrl;
						else if( _T("btlr") == sType )
							oOutputProperty.m_eTextFollow = RtfParagraphProperty::tf_frmtxbtlr;
						else if( _T("lrtbv") == sType )
							oOutputProperty.m_eTextFollow = RtfParagraphProperty::tf_frmtxlrtbv;
						else if( _T("tbrlv") == sType )
							oOutputProperty.m_eTextFollow = RtfParagraphProperty::tf_frmtxtbrlv;
					}
					else if( _T("w:tabs") == sNodeName )
					{
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode( i , oNode );
						ReaderParameter oNewParam = oParam;
						oNewParam.oNode = oNode;
						OOXpPrTabReader oTabReader;
						oTabReader.Parse( oNewParam, oOutputProperty.m_oTabs );
					}
					else if( _T("w:sectPr") == sNodeName )
					{
						//XML::IXMLDOMNodePtr oNode;
						//oXmlReader.GetNode( i , oNode );
						//ReaderParameter oNewParam = oParam;
						//oNewParam.oNode = oNode;
						bStartNewSection = true;
						//OOXSectionPropertyReader oSectReader;
						//oSectReader.Parse( oNewParam, oParam.oReader->m_oCurSectionProp );
					}
					else if( _T("w:rPr") == sNodeName )
					{
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode( i , oNode );
						ReaderParameter oNewParam = oParam;
						oNewParam.oNode = oNode;
						OOXrPrReader orPrReader;
						orPrReader.Parse( oNewParam, oOutputProperty.m_oCharProperty );
					}
				}

				return true;
			}
	};
