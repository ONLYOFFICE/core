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
#include <Logic/Biff_structures/BorderFillInfo.h>

namespace XLS
{


// Logical representation of Font record in BIFF8
class Font: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Font)
	BASE_OBJECT_DEFINE_CLASS_NAME(Font)
public:
	Font();
	~Font();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeFont;

	int serialize				(std::wostream & stream);
	int serialize_properties	(std::wostream & stream, bool rPr = false);
	int serialize_rPr			(std::wostream & stream, bool rtl = false, bool defRPr = false, std::wstring namespace_ = L"a:");

	void set_color_ext			(FillInfoExt & color_ext);

	GlobalWorkbookInfoPtr global_info;

//-----------------------------
	_UINT16 dyHeight; // measured in twips (1/20 of of a printer's point)

	bool fItalic;
	bool fStrikeOut;
	bool fOutline;
	bool fShadow;
	bool fCondense;
	bool fExtend;

	_UINT16 icv;
	_UINT16 bls;
	_UINT16 sss;
	unsigned char	uls;
	unsigned char	bFamily;
	unsigned char	bCharSet;

	FillInfoExt				color_ext;
	ShortXLUnicodeString	fontName;

	bool					correct;
	
};

} // namespace XLS

