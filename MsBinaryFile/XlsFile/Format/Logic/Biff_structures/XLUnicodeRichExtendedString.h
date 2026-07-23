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

#include "BiffStructure.h"
#include "FormatRun.h"
#include "ExtRst.h"

namespace XLS
{

class GlobalWorkbookInfo;
typedef boost::shared_ptr<GlobalWorkbookInfo> GlobalWorkbookInfoPtr;

class XLUnicodeRichExtendedString : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XLUnicodeRichExtendedString)
public:
	XLUnicodeRichExtendedString(std::list<CFRecordPtr>& cont_recs);
	~XLUnicodeRichExtendedString();
	XLUnicodeRichExtendedString operator=(const XLUnicodeRichExtendedString& other);

	static const ElementType	type = typeXLUnicodeRichExtendedString;

	BiffStructurePtr clone();

	int serialize		(std::wostream & _stream);	
	int serialize_rPr	(std::wostream & _stream, int iFmt);

	void set_code_page(short cp) ;
	
	virtual void load	(CFRecord& record);
    virtual void save	(CFRecord& record);

	GlobalWorkbookInfoPtr pGlobalWorkbookInfoPtr;

	void loadSymbols(CFRecord& record, const size_t cch, const bool is_wide);

	const size_t getNonVariablePartSize() const;
	const size_t getFullSize() const;

	const bool appendNextContinue(CFRecord& record, const bool read_high_byte);

	std::wstring  str_;

    bool fHighByte = true;
    bool fExtSt = false;
    bool fRichSt = false;
	
	std::vector<FormatRun> rgRun;
	
	ExtRst extRst;
	std::list<CFRecordPtr>& cont_recs_;
    bool mark_set_start = false;

    unsigned short code_page_ = 0 ;
};

typedef boost::shared_ptr<XLUnicodeRichExtendedString> XLUnicodeRichExtendedStringPtr;

CFRecord& operator>>(CFRecord& record, XLUnicodeRichExtendedString& val);

} // namespace XLS
