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

#include "../CompositeObject.h"

namespace XLS
{
class CHARTFORMATS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CHARTFORMATS)
public:
	CHARTFORMATS();
	~CHARTFORMATS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);
    virtual const bool saveContent(BinProcessor& proc);

	BaseObjectPtr				m_ChartRect;
	std::vector<BaseObjectPtr>	m_arSERIESFORMAT;
	std::vector<BaseObjectPtr>	m_arFONTLIST;
	BaseObjectPtr				m_Scl;
	BaseObjectPtr				m_PlotGrowth;
	BaseObjectPtr				m_FRAME;
	BaseObjectPtr				m_AxesUsed;
	BaseObjectPtr				m_ShtProps;
	BaseObjectPtr				m_TEXTPROPS;
	BaseObjectPtr				m_DAT;
	BaseObjectPtr				m_CrtLayout12A;

	std::vector<BaseObjectPtr>	m_arSS;
	std::vector<BaseObjectPtr>	m_arAXISPARENT;
	std::vector<BaseObjectPtr>	m_arATTACHEDLABEL;
	std::vector<BaseObjectPtr>	m_arDFTTEXT;

	struct _chart_format
	{
		BaseObjectPtr	attachedLABEL;
		BaseObjectPtr	dataLabExt;
	};
	std::vector<_chart_format>	m_arChartFormats;

//------------------------------------------------------------------------------
	BaseObjectPtr								find_label	( _UINT16 link_id, unsigned short ex);
	std::vector<std::pair<int, BaseObjectPtr>>	find_labels ( _UINT16 link_id, unsigned short ser);
	
	BaseObjectPtr	find_default_text	( _UINT16 link_id);
};

} // namespace XLS

