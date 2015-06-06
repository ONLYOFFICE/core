
#include "OOXParagraphReader.h"

#include "OOXpPrFrameReader.h"
#include "OOXpPrTabReader.h"
#include "OOXTableReader.h"

#include "../RtfOle.h"

bool OOXParagraphReader::Parse( ReaderParameter oParam , RtfParagraph& oOutputParagraph, CcnfStyle oConditionalTableStyle, bool& bStartNewSection )
{
	if (m_ooxParagraph == NULL) return false;

	//надо default стиль применять до OOXParagraphReader
	////применяем default
	//oOutputParagraph.m_oProperty = oParam.oRtf->m_oDefaultParagraphProp; 

	RtfTableStylePtr oResultTableStyle;
	//сначала применяем default стиль (если это нужно)

	if (m_ooxParagraph->m_oParagraphProperty)
	{
		OOXpPrReader opPrReader(m_ooxParagraph->m_oParagraphProperty);
		opPrReader.Parse( oParam, oOutputParagraph.m_oProperty, oConditionalTableStyle, bStartNewSection);
		oResultTableStyle = oConditionalTableStyle.ApplyTableStyle( oParam.poTableStyle );
	}
	else
	{
		oResultTableStyle = oConditionalTableStyle.ApplyTableStyle( oParam.poTableStyle );
		if( NULL != oResultTableStyle )
		{
			oOutputParagraph.m_oProperty.Merge( oResultTableStyle->m_oParProp );
			oOutputParagraph.m_oProperty.m_oCharProperty.Merge( oResultTableStyle->m_oCharProp );
		}
	}
	//формируем внешний стиль для вложенных элементов
	RtfCharStylePtr poExternalStyle;
	if( NULL != oResultTableStyle || PROP_DEF != oOutputParagraph.m_oProperty.m_nStyle )
	{
		poExternalStyle = RtfCharStylePtr( new RtfCharStyle() );
		if( NULL != oResultTableStyle )
			poExternalStyle->Merge( oResultTableStyle );
		if( PROP_DEF != oOutputParagraph.m_oProperty.m_nStyle )
		{
			RtfStylePtr oTempStyle;
			if( true == oParam.oRtf->m_oStyleTable.GetStyle(oOutputParagraph.m_oProperty.m_nStyle, oTempStyle) )
			{
				RtfStylePtr oResultStyle = oParam.oRtf->m_oStyleTable.GetStyleResulting( oTempStyle );
				poExternalStyle->Merge( oResultStyle );
			}
		}
	}

	m_ooxElement = dynamic_cast<OOX::WritingElementWithChilds<OOX::WritingElement>*>(m_ooxParagraph);
	return Parse2(oParam ,oOutputParagraph, oConditionalTableStyle, bStartNewSection, poExternalStyle );
}



bool OOXParagraphReader::Parse2( ReaderParameter oParam , RtfParagraph& oOutputParagraph, CcnfStyle oConditionalTableStyle, bool& bStartNewSection, RtfStylePtr poStyle )
{
	if (m_ooxElement == NULL) return false;
	
	RtfStylePtr poExternalStyle;

	for (long i = 0; i< m_ooxElement->m_arrItems.size(); i++)
	{
		if (m_ooxElement->m_arrItems[i] == NULL) continue;

		switch (m_ooxElement->m_arrItems[i]->getType())
		{
			case OOX::et_w_tbl:
			{
				oParam.oReader->m_nCurItap ++ ;
				RtfTablePtr oNewTabel( new RtfTable() );					
				OOX::Logic::CTbl * pTbl = dynamic_cast<OOX::Logic::CTbl*>(m_ooxElement->m_arrItems[i]);

				OOXTableReader oTableReader(pTbl);
				oTableReader.Parse( oParam, *oNewTabel);
				oOutputParagraph.AddItem( oNewTabel );
				oParam.oReader->m_nCurItap -- ;				
			}break;
			case OOX::et_w_r:
			{
				OOX::Logic::CRun * pRun = dynamic_cast<OOX::Logic::CRun*>(m_ooxElement->m_arrItems[i]);
				OOXRunReader m_oRunReader(pRun);
				m_oRunReader.Parse ( oParam, oOutputParagraph, poExternalStyle );
			}break;
			case OOX::et_w_fldSimple:
			{
				OOX::Logic::CFldSimple * pFldSimple = dynamic_cast<OOX::Logic::CFldSimple*>(m_ooxElement->m_arrItems[i]);
			
				RtfFieldPtr oCurField( new RtfField() );
				oCurField->m_oInsert = TextItemContainerPtr( new TextItemContainer() );
				oCurField->m_oResult = TextItemContainerPtr( new TextItemContainer() );
				//добавляем insert
				RtfCharPtr oNewChar( new RtfChar() );
				oNewChar->m_bRtfEncode = false;
				if (pFldSimple->m_sInstr.IsInit())
				{
					oNewChar->setText( pFldSimple->m_sInstr.get2() );
				}
				RtfParagraphPtr oNewInsertParagraph( new RtfParagraph() );
				oNewInsertParagraph->AddItem( oNewChar );
				oCurField->m_oInsert->AddItem( oNewInsertParagraph );
				
				//добаляем свойства
				if( TRUE == pFldSimple->m_oFldLock.ToBool() )
					oCurField->m_eMode = RtfField::fm_fldlock;

				if( TRUE == pFldSimple->m_oDirty.ToBool() )
					oCurField->m_eMode = RtfField::fm_flddirty;

				RtfParagraphPtr oNewResultParagraph( new RtfParagraph() );
				//применяем к новому параграфу default property
				oNewResultParagraph->m_oProperty = oParam.oRtf->m_oDefaultParagraphProp;
				oNewResultParagraph->m_oProperty.m_oCharProperty = oParam.oRtf->m_oDefaultCharProp;
				//применяем к новому параграфу свойства данного параграфа
				oNewResultParagraph->m_oProperty = oOutputParagraph.m_oProperty;

				if (pFldSimple->m_arrItems.size() >0)
				{
					OOXParagraphReader oSubParReader(pFldSimple);
					bool bStartNewSection;
					oSubParReader.Parse2( oParam, *oNewResultParagraph, CcnfStyle(), bStartNewSection, poExternalStyle);
					oCurField->m_oResult->AddItem( oNewResultParagraph 	);			
				}
				oOutputParagraph.AddItem( oCurField );
			}break;
			case OOX::et_w_hyperlink:
			{
				OOX::Logic::CHyperlink * pHyperlink = dynamic_cast<OOX::Logic::CHyperlink*>(m_ooxElement->m_arrItems[i]);

				if( pHyperlink->m_oId.IsInit() )
				{
					CString sTarget;
					smart_ptr<OOX::File> oFile = oParam.oDocx->GetDocument()->Find(pHyperlink->m_oId->GetValue());
					if ((oFile.IsInit()) && (OOX::FileTypes::HyperLink == oFile->type()))
					{
						OOX::HyperLink* pH = (OOX::HyperLink*)oFile.operator->();
						sTarget = pH->Uri().GetPath();
					}
					if( _T("") != sTarget )
					{
						//заменяем пробелы на %20
						sTarget.Replace( _T(" "), _T("%20") );
						CString sFileUrl = _T("file:///");
						if( 0 == sTarget.Find( sFileUrl ) )
						{
							int nFirstDDot = sTarget.Find( ':', sFileUrl.GetLength() );
							int nLen = sTarget.GetLength();
							if( -1 != nFirstDDot && nFirstDDot + 2 < nLen && '\\' == sTarget[nFirstDDot+1] )
							{
								if( '\\' != sTarget[nFirstDDot+2] ) 
									sTarget.Insert( nFirstDDot + 1, '\\'  );
							}
						}
						RtfFieldPtr oCurField( new RtfField() );
						oCurField->m_oInsert = TextItemContainerPtr( new TextItemContainer() );
						oCurField->m_oResult = TextItemContainerPtr( new TextItemContainer() );
						//добавляем insert
						RtfCharPtr oNewChar( new RtfChar() );
						oNewChar->m_bRtfEncode = false;
						CString sFieldText;
                        sFieldText.AppendFormat(_T("HYPERLINK \"%ls\""), sTarget.GetBuffer() );
						oNewChar->setText( sFieldText );
						RtfParagraphPtr oNewInsertParagraph( new RtfParagraph() );
						oNewInsertParagraph->AddItem( oNewChar );
						oCurField->m_oInsert->AddItem( oNewInsertParagraph );
						//добавляем свойства

						//pHyperlink->m_arrItems todoooo 
						//BOOL bLock = Strings::ToBoolean(oXmlReader.ReadNodeAttribute(i,_T("w:fldLock"),_T("false")));
						//if( TRUE == bLock )
						//	oCurField->m_eMode = RtfField::fm_fldlock;
						//BOOL bDirty = Strings::ToBoolean(oXmlReader.ReadNodeAttribute(i,_T("w:dirty"),_T("false")));
						//if( TRUE == bDirty )
						//	oCurField->m_eMode = RtfField::fm_flddirty;

						RtfParagraphPtr oNewResultParagraph( new RtfParagraph() );
						//применяем к новому параграфу default property
						oNewResultParagraph->m_oProperty = oParam.oRtf->m_oDefaultParagraphProp;
						oNewResultParagraph->m_oProperty.m_oCharProperty = oParam.oRtf->m_oDefaultCharProp;
						//применяем к новому параграфу свойства данного параграфа
						oNewResultParagraph->m_oProperty = oOutputParagraph.m_oProperty;
						
						if (pHyperlink->m_arrItems.size() >0)
						{
							OOXParagraphReader oSubParReader(pHyperlink);
							bool bStartNewSection;
							oSubParReader.Parse2( oParam, *oNewResultParagraph, CcnfStyle(), bStartNewSection, poExternalStyle);
							oCurField->m_oResult->AddItem( oNewResultParagraph );
						}
						oOutputParagraph.AddItem( oCurField );
					}
				}
				if( pHyperlink->m_sAnchor.IsInit() )
				{
					RtfFieldPtr oCurField( new RtfField() );
					oCurField->m_oInsert = TextItemContainerPtr( new TextItemContainer() );
					oCurField->m_oResult = TextItemContainerPtr( new TextItemContainer() );
					//добавляем insert
					RtfCharPtr oNewCharHYPER( new RtfChar() );
					oNewCharHYPER->m_bRtfEncode = false;
					oNewCharHYPER->setText( _T("HYPERLINK \\l \"") + pHyperlink->m_sAnchor.get() +_T("\"") );

					RtfParagraphPtr oNewInsertParagraph( new RtfParagraph() );
					oNewInsertParagraph->AddItem( oNewCharHYPER );

					oCurField->m_oInsert->AddItem( oNewInsertParagraph );
					////добаляем свойства
					//BOOL bLock = Strings::ToBoolean(oXmlReader.ReadNodeAttribute(i,_T("w:fldLock"),_T("false")));
					//if( TRUE == bLock )
					//	oCurField->m_eMode = RtfField::fm_fldlock;
					//BOOL bDirty = Strings::ToBoolean(oXmlReader.ReadNodeAttribute(i,_T("w:dirty"),_T("false")));
					//if( TRUE == bDirty )
					//	oCurField->m_eMode = RtfField::fm_flddirty;

					RtfParagraphPtr oNewResultParagraph( new RtfParagraph() );
					//применяем к новому параграфу default property
					oNewResultParagraph->m_oProperty = oParam.oRtf->m_oDefaultParagraphProp;
					oNewResultParagraph->m_oProperty.m_oCharProperty = oParam.oRtf->m_oDefaultCharProp;
					//применяем к новому параграфу свойства данного параграфа
					oNewResultParagraph->m_oProperty = oOutputParagraph.m_oProperty;
					
					if (pHyperlink->m_arrItems.size() > 0)
					{
						OOXParagraphReader oSubParReader(pHyperlink);					
						bool bStartNewSection;
						oSubParReader.Parse2( oParam, *oNewResultParagraph, CcnfStyle(), bStartNewSection, poExternalStyle);
						oCurField->m_oResult->AddItem( oNewResultParagraph );
					}
					oOutputParagraph.AddItem( oCurField );
				}
			}break;
			case OOX::et_w_bookmarkStart:
			{
				OOX::Logic::CBookmarkStart * pBookmarkStart = dynamic_cast<OOX::Logic::CBookmarkStart*>(m_ooxElement->m_arrItems[i]);
				RtfBookmarkStartPtr oNewBookmark( new RtfBookmarkStart() );
				
				oNewBookmark->m_sName = pBookmarkStart->m_sName.IsInit() ? pBookmarkStart->m_sName.get2() : _T("");

				if (pBookmarkStart->m_oColFirst.IsInit())
					oNewBookmark->nFirstColumn = pBookmarkStart->m_oColFirst->GetValue();
				if (pBookmarkStart->m_oColLast.IsInit())
					oNewBookmark->nLastColumn = pBookmarkStart->m_oColLast->GetValue();
				
				if(pBookmarkStart->m_oId.IsInit())
				{
					int nId = pBookmarkStart->m_oId->GetValue();
					oParam.oReader->m_aBookmarks.insert(std::pair<int, CString>( nId, oNewBookmark->m_sName ));
					oOutputParagraph.AddItem( oNewBookmark );
				}
			}break;
			case OOX::et_w_bookmarkEnd:
			{
				OOX::Logic::CBookmarkEnd * pBookmarkEnd = dynamic_cast<OOX::Logic::CBookmarkEnd*>(m_ooxElement->m_arrItems[i]);

				RtfBookmarkEndPtr oNewBookmark( new RtfBookmarkEnd() );
				//oNewBookmark->m_sName = pBookmarkEnd->;

				int nId = pBookmarkEnd->m_oId->GetValue();
				std::map<int, CString>::iterator pPair = oParam.oReader->m_aBookmarks.find( nId );
				if( pPair !=  oParam.oReader->m_aBookmarks.end())
				{
					oNewBookmark->m_sName = pPair->second;
					oOutputParagraph.AddItem( oNewBookmark );
				}
			}break;
			case OOX::et_w_smartTag:
			{
				OOX::Logic::CSmartTag * pSmartTag = dynamic_cast<OOX::Logic::CSmartTag*>(m_ooxElement->m_arrItems[i]);

				for (long i = 0 ; i < pSmartTag->m_arrItems.size(); i++)
				{
					OOX::Logic::CRun * pRun = dynamic_cast<OOX::Logic::CRun*>(pSmartTag->m_arrItems[i]);
					if (pRun == NULL) continue;

					OOXRunReader m_oRunReader(pRun);
					m_oRunReader.Parse( oParam, oOutputParagraph, poExternalStyle );
				}		
			}break;
			case OOX::et_m_oMathPara:
			{
				OOX::Logic::COMathPara * pMathPara = dynamic_cast<OOX::Logic::COMathPara*>(m_ooxElement->m_arrItems[i]);
				
				RtfMathPtr oNewMath( new RtfMath() );
				oNewMath->SetOOXName(_T("m:oMathPara") );
				
				OOXMathReader oMathRreader(pMathPara);
				if(true == oMathRreader.Parse( oParam, (*oNewMath) ) )
					oOutputParagraph.AddItem( oNewMath );
			}break;
			case OOX::et_w_sdt:
			{
				OOX::Logic::CSdt * pSdt = dynamic_cast<OOX::Logic::CSdt*>(m_ooxElement->m_arrItems[i]);
				if( pSdt->m_oSdtEndPr.IsInit() )
				{
					//todo
				}
				if(pSdt->m_oSdtContent.IsInit())
				{
					if (pSdt->m_oSdtContent->m_arrItems.size() > 0)
					{
						OOXParagraphReader oStdReader(pSdt->m_oSdtContent.GetPointer());
						bool bStartNewSection;
						oStdReader.Parse2( oParam, oOutputParagraph, CcnfStyle(), bStartNewSection, poExternalStyle );	
					}
				}
			}break;
		}
	}
	return true;
}

bool OOXRunReader::Parse( ReaderParameter oParam , RtfParagraph& oOutputParagraph, RtfStylePtr poStyle )
{
	if (m_ooxRun == NULL) return false;

	RtfCharProperty oNewProperty;
	oNewProperty.SetDefaultOOX();
	//применяем default
	oNewProperty = oParam.oRtf->m_oDefaultCharProp; 
	//применяем внешний стиль
	if( NULL != poStyle && TYPE_RTF_PROPERTY_STYLE_CHAR == poStyle->GetType() )
	{
		RtfCharStylePtr oCharStyle = boost::static_pointer_cast<RtfCharStyle, RtfStyle>( poStyle );
		oNewProperty.Merge( oCharStyle->m_oCharProp );
	}

	if (m_ooxRun->m_oRunProperty)
	{
		OOXrPrReader orPrReader(m_ooxRun->m_oRunProperty);
		orPrReader.Parse( oParam, oNewProperty );
	}

	for (long i =0 ; i < m_ooxRun->m_arrItems.size(); i++)
	{
		switch(m_ooxRun->m_arrItems[i]->getType())
		{
			case OOX::et_w_t:
			{
				OOX::Logic::CText * ooxText = dynamic_cast<OOX::Logic::CText*>(m_ooxRun->m_arrItems[i]);
				if (ooxText)
				{
					CString sValue;
					if ((ooxText->m_oSpace.IsInit()) && (ooxText->m_oSpace->GetValue() == SimpleTypes::xmlspacePreserve))
						sValue = RtfUtility::Preserve(ooxText->m_sText);
					else
						sValue = ooxText->m_sText;
				
					RtfCharPtr oNewChar = RtfCharPtr( new RtfChar() );
					oNewChar->m_oProperty = oNewProperty;
					oNewChar->setText( sValue );
					oOutputParagraph.AddItem( oNewChar );		
				}
			}break;
			case OOX::et_w_fldChar:
			{
				OOX::Logic::CFldChar * ooxFldChar = dynamic_cast<OOX::Logic::CFldChar*>(m_ooxRun->m_arrItems[i]);
				if ((ooxFldChar) && (ooxFldChar->m_oFldCharType.IsInit()))
				{
					switch(ooxFldChar->m_oFldCharType->GetValue())
					{
					case SimpleTypes::fldchartypeBegin:
						{
							OOXFieldBeginPtr oNewField( new OOXFieldBegin() );
							if (ooxFldChar->m_oFldLock.IsInit())
								oNewField->m_bLock = ooxFldChar->m_oFldLock->ToBool();
							if (ooxFldChar->m_oDirty.IsInit())
								oNewField->m_bDirty = ooxFldChar->m_oDirty->ToBool();
							oNewField->m_oCharProp = oNewProperty;
							oOutputParagraph.AddItem( oNewField );
						}break;
					case SimpleTypes::fldchartypeEnd: 
						{
							OOXFieldEndPtr oNewField( new OOXFieldEnd() );
							oOutputParagraph.AddItem( oNewField );
						}break;
					case SimpleTypes::fldchartypeSeparate:
						{
							OOXFieldSeparatePtr oNewField( new OOXFieldSeparate() );
							oOutputParagraph.AddItem( oNewField );
						}break;
					}
				}
			}break;
			case OOX::et_w_instrText:
			{
				OOX::Logic::CInstrText * ooxInstrText = dynamic_cast<OOX::Logic::CInstrText*>(m_ooxRun->m_arrItems[i]);
				OOXFieldInsertTextPtr oNewField( new OOXFieldInsertText() );
				RtfCharPtr oNewChar( new RtfChar() );
				if (ooxInstrText)
					oNewChar->setText( ooxInstrText->m_sText );//add preserve .. todooo
				oNewField->m_oText = oNewChar;
				oOutputParagraph.AddItem( oNewField );
			}break;
			case OOX::et_w_footnoteReference:
			{
				OOX::Logic::CFootnoteReference * ooxFootnoteReference = dynamic_cast<OOX::Logic::CFootnoteReference*>(m_ooxRun->m_arrItems[i]);
				if ((ooxFootnoteReference) && (ooxFootnoteReference->m_oId.IsInit()))
				{
					int nID = ooxFootnoteReference->m_oId->GetValue();
					std::map<int, TextItemContainerPtr>::iterator oPair = oParam.oReader->m_mapFootnotes.find( nID );
					if( oParam.oReader->m_mapFootnotes.end() != oPair )
					{
						RtfFootnotePtr oNewFootnote( new RtfFootnote() );
						oNewFootnote->m_oCharProp = oNewProperty;
						oNewFootnote->m_oContent = oPair->second;
						oOutputParagraph.AddItem( oNewFootnote );
					}
				}
			}break;
			case OOX::et_w_endnoteReference:
			{
				OOX::Logic::CEndnoteReference * ooxEndnoteReference = dynamic_cast<OOX::Logic::CEndnoteReference*>(m_ooxRun->m_arrItems[i]);
				if ((ooxEndnoteReference) && (ooxEndnoteReference->m_oId.IsInit()))
				{
					int nID = ooxEndnoteReference->m_oId->GetValue();
					std::map<int, TextItemContainerPtr>::iterator oPair = oParam.oReader->m_mapEndnotes.find ( nID );
					if( oParam.oReader->m_mapEndnotes.end() != oPair )
					{
						RtfFootnotePtr oNewEndnote( new RtfFootnote() );
						oNewEndnote->m_oCharProp = oNewProperty;
						oNewEndnote->m_oContent = oPair->second;
						oNewEndnote->m_bEndNote = true;
						oOutputParagraph.AddItem( oNewEndnote );
					}
				}
			}break;
			case OOX::et_w_object:
			{
				OOX::Logic::CObject * ooxObject = dynamic_cast<OOX::Logic::CObject*>(m_ooxRun->m_arrItems[i]);
				if (ooxObject)
				{
					long nOleWidth = PROP_DEF;
					long nOleHeight = PROP_DEF;

					RtfShapePtr aPictShape;
					RtfOlePtr	oCurOle;

					if (ooxObject->m_oShape.IsInit())
					{
						RtfShapePtr oNewShape( new RtfShape() );
						oNewShape->m_eShapeType = RtfShape::st_inline;
						
						OOXShapeReader oShapeReader(ooxObject->m_oShape.GetPointer());
						if( true == oShapeReader.Parse( oParam, oNewShape ) )
							 aPictShape = oNewShape;
					}
					//if (ooxObject->m_oGroupShape.IsInit())
					//{
					//	RtfShapeGroupPtr oNewShape( new RtfShapeGroup() );
					//	oNewShape->m_eShapeType = RtfShape::st_inline;
					//	OOXShapeGroupReader oShapeReader;
					//	if( true == oShapeReader.Parse( oParam, oNewShape ) )
					//		 aPictShape = oNewShape;
					//}
					if (ooxObject->m_oOleObject.IsInit())
					{
						RtfOlePtr oNewOle( new RtfOle() );
						
						oNewOle->m_nWidth = nOleWidth; //?? todooo
						oNewOle->m_nHeight = nOleHeight;

						int nShapeId = PROP_DEF;
						if (ooxObject->m_oOleObject->m_sShapeId.IsInit())
							nShapeId = oParam.oReader->m_oOOXIdGenerator.GetId(ooxObject->m_oOleObject->m_sShapeId.get2());
						if( PROP_DEF != nShapeId )
						{
							oNewOle->m_nShapeId = nShapeId;
							//ставим соответствующий shape
							if( NULL != aPictShape && aPictShape->m_nID == nShapeId )
								aPictShape->m_bIsOle = true;									
						}
						if( ooxObject->m_oOleObject->m_sProgId.IsInit() )
							oNewOle->m_sOleClass = ooxObject->m_oOleObject->m_sProgId.get2();
						
						if(ooxObject->m_oOleObject->m_oType.IsInit())
						{
							switch( ooxObject->m_oOleObject->m_oType->GetValue())
							{
								case SimpleTypes::oletypeLink: 	oNewOle->m_eOleType = RtfOle::ot_link;	break;
								case SimpleTypes::oletypeEmbed: oNewOle->m_eOleType = RtfOle::ot_emb;	break;
							}
						}
						if(ooxObject->m_oOleObject->m_oId.IsInit())
						{
							CString sRelativePath;
							smart_ptr<OOX::File> oFile = oParam.oDocx->GetDocument()->Find(ooxObject->m_oOleObject->m_oId->GetValue());
							
							if ((oFile.IsInit() && (OOX::FileTypes::OleObject == oFile->type())))
							{
								OOX::OleObject* pO = (OOX::OleObject*)oFile.operator->();
								sRelativePath = pO->m_sFilename;
							}
							//todooo проверить что тут за путь ..
							CString sOlePath = oParam.oReader->m_sPath + FILE_SEPARATOR_STR + sRelativePath;

							POLE::Storage *storage = new POLE::Storage(sOlePath.GetBuffer());
							if (storage)
							{
								//RtfOle2ToOle1Stream oStream;
								//oStream.lpstbl = new OLESTREAMVTBL();
								//oStream.lpstbl->Get = &OleGet2;
								//oStream.lpstbl->Put = &OlePut2;
								//todooo convert ole2 to ole1
								//if( SUCCEEDED( OleConvertIStorageToOLESTREAM( piStorage, &oStream ) ) )
								//{
								//	//сохраняем в файл
								//	CString sOleStorageName = Utils::CreateTempFile( oParam.oReader->m_sTempFolder );
								//	HANDLE hFile = CreateFile ( sOleStorageName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
								//	if( INVALID_HANDLE_VALUE != hFile )
								//	{
								//		DWORD dwByteWrite = 0;
								//		WriteFile( hFile, oStream.aBuffer.GetData(), (DWORD)oStream.aBuffer.size(), &dwByteWrite, NULL );
								//		CloseHandle( hFile );

								//		oNewOle->SetFilename( sOleStorageName );
								//		//только сейчас запоминаем
								//		oCurOle = oNewOle;
								//	}
								//	else
								//		Utils::RemoveDirOrFile( sOleStorageName );
								//}
								delete( storage );
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
			}break;
			case OOX::et_w_drawing:
			{
				OOX::Logic::CDrawing* ooxDrawing = dynamic_cast<OOX::Logic::CDrawing*>(m_ooxRun->m_arrItems[i]);
				RtfShapePtr oNewPicture( new RtfShape() );
			
				OOXPictureReader oPictureReader(ooxDrawing);
				if( true == oPictureReader.Parse( oParam, (*oNewPicture) ) )
				{
					 oOutputParagraph.AddItem( oNewPicture );
				}
			}break;
			case OOX::et_w_pict:
			{
				OOX::Logic::CPicture *ooxPicture = dynamic_cast<OOX::Logic::CPicture*>(m_ooxRun->m_arrItems[i]);
				if( (ooxPicture) && (ooxPicture->m_oShape.IsInit())) //??? todooo ваще то могут быть и др элементы/инициализции
				{
					RtfShapePtr oNewPicture( new RtfShape() );
					OOXShapeReader oShapeReader(ooxPicture->m_oShape.GetPointer());
					if( true == oShapeReader.Parse( oParam, oNewPicture ) )
						 oOutputParagraph.AddItem( oNewPicture );
				}
			}break;
			case OOX::et_w_nonBreakHyphen:
			{
				RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
				oNewChar->m_oProperty = oNewProperty;
				oNewChar->m_eType = RtfCharSpecial::rsc_NonBrHyphen;
				oOutputParagraph.AddItem( oNewChar );
			}break;
			case OOX::et_w_softHyphen:
			{
				RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
				oNewChar->m_oProperty = oNewProperty;
				oNewChar->m_eType = RtfCharSpecial::rsc_OptHyphen;
				oOutputParagraph.AddItem( oNewChar );
			}break;
			case OOX::et_w_pgNum:
			{
				RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
				oNewChar->m_oProperty = oNewProperty;
				oNewChar->m_eType = RtfCharSpecial::rsc_chpgn;
				oOutputParagraph.AddItem( oNewChar );
			}break;
			case OOX::et_w_footnoteRef:
			{
				RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
				oNewChar->m_oProperty = oNewProperty;
				oNewChar->m_eType = RtfCharSpecial::rsc_chftn;
				oOutputParagraph.AddItem( oNewChar );
			}break;
			case OOX::et_w_endnoteRef:
			{
				RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
				oNewChar->m_oProperty = oNewProperty;
				oNewChar->m_eType = RtfCharSpecial::rsc_chftnEnd;
				oOutputParagraph.AddItem( oNewChar );
			}break;
			case OOX::et_w_separator:
			{
				RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
				oNewChar->m_oProperty = oNewProperty;
				oNewChar->m_eType = RtfCharSpecial::rsc_chftnsep;
				oOutputParagraph.AddItem( oNewChar );
			}break;
			case OOX::et_w_continuationSeparator:
			{
				RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
				oNewChar->m_oProperty = oNewProperty;
				oNewChar->m_eType = RtfCharSpecial::rsc_chftnsepc;
				oOutputParagraph.AddItem( oNewChar );
			}break;
			case OOX::et_w_tab:
			{
				RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
				oNewChar->m_oProperty = oNewProperty;
				oNewChar->m_eType = RtfCharSpecial::rsc_tab;
				oOutputParagraph.AddItem( oNewChar );
			}break;
			case OOX::et_w_br:
			{
				OOX::Logic::CBr *ooxBr = dynamic_cast<OOX::Logic::CBr*>(m_ooxRun->m_arrItems[i]);
				
				RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
				oNewChar->m_oProperty = oNewProperty;
				
				if (ooxBr)
				{
					switch(ooxBr->m_oType.GetValue())
					{
					case SimpleTypes::brtypeColumn       : oNewChar->m_eType = RtfCharSpecial::rsc_page;	break;
					case SimpleTypes::brtypePage         : oNewChar->m_eType = RtfCharSpecial::rsc_column;	break;
					case SimpleTypes::brtypeTextWrapping :
						switch(ooxBr->m_oClear.GetValue())
						{
							case SimpleTypes::brclearAll   : oNewChar->m_nTextWrapBreak = 3;	break;
							case SimpleTypes::brclearLeft  : oNewChar->m_nTextWrapBreak = 1;	break;
							case SimpleTypes::brclearRight : oNewChar->m_nTextWrapBreak = 2;	break;
							default:	oNewChar->m_eType = RtfCharSpecial::rsc_line;
						}
						break;
					}
				}else oNewChar->m_eType = RtfCharSpecial::rsc_line;							
				oOutputParagraph.AddItem( oNewChar );
			}break;
			case OOX::et_mc_alternateContent:
			{//???? todooo
				//XmlUtils::IXMLDOMNodePtr oNode;
				//oXmlReader.GetNode( i , oNode );
				//XmlUtils::CXmlLiteReader oSubReader;
				//if( TRUE == oSubReader.OpenFromXmlNode( oNode ) )
				//	if( TRUE == oSubReader.ReadNode( _T("mc:Fallback") ) )
				//	{
				//		XmlUtils::IXMLDOMNodePtr oSubNode;
				//		oSubReader.GetNode( oSubNode );
				//		ReaderParameter oParam = oParam;
				//		oParam.oNode = oSubNode;
				//		OOXRunReader oSubReader;
				//		oSubReader.Parse( oParam, oOutputParagraph, poStyle );
				//	}
			}break;
			case OOX::et_w_sym:
			{
				OOX::Logic::CSym *ooxSym = dynamic_cast<OOX::Logic::CSym* >(m_ooxRun->m_arrItems[i]);

				if(ooxSym->m_oFont.IsInit() && ooxSym->m_oChar.IsInit() )
				{	
					CString sFont = *ooxSym->m_oFont;
					//CString sChar = ooxSym->m_oChar->GetValue();

					//sChar.MakeLower();
					////оставляем только 2 байта (4 символа)
					//if( sChar.GetLength() > 4 )
					//	sChar = sChar.Right( 4 );
					////убираем маску F000
					//if( sChar.GetLength() == 4 && 'f' == sChar[0] )
					//	sChar = sChar.Right( 3 );
					
					int nChar = ooxSym->m_oChar->GetValue();
					CString sCharUnicode; sCharUnicode.AppendChar( nChar );
					RtfFont oCurFont;
					if( false == oParam.oRtf->m_oFontTable.GetFont( sFont, oCurFont ) )
					{
						oCurFont.m_nID = oParam.oRtf->m_oFontTable.GetCount() + 1;
						oCurFont.m_sName = sFont;
						oParam.oRtf->m_oFontTable.DirectAddItem( oCurFont );
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
                    sFieldText.AppendFormat(_T("SYMBOL %d \\\\f \"%ls\" \\\\s %d"), nChar, sFont.GetBuffer(), nFontSize );
					oNewChar->setText( sFieldText );
					RtfParagraphPtr oNewInsertParagraph( new RtfParagraph() );
					oNewInsertParagraph->AddItem( oNewChar );
					oNewField->m_oInsert->AddItem( oNewInsertParagraph );

					oOutputParagraph.AddItem( oNewField );	
				}
			}break;
		}
	}
	return true;
}


bool OOXpPrReader::Parse( ReaderParameter oParam ,RtfParagraphProperty& oOutputProperty, CcnfStyle& oConditionalTableStyle, bool& bStartNewSection )
{
	if (m_ooxParaProps == NULL) return false;

	//применяем внешний стиль 
	if( NULL != oParam.poTableStyle )
	{
		if( m_ooxParaProps->m_oCnfStyle.IsInit() )
		{
			CcnfStyle ocnfStyle;
			OOXcnfStyleReader ocnfStyleReader(m_ooxParaProps->m_oCnfStyle.GetPointer());
			ocnfStyleReader.Parse( oParam, ocnfStyle );

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
	if(( m_ooxParaProps->m_oPStyle.IsInit() == false ) && (true == m_bDefStyle && NULL != oParam.oReader->m_oDefParStyle ))
	{
		oOutputProperty.Merge( oParam.oReader->m_oDefParStyle->m_oParProp );
		oOutputProperty.m_nStyle = oParam.oReader->m_oDefParStyle->m_nID;
	}

	//применяем все остальные свойчтва direct formating
	if( m_ooxParaProps->m_oPStyle.IsInit() && m_ooxParaProps->m_oPStyle->m_sVal.IsInit())
	{
		CString sStyleName = m_ooxParaProps->m_oPStyle->m_sVal.get2();
		RtfStylePtr oCurStyle;
		if( true == oParam.oRtf->m_oStyleTable.GetStyle(sStyleName, oCurStyle) )
		{
			if( oCurStyle->GetType() == TYPE_RTF_PROPERTY_STYLE_PARAGRAPH )
			{
				RtfStylePtr oResultStyle = oParam.oRtf->m_oStyleTable.GetStyleResulting( oCurStyle );
				RtfParagraphStylePtr oParagraphStyle = boost::static_pointer_cast<RtfParagraphStyle, RtfStyle>( oResultStyle );
				oOutputProperty.Merge( oParagraphStyle->m_oParProp );
				oOutputProperty.m_oCharProperty.Merge( oParagraphStyle->m_oCharProp );
				oOutputProperty.m_nStyle = oCurStyle->m_nID;
			}
		}
	}
	if( m_ooxParaProps->m_oSuppressAutoHyphens.IsInit() )
	{
		if (m_ooxParaProps->m_oSuppressAutoHyphens->m_oVal.ToBool())
			oOutputProperty.m_bAutoHyphenation = 0;
		else
			oOutputProperty.m_bAutoHyphenation = 1;
	}
	if( m_ooxParaProps->m_oKeepLines.IsInit())
		oOutputProperty.m_bKeep = m_ooxParaProps->m_oKeepLines->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxParaProps->m_oKeepNext.IsInit())
		oOutputProperty.m_bKeepNext = m_ooxParaProps->m_oKeepNext->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxParaProps->m_oOutlineLvl.IsInit() && m_ooxParaProps->m_oOutlineLvl->m_oVal.IsInit())
		oOutputProperty.m_nOutlinelevel = m_ooxParaProps->m_oOutlineLvl->m_oVal->GetValue();

	if (m_ooxParaProps->m_oPageBreakBefore.IsInit())
		oOutputProperty.m_bPageBB = m_ooxParaProps->m_oPageBreakBefore->m_oVal.ToBool() ? 1 : 0;

	if( m_ooxParaProps->m_oJc.IsInit() && m_ooxParaProps->m_oJc->m_oVal.IsInit())
	{
		switch(m_ooxParaProps->m_oJc->m_oVal->GetValue())
		{
		case SimpleTypes::jcBoth            : oOutputProperty.m_eAlign = RtfParagraphProperty::pa_qj;break;
		case SimpleTypes::jcCenter          : oOutputProperty.m_eAlign = RtfParagraphProperty::pa_qc;break;
		case SimpleTypes::jcDistribute      : oOutputProperty.m_eAlign = RtfParagraphProperty::pa_qd;break;
		case SimpleTypes::jcEnd             : oOutputProperty.m_eAlign = RtfParagraphProperty::pa_qr;break;
		case SimpleTypes::jcHighKashida     : oOutputProperty.m_eAlign = RtfParagraphProperty::pa_qk20;break;
		case SimpleTypes::jcLowKashida      : oOutputProperty.m_eAlign = RtfParagraphProperty::pa_qk0; break;
		case SimpleTypes::jcMediumKashida   : oOutputProperty.m_eAlign = RtfParagraphProperty::pa_qk10; break;
		case SimpleTypes::jcNumTab          : break;
		case SimpleTypes::jcStart           : oOutputProperty.m_eAlign = RtfParagraphProperty::pa_ql;break;
		case SimpleTypes::jcThaiDistribute  : break;
		case SimpleTypes::jcLeft            : oOutputProperty.m_eAlign = RtfParagraphProperty::pa_ql;break;
		case SimpleTypes::jcRight           : oOutputProperty.m_eAlign = RtfParagraphProperty::pa_qr;break;
		}
	}
	if( m_ooxParaProps->m_oTextAlignment.IsInit() && m_ooxParaProps->m_oTextAlignment->m_oVal.IsInit())
	{
		switch(m_ooxParaProps->m_oTextAlignment->m_oVal->GetValue())
		{
		case SimpleTypes::textalignAuto     : oOutputProperty.m_eFontAlign = RtfParagraphProperty::fa_faauto;	break;
		case SimpleTypes::textalignBaseLine : oOutputProperty.m_eFontAlign = RtfParagraphProperty::fa_faroman;	break;
		case SimpleTypes::textalignBottom   : oOutputProperty.m_eFontAlign = RtfParagraphProperty::fa_favar;	break;
		case SimpleTypes::textalignCenter   : oOutputProperty.m_eFontAlign = RtfParagraphProperty::fa_facenter;	break;
		case SimpleTypes::textalignTop      : oOutputProperty.m_eFontAlign = RtfParagraphProperty::fa_fahang;	break;
		}
	}
	if( m_ooxParaProps->m_oInd.IsInit() )
	{
		int nFirstLine = PROP_DEF;

		if (m_ooxParaProps->m_oInd->m_oHanging.IsInit())
			nFirstLine = m_ooxParaProps->m_oInd->m_oHanging->ToTwips();
		if( PROP_DEF != nFirstLine )
			oOutputProperty.m_nIndFirstLine = -nFirstLine;

		if (m_ooxParaProps->m_oInd->m_oFirstLine.IsInit())
			oOutputProperty.m_nIndFirstLine = m_ooxParaProps->m_oInd->m_oFirstLine->ToTwips();

		if (m_ooxParaProps->m_oInd->m_oStart.IsInit())
			oOutputProperty.m_nIndStart = m_ooxParaProps->m_oInd->m_oStart->ToTwips();

		if (m_ooxParaProps->m_oInd->m_oEnd.IsInit())
			oOutputProperty.m_nIndEnd = m_ooxParaProps->m_oInd->m_oEnd->ToTwips();
	}
	if (m_ooxParaProps->m_oAdjustRightInd.IsInit())
		oOutputProperty.m_bIndRightAuto = m_ooxParaProps->m_oAdjustRightInd->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxParaProps->m_oMirrorIndents.IsInit())
		oOutputProperty.m_bIndMirror = m_ooxParaProps->m_oMirrorIndents->m_oVal.ToBool() ? 1 : 0;

	if( m_ooxParaProps->m_oSpacing.IsInit())
	{
		if (m_ooxParaProps->m_oSpacing->m_oBefore.IsInit())
			oOutputProperty.m_nSpaceBefore = m_ooxParaProps->m_oSpacing->m_oBefore->ToTwips();
		if (m_ooxParaProps->m_oSpacing->m_oAfter.IsInit())
			oOutputProperty.m_nSpaceAfter = m_ooxParaProps->m_oSpacing->m_oAfter->ToTwips();
		if (m_ooxParaProps->m_oSpacing->m_oBeforeAutospacing.IsInit())
			oOutputProperty.m_nSpaceBeforeAuto = m_ooxParaProps->m_oSpacing->m_oBeforeAutospacing->ToBool();
		if (m_ooxParaProps->m_oSpacing->m_oAfterAutospacing.IsInit())
			oOutputProperty.m_nSpaceAfterAuto = m_ooxParaProps->m_oSpacing->m_oAfterAutospacing->ToBool();
		if (m_ooxParaProps->m_oSpacing->m_oBeforeLines.IsInit())
			oOutputProperty.m_nSpaceBeforeLine = m_ooxParaProps->m_oSpacing->m_oBeforeLines->GetValue();
		if (m_ooxParaProps->m_oSpacing->m_oAfterLines.IsInit())
			oOutputProperty.m_nSpaceAfterLine = m_ooxParaProps->m_oSpacing->m_oAfterLines->GetValue();
		
		if( m_ooxParaProps->m_oSpacing->m_oLine.IsInit())
		{
			if ( m_ooxParaProps->m_oSpacing->m_oLineRule.IsInit())
			{
				if (m_ooxParaProps->m_oSpacing->m_oLineRule->GetValue() == SimpleTypes::linespacingruleExact)
				{
					oOutputProperty.m_nSpaceBetween = - m_ooxParaProps->m_oSpacing->m_oLine->ToTwips();//twips ??? todooo
					oOutputProperty.m_nSpaceMultiLine = 0;
				}
				else if (m_ooxParaProps->m_oSpacing->m_oLineRule->GetValue() == SimpleTypes::linespacingruleAtLeast)
				{
					oOutputProperty.m_nSpaceBetween = m_ooxParaProps->m_oSpacing->m_oLine->ToTwips();
					oOutputProperty.m_nSpaceMultiLine = 0;
				}

			}
		}
		else
		{
			oOutputProperty.m_nSpaceBetween = 240;
			oOutputProperty.m_nSpaceMultiLine = 1;
		}
	}
	if (m_ooxParaProps->m_oSnapToGrid.IsInit())
		oOutputProperty.m_bSnapToGrid = m_ooxParaProps->m_oSnapToGrid->m_oVal.ToBool();
	
	if (m_ooxParaProps->m_oContextualSpacing.IsInit())
		oOutputProperty.m_bContextualSpacing = m_ooxParaProps->m_oContextualSpacing->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxParaProps->m_oBidi.IsInit())
		oOutputProperty.m_bRtl = m_ooxParaProps->m_oBidi->m_oVal.ToBool() ? 1 : 0;

	if( m_ooxParaProps->m_oWordWrap.IsInit() )
	{
		if( m_ooxParaProps->m_oWordWrap->m_oVal.ToBool() )
			oOutputProperty.m_bNoWordWrap = 0;
		else
			oOutputProperty.m_bNoWordWrap = 1;
	}
	if( m_ooxParaProps->m_oTextboxTightWrap.IsInit() && m_ooxParaProps->m_oTextboxTightWrap->m_oVal.IsInit())
	{
		switch(m_ooxParaProps->m_oTextboxTightWrap->m_oVal->GetValue())
		{
		case SimpleTypes::textboxtightwrapAllLines         : oOutputProperty.m_eTextBoxWrap = RtfParagraphProperty::tbw_txbxtwalways;	break;
		case SimpleTypes::textboxtightwrapFirstAndLastLine : oOutputProperty.m_eTextBoxWrap = RtfParagraphProperty::tbw_txbxtwfirstlast;break;
		case SimpleTypes::textboxtightwrapFirstLineOnly    : oOutputProperty.m_eTextBoxWrap = RtfParagraphProperty::tbw_txbxtwfirst;	break;
		case SimpleTypes::textboxtightwrapLastLineOnly     : oOutputProperty.m_eTextBoxWrap = RtfParagraphProperty::tbw_txbxtwlast;		break;
		case SimpleTypes::textboxtightwrapNone             : oOutputProperty.m_eTextBoxWrap = RtfParagraphProperty::tbw_txbxtwno;		break;
		}
	}
	if( m_ooxParaProps->m_oNumPr.IsInit() )
	{
		if( m_ooxParaProps->m_oNumPr->m_oIlvl.IsInit() && m_ooxParaProps->m_oNumPr->m_oIlvl->m_oVal.IsInit())
		{
			oOutputProperty.m_nListLevel = m_ooxParaProps->m_oNumPr->m_oIlvl->m_oVal->GetValue();
		}
		if( m_ooxParaProps->m_oNumPr->m_oNumID.IsInit() && m_ooxParaProps->m_oNumPr->m_oNumID->m_oVal.IsInit())
		{
			oOutputProperty.m_nListId = m_ooxParaProps->m_oNumPr->m_oNumID->m_oVal->GetValue();
			if( PROP_DEF == oOutputProperty.m_nListLevel )
				oOutputProperty.m_nListLevel = 0;
		}
		if( PROP_DEF != oOutputProperty.m_nListLevel && PROP_DEF != oOutputProperty.m_nListId )
		{
			//применяем direct свойства списка к параграфу
			RtfListOverrideProperty oListOverrideProperty;
			//ищем по override table
			if( true == oParam.oRtf->m_oListOverrideTabel.GetList( oOutputProperty.m_nListId, oListOverrideProperty ) )
			{
				RtfListProperty oListProperty;
				//Ищем по List Table
				if( true == oParam.oRtf->m_oListTabel.GetList( oListOverrideProperty.m_nListID, oListProperty) )
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
	if( m_ooxParaProps->m_oShd.IsInit() )
	{
		OOXShadingReader oShadingReader(m_ooxParaProps->m_oShd.GetPointer());
		oShadingReader.Parse( oParam, oOutputProperty.m_oShading  );
	}
	if( m_ooxParaProps->m_oPBdr.IsInit() ) //todo
	{
		RtfBorder oNewBorder;
		if( m_ooxParaProps->m_oPBdr->m_oTop.IsInit() )
		{
			OOXBorderReader oBorderReader(m_ooxParaProps->m_oPBdr->m_oTop.GetPointer());
			if( true ==  oBorderReader.Parse( oParam, oNewBorder) )
				oOutputProperty.m_oBorderTop = oNewBorder;
		}
		if( m_ooxParaProps->m_oPBdr->m_oBottom.IsInit() )
		{
			OOXBorderReader oBorderReader(m_ooxParaProps->m_oPBdr->m_oBottom.GetPointer());
			if( true ==  oBorderReader.Parse( oParam, oNewBorder) )
				oOutputProperty.m_oBorderBottom = oNewBorder;
		}
		if( m_ooxParaProps->m_oPBdr->m_oLeft.IsInit() )
		{
			OOXBorderReader oBorderReader(m_ooxParaProps->m_oPBdr->m_oLeft.GetPointer());
			if( true ==  oBorderReader.Parse( oParam, oNewBorder) )
				oOutputProperty.m_oBorderLeft = oNewBorder;
		}

		if( m_ooxParaProps->m_oPBdr->m_oRight.IsInit() )
		{
			OOXBorderReader oBorderReader(m_ooxParaProps->m_oPBdr->m_oRight.GetPointer());
			if( true ==  oBorderReader.Parse( oParam, oNewBorder) )
				oOutputProperty.m_oBorderRight = oNewBorder;
		}

		if( m_ooxParaProps->m_oPBdr->m_oBetween.IsInit() )
		{
			OOXBorderReader oBorderReader(m_ooxParaProps->m_oPBdr->m_oBetween.GetPointer());
			if( true ==  oBorderReader.Parse( oParam, oNewBorder) )
				oOutputProperty.m_oBorderBox = oNewBorder;
		}

		if( m_ooxParaProps->m_oPBdr->m_oBar.IsInit() )
		{
			OOXBorderReader oBorderReader(m_ooxParaProps->m_oPBdr->m_oBar.GetPointer());
			if( true ==  oBorderReader.Parse( oParam, oNewBorder) )
				oOutputProperty.m_oBorderBar = oNewBorder;
		}
	}
	
	if( m_ooxParaProps->m_oFramePr.IsInit() )
	{
		OOXpPrFrameReader oFrameReader(m_ooxParaProps->m_oFramePr.GetPointer());
		oFrameReader.Parse( oParam, oOutputProperty.m_oFrame );
	}
	
	if( m_ooxParaProps->m_oSuppressOverlap.IsInit())
		oOutputProperty.m_bOverlap = m_ooxParaProps->m_oSuppressOverlap->m_oVal.ToBool() ? 1 : 0;

	if( m_ooxParaProps->m_oTextDirection.IsInit() && m_ooxParaProps->m_oTextDirection->m_oVal.IsInit())
	{
		switch(m_ooxParaProps->m_oTextDirection->m_oVal->GetValue())
		{
		case SimpleTypes::textdirectionLr  : oOutputProperty.m_eTextFollow = RtfParagraphProperty::tf_frmtxlrtb; break;
		case SimpleTypes::textdirectionLrV : oOutputProperty.m_eTextFollow = RtfParagraphProperty::tf_frmtxlrtbv; break;
		case SimpleTypes::textdirectionRl  : oOutputProperty.m_eTextFollow = RtfParagraphProperty::tf_frmtxtbrl; break;
		case SimpleTypes::textdirectionRlV : break;
		case SimpleTypes::textdirectionTb  : oOutputProperty.m_eTextFollow = RtfParagraphProperty::tf_frmtxbtlr; break;
		case SimpleTypes::textdirectionTbV : oOutputProperty.m_eTextFollow = RtfParagraphProperty::tf_frmtxtbrlv;break;
		}
	}
	if( m_ooxParaProps->m_oTabs.IsInit())
	{
		OOXpPrTabReader oTabReader(m_ooxParaProps->m_oTabs.GetPointer());
		oTabReader.Parse( oParam, oOutputProperty.m_oTabs );
	}
	if( m_ooxParaProps->m_oSectPr.IsInit())
	{//todooo что то тут не тооооо 
		bStartNewSection = true;
		//OOXSectionPropertyReader oSectReader;
		//oSectReader.Parse( oParam, oParam.oReader->m_oCurSectionProp );
	}
	
	if( m_ooxParaProps->m_oRPr.IsInit() )
	{
		OOXrPrReader orPrReader(m_ooxParaProps->m_oRPr.GetPointer());
		orPrReader.Parse( oParam, oOutputProperty.m_oCharProperty );
	}
	return true;
}
bool OOXrPrReader::Parse( ReaderParameter oParam ,RtfCharProperty& oOutputProperty)
{
	if (m_ooxRunProps == NULL) return false;

	//сначала применяем стили
	//применяем стиль по-умолчанию,если не определен свой
	if(( m_ooxRunProps->m_oRStyle.IsInit() == false) && (true == m_bDefStyle && NULL != oParam.oReader->m_oDefCharStyle ))
	{
		oOutputProperty.Merge( oParam.oReader->m_oDefCharStyle->m_oCharProp );
	}

	if (m_ooxRunProps->m_oEffect.IsInit() && m_ooxRunProps->m_oEffect->m_oVal.IsInit())
	{
		switch(m_ooxRunProps->m_oEffect->m_oVal->GetValue())
		{
		case SimpleTypes::texteffectAntsBlack       : oOutputProperty.m_nAnimated = 4; break;
		case SimpleTypes::texteffectAntsRed         : oOutputProperty.m_nAnimated = 5; break;
		case SimpleTypes::texteffectBlinkBackground : oOutputProperty.m_nAnimated = 2; break;
		case SimpleTypes::texteffectLights          : oOutputProperty.m_nAnimated = 1; break;
		case SimpleTypes::texteffectNone            : oOutputProperty.m_nAnimated = 0; break;
		case SimpleTypes::texteffectShimmer         : oOutputProperty.m_nAnimated = 6; break;
		case SimpleTypes::texteffectSparkle         : oOutputProperty.m_nAnimated = 3; break;

		}
	}
	if (m_ooxRunProps->m_oRStyle.IsInit() && m_ooxRunProps->m_oRStyle->m_sVal.IsInit())
	{
		CString sStyleID = m_ooxRunProps->m_oRStyle->m_sVal.get2();
		RtfStylePtr oCurStyle;
		if( true == oParam.oRtf->m_oStyleTable.GetStyle(sStyleID, oCurStyle)) 
		{
			if( oCurStyle->GetType() == TYPE_RTF_PROPERTY_STYLE_CHAR )
			{
				oCurStyle = oParam.oRtf->m_oStyleTable.GetStyleResulting( oCurStyle );
				RtfCharStylePtr oCharStyle = boost::static_pointer_cast<RtfCharStyle, RtfStyle>( oCurStyle );
				oOutputProperty.m_nCharStyle = oCharStyle->m_nID;
				oOutputProperty.Merge( oCharStyle->m_oCharProp );
			}
		}
	}

	if (m_ooxRunProps->m_oBold.IsInit())
		oOutputProperty.m_bBold = m_ooxRunProps->m_oBold->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxRunProps->m_oCaps.IsInit())
		oOutputProperty.m_bCaps = m_ooxRunProps->m_oCaps->m_oVal.ToBool() ? 1 : 0;
	
	if (m_ooxRunProps->m_oW.IsInit() && m_ooxRunProps->m_oW->m_oVal.IsInit())
		oOutputProperty.m_nScalex = m_ooxRunProps->m_oW->m_oVal->GetValue();

	if (m_ooxRunProps->m_oPosition.IsInit() && m_ooxRunProps->m_oPosition->m_oVal.IsInit())
	{
		int nValue = m_ooxRunProps->m_oPosition->m_oVal->ToTwips();
		if( nValue >= 0 )
			oOutputProperty.m_nUp = nValue;
		else
			oOutputProperty.m_nDown = -nValue;
	}

	if (m_ooxRunProps->m_oEmboss.IsInit())
		oOutputProperty.m_bEmbo = m_ooxRunProps->m_oEmboss->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxRunProps->m_oSpacing.IsInit() && m_ooxRunProps->m_oSpacing->m_oVal.IsInit())
		oOutputProperty.m_nCharacterSpacing = m_ooxRunProps->m_oSpacing->m_oVal->ToTwips();

	if( m_ooxRunProps->m_oFitText.IsInit() && m_ooxRunProps->m_oFitText->m_oVal.IsInit())
	{
		int iID = m_ooxRunProps->m_oFitText->m_oID.IsInit();
	
			int nValue = m_ooxRunProps->m_oFitText->m_oVal->ToTwips();

			if(m_ooxRunProps->m_oFitText->m_oID.IsInit() == false )
			{
				oOutputProperty.m_nFitText = -1;
			}
			else
			{
				int nID = m_ooxRunProps->m_oFitText->m_oID->GetValue();
				if (oParam.oReader->m_nCurFittextId != nID ) //??? 
				{
					oParam.oReader->m_nCurFittextId = nID;
					oOutputProperty.m_nFitText = nValue;
				}
			}
	}
	if( m_ooxRunProps->m_oRFonts.IsInit())
	{
		OOXFontReader2 oFontReader2(m_ooxRunProps->m_oRFonts.GetPointer());
		oFontReader2.Parse( oParam, oOutputProperty.m_nFont);
	}

	if( m_ooxRunProps->m_oSz.IsInit() && m_ooxRunProps->m_oSz->m_oVal.IsInit())
		oOutputProperty.m_nFontSize = 2 * m_ooxRunProps->m_oSz->m_oVal->ToPoints();

	if (m_ooxRunProps->m_oItalic.IsInit())
		oOutputProperty.m_bItalic = m_ooxRunProps->m_oItalic->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxRunProps->m_oImprint.IsInit())
		oOutputProperty.m_bImprint = m_ooxRunProps->m_oImprint->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxRunProps->m_oKern.IsInit() && m_ooxRunProps->m_oKern->m_oVal.IsInit())
		oOutputProperty.m_nKerning = m_ooxRunProps->m_oKern->m_oVal->ToTwips();

	if (m_ooxRunProps->m_oRtL.IsInit())
		oOutputProperty.m_bRightToLeft = m_ooxRunProps->m_oRtL->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxRunProps->m_oCs.IsInit())
		oOutputProperty.m_nComplexScript = m_ooxRunProps->m_oCs->m_oVal.ToBool() ? 1 : 0;;

	if (m_ooxRunProps->m_oOutline.IsInit())
		oOutputProperty.m_bOutline = m_ooxRunProps->m_oOutline->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxRunProps->m_oSmallCaps.IsInit())
		oOutputProperty.m_bScaps = m_ooxRunProps->m_oSmallCaps->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxRunProps->m_oShadow.IsInit())
		oOutputProperty.m_bShadow = m_ooxRunProps->m_oShadow->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxRunProps->m_oStrike.IsInit())
		oOutputProperty.m_bStrike = m_ooxRunProps->m_oStrike->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxRunProps->m_oDStrike.IsInit())
		oOutputProperty.m_nStriked = m_ooxRunProps->m_oDStrike->m_oVal.ToBool() ? 1 : 0;
	
	if( m_ooxRunProps->m_oVertAlign.IsInit() && m_ooxRunProps->m_oVertAlign->m_oVal.IsInit())
	{
		switch(m_ooxRunProps->m_oVertAlign->m_oVal->GetValue())
		{
		case SimpleTypes::verticalalignrunBaseline    : break;
		case SimpleTypes::verticalalignrunSubscript   : oOutputProperty.m_bSub = 1;		break;
		case SimpleTypes::verticalalignrunSuperscript : oOutputProperty.m_bSuper = 1;	break;
		}
	}
	if( m_ooxRunProps->m_oHighlight.IsInit() && m_ooxRunProps->m_oHighlight->m_oVal.IsInit() )
	{
		//switch(m_ooxRunProps->m_oHighlight->m_oVal->GetValue())
		//{//незачем
		//}
		oOutputProperty.m_nHightlited = oParam.oRtf->m_oColorTable.AddItem(RtfColor(m_ooxRunProps->m_oHighlight->m_oVal->Get_R(),
																					m_ooxRunProps->m_oHighlight->m_oVal->Get_G(),
																					m_ooxRunProps->m_oHighlight->m_oVal->Get_B()));
	}
	if( m_ooxRunProps->m_oColor.IsInit() )
	{
		OOXColorReader oColorReader;
		RtfColor oColor;
		if( true == oColorReader.Parse( oParam, m_ooxRunProps->m_oColor.get2(), oColor ) )
			oOutputProperty.m_nForeColor = oParam.oRtf->m_oColorTable.AddItem( oColor );
	}
	if( m_ooxRunProps->m_oU.IsInit() &&  m_ooxRunProps->m_oU->m_oVal.IsInit())
	{
		switch(m_ooxRunProps->m_oU->m_oVal->GetValue())
		{
		case SimpleTypes::underlineDash            : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Dashed;				break;
		case SimpleTypes::underlineDashDotDotHeavy : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Thick_dash_dot_dotted;break;
		case SimpleTypes::underlineDashDotHeavy    : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Thick_dash_dotted;	break;
		case SimpleTypes::underlineDashedHeavy     : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Thick_dashed;			break;
		case SimpleTypes::underlineDashLong        : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Long_dashe;			break;
		case SimpleTypes::underlineDashLongHeavy   : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Thick_long_dashed;	break;
		case SimpleTypes::underlineDotDash         : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Dash_dotted;			break;
		case SimpleTypes::underlineDotDotDash      : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Dash_dot_dotted;		break;
		case SimpleTypes::underlineDotted          : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Dotted;				break;
		case SimpleTypes::underlineDottedHeavy     : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Thick_dotted;			break;
		case SimpleTypes::underlineDouble          : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Double;				break;
		case SimpleTypes::underlineNone            : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_none;					break;
		case SimpleTypes::underlineSingle          : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Single;				break;
		case SimpleTypes::underlineThick           : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Thick;				break;
		case SimpleTypes::underlineWave            : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Wave;					break;
		case SimpleTypes::underlineWavyDouble      : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Double_wave;			break;
		case SimpleTypes::underlineWavyHeavy       : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Heavy_wave;			break;
		case SimpleTypes::underlineWords           : oOutputProperty.m_eUnderStyle = RtfCharProperty::uls_Word;					break;
		}

		if (m_ooxRunProps->m_oU->m_oColor.IsInit())
		{
			RtfColor oColor(m_ooxRunProps->m_oU->m_oColor->Get_R(), m_ooxRunProps->m_oU->m_oColor->Get_G(), m_ooxRunProps->m_oU->m_oColor->Get_B());	
			oOutputProperty.m_nUnderlineColor =  oParam.oRtf->m_oColorTable.AddItem( oColor );
		}//todooo theme color, tint, shadow
	}
	if( m_ooxRunProps->m_oBdr.IsInit())
	{
		OOXBorderReader oBorderReader(m_ooxRunProps->m_oBdr.GetPointer());
		oBorderReader.Parse( oParam, oOutputProperty.m_poBorder );
	}
	if(m_ooxRunProps->m_oShadow.IsInit())
	{
		oOutputProperty.m_poShading.m_nValue = m_ooxRunProps->m_oShadow->m_oVal.ToBool() ? 1 : 0;
		//OOXShadingReader oShadingReader(m_ooxRunProps->m_oShadow.GetPointer());
		//oShadingReader.Parse( oParam, oOutputProperty.m_poShading );
	}
	return true;
}
bool OOXpPrFrameReader::Parse( ReaderParameter oParam ,RtfFrame& oOutputProperty)
{
	if (m_ooxFramePr == NULL) return false;

	oOutputProperty.m_nWidth	= m_ooxFramePr->m_oW.IsInit() ? m_ooxFramePr->m_oW->ToTwips() : PROP_DEF;//todooo twips??? pt ???
	oOutputProperty.m_nHeight	= m_ooxFramePr->m_oH.IsInit() ? m_ooxFramePr->m_oH->ToTwips() : PROP_DEF;

	if (m_ooxFramePr->m_oHAnchor.IsInit())
	{
		switch(m_ooxFramePr->m_oHAnchor->GetValue())
		{
			case SimpleTypes::hanchorMargin : oOutputProperty.m_eHRef = RtfFrame::hr_phmrg;	break;
			case SimpleTypes::hanchorPage   : oOutputProperty.m_eHRef = RtfFrame::hr_phpg;	break;
			case SimpleTypes::hanchorText   : oOutputProperty.m_eHRef = RtfFrame::hr_phcol;	break;
		}
	}
	oOutputProperty.m_nHPos = m_ooxFramePr->m_oX.IsInit() ? m_ooxFramePr->m_oX->ToTwips() : PROP_DEF;//???
	oOutputProperty.m_nVPos = m_ooxFramePr->m_oY.IsInit() ? m_ooxFramePr->m_oY->ToTwips() : PROP_DEF;//???
		
	if (m_ooxFramePr->m_oXAlign.IsInit())
	{
		switch(m_ooxFramePr->m_oXAlign->GetValue())
		{
			case SimpleTypes::xalignCenter  : oOutputProperty.m_eHPos = RtfFrame::hp_posxc;	break;
			case SimpleTypes::xalignInside  : oOutputProperty.m_eHPos = RtfFrame::hp_posxi;	break;
			case SimpleTypes::xalignLeft    : oOutputProperty.m_eHPos = RtfFrame::hp_posxl;	break;
			case SimpleTypes::xalignOutside : oOutputProperty.m_eHPos = RtfFrame::hp_posxo;	break;
			case SimpleTypes::xalignRight   : oOutputProperty.m_eHPos = RtfFrame::hp_posxr;	break;
		}
	}

	if (m_ooxFramePr->m_oVAnchor.IsInit())
	{
		switch(m_ooxFramePr->m_oVAnchor->GetValue())
		{
			case SimpleTypes::vanchorMargin : oOutputProperty.m_eVRef = RtfFrame::vr_pvmrg;	break;
			case SimpleTypes::vanchorPage   : oOutputProperty.m_eVRef = RtfFrame::vr_pvpg;	break;
			case SimpleTypes::vanchorText   : oOutputProperty.m_eVRef = RtfFrame::vr_pvpara;break;
		}
	}
	if (m_ooxFramePr->m_oYAlign.IsInit())
	{
		switch(m_ooxFramePr->m_oYAlign->GetValue())
		{
			case SimpleTypes::yalignBottom  : oOutputProperty.m_eVPos = RtfFrame::vp_posyb;	break;
			case SimpleTypes::yalignCenter  : oOutputProperty.m_eVPos = RtfFrame::vp_posyc;	break;
			case SimpleTypes::yalignInline  : oOutputProperty.m_eVPos = RtfFrame::vp_posyil;break;
			case SimpleTypes::yalignInside  : oOutputProperty.m_eVPos = RtfFrame::vp_posyin;break;
			case SimpleTypes::yalignOutside : oOutputProperty.m_eVPos = RtfFrame::vp_posyout;break;
			case SimpleTypes::yalignTop     : oOutputProperty.m_eVPos = RtfFrame::vp_posyt;	break;
		}
	}
	oOutputProperty.m_bLockAnchor = m_ooxFramePr->m_oAnchorLock.IsInit()? m_ooxFramePr->m_oAnchorLock->ToBool() : false;
	if (m_ooxFramePr->m_oWrap.IsInit())
	{
		switch(m_ooxFramePr->m_oWrap->GetValue())
		{
			case SimpleTypes::wrapAround    : oOutputProperty.m_eWrap = RtfFrame::tw_wraparound;	break;
			case SimpleTypes::wrapAuto      : oOutputProperty.m_eWrap = RtfFrame::tw_wrapdefault;	break;
			//case SimpleTypes::wrapNone      : break;
			//case SimpleTypes::wrapNotBeside : break;
			case SimpleTypes::wrapThrough   : oOutputProperty.m_eWrap = RtfFrame::tw_wrapthrough;	break;
			case SimpleTypes::wrapTight     : oOutputProperty.m_eWrap = RtfFrame::tw_wraptight;		break;
		}
	}
	if (m_ooxFramePr->m_oDropCap.IsInit())
	{
        switch(m_ooxFramePr->m_oDropCap->GetValue())
		{
			case SimpleTypes::dropcapDrop   : oOutputProperty.m_DropcapType = 1; break;
			case SimpleTypes::dropcapMargin : oOutputProperty.m_DropcapType = 2; break;
			case SimpleTypes::dropcapNone   : break;
		}
	}
	oOutputProperty.m_DropcapLines = m_ooxFramePr->m_oLines.IsInit() ? m_ooxFramePr->m_oLines->GetValue() : 0; //todoo ??? или 1 ???

	if( m_ooxFramePr->m_oHSpace.IsInit() )
	{
		oOutputProperty.m_nHorSpace = m_ooxFramePr->m_oHSpace->ToTwips();//todooo twips???
		if( PROP_DEF == oOutputProperty.m_nAllSpace || oOutputProperty.m_nAllSpace < oOutputProperty.m_nHorSpace )
		{
			oOutputProperty.m_nAllSpace = oOutputProperty.m_nHorSpace;
		}
	}
	if( m_ooxFramePr->m_oVSpace.IsInit() )
	{
		oOutputProperty.m_nVerSpace = m_ooxFramePr->m_oVSpace->ToTwips();//todooo twips???
		if( PROP_DEF == oOutputProperty.m_nAllSpace || oOutputProperty.m_nAllSpace < oOutputProperty.m_nVerSpace )
		{
			oOutputProperty.m_nAllSpace = oOutputProperty.m_nVerSpace;
		}
	}

	return true;
}
bool OOXSectionPropertyReader::Parse( ReaderParameter oParam , RtfSectionProperty& oOutput)
{
	if (m_ooxSectionProperty == NULL) return false;
	
	oOutput.SetDefaultOOX();

	if (m_ooxSectionProperty->m_oBidi.IsInit())
		oOutput.m_bBidi = m_ooxSectionProperty->m_oBidi->m_oVal.ToBool() ? 1 : 0;

	if(m_ooxSectionProperty->m_oPaperSrc.IsInit())
	{
		if (m_ooxSectionProperty->m_oPaperSrc->m_oFirst.IsInit()) 
			oOutput.m_nPaperSourceFirst = m_ooxSectionProperty->m_oPaperSrc->m_oFirst->GetValue();
		if (m_ooxSectionProperty->m_oPaperSrc->m_oOther.IsInit()) 
			oOutput.m_nPaperSourceOther = m_ooxSectionProperty->m_oPaperSrc->m_oOther->GetValue();
	}
	if (m_ooxSectionProperty->m_oRtlGutter.IsInit())
		oOutput.m_bRtlGutter =  m_ooxSectionProperty->m_oRtlGutter->m_oVal.ToBool() ;

	if( (m_ooxSectionProperty->m_oType.IsInit() ) && (m_ooxSectionProperty->m_oType->m_oVal.IsInit() ))
	{
		switch(m_ooxSectionProperty->m_oType->m_oVal->GetValue())
		{
			case SimpleTypes::sectionmarkContinious : oOutput.m_eSectionBreak = RtfSectionProperty::sb_sbknone; break;
			case SimpleTypes::sectionmarkEvenPage   : oOutput.m_eSectionBreak = RtfSectionProperty::sb_sbkeven; break;
			case SimpleTypes::sectionmarkNextColumn : oOutput.m_eSectionBreak = RtfSectionProperty::sb_sbkcol;	break;
			case SimpleTypes::sectionmarkNextPage   : oOutput.m_eSectionBreak = RtfSectionProperty::sb_sbkpage; break;
			case SimpleTypes::sectionmarkOddPage    : oOutput.m_eSectionBreak = RtfSectionProperty::sb_sbkodd;	break;
		}
	}
	if( m_ooxSectionProperty->m_oCols.IsInit() )
	{
		if(m_ooxSectionProperty->m_oCols->m_oNum.IsInit())
			oOutput.m_nColumnNumber = m_ooxSectionProperty->m_oCols->m_oNum->GetValue();
		
		if(m_ooxSectionProperty->m_oCols->m_oSpace.IsInit())
			oOutput.m_nColumnSpace = m_ooxSectionProperty->m_oCols->m_oSpace->ToTwips(); //todooo twips????	

		for( int i = 0; i < m_ooxSectionProperty->m_oCols->m_arrColumns.size(); i++ )
		{
			RtfSectionProperty::ColumnProperty::CollumnVar oNewColumn;
			
			if (m_ooxSectionProperty->m_oCols->m_arrColumns[i] != NULL)
			{
				if(m_ooxSectionProperty->m_oCols->m_arrColumns[i]->m_oW.IsInit() )
					oNewColumn.m_nColumnWidth = m_ooxSectionProperty->m_oCols->m_arrColumns[i]->m_oW->ToTwips(); //twips??? todooo 

				if(m_ooxSectionProperty->m_oCols->m_arrColumns[i]->m_oSpace.IsInit())
					oNewColumn.m_nColumnSpaceToRightOfCol = m_ooxSectionProperty->m_oCols->m_arrColumns[i]->m_oSpace->ToTwips();
			}
			oOutput.m_oCollumnProperty.m_aCollumnProperty.push_back( oNewColumn );
		}
	}
	if(m_ooxSectionProperty->m_oEndnotePr.IsInit())
	{
		if (m_ooxSectionProperty->m_oEndnotePr->m_oNumRestart.IsInit())
		{
			if (m_ooxSectionProperty->m_oEndnotePr->m_oNumRestart->m_oVal.IsInit())
			{
				switch(m_ooxSectionProperty->m_oEndnotePr->m_oNumRestart->m_oVal->GetValue())
				{
					case SimpleTypes::restartnumberContinious : break;// ???? todooo проверить
					case SimpleTypes::restartnumberEachPage   : oOutput.m_eEndnotesRestart = RtfSectionProperty::er_saftnrestart; break;
					case SimpleTypes::restartnumberEachSect   : oOutput.m_eEndnotesRestart = RtfSectionProperty::er_saftnrstcont; break;
				}
			}
		}
		if (m_ooxSectionProperty->m_oEndnotePr->m_oNumStart.IsInit() && 
							m_ooxSectionProperty->m_oEndnotePr->m_oNumStart->m_oVal.IsInit())
			oOutput.m_nEndnotesStart = m_ooxSectionProperty->m_oEndnotePr->m_oNumStart->m_oVal->GetValue();

		if (m_ooxSectionProperty->m_oEndnotePr->m_oNumFmt.IsInit())
		{
			if (m_ooxSectionProperty->m_oEndnotePr->m_oNumFmt->m_oVal.IsInit())
			{
				int enumVal = m_ooxSectionProperty->m_oEndnotePr->m_oNumFmt->m_oVal->GetValue();
				//todo
			}
			oOutput.m_eEndnotesRestart = RtfSectionProperty::er_saftnrestart;
		}		
	}
	if(m_ooxSectionProperty->m_oFootnotePr.IsInit() )
	{
		if(m_ooxSectionProperty->m_oFootnotePr->m_oPos.IsInit() && 
						m_ooxSectionProperty->m_oFootnotePr->m_oPos->m_oVal.IsInit())
		{
			switch (m_ooxSectionProperty->m_oFootnotePr->m_oPos->m_oVal->GetValue())
			{
			case SimpleTypes::ftnposBeneathText : oOutput.m_eFootnotesJust = RtfSectionProperty::fj_sftntj; break;
			case SimpleTypes::ftnposDocEnd      : break;// todooo !!!
			case SimpleTypes::ftnposPageBottom  : oOutput.m_eFootnotesJust = RtfSectionProperty::fj_sftnbj; break;
			case SimpleTypes::ftnposSectEnd     : break;// todooo !!!
			}
		}
		if (m_ooxSectionProperty->m_oFootnotePr->m_oNumStart.IsInit() && 
					m_ooxSectionProperty->m_oFootnotePr->m_oNumStart->m_oVal.IsInit())
			oOutput.m_nFootnotesStart = m_ooxSectionProperty->m_oFootnotePr->m_oNumStart->m_oVal->GetValue();
		
		if(m_ooxSectionProperty->m_oFootnotePr->m_oNumRestart.IsInit() &&
			m_ooxSectionProperty->m_oFootnotePr->m_oNumRestart->m_oVal.IsInit())
		{
			switch(m_ooxSectionProperty->m_oFootnotePr->m_oNumRestart->m_oVal->GetValue())
			{
			case SimpleTypes::restartnumberContinious : oOutput.m_eFootnotesRestart = RtfSectionProperty::fr_sftnrstcont; break;
			case SimpleTypes::restartnumberEachPage   : oOutput.m_eFootnotesRestart = RtfSectionProperty::fr_sftnrstpg; break;
			case SimpleTypes::restartnumberEachSect   : oOutput.m_eFootnotesRestart = RtfSectionProperty::fr_sftnrestart; break;
			}
		}
		if(m_ooxSectionProperty->m_oFootnotePr->m_oNumFmt.IsInit())
		{
			if (m_ooxSectionProperty->m_oFootnotePr->m_oNumFmt->m_oVal.IsInit())
				int enumVal = m_ooxSectionProperty->m_oFootnotePr->m_oNumFmt->m_oVal->GetValue();
			//todo
			oOutput.m_eFootnotesRestart = RtfSectionProperty::fr_sftnrstpg;
		}
	}
	if (m_ooxSectionProperty->m_oLnNumType.IsInit() )
	{
		if (m_ooxSectionProperty->m_oLnNumType->m_oCountBy.IsInit())
			oOutput.m_nLineModulus = m_ooxSectionProperty->m_oLnNumType->m_oCountBy->GetValue();

		if( m_ooxSectionProperty->m_oLnNumType->m_oDistance.IsInit() )
			oOutput.m_nLineX = m_ooxSectionProperty->m_oLnNumType->m_oDistance->ToTwips(); //twips??? todooo
	
		if( m_ooxSectionProperty->m_oLnNumType->m_oStart.IsInit())
			oOutput.m_nLineStart = m_ooxSectionProperty->m_oLnNumType->m_oStart->GetValue();

		if( m_ooxSectionProperty->m_oLnNumType->m_oRestart.IsInit())
		{
			switch(m_ooxSectionProperty->m_oLnNumType->m_oRestart->GetValue())
			{
				case SimpleTypes::linenumberrestartContinious : oOutput.m_eLineNumberRestart = RtfSectionProperty::lnr_linecont; break;
				case SimpleTypes::linenumberrestartNewPage    : oOutput.m_eLineNumberRestart = RtfSectionProperty::lnr_lineppage; break;
				case SimpleTypes::linenumberrestartNewSection : oOutput.m_eLineNumberRestart = RtfSectionProperty::lnr_linerestart; break;
			}
		}
	}
	if (m_ooxSectionProperty->m_oPgSz.IsInit())
	{
		if( m_ooxSectionProperty->m_oPgSz->m_oW.IsInit())
			oOutput.m_nPageWidth = m_ooxSectionProperty->m_oPgSz->m_oW->ToTwips();//?? pt?? tw??

		if( m_ooxSectionProperty->m_oPgSz->m_oH.IsInit())
			oOutput.m_nPageHeight = m_ooxSectionProperty->m_oPgSz->m_oH->ToTwips();

		if (m_ooxSectionProperty->m_oPgSz->m_oOrient.IsInit())
		{
			if (m_ooxSectionProperty->m_oPgSz->m_oOrient->GetValue() == SimpleTypes::pageorientLandscape)
				oOutput.m_bLandscapeFormat = 1;
		}
	}
	if (m_ooxSectionProperty->m_oPgMar.IsInit())
	{
		if(m_ooxSectionProperty->m_oPgMar->m_oLeft.IsInit() )
			oOutput.m_nMarginLeft = m_ooxSectionProperty->m_oPgMar->m_oLeft->ToTwips();

		if( m_ooxSectionProperty->m_oPgMar->m_oRight.IsInit() )
			oOutput.m_nMarginRight = m_ooxSectionProperty->m_oPgMar->m_oRight->ToTwips();

		if( m_ooxSectionProperty->m_oPgMar->m_oTop.IsInit() )
			oOutput.m_nMarginTop = m_ooxSectionProperty->m_oPgMar->m_oTop->ToTwips();

		if( m_ooxSectionProperty->m_oPgMar->m_oBottom.IsInit() )
			oOutput.m_nMarginBottom = m_ooxSectionProperty->m_oPgMar->m_oBottom->ToTwips();

		if(m_ooxSectionProperty->m_oPgMar->m_oGutter.IsInit() )
			oOutput.m_nGutterMarginWidth = m_ooxSectionProperty->m_oPgMar->m_oGutter->ToTwips();

		if( m_ooxSectionProperty->m_oPgMar->m_oHeader.IsInit() )
			oOutput.m_nHeaderTop = m_ooxSectionProperty->m_oPgMar->m_oHeader->ToTwips();

		if(m_ooxSectionProperty->m_oPgMar->m_oFooter.IsInit())
			oOutput.m_nFooterBottom = m_ooxSectionProperty->m_oPgMar->m_oFooter->ToTwips();
	}
	if (m_ooxSectionProperty->m_oTitlePg.IsInit())
		oOutput.m_bTitlePage = m_ooxSectionProperty->m_oTitlePg->m_oVal.ToBool() ? 1 : 0;
		
	if (m_ooxSectionProperty->m_oPgNumType.IsInit())
	{
		if( m_ooxSectionProperty->m_oPgNumType->m_oStart.IsInit() )
			oOutput.m_nPageNumberStart = m_ooxSectionProperty->m_oPgNumType->m_oStart->GetValue();
	}
	
	if (m_ooxSectionProperty->m_oVAlign.IsInit() &&
					m_ooxSectionProperty->m_oVAlign->m_oVal.IsInit())
	{
		switch(m_ooxSectionProperty->m_oVAlign->m_oVal->GetValue())
		{
			case SimpleTypes::verticaljcBoth   : oOutput.m_eVerticalAlignment = RtfSectionProperty::va_vertalj;	break;
			case SimpleTypes::verticaljcBottom : oOutput.m_eVerticalAlignment = RtfSectionProperty::va_vertalb;	break;
			case SimpleTypes::verticaljcCenter : oOutput.m_eVerticalAlignment = RtfSectionProperty::va_vertalc;	break;
			case SimpleTypes::verticaljcTop    : oOutput.m_eVerticalAlignment = RtfSectionProperty::va_vertalt;	break;

		}
	}
	if (m_ooxSectionProperty->m_oTextDirection.IsInit() &&
							m_ooxSectionProperty->m_oTextDirection->m_oVal.IsInit())
	{
		switch(m_ooxSectionProperty->m_oTextDirection->m_oVal->GetValue())
		{
			case SimpleTypes::textdirectionLr  : oOutput.m_nTextFollow = 0; break;
			case SimpleTypes::textdirectionLrV : oOutput.m_nTextFollow = 4; break;
			case SimpleTypes::textdirectionRl  : oOutput.m_nTextFollow = 3; break;
			case SimpleTypes::textdirectionRlV : oOutput.m_nTextFollow = 3; break;
			case SimpleTypes::textdirectionTb  : oOutput.m_nTextFollow = 0; break;
			case SimpleTypes::textdirectionTbV : oOutput.m_nTextFollow = 5; break;
				//oOutput.m_nTextFollow = 2; bottom to top ( .. todooo переделать DocxFormat
		}
	}
	if (m_ooxSectionProperty->m_oPgBorders.IsInit() )
	{
		oOutput.m_nBorderMeasure = 0;
		if (m_ooxSectionProperty->m_oPgBorders->m_oDisplay.IsInit())
		{
			switch(m_ooxSectionProperty->m_oPgBorders->m_oDisplay->GetValue())
			{
				case SimpleTypes::pageborderdisplayAllPages     : oOutput.m_nBorderMeasure = oOutput.m_nBorderMeasure | 0; break;
				case SimpleTypes::pageborderdisplayFirstPage    : oOutput.m_nBorderMeasure = oOutput.m_nBorderMeasure | 1; break;
				case SimpleTypes::pageborderdisplayNotFirstPage : oOutput.m_nBorderMeasure = oOutput.m_nBorderMeasure | 2; break;
			}
		}
		if (m_ooxSectionProperty->m_oPgBorders->m_oZOrder.IsInit())
		{
            switch(m_ooxSectionProperty->m_oPgBorders->m_oZOrder->GetValue())
			{
			case SimpleTypes::pageborderzorderBack  : oOutput.m_nBorderMeasure = oOutput.m_nBorderMeasure | 8; break;
			case SimpleTypes::pageborderzorderFront : break;
			}
		}
		if (m_ooxSectionProperty->m_oPgBorders->m_oOffsetFrom.IsInit())
		{
			switch(m_ooxSectionProperty->m_oPgBorders->m_oOffsetFrom->GetValue())
			{
			case SimpleTypes::pageborderoffsetPage : oOutput.m_nBorderMeasure = oOutput.m_nBorderMeasure | 32; break;
			case SimpleTypes::pageborderoffsetText : break;
			}
		}
		if (m_ooxSectionProperty->m_oPgBorders->m_oLeft.IsInit() )
		{
			OOXBorderReader oBorderReader(m_ooxSectionProperty->m_oPgBorders->m_oLeft.GetPointer());
			oBorderReader.Parse( oParam, oOutput.m_oBorderLeft );
		}
		if (m_ooxSectionProperty->m_oPgBorders->m_oTop.IsInit() )
		{
			OOXBorderReader oBorderReader(dynamic_cast<ComplexTypes::Word::CPageBorder*>(m_ooxSectionProperty->m_oPgBorders->m_oTop.GetPointer()));
			oBorderReader.Parse( oParam, oOutput.m_oBorderTop );
		}
		if (m_ooxSectionProperty->m_oPgBorders->m_oRight.IsInit())
		{
			OOXBorderReader oBorderReader(m_ooxSectionProperty->m_oPgBorders->m_oRight.GetPointer());
			oBorderReader.Parse( oParam, oOutput.m_oBorderRight );
		}
		if (m_ooxSectionProperty->m_oPgBorders->m_oBottom.IsInit() )
		{
			OOXBorderReader oBorderReader(dynamic_cast<ComplexTypes::Word::CPageBorder*>(m_ooxSectionProperty->m_oPgBorders->m_oBottom.GetPointer()));
			oBorderReader.Parse( oParam, oOutput.m_oBorderBottom );
		}
	}
	for (long i =0 ; i < m_ooxSectionProperty->m_arrHeaderReference.size() > 0; i++ )
	{
		OOXHeaderReader oHeaderReader(m_ooxSectionProperty->m_arrHeaderReference[i]);
		
		if(m_ooxSectionProperty->m_arrHeaderReference[i]->m_oType.IsInit() &&
			m_ooxSectionProperty->m_arrHeaderReference[i]->m_oId.IsInit() )
		{
			CString sID = m_ooxSectionProperty->m_arrHeaderReference[i]->m_oId->GetValue();
            switch(m_ooxSectionProperty->m_arrHeaderReference[i]->m_oType->GetValue())
			{
			case SimpleTypes::hdrftrDefault : 
				{
					if( sID != oOutput.m_sIDHeaderRight )
					{
						oHeaderReader.Parse( oParam, sID, oOutput.m_oHeaderRight, true );	
						oOutput.m_sIDHeaderRight = sID;
					}
				}break;
			case SimpleTypes::hdrftrEven    : 
				{
					if( sID != oOutput.m_sIDHeaderLeft )
					{
						oHeaderReader.Parse( oParam, sID, oOutput.m_oHeaderLeft, true );	
						oOutput.m_sIDHeaderLeft = sID;
					}
				}break;
			case SimpleTypes::hdrftrFirst   : 
				{
					if( sID != oOutput.m_sIDHeaderFirst )
					{
						oHeaderReader.Parse( oParam, sID, oOutput.m_oHeaderFirst, true );	
						oOutput.m_sIDHeaderFirst = sID;
					}
				}break;
			}
		}
	}
	for (long i =0 ; i < m_ooxSectionProperty->m_arrFooterReference.size() > 0; i++ )
	{
		OOXHeaderReader oFooterReader(m_ooxSectionProperty->m_arrFooterReference[i]);
		
		if(m_ooxSectionProperty->m_arrFooterReference[i]->m_oType.IsInit() &&
			m_ooxSectionProperty->m_arrFooterReference[i]->m_oId.IsInit() )
		{
			CString sID = m_ooxSectionProperty->m_arrFooterReference[i]->m_oId->GetValue();
            switch(m_ooxSectionProperty->m_arrFooterReference[i]->m_oType->GetValue())
			{
			case SimpleTypes::hdrftrDefault : 
				{
					if( sID != oOutput.m_sIDFooterRight )
					{
						oFooterReader.Parse( oParam, sID, oOutput.m_oFooterRight, false );	
						oOutput.m_sIDFooterRight = sID;
					}
				}break;
			case SimpleTypes::hdrftrEven    : 
				{
					if( sID != oOutput.m_sIDFooterLeft )
					{
						oFooterReader.Parse( oParam, sID, oOutput.m_oFooterLeft, false );	
						oOutput.m_sIDFooterLeft = sID;
					}
				}break;
			case SimpleTypes::hdrftrFirst   : 
				{
					if( sID != oOutput.m_sIDFooterFirst )
					{
						oFooterReader.Parse( oParam, sID, oOutput.m_oFooterFirst, false );	
						oOutput.m_sIDFooterFirst = sID;
					}
				}break;
			}
		}
	}

	return true;
}

