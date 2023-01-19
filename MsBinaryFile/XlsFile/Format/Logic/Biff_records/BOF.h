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

namespace XLS
{

class StreamCacheReader;

class BOF: public BiffRecord
{
	//BIFF_RECORD_DEFINE_TYPE_INFO(BOF)
	BASE_OBJECT_DEFINE_CLASS_NAME(BOF)
public:
	BOF();
	~BOF();

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record);

	static const ElementType type = typeBOF;

	unsigned short getSubstreamType();

	const CFRecordType::TypeId getTypeId() const
	{
		return type_id_;
	}

	typedef enum
	{
		st_Workbook		= 0x0005,
		st_Globals		= st_Workbook,
		st_Dialog		= 0x0010,
		st_Worksheet	= st_Dialog,
		st_Chart		= 0x0020,
		st_Macro		= 0x040
	} SubstreamType;


//-----------------------------
	_UINT16 vers = 0;
	_UINT16 dt = 0;
	_UINT16 rupBuild = 0;
	_UINT16 rupYear = 0;
	
	bool fWin;
	bool fRisc;
	bool fBeta;
	bool fWinAny;
	bool fMacAny;
	bool fBetaAny;
	bool fRiscAny;
	bool fOOM;
	bool fGlJmp;
	bool fFontLimit;
	
	_UINT16 verXLHigh;
	unsigned char	verLowestBiff;
	unsigned char	verLastXLSaved;
	
	ForwardOnlyParam<unsigned int> stream_ptr;

	CFRecordType::TypeId type_id_ = rt_BOF_BIFF8;	
};

} // namespace XLS

