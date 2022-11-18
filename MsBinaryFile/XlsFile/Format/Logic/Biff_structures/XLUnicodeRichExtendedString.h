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

	GlobalWorkbookInfoPtr pGlobalWorkbookInfoPtr;

	void loadSymbols(CFRecord& record, const size_t cch, const bool is_wide);

	const size_t getNonVariablePartSize() const;
	const size_t getFullSize() const;

	const bool appendNextContinue(CFRecord& record, const bool read_high_byte);

	std::wstring  str_;

	bool fHighByte;
	bool fExtSt;
	bool fRichSt;
	
	std::vector<FormatRun> rgRun;
	
	ExtRst extRst;
	std::list<CFRecordPtr>& cont_recs_;
	bool mark_set_start;

	unsigned short code_page_ ;
};

typedef boost::shared_ptr<XLUnicodeRichExtendedString> XLUnicodeRichExtendedStringPtr;

CFRecord& operator>>(CFRecord& record, XLUnicodeRichExtendedString& val);

} // namespace XLS
