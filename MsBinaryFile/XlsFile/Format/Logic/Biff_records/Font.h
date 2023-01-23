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

#include "BiffRecord.h"
#include "../Biff_structures/BorderFillInfo.h"
#include "../../../../../OOXML/XlsbFormat/Biff12_records/Color.h"

namespace XLS
{

class Font: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Font)
	BASE_OBJECT_DEFINE_CLASS_NAME(Font)
public:
	Font();
	~Font();

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record);
	void set(FontInfo & info);

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
	_UINT16 bls = 0;
	_UINT16 sss = 0;
	unsigned char uls = 0;
	unsigned char bFamily = 0;
	unsigned char bCharSet = 0;

    std::wstring fontName;
	FillInfoExt color_ext;

	bool correct;

 //xlsb
    XLSB::Color brtColor;
    unsigned char bFontScheme;
	
};
class Font_BIFF34 : public Font
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Font_BIFF34)
	BASE_OBJECT_DEFINE_CLASS_NAME(Font_BIFF34)
public:
	Font_BIFF34();
	~Font_BIFF34();

	BaseObjectPtr clone();
};
//0045H 5.47 FONTCOLOR
} // namespace XLS

