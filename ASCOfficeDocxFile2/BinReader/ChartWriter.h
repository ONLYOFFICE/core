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
			std::wstring content;
            std::wstring filename;
			int index;
		};
		std::vector<ChartElem*> m_aCharts;
        ContentTypesWriter&     m_oContentTypesWriter;
	public:
        std::wstring m_sDir;
	public:
        ChartWriter(std::wstring sDir, ContentTypesWriter& oContentTypesWriter):m_sDir(sDir),m_oContentTypesWriter(oContentTypesWriter)
		{
		}
		~ChartWriter()
		{
			for(size_t i = 0, length = m_aCharts.size(); i < length; ++i)
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
                NSDirectory::CreateDirectory(pathChartDir.GetPath());

                for(size_t i = 0, length = m_aCharts.size(); i < length; ++i)
				{
					ChartElem* elem = m_aCharts[i];

                    OOX::CPath filePath = pathChartDir + FILE_SEPARATOR_STR + elem->filename;

					NSFile::CFileBinary oFile;
					oFile.CreateFileW(filePath.GetPath());
					oFile.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n");
					oFile.WriteStringUTF8(elem->content);
					oFile.CloseFile();

					//Content_Types
                    std::wstring sRelPath = L"/word/charts/" + elem->filename;
                    m_oContentTypesWriter.AddOverride(sRelPath, L"application/vnd.openxmlformats-officedocument.drawingml.chart+xml");
				}
			}
		}
		void AddChart(std::wstring& content, std::wstring& sRelsName, std::wstring& sFileName, int index)
		{
			ChartElem* pChartElem = new ChartElem();
			pChartElem->content = content;
			pChartElem->index = index;
            pChartElem->filename = L"chart" + std::to_wstring(pChartElem->index) + L".xml";
			
			sRelsName = L"charts/" + pChartElem->filename;
			sFileName = pChartElem->filename;

			m_aCharts.push_back(pChartElem);
		}
	};
}
#endif	// #ifndef CHART_WRITER
