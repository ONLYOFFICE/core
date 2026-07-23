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
#pragma once

#include "CommonSubstream.h"

#include <unordered_map>

namespace XLS
{;

class CHARTFORMATS;
class SERIESDATA;
class CRT;

class ChartSheetSubstream;
typedef boost::shared_ptr<ChartSheetSubstream>	ChartSheetSubstreamPtr;

class ChartSheetSubstream: public CompositeObject, public CommonSubstream
{
	BASE_OBJECT_DEFINE_CLASS_NAME(ChartSheetSubstream)
public:
	ChartSheetSubstream(const size_t ws_index);
	~ChartSheetSubstream();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);
    virtual const bool saveContent(BinProcessor& proc);

	int serialize			(std::wostream & _stream);
	int serialize_title		(std::wostream & _stream);
	int serialize_plot_area (std::wostream & _stream);
	int serialize_legend	(std::wostream & _stream, const std::wstring & legend_entries);
	int serialize_ser		(std::wstring sNodeName, std::wostream & _stream, int series_id, const BaseObjectPtr & ai, int type, int count);
	int serialize_dPt		(std::wostream & _stream, int series_id, CRT *crt, int count_point);
	int serialize_dLbls		(std::wostream & _stream, int series_id, CRT *crt);
	int serialize_3D		(std::wostream & _stream);
	
	int serialize_scatter_style(std::wostream & _stream, CRT *crt);

	static const ElementType	type = typeChartSheetSubstream;

	std::vector<BaseObjectPtr>	m_arFbi;
	BaseObjectPtr				m_CHARTFORMATS;
	BaseObjectPtr				m_SERIESDATA;
	BaseObjectPtr				m_Units;
	BaseObjectPtr				m_ExternSheet;
	BaseObjectPtr				m_SXViewLink;
	BaseObjectPtr				m_PivotChartBits;
	BaseObjectPtr				m_SBaseRef;
	BaseObjectPtr				m_PrintSize;
	BaseObjectPtr				m_Palette;
	BaseObjectPtr				m_WriteProtect;
	bool						separate = true;
	
private:

	void recalc(CHARTFORMATS*	charts);
	void recalc(SERIESDATA*		data);

	std::unordered_map<int, std::vector<int>> m_mapTypeChart;
};

} // namespace XLS

