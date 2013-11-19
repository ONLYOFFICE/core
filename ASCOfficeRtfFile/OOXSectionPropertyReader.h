#pragma once
#include "OOXReaderBasic.h"
#include "OOXHeaderReader.h"

	class OOXSectionPropertyReader
	{
	public: OOXSectionPropertyReader()
			{
			}
	public: bool Parse( ReaderParameter oParam , RtfSectionProperty& oOutput)
			{
				oOutput.SetDefaultOOX();

				XmlUtils::CXmlReader oXmlReader;
				if( TRUE == oXmlReader.OpenFromXmlNode(oParam.oNode) )
					if( TRUE == oXmlReader.ReadNodeList( _T("*") ) )
					{
						for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
						{
							CString sNodeName = oXmlReader.ReadNodeName(i);
							if( _T("*") == sNodeName )
								;
							COMMAND_OOX_BOOL( _T("w:bidi"), oOutput.m_bBidi, sNodeName, oXmlReader, i )
							else if( _T("w:paperSrc") == sNodeName )
							{
								COMMAND_OOX_INT_ATTRIBUTE( _T("w:first"), oOutput.m_nPaperSourceFirst, oXmlReader, i );
								COMMAND_OOX_INT_ATTRIBUTE( _T("w:other"), oOutput.m_nPaperSourceOther, oXmlReader, i );
							}
							COMMAND_OOX_BOOL( _T("w:rtlGutter"), oOutput.m_bRtlGutter, sNodeName, oXmlReader, i )
							else if( _T("w:type") == sNodeName )
							{
								CString sType = oXmlReader.ReadNodeAttribute( i, _T("w:val") );
								if( _T("") != sType )
								{
									if( _T("continuous") == sType )
										oOutput.m_eSectionBreak = RtfSectionProperty::sb_sbknone;
									else if( _T("nextColumn") == sType )
										oOutput.m_eSectionBreak = RtfSectionProperty::sb_sbkcol;
									else if( _T("nextPage") == sType )
										oOutput.m_eSectionBreak = RtfSectionProperty::sb_sbkpage;
									else if( _T("evenPage") == sType )
										oOutput.m_eSectionBreak = RtfSectionProperty::sb_sbkeven;
									else if( _T("oddPage") == sType )
										oOutput.m_eSectionBreak = RtfSectionProperty::sb_sbkodd;
								}
							}
							else if( _T("w:cols") == sNodeName )
							{
								CString sNumber = oXmlReader.ReadNodeAttribute( i, _T("w:num") );
								if( _T("") != sNumber )
									oOutput.m_nColumnNumber = Strings::ToInteger( sNumber );
								CString sSpace = oXmlReader.ReadNodeAttribute( i, _T("w:space") );
								if( _T("") != sSpace )
									oOutput.m_nColumnSpace = Strings::ToInteger( sSpace );	

								XML::IXMLDOMNodePtr oNode;
								oXmlReader.GetNode(i, oNode);
								XmlUtils::CXmlReader oXmlSubReader;
								oXmlSubReader.OpenFromXmlNode( oNode );
								oXmlSubReader.ReadNodeList( _T("w:col") );
								for( int j = 0; j < oXmlSubReader.GetLengthList(); j++ )
								{
									RtfSectionProperty::ColumnProperty::CollumnVar oNewColumn;
									CString sWidth = oXmlReader.ReadNodeAttribute( j, _T("w:w") );
									if( _T("") != sWidth )
										oNewColumn.m_nColumnWidth = Strings::ToInteger( sWidth );
									CString sSpace = oXmlReader.ReadNodeAttribute( j, _T("w:space") );
									if( _T("") != sSpace )
										oNewColumn.m_nColumnSpaceToRightOfCol = Strings::ToInteger( sSpace );
									oOutput.m_oCollumnProperty.m_aCollumnProperty.Add( oNewColumn );
								}
							}
							else if( _T("w:endnotePr") == sNodeName )
							{
								XML::IXMLDOMNodePtr oNode;
								oXmlReader.GetNode(i, oNode);
								XmlUtils::CXmlReader oXmlSubReader;
								oXmlSubReader.OpenFromXmlNode( oNode );
								oXmlSubReader.ReadNodeList( _T("*") );
								for( int j = 0; j < oXmlSubReader.GetLengthList(); j++ )
								{
									CString sNodeName = oXmlSubReader.ReadNodeName( j );
									if( _T("w:numRestart") == sNodeName )
									{
										CString sVal = oXmlSubReader.ReadNodeAttribute( _T("w:val") );
										if( _T("eachPage") == sVal )
											oOutput.m_eEndnotesRestart = RtfSectionProperty::er_saftnrestart;
										else if( _T("eachSect") == sVal )
											oOutput.m_eEndnotesRestart = RtfSectionProperty::er_saftnrstcont;
									}
									COMMAND_OOX_INT( _T("w:numStart"), oOutput.m_nEndnotesStart, sNodeName, oXmlSubReader, j )
									else if( _T("w:numFmt") == sNodeName )
									{
										CString sVal = oXmlSubReader.ReadNodeAttribute( _T("w:val") );
										//todo
										oOutput.m_eEndnotesRestart = RtfSectionProperty::er_saftnrestart;
									}
								}
							}
							else if( _T("w:footnotePr") == sNodeName )
							{
								XML::IXMLDOMNodePtr oNode;
								oXmlReader.GetNode(i, oNode);
								XmlUtils::CXmlReader oXmlSubReader;
								oXmlSubReader.OpenFromXmlNode( oNode );
								oXmlSubReader.ReadNodeList( _T("*") );
								for( int j = 0; j < oXmlSubReader.GetLengthList(); j++ )
								{
									CString sNodeName = oXmlSubReader.ReadNodeName( j );
									if( _T("w:pos") == sNodeName )
									{
										CString sVal = oXmlSubReader.ReadNodeAttribute( _T("w:val") );
										if( _T("beneathText") == sVal )
											oOutput.m_eFootnotesJust = RtfSectionProperty::fj_sftntj;
										else if( _T("pageBottom") == sVal )
											oOutput.m_eFootnotesJust = RtfSectionProperty::fj_sftnbj;
									}
									COMMAND_OOX_INT( _T("w:numStart"), oOutput.m_nFootnotesStart, sNodeName, oXmlSubReader, j )
									else if( _T("w:numRestart") == sNodeName )
									{
										CString sVal = oXmlSubReader.ReadNodeAttribute( _T("w:val") );
										if( _T("eachPage") == sVal )
											oOutput.m_eFootnotesRestart = RtfSectionProperty::fr_sftnrstpg;
										else if( _T("eachSect") == sVal )
											oOutput.m_eFootnotesRestart = RtfSectionProperty::fr_sftnrestart;
										else if( _T("continuous") == sVal )
											oOutput.m_eFootnotesRestart = RtfSectionProperty::fr_sftnrstcont;
									}
									else if( _T("w:numFmt") == sNodeName )
									{
										CString sVal = oXmlSubReader.ReadNodeAttribute( _T("w:val") );
										//todo
										oOutput.m_eFootnotesRestart = RtfSectionProperty::fr_sftnrstpg;
									}
								}
							}
							else if( _T("w:lnNumType") == sNodeName )
							{
								CString scountBy = oXmlReader.ReadNodeAttribute( i, _T("w:countBy") );
								if( _T("") != scountBy )
									oOutput.m_nLineModulus = Strings::ToInteger( scountBy );
								CString sdistance = oXmlReader.ReadNodeAttribute( i, _T("w:distance") );
								if( _T("") != sdistance )
									oOutput.m_nLineX = Strings::ToInteger( sdistance );
								CString sstart = oXmlReader.ReadNodeAttribute( i, _T("w:start") );
								if( _T("") != sstart )
									oOutput.m_nLineStart = Strings::ToInteger( sstart );
								CString srestart = oXmlReader.ReadNodeAttribute( i, _T("w:restart") );
								if( _T("") != srestart )
								{
									if( _T("newSection") == srestart )
										oOutput.m_eLineNumberRestart = RtfSectionProperty::lnr_linerestart;
									else if( _T("newPage") == srestart )
										oOutput.m_eLineNumberRestart = RtfSectionProperty::lnr_lineppage;
									else if( _T("continuous") == srestart )
										oOutput.m_eLineNumberRestart = RtfSectionProperty::lnr_linecont;
								}
							}
							else if( sNodeName == _T("w:pgSz") )
							{
								CString sPageWidth = oXmlReader.ReadNodeAttribute( i, _T("w:w") );
								if( _T("") != sPageWidth )
									oOutput.m_nPageWidth = Strings::ToInteger( sPageWidth );
								CString sPageHeight = oXmlReader.ReadNodeAttribute( i, _T("w:h") );
								if( _T("") != sPageHeight )
									oOutput.m_nPageHeight = Strings::ToInteger( sPageHeight );
								CString sOrientation = oXmlReader.ReadNodeAttribute( i, _T("w:orient") );
								if( _T("landscape") == sOrientation )
									oOutput.m_bLandscapeFormat = 1;
							}
							if(sNodeName == _T("w:pgMar") )
							{
								CString sMarginLeft = oXmlReader.ReadNodeAttribute( i, _T("w:left") );
								if( _T("") != sMarginLeft )
									oOutput.m_nMarginLeft = Strings::ToInteger( sMarginLeft );
								CString sMarginRight = oXmlReader.ReadNodeAttribute( i, _T("w:right") );
								if( _T("") != sMarginRight )
									oOutput.m_nMarginRight = Strings::ToInteger( sMarginRight );
								CString sMarginTop = oXmlReader.ReadNodeAttribute( i, _T("w:top") );
								if( _T("") != sMarginTop )
									oOutput.m_nMarginTop = Strings::ToInteger( sMarginTop );
								CString sMarginBottom = oXmlReader.ReadNodeAttribute( i, _T("w:bottom") );
								if( _T("") != sMarginBottom )
									oOutput.m_nMarginBottom = Strings::ToInteger( sMarginBottom );
								CString sGutterWidth = oXmlReader.ReadNodeAttribute( i, _T("w:gutter") );
								if( _T("") != sGutterWidth )
									oOutput.m_nGutterMarginWidth = Strings::ToInteger( sGutterWidth );
								CString sHeader = oXmlReader.ReadNodeAttribute( i, _T("w:header") );
								if( _T("") != sHeader )
									oOutput.m_nHeaderTop = Strings::ToInteger( sHeader );
								CString sFotter = oXmlReader.ReadNodeAttribute( i, _T("w:footer") );
								if( _T("") != sFotter )
									oOutput.m_nFotterBottom = Strings::ToInteger( sFotter );
							}
							COMMAND_OOX_BOOL( _T("w:titlePg"), oOutput.m_bTitlePage, sNodeName, oXmlReader, i)
							else if( _T("w:pgNumType") == sNodeName )
							{
								CString sStartNum = oXmlReader.ReadNodeAttribute( i, _T("w:start") );
								if( _T("") != sStartNum )
									oOutput.m_nPageNumberStart = Strings::ToInteger( sStartNum );
							}
							else if( _T("w:vAlign") == sNodeName )
							{
								CString sVal = oXmlReader.ReadNodeAttribute( i, _T("w:val") );
								if( _T("") != sVal )
								{
									if( _T("top") == sVal )
										oOutput.m_eVerticalAlignment = RtfSectionProperty::va_vertalt;
									else if( _T("bottom") == sVal )
										oOutput.m_eVerticalAlignment = RtfSectionProperty::va_vertalb;
									else if( _T("center") == sVal )
										oOutput.m_eVerticalAlignment = RtfSectionProperty::va_vertalc;
									else if( _T("both") == sVal )
										oOutput.m_eVerticalAlignment = RtfSectionProperty::va_vertalj;
								}
							}
							else if( _T("w:textDirection") == sNodeName )
							{
								CString sVal = oXmlReader.ReadNodeAttribute( i, _T("w:val") );
								if( _T("") != sVal )
								{
									CharLower( sVal.GetBuffer() );
									sVal.ReleaseBuffer();
									if( _T("lrtb") == sVal )
										oOutput.m_nTextFollow = 0;
									else if( _T("tbrl") == sVal )
										oOutput.m_nTextFollow = 1;
									else if( _T("lrbt") == sVal )
										oOutput.m_nTextFollow = 2;
									else if( _T("rltb") == sVal )
										oOutput.m_nTextFollow = 3;
									else if( _T("lrtbv") == sVal )
										oOutput.m_nTextFollow = 4;
									else if( _T("tblrv") == sVal )
										oOutput.m_nTextFollow = 5;
								}
							}
							else if( _T("w:pgBorders") == sNodeName )
							{
								CString sDisplay = oXmlReader.ReadNodeAttribute( i, _T("w:display") );
								CString sZOrder = oXmlReader.ReadNodeAttribute( i, _T("w:zOrder") );
								CString sOffset = oXmlReader.ReadNodeAttribute( i, _T("w:offsetFrom") );
								if( _T("") != sDisplay || _T("") != sZOrder || _T("") != sOffset )
								{
									oOutput.m_nBorderMeasure = 0;
									if( _T("allPages") == sDisplay )
										oOutput.m_nBorderMeasure = oOutput.m_nBorderMeasure | 0;
									else if( _T("firstPage") == sDisplay )
										oOutput.m_nBorderMeasure = oOutput.m_nBorderMeasure | 1;
									else if( _T("notFirstPage") == sDisplay )
										oOutput.m_nBorderMeasure = oOutput.m_nBorderMeasure | 2;

									if( _T("back") == sZOrder )
										oOutput.m_nBorderMeasure = oOutput.m_nBorderMeasure | 8;

									if( _T("page") == sOffset )
										oOutput.m_nBorderMeasure = oOutput.m_nBorderMeasure | 32;
								}
								XML::IXMLDOMNodePtr oNode;
								oXmlReader.GetNode(i, oNode);
								oParam.oNode = oNode;
								XmlUtils::CXmlReader oXmlSubReader;
								oXmlSubReader.OpenFromXmlNode( oNode );
								oXmlSubReader.ReadNodeList( _T("*") );
								OOXBorderReader oBorderReader;
								for( int j = 0; j < oXmlSubReader.GetLengthList(); j++ )
								{
									CString sNodeName = oXmlSubReader.ReadNodeName( j );
									if( _T("w:left") == sNodeName )
										oBorderReader.Parse( oParam, oOutput.m_oBorderLeft );
									else if( _T("w:top") == sNodeName )
										oBorderReader.Parse( oParam, oOutput.m_oBorderTop );
									else if( _T("w:right") == sNodeName )
										oBorderReader.Parse( oParam, oOutput.m_oBorderRight );
									else if( _T("w:bottom") == sNodeName )
										oBorderReader.Parse( oParam, oOutput.m_oBorderBottom );
								}
							}
							else if(sNodeName == _T("w:headerReference") )
							{
								CString sType = oXmlReader.ReadNodeAttribute( i, _T("w:type") );
								CString sID = oXmlReader.ReadNodeAttribute( i, _T("r:id") );
								OOXHeaderReader oHeaderReader;
								if( _T("") != sType && _T("") != sID )
								{
									if( _T("even") == sType )
									{
										if( sID != oOutput.m_sIDHeaderLeft )
										{
											oHeaderReader.Parse( oParam, sID, oOutput.m_oHeaderLeft, true );	
											oOutput.m_sIDHeaderLeft = sID;
										}
									}
									else if( _T("first") == sType )
									{
										if( sID != oOutput.m_sIDHeaderFirst )
										{
											oHeaderReader.Parse( oParam, sID, oOutput.m_oHeaderFirst, true );	
											oOutput.m_sIDHeaderFirst = sID;
										}
									}
									else if( _T("default") == sType )
									{
										if( sID != oOutput.m_sIDHeaderRight )
										{
											oHeaderReader.Parse( oParam, sID, oOutput.m_oHeaderRight, true );	
											oOutput.m_sIDHeaderRight = sID;
										}
									}
								}
							}
							else if(sNodeName == _T("w:footerReference") )
							{
								CString sType = oXmlReader.ReadNodeAttribute( i, _T("w:type") );
								CString sID = oXmlReader.ReadNodeAttribute( i, _T("r:id") );
								OOXHeaderReader oFooterReader;
								if( _T("") != sType && _T("") != sID )
								{
									if( _T("even") == sType )
									{
										if( sID != oOutput.m_sIDFooterLeft )
										{
											oFooterReader.Parse( oParam, sID, oOutput.m_oFooterLeft, false );	
											oOutput.m_sIDFooterLeft = sID;
										}
									}
									else if( _T("first") == sType )
									{
										if( sID != oOutput.m_sIDFooterFirst )
										{
											oFooterReader.Parse( oParam, sID, oOutput.m_oFooterFirst, false );	
											oOutput.m_sIDFooterFirst = sID;
										}
									}
									else if( _T("default") == sType )
									{
										if( sID != oOutput.m_sIDFooterRight )
										{
											oFooterReader.Parse( oParam, sID, oOutput.m_oFooterRight, false );	
											oOutput.m_sIDFooterRight = sID;
										}
									}
								}
							}
						}
						return true;
					}
				return false;
			}
	};
