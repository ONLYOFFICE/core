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

#include "SmartArt.h"
#include "../Slide.h"
#include "../SlideLayout.h"
#include "../SlideMaster.h"

#include "../../PPTXFormat/DrawingConverter/ASCOfficeDrawingConverter.h"

#include "../../Binary/Sheets/Reader/BinaryWriter.h"

#include "../../Binary/Document/DocWrapper/XlsxSerializer.h"
#include "../../Binary/Document/DocWrapper/FontProcessor.h"
#include "../../Binary/Document/BinWriter/BinWriters.h"

#include "../../DocxFormat/Diagram/DiagramData.h"
#include "../../DocxFormat/Diagram/DiagramDrawing.h"
#include "../../DocxFormat/Diagram/DiagramColors.h"
#include "../../DocxFormat/Diagram/DiagramQuickStyle.h"
#include "../../DocxFormat/Diagram/DiagramLayout.h"

#include "../../../Common/OfficeFileFormatChecker.h"

#include "../../../OfficeUtils/src/OfficeUtils.h"
namespace PPTX
{
	namespace Logic
	{
		SmartArt::SmartArt()
		{
		}
		SmartArt& SmartArt::operator=(const SmartArt& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			return *this;
		}
		OOX::EElementType SmartArt::getType () const
		{
			return OOX::et_dgm_DiagrammParts;
		}
		void SmartArt::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
		}
		void SmartArt::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"r:dm", id_data);
			XmlMacroReadAttributeBase(node, L"r:cs", id_color);
			XmlMacroReadAttributeBase(node, L"r:lo", id_layout);
			XmlMacroReadAttributeBase(node, L"r:qs", id_style);
		}
		smart_ptr<OOX::File> SmartArt::FindDiagramDrawing(OOX::CDiagramData* pDiagramData) const
		{
			if (!pDiagramData) return NULL;

			// easy4cargo1.pptx - слайд 2 - в диаграмме Smart вместо ссылки на drawing.xml ссылка на стороннюю картинку
			OOX::CPath pathDiagramData = pDiagramData->m_strFilename;

			int a1 = (int)pathDiagramData.GetFilename().find(L".");
			std::wstring strId = pathDiagramData.GetFilename().substr(4, pathDiagramData.GetFilename().length() - 8);
			//стандартизированные имена only

			OOX::CPath pathDiagramDrawing = pathDiagramData.GetDirectory() + FILE_SEPARATOR_STR + L"drawing" + strId + L".xml";

			return smart_ptr<OOX::File>(dynamic_cast<OOX::File*>(new OOX::CDiagramDrawing(NULL, pathDiagramDrawing)));
		}
		bool SmartArt::LoadDrawing(OOX::IFileContainer* pRels)
		{
			if (m_oDrawing.IsInit())
				return true;

			if (id_data.IsInit() == false) return false;
			if (pRels == NULL) return false;

			smart_ptr<OOX::File> oFileData = pRels->Find(*id_data);
			OOX::CDiagramData* pDiagramData = dynamic_cast<OOX::CDiagramData*>(oFileData.GetPointer());

			if (!pDiagramData) return false;

			// это smart art ..есть у него drawing или нет - неважно
			smart_ptr<OOX::File> oFileDrawing;
			OOX::CDiagramDrawing* pDiagramDrawing = NULL;

			if (pDiagramData->id_drawing.IsInit())
			{
				if (parentFileIs<OOX::IFileContainer>()) oFileDrawing = parentFileAs<OOX::IFileContainer>().Find(*pDiagramData->id_drawing);
				else if (pRels != NULL) oFileDrawing = pRels->Find(*pDiagramData->id_drawing);
			}
			else
			{
				//Monetizing_Innovation.pptx (слайд 13) - diagrams/data1.xml не ссылается на diagrams/drawing1.xml
				//пробуем по тому же пути с номером data.xml - ниже			
			}
			pDiagramDrawing = dynamic_cast<OOX::CDiagramDrawing*>(oFileDrawing.GetPointer());

			if (!pDiagramDrawing)
			{
				oFileDrawing = FindDiagramDrawing(pDiagramData);
				pDiagramDrawing = dynamic_cast<OOX::CDiagramDrawing*>(oFileDrawing.GetPointer());
			}

			if ((pDiagramDrawing) && (pDiagramDrawing->m_oShapeTree.IsInit()))
			{
				m_oDrawing = pDiagramDrawing->m_oShapeTree;
				FillParentPointersForChilds();

				m_pDrawingContainer = oFileDrawing.smart_dynamic_cast<OOX::IFileContainer>();

				if (!m_oDrawing->grpSpPr.xfrm.IsInit())
					m_oDrawing->grpSpPr.xfrm = new PPTX::Logic::Xfrm;
			}
			else
			{
				//parse pDiagramData !!
			}
			return true;
		}
		void SmartArt::LoadDrawing(NSBinPptxRW::CBinaryFileWriter* pWriter)
		{
			if (m_oDrawing.IsInit())
				return;

			OOX::IFileContainer	& pRelsPPTX = parentFileAs<OOX::IFileContainer>();
			OOX::IFileContainer	* pRels = NULL;

			if (pWriter)
			{
				pRels = pWriter->GetRels().GetPointer();
			}

			bool result = LoadDrawing(&pRelsPPTX);
			if (!result)
				result = LoadDrawing(pRels);
		}
		void SmartArt::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			NSCommon::smart_ptr<OOX::IFileContainer> documentContainer = pWriter->GetRels();
			OOX::IFileContainer* pDocumentRels = documentContainer.is_init() ? documentContainer.GetPointer() : NULL;

			BinDocxRW::CDocxSerializer *main_document = pWriter->m_pMainDocument;
			pWriter->m_pMainDocument = NULL;
			if (id_data.IsInit())
			{
				smart_ptr<OOX::File> oFileData;

				if (parentFileIs<OOX::IFileContainer>()) oFileData = parentFileAs<OOX::IFileContainer>().Find(*id_data);
				else if (pDocumentRels != NULL) oFileData = pDocumentRels->Find(*id_data);

				OOX::CDiagramData* pDiagramData = dynamic_cast<OOX::CDiagramData*>(oFileData.GetPointer());

				if (pDiagramData)
				{
					OOX::CDiagramDrawing* pDiagramDrawing = NULL;
					smart_ptr<OOX::File> oFileDrawing;

					if (pDiagramData->id_drawing.IsInit())
					{
						if (parentFileIs<OOX::IFileContainer>()) oFileDrawing = parentFileAs<OOX::IFileContainer>().Find(*pDiagramData->id_drawing);
						else if (pDocumentRels != NULL) oFileDrawing = pDocumentRels->Find(*pDiagramData->id_drawing);

						pDiagramDrawing = dynamic_cast<OOX::CDiagramDrawing*>(oFileDrawing.GetPointer());

					}
					if (!pDiagramDrawing)
					{
						//Demo-Hayden-Management-v2.docx 
						oFileDrawing = FindDiagramDrawing(pDiagramData);
						pDiagramDrawing = dynamic_cast<OOX::CDiagramDrawing*>(oFileDrawing.GetPointer());
					}

					if (pDiagramDrawing)
					{
						pWriter->SetRels(dynamic_cast<OOX::IFileContainer*>(oFileDrawing.GetPointer()));

						pWriter->StartRecord(0);
						pDiagramDrawing->toPPTY(pWriter);
						pWriter->EndRecord();
					}

					pWriter->SetRels(dynamic_cast<OOX::IFileContainer*>(oFileData.GetPointer()));

					pWriter->StartRecord(1);
					pDiagramData->toPPTY(pWriter);
					pWriter->EndRecord();
				}
			}

			if (id_color.IsInit())
			{
				smart_ptr<OOX::File> oFileColors;

				if (parentFileIs<OOX::IFileContainer>()) oFileColors = parentFileAs<OOX::IFileContainer>().Find(*id_color);
				else if (pDocumentRels != NULL) oFileColors = pDocumentRels->Find(*id_color);

				OOX::CDiagramColors* pDiagramColors = dynamic_cast<OOX::CDiagramColors*>(oFileColors.GetPointer());

				if (pDiagramColors)
				{
					pWriter->SetRels(dynamic_cast<OOX::IFileContainer*>(oFileColors.GetPointer()));

					pWriter->StartRecord(2);
					pDiagramColors->toPPTY(pWriter);
					pWriter->EndRecord();
				}
			}
			if (id_layout.IsInit())
			{
				smart_ptr<OOX::File> oFileLayout;

				if (parentFileIs<OOX::IFileContainer>()) oFileLayout = parentFileAs<OOX::IFileContainer>().Find(*id_layout);
				else if (pDocumentRels != NULL) oFileLayout = pDocumentRels->Find(*id_layout);

				OOX::CDiagramLayout* pDiagramLayout = dynamic_cast<OOX::CDiagramLayout*>(oFileLayout.GetPointer());

				if (pDiagramLayout)
				{
					pWriter->SetRels(dynamic_cast<OOX::IFileContainer*>(oFileLayout.GetPointer()));

					pWriter->StartRecord(3);
					pDiagramLayout->toPPTY(pWriter);
					pWriter->EndRecord();
				}
			}
			if (id_style.IsInit())
			{
				smart_ptr<OOX::File> oFileStyle;

				if (parentFileIs<OOX::IFileContainer>()) oFileStyle = parentFileAs<OOX::IFileContainer>().Find(*id_style);
				else if (pDocumentRels != NULL) oFileStyle = pDocumentRels->Find(*id_style);

				OOX::CDiagramQuickStyle* pDiagramStyle = dynamic_cast<OOX::CDiagramQuickStyle*>(oFileStyle.GetPointer());
				if (pDiagramStyle)
				{
					pWriter->SetRels(dynamic_cast<OOX::IFileContainer*>(oFileStyle.GetPointer()));

					pWriter->StartRecord(4);
					pDiagramStyle->toPPTY(pWriter);
					pWriter->EndRecord();
				}
			}
			pWriter->SetRels(documentContainer);
			pWriter->m_pMainDocument = main_document;
		}
		void SmartArt::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			int mainDocumentType = pReader->m_nDocumentType;
			pReader->m_nDocumentType = pReader->m_pRels->m_pManager->m_nDocumentType = XMLWRITER_DOC_TYPE_DIAGRAM;

			int nCountDiagram = pReader->m_nCountDiagram;
			pReader->m_nCountDiagram++;
			
			std::wstring strDstDiagram = pReader->m_pRels->m_pManager->GetDstDiagram();

			std::wstring contenttype_override_path = (mainDocumentType == XMLWRITER_DOC_TYPE_DOCX) ? L"/word/diagrams" :
													((mainDocumentType == XMLWRITER_DOC_TYPE_XLSX) ? L"/xl/diagrams" : L"/ppt/diagrams");
			
			std::wstring rels_path = (mainDocumentType == XMLWRITER_DOC_TYPE_DOCX) ? L"diagrams/" : L"../diagrams/";
			
			if (strDstDiagram.empty())
			{
				strDstDiagram = pReader->m_pRels->m_pManager->GetDstFolder() + FILE_SEPARATOR_STR + L"diagrams";
				pReader->m_pRels->m_pManager->SetDstCharts(strDstDiagram);
			}
			NSDirectory::CreateDirectory(strDstDiagram);
			NSDirectory::CreateDirectory(strDstDiagram  + FILE_SEPARATOR_STR + L"_rels");
			//--------------------------------------------------------------------------------------------------			
			smart_ptr<OOX::CDiagramDrawing> pDiagramDrawing;

			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						pReader->SetDstContentRels();

						pDiagramDrawing = new OOX::CDiagramDrawing(NULL);
						pDiagramDrawing->m_sOutputFilename = L"drawing" + std::to_wstring(nCountDiagram) + L".xml";
						pDiagramDrawing->fromPPTY(pReader);

						pDiagramDrawing->write(strDstDiagram + FILE_SEPARATOR_STR + pDiagramDrawing->m_sOutputFilename, contenttype_override_path, *pReader->m_pRels->m_pManager->m_pContentTypes);
						pReader->SaveDstContentRels(strDstDiagram + FILE_SEPARATOR_STR + L"_rels" + FILE_SEPARATOR_STR + L"drawing" + std::to_wstring(nCountDiagram) + L".xml.rels");

						pDiagramDrawing->m_sOutputFilename = rels_path + pDiagramDrawing->m_sOutputFilename;
					}break;
					case 1:
					{
						pReader->SetDstContentRels();

						smart_ptr<OOX::CDiagramData> pDiagramData = new OOX::CDiagramData(NULL);
						pDiagramData->m_sOutputFilename = L"data" + std::to_wstring(nCountDiagram) + L".xml";

						pDiagramData->fromPPTY(pReader);	

						if (pDiagramDrawing.IsInit())
						{
							unsigned int nRId = pReader->m_pRels->WriteRels(pDiagramDrawing->type().RelationType(), pDiagramDrawing->m_sOutputFilename, L"");
							pDiagramData->id_drawing = new OOX::RId(nRId);
							
							pDiagramData->m_oDataModel->m_oExtLst.Init();
							pDiagramData->m_oDataModel->m_oExtLst->m_arrExt.push_back(new OOX::Drawing::COfficeArtExtension());
							pDiagramData->m_oDataModel->m_oExtLst->m_arrExt[0]->m_oDataModelExt.Init();

							pDiagramData->m_oDataModel->m_oExtLst->m_arrExt[0]->m_sUri = L"http://schemas.microsoft.com/office/drawing/2008/diagram";
							pDiagramData->m_oDataModel->m_oExtLst->m_arrExt[0]->m_oDataModelExt->m_oRelId = pDiagramData->id_drawing->ToString();
						}
						pDiagramData->write(strDstDiagram + FILE_SEPARATOR_STR + pDiagramData->m_sOutputFilename, contenttype_override_path, *pReader->m_pRels->m_pManager->m_pContentTypes);

						pReader->SaveDstContentRels(strDstDiagram + FILE_SEPARATOR_STR + L"_rels" + FILE_SEPARATOR_STR + pDiagramData->m_sOutputFilename + L".rels");
						pDiagramData->m_sOutputFilename = rels_path + pDiagramData->m_sOutputFilename;

						unsigned int nRId = pReader->m_pRels->WriteRels(pDiagramData->type().RelationType(), pDiagramData->m_sOutputFilename, L"");
						id_data = new OOX::RId(nRId);		

						if (pDiagramDrawing.IsInit())
						{
							nRId = pReader->m_pRels->WriteRels(pDiagramDrawing->type().RelationType(), pDiagramDrawing->m_sOutputFilename, L"");
						}
					}break;
					case 2:
					{
						smart_ptr<OOX::CDiagramColors> pDiagramColors = new OOX::CDiagramColors(NULL);
						pDiagramColors->m_sOutputFilename = L"colors" + std::to_wstring(nCountDiagram) + L".xml";
						pDiagramColors->fromPPTY(pReader);
						
						pDiagramColors->write(strDstDiagram + FILE_SEPARATOR_STR + pDiagramColors->m_sOutputFilename, contenttype_override_path, *pReader->m_pRels->m_pManager->m_pContentTypes);

						pDiagramColors->m_sOutputFilename = rels_path + pDiagramColors->m_sOutputFilename;

						unsigned int nRId = pReader->m_pRels->WriteRels(pDiagramColors->type().RelationType(), pDiagramColors->m_sOutputFilename, L"");
						id_color = new OOX::RId(nRId);
					}break;
					case 3:
					{
						smart_ptr<OOX::CDiagramLayout> pDiagramLayout = new OOX::CDiagramLayout(NULL);
						pDiagramLayout->m_sOutputFilename = L"layout" + std::to_wstring(nCountDiagram) + L".xml";
						pDiagramLayout->fromPPTY(pReader);

						pDiagramLayout->write(strDstDiagram + FILE_SEPARATOR_STR + pDiagramLayout->m_sOutputFilename, contenttype_override_path, *pReader->m_pRels->m_pManager->m_pContentTypes);

						pDiagramLayout->m_sOutputFilename = rels_path + pDiagramLayout->m_sOutputFilename;

						unsigned int nRId = pReader->m_pRels->WriteRels(pDiagramLayout->type().RelationType(), pDiagramLayout->m_sOutputFilename, L"");
						id_layout = new OOX::RId(nRId);
					}break;
					case 4:
					{
						smart_ptr<OOX::CDiagramQuickStyle> pDiagramStyle = new OOX::CDiagramQuickStyle(NULL);
						pDiagramStyle->m_sOutputFilename = L"quickStyle" + std::to_wstring(nCountDiagram) + L".xml";
						pDiagramStyle->fromPPTY(pReader);
						
						pDiagramStyle->write(strDstDiagram + FILE_SEPARATOR_STR + pDiagramStyle->m_sOutputFilename, contenttype_override_path, *pReader->m_pRels->m_pManager->m_pContentTypes);

						pDiagramStyle->m_sOutputFilename = rels_path + pDiagramStyle->m_sOutputFilename;

						unsigned int nRId = pReader->m_pRels->WriteRels(pDiagramStyle->type().RelationType(), pDiagramStyle->m_sOutputFilename, L"");
						id_style = new OOX::RId(nRId);
					}break;

					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(end);

			pReader->m_nDocumentType = pReader->m_pRels->m_pManager->m_nDocumentType = mainDocumentType;
		}
		std::wstring SmartArt::toXML() const
		{
			if (!id_data.is_init())
				return L"";

			std::wstring strData = L"<dgm:relIds xmlns:dgm=\"http://schemas.openxmlformats.org/drawingml/2006/diagram\"";
			strData += L" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\"";
			strData += L" r:dm=\"" + id_data->ToString() + L"\"";
			if (id_layout.IsInit()) strData += L" r:lo=\"" + id_layout->ToString() + L"\"";
			if (id_style.IsInit()) strData += L" r:qs=\"" + id_style->ToString() + L"\"";
			if (id_color.IsInit()) strData += L" r:cs=\"" + id_color->ToString() + L"\"";
			strData += L"/>";

			return strData;
		}
		void SmartArt::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void SmartArt::FillParentPointersForChilds()
		{
		}
		void SmartArt::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"r:cs"), id_color)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"r:dm"), id_data)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"r:lo"), id_layout)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"r:qs"), id_style)
			WritingElement_ReadAttributes_End(oReader)
		}

		ChartRec::ChartRec()
		{
		}
		ChartRec::ChartRec(const ChartRec& oSrc)
		{
			*this = oSrc;
		}
		ChartRec& ChartRec::operator=(const ChartRec& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			return *this;
		}
		void ChartRec::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_ReadSingle ( oReader, L"id", id_data )
			WritingElement_ReadAttributes_End_No_NS	( oReader )
		}
		void ChartRec::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring ns = XmlUtils::GetNamespace(oReader.GetName());

			m_bChartEx = false;

			ReadAttributes( oReader );
			FillParentPointersForChilds();

			if (ns == L"cx")
			{
				m_bChartEx = true;
			}
		}
		void ChartRec::fromXML(XmlUtils::CXmlNode& node)
		{
			m_bChartEx = false;

			std::wstring ns = XmlUtils::GetNamespace(node.GetName());

			XmlMacroReadAttributeBase(node, L"r:id", id_data);
			FillParentPointersForChilds();

			if (ns == L"cx")
			{
				m_bChartEx = true;
			}
		}
		void ChartRec::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			OOX::IFileContainer* pRels = pWriter->GetRels().GetPointer();

			smart_ptr<OOX::File> file;
			if(id_data.IsInit())
			{
				if(parentFileIs<OOX::IFileContainer>())	file = parentFileAs<OOX::IFileContainer>().Find(*id_data);
				else if (pRels != NULL)					file = pRels->Find(*id_data);
			}
			smart_ptr<OOX::Spreadsheet::CChartFile> pChart = file.smart_dynamic_cast<OOX::Spreadsheet::CChartFile>();
			smart_ptr<OOX::Spreadsheet::CChartExFile> pChartEx = file.smart_dynamic_cast<OOX::Spreadsheet::CChartExFile>();

			if (false == pChart.IsInit() && false == pChartEx.IsInit()) return;		

	//----------------------------------------------------------------
			std::wstring id;
			if ((pChart.IsInit()) && (pChart->m_oChartSpace.m_externalData) && (pChart->m_oChartSpace.m_externalData->m_id))
				id = *pChart->m_oChartSpace.m_externalData->m_id;
			else if ((pChartEx.IsInit()) && (true == pChartEx->m_oChartSpace.m_chartData.m_externalData.IsInit()))
				id = pChartEx->m_oChartSpace.m_chartData.m_externalData->m_id.get_value_or(L"");
			
			if (false == id.empty())
			{
				smart_ptr<OOX::File> oFile = pChart.IsInit() ? pChart->Find(id) : pChartEx->Find(id);
				smart_ptr<OOX::Media> oMediaFile = oFile.smart_dynamic_cast<OOX::Media>();

				if (oMediaFile.IsInit())
				{
					OOX::CPath oox_file = oMediaFile->filename();
					OOX::CPath embed_folder = oox_file.GetDirectory(true);
					OOX::CPath oox_unpacked = embed_folder + L"Temp_unpacked";
					NSDirectory::CreateDirectory(oox_unpacked.GetPath());

					COfficeUtils oOfficeUtils(NULL);
					oOfficeUtils.ExtractToDirectory(oox_file.GetPath(), oox_unpacked.GetPath(), NULL, 0);

					COfficeFileFormatChecker office_checker;
					office_checker.isOOXFormatFile(oox_file.GetPath());

					if (office_checker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX ||
						office_checker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM)
					{
						DocWrapper::FontProcessor oFontProcessor;
						NSBinPptxRW::CDrawingConverter oDrawingConverter;

						NSCommon::smart_ptr<OOX::IFileContainer>	old_rels = pWriter->GetRels();
						NSCommon::smart_ptr<PPTX::Theme>            old_theme = *pWriter->m_pTheme;

						NSShapeImageGen::CMediaManager* old_manager = oDrawingConverter.m_pBinaryWriter->m_pCommon->m_pMediaManager;
						oDrawingConverter.m_pBinaryWriter->m_pCommon->m_pMediaManager = pWriter->m_pCommon->m_pMediaManager;

						oDrawingConverter.SetFontPicker(pWriter->m_pCommon->m_pFontPicker);

//----------------------------
						BinXlsxRW::BinaryFileWriter xlsxBinaryWriter(oFontProcessor);
						OOX::Spreadsheet::CXlsx *pXlsxEmbedded = NULL;
						NSBinPptxRW::CXlsbBinaryWriter oXlsbWriter;

						if (office_checker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB)
							pXlsxEmbedded = new OOX::Spreadsheet::CXlsb();
						else
							pXlsxEmbedded = new OOX::Spreadsheet::CXlsx();

						//startheader for test
						//oXlsbWriter.WriteStringUtf8(xlsxBinaryWriter.WriteFileHeader(0, BinXlsxRW::g_nFormatVersionNoBase64));
						oXlsbWriter.WriteReserved(xlsxBinaryWriter.GetMainTableSize());
						unsigned int nXlsbWriterStartPos = oXlsbWriter.GetPositionAbsolute();

						pXlsxEmbedded->m_pXlsbWriter = &oXlsbWriter;
						pXlsxEmbedded->m_bNeedCalcChain = false;

						pXlsxEmbedded->Read(oox_unpacked);
						pXlsxEmbedded->PrepareWorkbook();

						unsigned int nXlsbWriterEndPos = oXlsbWriter.GetPositionAbsolute() ;

						if (office_checker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB)
						{
							dynamic_cast<OOX::Spreadsheet::CXlsb*>(pXlsxEmbedded)->PrepareSi();
							dynamic_cast<OOX::Spreadsheet::CXlsb*>(pXlsxEmbedded)->PrepareTableFormula();
                            dynamic_cast<OOX::Spreadsheet::CXlsb*>(pXlsxEmbedded)->ReadSheetData();
						}
						//startheader for test
						//oDrawingConverter.m_pBinaryWriter->WriteStringUtf8(xlsxBinaryWriter.WriteFileHeader(0, BinXlsxRW::g_nFormatVersionNoBase64));
						xlsxBinaryWriter.WriteMainTableStart(*oDrawingConverter.m_pBinaryWriter);

						if (nXlsbWriterEndPos  > nXlsbWriterStartPos)
						{
							xlsxBinaryWriter.WriteBinaryTable(oXlsbWriter.GetBuffer() + nXlsbWriterStartPos, nXlsbWriterEndPos - nXlsbWriterStartPos);
						}
						xlsxBinaryWriter.WriteContent(pXlsxEmbedded, NULL, &oDrawingConverter);
						xlsxBinaryWriter.WriteMainTableEnd();

						pXlsxEmbedded->m_pXlsbWriter = NULL;

						delete pXlsxEmbedded;
//------------------------------
						pWriter->SetRels(old_rels);
						*pWriter->m_pTheme = old_theme;
						oDrawingConverter.m_pBinaryWriter->m_pCommon->m_pMediaManager = old_manager;
						
						pWriter->StartRecord(/*c_oserct_chartspaceXLSX*/16);
						
						BYTE* pbBinBuffer = oDrawingConverter.m_pBinaryWriter->GetBuffer();
						int nBinBufferLen = oDrawingConverter.m_pBinaryWriter->GetPosition();

						pWriter->WriteBYTEArray(pbBinBuffer, nBinBufferLen);

						pWriter->EndRecord();

						//for test
						//NSFile::CFileBinary oFile;
						//oFile.CreateFileW(L"d:\\Editor.bin");
						//oFile.WriteFile(pbBinBuffer, nBinBufferLen);
						//oFile.CloseFile();
					}

					NSDirectory::DeleteDirectory(oox_unpacked.GetPath());
				}
			}
	//----------------------------------------------------------------
			NSBinPptxRW::CDrawingConverter oDrawingConverter;
			NSBinPptxRW::CBinaryFileWriter *pOldDrawingWriter = oDrawingConverter.m_pBinaryWriter;
			BinDocxRW::CDocxSerializer *pOldMainDocument = pWriter->m_pMainDocument;
			
			pWriter->m_pMainDocument = NULL;
			oDrawingConverter.m_pBinaryWriter = pWriter;
			smart_ptr<OOX::IFileContainer> oldRels = oDrawingConverter.GetRels();
			oDrawingConverter.SetRels(file.smart_dynamic_cast<OOX::IFileContainer>());
		
			BinXlsxRW::BinaryChartWriter oBinaryChartWriter(*pWriter, &oDrawingConverter);	
			if (pChart.IsInit())
			{
				oBinaryChartWriter.WriteCT_ChartFile(*pChart);
			}
			else if (pChartEx.IsInit())
			{
				oBinaryChartWriter.WriteCT_ChartExFile(*pChartEx);
			}
	//----------------------------------------------------------------
			std::vector<smart_ptr<OOX::File>>& container = pChartEx.IsInit() ? pChartEx->GetContainer() : pChart->GetContainer();

			for (size_t i = 0; i < container.size(); ++i)
			{
				if (OOX::FileTypes::ThemeOverride == container[i]->type())
				{
					PPTX::Theme* pThemeOverride = dynamic_cast<PPTX::Theme*>(container[i].GetPointer());

					pWriter->StartRecord(/*c_oserct_chartspaceTHEMEOVERRIDE = */15);
					pThemeOverride->toPPTY(pWriter);
					pWriter->EndRecord();
					break;
				}
				else if (OOX::FileTypes::ChartStyle == container[i]->type())
				{
					OOX::Spreadsheet::CChartStyleFile* pChartStyle = dynamic_cast<OOX::Spreadsheet::CChartStyleFile*>(container[i].GetPointer());

					pWriter->StartRecord(/*c_oserct_chartspaceSTYLES = */17);
					oBinaryChartWriter.WriteCT_ChartStyle(*pChartStyle);
					pWriter->EndRecord();
				}
				else if (OOX::FileTypes::ChartColors == container[i]->type())
				{
					OOX::Spreadsheet::CChartColorsFile* pChartColors = dynamic_cast<OOX::Spreadsheet::CChartColorsFile*>(container[i].GetPointer());

					pWriter->StartRecord(/*c_oserct_chartspaceCOLORS = */18);
					oBinaryChartWriter.WriteCT_ChartColor(*pChartColors);
					pWriter->EndRecord();
				}
			}
	//----------------------------------------------------------------
			oDrawingConverter.SetRels(oldRels);
			
			oDrawingConverter.m_pBinaryWriter = pOldDrawingWriter;
			pWriter->m_pMainDocument = pOldMainDocument;

		}
		std::wstring ChartRec::toXML() const
		{
			if (!id_data.is_init())
				return L"";

			std::wstring strData;
			if (m_bChartEx)
			{
				strData = L"<cx:chart xmlns:cx=\"http://schemas.microsoft.com/office/drawing/2014/chartex\"";
			}
			else
			{
				strData = L"<c:chart xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\"";
			}
			strData += L" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" r:id=\"" + id_data->ToString() + L"\"/>";
			return strData;
		}
		void ChartRec::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void ChartRec::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			ULONG lLen = pReader->GetLong();

			m_nCountCharts = pReader->m_nCountCharts;
			pReader->m_nCountCharts++;

			BinXlsxRW::CXlsxSerializer		oXlsxSerializer;
			NSBinPptxRW::CDrawingConverter	oDrawingConverter;

			NSBinPptxRW::CImageManager2*	pOldImageManager	= oDrawingConverter.m_pImageManager;
			NSBinPptxRW::CBinaryFileReader* pOldReader			= oDrawingConverter.m_pReader;
 			
			oDrawingConverter.m_pImageManager = pReader->m_pRels->m_pManager;
			oDrawingConverter.m_pReader = pReader;

			oXlsxSerializer.setDrawingConverter(&oDrawingConverter);

			std::wstring strDstChart = pReader->m_pRels->m_pManager->GetDstFolder() + FILE_SEPARATOR_STR + L"charts";
			pReader->m_pRels->m_pManager->SetDstCharts(strDstChart);

			NSDirectory::CreateDirectory(strDstChart);

			smart_ptr<OOX::File> file;
			if (m_bChartEx)
			{
				smart_ptr<OOX::Spreadsheet::CChartExFile> chartEx = new OOX::Spreadsheet::CChartExFile(NULL);
				chartEx->SetGlobalNumber(m_nCountCharts);
				file = chartEx.smart_dynamic_cast<OOX::File>();
			}
			else
			{
				smart_ptr<OOX::Spreadsheet::CChartFile> chart = new OOX::Spreadsheet::CChartFile(NULL);
				chart->SetGlobalNumber(m_nCountCharts);
				file = chart.smart_dynamic_cast<OOX::File>();
			}
			if (true == oXlsxSerializer.saveChart(pReader, lLen, file))
			{
				if (m_bChartEx)
				{
					unsigned int nRId = pReader->m_pRels->WriteRels(L"http://schemas.microsoft.com/office/2014/relationships/chartEx", file->m_sOutputFilename, L"");
					id_data = new OOX::RId(nRId);
				}
				else
				{
					unsigned int nRId = pReader->m_pRels->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/chart", file->m_sOutputFilename, L"");
					id_data = new OOX::RId(nRId);
				}
			}
			oDrawingConverter.m_pReader			= pOldReader;
			oDrawingConverter.m_pImageManager	= pOldImageManager;
		}
		void ChartRec::FillParentPointersForChilds()
		{
		}
	} // namespace Logic
} // namespace PPTX
