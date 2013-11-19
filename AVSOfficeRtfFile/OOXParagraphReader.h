#pragma once
#include "RtfDocument.h"
#include "OOXRunReader.h"
#include "OOXpPrReader.h"
#include "RtfField.h"
#include "OOXMathReader.h"

[ event_source(native)]
class OOXParagraphReader
{
public: __event void OnFieldChar(  );

private: OOXRunReader m_oRunReader;
public: bool Parse( ReaderParameter oParam , RtfParagraph& oOutputParagraph, CcnfStyle oConditionalTableStyle, bool& bStartNewSection )
		{
			//надо default стиль применять до OOXParagraphReader
			////применяем default
			//oOutputParagraph.m_oProperty = oParam.oDocument->m_oDefaultParagraphProp; 

			XmlUtils::CXmlReader oXmlReader;
			RtfTableStylePtr oResultTableStyle;
			//сначала применяем default стиль (если это нужно)
			oXmlReader.OpenFromXmlNode(oParam.oNode);
			if( TRUE == oXmlReader.ReadNode( _T("w:pPr") ) )
			{
				CString sXml = oXmlReader.ReadNodeXml();
				XML::IXMLDOMNodePtr oNode;
				oXmlReader.GetNode( oNode );
				ReaderParameter oNewParam = oParam;
				oNewParam.oNode = oNode;
				OOXpPrReader opPrReader;
				opPrReader.Parse( oNewParam, oOutputParagraph.m_oProperty, oConditionalTableStyle, bStartNewSection);
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
					if( true == oParam.oDocument->m_oStyleTable.GetStyle(oOutputParagraph.m_oProperty.m_nStyle, oTempStyle) )
					{
						RtfStylePtr oResultStyle = oParam.oDocument->m_oStyleTable.GetStyleResulting( oTempStyle );
						poExternalStyle->Merge( oResultStyle );
					}
				}
			}
			
			oXmlReader.OpenFromXmlNode(oParam.oNode);
			oXmlReader.ReadNodeList( _T("*") );
			for(int i=0; i<oXmlReader.GetLengthList();i++ )
			{
				CString sNodeName = oXmlReader.ReadNodeName(i);
				XML::IXMLDOMNodePtr oNode;
				oXmlReader.GetNode( i , oNode );
				ReaderParameter oNewParam = oParam;
				oNewParam.oNode = oNode;
				if( _T("w:r") == sNodeName )
					m_oRunReader.Parse( oNewParam, oOutputParagraph, poExternalStyle );
				else if( _T("w:fldSimple") == sNodeName )
				{
					RtfFieldPtr oCurField( new RtfField() );
					oCurField->m_oInsert = TextItemContainerPtr( new TextItemContainer() );
					oCurField->m_oResult = TextItemContainerPtr( new TextItemContainer() );
					//добавляем insert
					RtfCharPtr oNewChar( new RtfChar() );
					oNewChar->m_bRtfEncode = false;
					oNewChar->setText( oXmlReader.ReadNodeAttribute(i,_T("w:instr"),_T("")) );
					RtfParagraphPtr oNewInsertParagraph( new RtfParagraph() );
					oNewInsertParagraph->AddItem( oNewChar );
					oCurField->m_oInsert->AddItem( oNewInsertParagraph );
					//добаляем свойства
					BOOL bLock = Strings::ToBoolean(oXmlReader.ReadNodeAttribute(i,_T("w:fldLock"),_T("false")));
					if( TRUE == bLock )
						oCurField->m_eMode = RtfField::fm_fldlock;
					BOOL bDirty = Strings::ToBoolean(oXmlReader.ReadNodeAttribute(i,_T("w:dirty"),_T("false")));
					if( TRUE == bDirty )
						oCurField->m_eMode = RtfField::fm_flddirty;

					RtfParagraphPtr oNewResultParagraph( new RtfParagraph() );
					//применяем к новому параграфу default property
					oNewResultParagraph->m_oProperty = oParam.oDocument->m_oDefaultParagraphProp;
					oNewResultParagraph->m_oProperty.m_oCharProperty = oParam.oDocument->m_oDefaultCharProp;
					//применяем к новому параграфу свойства данного параграфа
					oNewResultParagraph->m_oProperty = oOutputParagraph.m_oProperty;
					OOXParagraphReader oSubParReader;
					bool bStartNewSection;
					oSubParReader.Parse( oNewParam, *oNewResultParagraph, CcnfStyle(), bStartNewSection);
					oCurField->m_oResult->AddItem( oNewResultParagraph );

					oOutputParagraph.AddItem( oCurField );
				}
				else if( _T("w:hyperlink") == sNodeName )
				{
					CString sId = oXmlReader.ReadNodeAttribute(i, _T("r:id"));
					CString sAnchor = oXmlReader.ReadNodeAttribute(i, _T("w:anchor"));

					if( _T("") != sId )
					{
						CString sTarget = oParam.oRels->GetByID(sId);
						if( _T("") != sTarget )
						{
							//заменяем пробелы на %20
							sTarget.Replace( _T(" "), _T("%20") );
							//заменяем пробелы на %20
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
							sFieldText.AppendFormat(_T("HYPERLINK \"%s\""), sTarget );
							oNewChar->setText( sFieldText );
							RtfParagraphPtr oNewInsertParagraph( new RtfParagraph() );
							oNewInsertParagraph->AddItem( oNewChar );
							oCurField->m_oInsert->AddItem( oNewInsertParagraph );
							//добаляем свойства
							BOOL bLock = Strings::ToBoolean(oXmlReader.ReadNodeAttribute(i,_T("w:fldLock"),_T("false")));
							if( TRUE == bLock )
								oCurField->m_eMode = RtfField::fm_fldlock;
							BOOL bDirty = Strings::ToBoolean(oXmlReader.ReadNodeAttribute(i,_T("w:dirty"),_T("false")));
							if( TRUE == bDirty )
								oCurField->m_eMode = RtfField::fm_flddirty;

							RtfParagraphPtr oNewResultParagraph( new RtfParagraph() );
							//применяем к новому параграфу default property
							oNewResultParagraph->m_oProperty = oParam.oDocument->m_oDefaultParagraphProp;
							oNewResultParagraph->m_oProperty.m_oCharProperty = oParam.oDocument->m_oDefaultCharProp;
							//применяем к новому параграфу свойства данного параграфа
							oNewResultParagraph->m_oProperty = oOutputParagraph.m_oProperty;
							OOXParagraphReader oSubParReader;
							bool bStartNewSection;
							oSubParReader.Parse( oNewParam, *oNewResultParagraph, CcnfStyle(), bStartNewSection);
							oCurField->m_oResult->AddItem( oNewResultParagraph );

							oOutputParagraph.AddItem( oCurField );
						}
					}
					else if( _T("") != sAnchor )
					{
							RtfFieldPtr oCurField( new RtfField() );
							oCurField->m_oInsert = TextItemContainerPtr( new TextItemContainer() );
							oCurField->m_oResult = TextItemContainerPtr( new TextItemContainer() );
							//добавляем insert
							RtfCharPtr oNewCharHYPER( new RtfChar() );
							oNewCharHYPER->m_bRtfEncode = false;
							oNewCharHYPER->setText( _T("HYPERLINK \\l \"") + sAnchor +_T("\"") );

							RtfParagraphPtr oNewInsertParagraph( new RtfParagraph() );
							oNewInsertParagraph->AddItem( oNewCharHYPER );

							oCurField->m_oInsert->AddItem( oNewInsertParagraph );
							//добаляем свойства
							BOOL bLock = Strings::ToBoolean(oXmlReader.ReadNodeAttribute(i,_T("w:fldLock"),_T("false")));
							if( TRUE == bLock )
								oCurField->m_eMode = RtfField::fm_fldlock;
							BOOL bDirty = Strings::ToBoolean(oXmlReader.ReadNodeAttribute(i,_T("w:dirty"),_T("false")));
							if( TRUE == bDirty )
								oCurField->m_eMode = RtfField::fm_flddirty;

							RtfParagraphPtr oNewResultParagraph( new RtfParagraph() );
							//применяем к новому параграфу default property
							oNewResultParagraph->m_oProperty = oParam.oDocument->m_oDefaultParagraphProp;
							oNewResultParagraph->m_oProperty.m_oCharProperty = oParam.oDocument->m_oDefaultCharProp;
							//применяем к новому параграфу свойства данного параграфа
							oNewResultParagraph->m_oProperty = oOutputParagraph.m_oProperty;
							OOXParagraphReader oSubParReader;
							bool bStartNewSection;
							oSubParReader.Parse( oNewParam, *oNewResultParagraph, CcnfStyle(), bStartNewSection);
							oCurField->m_oResult->AddItem( oNewResultParagraph );

							oOutputParagraph.AddItem( oCurField );
					}
				}
				else if( "w:bookmarkStart" == sNodeName )
					{
						RtfBookmarkStartPtr oNewBookmark( new RtfBookmarkStart() );
						oNewBookmark->m_sName = oXmlReader.ReadNodeAttribute( i, _T("w:name") );
						COMMAND_OOX_BOOL_ATTRIBUTE( _T("w:colFirst"), oNewBookmark->nFirstColumn, oXmlReader, i )
						COMMAND_OOX_BOOL_ATTRIBUTE( _T("w:colLast"), oNewBookmark->nLastColumn, oXmlReader, i )
						CString sId = oXmlReader.ReadNodeAttribute( i, _T("w:id") );
						if( _T("") != sId )
						{
							int nId = Strings::ToInteger( sId );
							oParam.oReader->aBookmarks.Add( nId, oNewBookmark->m_sName );
							oOutputParagraph.AddItem( oNewBookmark );
						}
					}
				else if( "w:bookmarkEnd" == sNodeName )
					{
						RtfBookmarkEndPtr oNewBookmark( new RtfBookmarkEnd() );
						oNewBookmark->m_sName = oXmlReader.ReadNodeAttribute( i, _T("w:name") );

						int nId = Strings::ToInteger( oXmlReader.ReadNodeAttribute( i, _T("w:id") ) );
						int nIndex = oParam.oReader->aBookmarks.FindKey( nId );
						if( -1 != nIndex )
						{
							oNewBookmark->m_sName = oParam.oReader->aBookmarks.GetValueAt( nIndex );
							oOutputParagraph.AddItem( oNewBookmark );
						}
					}
				else if( _T("w:smartTag") == sNodeName )
				{
					XmlUtils::CXmlReader oXmlSubReader;
					if( TRUE == oXmlSubReader.OpenFromXmlNode( oNode ) )
						if( TRUE == oXmlSubReader.ReadNodeList( _T("w:r") ) )
						{
							for( int j = 0;  j < oXmlSubReader.GetLengthList(); j++ )
							{
								oXmlSubReader.GetNode( j , oNode );
								oNewParam.oNode = oNode;
								m_oRunReader.Parse( oNewParam, oOutputParagraph, poExternalStyle );
							}
						}
				}
				else if( _T("m:oMathPara") == sNodeName )
				{
					RtfMathPtr oNewMath( new RtfMath() );
					oNewMath->SetOOXName( sNodeName );
					OOXMathReader oMathRreader;
					if(true == oMathRreader.Parse( oNewParam, (*oNewMath) ) )
						oOutputParagraph.AddItem( oNewMath );
				}
				else if( _T("w:sdt") == sNodeName )
				{
					XmlUtils::CXmlReader oXmlSubReader;
					if(TRUE == oXmlSubReader.OpenFromXmlNode( oNode ) )
					{
						if( TRUE == oXmlSubReader.ReadNode( _T("w:sdtEndPr") ) )
						{
							//todo
						}
						oXmlSubReader.OpenFromXmlNode( oNode );
						if( TRUE == oXmlSubReader.ReadNode( _T("w:sdtContent") ) )
						{
							XML::IXMLDOMNodePtr oSubNode;
							oXmlSubReader.GetNode(oSubNode);
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oSubNode;
							OOXParagraphReader oStdReader;
							bool bStartNewSection;
							oStdReader.Parse( oNewParam, oOutputParagraph, CcnfStyle(), bStartNewSection );						}
					}
				}
			}
			return true;
		}

};
