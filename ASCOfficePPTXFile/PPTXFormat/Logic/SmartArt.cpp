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
//#include "stdafx.h"

#include "./SmartArt.h"
#include "../Slide.h"
#include "../SlideLayout.h"
#include "../SlideMaster.h"

#include "../../ASCOfficeDrawingConverter.h"
#include "../../../ASCOfficeDocxFile2/DocWrapper/XlsxSerializer.h"

namespace PPTX
{
	namespace Logic
	{
		void SmartArt::LoadDrawing(NSBinPptxRW::CBinaryFileWriter* pWriter)
		{
            std::wstring strDataPath;

			FileContainer* pRels = NULL;
			if (pWriter->m_pCommonRels->is_init())
				pRels = pWriter->m_pCommonRels->operator ->();

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
					smart_ptr<PPTX::Image> p = pRels->image(*id_data);
					if (p.is_init())
						strDataPath = p->filename().m_strFilename;
				}
			}

            if (strDataPath.empty())
				return;

			nullable<PPTX::RId> id_drawing;

			XmlUtils::CXmlNode oNode;
			if (oNode.FromXmlFile(strDataPath) == false) return;

            XmlUtils::CXmlNode oNode2 = oNode.ReadNode(L"dgm:extLst");
			if (oNode2.IsValid())
			{
                XmlUtils::CXmlNode oNode3 = oNode2.ReadNode(L"a:ext");
				if (oNode3.IsValid())
				{
					//https://msdn.microsoft.com/library/documentformat.openxml.office.drawing.datamodelextensionblock.aspx
                    XmlUtils::CXmlNode oNode4 = oNode3.ReadNode(L"dsp:dataModelExt");
					if (oNode4.IsValid())
					{
						oNode4.ReadAttributeBase(L"relId", id_drawing);
					}
				}
			}

			if (id_drawing.is_init())
			{
                std::wstring strDWPath;

                if(parentFileIs<Slide>())
					strDWPath = parentFileAs<Slide>().GetMediaFullPathNameFromRId(*id_drawing);
				else if(parentFileIs<SlideLayout>())
					strDWPath = parentFileAs<SlideLayout>().GetMediaFullPathNameFromRId(*id_drawing);
				else if(parentFileIs<SlideMaster>())
					strDWPath = parentFileAs<SlideMaster>().GetMediaFullPathNameFromRId(*id_drawing);
				else if(parentFileIs<Theme>())
					strDWPath = parentFileAs<Theme>().GetMediaFullPathNameFromRId(*id_drawing);
				else if (pRels != NULL)
				{
					smart_ptr<PPTX::Image> p = pRels->image(*id_drawing);
					if (p.is_init())
						strDWPath = p->filename().m_strFilename;
				}


                if (!strDWPath.empty())
				{
					XmlUtils::CXmlNode oNodeDW;
                    if (oNodeDW.FromXmlFile(strDWPath))
					{
                        XmlUtils::CXmlNode oNodeS = oNodeDW.ReadNodeNoNS(L"spTree");

						if (oNodeS.IsValid())
						{
							m_diag = oNodeS;

							CCommonRels* pRels = new CCommonRels();
							OOX::CPath filename = strDWPath;
							pRels->_read(filename);

							m_oCommonRels = pRels;

							return;
						}
					}
				}
			}
			//Monetizing_Innovation.pptx (слайд 13) - diagrams/data1.xml не ссылается на diagrams/drawing1.xml
			//пробуем по тому же пути с номером data.xml				
			// easy4cargo1.pptx - слайд 2 - в диаграмме Smart вместо ссылки на drawing.xml ссылка на стороннюю картинку


			OOX::CPath pathDiagramData = strDataPath;

			int a1 = pathDiagramData.GetFilename().find(L".");
			std::wstring strId = pathDiagramData.GetFilename().substr(4,pathDiagramData.GetFilename().length() - 8);
			
			OOX::CPath pathDiagramDrawing = pathDiagramData.GetDirectory() + FILE_SEPARATOR_STR + L"drawing" + strId + L".xml";

			XmlUtils::CXmlNode oNodeDW;
            if (oNodeDW.FromXmlFile(pathDiagramDrawing.GetPath()))//todooo ... сделать отдельно .. повтор
			{
                XmlUtils::CXmlNode oNodeS = oNodeDW.ReadNodeNoNS(L"spTree");

				if (oNodeS.IsValid())
				{
					m_diag = oNodeS;

					CCommonRels* pRels = new CCommonRels();
					pRels->_read(pathDiagramDrawing);

					m_oCommonRels = pRels;

					return;
				}
			}

		}

		void ChartRec::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			FileContainer* pRels = NULL;
			if (pWriter->m_pCommonRels->is_init())
				pRels = pWriter->m_pCommonRels->operator ->();

			std::wstring strDataPath = _T("");
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
					smart_ptr<PPTX::Image> p = pRels->image(*id_data);
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

			id_data = new PPTX::RId((size_t)lId);
		}
	} // namespace Logic
} // namespace PPTX
