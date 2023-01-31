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

#include "ChartWriter.h"

namespace Writers
{
	ChartWriter::ChartWriter(std::wstring sDir) : m_sDir(sDir)
	{
		nEmbeddedCount = 1000;
	}
	ChartWriter::~ChartWriter()
	{
	}
	bool ChartWriter::IsEmpty()
	{
		return 0 == m_aCharts.size();
	}
	bool ChartWriter::Write()
	{
		if(IsEmpty()) return false;

		OOX::CPath pathChartDir = m_sDir + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"charts";

		for(size_t i = 0; i < m_aCharts.size(); ++i)
		{
			_chartElem & elem = m_aCharts[i];

			OOX::CPath filePath = pathChartDir + FILE_SEPARATOR_STR + elem.filename;

			NSFile::CFileBinary oFile;
			oFile.CreateFileW(filePath.GetPath());
			oFile.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n");
			oFile.WriteStringUTF8(elem.content);
			oFile.CloseFile();
		}
		return true;
	}
	void ChartWriter::AddChart(std::wstring& content, std::wstring& sRelsName, std::wstring& sFileName, int index)
	{
		_chartElem oChartElem;

		oChartElem.content	= content;
		oChartElem.index	= index;

		oChartElem.filename = L"chart" + std::to_wstring(oChartElem.index) + L".xml";

		sRelsName	= L"charts/" + oChartElem.filename;
		sFileName	= oChartElem.filename;

		m_aCharts.push_back(oChartElem);
	}
}
