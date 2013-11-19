#pragma once
#include "OOXFontReader.h"
#include "OOXMathPropReader.h"
#include "Utils.h"

	class OOXSettingsReader
	{
	private: CString m_sFileName;

	public: OOXSettingsReader(CString sFilename)
			{
				m_sFileName = sFilename;
			}
	public: bool Parse( ReaderParameter oParam )
			{
				XmlUtils::CXmlReader oXmlReader;
				oXmlReader.OpenFromFile( m_sFileName);
				oXmlReader.ReadRootNode( _T("w:settings") );
				oXmlReader.ReadNodeList( _T("*") );

				//oParam.oDocument->m_oColorSchemeMapping.SetDefaultOOX();
				oParam.oDocument->m_oMathProp.SetDefaultOOX();
				for(int i = 0 ; i < oXmlReader.GetLengthList(); i++)
				{
					XML::IXMLDOMNodePtr oNode;
					oXmlReader.GetNode(i,oNode);
					oParam.oNode = oNode;
					CString sNodeName = oXmlReader.ReadNodeName(i);
					if( _T("m:mathPr") == sNodeName )
					{
						OOXMathPropReader oMathProp;
						oMathProp.Parse( oParam );
					}
					else if( _T("w:doNotHyphenateCaps") == sNodeName )
					{
						CString sVal = oXmlReader.ReadNodeAttribute( i, _T("w:val") );
						if( _T("") != sVal )
						{
							if( _T("false") == sVal || _T("0") == sVal || _T("off") == sVal )
								oParam.oDocument->m_oProperty.m_bHypCaps = 1;
							else
								oParam.oDocument->m_oProperty.m_bHypCaps = 0;
						}
					}
					COMMAND_OOX_BOOL( _T("w:autoHyphenation"), oParam.oDocument->m_oProperty.m_bAutoHyp, sNodeName, oXmlReader, i )
					COMMAND_OOX_INT( _T("w:consecutiveHyphenLimit"), oParam.oDocument->m_oProperty.m_nMaxHypen, sNodeName, oXmlReader, i )
					COMMAND_OOX_INT( _T("w:hyphenationZone"), oParam.oDocument->m_oProperty.m_nHyphenationRight, sNodeName, oXmlReader, i )
					COMMAND_OOX_INT( _T("w:defaultTabStop"), oParam.oDocument->m_oProperty.m_nTabWidth, sNodeName, oXmlReader, i )
					else if( _T("w:bordersDoNotSurroundHeader") == sNodeName )
					{
						CString sVal = oXmlReader.ReadNodeAttribute( i, _T("w:val") );
						if( _T("") != sVal )
						{
							if( _T("false") == sVal || _T("0") == sVal || _T("off") == sVal )
								oParam.oDocument->m_oProperty.m_bDorderSurroundHeader = 1;
							else
								oParam.oDocument->m_oProperty.m_bDorderSurroundHeader = 0;
						}
					}
					else if( _T("w:bordersDoNotSurroundFooter") == sNodeName )
					{
						CString sVal = oXmlReader.ReadNodeAttribute( i, _T("w:val") );
						if( _T("") != sVal )
						{
							if( _T("false") == sVal || _T("0") == sVal || _T("off") == sVal )
								oParam.oDocument->m_oProperty.m_bDorderSurroundFotter = 1;
							else
								oParam.oDocument->m_oProperty.m_bDorderSurroundFotter = 0;
						}
					}
					COMMAND_OOX_BOOL( _T("w:alignBordersAndEdges"), oParam.oDocument->m_oProperty.m_bAlignBordersAndEdges, sNodeName, oXmlReader, i )
					COMMAND_OOX_BOOL( _T("w:gutterAtTop"), oParam.oDocument->m_oProperty.m_bGutterAtTop, sNodeName, oXmlReader, i )
					COMMAND_OOX_BOOL( _T("w:mirrorMargins"), oParam.oDocument->m_oProperty.m_bSwitchMargins, sNodeName, oXmlReader, i )
					COMMAND_OOX_BOOL( _T("w:evenAndOddHeaders"), oParam.oDocument->m_oProperty.m_bFacingPage, sNodeName, oXmlReader, i )
					COMMAND_OOX_BOOL( _T("w:displayBackgroundShape"), oParam.oDocument->m_oProperty.m_nDisplayBackground, sNodeName, oXmlReader, i )
					else if( _T("w:compat") == sNodeName )
					{
						XmlUtils::CXmlReader oXmlSubReader;
						if( TRUE == oXmlSubReader.OpenFromXmlNode( oNode ) )
							if( TRUE == oXmlSubReader.ReadNodeList( _T("*") ) )
							{
								for( int j = 0; j < oXmlSubReader.GetLengthList(); j++ )
								{
									CString sNodeName = oXmlSubReader.ReadNodeName( j );
									if( _T("w:doNotUseHTMLParagraphAutoSpacing") == sNodeName )
										oParam.oDocument->m_oProperty.m_bHtmlAutoSpace = 0;
									else if( _T("w:forgetLastTabAlignment") == sNodeName )
										oParam.oDocument->m_oProperty.m_bUseTabAlignment = 0;
								}
							}
					}
					else if( _T("w:footnotePr") == sNodeName )
					{
						XmlUtils::CXmlReader oXmlSubReader;
						if( TRUE == oXmlSubReader.OpenFromXmlNode( oNode ) )
							if( TRUE == oXmlSubReader.ReadNodeList( _T("*") ) )
							{
								for( int j = 0; j < oXmlSubReader.GetLengthList(); j++ )
								{
									CString sNodeName = oXmlSubReader.ReadNodeName( j );
									if( _T("w:footnote") == sNodeName )
									{
										CString sId = oXmlSubReader.ReadNodeAttribute( _T("w:id") );
										if( _T("") != sId )
											oParam.oDocument->m_oProperty.m_aSpecialFootnotes.Add( Strings::ToInteger(sId) );
									}
									else if( _T("w:numFmt") == sNodeName )
									{
										CString sFormat = oXmlSubReader.ReadNodeAttribute( _T("w:val") );
										if( _T("") != sFormat )
											oParam.oDocument->m_oProperty.m_nFootnoteNumberingFormat = RtfListLevelProperty::GetFormat(sFormat);
									}
									else if( _T("w:numRestart") == sNodeName )
									{
										CString sRestart = oXmlSubReader.ReadNodeAttribute( _T("w:val") );
										if( _T("eachPage") == sRestart )
											oParam.oDocument->m_oProperty.m_eFootnoteRestart = RtfDocumentProperty::fr_EachPage;
										else if( _T("eachSect") == sRestart )
											oParam.oDocument->m_oProperty.m_eFootnoteRestart = RtfDocumentProperty::fr_EachSection;
										else if( _T("continuous") == sRestart )
											oParam.oDocument->m_oProperty.m_eFootnoteRestart = RtfDocumentProperty::fr_Continuous;
									}
									COMMAND_OOX_INT( _T("w:numStart"), oParam.oDocument->m_oProperty.m_nFootnoteStart, sNodeName, oXmlReader, j )
									else if( _T("w:pos") == sNodeName )
									{
										CString sPos = oXmlSubReader.ReadNodeAttribute( _T("w:val") );
										if( _T("beneathText") == sPos )
											oParam.oDocument->m_oProperty.m_eFootnotePlacement = RtfDocumentProperty::fp_BeneathText;
										else if( _T("docEnd") == sPos )
											oParam.oDocument->m_oProperty.m_eFootnotePlacement = RtfDocumentProperty::fp_EndDocument;
										else if( _T("pageBottom") == sPos )
											oParam.oDocument->m_oProperty.m_eFootnotePlacement = RtfDocumentProperty::fp_BottomPage;
										else if( _T("sectEnd") == sPos )
											oParam.oDocument->m_oProperty.m_eFootnotePlacement = RtfDocumentProperty::fp_EndSection;
									}
								}
							}
					}
					else if( _T("w:endnotePr") == sNodeName )
					{
						XmlUtils::CXmlReader oXmlSubReader;
						if( TRUE == oXmlSubReader.OpenFromXmlNode( oNode ) )
							if( TRUE == oXmlSubReader.ReadNodeList( _T("*") ) )
							{
								for( int j = 0; j < oXmlSubReader.GetLengthList(); j++ )
								{
									CString sNodeName = oXmlSubReader.ReadNodeName( j );
									if( _T("w:endnote") == sNodeName )
									{
										CString sId = oXmlSubReader.ReadNodeAttribute( _T("w:id") );
										if( _T("") != sId )
											oParam.oDocument->m_oProperty.m_aSpecialEndnotes.Add( Strings::ToInteger(sId) );
									}
									else if( _T("w:numFmt") == sNodeName )
									{
										CString sFormat = oXmlSubReader.ReadNodeAttribute( _T("w:val") );
										if( _T("") != sFormat )
											oParam.oDocument->m_oProperty.m_nEndnoteNumberingFormat = RtfListLevelProperty::GetFormat(sFormat);

									}
									else if( _T("w:numRestart") == sNodeName )
									{
										CString sRestart = oXmlSubReader.ReadNodeAttribute( _T("w:val") );
										if( _T("eachSect") == sRestart )
											oParam.oDocument->m_oProperty.m_eEndnoteRestart = RtfDocumentProperty::er_EachSection;
										else if( _T("continuous") == sRestart )
											oParam.oDocument->m_oProperty.m_eEndnoteRestart = RtfDocumentProperty::er_Continuous;
									}
									COMMAND_OOX_INT( _T("w:numStart"), oParam.oDocument->m_oProperty.m_nEndnoteStart, sNodeName, oXmlReader, j )
									else if( _T("w:pos") == sNodeName )
									{
										CString sPos = oXmlSubReader.ReadNodeAttribute( _T("w:val") );
										if( _T("beneathText") == sPos )
											oParam.oDocument->m_oProperty.m_eEndnotePlacement = RtfDocumentProperty::ep_BeneathText;
										else if( _T("docEnd") == sPos )
											oParam.oDocument->m_oProperty.m_eEndnotePlacement = RtfDocumentProperty::ep_EndDocument;
										else if( _T("pageBottom") == sPos )
											oParam.oDocument->m_oProperty.m_eEndnotePlacement = RtfDocumentProperty::ep_BottomPage;
										else if( _T("sectEnd") == sPos )
											oParam.oDocument->m_oProperty.m_eEndnotePlacement = RtfDocumentProperty::ep_EndSection;
									}
								}
							}
					}
				}	  
				return true;
			}
	};
