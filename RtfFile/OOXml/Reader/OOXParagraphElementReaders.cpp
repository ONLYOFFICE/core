/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "OOXTableReader.h"
#include "OOXParagraphReader.h"

#include "OOXTextItemReader.h"
#include "OOXpPrFrameReader.h"
#include "OOXpPrTabReader.h"
#include "OOXMathReader.h"

#include "../../Format/RtfOle.h"

#include "../../../OOXML/DocxFormat/Comments.h"

#include "../../../OOXML/DocxFormat/Math/OMath.h"
#include "../../../OOXML/DocxFormat/Math/oMathPara.h"

#include "../../../OOXML/DocxFormat/Logic/AlternateContent.h"
#include "../../../OOXML/DocxFormat/Logic/Annotations.h"
#include "../../../OOXML/DocxFormat/Logic/Pict.h"
#include "../../../OOXML/DocxFormat/Logic/Sdt.h"
#include "../../../OOXML/DocxFormat/Logic/Table.h"
#include "../../../OOXML/DocxFormat/Logic/Hyperlink.h"
#include "../../../OOXML/DocxFormat/Logic/Paragraph.h"
#include "../../../OOXML/DocxFormat/Logic/ParagraphProperty.h"
#include "../../../OOXML/DocxFormat/Logic/Dir.h"
#include "../../../OOXML/DocxFormat/Logic/FldSimple.h"
#include "../../../OOXML/DocxFormat/Logic/SmartTag.h"
#include "../../../OOXML/DocxFormat/Logic/Run.h"
#include "../../../OOXML/DocxFormat/Logic/RunProperty.h"

OOXParagraphReader::OOXParagraphReader (OOX::Logic::CParagraph *ooxParagraph)
{
    m_ooxElement		= NULL;
    m_ooxParagraph		= ooxParagraph;
    m_drawingParagraph	= NULL;

    m_oCharProperty.SetDefault();
}
OOXParagraphReader::OOXParagraphReader (PPTX::Logic::Paragraph *ooxParagraph)
{
    m_ooxElement		= NULL;
    m_ooxParagraph		= NULL;
    m_drawingParagraph	= ooxParagraph;

    m_oCharProperty.SetDefault();
}
OOXParagraphReader::OOXParagraphReader (OOX::WritingElementWithChilds<OOX::WritingElement> *ooxElement)
{
    m_drawingParagraph	= NULL;
    m_ooxParagraph		= NULL;
    m_ooxElement		= ooxElement;
}
bool OOXParagraphReader::Parse( ReaderParameter oParam , RtfParagraph& oOutputParagraph, CcnfStyle oConditionalTableStyle )
{
	if (m_drawingParagraph)
	{
		if (m_drawingParagraph->pPr.IsInit())
		{
			OOXpPrReader opPrReader(m_drawingParagraph->pPr.GetPointer());
			
			opPrReader.Parse( oParam, oOutputParagraph.m_oProperty, oConditionalTableStyle);
		}
		RtfStylePtr poStyle;
		for (size_t i = 0; i < m_drawingParagraph->RunElems.size(); ++i)
		{
			NSCommon::smart_ptr<PPTX::Logic::RunBase> run = m_drawingParagraph->RunElems[i].GetElem();
			Parse3(oParam , oOutputParagraph, oConditionalTableStyle, poStyle, dynamic_cast<OOX::WritingElement*>(run.operator ->()));
		}
		

		return true;		
	}	
	if (m_ooxParagraph == NULL) return false;

	//надо default стиль применять до OOXParagraphReader
	////применяем default
	//oOutputParagraph.m_oProperty = oParam.oRtf->m_oDefaultParagraphProp; 

	RtfTableStylePtr oResultTableStyle;
	//сначала применяем default стиль (если это нужно)

	oOutputParagraph.m_oProperty.m_oCharProperty = m_oCharProperty;

	if (m_ooxParagraph->m_oParagraphProperty)
	{
		OOXpPrReader opPrReader(m_ooxParagraph->m_oParagraphProperty);
		
		opPrReader.Parse( oParam, oOutputParagraph.m_oProperty, oConditionalTableStyle);
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

	//m_oCharProperty = oOutputParagraph.m_oProperty.m_oCharProperty;

	m_ooxElement = dynamic_cast<OOX::WritingElementWithChilds<OOX::WritingElement>*>(m_ooxParagraph);

	bool res = Parse2(oParam, oOutputParagraph, oConditionalTableStyle, poExternalStyle );

	return res;
}
bool OOXParagraphReader::Parse2( ReaderParameter oParam , RtfParagraph& oOutputParagraph, CcnfStyle oConditionalTableStyle, RtfStylePtr poStyle )
{
	if (m_ooxElement == NULL) return false;
	
    for (std::vector<OOX::WritingElement*>::iterator it = m_ooxElement->m_arrItems.begin(); it != m_ooxElement->m_arrItems.end(); ++it)
	{
		Parse3(oParam , oOutputParagraph, oConditionalTableStyle, poStyle , *it);
	}
	return true;
}
bool OOXParagraphReader::Parse3( ReaderParameter oParam , RtfParagraph& oOutputParagraph, CcnfStyle oConditionalTableStyle, RtfStylePtr poStyle, OOX::WritingElement* m_ooxElement)
{	
	if (m_ooxElement == NULL) return false;

	RtfStylePtr poExternalStyle;
	switch (m_ooxElement->getType())
	{
		case OOX::et_w_tbl:
		{
			oParam.oReader->m_nCurItap ++ ;
			RtfTablePtr oNewTable ( new RtfTable() );					
			OOX::Logic::CTbl * pTbl = dynamic_cast<OOX::Logic::CTbl*>(m_ooxElement);

			OOXTableReader oTableReader(pTbl);
			//oNewTable->m_oCharProperty = oOutputParagraph.m_oProperty.m_oCharProperty;
			//Merge(oOutputParagraph.m_oProperty.m_oCharProperty)

			oTableReader.Parse( oParam, *oNewTable);
			oOutputParagraph.AddItem( oNewTable );
			oParam.oReader->m_nCurItap -- ;				
		}break;
		case OOX::et_w_ins:
		{
			OOX::Logic::CIns * pIns = dynamic_cast<OOX::Logic::CIns*>(m_ooxElement);

			OOXParagraphReader oSubParReader(pIns);	
			oSubParReader.m_oCharProperty = m_oCharProperty;
			oSubParReader.m_oCharProperty.m_nRevised = 1;
			
			if (pIns->m_sAuthor.IsInit())
				oSubParReader.m_oCharProperty.m_nRevauth = oParam.oRtf->m_oRevisionTable.AddAuthor( pIns->m_sAuthor.get2() ) + 1;
			
			if (pIns->m_oDate.IsInit())
            {
                std::wstring sVal = pIns->m_oDate->GetValue();
                oSubParReader.m_oCharProperty.m_nRevdttm = RtfUtility::convertDateTime( sVal );
            }

			oSubParReader.Parse2( oParam, oOutputParagraph, oConditionalTableStyle, poStyle);
		}break;
		case OOX::et_w_del:
		{
			OOX::Logic::CDel * pDel = dynamic_cast<OOX::Logic::CDel*>(m_ooxElement);

			OOXParagraphReader oSubParReader(pDel);					
			oSubParReader.m_oCharProperty = m_oCharProperty;
			oSubParReader.m_oCharProperty.m_nDeleted = 1;

			if (pDel->m_sAuthor.IsInit())
				oSubParReader.m_oCharProperty.m_nRevauthDel = oParam.oRtf->m_oRevisionTable.AddAuthor( pDel->m_sAuthor.get2() ) + 1;
			
			if (pDel->m_oDate.IsInit())
            {
                std::wstring sVal = pDel->m_oDate->GetValue();
                oSubParReader.m_oCharProperty.m_nRevdttmDel = RtfUtility::convertDateTime( sVal );
            }
			
			oSubParReader.Parse2( oParam, oOutputParagraph, oConditionalTableStyle, poStyle);
		}break;
		case OOX::et_a_r:
		{
			PPTX::Logic::Run * pRun = dynamic_cast<PPTX::Logic::Run*>(m_ooxElement);
			
			OOXRunReader oRunReader(pRun);				
			oRunReader.Parse ( oParam, oOutputParagraph, poExternalStyle );
		}break;
		case OOX::et_w_r:
		{
			OOX::Logic::CRun * pRun = dynamic_cast<OOX::Logic::CRun*>(m_ooxElement);
			
			OOXRunReader oRunReader(pRun);
			oRunReader.m_oCharProperty = m_oCharProperty;
			
			oRunReader.Parse ( oParam, oOutputParagraph, poExternalStyle );
		}break;
		case OOX::et_w_fldSimple:
		{
			OOX::Logic::CFldSimple * pFldSimple = dynamic_cast<OOX::Logic::CFldSimple*>(m_ooxElement);
		
			RtfFieldPtr oCurField ( new RtfField() );
			
			oCurField->m_pInsert = RtfFieldInstPtr		( new RtfFieldInst() );
			oCurField->m_pResult = RtfFieldInstPtr		( new RtfFieldInst() );
		//добавляем insert
			RtfCharPtr pNewChar ( new RtfChar() );
			pNewChar->m_bRtfEncode = false;
			if (pFldSimple->m_sInstr.IsInit())
			{
				pNewChar->setText( pFldSimple->m_sInstr.get2() );
			}
			RtfParagraphPtr oNewInsertParagraph ( new RtfParagraph() );
			oNewInsertParagraph->AddItem( pNewChar );
			oCurField->m_pInsert->m_pTextItems->AddItem( oNewInsertParagraph );
			
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
				oSubParReader.m_oCharProperty = m_oCharProperty;
				
				oSubParReader.Parse2( oParam, *oNewResultParagraph, CcnfStyle(), poExternalStyle);
				oCurField->m_pResult->m_pTextItems->AddItem( oNewResultParagraph 	);			
			}
			oOutputParagraph.AddItem( oCurField );
		}break;
		case OOX::et_w_hyperlink:
		{
			OOX::Logic::CHyperlink * pHyperlink = dynamic_cast<OOX::Logic::CHyperlink*>(m_ooxElement);

			if( pHyperlink->m_oId.IsInit() )
			{
				std::wstring sTarget, sLocation;
				
				if (oParam.oReader->m_currentContainer)
				{ 
					smart_ptr<OOX::File> oFile = oParam.oReader->m_currentContainer->Find(pHyperlink->m_oId->GetValue());
					if ((oFile.IsInit()) && (OOX::FileTypes::HyperLink == oFile->type()))
					{
						OOX::HyperLink* pH = (OOX::HyperLink*)oFile.GetPointer();
						sTarget = pH->Uri().GetPath();
					}
				}

				if( !sTarget.empty() )
				{
					if (pHyperlink->m_sAnchor.IsInit())
					{
						sTarget += L"#" + *pHyperlink->m_sAnchor;
					}
					//заменяем пробелы на %20
                    XmlUtils::replace_all(sTarget, L" ", L"%20" );

                    std::wstring sFileUrl = L"file:///";

                    if( 0 == sTarget.find( sFileUrl ) )
					{
                        size_t nFirstDDot = sTarget.find( ':', sFileUrl.length() );
						size_t nLen = sTarget.length();
						if( std::wstring::npos != nFirstDDot && nFirstDDot + 2 < nLen && '\\' == sTarget[nFirstDDot+1] )
						{
							if( '\\' != sTarget[nFirstDDot+2] ) 
                                sTarget.insert( sTarget.begin() + nFirstDDot + 1, '\\'  );
						}
					}
					RtfFieldPtr oCurField( new RtfField() );
					
					oCurField->m_pInsert = RtfFieldInstPtr ( new RtfFieldInst() );
					oCurField->m_pResult = RtfFieldInstPtr ( new RtfFieldInst() );
				//добавляем insert
					RtfCharPtr pNewChar( new RtfChar() );
					pNewChar->m_bRtfEncode = true;// false;
					std::wstring sFieldText;
                    sFieldText += L"HYPERLINK \"" + sTarget + L"\"";
					pNewChar->setText( sFieldText );
					
					RtfParagraphPtr oNewInsertParagraph( new RtfParagraph() );
					oNewInsertParagraph->AddItem( pNewChar );
					oCurField->m_pInsert->m_pTextItems->AddItem( oNewInsertParagraph );
					//добавляем свойства

					//pHyperlink->m_arrItems todoooo 
					//BOOL bLock = Strings::ToBoolean(oXmlReader.ReadNodeAttribute(i, L"w:fldLock", L"false)));
					//if( TRUE == bLock )
					//	oCurField->m_eMode = RtfField::fm_fldlock;
					//BOOL bDirty = Strings::ToBoolean(oXmlReader.ReadNodeAttribute(i, L"w:dirty", L"false"));
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
						oSubParReader.m_oCharProperty = m_oCharProperty;

						oSubParReader.Parse2( oParam, *oNewResultParagraph, CcnfStyle(), poExternalStyle);
						oCurField->m_pResult->m_pTextItems->AddItem( oNewResultParagraph );
					}
					oOutputParagraph.AddItem( oCurField );
				}
			}
			else if( pHyperlink->m_sAnchor.IsInit() )
			{
				RtfFieldPtr oCurField( new RtfField() );
				
				oCurField->m_pInsert = RtfFieldInstPtr ( new RtfFieldInst() );
				oCurField->m_pResult = RtfFieldInstPtr ( new RtfFieldInst() );
				//добавляем insert
				RtfCharPtr pNewCharHYPER ( new RtfChar() );
				pNewCharHYPER->m_bRtfEncode = false;
				pNewCharHYPER->setText( L"HYPERLINK \\l \"" + pHyperlink->m_sAnchor.get() +L"\"");

				RtfParagraphPtr oNewInsertParagraph ( new RtfParagraph() );
				oNewInsertParagraph->AddItem( pNewCharHYPER );

				oCurField->m_pInsert->m_pTextItems->AddItem( oNewInsertParagraph );
				////добаляем свойства
				//BOOL bLock = Strings::ToBoolean(oXmlReader.ReadNodeAttribute(i, L"w:fldLock" ,L"false"));
				//if( TRUE == bLock )
				//	oCurField->m_eMode = RtfField::fm_fldlock;
				//BOOL bDirty = Strings::ToBoolean(oXmlReader.ReadNodeAttribute(i, L"w:dirty", L"false"));
				//if( TRUE == bDirty )
				//	oCurField->m_eMode = RtfField::fm_flddirty;

				RtfParagraphPtr oNewResultParagraph ( new RtfParagraph() );
				//применяем к новому параграфу default property
				oNewResultParagraph->m_oProperty = oParam.oRtf->m_oDefaultParagraphProp;
				oNewResultParagraph->m_oProperty.m_oCharProperty = oParam.oRtf->m_oDefaultCharProp;
				//применяем к новому параграфу свойства данного параграфа
				oNewResultParagraph->m_oProperty = oOutputParagraph.m_oProperty;
				
				if (pHyperlink->m_arrItems.size() > 0)
				{
					OOXParagraphReader oSubParReader(pHyperlink);					
					oSubParReader.m_oCharProperty = m_oCharProperty;
					oSubParReader.m_oCharProperty.Merge(oOutputParagraph.m_oProperty.m_oCharProperty);
					
					oSubParReader.Parse2( oParam, *oNewResultParagraph, CcnfStyle(), poExternalStyle);
					oCurField->m_pResult->m_pTextItems->AddItem( oNewResultParagraph );
				}
				oOutputParagraph.AddItem( oCurField );
			}
		}break;
		case OOX::et_w_bookmarkStart:
		{
			OOX::Logic::CBookmarkStart * pBookmarkStart = dynamic_cast<OOX::Logic::CBookmarkStart*>(m_ooxElement);
			RtfBookmarkStartPtr oNewBookmark ( new RtfBookmarkStart() );
			
			oNewBookmark->m_sName = pBookmarkStart->m_sName.IsInit() ? pBookmarkStart->m_sName.get2() : L"";

			if (pBookmarkStart->m_oColFirst.IsInit())
				oNewBookmark->nFirstColumn = pBookmarkStart->m_oColFirst->GetValue();
			if (pBookmarkStart->m_oColLast.IsInit())
				oNewBookmark->nLastColumn = pBookmarkStart->m_oColLast->GetValue();
			
			if(pBookmarkStart->m_oId.IsInit())
			{
				int nId = pBookmarkStart->m_oId->GetValue();
				oParam.oReader->m_aBookmarks.insert(std::pair<int, std::wstring>( nId, oNewBookmark->m_sName ));
				oOutputParagraph.AddItem( oNewBookmark );
			}
		}break;
		case OOX::et_w_bookmarkEnd:
		{
			OOX::Logic::CBookmarkEnd * pBookmarkEnd = dynamic_cast<OOX::Logic::CBookmarkEnd*>(m_ooxElement);

			RtfBookmarkEndPtr oNewBookmark ( new RtfBookmarkEnd() );
			//oNewBookmark->m_sName = pBookmarkEnd->;

			int nId = pBookmarkEnd->m_oId->GetValue();
			std::map<int, std::wstring>::iterator pPair = oParam.oReader->m_aBookmarks.find( nId );
			if( pPair !=  oParam.oReader->m_aBookmarks.end())
			{
				oNewBookmark->m_sName = pPair->second;
				oOutputParagraph.AddItem( oNewBookmark );
			}
		}break;
		case OOX::et_w_smartTag:
		{
			OOX::Logic::CSmartTag * pSmartTag = dynamic_cast<OOX::Logic::CSmartTag*>(m_ooxElement);

            for (size_t i = 0; i < pSmartTag->m_arrItems.size(); ++i)
			{
				OOX::Logic::CRun * pRun = dynamic_cast<OOX::Logic::CRun*>(pSmartTag->m_arrItems[i]);
				if (pRun == NULL) continue;

				OOXRunReader oRunReader(pRun);
				oRunReader.m_oCharProperty = m_oCharProperty;
				
				oRunReader.Parse( oParam, oOutputParagraph, poExternalStyle );
			}		
		}break;
		case OOX::et_m_oMath:
		{
			OOX::Logic::COMath * pMath = dynamic_cast<OOX::Logic::COMath*>(m_ooxElement);
			
			RtfMathPtr oNewMath ( new RtfMath() );
			oNewMath->SetOOXType( OOX::et_m_oMath );
		
			OOXMathReader oMathReader(pMath);
			oMathReader.m_oCharProperty = m_oCharProperty;
			oMathReader.m_oCharProperty.Merge(oOutputParagraph.m_oProperty.m_oCharProperty);

			if(true == oMathReader.Parse( oParam, (*oNewMath) ) )
				oOutputParagraph.AddItem( oNewMath );
		}break;
		case OOX::et_m_oMathPara:
		{
			OOX::Logic::COMathPara * pMathPara = dynamic_cast<OOX::Logic::COMathPara*>(m_ooxElement);
			
			RtfMathPtr oNewMath ( new RtfMath() );				
			oNewMath->SetOOXType( OOX::et_m_oMathPara );
			
			OOXMathReader oMathReader(pMathPara);
			oMathReader.m_oCharProperty = m_oCharProperty;
			oMathReader.m_oCharProperty.Merge(oOutputParagraph.m_oProperty.m_oCharProperty);

			if(true == oMathReader.Parse( oParam, (*oNewMath) ) )
				oOutputParagraph.AddItem( oNewMath );
		}break;
		case OOX::et_w_sdt:
		{
			OOX::Logic::CSdt * pSdt = dynamic_cast<OOX::Logic::CSdt*>(m_ooxElement);
			if( pSdt->m_oSdtEndPr.IsInit() )
			{
				//todo
			}
			if(pSdt->m_oSdtContent.IsInit())
			{
				if (pSdt->m_oSdtContent->m_arrItems.size() > 0)
				{
					OOXParagraphReader oSubParReader(pSdt->m_oSdtContent.GetPointer());
					oSubParReader.m_oCharProperty = m_oCharProperty;

					oSubParReader.Parse2( oParam, oOutputParagraph, CcnfStyle(), poExternalStyle );	
				}
			}
		}break;
		case OOX::et_w_commentRangeStart:
		case OOX::et_w_commentReference:
		{
			OOX::Logic::CCommentRangeStart * pCommentStart = dynamic_cast<OOX::Logic::CCommentRangeStart*>(m_ooxElement);
						
			if(pCommentStart->m_oId.IsInit())
			{
				int nId = pCommentStart->m_oId->GetValue();				
				std::map<int, OOXReader::_comment>::iterator pFind = oParam.oReader->m_mapComments.find( nId );

				if( pFind ==  oParam.oReader->m_mapComments.end())
				{
					RtfAnnotElemPtr oNewAnnotElem ( new RtfAnnotElem(1) );
					oNewAnnotElem->m_sValue = std::to_wstring(0x7700000 + nId);

					OOXReader::_comment comment;
					comment.ref		= oNewAnnotElem->m_sValue;
					comment.index	= (int)oParam.oReader->m_mapComments.size();

					oParam.oReader->m_mapComments.insert(std::make_pair( nId, comment));
					oOutputParagraph.AddItem( oNewAnnotElem );
				}
			}
		}break;
		case OOX::et_w_commentRangeEnd:
		{
			OOX::Logic::CCommentRangeEnd * pCommentEnd = dynamic_cast<OOX::Logic::CCommentRangeEnd*>(m_ooxElement);

			int nId = pCommentEnd->m_oId->GetValue();
			
			std::map<int, OOXReader::_comment>::iterator pFindRef = oParam.oReader->m_mapComments.find( nId );
			
			if( pFindRef != oParam.oReader->m_mapComments.end())
			{
				RtfAnnotElemPtr oNewAnnotElem ( new RtfAnnotElem(2) );
				oNewAnnotElem->m_sValue = pFindRef->second.ref;
				oOutputParagraph.AddItem( oNewAnnotElem );
			
				//find comment and add info
				std::map<int, int>::iterator pFindComment = oParam.oDocx->m_oMain.comments->m_mapComments.find(nId);

				if (pFindComment != oParam.oDocx->m_oMain.comments->m_mapComments.end())
				{
					if ( pFindComment->second < (int)oParam.oDocx->m_oMain.comments->m_arrComments.size() && pFindComment->second >= 0)
					{
						OOX::CComment* oox_comment = oParam.oDocx->m_oMain.comments->m_arrComments[pFindComment->second];
						if (oox_comment)
						{
							if (oox_comment->m_oAuthor.IsInit())
							{
								RtfAnnotElemPtr oNewAnnotAuthor ( new RtfAnnotElem(4) );
								oNewAnnotAuthor->m_sValue = *oox_comment->m_oAuthor;
								oOutputParagraph.AddItem( oNewAnnotAuthor );
							}
							if (oox_comment->m_oInitials.IsInit())
							{
								RtfAnnotElemPtr oNewAnnotAuthorId ( new RtfAnnotElem(5) );
								oNewAnnotAuthorId->m_sValue = *oox_comment->m_oInitials;
								oOutputParagraph.AddItem( oNewAnnotAuthorId );
							}

							RtfAnnotationPtr oNewAnnotContent(new RtfAnnotation());
							
							oNewAnnotContent->m_oRef = RtfAnnotElemPtr ( new RtfAnnotElem(3) );
							oNewAnnotContent->m_oRef->m_sValue = pFindRef->second.ref;
							
							if (oox_comment->m_oDate.IsInit())
							{
								oNewAnnotContent->m_oDate = RtfAnnotElemPtr ( new RtfAnnotElem(6) );

								int nDate = RtfUtility::convertDateTime(oox_comment->m_oDate->GetValue());
								oNewAnnotContent->m_oDate->m_sValue =  std::to_wstring(nDate);
							}

							OOXTextItemReader oTextItemReader;
							oTextItemReader.m_oTextItems = oNewAnnotContent->m_oContent;
							
							for (size_t i = 0; i < oox_comment->m_arrItems.size(); ++i)
							{
								if (oParam.oDocx->m_pCommentsExt)
								{
									OOX::Logic::CParagraph *pParagraph = dynamic_cast<OOX::Logic::CParagraph*>(oox_comment->m_arrItems[i]);
									if ((pParagraph) && (pParagraph->m_oParaId.IsInit()))
									{
										std::map<unsigned int, int>::iterator pFindPara = oParam.oDocx->m_pCommentsExt->m_mapComments.find(pParagraph->m_oParaId->GetValue());
										if (pFindPara != oParam.oDocx->m_pCommentsExt->m_mapComments.end())
										{
											oParam.oReader->m_mapCommentsPara.insert(std::make_pair( pParagraph->m_oParaId->GetValue(), pFindRef->second.index));

											if (oParam.oDocx->m_pCommentsExt->m_arrComments[pFindPara->second]->m_oParaIdParent.IsInit())
											{
												std::map<int, int>::iterator pFindParent = oParam.oReader->m_mapCommentsPara.find(oParam.oDocx->m_pCommentsExt->m_arrComments[pFindPara->second]->m_oParaIdParent->GetValue());
												if (pFindParent != oParam.oReader->m_mapCommentsPara.end())
												{
													oNewAnnotContent->m_oParent = RtfAnnotElemPtr ( new RtfAnnotElem(7) );
													oNewAnnotContent->m_oParent->m_sValue =  std::to_wstring( pFindParent->second - pFindRef->second.index);
												}
											}
										}
									}
								}
								oTextItemReader.Parse(oox_comment->m_arrItems[i], oParam);
							}							

							oOutputParagraph.AddItem( oNewAnnotContent );
						}
					}
				}
			}

		}break;
        default:
            break;
    }
	return true;
}


OOXRunReader::OOXRunReader(OOX::Logic::CRun *ooxRun)
{
    m_drawingRun	= NULL;
    m_ooxRun		= ooxRun;
    m_oCharProperty.SetDefault();
}
OOXRunReader::OOXRunReader(PPTX::Logic::Run *ooxRun)
{
    m_drawingRun	= ooxRun;
    m_ooxRun		= NULL;
    m_oCharProperty.SetDefault();
}
bool OOXRunReader::Parse( ReaderParameter oParam , RtfParagraph& oOutputParagraph, RtfStylePtr poStyle, RtfCharProperty& oNewProperty, OOX::WritingElement* ooxItem )
{
	if (!ooxItem) return false;

	switch(ooxItem->getType())
	{
		case OOX::et_w_t:
		{
			OOX::Logic::CText * ooxText = dynamic_cast<OOX::Logic::CText*>(ooxItem);
			if (ooxText)
			{
				std::wstring sValue;
				//if ((ooxText->m_oSpace.IsInit()) && (ooxText->m_oSpace->GetValue() == SimpleTypes::xmlspacePreserve))
				//{
				//	sValue = RtfUtility::Preserve(ooxText->m_sText);
				//}
				//else
					sValue = ooxText->m_sText;
			
				RtfCharPtr pNewChar ( new RtfChar() );
				
				pNewChar->m_oProperty = oNewProperty;
				pNewChar->setText( sValue );
				
				oOutputParagraph.AddItem( pNewChar );		
			}
		}break;
		case OOX::et_w_delText:
		{
			OOX::Logic::CDelText * ooxText = dynamic_cast<OOX::Logic::CDelText*>(ooxItem);
			if (ooxText)
			{
				std::wstring sValue;
				//if ((ooxText->m_oSpace.IsInit()) && (ooxText->m_oSpace->GetValue() == SimpleTypes::xmlspacePreserve))
				//{
				//	sValue = RtfUtility::Preserve(ooxText->m_sText);
				//}
				//else
					sValue = ooxText->m_sText;
			
				RtfCharPtr pNewChar ( new RtfChar() );
				
				pNewChar->m_oProperty = oNewProperty;
				pNewChar->setText( sValue );
				
				oOutputParagraph.AddItem( pNewChar );		
			}
		}break;
		case OOX::et_w_fldChar:
		{
			OOX::Logic::CFldChar * ooxFldChar = dynamic_cast<OOX::Logic::CFldChar*>(ooxItem);
			if ((ooxFldChar) && (ooxFldChar->m_oFldCharType.IsInit()))
			{
				switch(ooxFldChar->m_oFldCharType->GetValue())
				{
				case SimpleTypes::fldchartypeBegin:
					{
						OOXFieldBeginPtr oNewField ( new OOXFieldBegin() );
						if (ooxFldChar->m_oFldLock.IsInit())
							oNewField->m_bLock = ooxFldChar->m_oFldLock->ToBool();
						if (ooxFldChar->m_oDirty.IsInit())
							oNewField->m_bDirty = ooxFldChar->m_oDirty->ToBool();
						
						oNewField->m_oCharProperty = oNewProperty;
						oOutputParagraph.AddItem( oNewField );
					}break;
				case SimpleTypes::fldchartypeEnd: 
					{
						OOXFieldEndPtr oNewField ( new OOXFieldEnd() );
						oOutputParagraph.AddItem( oNewField );
					}break;
				case SimpleTypes::fldchartypeSeparate:
					{
						OOXFieldSeparatePtr oNewField ( new OOXFieldSeparate() );
						oOutputParagraph.AddItem( oNewField );
					}break;
                default: break;
                }
			}
		}break;
		case OOX::et_w_instrText:
		{
			OOX::Logic::CInstrText * ooxInstrText = dynamic_cast<OOX::Logic::CInstrText*>(ooxItem);
			OOXFieldInsertTextPtr oNewField ( new OOXFieldInsertText() );
			
			RtfCharPtr pNewChar ( new RtfChar() );
			pNewChar->m_oProperty	= oNewProperty;
			
			if (ooxInstrText)
				pNewChar->setText( ooxInstrText->m_sText );//add preserve .. todooo
			
			oNewField->m_oText		= pNewChar;
			oOutputParagraph.AddItem( oNewField );
		}break;
		case OOX::et_w_footnoteReference:
		{
			OOX::Logic::CFootnoteReference * ooxFootnoteReference = dynamic_cast<OOX::Logic::CFootnoteReference*>(ooxItem);
			if ((ooxFootnoteReference) && (ooxFootnoteReference->m_oId.IsInit()))
			{
				int nID = ooxFootnoteReference->m_oId->GetValue();
				std::map<int, TextItemContainerPtr>::iterator oPair = oParam.oReader->m_mapFootnotes.find( nID );
				
				if( oParam.oReader->m_mapFootnotes.end() != oPair )
				{
					RtfFootnotePtr oNewFootnote ( new RtfFootnote() );
					oNewFootnote->m_oCharProp	= oNewProperty;
					oNewFootnote->m_oContent	= oPair->second;
					
					oOutputParagraph.AddItem( oNewFootnote );
				}
			}
		}break;
		case OOX::et_w_endnoteReference:
		{
			OOX::Logic::CEndnoteReference * ooxEndnoteReference = dynamic_cast<OOX::Logic::CEndnoteReference*>(ooxItem);
			if ((ooxEndnoteReference) && (ooxEndnoteReference->m_oId.IsInit()))
			{
				int nID = ooxEndnoteReference->m_oId->GetValue();
				std::map<int, TextItemContainerPtr>::iterator oPair = oParam.oReader->m_mapEndnotes.find ( nID );
				
				if( oParam.oReader->m_mapEndnotes.end() != oPair )
				{
					RtfFootnotePtr oNewEndnote ( new RtfFootnote() );
					oNewEndnote->m_oCharProp	= oNewProperty;
					oNewEndnote->m_oContent		= oPair->second;
					oNewEndnote->m_bEndNote		= true;
					
					oOutputParagraph.AddItem( oNewEndnote );
				}
			}
		}break;
		case OOX::et_w_object:
		{
			OOX::Logic::CObject * ooxObject = dynamic_cast<OOX::Logic::CObject*>(ooxItem);

			RtfShapePtr pNewShape ( new RtfShape() );
			if (ooxObject)
			{//важна последовательность обработки
				OOXShapeReader oShapeReaderType(ooxObject->m_oShapeType.GetPointer());
				oShapeReaderType.Parse( oParam, pNewShape );
				
				OOXShapeReader oShapeReaderShape(ooxObject->m_oShape.GetPointer());
				oShapeReaderShape.Parse( oParam, pNewShape );
				
				for (size_t i = 0; i < ooxObject->m_arrItems.size(); ++i)
				{
					OOXShapeReader oShapeReader(ooxObject->m_arrItems[i]);
					oShapeReader.Parse( oParam, pNewShape );
				}			

				OOXShapeReader oShapeReaderControl(ooxObject->m_oControl.GetPointer());
				oShapeReaderControl.Parse( oParam, pNewShape );
				
				OOXShapeReader oShapeReaderObject(ooxObject->m_oOleObject.GetPointer());
				oShapeReaderObject.Parse( oParam, pNewShape );
			}
				
			if (pNewShape->m_bIsOle && pNewShape->m_pOleObject)
			{
				if (ooxObject->m_oDxaOrig.IsInit())	pNewShape->m_pOleObject->m_nWidth = *ooxObject->m_oDxaOrig;
				if (ooxObject->m_oDyaOrig.IsInit())	pNewShape->m_pOleObject->m_nHeight = *ooxObject->m_oDyaOrig;

				pNewShape->m_pOleObject->m_oResultShape = pNewShape;				
 				
				oOutputParagraph.AddItem( pNewShape->m_pOleObject );
			}
			else
				oOutputParagraph.AddItem( pNewShape );
		}break;
		case OOX::et_w_drawing:
		{
			bool bAddDrawing = false;

			OOX::Logic::CDrawing* ooxDrawing = dynamic_cast<OOX::Logic::CDrawing*>(ooxItem);
			
			RtfShapePtr pNewDrawing ( new RtfShape() );
			pNewDrawing->m_oCharProperty = oNewProperty;
		
			OOXDrawingReader oDrawingReader(ooxDrawing);
			int result = oDrawingReader.Parse( oParam, pNewDrawing );

			if (result == 1)
			{
				if (pNewDrawing->m_bIsOle && pNewDrawing->m_pOleObject)
				{
					pNewDrawing->m_pOleObject->m_oResultShape = pNewDrawing;

					oOutputParagraph.AddItem( pNewDrawing->m_pOleObject );
				}
				else
					oOutputParagraph.AddItem( pNewDrawing );
				bAddDrawing = true;
			}
			if (!bAddDrawing)
			{
				pNewDrawing->SetNotSupportShape();
				oOutputParagraph.AddItem( pNewDrawing );
			}
		}break;
		case OOX::et_w_pict:
		{
			OOX::Logic::CPicture *ooxPicture = dynamic_cast<OOX::Logic::CPicture*>(ooxItem);
			if( ooxPicture) 
			{
				OOX::Vml::CGroup* pGroup = dynamic_cast<OOX::Vml::CGroup*>(ooxPicture->m_oShapeElement.GetPointer());
				if (pGroup)
				{
					RtfShapePtr pNewShape ( new RtfShape() );
					pNewShape->m_oCharProperty = oNewProperty;
					
					OOXShapeGroupReader oShapeGroupReader(pGroup);
					if( true == oShapeGroupReader.Parse( oParam, pNewShape ) )
						 oOutputParagraph.AddItem( pNewShape );
				}		
				else 
				{
					RtfShapePtr pNewShape ( new RtfShape() );
					pNewShape->m_oCharProperty = oNewProperty;
					
					OOXShapeReader* pShapeReader = NULL;
					
					if (ooxPicture->m_oShapeType.IsInit())
					{	
						pShapeReader = new OOXShapeReader(ooxPicture->m_oShapeType.GetPointer());
						if(pShapeReader)
						{
							pShapeReader->Parse( oParam, pNewShape );
							delete pShapeReader; pShapeReader = NULL;
						}
					}					
					if (ooxPicture->m_oShapeElement.IsInit())
						pShapeReader = new OOXShapeReader(dynamic_cast<OOX::Vml::CVmlCommonElements*>(ooxPicture->m_oShapeElement.GetPointer()));
					
					if (pShapeReader)
					{
						if( true == pShapeReader->Parse( oParam, pNewShape ) )
							 oOutputParagraph.AddItem( pNewShape );		
						delete pShapeReader;
					}
				}
			}
		}break;
		case OOX::et_w_nonBreakHyphen:
		{
			RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );
			pNewChar->m_oProperty	= oNewProperty;
			pNewChar->m_eType		= RtfCharSpecial::rsc_NonBrHyphen;
			
			oOutputParagraph.AddItem( pNewChar );
		}break;
		case OOX::et_w_softHyphen:
		{
			RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );
			pNewChar->m_oProperty	= oNewProperty;
			pNewChar->m_eType		= RtfCharSpecial::rsc_OptHyphen;
			
			oOutputParagraph.AddItem( pNewChar );
		}break;
		case OOX::et_w_pgNum:
		{
			RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );
			pNewChar->m_oProperty	= oNewProperty;
			pNewChar->m_eType		= RtfCharSpecial::rsc_chpgn;

			oOutputParagraph.AddItem( pNewChar );
		}break;
		case OOX::et_w_footnoteRef:
		{
			RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );
			pNewChar->m_oProperty	= oNewProperty;
			pNewChar->m_eType		= RtfCharSpecial::rsc_chftn;

			oOutputParagraph.AddItem( pNewChar );
		}break;
		case OOX::et_w_endnoteRef:
		{
			RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );
			pNewChar->m_oProperty	= oNewProperty;
			pNewChar->m_eType		= RtfCharSpecial::rsc_chftnEnd;

			oOutputParagraph.AddItem( pNewChar );
		}break;
		case OOX::et_w_separator:
		{
			RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );
			pNewChar->m_oProperty	= oNewProperty;
			pNewChar->m_eType		= RtfCharSpecial::rsc_chftnsep;

			oOutputParagraph.AddItem( pNewChar );
		}break;
		case OOX::et_w_continuationSeparator:
		{
			RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );
			pNewChar->m_oProperty	= oNewProperty;
			pNewChar->m_eType		= RtfCharSpecial::rsc_chftnsepc;

			oOutputParagraph.AddItem( pNewChar );
		}break;
		case OOX::et_w_tab:
		{
			RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );
			pNewChar->m_oProperty	= oNewProperty;
			pNewChar->m_eType		= RtfCharSpecial::rsc_tab;

			oOutputParagraph.AddItem( pNewChar );
		}break;
		case OOX::et_w_br:
		{
			OOX::Logic::CBr *ooxBr = dynamic_cast<OOX::Logic::CBr*>(ooxItem);
			
			RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );
			pNewChar->m_oProperty = oNewProperty;
			
			if (ooxBr)
			{
				switch(ooxBr->m_oType.GetValue())
				{
				case SimpleTypes::brtypeColumn       : 
					pNewChar->m_eType = RtfCharSpecial::rsc_column;	break;
				case SimpleTypes::brtypePage         : 
					pNewChar->m_eType = RtfCharSpecial::rsc_page;	break;
				case SimpleTypes::brtypeTextWrapping :
					switch(ooxBr->m_oClear.GetValue())
					{
						case SimpleTypes::brclearAll   : pNewChar->m_nTextWrapBreak = 3;	break;
						case SimpleTypes::brclearLeft  : pNewChar->m_nTextWrapBreak = 1;	break;
						case SimpleTypes::brclearRight : pNewChar->m_nTextWrapBreak = 2;	break;
						default:	pNewChar->m_eType = RtfCharSpecial::rsc_line;
					}
					break;
				}
			}else pNewChar->m_eType = RtfCharSpecial::rsc_line;							
			oOutputParagraph.AddItem( pNewChar );
		}break;
		case OOX::et_mc_alternateContent:
		{
			OOX::Logic::CAlternateContent *ooxAlt = dynamic_cast<OOX::Logic::CAlternateContent* >(ooxItem);
			if (!ooxAlt->m_arrChoiceItems.empty())
			{
				for (size_t i = 0; i < ooxAlt->m_arrChoiceItems.size(); i++)
				{
					Parse(oParam , oOutputParagraph, poStyle, oNewProperty, ooxAlt->m_arrChoiceItems[i]);
				}
			}
			else
			{
				for (size_t i = 0; i < ooxAlt->m_arrFallbackItems.size(); i++)
				{
					Parse(oParam , oOutputParagraph, poStyle, oNewProperty, ooxAlt->m_arrFallbackItems[i]);
				}					
			}
		}break;
		case OOX::et_w_sym:
		{
			OOX::Logic::CSym *ooxSym = dynamic_cast<OOX::Logic::CSym* >(ooxItem);

			if(ooxSym->m_oFont.IsInit() && ooxSym->m_oChar.IsInit() )
			{	
				std::wstring sFont = *ooxSym->m_oFont;
				//std::wstring sChar = ooxSym->m_oChar->GetValue();

				//sChar.MakeLower();
				////оставляем только 2 байта (4 символа)
				//if( sChar.length() > 4 )
				//	sChar = sChar.Right( 4 );
				////убираем маску F000
				//if( sChar.length() == 4 && 'f' == sChar[0] )
				//	sChar = sChar.Right( 3 );
				
				int nChar = ooxSym->m_oChar->GetValue();
                std::wstring sCharUnicode; sCharUnicode += nChar;
				RtfFont oCurFont;
				if( false == oParam.oRtf->m_oFontTable.GetFont( sFont, oCurFont ) )
				{
					oCurFont.m_nID = oParam.oRtf->m_oFontTable.GetCount() + 1;
					oCurFont.m_sName = sFont;
					oParam.oRtf->m_oFontTable.DirectAddItem( oCurFont );
				}
				RtfFieldPtr oNewField ( new RtfField() );
				
				oNewField->m_pInsert = RtfFieldInstPtr( new RtfFieldInst() );
				oNewField->m_pResult = RtfFieldInstPtr( new RtfFieldInst() );
				
				RtfCharPtr pNewChar ( new RtfChar() );
				pNewChar->m_bRtfEncode = false;
				std::wstring sFieldText;
				int nFontSize = 10;
				if( PROP_DEF != oNewProperty.m_nFontSize )
					nFontSize = oNewProperty.m_nFontSize / 2;

				sFieldText = L"SYMBOL";
                sFieldText += std::to_wstring( nChar );
				sFieldText += L" \\\\f \"" + sFont + L"\" \\\\s ";
                sFieldText += std::to_wstring( nFontSize );
				
				pNewChar->setText( sFieldText );
				
				RtfParagraphPtr oNewInsertParagraph ( new RtfParagraph() );
				oNewInsertParagraph->AddItem( pNewChar );
				oNewField->m_pInsert->m_pTextItems->AddItem( oNewInsertParagraph );

				oOutputParagraph.AddItem( oNewField );	
			}
		}break;
        default:
            break;
    }
	return true;
}
bool OOXRunReader::Parse( ReaderParameter oParam , RtfParagraph& oOutputParagraph, RtfStylePtr poStyle )
{
	if (m_ooxRun == NULL && m_drawingRun == NULL) return false;
	
	if (m_drawingRun)
	{
		RtfCharProperty oNewProperty;
		oNewProperty.SetDefaultOOX();
		
		if (m_drawingRun->rPr.IsInit())
		{
			OOXrPrReader orPrReader(m_drawingRun->rPr.GetPointer());
			orPrReader.Parse( oParam, oNewProperty );
		}

		std::wstring sValue = m_drawingRun->GetText();
	
		RtfCharPtr pNewChar ( new RtfChar() );
		
		pNewChar->m_oProperty = oNewProperty;
		pNewChar->setText( sValue );
		
		oOutputParagraph.AddItem( pNewChar );		
	}
	else
	{
		RtfCharProperty oNewProperty;
		oNewProperty.SetDefaultOOX();
		//применяем default
		oNewProperty = oParam.oRtf->m_oDefaultCharProp; 
		
		//применяем внешний стиль (часть свойств не наследуется!!)
		oNewProperty.Merge( oOutputParagraph.m_oProperty.m_oCharProperty, false );
		oNewProperty.Merge( m_oCharProperty );

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

        for (std::vector<OOX::WritingElement*>::iterator it = m_ooxRun->m_arrItems.begin(); it != m_ooxRun->m_arrItems.end(); ++it)
		{
			Parse(oParam, oOutputParagraph, poStyle, oNewProperty, *it);
		}
	}
	return true;
}


OOXpPrReader::OOXpPrReader(OOX::Logic::CParagraphProperty *ooxParaProps)
{
	m_bDefStyle			= true;
	m_ooxParaProps		= ooxParaProps;
	m_drawingParaProps	= NULL;
}
OOXpPrReader::OOXpPrReader(PPTX::Logic::TextParagraphPr *ooxParaProps)
{
	m_bDefStyle			= true;
	m_ooxParaProps		= NULL;
	m_drawingParaProps	= ooxParaProps;
}
bool OOXpPrReader::Parse( ReaderParameter oParam, RtfParagraphProperty& oOutputProperty, CcnfStyle& oConditionalTableStyle )
{
	if (m_drawingParaProps) return ParseDrawing( oParam, oOutputProperty);

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
		std::wstring sStyleName = *m_ooxParaProps->m_oPStyle->m_sVal;
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
// ------------ test
	//if( m_ooxParaProps->m_oPStyle.IsInit() && m_ooxParaProps->m_oPStyle->m_sVal.IsInit())
	//{
	//	std::wstring sStyleName = m_ooxParaProps->m_oPStyle->m_sVal.get2();
	//	RtfStylePtr oCurStyle;
	//	if( true == oParam.oRtf->m_oStyleTable.GetStyle(sStyleName, oCurStyle) )
	//	{
	//		oOutputProperty.m_nStyle = oCurStyle->m_nID;
	//	}
	//}
// ------------ test
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
		oOutputProperty.m_nOutlinelevel = *m_ooxParaProps->m_oOutlineLvl->m_oVal;

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
        default: break;
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
        default: break;
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
					oOutputProperty.m_nSpaceBetween		= - m_ooxParaProps->m_oSpacing->m_oLine->ToTwips();//twips ??? todooo
					oOutputProperty.m_nSpaceMultiLine	= 0;
				}
				else if (m_ooxParaProps->m_oSpacing->m_oLineRule->GetValue() == SimpleTypes::linespacingruleAtLeast)
				{
					oOutputProperty.m_nSpaceBetween		= m_ooxParaProps->m_oSpacing->m_oLine->ToTwips();
					oOutputProperty.m_nSpaceMultiLine	= 0;
				}
				else //auto
				{
					oOutputProperty.m_nSpaceBetween		= m_ooxParaProps->m_oSpacing->m_oLine->ToTwips();
					oOutputProperty.m_nSpaceMultiLine	= 1;
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
        default: break;
        }
	}
	if( m_ooxParaProps->m_oNumPr.IsInit() )
	{
		if( m_ooxParaProps->m_oNumPr->m_oIlvl.IsInit() && m_ooxParaProps->m_oNumPr->m_oIlvl->m_oVal.IsInit())
		{
			oOutputProperty.m_nListLevel = *m_ooxParaProps->m_oNumPr->m_oIlvl->m_oVal;
		}
		if( m_ooxParaProps->m_oNumPr->m_oNumID.IsInit() && m_ooxParaProps->m_oNumPr->m_oNumID->m_oVal.IsInit())
		{
			oOutputProperty.m_nListId = *m_ooxParaProps->m_oNumPr->m_oNumID->m_oVal;
			if( PROP_DEF == oOutputProperty.m_nListLevel )
				oOutputProperty.m_nListLevel = 0;
		}
		if( PROP_DEF != oOutputProperty.m_nListLevel && PROP_DEF != oOutputProperty.m_nListId )
		{
			//применяем direct свойства списка к параграфу
			RtfListOverrideProperty oListOverrideProperty;
			//ищем по override table
			if( true == oParam.oRtf->m_oListOverrideTable.GetList( oOutputProperty.m_nListId, oListOverrideProperty ) )
			{
				RtfListProperty oListProperty;
				//Ищем по List Table
				if( true == oParam.oRtf->m_oListTable.GetList( oListOverrideProperty.m_nListID, oListProperty) )
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

	if( m_ooxParaProps->m_oRPr.IsInit() )
	{
		OOXrPrReader orPrReader(m_ooxParaProps->m_oRPr.GetPointer());
		orPrReader.Parse( oParam, oOutputProperty.m_oCharProperty );
	}

	if (m_ooxParaProps->m_oPPrChange.IsInit())
	{
		if (m_ooxParaProps->m_oPPrChange->m_sAuthor.IsInit())
			oOutputProperty.m_nPrAuth = oParam.oRtf->m_oRevisionTable.AddAuthor( m_ooxParaProps->m_oPPrChange->m_sAuthor.get2() ) + 1;
		
		if (m_ooxParaProps->m_oPPrChange->m_oDate.IsInit())
        {
            std::wstring sVal = m_ooxParaProps->m_oPPrChange->m_oDate->GetValue();
            oOutputProperty.m_nPrDate = RtfUtility::convertDateTime( sVal );
        }
		
		RtfParagraphPropertyPtr props ( new RtfParagraphProperty() );
		OOXpPrReader opPrReader(m_ooxParaProps->m_oPPrChange->m_pParPr.GetPointer());
		
		CcnfStyle style;
		if (opPrReader.Parse( oParam, *props.get(), style))
		{
			oOutputProperty.m_pOldParagraphProp = props;
		}

	}
	return true;
}
bool OOXpPrReader::ParseDrawing( ReaderParameter oParam, RtfParagraphProperty& oOutputProperty)
{
	if (m_drawingParaProps == NULL) return false;

	if (m_drawingParaProps->lvl.IsInit())
		oOutputProperty.m_nOutlinelevel = m_drawingParaProps->lvl.get();

	if( m_drawingParaProps->algn.IsInit())
	{
		switch(m_drawingParaProps->algn->GetBYTECode())
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
		default: break;
		}
	}

	//if( m_drawingParaProps->m_oInd.IsInit() )
	//{
	//	int nFirstLine = PROP_DEF;

	//	if (m_drawingParaProps->m_oInd->m_oHanging.IsInit())
	//		nFirstLine = m_drawingParaProps->m_oInd->m_oHanging->ToTwips();
	//	if( PROP_DEF != nFirstLine )
	//		oOutputProperty.m_nIndFirstLine = -nFirstLine;

	//	if (m_drawingParaProps->m_oInd->m_oFirstLine.IsInit())
	//		oOutputProperty.m_nIndFirstLine = m_drawingParaProps->m_oInd->m_oFirstLine->ToTwips();

	//	if (m_drawingParaProps->m_oInd->m_oStart.IsInit())
	//		oOutputProperty.m_nIndStart = m_drawingParaProps->m_oInd->m_oStart->ToTwips();

	//	if (m_drawingParaProps->m_oInd->m_oEnd.IsInit())
	//		oOutputProperty.m_nIndEnd = m_drawingParaProps->m_oInd->m_oEnd->ToTwips();
	//}

	if (	m_drawingParaProps->spcBef.IsInit()
		&&	m_drawingParaProps->spcBef->spcPts.IsInit())
			oOutputProperty.m_nSpaceBefore = m_drawingParaProps->spcBef->spcPts.get();

	if (	m_drawingParaProps->spcAft.IsInit()
		&&	m_drawingParaProps->spcAft->spcPts.IsInit())
			oOutputProperty.m_nSpaceAfter = m_drawingParaProps->spcAft->spcPts.get();

	if (m_drawingParaProps->ParagraphBullet.has_bullet())
	{
		oOutputProperty.m_nListLevel	= 0;
		oOutputProperty.m_nListId		= oParam.oRtf->m_oListTable.GetCount() + 1;

		RtfListProperty oNewList;
		oNewList.m_nID = oOutputProperty.m_nListId;
		oNewList.m_nListSimple = 1;

		RtfListLevelProperty oNewLevel;
		//if (m_drawingParaProps->ParagraphBullet.IsInit() && m_drawingParaProps->m_oBuChar->m_sChar.IsInit())
		//{
		//	oNewLevel.m_sText		= m_drawingParaProps->m_oBuChar->m_sChar.get();
		//	oNewLevel.m_nNumberType = 23;
		//}
		//else if ( m_drawingParaProps->m_oBuAutoNum.IsInit() )
		//{
		//	if (m_drawingParaProps->m_oBuAutoNum->m_sType.IsInit())
		//		oNewLevel.m_nNumberType = oNewLevel.GetFormat( m_drawingParaProps->m_oBuAutoNum->m_sType.get());
		//	else
		//		oNewLevel.m_nNumberType = 0;

		//	if (m_drawingParaProps->m_oBuAutoNum->m_nStartAt.IsInit())
		//		oNewLevel.m_nStart =  m_drawingParaProps->m_oBuAutoNum->m_nStartAt->GetValue();
		//}

		oNewList.AddItem( oNewLevel );
		oParam.oRtf->m_oListTable.AddItem( oNewList );	}

	if (m_drawingParaProps->rtl.IsInit())
		oOutputProperty.m_bRtl = m_drawingParaProps->rtl.get() ? 1 : 0;

	if( m_drawingParaProps->defRPr.IsInit() )
	{
		OOXrPrReader orPrReader(m_drawingParaProps->defRPr.GetPointer());
		orPrReader.Parse( oParam, oOutputProperty.m_oCharProperty );
	}

	return true;
}


OOXrPrReader::OOXrPrReader(OOX::Logic::CRunProperty *ooxRunProps)
{
	m_bDefStyle			= true;
	m_ooxRunProps		= ooxRunProps;
	m_drawingRunProps	= NULL;
}
OOXrPrReader::OOXrPrReader(PPTX::Logic::RunProperties *ooxRunProps)
{
	m_bDefStyle			= true;
	m_ooxRunProps		= NULL;
	m_drawingRunProps	= ooxRunProps;
}
bool OOXrPrReader::Parse( ReaderParameter oParam, RtfCharProperty& oOutputProperty)
{
	if (m_drawingRunProps) ParseDrawing( oParam, oOutputProperty );

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
        default: break;
		}
	}
	if (m_ooxRunProps->m_oRStyle.IsInit() && m_ooxRunProps->m_oRStyle->m_sVal.IsInit())
	{
		std::wstring sStyleID = *m_ooxRunProps->m_oRStyle->m_sVal;
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
		int nValue = m_ooxRunProps->m_oPosition->m_oVal->ToTwips() / 10;
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
		oOutputProperty.m_nFontSize = (int)(2 * m_ooxRunProps->m_oSz->m_oVal->ToPoints());

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
        default: break;
        }
	}
	if( m_ooxRunProps->m_oHighlight.IsInit() && m_ooxRunProps->m_oHighlight->m_oVal.IsInit() )
	{
		if (m_ooxRunProps->m_oHighlight->m_oVal->GetValue() != SimpleTypes::highlightcolorNone)
		{ 
			oOutputProperty.m_nHightlited = oParam.oRtf->m_oColorTable.AddItem(RtfColor(m_ooxRunProps->m_oHighlight->m_oVal->Get_R(),
																						m_ooxRunProps->m_oHighlight->m_oVal->Get_G(),
																						m_ooxRunProps->m_oHighlight->m_oVal->Get_B()));
		}
	}
	if ( m_ooxRunProps->m_oTextOutline.IsInit())
	{
		unsigned int nColor = 0; //black
		_CP_OPT(double) opacity;

		OOXShapeReader::Parse(oParam, &m_ooxRunProps->m_oTextOutline->Fill, nColor, opacity);
		
		RtfColor rtfColor; 
		rtfColor.SetRGB(nColor);

	}
	if ( m_ooxRunProps->m_oTextFill.is_init())
	{
		unsigned int nColor = 0; //black
		_CP_OPT(double) opacity;
		
		OOXShapeReader::Parse(oParam, &m_ooxRunProps->m_oTextFill, nColor, opacity);

		RtfColor rtfColor; 
		rtfColor.SetRGB(nColor);
	}
	if( m_ooxRunProps->m_oColor.IsInit() )
	{
		OOXColorReader oColorReader;
		RtfColor oColor;
		if( true == oColorReader.Parse( oParam, m_ooxRunProps->m_oColor.get2(), oColor ) )
		{
			oOutputProperty.m_nForeColor = oParam.oRtf->m_oColorTable.AddItem( oColor );
		}
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
        default: break;
        }

		if ((m_ooxRunProps->m_oU->m_oColor.IsInit()) && (m_ooxRunProps->m_oU->m_oColor->GetValue() == SimpleTypes::hexcolorRGB))
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
	}

	if (m_ooxRunProps->m_oShd.IsInit())
	{
		if ((m_ooxRunProps->m_oShd->m_oFill.IsInit()) && (m_ooxRunProps->m_oShd->m_oFill->GetValue() == SimpleTypes::hexcolorRGB))
		{
			RtfColor oColor(m_ooxRunProps->m_oShd->m_oFill->Get_R(), m_ooxRunProps->m_oShd->m_oFill->Get_G(), m_ooxRunProps->m_oShd->m_oFill->Get_B());	
			oOutputProperty.m_poShading.m_nBackColor = oParam.oRtf->m_oColorTable.AddItem( oColor );
		}
		
		if (m_ooxRunProps->m_oShd->m_oColor.IsInit())
		{
		}
		{}//.... 
	}
//-------------------------------------------------------------- 
	if (m_ooxRunProps->m_oIns.IsInit())
	{
		oOutputProperty.m_nRevised = 1;
	
		if (m_ooxRunProps->m_oIns->m_sAuthor.IsInit())
			oOutputProperty.m_nRevauth = oParam.oRtf->m_oRevisionTable.AddAuthor( m_ooxRunProps->m_oIns->m_sAuthor.get2() ) + 1;
		
		if (m_ooxRunProps->m_oIns->m_oDate.IsInit())
        {
            std::wstring sVal = m_ooxRunProps->m_oIns->m_oDate->GetValue();
            oOutputProperty.m_nRevdttm = RtfUtility::convertDateTime( sVal );
        }
	}
	if (m_ooxRunProps->m_oDel.IsInit())
	{
		oOutputProperty.m_nDeleted = 1;
		
		if (m_ooxRunProps->m_oDel->m_sAuthor.IsInit())
			oOutputProperty.m_nRevauthDel = oParam.oRtf->m_oRevisionTable.AddAuthor( m_ooxRunProps->m_oDel->m_sAuthor.get2() ) + 1;

		if (m_ooxRunProps->m_oDel->m_oDate.IsInit())
        {
            std::wstring sVal = m_ooxRunProps->m_oDel->m_oDate->GetValue();
            oOutputProperty.m_nRevdttmDel = RtfUtility::convertDateTime( sVal );
        }
	}

	if (m_ooxRunProps->m_oRPrChange.IsInit())
	{
		if (m_ooxRunProps->m_oRPrChange->m_sAuthor.IsInit())
			oOutputProperty.m_nCrAuth = oParam.oRtf->m_oRevisionTable.AddAuthor( m_ooxRunProps->m_oRPrChange->m_sAuthor.get2() ) + 1;
		
		if (m_ooxRunProps->m_oRPrChange->m_oDate.IsInit())
        {
            std::wstring sVal = m_ooxRunProps->m_oRPrChange->m_oDate->GetValue();
            oOutputProperty.m_nCrDate = RtfUtility::convertDateTime( sVal );
        }
		
		OOXrPrReader orPrReader(m_ooxRunProps->m_oRPrChange->m_pRunPr.GetPointer());
		RtfCharPropertyPtr props ( new RtfCharProperty() );
		
		if (orPrReader.Parse( oParam, *props.get() ))
		{
			oOutputProperty.m_pOldCharProp = props;
		}
	}
	return true;
}
bool OOXrPrReader::ParseDrawing(ReaderParameter oParam, RtfCharProperty& oOutputProperty)
{
	if (m_drawingRunProps == NULL) return false;


	if (m_drawingRunProps->b.IsInit())
		oOutputProperty.m_bBold = m_drawingRunProps->b.get() ? 1 : 0;

	//if (m_drawingRunProps->m_oCaps.IsInit())
	//	oOutputProperty.m_bCaps = m_drawingRunProps->m_oCaps->ToBool() ? 1 : 0;

	if (m_drawingRunProps->sz.IsInit())
		oOutputProperty.m_nFontSize = m_drawingRunProps->sz.get() / 50;

	if (m_drawingRunProps->i.IsInit())
		oOutputProperty.m_bItalic = m_drawingRunProps->i.get() ? 1 : 0;

	if (m_drawingRunProps->latin.IsInit() || m_drawingRunProps->cs.IsInit() || m_drawingRunProps->ea.IsInit())
	{
		OOXFontReader3 oFontReader3(m_drawingRunProps->latin.GetPointer(),
			m_drawingRunProps->ea.GetPointer(),
			m_drawingRunProps->cs.GetPointer());
		oFontReader3.Parse(oParam, oOutputProperty.m_nFont);
	}
	//if (m_drawingRunProps->m_oComplexFont.IsInit() && m_drawingRunProps->m_oComplexFont->m_oTypeFace.IsInit())
	//	oOutputProperty.m_nComplexScript = m_drawingRunProps->m_oCs->m_oVal.ToBool() ? 1 : 0;;

	if (m_drawingRunProps->ln.IsInit())
	{
		//oOutputProperty.m_bOutline = m_drawingRunProps->m_oOutline->ToBool() ? 1 : 0;
	}

	//if (m_drawingRunProps->m_oSmallCaps.IsInit())
	//	oOutputProperty.m_bScaps = m_drawingRunProps->m_oSmallCaps->m_oVal.ToBool() ? 1 : 0;

	//if (m_drawingRunProps->m_oShadow.IsInit())
	//	oOutputProperty.m_bShadow = m_drawingRunProps->m_oShadow->m_oVal.ToBool() ? 1 : 0;

	//if (m_drawingRunProps->m_oStrike.IsInit())
	//	oOutputProperty.m_bStrike = m_drawingRunProps->m_oStrike->m_oVal.ToBool() ? 1 : 0;

	//if (m_drawingRunProps->m_oDStrike.IsInit())
	//	oOutputProperty.m_nStriked = m_drawingRunProps->m_oDStrike->m_oVal.ToBool() ? 1 : 0;

	//if( m_drawingRunProps->m_oVertAlign.IsInit() && m_drawingRunProps->m_oVertAlign->m_oVal.IsInit())
	//{
	//	switch(m_drawingRunProps->m_oVertAlign->m_oVal->GetValue())
	//	{
	//	case SimpleTypes::verticalalignrunBaseline    : break;
	//	case SimpleTypes::verticalalignrunSubscript   : oOutputProperty.m_bSub = 1;		break;
	//	case SimpleTypes::verticalalignrunSuperscript : oOutputProperty.m_bSuper = 1;	break;
 //       default: break;
 //       }
	//}
	//if( m_drawingRunProps->m_oHighlight.IsInit() && m_drawingRunProps->m_oHighlight->m_oVal.IsInit() )
	//{
		//if (m_drawingRunProps->m_oHighlight->m_oVal->GetValue() != SimpleTypes::highlightcolorNone)
		//{
			//	oOutputProperty.m_nHightlited = oParam.oRtf->m_oColorTable.AddItem(RtfColor(m_drawingRunProps->m_oHighlight->m_oVal->Get_R(),
	//																				m_drawingRunProps->m_oHighlight->m_oVal->Get_G(),
	//																				m_drawingRunProps->m_oHighlight->m_oVal->Get_B()));
		//}
	//}
	if( m_drawingRunProps->Fill.is_init() )
	{
		//m_drawingRunProps->m_oSolidFill
		//OOXColorReader oColorReader;
		//RtfColor oColor;
		//if( true == oColorReader.Parse( oParam, m_drawingRunProps->m_oColor.get2(), oColor ) )
		//{
		//	oOutputProperty.m_nForeColor = oParam.oRtf->m_oColorTable.AddItem( oColor );
		//}
	}
	if( m_drawingRunProps->u.IsInit())
	{
		switch(m_drawingRunProps->u->GetBYTECode())
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
		default: break;
		}

		//if ((m_drawingRunProps->m_oU->m_oColor.IsInit()) && (m_drawingRunProps->m_oU->m_oColor->GetValue() == SimpleTypes::hexcolorRGB))
		//{
		//	RtfColor oColor(m_drawingRunProps->m_oU->m_oColor->Get_R(), m_drawingRunProps->m_oU->m_oColor->Get_G(), m_drawingRunProps->m_oU->m_oColor->Get_B());
		//	oOutputProperty.m_nUnderlineColor =  oParam.oRtf->m_oColorTable.AddItem( oColor );
		//}//todooo theme color, tint, shadow
	}

	return true;
}

OOXpPrFrameReader::OOXpPrFrameReader(ComplexTypes::Word::CFramePr *ooxFramePr)
{
	m_ooxFramePr = ooxFramePr;
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
            default: break;
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
            default: break;
        }
	}

	if (m_ooxFramePr->m_oVAnchor.IsInit())
	{
		switch(m_ooxFramePr->m_oVAnchor->GetValue())
		{
			case SimpleTypes::vanchorMargin : oOutputProperty.m_eVRef = RtfFrame::vr_pvmrg;	break;
			case SimpleTypes::vanchorPage   : oOutputProperty.m_eVRef = RtfFrame::vr_pvpg;	break;
			case SimpleTypes::vanchorText   : oOutputProperty.m_eVRef = RtfFrame::vr_pvpara;break;
            default: break;
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
            default: break;
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
            default: break;
        }
	}
	if (m_ooxFramePr->m_oDropCap.IsInit())
	{
        switch(m_ooxFramePr->m_oDropCap->GetValue())
		{
			case SimpleTypes::dropcapDrop   : oOutputProperty.m_DropcapType = 1; break;
			case SimpleTypes::dropcapMargin : oOutputProperty.m_DropcapType = 2; break;
			case SimpleTypes::dropcapNone   : break;
            default: break;
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


OOXSectionPropertyReader::OOXSectionPropertyReader(OOX::Logic::CSectionProperty *ooxSectionProperty)
{
	m_ooxSectionProperty = ooxSectionProperty;
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
            default: break;
        }
	}
	if( m_ooxSectionProperty->m_oCols.IsInit() )
	{
		if (!m_ooxSectionProperty->m_oCols->m_arrColumns.empty())
		{
			oOutput.m_nColumnNumber = (int)m_ooxSectionProperty->m_oCols->m_arrColumns.size();
		}
		
		if(m_ooxSectionProperty->m_oCols->m_oNum.IsInit())
		{
			oOutput.m_nColumnNumber = m_ooxSectionProperty->m_oCols->m_oNum->GetValue();
		}

		if(m_ooxSectionProperty->m_oCols->m_oSpace.IsInit())
			oOutput.m_nColumnSpace = m_ooxSectionProperty->m_oCols->m_oSpace->ToTwips(); //todooo twips????	

		if (m_ooxSectionProperty->m_oCols->m_oSep.IsInit())
			oOutput.m_bColumnLineBetween = m_ooxSectionProperty->m_oCols->m_oSep->ToBool();

		for (size_t i = 0; i < m_ooxSectionProperty->m_oCols->m_arrColumns.size(); i++ )
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
                    default: break;
                }
			}
		}
		if (m_ooxSectionProperty->m_oEndnotePr->m_oNumStart.IsInit() && 
							m_ooxSectionProperty->m_oEndnotePr->m_oNumStart->m_oVal.IsInit())
			oOutput.m_nEndnotesStart = *m_ooxSectionProperty->m_oEndnotePr->m_oNumStart->m_oVal;

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
            default: break;
            }
		}
		if (m_ooxSectionProperty->m_oFootnotePr->m_oNumStart.IsInit() && 
					m_ooxSectionProperty->m_oFootnotePr->m_oNumStart->m_oVal.IsInit())
			oOutput.m_nFootnotesStart = *m_ooxSectionProperty->m_oFootnotePr->m_oNumStart->m_oVal;
		
		if(m_ooxSectionProperty->m_oFootnotePr->m_oNumRestart.IsInit() &&
			m_ooxSectionProperty->m_oFootnotePr->m_oNumRestart->m_oVal.IsInit())
		{
			switch(m_ooxSectionProperty->m_oFootnotePr->m_oNumRestart->m_oVal->GetValue())
			{
			case SimpleTypes::restartnumberContinious : oOutput.m_eFootnotesRestart = RtfSectionProperty::fr_sftnrstcont; break;
			case SimpleTypes::restartnumberEachPage   : oOutput.m_eFootnotesRestart = RtfSectionProperty::fr_sftnrstpg; break;
			case SimpleTypes::restartnumberEachSect   : oOutput.m_eFootnotesRestart = RtfSectionProperty::fr_sftnrestart; break;
            default: break;
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
                default: break;
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
            default: break;
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
            default: break;
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
                default: break;
            }
		}
		if (m_ooxSectionProperty->m_oPgBorders->m_oZOrder.IsInit())
		{
            switch(m_ooxSectionProperty->m_oPgBorders->m_oZOrder->GetValue())
            {
            case SimpleTypes::pageborderzorderBack  : oOutput.m_nBorderMeasure = oOutput.m_nBorderMeasure | 8; break;
            case SimpleTypes::pageborderzorderFront : break;
            default: break;
            }
        }
		if (m_ooxSectionProperty->m_oPgBorders->m_oOffsetFrom.IsInit())
		{
			switch(m_ooxSectionProperty->m_oPgBorders->m_oOffsetFrom->GetValue())
			{
			case SimpleTypes::pageborderoffsetPage : oOutput.m_nBorderMeasure = oOutput.m_nBorderMeasure | 32; break;
			case SimpleTypes::pageborderoffsetText : break;
            default: break;
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
	for (size_t i = 0 ; i < m_ooxSectionProperty->m_arrHeaderReference.size(); i++ )
	{
		OOXHeaderReader oHeaderReader(m_ooxSectionProperty->m_arrHeaderReference[i]);
		
		if(m_ooxSectionProperty->m_arrHeaderReference[i]->m_oType.IsInit() &&
			m_ooxSectionProperty->m_arrHeaderReference[i]->m_oId.IsInit() )
		{
			std::wstring sID = m_ooxSectionProperty->m_arrHeaderReference[i]->m_oId->GetValue();
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
            default:
                break;
            }
        }
	}
	for (size_t i = 0 ; i < m_ooxSectionProperty->m_arrFooterReference.size(); i++ )
	{
		OOXHeaderReader oFooterReader(m_ooxSectionProperty->m_arrFooterReference[i]);
		
		if(m_ooxSectionProperty->m_arrFooterReference[i]->m_oType.IsInit() &&
			m_ooxSectionProperty->m_arrFooterReference[i]->m_oId.IsInit() )
		{
			std::wstring sID = m_ooxSectionProperty->m_arrFooterReference[i]->m_oId->GetValue();
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
            default:
                break;
            }
		}
	}

	if (m_ooxSectionProperty->m_oSectPrChange.IsInit())
	{
		if (m_ooxSectionProperty->m_oSectPrChange->m_sAuthor.IsInit())
			oOutput.m_nSrAuth = oParam.oRtf->m_oRevisionTable.AddAuthor( m_ooxSectionProperty->m_oSectPrChange->m_sAuthor.get2() ) + 1;
		
		if (m_ooxSectionProperty->m_oSectPrChange->m_oDate.IsInit())
        {
            std::wstring sVal = m_ooxSectionProperty->m_oSectPrChange->m_oDate->GetValue();
            oOutput.m_nSrDate = RtfUtility::convertDateTime( sVal );
        }
		
		RtfSectionPropertyPtr props ( new RtfSectionProperty() );
		OOXSectionPropertyReader opPrReader(m_ooxSectionProperty->m_oSectPrChange->m_pSecPr.GetPointer());
		
		if (opPrReader.Parse( oParam, *props.get() ))
		{
			oOutput.m_pOldSectionProp = props;
		}
	}
	return true;
}
