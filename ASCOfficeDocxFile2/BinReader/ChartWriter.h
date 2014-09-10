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
				CString sChartDir = m_sDir + _T("/word/charts");
				CreateDirectory(sChartDir, NULL);
				for(int i = 0, length = m_aCharts.size(); i < length; ++i)
				{
					ChartElem* elem = m_aCharts[i];
					CString sRelPath = _T("/word/charts/") + elem->filename;
					CString sAbsPath = m_sDir + sRelPath;

					CFile oFile;
					oFile.CreateFile(sAbsPath);
					oFile.WriteStringUTF8(elem->content);
					oFile.CloseFile();

					//Content_Types
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