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

#include "BiffRecord.h"
#include "../Biff_structures/BorderFillInfo.h"
#include "../Biff_structures/BIFF12/Color.h"

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
	
	void readFields(CFRecord& record) override;
	void writeFields(CFRecord& record) override;
	void set(FontInfo & info);

	static const ElementType	type = typeFont;

	int serialize				(std::wostream & stream);
	int serialize_properties	(std::wostream & stream, bool rPr = false);
	int serialize_rPr			(std::wostream & stream, bool rtl = false, bool defRPr = false, std::wstring namespace_ = L"a:");

	void set_color_ext			(FillInfoExt & color_ext);

	GlobalWorkbookInfoPtr global_info;

//-----------------------------
	_UINT16 dyHeight = 220; // measured in twips (1/20 of of a printer's point)

	bool fItalic = false;
	bool fStrikeOut = false;
	bool fOutline = false;
	bool fShadow = false;
	bool fCondense = false;
	bool fExtend = false;

	_UINT16 icv = 0;
	_UINT16 bls = 400;
	_UINT16 sss = 0;
	unsigned char uls = 0;
	unsigned char bFamily = 0x02;
	unsigned char bCharSet = 0xCC;

	std::wstring fontName = L"calibri";
	FillInfoExt color_ext;

	bool correct = true;

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

