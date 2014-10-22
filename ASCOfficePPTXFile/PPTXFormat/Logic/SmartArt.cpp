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
			CString strDataPath = _T("");

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

			if (_T("") == strDataPath)
				return;

			nullable<PPTX::RId> id_drawing;

			XmlUtils::CXmlNode oNode;
			oNode.FromXmlFile2(strDataPath);

			XmlUtils::CXmlNode oNode2 = oNode.ReadNode(_T("dgm:extLst"));
			if (oNode2.IsValid())
			{
				XmlUtils::CXmlNode oNode3 = oNode2.ReadNode(_T("a:ext"));
				if (oNode3.IsValid())
				{
					XmlUtils::CXmlNode oNode4 = oNode3.ReadNode(_T("dsp:dataModelExt"));
					if (oNode4.IsValid())
					{
						oNode4.ReadAttributeBase(L"relId", id_drawing);
					}
				}
			}

			if (!id_drawing.is_init())
				return;

			CString strDWPath = _T("");
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


			if (_T("") != strDWPath)
			{
				XmlUtils::CXmlNode oNodeDW;
				oNodeDW.FromXmlFile2(strDWPath);

				XmlUtils::CXmlNode oNodeS = oNodeDW.ReadNodeNoNS(_T("spTree"));
				diag = oNodeS;

				CCommonRels* pRels = new CCommonRels();
				OOX::CPath filename = strDWPath;
				pRels->_read(filename);

				m_oCommonRels = (FileContainer*)pRels;
			}
		}

		void ChartRec::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			FileContainer* pRels = NULL;
			if (pWriter->m_pCommonRels->is_init())
				pRels = pWriter->m_pCommonRels->operator ->();

			CString strDataPath = _T("");
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

			if (_T("") == strDataPath)
				return;

			BinXlsxRW::CXlsxSerializer oXlsxSerializer;
			NSBinPptxRW::CDrawingConverter oDrawingConverter;
			NSBinPptxRW::CBinaryFileWriter* pOldWriter = oDrawingConverter.m_pBinaryWriter;
			oDrawingConverter.m_pBinaryWriter = pWriter;
			NSCommon::smart_ptr<PPTX::FileContainer> pOldRels = *oDrawingConverter.m_pBinaryWriter->m_pCommonRels;

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
			
			CString strData = _T("<c:chart xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" r:id=\"") + id_data->ToString() + _T("\"/>");

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

			CString strDstChart = pReader->m_pRels->m_pManager->GetDstMedia();
			int nPos = strDstChart.ReverseFind(TCHAR('m'));
			if (-1 != nPos)
				strDstChart = strDstChart.Mid(0, nPos);

			strDstChart += _T("charts");
			if (1 == m_lChartNumber)
			{
                FileSystem::Directory::CreateDirectory(strDstChart);
			}
			CString strChart = _T("");
			strChart.Format(_T("chart%d.xml"), m_lChartNumber);

			strChart = strDstChart + _T("\\") + strChart;

			CString* sContentTypes = NULL;
			if (pReader->m_lDocumentType == XMLWRITER_DOC_TYPE_DOCX)
				oXlsxSerializer.saveChart(*pReader, lLen, strChart, CString(L"/word/charts/"), &sContentTypes);
			else if (pReader->m_lDocumentType == XMLWRITER_DOC_TYPE_XLSX)
				oXlsxSerializer.saveChart(*pReader, lLen, strChart, CString(L"/xl/charts/"), &sContentTypes);
			else
				oXlsxSerializer.saveChart(*pReader, lLen, strChart, CString(L"/ppt/charts/"), &sContentTypes);

			pReader->m_strContentTypes += (*sContentTypes);
			RELEASEOBJECT(sContentTypes);

			oDrawingConverter.m_pReader = pOldReader;
			oDrawingConverter.m_pImageManager = pOldImageManager;

			id_data = new PPTX::RId((size_t)lId);
		}
	} // namespace Logic
} // namespace PPTX
