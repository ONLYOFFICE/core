/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "Drawing.h"
#include "Pos.h"
#include "../../../MsBinaryFile/XlsFile/Format/Binary/CFStreamCacheWriter.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_unions/OBJECTS.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_unions/MSODRAWINGGROUP.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_unions/OBJ.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/MsoDrawing.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/MsoDrawingGroup.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Obj.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/WorksheetSubstream.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/GlobalsSubstream.h"
#include "../../PPTXFormat/Logic/Shape.h"
#include "../Chart/Chart.h"

namespace OOX
{
	using namespace Drawing;

	namespace Spreadsheet
	{
		CDrawingWorksheet::CDrawingWorksheet()
		{
		}
		CDrawingWorksheet::~CDrawingWorksheet()
		{
		}
		void CDrawingWorksheet::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CDrawingWorksheet::toXML() const
		{
			return (L"");
		}
		void CDrawingWorksheet::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if(m_oId.IsInit())
			{
				writer.WriteString(L"<drawing r:id=\"");
				writer.WriteString(m_oId->ToString());
				writer.WriteString(L"\"/>");
			}

		}
		void CDrawingWorksheet::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CDrawingWorksheet::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CDrawingWorksheet::toBin()
		{
			auto castedPtr(new XLSB::Drawing);
			XLS::BaseObjectPtr ptr(castedPtr);
				if(m_oId.IsInit())
				{
					if(!m_oId->GetValue().empty())
						castedPtr->stRelId.value = m_oId->GetValue();
				}
			return ptr;
		}
        void CDrawingWorksheet::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            auto record = writer->getNextRecord(XLSB::rt_Drawing);
            XLSB::RelID stRelId;
            if(m_oId.IsInit())
                stRelId = m_oId->GetValue();
            else
                stRelId.value.setSize(0xFFFFFFFF);
            *record << stRelId;
            writer->storeNextRecord(record);
        }
		EElementType CDrawingWorksheet::getType () const
		{
			return et_x_FromTo;
		}
		void CDrawingWorksheet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS ( oReader )
				WritingElement_ReadAttributes_Read_if ( oReader, L"id", m_oId )
			WritingElement_ReadAttributes_End_No_NS ( oReader )
		}
		void CDrawingWorksheet::ReadAttributes(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::Drawing*>(obj.get());
				if(ptr != nullptr)
				{
					if(!ptr->stRelId.value.value().empty())
						m_oId = ptr->stRelId.value.value();
				}
			}

		CDrawing::CDrawing(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bSpreadsheets = true;
		}
		CDrawing::CDrawing(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bSpreadsheets = true;
			read( oRootPath, oPath );
		}
		CDrawing::~CDrawing()
		{
		}
		void CDrawing::read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		void CDrawing::read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;
			IFileContainer::Read( oRootPath, oPath );

			XmlUtils::CXmlLiteReader oReader;

			if ( !oReader.FromFile( oPath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if ( (L"wsDr") == sName )
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
				{
					int nCurDepth = oReader.GetDepth();
					while ( oReader.ReadNextSiblingNode( nCurDepth ) )
					{
						sName = XmlUtils::GetNameNoNS(oReader.GetName());

						CCellAnchor *pItem = NULL;

						if ( (L"absoluteAnchor") == sName )
						{
							pItem = new CCellAnchor();
							*pItem = oReader;
							pItem->m_oAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorAbsolute);
						}
						else if ( (L"oneCellAnchor") == sName )
						{
							pItem = new CCellAnchor();
							*pItem = oReader;
							pItem->m_oAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorOneCell);
						}
						else if ( (L"twoCellAnchor") == sName )
						{
							pItem = new CCellAnchor();
							*pItem = oReader;
							pItem->m_oAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorTwoCell);
						}
						else if ( (L"AlternateContent") == sName)
						{
							nCurDepth++;
							while( oReader.ReadNextSiblingNode( nCurDepth ) )
							{
								sName = XmlUtils::GetNameNoNS(oReader.GetName());
								if ( (L"Choice") != sName  && (L"Fallback") != sName ) continue;

								nullable<std::wstring> sRequires;
								WritingElement_ReadAttributes_Start( oReader )
									WritingElement_ReadAttributes_Read_if ( oReader, (L"Requires"), sRequires )
								WritingElement_ReadAttributes_End( oReader )

								if (sRequires.IsInit() == false) continue;
								if (*sRequires != (L"a14"))continue;

								nCurDepth++;
								while( oReader.ReadNextSiblingNode( nCurDepth ) )
								{
									sName = XmlUtils::GetNameNoNS(oReader.GetName());
									if ( (L"absoluteAnchor") == sName )
									{
										pItem = new CCellAnchor();
										*pItem = oReader;
										pItem->m_oAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorAbsolute);
									}
									else if ( (L"oneCellAnchor") == sName )
									{
										pItem = new CCellAnchor();
										*pItem = oReader;
										pItem->m_oAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorOneCell);
									}
									else if ( (L"twoCellAnchor") == sName )
									{
										pItem = new CCellAnchor();
										*pItem = oReader;
										pItem->m_oAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorTwoCell);
									}
								}nCurDepth--;
								break;
							}
							nCurDepth--;
						}

						if (pItem)
						{
							m_arrItems.push_back(pItem);

							if (pItem->m_nId.IsInit() && pItem->m_oElement.is_init())
							{
								m_mapShapes.insert(std::make_pair(*pItem->m_nId, pItem->m_oElement->GetElem().GetPointer()));
							}
						}
					}
				}
			}
		}
		void CDrawing::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			NSStringUtils::CStringBuilder sXml;
			sXml.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><xdr:wsDr \
xmlns:xdr=\"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing\" \
xmlns:xdr14=\"http://schemas.microsoft.com/office/excel/2010/spreadsheetDrawing\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">"));
			for(size_t i = 0, length = m_arrItems.size(); i < length; ++i)
				m_arrItems[i]->toXML(sXml);
			sXml.WriteString((L"</xdr:wsDr>"));

			std::wstring sPath = oPath.GetPath();
			NSFile::CFileBinary::SaveToFile(sPath.c_str(), sXml.GetData());

			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			IFileContainer::Write(oPath, oDirectory, oContent);
		}
		void CDrawing::toXls(XLS::BaseObjectPtr GlobalsSubstream, XLS::BaseObjectPtr WorksheetStream)
		{
			auto worksheet = static_cast<XLS::WorksheetSubstream*>(WorksheetStream.get());
			auto workbook = static_cast<XLS::GlobalsSubstream*>(GlobalsSubstream.get());
			XLS::OBJECTS *wsObjects;


			XLS::MsoDrawingGroup* drawingGroupPtr;

			if(workbook->m_arMSODRAWINGGROUP.empty())
			{
				drawingGroupPtr = new XLS::MsoDrawingGroup;
				auto drawingGroupUnion = new XLS::MSODRAWINGGROUP(false);
				drawingGroupUnion->m_MsoDrawingGroup = XLS::BaseObjectPtr(drawingGroupPtr);
				workbook->m_arMSODRAWINGGROUP.push_back(XLS::BaseObjectPtr(drawingGroupUnion));
			}
			else
			{
				auto drawingGroupUnion = static_cast<XLS::MSODRAWINGGROUP*>(workbook->m_arMSODRAWINGGROUP.back().get());
				drawingGroupPtr = static_cast<XLS::MsoDrawingGroup*>(drawingGroupUnion->m_MsoDrawingGroup.get());
			}

			if(worksheet->m_OBJECTS == nullptr)
			{
				wsObjects = new XLS::OBJECTS(false);
				worksheet->m_OBJECTS = XLS::BaseObjectPtr(wsObjects);
			}
			else
				wsObjects = static_cast<XLS::OBJECTS*>(worksheet->m_OBJECTS.get());
			for(auto anchor : m_arrItems)
			{

				auto drawing = new XLS::MsoDrawing(false);
				auto drawingPtr = XLS::MsoDrawingPtr(drawing);
				if(!wsObjects->m_arrObject.empty())
					drawing->rgChildRec.first = false;
				else
					wsObjects->m_MsoDrawing = drawingPtr;
				if(anchor->m_oElement->is<PPTX::Logic::Pic>())
				{

					auto anchorElem = anchor->m_oElement->GetElem();
					auto picElem =  static_cast<PPTX::Logic::Pic*>(anchorElem.GetPointer());
					auto picRid =  picElem->blipFill.blip->embed->get();
					if(picRid.empty())
						continue;
					auto castedPic = Get<OOX::Media>(picRid);
					if(!castedPic->IsExist())
						continue;
					auto pictName = castedPic->filename();
					auto PicNumber = drawingGroupPtr->AddPict(pictName);

					auto shapeCount = drawingGroupPtr->drawingCount+1;
					auto left = 0, leftOff = 0, right = 0, righOff = 0, top = 0, topOff = 0, bot = 0, botOff = 0;
					anchor->getAnchorPos(left, leftOff, top, topOff, right, righOff, bot, botOff);
					drawing->prepareDrawing(XLS::MsoDrawing::DrawingType::pic, shapeCount, top, left, bot, right, leftOff,
					righOff, topOff, botOff,  PicNumber);
					std::pair<XLS::BaseObjectPtr, std::vector<XLS::BaseObjectPtr>> objPair;
					auto objPt = new XLS::Obj(drawingPtr);
					objPt->cmo.ot = 0x8;
					objPt->cmo.id = drawingGroupPtr->drawingCount+1;
					objPt->cmo.fPrint = true;
					objPair.first = drawingPtr;
					objPair.second.push_back(XLS::BaseObjectPtr(objPt));
					wsObjects->m_arrObject.push_back(objPair);
					drawingGroupPtr->drawingCount++;
				}
				else if(anchor->m_oElement->is<PPTX::Logic::GraphicFrame>())
				{
					auto graphicFrame =  static_cast<PPTX::Logic::GraphicFrame*>(anchor->m_oElement->GetElem().GetPointer());
					if(graphicFrame->chartRec.IsInit() && graphicFrame->chartRec->id_data.IsInit())
					{
						auto chartRid = graphicFrame->chartRec->id_data.get();
						auto castedChart = Get<OOX::File>(chartRid);
						auto ChartFile = static_cast<OOX::Spreadsheet::CChartFile*>(castedChart.GetPointer());
						if(!(ChartFile->type() == OOX::FileTypes::Chart))
							continue;
						if(ChartFile->m_oChartSpace.m_chart == nullptr)
							continue;
						auto xlsChart = ChartFile->m_oChartSpace.toXLS();
						auto shapeCount = drawingGroupPtr->drawingCount+1;

						auto drawingObj = new XLS::MsoDrawing(false);
						{
							auto left = 0, leftOff = 0, right = 0, righOff = 0, top = 0, topOff = 0, bot = 0, botOff = 0;
							anchor->getAnchorPos(left, leftOff, top, topOff, right, righOff, bot, botOff);
							if(!wsObjects->m_arrObject.empty())
								drawingObj->rgChildRec.first = false;
							drawingObj->prepareDrawing(XLS::MsoDrawing::DrawingType::chart, shapeCount, top, left, bot, right,
							leftOff, righOff, topOff, botOff);
						}

						std::pair<XLS::BaseObjectPtr, std::vector<XLS::BaseObjectPtr>> objPair;
						{
							auto drawingObjPtr = XLS::MsoDrawingPtr(drawingObj);
							objPair.first = drawingObjPtr;

							if(drawingObj->rgChildRec.first)
								wsObjects->m_MsoDrawing = drawingObjPtr;
						}
						auto objPt = new XLS::Obj(wsObjects->m_MsoDrawing);
						objPt->cmo.ot = 5;
						objPt->cmo.fPrint = true;
						objPt->cmo.fRecalcObj = true;
						objPt->cmo.id = drawingGroupPtr->drawingCount;

						auto objUnion = new XLS::OBJ(wsObjects->m_MsoDrawing);
						objUnion->m_Obj = XLS::BaseObjectPtr(objPt);

						objUnion->m_arrChart.push_back(xlsChart);
						objPair.second.push_back(XLS::BaseObjectPtr(objUnion));
						wsObjects->m_arrObject.push_back(objPair);
						drawingGroupPtr->drawingCount++;
					}
				}
			}
		}
		const OOX::FileType CDrawing::type() const
		{
			return OOX::Spreadsheet::FileTypes::Drawings;
		}
		const CPath CDrawing::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		const CPath CDrawing::DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& CDrawing::GetReadPath()
		{
			return m_oReadPath;
		}
		bool CDrawing::IsEmpty()
		{
			return m_arrItems.empty();
		}
		void CDrawing::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}
		void CDrawing::ClearItems()
		{
			m_mapShapes.clear();
			for ( unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
			{
				if ( m_arrItems[nIndex] )
					delete m_arrItems[nIndex];
				
				m_arrItems[nIndex] = NULL;
			}
			m_arrItems.clear();
		}

	} //Spreadsheet
} // namespace OOX
