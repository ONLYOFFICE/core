#pragma once
#include "RtfDocument.h"
#include "XmlUtils.h"
#include "OOXBorderReader.h"
#include "OOXcnfStyleReader.h"

	class OOXtcPrReader
	{
	public: OOXtcPrReader()
			{
			}
	public: bool Parse( ReaderParameter oParam ,RtfCellProperty& oOutputProperty,  CcnfStyle& oConditionalTableStyle, int nCurCell, int nCellCount, int nCurRow, int nRowCount  )
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
					oOutputProperty.Merge( oResultStyle->m_oCellProp );
				oConditionalTableStyle.ApplyTableStyleToCellBorder( oParam.poTableStyle, oOutputProperty, nCurCell, nCellCount, nCurRow, nRowCount );

				oXmlReader.OpenFromXmlNode(oParam.oNode);
				oXmlReader.ReadNodeList( _T("*") );

				for(int i=0; i<oXmlReader.GetLengthList();i++ )
				{
					CString sNodeName = oXmlReader.ReadNodeName(i);
					if( _T("w:hMerge") == sNodeName )
					{
						CString sValue = oXmlReader.ReadNodeAttribute( i, _T("w:val") );
						if( _T("restart") == sValue )
							oOutputProperty.m_bMergeFirst = 1;
						else if( _T("continue") == sValue )
							oOutputProperty.m_bMerge = 1;
					}
					else if( _T("w:vMerge") == sNodeName )
					{
						CString sText = oXmlReader.ReadNodeXml(i);
						CString sValue = oXmlReader.ReadNodeAttribute( i, _T("w:val") );
						if( _T("restart") == sValue )
							oOutputProperty.m_bMergeFirstVertical = 1;
						else //if( "continue" == sValue )
							oOutputProperty.m_bMergeVertical = 1;
					}
					COMMAND_OOX_BOOL( _T("w:tcFitText"), oOutputProperty.m_bFitText, sNodeName, oXmlReader, i )
					COMMAND_OOX_BOOL( _T("w:noWrap"), oOutputProperty.m_bNoWrap, sNodeName, oXmlReader, i )
					else if( _T("w:tcMar") == sNodeName )
					{
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode( i , oNode );

						XmlUtils::CXmlReader oXmlSubReader;
						oXmlSubReader.OpenFromXmlNode(oNode);
						oXmlSubReader.ReadNodeList( _T("*") );
						for(int j = 0; j < oXmlSubReader.GetLengthList(); j++)
						{
							CString sNodeName = oXmlSubReader.ReadNodeName( j );
							MetricUnits eMetric = mu_none;
							int nValue = PROP_DEF;
							COMMAND_READ_MARGIN( oXmlSubReader, j, eMetric, nValue );
							if( mu_Twips == eMetric && PROP_DEF != nValue )
							{
								if( _T("w:bottom") == sNodeName )
								{
									oOutputProperty.m_nIsPaddingBottom = 3;
									oOutputProperty.m_nPaddingBottom = nValue;
								}
								else if( _T("w:left") == sNodeName )
								{
									oOutputProperty.m_nIsPaddingLeft = 3;
									oOutputProperty.m_nPaddingLeft = nValue;
								}
								else if( _T("w:right") == sNodeName )
								{
									oOutputProperty.m_nIsPaddingRight = 3;
									oOutputProperty.m_nPaddingRight = nValue;
								}
								else if( _T("w:top") == sNodeName )
								{
									oOutputProperty.m_nIsPaddingTop = 3;
									oOutputProperty.m_nPaddingTop = nValue;
								}
							}
						}
					}
					else if( _T("w:tcW") == sNodeName ) 
						COMMAND_READ_MARGIN( oXmlReader, i, oOutputProperty.m_eWidthUnits, oOutputProperty.m_nWidth )
					COMMAND_OOX_BOOL( _T("w:hideMark"), oOutputProperty.m_bHideMark, sNodeName, oXmlReader, i )
					else if( _T("w:tcBorders") == sNodeName )
					{
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode(i,oNode);
						XmlUtils::CXmlReader oXmlSubReader;
						oXmlSubReader.OpenFromXmlNode( oNode );
						oXmlSubReader.ReadNodeList( _T("*") );
						OOXBorderReader oBorderReader;
						for( int j = 0; j < oXmlSubReader.GetLengthList(); j++ )
						{
							CString sXml = oXmlSubReader.ReadNodeXml(j);
							XML::IXMLDOMNodePtr oSubNode;
							oXmlSubReader.GetNode(j, oSubNode);
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oSubNode;
							CString sSubNodeName = oXmlSubReader.ReadNodeName(j);
							if( _T("w:tl2br") == sSubNodeName )
								oBorderReader.Parse( oNewParam,oOutputProperty.m_oBorderDiagonalLR  );
							else if( _T("w:tr2bl") == sSubNodeName )
								oBorderReader.Parse( oNewParam,oOutputProperty.m_oBorderDiagonalRL  );
							else if( _T("w:left") == sSubNodeName )
								oBorderReader.Parse( oNewParam,oOutputProperty.m_oBorderLeft  );
							else if( _T("w:top") == sSubNodeName )
								oBorderReader.Parse( oNewParam,oOutputProperty.m_oBorderTop  );
							else if( _T("w:right") == sSubNodeName )
								oBorderReader.Parse( oNewParam,oOutputProperty.m_oBorderRight  );
							else if( _T("w:bottom") == sSubNodeName )
								oBorderReader.Parse( oNewParam,oOutputProperty.m_oBorderBottom  );
						}
					}
					else if( _T("w:shd") == sNodeName )
					{
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode(i,oNode);
						ReaderParameter oNewParam = oParam;
						oNewParam.oNode = oNode;
						OOXShadingReader oShadingReader;
						oShadingReader.Parse( oNewParam,oOutputProperty.m_oShading  );
					}
					else if( _T("w:vAlign") == sNodeName )
					{
						CString sValue = oXmlReader.ReadNodeAttribute( i, _T("w:val") );
						if( _T("top") == sValue )
							oOutputProperty.m_eAlign = RtfCellProperty::ca_Top;
						else if( _T("center") == sValue )
							oOutputProperty.m_eAlign = RtfCellProperty::ca_Center;
						else if( _T("bottom") == sValue )
							oOutputProperty.m_eAlign = RtfCellProperty::ca_Bottom;
					}
					else if( _T("w:textDirection") == sNodeName )
					{
						CString sValue = oXmlReader.ReadNodeAttribute( i, _T("w:val") );
						sValue.MakeLower();
						if( _T("lrtb") == sValue )
							oOutputProperty.m_oCellFlow = RtfCellProperty::cf_lrtb;
						else if( _T("tbrl") == sValue )
							oOutputProperty.m_oCellFlow = RtfCellProperty::cf_tbrl;
						else if( _T("btlr") == sValue )
							oOutputProperty.m_oCellFlow = RtfCellProperty::cf_btlr;
						else if( _T("lrtbv") == sValue )
							oOutputProperty.m_oCellFlow = RtfCellProperty::cf_lrtbv;
						else if( _T("tbrlv") == sValue )
							oOutputProperty.m_oCellFlow = RtfCellProperty::cf_tbrlv;

					}
					COMMAND_OOX_INT( _T("w:gridSpan"), oOutputProperty.m_nSpan, sNodeName, oXmlReader, i )
				}
				return true;
			}
	};