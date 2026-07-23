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

#include "HyperlinkMoniker.h"
#include "BiffString.h"

#if !defined(_WIN32) && !defined(_WIN64)

    typedef struct _FILETIME {
        _UINT32 dwLowDateTime;
        _UINT32 dwHighDateTime;
    } FILETIME;
#else
	#include <windows.h>
#endif

namespace OSHARED
{


class HyperlinkObject : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(HyperlinkObject)
public:
	XLS::BiffStructurePtr clone();

	virtual void load(XLS::CFRecord& record);
    virtual void save(XLS::CFRecord& record);
	void load(IBinaryReader* reader);

	std::wstring loadHyperlinkString(XLS::CFRecord& record);
    void saveHyperlinkString(XLS::CFRecord& record, std::wstring hlinkString);
	std::wstring loadHyperlinkString(IBinaryReader* reader);

    _UINT32 streamVersion = 2;

	static const XLS::ElementType type = XLS::typeHyperlinkObject;

    bool hlstmfHasMoniker = 0;
    bool hlstmfIsAbsolute = 0;
    bool hlstmfSiteGaveDisplayName = 0;
    bool hlstmfHasLocationStr = 0;
    bool hlstmfHasDisplayName = 0;
    bool hlstmfHasGUID = 0;
    bool hlstmfHasCreationTime = 0;
    bool hlstmfHasFrameName = 0;
    bool hlstmfMonikerSavedAsStr = 0;
    bool hlstmfAbsFromGetdataRel = 0;

	std::wstring		displayName;
	std::wstring		targetFrameName;
	std::wstring		moniker;
	HyperlinkMoniker	oleMoniker;	
	std::wstring		location;
	std::wstring		guid;
	FILETIME			fileTime;

};

} // namespace OSHARED

