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
	
	std::vector<BiffStructurePtr>	m_arHFPictureDrawing;

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

