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

class SUPBOOK: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SUPBOOK)
public:
	struct _xct
	{
		BaseObjectPtr				m_XCT;
		std::vector<BaseObjectPtr>	m_arCRN;
	};

	struct _def_name
	{
		std::wstring fmla;
		int sheetId;
	};

	SUPBOOK();
	~SUPBOOK();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);
    virtual const bool saveContent(BinProcessor& proc);

	int serialize(std::wostream & strm);

	static const ElementType type = typeSUPBOOK;

	BaseObjectPtr				m_SupBook;
	BaseObjectPtr				m_ExternSheet;

	std::vector<BaseObjectPtr>	m_arExternName;
	std::vector<_xct>			m_arXCT;

	GlobalWorkbookInfoPtr		global_info;

	bool						IsExternal();
	
	std::wstring				sExternPathLink;	
	int							nExternIndex;

	std::map<std::wstring, _def_name>	mapNamesExt;
	std::vector<std::wstring>			arNames;

private:
	int serialize_book(std::wostream & strm);
	int serialize_dde(std::wostream & strm);
};

} // namespace XLS

