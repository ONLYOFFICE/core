#pragma once
#include "RtfDocument.h"
#include "RtfTable.h"
#include "OOXTableReader.h"
#include "OOXParagraphReader.h"
#include "../Common/OfficeFileTemplate.h"

class OOXTextItemReader
{
private: bool m_bRaiseProgress;
public: TextItemContainerPtr m_oTextItems;

private: OOXParagraphReader m_oParagraphReader;
private: OOXTableReader m_oTableReader;

public: OOXTextItemReader( bool bRaiseProgress = false ):m_bRaiseProgress(bRaiseProgress)
		{
			m_oTextItems = TextItemContainerPtr( new TextItemContainer() );
		}
public: bool Parse( ReaderParameter oParam )
		{
			//парсим документ
			XmlUtils::CXmlReader oXmlReader;
			if( TRUE == oXmlReader.OpenFromXmlNode( oParam.oNode ) )
			{
				oXmlReader.ReadNodeList( _T("*") );
				int nXmlLength = oXmlReader.GetLengthList();
				for( int i = 0; i < nXmlLength; i++ )
				{
					CString sNodeName = oXmlReader.ReadNodeName(i);
					XML::IXMLDOMNodePtr oNode;
					oXmlReader.GetNode(i,oNode);
					ReaderParameter oNewParam = oParam;
					oNewParam.oNode = oNode;
					if( _T("w:p") == sNodeName )
					{
						bool bStartNewSection = false;
						RtfParagraphPtr oNewParagraph( new RtfParagraph() );
						//примен€ем к новому параграфу default property
						oNewParagraph->m_oProperty = oParam.oDocument->m_oDefaultParagraphProp;
						oNewParagraph->m_oProperty.m_oCharProperty = oParam.oDocument->m_oDefaultCharProp;
						oNewParagraph->m_oProperty.m_nItap = 0;

						if( true == m_oParagraphReader.Parse( oNewParam, (*oNewParagraph), CcnfStyle(), bStartNewSection ))
						{
							m_oTextItems->AddItem( oNewParagraph );
							if( true == m_bRaiseProgress )
							{
								oParam.oReader->OnCompleteItem();
								short shCancel = c_shProgressContinue;
								oParam.oReader->OnProgress( g_cdMaxPercent * (i + 1) / nXmlLength, &shCancel ); //todo исправить дл€ field
								if( c_shProgressCancel == shCancel )
									break;
							}
							if( true == bStartNewSection )
							{
								RtfSectionPtr oCurSection;
								//1 - т.к. секции удал€ютс€ когда полностью запишутс€
								if( true == oParam.oDocument->GetItem( oCurSection, 1 ) )
								{
									m_oTextItems = oCurSection;
								}
								//OnSectionEnd();
							}
						}
					}
					else if( _T("w:tbl") == sNodeName )
					{
						RtfTablePtr oNewTable( new RtfTable() );
						
						oParam.oReader->m_nCurItap = 1;
						if( true == m_oTableReader.Parse( oNewParam, (*oNewTable)) )
						{
							m_oTextItems->AddItem( oNewTable );
							if( true == m_bRaiseProgress )
							{
								oParam.oReader->OnCompleteItem();
								short shCancel = c_shProgressContinue;
								oParam.oReader->OnProgress( g_cdMaxPercent * (i + 1) / nXmlLength, &shCancel );
								if( c_shProgressCancel == shCancel )
									break;
							}
						}
						oParam.oReader->m_nCurItap = 0;
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
								Parse( oNewParam );
							}
						}
					}
				}
			}
			return true;
		}
};