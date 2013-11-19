#pragma once
#include "RtfDocument.h"
#include "XmlUtils.h"
#include "RtfProperty.h"
#include "OOXrPrReader.h"
#include "RtfField.h"
#include "RtfBookmark.h"
#include "OOXShapeReader.h"
#include "OOXPictureReader.h"
#include "RtfOle.h"

	class OOXRunReader
	{
	public: OOXRunReader()
			{
			}
	public: bool Parse( ReaderParameter oParam , RtfParagraph& oOutputParagraph, RtfStylePtr poStyle )
			{
				RtfCharProperty oNewProperty;
				oNewProperty.SetDefaultOOX();
				//применяем default
				oNewProperty = oParam.oDocument->m_oDefaultCharProp; 
				//применяем внешний стиль
				if( NULL != poStyle && TYPE_RTF_PROPERTY_STYLE_CHAR == poStyle->GetType() )
				{
					RtfCharStylePtr oCharStyle = boost::shared_static_cast<RtfCharStyle, RtfStyle>( poStyle );
					oNewProperty.Merge( oCharStyle->m_oCharProp );
				}

				XmlUtils::CXmlReader oXmlReader;
				//сначала читаем свойства
				oXmlReader.OpenFromXmlNode(oParam.oNode);
				if( TRUE == oXmlReader.ReadNode( _T("w:rPr") ) )
				{
					XML::IXMLDOMNodePtr oNode;
					oXmlReader.GetNode( oNode );
					ReaderParameter oNewParam = oParam;
					oNewParam.oNode = oNode;
					OOXrPrReader orPrReader;
					orPrReader.Parse( oNewParam, oNewProperty );
				}

				if( TRUE == oXmlReader.OpenFromXmlNode(oParam.oNode) )
				{
					oXmlReader.ReadNodeList( _T("*") );
					for(int i=0; i<oXmlReader.GetLengthList();i++ )
					{
						CString sNodeName = oXmlReader.ReadNodeName(i);

						if( _T("w:t") == sNodeName )
						{
							CString sXml = oXmlReader.ReadNodeXml(i);
							CString sNodeName = oXmlReader.ReadNodeAttribute(i, _T("xml:space"), _T("") );
							CString sNodeValue= oXmlReader.ReadNodeText(i);
							if( _T("preserve") != sNodeName )
								sNodeValue = RtfUtility::Preserve( sNodeValue );
							RtfCharPtr oNewChar = RtfCharPtr( new RtfChar() );
							oNewChar->m_oProperty = oNewProperty;
							oNewChar->setText( sNodeValue );
							oOutputParagraph.AddItem( oNewChar );				
						}
						else if( _T("w:fldChar") == sNodeName )
						{
							CString sFieldType = oXmlReader.ReadNodeAttribute( i, _T("w:fldCharType"), _T("") );
							if( _T("end") == sFieldType )
							{
								OOXFieldEndPtr oNewField( new OOXFieldEnd() );
								oOutputParagraph.AddItem( oNewField );
							}
							else if( _T("begin") == sFieldType )
							{
								OOXFieldBeginPtr oNewField( new OOXFieldBegin() );
								COMMAND_OOX_BOOL_ATTRIBUTE( _T("w:fldLock"), oNewField->m_bLock, oXmlReader, i );
								COMMAND_OOX_BOOL_ATTRIBUTE( _T("w:dirty"), oNewField->m_bDirty, oXmlReader, i );
								oNewField->m_oCharProp = oNewProperty;
								oOutputParagraph.AddItem( oNewField );

							}
							else if( _T("separate") == sFieldType )
							{
								OOXFieldSeparatePtr oNewField( new OOXFieldSeparate() );
								oOutputParagraph.AddItem( oNewField );
							}
						}
						else if( _T("w:instrText") == sNodeName )
						{
							OOXFieldInsertTextPtr oNewField( new OOXFieldInsertText() );
							RtfCharPtr oNewChar( new RtfChar() );
							oNewChar->setText( oXmlReader.ReadNodeText(i, _T("") ) );
							oNewField->m_oText = oNewChar;
							oOutputParagraph.AddItem( oNewField );
						}
						else if( _T("w:footnoteReference") == sNodeName )
						{
							CString sID = oXmlReader.ReadNodeAttribute( i, _T("w:id") );
							if( _T("") != sID )
							{
								int nID = Strings::ToInteger( sID );
								CAtlMap<int, TextItemContainerPtr>::CPair* oPair = oParam.oReader->m_mapFootnotes.Lookup( nID );
								if( NULL != oPair )
								{
									RtfFootnotePtr oNewFootnote( new RtfFootnote() );
									oNewFootnote->m_oCharProp = oNewProperty;
									oNewFootnote->m_oContent = oPair->m_value;
									oOutputParagraph.AddItem( oNewFootnote );
								}
							}
						}
						else if( _T("w:endnoteReference") == sNodeName )
						{
							CString sID = oXmlReader.ReadNodeAttribute( i, _T("w:id") );
							if( _T("") != sID )
							{
								int nID = Strings::ToInteger( sID );
								CAtlMap<int, TextItemContainerPtr>::CPair* oPair = oParam.oReader->m_mapEndnotes.Lookup( nID );
								if( NULL != oPair )
								{
									RtfFootnotePtr oNewFootnote( new RtfFootnote() );
									oNewFootnote->m_oCharProp = oNewProperty;
									oNewFootnote->m_oContent = oPair->m_value;
									oNewFootnote->m_bEndNote = true;
									oOutputParagraph.AddItem( oNewFootnote );
								}
							}
						}
						else if( _T("w:object") == sNodeName )
						{
							long nOleWidth = PROP_DEF;
							long nOleHeight = PROP_DEF;
							COMMAND_OOX_INT_ATTRIBUTE( _T("w:dxaOrig"), nOleWidth, oXmlReader, i );
							COMMAND_OOX_INT_ATTRIBUTE( _T("w:dyaOrig"), nOleHeight, oXmlReader, i );

							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode( i , oNode );
							ReaderParameter oNewParam = oParam;
							RtfShapePtr aPictShape;
							RtfOlePtr oCurOle;

							XmlUtils::CXmlReader oObjectReader;
							oObjectReader.OpenFromXmlNode( oNode );
							if( TRUE == oObjectReader.ReadNodeList(_T("*")) )
							{
								for( int j = 0; j < oObjectReader.GetLengthList(); j++ )
								{
									CString sNodeName = oObjectReader.ReadNodeName( j );
									XML::IXMLDOMNodePtr oObjectNode;
									oObjectReader.GetNode( j, oObjectNode );
									if( _T("v:shape") == sNodeName )
									{
										oNewParam.oNode = oObjectNode;
										RtfShapePtr oNewShape( new RtfShape() );
										oNewShape->m_eShapeType = RtfShape::st_inline;
										OOXShapeReader oShapeReader;
										if( true == oShapeReader.Parse( oNewParam, oNewShape ) )
											 aPictShape = oNewShape;
									}
									else if( _T("v:group") == sNodeName )
									{
										oNewParam.oNode = oObjectNode;
										RtfShapeGroupPtr oNewShape( new RtfShapeGroup() );
										oNewShape->m_eShapeType = RtfShape::st_inline;
										OOXShapeGroupReader oShapeReader;
										if( true == oShapeReader.Parse( oNewParam, oNewShape ) )
											 aPictShape = oNewShape;
									}
									else if( _T("o:OLEObject") == sNodeName )
									{
										CString sType = oObjectReader.ReadNodeAttribute( j, _T("Type") );
										CString sClass = oObjectReader.ReadNodeAttribute( j, _T("ProgID") );
										CString sShapeId = oObjectReader.ReadNodeAttribute( j, _T("ShapeID") );
										CString sRID = oObjectReader.ReadNodeAttribute( j, _T("r:id") );

										int nShapeId = PROP_DEF;

										if( _T("") != sShapeId )
											nShapeId = oParam.oReader->m_oOOXIdGenerator.GetId( sShapeId );

										RtfOlePtr oNewOle( new RtfOle() );
										oNewOle->m_nWidth = nOleWidth;
										oNewOle->m_nHeight = nOleHeight;


										if( PROP_DEF != nShapeId )
										{
											oNewOle->m_nShapeId = nShapeId;
											//ставим соответствующий shape
											if( NULL != aPictShape && aPictShape->m_nID == nShapeId )
												aPictShape->m_bIsOle = true;									
										}
										if( _T("") != sClass )
											oNewOle->m_sOleClass = sClass;
										if( _T("") != sType )
										{
											if( _T("Link") == sType )
												oNewOle->m_eOleType = RtfOle::ot_link;
											else if( _T("Embed") == sType )
												oNewOle->m_eOleType = RtfOle::ot_emb;
										}
										if( _T("") != sRID )
										{
											CString sRelativePath = oParam.oRels->GetByID( sRID ); //todo rels могут быть в других местах
											CString sOlePath = oParam.oReader->m_sDocumentPath + _T("\\")+ sRelativePath;

											IStoragePtr piStorage = NULL;
											if( TRUE == SUCCEEDED( StgOpenStorageEx( sOlePath, STGM_DIRECT|STGM_SHARE_EXCLUSIVE|STGM_READWRITE, STGFMT_ANY, 0, NULL,  NULL, IID_IStorage, (void**)&piStorage) ) )
											{
												RtfOle2ToOle1Stream oStream;
												oStream.lpstbl = new OLESTREAMVTBL();
												oStream.lpstbl->Get = &OleGet2;
												oStream.lpstbl->Put = &OlePut2;
												if( SUCCEEDED( OleConvertIStorageToOLESTREAM( piStorage, &oStream ) ) )
												{
													//сохраняем в файл
													CString sOleStorageName = Utils::CreateTempFile( oParam.oReader->m_sTempFolder );
													HANDLE hFile = CreateFile ( sOleStorageName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
													if( INVALID_HANDLE_VALUE != hFile )
													{
														DWORD dwByteWrite = 0;
														WriteFile( hFile, oStream.aBuffer.GetData(), (DWORD)oStream.aBuffer.GetCount(), &dwByteWrite, NULL );
														CloseHandle( hFile );

														oNewOle->SetFilename( sOleStorageName );
														//только сейчас запоминаем
														oCurOle = oNewOle;
													}
													else
														Utils::RemoveDirOrFile( sOleStorageName );
												}
												delete( oStream.lpstbl );
											}
										}
									}
								}

							}
							if( NULL != aPictShape && NULL != oCurOle )
							{
								//надо пересчитать размеры картинки так чтобы scalex == 100 и scaley = 100
								//если scalex != 100, то после редактирование ole киртинка сжимается(растягивается)
								if( NULL != aPictShape->m_oPicture )
								{
									if( PROP_DEF != (int)aPictShape->m_oPicture->m_dScaleX && PROP_DEF != (int)aPictShape->m_oPicture->m_dScaleY &&
										PROP_DEF != aPictShape->m_oPicture->m_nWidthGoal && PROP_DEF != aPictShape->m_oPicture->m_nHeightGoal )
									{
										if( 100 != (int)aPictShape->m_oPicture->m_dScaleX )
										{
											aPictShape->m_oPicture->m_nWidthGoal =  long (( aPictShape->m_oPicture->m_dScaleX / 100.0 ) * aPictShape->m_oPicture->m_nWidthGoal );
											aPictShape->m_oPicture->m_dScaleX = 100;
										}
										if( 100 != (int)aPictShape->m_oPicture->m_dScaleY )
										{
											aPictShape->m_oPicture->m_nHeightGoal =  long (( aPictShape->m_oPicture->m_dScaleY / 100.0 ) * aPictShape->m_oPicture->m_nHeightGoal );
											aPictShape->m_oPicture->m_dScaleY = 100;
										}
									}
								}

								TextItemContainerPtr oNewTextItemContainer = TextItemContainerPtr( new TextItemContainer() );
								RtfParagraphPtr oNewPar = RtfParagraphPtr( new RtfParagraph() );

								oNewPar->AddItem( oCurOle );
								oNewTextItemContainer->AddItem( oNewPar );
								aPictShape->m_aTextItems = oNewTextItemContainer;

								oCurOle->m_oResultPic = aPictShape;
								oOutputParagraph.AddItem( oCurOle );
							}

						}
						else if( _T("w:drawing") == sNodeName )
						{
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode( i , oNode );
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oNode;
							RtfShapePtr oNewPicture( new RtfShape() );
							OOXPictureReader oPictureReader;
							if( true == oPictureReader.Parse( oNewParam, (*oNewPicture) ) )
								 oOutputParagraph.AddItem( oNewPicture );
						}
						else if( _T("w:pict") == sNodeName )
						{
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode( i , oNode );
							XmlUtils::CXmlReader oXmlSubReader;
							if( TRUE == oXmlSubReader.OpenFromXmlNode( oNode ) )
								if( TRUE == oXmlSubReader.ReadNode( _T("v:shape") ) )
								{
									XML::IXMLDOMNodePtr oSubNode;
									oXmlSubReader.GetNode( oSubNode );
									ReaderParameter oNewParam = oParam;
									oNewParam.oNode = oSubNode;
									RtfShapePtr oNewPicture( new RtfShape() );
									OOXShapeReader oShapeReader;
									if( true == oShapeReader.Parse( oNewParam, oNewPicture ) )
										 oOutputParagraph.AddItem( oNewPicture );
								}
						}
						else if( _T("w:noBreakHyphen") == sNodeName )
						{
							RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
							oNewChar->m_oProperty = oNewProperty;
							oNewChar->m_eType = RtfCharSpecial::rsc_NonBrHyphen;
							oOutputParagraph.AddItem( oNewChar );
						}
						else if( _T("w:softHyphen") == sNodeName )
						{
							RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
							oNewChar->m_oProperty = oNewProperty;
							oNewChar->m_eType = RtfCharSpecial::rsc_OptHyphen;
							oOutputParagraph.AddItem( oNewChar );
						}
						else if( _T("w:pgNum") == sNodeName )
						{
							RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
							oNewChar->m_oProperty = oNewProperty;
							oNewChar->m_eType = RtfCharSpecial::rsc_chpgn;
							oOutputParagraph.AddItem( oNewChar );
						}
						else if( _T("w:footnoteRef") == sNodeName )
						{
							RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
							oNewChar->m_oProperty = oNewProperty;
							oNewChar->m_eType = RtfCharSpecial::rsc_chftn;
							oOutputParagraph.AddItem( oNewChar );
						}
						else if( _T("w:endnoteRef") == sNodeName )
						{
							RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
							oNewChar->m_oProperty = oNewProperty;
							oNewChar->m_eType = RtfCharSpecial::rsc_chftnEnd;
							oOutputParagraph.AddItem( oNewChar );
						}
						else if( _T("w:separator") == sNodeName )
						{
							RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
							oNewChar->m_oProperty = oNewProperty;
							oNewChar->m_eType = RtfCharSpecial::rsc_chftnsep;
							oOutputParagraph.AddItem( oNewChar );
						}
						else if( _T("w:continuationSeparator") == sNodeName )
						{
							RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
							oNewChar->m_oProperty = oNewProperty;
							oNewChar->m_eType = RtfCharSpecial::rsc_chftnsepc;
							oOutputParagraph.AddItem( oNewChar );
						}
						else if( _T("w:tab") == sNodeName )
						{
							RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
							oNewChar->m_oProperty = oNewProperty;
							oNewChar->m_eType = RtfCharSpecial::rsc_tab;
							oOutputParagraph.AddItem( oNewChar );
						}
						else if( _T("w:br") == sNodeName )
						{
							RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
							oNewChar->m_oProperty = oNewProperty;

							CString sType = oXmlReader.ReadNodeAttribute(i, _T("w:type"), _T("") );
							if( _T("page") == sType )
								oNewChar->m_eType = RtfCharSpecial::rsc_page;
							else if( _T("column") == sType )
								oNewChar->m_eType = RtfCharSpecial::rsc_column;
							else if( _T("textWrapping") == sType )
							{
								CString sClear = oXmlReader.ReadNodeAttribute(i, _T("w:clear"), _T("") );
								if( _T("left") == sClear )
									oNewChar->m_nTextWrapBreak = 1;
								else if( _T("right") == sClear )
									oNewChar->m_nTextWrapBreak = 2;
								else if( _T("all") == sClear )
									oNewChar->m_nTextWrapBreak = 3;
								else
									oNewChar->m_eType = RtfCharSpecial::rsc_line;
							}
							else//по умолчанию - строки
								oNewChar->m_eType = RtfCharSpecial::rsc_line;
								
							oOutputParagraph.AddItem( oNewChar );
						}
						else if( _T("mc:AlternateContent") == sNodeName )
						{
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode( i , oNode );
							XmlUtils::CXmlReader oSubReader;
							if( TRUE == oSubReader.OpenFromXmlNode( oNode ) )
								if( TRUE == oSubReader.ReadNode( _T("mc:Fallback") ) )
								{
									XML::IXMLDOMNodePtr oSubNode;
									oSubReader.GetNode( oSubNode );
									ReaderParameter oNewParam = oParam;
									oNewParam.oNode = oSubNode;
									OOXRunReader oSubReader;
									oSubReader.Parse( oNewParam, oOutputParagraph, poStyle );
								}
						}
						else if( _T("w:sym") == sNodeName )
						{
							CString sFont = oXmlReader.ReadNodeAttribute( i, _T("w:font") );
							CString sChar = oXmlReader.ReadNodeAttribute( i, _T("w:char") );
							if( _T("") != sFont && _T("") != sChar )
							{
								sChar.MakeLower();
								//оставляем только 2 байта (4 символа)
								if( sChar.GetLength() > 4 )
									sChar = sChar.Right( 4 );
								//убираем маску F000
								if( sChar.GetLength() == 4 && 'f' == sChar[0] )
									sChar = sChar.Right( 3 );
								int nChar = Convert::ToInt32( sChar, 16 );
								CString sCharUnicode; sCharUnicode.AppendChar( nChar );
								RtfFont oCurFont;
								if( false == oParam.oDocument->m_oFontTable.GetFont( sFont, oCurFont ) )
								{
									oCurFont.m_nID = oParam.oDocument->m_oFontTable.GetCount() + 1;
									oCurFont.m_sName = sFont;
									oParam.oDocument->m_oFontTable.DirectAddItem( oCurFont );
								}
								RtfFieldPtr oNewField = RtfFieldPtr( new RtfField() );
								oNewField->m_oInsert = TextItemContainerPtr( new TextItemContainer() );
								oNewField->m_oResult = TextItemContainerPtr( new TextItemContainer() );
								RtfCharPtr oNewChar( new RtfChar() );
								oNewChar->m_bRtfEncode = false;
								CString sFieldText;
								int nFontSize = 10;
								if( PROP_DEF != oNewProperty.m_nFontSize )
									nFontSize = oNewProperty.m_nFontSize / 2;
								sFieldText.AppendFormat(_T("SYMBOL %d \\\\f \"%s\" \\\\s %d"), nChar, sFont, nFontSize );
								oNewChar->setText( sFieldText );
								RtfParagraphPtr oNewInsertParagraph( new RtfParagraph() );
								oNewInsertParagraph->AddItem( oNewChar );
								oNewField->m_oInsert->AddItem( oNewInsertParagraph );

								oOutputParagraph.AddItem( oNewField );	
							}
						}
					}
				}

				return true;
			}
	};
