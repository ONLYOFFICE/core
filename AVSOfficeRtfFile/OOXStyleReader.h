#pragma once
#include "OOXpPrReader.h"
#include "OOXrPrReader.h"
#include "OOXtblPrReader.h"
#include "OOXtrPrReader.h"
#include "OOXtcPrReader.h"

	class OOXStyleReader
	{
	public: OOXStyleReader()
			{
			}
	public: bool ParseStyle( ReaderParameter oParam, RtfStylePtr& oOutputStyle)
			{
				XmlUtils::CXmlReader oXmlReader;
				if( TRUE ==  oXmlReader.OpenFromXmlNode(oParam.oNode) )
				{
					RtfStylePtr oNewStyle;
					CString sStyleType = oXmlReader.ReadNodeAttribute( _T("w:type"), _T("") );

					bool bDefault = false;
					COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("w:default"), bDefault, oXmlReader );

					RtfStyle::StyleType eStyleType;
					if( _T("character") == sStyleType )
						oNewStyle = RtfStylePtr( new RtfCharStyle() );
					else if( _T("paragraph") == sStyleType )
						oNewStyle = RtfStylePtr( new RtfParagraphStyle() );
					else if( _T("table") == sStyleType || _T("band1Horz") == sStyleType || _T("band1Vert") == sStyleType ||
							 _T("band2Horz") == sStyleType || _T("band2Vert") == sStyleType || _T("firstCol") == sStyleType ||
							 _T("firstRow") == sStyleType || _T("lastCol") == sStyleType || _T("lastRow") == sStyleType ||
							 _T("neCell") == sStyleType || _T("nwCell") == sStyleType || _T("seCell") == sStyleType || _T("swCell") == sStyleType)
					{
						sStyleType = _T("table");
						oNewStyle = RtfStylePtr( new RtfTableStyle() );
					}
					else
						return false;
					eStyleType = oNewStyle->m_eType;

					oNewStyle->m_nID = oParam.oDocument->m_oStyleTable.GetCount() + 1;

					CString sStyleName = oXmlReader.ReadNodeAttribute( _T("w:styleId"), _T("") );
					oNewStyle->m_sID = sStyleName;

					oXmlReader.ReadNodeList( _T("*") );
					for( int i = 0; i < oXmlReader.GetLengthList(); i++)
					{
						CString sNodeName = oXmlReader.ReadNodeName(i);
						if( _T("w:name") == sNodeName )
						{
							oNewStyle->m_sName = oXmlReader.ReadNodeAttribute(i, _T("w:val"), _T("") );
						}
						COMMAND_OOX_BOOL( _T("w:hidden"), oNewStyle->m_bHidden, sNodeName, oXmlReader, i )
						COMMAND_OOX_BOOL( _T("w:locked"), oNewStyle->m_bLocked, sNodeName, oXmlReader, i )
						COMMAND_OOX_BOOL( _T("w:semiHidden"), oNewStyle->m_nSemiHidden, sNodeName, oXmlReader, i )
						COMMAND_OOX_BOOL( _T("w:qformat"), oNewStyle->m_bQFormat, sNodeName, oXmlReader, i )
						COMMAND_OOX_BOOL( _T("w:unhideWhenUsed"), oNewStyle->m_bUnhiddenWhenUse, sNodeName, oXmlReader, i )
						COMMAND_OOX_BOOL( _T("w:personal"), oNewStyle->m_bPersonal, sNodeName, oXmlReader, i )
						COMMAND_OOX_BOOL( _T("w:personalReply"), oNewStyle->m_bReply, sNodeName, oXmlReader, i )
						COMMAND_OOX_BOOL( _T("w:personalCompose"), oNewStyle->m_bCompose, sNodeName, oXmlReader, i )
						COMMAND_OOX_INT( _T("w:uiPriority"), oNewStyle->m_nPriority, sNodeName, oXmlReader, i )

						else if( _T("w:rPr") == sNodeName  && ( RtfStyle::stCharacter == eStyleType || RtfStyle::stParagraph == eStyleType || RtfStyle::stTable == eStyleType ))//todo с rtf стилями
						{
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode(i, oNode);
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oNode;
							RtfCharStylePtr oNewCharStyle = boost::shared_static_cast<RtfCharStyle, RtfStyle>( oNewStyle );
							OOXrPrReader orPrReader;
							orPrReader.m_bDefStyle = false;
							orPrReader.Parse( oNewParam, oNewCharStyle->m_oCharProp );
						}
						else if( _T("w:pPr") == sNodeName  && ( RtfStyle::stParagraph == eStyleType || RtfStyle::stTable == eStyleType ) )
						{
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode(i, oNode);
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oNode;
							RtfParagraphStylePtr oNewParStyle = boost::shared_static_cast<RtfParagraphStyle, RtfStyle>( oNewStyle );
							OOXpPrReader opPrReader;
							opPrReader.m_bDefStyle = false;
							bool bStartNewSection;
							opPrReader.Parse( oNewParam, oNewParStyle->m_oParProp, CcnfStyle(), bStartNewSection );
							oNewParStyle->m_oParProp.m_nListId = PROP_DEF; //экспериментально вроде нельзя иметь numbering в параграф стиле
							oNewParStyle->m_oParProp.m_nListLevel = PROP_DEF;

						}
						else if( _T("w:tblPr") == sNodeName  && RtfStyle::stTable == eStyleType )
						{
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode(i, oNode);
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oNode;
							RtfTableStylePtr oNewTableStyle = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oNewStyle );
							OOXtblPrReader otblPrReader;
							otblPrReader.m_bDefStyle = false;
							otblPrReader.Parse( oNewParam, oNewTableStyle->m_oTableProp );
						}
						else if( _T("w:trPr") == sNodeName  && RtfStyle::stTable == eStyleType )
						{
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode(i, oNode);
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oNode;
							RtfTableStylePtr oNewTableStyle = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oNewStyle );
							OOXtrPrReader otrPrReader;
							otrPrReader.Parse( oNewParam, oNewTableStyle->m_oRowProp, CcnfStyle() );
						}
						else if( _T("w:tcPr") == sNodeName  && RtfStyle::stTable == eStyleType )
						{
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode(i, oNode);
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oNode;
							RtfTableStylePtr oNewTableStyle = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oNewStyle );
							OOXtcPrReader otcPrReader;
							otcPrReader.Parse( oNewParam, oNewTableStyle->m_oCellProp, CcnfStyle(), -1, -1, -1, -1 );
						}
						else if( _T("w:tblStylePr") == sNodeName  && RtfStyle::stTable == eStyleType )
						{
							RtfTableStylePtr oTableStyle =boost::shared_static_cast<RtfTableStyle, RtfStyle>(oNewStyle);
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode(i, oNode);
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oNode;
							RtfStylePtr oNewSubStyle;
							OOXStyleReader oStyleReader;
							oStyleReader.ParseStyle( oNewParam, oNewSubStyle);

							oNewSubStyle->m_sName = oNewStyle->m_sName;

							if( true == oNewSubStyle->IsValid() && RtfStyle::stTable == oNewSubStyle->m_eType )
							{
								oNewSubStyle->m_nID = oParam.oDocument->m_oStyleTable.GetCount() + 1;
								CString sTableStyletype = oXmlReader.ReadNodeAttribute(i, _T("w:type"), _T("") );
								if( _T("band1Horz") == sTableStyletype )
								{
									oTableStyle->m_oBandHorOdd = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
									oTableStyle->m_oBandHorOdd->m_oParProp.m_bStyleOddRowBand = 1;
								}
								else if( _T("band1Vert") == sTableStyletype )
								{
									oTableStyle->m_oBandVerOdd = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
									oTableStyle->m_oBandVerOdd->m_oParProp.m_bStyleOddColBand = 1; //для конвертации в Rtf
								}
								else if( _T("band2Horz") == sTableStyletype )
								{
									oTableStyle->m_oBandHorEven = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
									oTableStyle->m_oBandHorEven->m_oParProp.m_bStyleEvenRowBand = 1;
								}
								else if( _T("band2Vert") == sTableStyletype )
								{
									oTableStyle->m_oBandVerEven = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
									oTableStyle->m_oBandVerEven->m_oParProp.m_bStyleEvenColBand = 1;
								}
								else if( _T("firstCol") == sTableStyletype )
								{
									oTableStyle->m_oFirstCol = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
									oTableStyle->m_oFirstCol->m_oParProp.m_bStyleFirstCollumn = 1;
								}
								else if( _T("firstRow") == sTableStyletype )
								{
									oTableStyle->m_oFirstRow = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
									oTableStyle->m_oFirstRow->m_oParProp.m_bStyleFirstRow = 1;
								}
								else if( _T("lastCol") == sTableStyletype )
								{
									oTableStyle->m_oLastCol = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
									oTableStyle->m_oLastCol->m_oParProp.m_bStyleLastCollumn = 1;
								}
								else if( _T("lastRow") == sTableStyletype )
								{
									oTableStyle->m_oLastRow = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
									oTableStyle->m_oLastRow->m_oParProp.m_bStyleLastRow = 1;
								}
								else if( _T("neCell") == sTableStyletype )
								{
									oTableStyle->m_oNECell = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
									oTableStyle->m_oNECell->m_oParProp.m_bStyleNECell = 1;
								}
								else if( _T("nwCell") == sTableStyletype )
								{
									oTableStyle->m_oNWCell = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
									oTableStyle->m_oNWCell->m_oParProp.m_bStyleNWCell = 1;
								}
								else if( _T("seCell") == sTableStyletype )
								{
									oTableStyle->m_oSECell = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
									oTableStyle->m_oSECell->m_oParProp.m_bStyleSECell = 1;
								}
								else if( _T("swCell") == sTableStyletype )
								{
									oTableStyle->m_oSWCell = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
									oTableStyle->m_oSWCell->m_oParProp.m_bStyleSWCell = 1;
								}
							}
						}
					}

					if( true == oNewStyle->IsValid() )
					{
						if( true == bDefault )
						{
							if( RtfStyle::stCharacter == eStyleType && NULL == oParam.oReader->m_oDefCharStyle )
								oParam.oReader->m_oDefCharStyle = boost::shared_static_cast<RtfCharStyle, RtfStyle>( oNewStyle );
							else if( RtfStyle::stParagraph == eStyleType && NULL == oParam.oReader->m_oDefParStyle )
								oParam.oReader->m_oDefParStyle = boost::shared_static_cast<RtfParagraphStyle, RtfStyle>( oNewStyle );
							else if( RtfStyle::stTable == eStyleType && NULL == oParam.oReader->m_oDefTableStyle )
								oParam.oReader->m_oDefTableStyle = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oNewStyle );
						}
						oOutputStyle = oNewStyle;
						return true;
					}
				}
				return false;
			}
	public: bool ParseRelations( ReaderParameter oParam )
			{
				XmlUtils::CXmlReader oXmlReader;
				if( TRUE ==  oXmlReader.OpenFromXmlNode(oParam.oNode) )
				{
					RtfStylePtr oCurStyle ;
					CString sStyleName = oXmlReader.ReadNodeAttribute( _T("w:styleId"), _T("") );

					if( true == oParam.oDocument->m_oStyleTable.GetStyle(sStyleName, oCurStyle) )
					{
						oXmlReader.ReadNodeList( _T("*") );
						for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
						{
							CString sNodeName = oXmlReader.ReadNodeName(i);
							if( _T("w:basedOn") == sNodeName )
							{
								CString sVal = oXmlReader.ReadNodeAttribute(i, _T("w:val"), _T("") );
								RtfStylePtr oBaseStyle;
								if( true == oParam.oDocument->m_oStyleTable.GetStyle(sVal, oBaseStyle) )
									oCurStyle->m_nBasedOn = oBaseStyle->m_nID;
							}
							else if( _T("w:next") == sNodeName )
							{
								CString sVal = oXmlReader.ReadNodeAttribute(i, _T("w:val"), _T("") );
								RtfStylePtr oNextStyle;
								if( true == oParam.oDocument->m_oStyleTable.GetStyle(sVal, oNextStyle) )
									oCurStyle->m_nNext = oNextStyle->m_nID;
							}
							else if( _T("w:link") == sNodeName )
							{
								CString sVal = oXmlReader.ReadNodeAttribute(i, _T("w:val"), _T("") );
								RtfStylePtr oLinkStyle;
								if( true == oParam.oDocument->m_oStyleTable.GetStyle(sVal, oLinkStyle) )
									oCurStyle->m_nLink = oLinkStyle->m_nID;
							}
						}
					}
				}
				return false;
			}
	};
