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

#include "../../ASCOfficeDrawingConverter.h"

#include "../../../XlsxSerializerCom/Reader/ChartFromToBinary.h"
#include "../../../ASCOfficeDocxFile2/DocWrapper/XlsxSerializer.h"
#include "../../../ASCOfficeDocxFile2/BinWriter/BinWriters.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramData.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramDrawing.h"

namespace PPTX
{
	namespace Logic
	{
		bool SmartArt::LoadDrawing(OOX::IFileContainer* pRels)
		{
			if (m_diag.IsInit()) 
				return true;

			if(id_data.IsInit() == false) return false;
			if (pRels == NULL) return false;

			bool result = false;

			smart_ptr<OOX::File>	oFileData;
			smart_ptr<OOX::File>	oFileDrawing;
			nullable<std::wstring>	id_drawing;

			OOX::CDiagramData*		pDiagramData	= NULL;
			OOX::CDiagramDrawing*	pDiagramDrawing	= NULL;

			oFileData = pRels->Find(*id_data);
			
			if (oFileData.IsInit())
			{
                pDiagramData = dynamic_cast<OOX::CDiagramData*>(oFileData.GetPointer());
				if (pDiagramData) result = true; // это smart art ..есть у него drawing или нет - неважно

				if ((pDiagramData) && (pDiagramData->m_oExtLst.IsInit()))
				{
					for (size_t i = 0; i < pDiagramData->m_oExtLst->m_arrExt.size(); i++)
					{
						if (pDiagramData->m_oExtLst->m_arrExt[i]->m_oDataModelExt.IsInit())
						{
							id_drawing = pDiagramData->m_oExtLst->m_arrExt[i]->m_oDataModelExt->m_oRelId;
							break;
						}
					}
				}

				if (id_drawing.IsInit() && pDiagramData)
				{
					if		(parentFileIs<OOX::IFileContainer>())	oFileDrawing = parentFileAs<OOX::IFileContainer>().Find(*id_drawing);
					else if (pRels != NULL)							oFileDrawing = pRels->Find(*id_drawing);
				}
				else
				{
					//Monetizing_Innovation.pptx (слайд 13) - diagrams/data1.xml не ссылается на diagrams/drawing1.xml
					//пробуем по тому же пути с номером data.xml - ниже			
 				}
			}
			pDiagramDrawing = dynamic_cast<OOX::CDiagramDrawing*>(oFileDrawing.GetPointer());

			if (!pDiagramDrawing && pDiagramData)
			{
				// easy4cargo1.pptx - слайд 2 - в диаграмме Smart вместо ссылки на drawing.xml ссылка на стороннюю картинку
               OOX::CPath pathDiagramData = pDiagramData->m_strFilename;

				int a1 = (int)pathDiagramData.GetFilename().find(L".");
				std::wstring strId = pathDiagramData.GetFilename().substr(4, pathDiagramData.GetFilename().length() - 8);
				
				OOX::CPath pathDiagramDrawing = pathDiagramData.GetDirectory() + FILE_SEPARATOR_STR + L"drawing" + strId + L".xml";	

				oFileDrawing = smart_ptr<OOX::File>(dynamic_cast<OOX::File*>(new OOX::CDiagramDrawing(NULL, pathDiagramDrawing)));
				if (oFileDrawing.IsInit())
					pDiagramDrawing = dynamic_cast<OOX::CDiagramDrawing*>(oFileDrawing.GetPointer());
			}

			if ((pDiagramDrawing) && (pDiagramDrawing->m_oShapeTree.IsInit()))
			{
				m_diag = pDiagramDrawing->m_oShapeTree;
				FillParentPointersForChilds();

				m_pFileContainer = oFileDrawing.smart_dynamic_cast<OOX::IFileContainer>();

				if (!m_diag->grpSpPr.xfrm.IsInit())
					m_diag->grpSpPr.xfrm = new PPTX::Logic::Xfrm;
			}
			else
			{
				//parse pDiagramData !!
			}
			return true;
		}
		void SmartArt::LoadDrawing(NSBinPptxRW::CBinaryFileWriter* pWriter)
		{
			if (m_diag.IsInit()) 
				return ;

			OOX::IFileContainer	& pRelsPPTX	= parentFileAs<OOX::IFileContainer>();
			OOX::IFileContainer	* pRels		= NULL;
			
			if (pWriter)
			{
				if (pWriter->m_pCurrentContainer->is_init())
					pRels = pWriter->m_pCurrentContainer->operator ->();
			}
			
			bool result = LoadDrawing(&pRelsPPTX);
			if (!result)
				result	= LoadDrawing( pRels );
		}
		void SmartArt::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (m_diag.is_init())
			{
				smart_ptr<OOX::IFileContainer> oldContainer = *pWriter->m_pCurrentContainer;
				*pWriter->m_pCurrentContainer = m_pFileContainer;
				if (pWriter->m_pMainDocument)
					pWriter->m_pMainDocument->m_pParamsWriter->m_pCurRels = (OOX::IFileContainer*)m_pFileContainer.GetPointer();
				
				m_diag->toPPTY(pWriter);
				
				*pWriter->m_pCurrentContainer = oldContainer;
				if (pWriter->m_pMainDocument)
					pWriter->m_pMainDocument->m_pParamsWriter->m_pCurRels = oldContainer.GetPointer();
			}
		}
		void ChartRec::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			OOX::IFileContainer* pRels = NULL;
			if (pWriter->m_pCurrentContainer->is_init())
				pRels = pWriter->m_pCurrentContainer->operator ->();

			smart_ptr<OOX::File> file;
			if(id_data.IsInit())
			{
				if(parentFileIs<OOX::IFileContainer>())	file = parentFileAs<OOX::IFileContainer>().Find(*id_data);
				else if (pRels != NULL)					file = pRels->Find(*id_data);
			}
			smart_ptr<OOX::Spreadsheet::CChartSpace> pChart = file.smart_dynamic_cast<OOX::Spreadsheet::CChartSpace>();
			smart_ptr<OOX::Spreadsheet::CChartSpaceEx> pChartEx = file.smart_dynamic_cast<OOX::Spreadsheet::CChartSpaceEx>();

			if (false == pChart.IsInit() && false == pChartEx.IsInit()) return;

			NSBinPptxRW::CDrawingConverter oDrawingConverter;
			
			NSBinPptxRW::CBinaryFileWriter *pOldDrawingWriter = oDrawingConverter.m_pBinaryWriter;
			BinDocxRW::CDocxSerializer *pOldMainDocument = pWriter->m_pMainDocument;
			
			pWriter->m_pMainDocument = NULL;
			oDrawingConverter.m_pBinaryWriter = pWriter;
			smart_ptr<OOX::IFileContainer> oldRels = oDrawingConverter.GetRels();
			oDrawingConverter.SetRels(pChart.smart_dynamic_cast<OOX::IFileContainer>());
		
			BinXlsxRW::BinaryChartWriter oBinaryChartWriter(*pWriter, &oDrawingConverter);	
			if (pChart.IsInit())
			{
				oBinaryChartWriter.WriteCT_ChartSpace(*pChart);
			}
			else if (pChartEx.IsInit())
			{
				//oBinaryChartWriter.WriteCT_ChartSpace(*pChartEx);
			}

			oDrawingConverter.SetRels(oldRels);
			
			oDrawingConverter.m_pBinaryWriter = pOldDrawingWriter;
			pWriter->m_pMainDocument = pOldMainDocument;
		}
		std::wstring ChartRec::toXML() const
		{
			if (!id_data.is_init() || false == m_bData)
				return L"";
            std::wstring strData = L"<c:chart xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" r:id=\"" + id_data->ToString() + L"\"/>";

			return strData;
		}
		void ChartRec::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (!id_data.is_init() || false == m_bData)
				return;
			
            std::wstring strData = L"<c:chart xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" r:id=\"" + id_data->ToString() + L"\"/>";

			pWriter->WriteString(strData);
		}

		void ChartRec::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			ULONG lLen = pReader->GetLong();
			m_bData = true;

			m_lChartNumber = pReader->m_lChartNumber;
			pReader->m_lChartNumber++;
			unsigned int lId = pReader->m_pRels->WriteChart(m_lChartNumber, pReader->m_nDocumentType);

			BinXlsxRW::CXlsxSerializer		oXlsxSerializer;
			NSBinPptxRW::CDrawingConverter	oDrawingConverter;

			NSBinPptxRW::CImageManager2*	pOldImageManager	= oDrawingConverter.m_pImageManager;
			NSBinPptxRW::CBinaryFileReader* pOldReader			= oDrawingConverter.m_pReader;
 			
			oDrawingConverter.m_pImageManager = pReader->m_pRels->m_pManager;
			oDrawingConverter.m_pReader = pReader;

			oXlsxSerializer.setDrawingConverter(&oDrawingConverter);

            std::wstring strDstChart = pReader->m_pRels->m_pManager->GetDstMedia();
            int nPos = (int)strDstChart.rfind(wchar_t('m'));
			if (-1 != nPos)
                strDstChart = strDstChart.substr(0, nPos);

            strDstChart += L"charts";
        //на всякий случай всегда создаем, нет уверенности что 1 == m_lChartNumber для первого chart
            NSDirectory::CreateDirectory(strDstChart);

			std::wstring strChart = strDstChart + FILE_SEPARATOR_STR + L"chart" + std::to_wstring(m_lChartNumber) + L".xml";

			oXlsxSerializer.saveChart(pReader, lLen, strChart, m_lChartNumber);

			oDrawingConverter.m_pReader			= pOldReader;
			oDrawingConverter.m_pImageManager	= pOldImageManager;

			id_data = new OOX::RId(lId);
		}
	} // namespace Logic
} // namespace PPTX
