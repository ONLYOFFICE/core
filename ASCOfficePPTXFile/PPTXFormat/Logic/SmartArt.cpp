/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "SmartArt.h"
#include "../Slide.h"
#include "../SlideLayout.h"
#include "../SlideMaster.h"

#include "../../ASCOfficeDrawingConverter.h"
#include "../../../ASCOfficeDocxFile2/DocWrapper/XlsxSerializer.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramData.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramDrawing.h"

namespace PPTX
{
	namespace Logic
	{
		void SmartArt::LoadDrawing(NSBinPptxRW::CBinaryFileWriter* pWriter)
		{
			FileContainer* pRels = NULL;
			if (pWriter->m_pCommonRels->is_init())
				pRels = pWriter->m_pCommonRels->operator ->();

			smart_ptr<OOX::File>	oFileData;
			smart_ptr<OOX::File>	oFileDrawing;
			nullable<std::wstring>	id_drawing;

			OOX::CDiagramData*		pDiagramData	= NULL;
			OOX::CDiagramDrawing*	pDiagramDrawing	= NULL;

			if(id_data.IsInit())
			{
				if		(parentFileIs<Slide>())			oFileData = parentFileAs<Slide>().Find(*id_data);
				else if	(parentFileIs<SlideLayout>())	oFileData = parentFileAs<SlideLayout>().Find(*id_data);
				else if	(parentFileIs<SlideMaster>())	oFileData = parentFileAs<SlideMaster>().Find(*id_data);
				else if	(parentFileIs<Theme>())			oFileData = parentFileAs<Theme>().Find(*id_data);
				else if (pRels != NULL)					oFileData = pRels->Find(*id_data);
			}
			
			if (oFileData.IsInit())
			{
                pDiagramData = dynamic_cast<OOX::CDiagramData*>(oFileData.operator->());

				if ((pDiagramData) && (pDiagramData->m_oExtLst.IsInit()))
				{
					for (int i = 0; i < pDiagramData->m_oExtLst->m_arrExt.size(); i++)
					{
						if (pDiagramData->m_oExtLst->m_arrExt[i]->m_oDataModelExt.IsInit())
						{
							id_drawing = pDiagramData->m_oExtLst->m_arrExt[i]->m_oDataModelExt->m_oRelId;
							break;
						}
					}
				}

				if (id_drawing.IsInit())
				{
					if		(parentFileIs<Slide>())			oFileDrawing = parentFileAs<Slide>().Find(*id_drawing);
					else if	(parentFileIs<SlideLayout>())	oFileDrawing = parentFileAs<SlideLayout>().Find(*id_drawing);
					else if	(parentFileIs<SlideMaster>())	oFileDrawing = parentFileAs<SlideMaster>().Find(*id_drawing);
					else if	(parentFileIs<Theme>())			oFileDrawing = parentFileAs<Theme>().Find(*id_drawing);
					else if (pRels != NULL)					oFileDrawing = pRels->Find(*id_data);
				}
				else
				{
					//Monetizing_Innovation.pptx (слайд 13) - diagrams/data1.xml не ссылается на diagrams/drawing1.xml
					//пробуем по тому же пути с номером data.xml - ниже			
 				}
			}
            if (oFileDrawing.IsInit())
			{
                pDiagramDrawing = dynamic_cast<OOX::CDiagramDrawing*>(oFileDrawing.operator->());
			}

			if (!pDiagramDrawing)
			{
				// easy4cargo1.pptx - слайд 2 - в диаграмме Smart вместо ссылки на drawing.xml ссылка на стороннюю картинку
               OOX::CPath pathDiagramData = pDiagramData->m_strFilename;

				int a1 = pathDiagramData.GetFilename().find(L".");
				std::wstring strId = pathDiagramData.GetFilename().substr(4, pathDiagramData.GetFilename().length() - 8);
				
				OOX::CPath pathDiagramDrawing = pathDiagramData.GetDirectory() + FILE_SEPARATOR_STR + L"drawing" + strId + L".xml";	

				oFileDrawing = smart_ptr<OOX::File>(dynamic_cast<OOX::File*>(new OOX::CDiagramDrawing(pathDiagramDrawing)));
				if (oFileDrawing.IsInit())
					pDiagramDrawing = dynamic_cast<OOX::CDiagramDrawing*>(oFileDrawing.operator->());
			}

			if ((pDiagramDrawing) && (pDiagramDrawing->m_oShapeTree.IsInit()))
			{
				m_diag			= pDiagramDrawing->m_oShapeTree;
				m_oCommonRels	= smart_ptr<PPTX::CCommonRels>( new PPTX::CCommonRels());
				m_oCommonRels->_read(pDiagramDrawing->m_oReadPath);
			}
			else
			{
				//parse pDiagramData !!
			}
		}

		void ChartRec::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			FileContainer* pRels = NULL;
			if (pWriter->m_pCommonRels->is_init())
				pRels = pWriter->m_pCommonRels->operator ->();

            std::wstring strDataPath = L"";
			if(id_data.IsInit())
			{
				if(parentFileIs<Slide>())
					strDataPath = parentFileAs<Slide>().GetMediaFullPathNameFromRId(*id_data);
				else if(parentFileIs<SlideLayout>())
					strDataPath = parentFileAs<SlideLayout>().GetMediaFullPathNameFromRId(*id_data);
				else if(parentFileIs<SlideMaster>())
					strDataPath = parentFileAs<SlideMaster>().GetMediaFullPathNameFromRId(*id_data);
				else if(parentFileIs<Theme>())
					strDataPath = parentFileAs<Theme>().GetMediaFullPathNameFromRId(*id_data);
				else if (pRels != NULL)
				{
					smart_ptr<OOX::Image> p = pRels->GetImage(*id_data);
					if (p.is_init())
						strDataPath = p->filename().m_strFilename;
				}
			}

            if (strDataPath.empty())
				return;

			BinXlsxRW::CXlsxSerializer oXlsxSerializer;
			NSBinPptxRW::CDrawingConverter oDrawingConverter;
			NSBinPptxRW::CBinaryFileWriter* pOldWriter = oDrawingConverter.m_pBinaryWriter;
			oDrawingConverter.m_pBinaryWriter = pWriter;
			NSCommon::smart_ptr<PPTX::CCommonRels> pOldRels = *oDrawingConverter.m_pBinaryWriter->m_pCommonRels;

			oXlsxSerializer.setDrawingConverter(&oDrawingConverter);

			long lDataSize = 0;
			oXlsxSerializer.loadChart(strDataPath, *pWriter, lDataSize);
			*oDrawingConverter.m_pBinaryWriter->m_pCommonRels = pOldRels;
			oDrawingConverter.m_pBinaryWriter = pOldWriter;
		}

		void ChartRec::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (!id_data.is_init() || NULL == m_bData)
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
			int lId = pReader->m_pRels->WriteChart(m_lChartNumber, pReader->m_lDocumentType);

			BinXlsxRW::CXlsxSerializer oXlsxSerializer;
			NSBinPptxRW::CDrawingConverter oDrawingConverter;

			NSBinPptxRW::CImageManager2* pOldImageManager = oDrawingConverter.m_pImageManager;
			oDrawingConverter.m_pImageManager = pReader->m_pRels->m_pManager;
			NSBinPptxRW::CBinaryFileReader* pOldReader = oDrawingConverter.m_pReader;
            oDrawingConverter.m_pReader = pReader;

			oXlsxSerializer.setDrawingConverter(&oDrawingConverter);

            std::wstring strDstChart = pReader->m_pRels->m_pManager->GetDstMedia();
            int nPos = strDstChart.rfind(wchar_t('m'));
			if (-1 != nPos)
                strDstChart = strDstChart.substr(0, nPos);

            strDstChart += L"charts";
        //на всякий случай всегда создаем, нет уверенности что 1 == m_lChartNumber для первого chart
            NSDirectory::CreateDirectory(strDstChart);

            std::wstring* sContentTypes = NULL;

            std::wstring strChart           = strDstChart + FILE_SEPARATOR_STR + L"chart" + std::to_wstring(m_lChartNumber) + L".xml";
            std::wstring strWordChartFolder = L"/word/charts/";
            std::wstring strXlChartFolder   = L"/xl/charts/";
            std::wstring strPptChartFolder  = L"/ppt/charts/";

            if (pReader->m_lDocumentType == XMLWRITER_DOC_TYPE_DOCX)
                oXlsxSerializer.saveChart(*pReader, lLen, strChart, strWordChartFolder, &sContentTypes, m_lChartNumber);
			else if (pReader->m_lDocumentType == XMLWRITER_DOC_TYPE_XLSX)
                oXlsxSerializer.saveChart(*pReader, lLen, strChart, strXlChartFolder, &sContentTypes, m_lChartNumber);
			else
                oXlsxSerializer.saveChart(*pReader, lLen, strChart, strPptChartFolder, &sContentTypes, m_lChartNumber);

            if (sContentTypes)
            {
                pReader->m_strContentTypes += (*sContentTypes);
                RELEASEOBJECT(sContentTypes);
            }
			oDrawingConverter.m_pReader = pOldReader;
			oDrawingConverter.m_pImageManager = pOldImageManager;

			id_data = new OOX::RId((size_t)lId);
		}
	} // namespace Logic
} // namespace PPTX
