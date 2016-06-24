/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once
#ifndef OOX_WORKSHEET_FILE_INCLUDE_H_
#define OOX_WORKSHEET_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "../../DocxFormat/External/HyperLink.h"
#include "../../DocxFormat/Media/Image.h"
#include "../../DocxFormat/VmlDrawing.h"

#include "SheetData.h"
#include "Cols.h"
#include "Hyperlinks.h"
#include "MergeCells.h"
#include "WorksheetChildOther.h"
#include "ConditionalFormatting.h"

#include "../Drawing/Drawing.h"
#include "../Chart/Chart.h"
#include "../Table/Table.h"
#include "../Comments/Comments.h"
#include "../Ole/OleObjects.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//необработанные child:
		//<cellWatches>
		//<colBreaks>
		//<controls>
		//<customProperties>
		//<dataConsolidate>
		//<dataValidations>
		//<drawing>
		//<drawingHF>
		//<extLst>
		//<headerFooter>
		//<oleObjects>
		//<phoneticPr>
		//<picture>
		//<protectedRanges>
		//<rowBreaks>
		//<scenarios>
		//<sheetCalcPr>
		//<sheetProtection>
		//<sheetViews>
		//<smartTags>
		//<sortState>
		//<webPublishItems>
		class CWorksheet : public OOX::File, public OOX::Spreadsheet::IFileContainer
		{
		public:
			CWorksheet()
			{
			}
			CWorksheet(const CPath& oRootPath, const CPath& oPath)
			{
				read( oRootPath, oPath );
			}
			virtual ~CWorksheet()
			{
				ClearItems();
			}
		public:

			virtual void read(const CPath& oPath)
			{
				//don't use this. instead use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath)
			{
				m_oReadPath = oPath;
				IFileContainer::Read( oRootPath, oPath );

				XmlUtils::CXmlLiteReader oReader;

				if ( !oReader.FromFile( oPath.GetPath() ) )
					return;

				if ( !oReader.ReadNextNode() )
					return;

				CString sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if ( _T("worksheet") == sName || _T("chartsheet") == sName)
				{
					if ( !oReader.IsEmptyNode() )
					{
						int nDocumentDepth = oReader.GetDepth();
						while ( oReader.ReadNextSiblingNode( nDocumentDepth ) )
						{
							sName = XmlUtils::GetNameNoNS(oReader.GetName());

							if ( _T("cols") == sName )
								m_oCols = oReader;
							else if ( _T("dimension") == sName )
								m_oDimension = oReader;
							else if ( _T("drawing") == sName )
								m_oDrawing = oReader;
							else if ( _T("hyperlinks") == sName )
								m_oHyperlinks = oReader;
							else if ( _T("mergeCells") == sName )
								m_oMergeCells = oReader;
							else if ( _T("pageMargins") == sName )
								m_oPageMargins = oReader;
							else if ( _T("pageSetup") == sName )
								m_oPageSetup = oReader;
							else if ( _T("printOptions") == sName )
								m_oPrintOptions = oReader;
							else if ( _T("sheetData") == sName )
								m_oSheetData = oReader;
							else if (_T("conditionalFormatting") == sName)
								m_arrConditionalFormatting.push_back(new CConditionalFormatting(oReader));
							else if ( _T("sheetFormatPr") == sName )
								m_oSheetFormatPr = oReader;
							else if ( _T("sheetViews") == sName )
								m_oSheetViews = oReader;
							else if ( _T("autoFilter") == sName )
								m_oAutofilter = oReader;
							else if ( _T("tableParts") == sName )
								m_oTableParts = oReader;
							else if ( _T("legacyDrawing") == sName )
								m_oLegacyDrawingWorksheet = oReader;
							else if ( _T("oleObjects") == sName )
								m_oOleObjects = oReader;
							else if (_T("sheetPr") == sName)
								m_oSheetPr = oReader;
                            else if (_T("extLst") == sName)
                                m_oExtLst = oReader;
						}
					}
					if(m_oLegacyDrawingWorksheet.IsInit() && m_oLegacyDrawingWorksheet->m_oId.IsInit())
					{
						OOX::RId oRId(m_oLegacyDrawingWorksheet->m_oId->GetValue());
						
						smart_ptr<OOX::File> oVmlDrawing = IFileContainer::Find(oRId);
						smart_ptr<OOX::File> oComments = IFileContainer::Get(FileTypes::Comments);
						
						if (oComments.IsInit() && FileTypes::Comments == oComments->type() && oVmlDrawing.IsInit() && OOX::FileTypes::VmlDrawing == oVmlDrawing->type())
						{
							OOX::Spreadsheet::CComments* pComments		= static_cast<OOX::Spreadsheet::CComments*>(oComments.operator->());
							OOX::CVmlDrawing* pVmlDrawing	= static_cast<OOX::CVmlDrawing*>(oVmlDrawing.operator->());
							
							PrepareComments(pComments, pVmlDrawing);
						}
					}
				}		
			}
			void PrepareComments(OOX::Spreadsheet::CComments* pComments, OOX::CVmlDrawing* pVmlDrawing)
			{
				std::vector<CString*> & aAuthors = pComments->m_oAuthors->m_arrItems;
				
				if(pComments->m_oCommentList.IsInit())
				{
					std::vector<OOX::Spreadsheet::CComment*> & aComments = pComments->m_oCommentList->m_arrItems;
					for(unsigned int i = 0, length = aComments.size(); i < length; ++i)
					{
						OOX::Spreadsheet::CComment* pComment = aComments[i];
						if(pComment->m_oRef.IsInit() && pComment->m_oAuthorId.IsInit())
						{
							int nRow, nCol;
							if(parseRef(pComment->m_oRef->GetValue(), nRow, nCol))
							{
								CCommentItem* pCommentItem = new CCommentItem();
								pCommentItem->m_nRow = nRow - 1;
								pCommentItem->m_nCol = nCol - 1;

								unsigned int nAuthorId = pComment->m_oAuthorId->GetValue();
								if(nAuthorId < aAuthors.size())
									pCommentItem->m_sAuthor = *aAuthors[nAuthorId];

								OOX::Spreadsheet::CSi* pSi = pComment->m_oText.GetPointerEmptyNullable();
								if(NULL != pSi)
									pCommentItem->m_oText.reset(pSi);
								CString sNewId;sNewId.Format(_T("%d-%d"), pCommentItem->m_nRow.get(), pCommentItem->m_nCol.get());
								m_mapComments [sNewId] = pCommentItem;
							}
						}
					}
				}

				for(unsigned int i = 0, length = pVmlDrawing->m_arrItems.size(); i < length; ++i)
				{
					OOX::Vml::CShape* pShape =  dynamic_cast<OOX::Vml::CShape*>(pVmlDrawing->m_arrItems[i]);
					
					if (pShape == NULL) continue;

					for(unsigned int j = 0, length2 = pShape->m_arrItems.size(); j < length2; ++j)
					{
						OOX::WritingElement* pElem = pShape->m_arrItems[j];
						if( OOX::et_v_ClientData == pElem->getType())
						{
							OOX::Vml::CClientData* pClientData = static_cast<OOX::Vml::CClientData*>(pElem);
							if(pClientData->m_oRow.IsInit() && pClientData->m_oColumn.IsInit())
							{
								int nRow = pClientData->m_oRow->GetValue();
								int nCol = pClientData->m_oColumn->GetValue();
								CString sId;sId.Format(_T("%d-%d"), nRow, nCol);

								std::map<CString, CCommentItem*>::const_iterator pPair = m_mapComments.find(sId);
								if(pPair != m_mapComments.end())
								{
									CCommentItem* pCommentItem = pPair->second;
									if(pShape->m_oGfxData.IsInit())
										pCommentItem->m_sGfxdata = pShape->m_oGfxData.get2();
									std::vector<int> m_aAnchor;
									pClientData->getAnchorArray(m_aAnchor);
									if(8 == m_aAnchor.size())
									{
										pCommentItem->m_nLeft = m_aAnchor[0];
										pCommentItem->m_nLeftOffset = m_aAnchor[1];
										pCommentItem->m_nTop = m_aAnchor[2];
										pCommentItem->m_nTopOffset = m_aAnchor[3];
										pCommentItem->m_nRight = m_aAnchor[4];
										pCommentItem->m_nRightOffset = m_aAnchor[5];
										pCommentItem->m_nBottom = m_aAnchor[6];
										pCommentItem->m_nBottomOffset = m_aAnchor[7];
									}

									if(pClientData->m_oMoveWithCells.IsInit())
										pCommentItem->m_bMove = pClientData->m_oMoveWithCells->ToBool();
									if(pClientData->m_oSizeWithCells.IsInit())
										pCommentItem->m_bSize = pClientData->m_oSizeWithCells->ToBool();

									for(unsigned int k = 0 ,length3 = pShape->m_oStyle->m_arrProperties.size(); k < length3; ++k)
									{
										if (pShape->m_oStyle->m_arrProperties[k] == NULL) continue;

										SimpleTypes::Vml::CCssProperty *oProperty = pShape->m_oStyle->m_arrProperties[k];
										if(SimpleTypes::Vml::cssptMarginLeft == oProperty->get_Type())
										{
											SimpleTypes::Vml::UCssValue oUCssValue= oProperty->get_Value();
											if(SimpleTypes::Vml::cssunitstypeUnits == oUCssValue.oValue.eType)
											{
												SimpleTypes::CPoint oPoint;
												oPoint.FromPoints(oUCssValue.oValue.dValue);
												pCommentItem->m_dLeftMM = oPoint.ToMm();
											}
										}
										else if(SimpleTypes::Vml::cssptMarginTop == oProperty->get_Type())
										{
											SimpleTypes::Vml::UCssValue oUCssValue= oProperty->get_Value();
											if(SimpleTypes::Vml::cssunitstypeUnits == oUCssValue.oValue.eType)
											{
												SimpleTypes::CPoint oPoint;
												oPoint.FromPoints(oUCssValue.oValue.dValue);
												pCommentItem->m_dTopMM = oPoint.ToMm();
											}
										}
										else if(SimpleTypes::Vml::cssptWidth == oProperty->get_Type())
										{
											SimpleTypes::Vml::UCssValue oUCssValue= oProperty->get_Value();
											if(SimpleTypes::Vml::cssunitstypeUnits == oUCssValue.oValue.eType)
											{
												SimpleTypes::CPoint oPoint;
												oPoint.FromPoints(oUCssValue.oValue.dValue);
												pCommentItem->m_dWidthMM = oPoint.ToMm();
											}
										}
										else if(SimpleTypes::Vml::cssptHeight == oProperty->get_Type())
										{
											SimpleTypes::Vml::UCssValue oUCssValue= oProperty->get_Value();
											if(SimpleTypes::Vml::cssunitstypeUnits == oUCssValue.oValue.eType)
											{
												SimpleTypes::CPoint oPoint;
												oPoint.FromPoints(oUCssValue.oValue.dValue);
												pCommentItem->m_dHeightMM = oPoint.ToMm();
											}
										}
									}
								}
							}
						}
					}
				}
			}
			void PrepareToWrite()
			{
				if(false == m_oSheetFormatPr.IsInit())
					m_oSheetFormatPr.Init();
				if(false == m_oSheetFormatPr->m_oDefaultRowHeight.IsInit())
				{
					m_oSheetFormatPr->m_oDefaultRowHeight.Init();
					m_oSheetFormatPr->m_oDefaultRowHeight->SetValue(15);
				}
				if(false == m_oSheetViews.IsInit())
					m_oSheetViews.Init();
				if(0 == m_oSheetViews->m_arrItems.size())
					m_oSheetViews->m_arrItems.push_back(new CSheetView());
				CSheetView* pSheetView = m_oSheetViews->m_arrItems[0];
				if(false == pSheetView->m_oWorkbookViewId.IsInit())
				{
					pSheetView->m_oWorkbookViewId.Init();
					pSheetView->m_oWorkbookViewId->SetValue(0);
				}
			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
				XmlUtils::CStringWriter sXml;
				sXml.WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><worksheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" mc:Ignorable=\"x14ac\" xmlns:x14ac=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/ac\">"));
				if(m_oSheetPr.IsInit())
					m_oSheetPr->toXML(sXml);
				if(m_oSheetViews.IsInit())
					m_oSheetViews->toXML(sXml);
				if(m_oSheetFormatPr.IsInit())
					m_oSheetFormatPr->toXML(sXml);
				if(m_oCols.IsInit())
					m_oCols->toXML(sXml);
				if(m_oSheetData.IsInit())
					m_oSheetData->toXML(sXml);
				for (unsigned int nIndex = 0, nLength = m_arrConditionalFormatting.size(); nIndex < nLength; ++nIndex)
					m_arrConditionalFormatting[nIndex]->toXML();
				if(m_oAutofilter.IsInit())
					m_oAutofilter->toXML(sXml);
				if(m_oMergeCells.IsInit())
					m_oMergeCells->toXML(sXml);
				if(m_oHyperlinks.IsInit())
					m_oHyperlinks->toXML(sXml);
				if(m_oPrintOptions.IsInit())
					m_oPrintOptions->toXML(sXml);
				if(m_oPageMargins.IsInit())
					m_oPageMargins->toXML(sXml);
				if(m_oPageSetup.IsInit())
					m_oPageSetup->toXML(sXml);
				if(m_oDrawing.IsInit())
					m_oDrawing->toXML(sXml);
				if(m_oLegacyDrawingWorksheet.IsInit())
					m_oLegacyDrawingWorksheet->toXML(sXml);
				if(m_oOleObjects.IsInit())
					m_oOleObjects->toXML(sXml);
				if(m_oTableParts.IsInit())
					m_oTableParts->toXML(sXml);
                if(m_oExtLst.IsInit())
                    sXml.WriteString(m_oExtLst->toXMLWithNS(_T("")));
				sXml.WriteString(_T("</worksheet>"));

				CDirectory::SaveToFile( oPath.GetPath(), sXml.GetData() );
				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
				IFileContainer::Write( oPath, oDirectory, oContent );
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::Worksheet;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& GetReadPath() const
			{
				return m_oReadPath;
			}
			const OOX::RId AddHyperlink (CString& sHref)
			{
				smart_ptr<OOX::HyperLink> oHyperlink = smart_ptr<OOX::HyperLink>( new OOX::HyperLink( OOX::CPath(sHref, false) ) );
				CString sExistRId = IsExistHyperlink(oHyperlink);
				if(sExistRId.IsEmpty())
				{
					smart_ptr<OOX::File> oHyperlinkFile = oHyperlink.smart_dynamic_cast<OOX::File>();
					const OOX::RId rId = Add( oHyperlinkFile );
					return rId;
				}
				else
				{
					const OOX::RId rId(sExistRId);
					return rId;
				}
			}
			static bool parse3DRef(const CString& sRef, CString& workbook, CString& sheetFrom, CString& sheetTo, int& nRow1, int& nCol1, int& nRow2, int& nCol2)
			{
				bool bRes = false;
				int nIndex = sRef.Find('!');
				CString sCellRef;
				if(-1 != nIndex)
				{
					CString sSheetPrefix = sRef.Left(nIndex);
					if(sSheetPrefix.GetLength() > 0 && '\'' == sSheetPrefix[0] && '\'' == sSheetPrefix[sSheetPrefix.GetLength() - 1])
					{
						sSheetPrefix = sSheetPrefix.Mid(1, sSheetPrefix.GetLength() - 2);
					}
					sSheetPrefix.Replace(L"''", L"'");
					int nIndexWbStart = sSheetPrefix.Find('[');
					int nIndexWbEnd = sSheetPrefix.Find(']');
					if(-1 != nIndexWbStart && -1 != nIndexWbEnd)
					{
						workbook = sSheetPrefix.Mid(nIndexWbStart + 1, nIndexWbEnd - nIndexWbStart - 1);
						sSheetPrefix = sSheetPrefix.Right(sSheetPrefix.GetLength() - nIndexWbEnd - 1);
					}
					int nIndexColon = sSheetPrefix.Find(':');
					if(-1 != nIndexColon)
					{
						sheetFrom = sSheetPrefix.Left(nIndexColon);
						sheetTo = sSheetPrefix.Right(sSheetPrefix.GetLength() - nIndexColon - 1);
					}
					else
					{
						sheetFrom = sSheetPrefix;
					}
					sCellRef = sRef.Right(sRef.GetLength() - nIndex - 1);
				}
				else
				{
					sCellRef = sRef;
				}
				sCellRef.Replace(L"$", L"");
				int nIndexColon = sCellRef.Find(':');
				if(-1 != nIndexColon)
				{
					bRes = parseRef(sCellRef.Left(nIndexColon), nRow1, nCol1) && parseRef(sCellRef.Right(sCellRef.GetLength() - nIndexColon - 1), nRow2, nCol2);
				}
				else
				{
					bRes = parseRef(sCellRef, nRow1, nCol1);
					nRow2 = nRow1;
					nCol2 = nCol1;
				}
				return bRes;
			}
			static bool parseRef(CString sRef, int& nRow, int& nCol)
			{
				bool bRes = false;
				nRow = 0;
				nCol = 0;
				int nLegnth = sRef.GetLength();
				if(nLegnth > 0)
				{
					int nIndex = 0;
					sRef.MakeUpper();
					TCHAR cCurLetter = sRef[nIndex];
					while('A' <= cCurLetter && cCurLetter <= 'Z' && nIndex < nLegnth)
					{
						nIndex++;
						cCurLetter = sRef[nIndex];
					}
					if(nIndex > 0)
					{
						CString sAdd = sRef.Left(nIndex);
						CString sDig = sRef.Right(nLegnth - nIndex);
						for(int i = 0, length = sAdd.GetLength(); i < length; ++i)
						{
							nCol = nCol * 26 + sAdd[i] - 'A' + 1;
						}
						nRow = _wtoi(sDig);
						bRes = true;
					}
				}
				return bRes;
			}
			static CString combineRef(int nRow, int nCol)
			{
				nRow++;
				CString sRes = _T("");
				if(nCol >= 0){
					int columnNumber = nCol + 1;
					while(columnNumber > 0){
						int currentLetterNumber = (columnNumber - 1) % 26;
						sRes.Insert(0, 'A' + currentLetterNumber);
						columnNumber = (columnNumber - (currentLetterNumber + 1)) / 26;
					}
				}
				sRes.AppendFormat(_T("%d"), nRow);
				return sRes;
			}
		private:
			void ClearItems()
			{
				for (std::map<CString, CCommentItem*>::const_iterator it = m_mapComments.begin(); it != m_mapComments.end(); ++it)
				{
					delete it->second;
				}

				m_mapComments.clear();

				// delete Conditional Formatting
				m_arrConditionalFormatting.clear();
			}
		private:
			CPath													m_oReadPath;

		public:
			nullable<OOX::Spreadsheet::CCols>						m_oCols;
			nullable<OOX::Spreadsheet::CDimension>					m_oDimension;
			nullable<OOX::Spreadsheet::CDrawingWorksheet>			m_oDrawing;
			nullable<OOX::Spreadsheet::CHyperlinks>					m_oHyperlinks;
			nullable<OOX::Spreadsheet::CMergeCells>					m_oMergeCells;
			nullable<OOX::Spreadsheet::CSheetData>					m_oSheetData;
			nullable<OOX::Spreadsheet::CSheetFormatPr>				m_oSheetFormatPr;
			nullable<OOX::Spreadsheet::CSheetViews>					m_oSheetViews;
			nullable<OOX::Spreadsheet::CPageMargins>				m_oPageMargins;
			nullable<OOX::Spreadsheet::CPageSetup>					m_oPageSetup;
			nullable<OOX::Spreadsheet::CPrintOptions>				m_oPrintOptions;
			nullable<OOX::Spreadsheet::CAutofilter>					m_oAutofilter;
			nullable<OOX::Spreadsheet::CTableParts>					m_oTableParts;
			nullable<OOX::Spreadsheet::CLegacyDrawingWorksheet>		m_oLegacyDrawingWorksheet;
			nullable<OOX::Spreadsheet::COleObjects>					m_oOleObjects;
			std::map<CString, CCommentItem*>						m_mapComments;
			std::vector<OOX::Spreadsheet::CConditionalFormatting*>	m_arrConditionalFormatting;
			nullable<OOX::Spreadsheet::CSheetPr>					m_oSheetPr;

			nullable<OOX::Drawing::COfficeArtExtensionList>			m_oExtLst;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_WORKSHEET_FILE_INCLUDE_H_
