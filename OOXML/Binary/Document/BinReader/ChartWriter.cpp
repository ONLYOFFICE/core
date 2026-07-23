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
