/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


// Logical representation of UserBView record in BIFF8
class UserBView: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(UserBView)
	BASE_OBJECT_DEFINE_CLASS_NAME(UserBView)
public:
	UserBView();
	~UserBView();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeUserBView;

	int serialize(std::wostream & stream);
//-----------------------------
	_UINT16			tabId;
	std::wstring	guid;
	
	BIFF_DWORD		x;
	BIFF_DWORD		y;
	BIFF_DWORD		dx;
	BIFF_DWORD		dy;

	_UINT16			wTabRatio;

	bool			fDspFmlaBar;
	bool			fDspStatus;
	
	std::wstring	mdNoteDisp;
	
	bool fDspHScroll;
	bool fDspVScroll;
	bool fBotAdornment;
	bool fZoom;
	
	std::wstring fHideObj;
	
	bool fPrintIncl;
	bool fRowColIncl;
	bool fInvalidTabId;
	bool fTimedUpdate;
	bool fAllMemChanges;
	bool fOnlySync;
	bool fPersonalView;
	bool fIconic;
	
	_UINT16			wMergeInterval;
	XLUnicodeString st;

};

} // namespace XLS

