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
	void load(IBinaryReader* reader);

	std::wstring loadHyperlinkString(XLS::CFRecord& record);
	std::wstring loadHyperlinkString(IBinaryReader* reader);

	_UINT32 streamVersion;

	static const XLS::ElementType type = XLS::typeHyperlinkObject;

	bool hlstmfHasMoniker;
	bool hlstmfIsAbsolute;
	bool hlstmfSiteGaveDisplayName;
	bool hlstmfHasLocationStr;
	bool hlstmfHasDisplayName;
	bool hlstmfHasGUID;
	bool hlstmfHasCreationTime;
	bool hlstmfHasFrameName;
	bool hlstmfMonikerSavedAsStr;
	bool hlstmfAbsFromGetdataRel;

	std::wstring		displayName;
	std::wstring		targetFrameName;
	std::wstring		moniker;
	HyperlinkMoniker	oleMoniker;	
	std::wstring		location;
	std::wstring		guid;
	FILETIME			fileTime;

};

} // namespace OSHARED

