#pragma once
#include "OOXReader.h"
#include "RtfDocument.h"
#include "OOXBorderReader.h"
#include "OOXShadingReader.h"
#include "OOXtblpPrReader.h"
#include "OOXReaderBasic.h"
#include "OOXtblLookReader.h"

	class OOXtblPrReader
	{
	public: bool m_bDefStyle;
	public: OOXtblPrReader()
			{
				m_bDefStyle = true;
			}
	public: bool Parse( ReaderParameter oParam , RtfTableProperty& oOutputProperty)
			{
				XmlUtils::CXmlReader oXmlReader;
				//сначала применяем стили
				oXmlReader.OpenFromXmlNode(oParam.oNode);
				//применяем стиль по-умолчанию,если не определен свой
				if( FALSE == oXmlReader.ReadNode(_T("w:tblStyle")) && true == m_bDefStyle && NULL != oParam.oReader->m_oDefTableStyle )
				{
					oOutputProperty.Merge( oParam.oReader->m_oDefTableStyle->m_oTableProp );
					oOutputProperty.m_nStyle = oParam.oReader->m_oDefTableStyle->m_nID;
				}

				oXmlReader.OpenFromXmlNode(oParam.oNode);
				if( TRUE ==  oXmlReader.OpenFromXmlNode(oParam.oNode) )
				{
					oXmlReader.ReadNodeList( _T("*") );
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						CString sNodeName = oXmlReader.ReadNodeName(i);
						if( _T("w:tblStyle") == sNodeName )
						{
							CString sStyle = oXmlReader.ReadNodeAttribute( i, _T("w:val"), _T("") );
							RtfStylePtr oStyle;
							if( true == oParam.oDocument->m_oStyleTable.GetStyle(sStyle, oStyle) )
							{
								RtfStylePtr oResultStyle = oParam.oDocument->m_oStyleTable.GetStyleResulting( oStyle );
								RtfTableStylePtr oTableStyle = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oResultStyle );
								oOutputProperty = oTableStyle->m_oTableProp;
								oOutputProperty.m_nStyle = oTableStyle->m_nID;
							}
						}
						else if( _T("w:tblInd") == sNodeName )
						{
							CString sType = oXmlReader.ReadNodeAttribute(i, _T("w:type") );
							CString sValue = oXmlReader.ReadNodeAttribute(i, _T("w:w") );
							if( _T("") != sValue )
							{
								if( _T("dxa") == sType )//сделаем не по документации, а как все остальные юниты !!!
								{
									oOutputProperty.nTableIndentUnits = 3;
									oOutputProperty.nTableIndent = Strings::ToInteger( sValue );
									oOutputProperty.m_nLeft = oOutputProperty.nTableIndent;
								}
								//else if( _T("pct") == sType )
								//{
								//	oOutputProperty.nTableIndentUnits = 3;
								//	oOutputProperty.nTableIndent = Strings::ToInteger( sValue );
								//}
							}
						}
						else if( _T("w:tblLayout") == sNodeName )
						{
							CString sType = oXmlReader.ReadNodeAttribute( i, _T("w:type") );
							if( _T("") != sType )
							{
								if( _T("fixed") == sType )
									oOutputProperty.m_nAutoFit = 0;
								else if( _T("autofit") == sType )
									oOutputProperty.m_nAutoFit = 1;
							}
						}
						else if( _T("w:tblOverlap") == sNodeName )
						{
							CString sOverlap = oXmlReader.ReadNodeAttribute(i, _T("w:val"), _T("never") );
							if( _T("overlap") == sOverlap )
								oOutputProperty.m_bOverlap = 1;
						}
						else if( _T("w:tblpPr") == sNodeName )
						{
							XML::IXMLDOMNodePtr oSubNode;
							oXmlReader.GetNode(i,oSubNode);
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oSubNode;
							OOXtblpPrReader otblpPrReader;
							otblpPrReader.Parse( oNewParam, oOutputProperty );
						}
						else if( _T("w:tblBorders") == sNodeName )
						{
							XML::IXMLDOMNodePtr oSubNode;
							oXmlReader.GetNode(i,oSubNode);
							XmlUtils::CXmlReader oXmlSubReader;
							oXmlSubReader.OpenFromXmlNode(oSubNode);
							oXmlSubReader.ReadNodeList( _T("*") );
							for( int j = 0; j < oXmlSubReader.GetLengthList(); j++ )
							{
								CString sSubNodeName = oXmlSubReader.ReadNodeName(j);
								XML::IXMLDOMNodePtr oSubSubNode;
								oXmlSubReader.GetNode(j,oSubSubNode);
								ReaderParameter oNewParam = oParam;
								oNewParam.oNode = oSubSubNode;
								OOXBorderReader oBorderReader;
								if( _T("w:top") == sSubNodeName )
									oBorderReader.Parse( oNewParam,oOutputProperty.m_oBorderTop);
								if( _T("w:left") == sSubNodeName )
									oBorderReader.Parse( oNewParam,oOutputProperty.m_oBorderLeft);
								if( _T("w:bottom") == sSubNodeName )
									oBorderReader.Parse( oNewParam,oOutputProperty.m_oBorderBottom);
								if( _T("w:right") == sSubNodeName )
									oBorderReader.Parse( oNewParam,oOutputProperty.m_oBorderRight);
								if( _T("w:insideH") == sSubNodeName )
									oBorderReader.Parse( oNewParam,oOutputProperty.m_oBorderHor);
								if( _T("w:insideV") == sSubNodeName )
									oBorderReader.Parse( oNewParam,oOutputProperty.m_oBorderVert);
								
							}
						}
						else if( _T("w:shd") == sNodeName )
						{
							XML::IXMLDOMNodePtr oSubNode;
							oXmlReader.GetNode(i,oSubNode);
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oSubNode;
							OOXShadingReader oShadingReader;
							oShadingReader.Parse( oNewParam, oOutputProperty.m_oShading );
						}
						else if( _T("w:tblCellMar") == sNodeName )
						{
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode( i , oNode );

							XmlUtils::CXmlReader oXmlSubReader;
							oXmlSubReader.OpenFromXmlNode(oNode);
							oXmlSubReader.ReadNodeList( _T("*") );
							for(int j = 0; j < oXmlSubReader.GetLengthList(); j++)
							{
								CString sXml = oXmlSubReader.ReadNodeXml( j );
								CString sNodeName = oXmlSubReader.ReadNodeName( j );
								MetricUnits eMetric = mu_none;
								int nValue = PROP_DEF;
								COMMAND_READ_MARGIN( oXmlSubReader, j, eMetric, nValue );
								if( mu_Twips == eMetric && PROP_DEF != nValue )
								{
									if( _T("w:bottom") == sNodeName )
									{
										oOutputProperty.m_nDefCellMarBottomUnits = 3;
										oOutputProperty.m_nDefCellMarBottom = nValue;
									}
									else if( _T("w:left") == sNodeName )
									{
										oOutputProperty.m_nDefCellMarLeftUnits = 3;
										oOutputProperty.m_nDefCellMarLeft = nValue;
									}
									else if( _T("w:right") == sNodeName )
									{
										oOutputProperty.m_nDefCellMarRightUnits = 3;
										oOutputProperty.m_nDefCellMarRight = nValue;
									}
									else if( _T("w:top") == sNodeName )
									{
										oOutputProperty.m_nDefCellMarTopUnits = 3;
										oOutputProperty.m_nDefCellMarTop = nValue;
									}
								}
							}
						}
						else if( _T("w:tblCellSpacing") == sNodeName )
						{
							MetricUnits eMetric = mu_none;
							int nValue = PROP_DEF;
							COMMAND_READ_MARGIN( oXmlReader, i, eMetric, nValue );
							if( mu_Twips == eMetric && PROP_DEF != nValue )
							{
								oOutputProperty.m_nDefCellSpTopUnits = 3;
								oOutputProperty.m_nDefCellSpBottomUnits = 3;
								oOutputProperty.m_nDefCellSpLeftUnits = 3;
								oOutputProperty.m_nDefCellSpRightUnits = 3;

								oOutputProperty.m_nDefCellSpTop = nValue;
								oOutputProperty.m_nDefCellSpBottom = nValue;
								oOutputProperty.m_nDefCellSpLeft = nValue;
								oOutputProperty.m_nDefCellSpRight = nValue;
							}
						}
						else if( _T("w:tblLook ") == sNodeName )
						{
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode( i , oNode );
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oNode;
							OOXtblLookReader oLookReader;
							CtblLook oLook;
							oLookReader.Parse( oNewParam, oLook );
							if( true == oLook.bFirstRow )
								oOutputProperty.m_bAutoFirstRow = 1;
							if( true == oLook.bLastRow )
								oOutputProperty.m_bAutoLastRow = 1;
							if( true == oLook.bFirstCol )
								oOutputProperty.m_bAutoFirstCol = 1;
							if( true == oLook.bLastCol )
								oOutputProperty.m_bAutoLastCol = 1;
							if( true == oLook.bNoHBand )
								oOutputProperty.m_bAutoNoRowBand = 1;
							if( true == oLook.bNoVBand )
								oOutputProperty.m_bAutoNoColBand = 1;
						}
						COMMAND_OOX_INT( _T("w:tblStyleRowBandSize"), oOutputProperty.m_nRowBandSize, sNodeName, oXmlReader, i )
						COMMAND_OOX_INT( _T("w:tblStyleColBandSize"), oOutputProperty.m_nColBandSize, sNodeName, oXmlReader, i )
						else if( _T("w:tblW") == sNodeName )
						{
							CString sType = oXmlReader.ReadNodeAttribute(i, _T("w:type") );
							CString sValue = oXmlReader.ReadNodeAttribute(i, _T("w:w") );
							if( _T("") != sValue )
							{
								if( _T("dxa") == sType )
								{
									oOutputProperty.m_nWidth = Strings::ToInteger( sValue );
									oOutputProperty.m_eMUWidth = mu_Twips;
								}
								else if( _T("pct") == sType )
								{
									oOutputProperty.m_nWidth = Strings::ToInteger( sValue );
									oOutputProperty.m_eMUWidth = mu_Percent;
								}
							}
						}
					}
				}
				return false;
			}
	};
