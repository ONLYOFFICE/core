#ifndef CHART_WRITER
#define CHART_WRITER

#include "../../XlsxSerializerCom/Common/Common.h"

namespace Writers
{
	class ChartWriter
	{
		class ChartElem
		{
		public:
			CString content;
			CString filename;
			int index;
		};
		std::vector<ChartElem*> m_aCharts;
		ContentTypesWriter& m_oContentTypesWriter;
		int nChartCount;
	public:
		CString m_sDir;
	public:
		ChartWriter(CString sDir, ContentTypesWriter& oContentTypesWriter):m_sDir(sDir),m_oContentTypesWriter(oContentTypesWriter)
		{
			nChartCount = 0;
		}
		~ChartWriter()
		{
			for(int i = 0, length = m_aCharts.size(); i < length; ++i)
			{
				delete m_aCharts[i];
			}
		}
		bool IsEmpty()
		{
			return 0 == m_aCharts.size();
		}
		void Write()
		{
			if(false == IsEmpty())
			{
                OOX::CPath pathChartDir = m_sDir + FILE_SEPARATOR_STR + _T("word") + FILE_SEPARATOR_STR + _T("charts");
				FileSystem::Directory::CreateDirectory(string2std_string(pathChartDir.GetPath()));

                for(int i = 0, length = m_aCharts.size(); i < length; ++i)
				{
					ChartElem* elem = m_aCharts[i];

                    OOX::CPath filePath = pathChartDir + FILE_SEPARATOR_STR + elem->filename;

					CFile oFile;
					oFile.CreateFile(filePath.GetPath());
					oFile.WriteStringUTF8(CString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n")));
					oFile.WriteStringUTF8(elem->content);
					oFile.CloseFile();

					//Content_Types
                    CString sRelPath = _T("/word/charts/") + elem->filename;
                    m_oContentTypesWriter.AddOverride(sRelPath, CString(_T("application/vnd.openxmlformats-officedocument.drawingml.chart+xml")));
				}
			}
		}
		void AddChart(CString& content, CString& sRelsName, CString& sFileName, int& index)
		{
			ChartElem* pChartElem = new ChartElem();
			pChartElem->content = content;
			pChartElem->index = nChartCount + 1;
			nChartCount++;
			pChartElem->filename.Format(_T("chart%d.xml"), pChartElem->index);
			sRelsName = _T("charts/") + pChartElem->filename;
			sFileName = pChartElem->filename;
			index = pChartElem->index;

			m_aCharts.push_back(pChartElem);
		}
		int getChartCount()
		{
			return nChartCount;
		}
		void setChartCount(int val)
		{
			nChartCount = val;
		}
	};
}
#endif	// #ifndef CHART_WRITER
