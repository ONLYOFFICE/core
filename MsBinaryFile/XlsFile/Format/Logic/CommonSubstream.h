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

#include "CompositeObject.h"
#include "Biff_structures/CellRef.h"

namespace XLS
{;

class CommonSubstream
{
public:
	CommonSubstream(const size_t ws_index) : ws_index_(ws_index) {}
	~CommonSubstream(){}

	int serialize_format(std::wostream & _stream);

	GlobalWorkbookInfoPtr			global_info_;
	size_t							ws_index_;
	
	std::vector<BiffStructurePtr>	m_arHFPictureDrawing; // here OfficeArtDgContainer - shape descriptions ...

	BaseObjectPtr					m_PAGESETUP;
	BaseObjectPtr					m_PROTECTION;
	BaseObjectPtr					m_BACKGROUND;
	BaseObjectPtr					m_GLOBALS;
	BaseObjectPtr					m_COLUMNS;
	BaseObjectPtr					m_CELLTABLE;
	BaseObjectPtr					m_SORTANDFILTER;
	BaseObjectPtr					m_OBJECTS;
	BaseObjectPtr					m_DCON;
	BaseObjectPtr					m_CodeName;
	BaseObjectPtr					m_SheetExt;
	BaseObjectPtr					m_SXADDLDBQUERY;
	BaseObjectPtr					m_CRTMLFRT;

	BaseObjectPtr					m_Formating; // for flat xls

	std::vector<BaseObjectPtr>		m_arHFPicture;
	std::vector<BaseObjectPtr>		m_arWINDOW;
	std::vector<BaseObjectPtr>		m_arCUSTOMVIEW;
	std::vector<BaseObjectPtr>		m_arFEAT;
	std::vector<BaseObjectPtr>		m_arRECORD12;
	std::vector<BaseObjectPtr>		m_arFEAT11;
	std::vector<BaseObjectPtr>		m_arSORT;

	std::vector<BaseObjectPtr>		m_arCellStyles;
	std::vector<BaseObjectPtr>		m_arCellXFs;

	void LoadHFPicture();
};

} // namespace XLS

