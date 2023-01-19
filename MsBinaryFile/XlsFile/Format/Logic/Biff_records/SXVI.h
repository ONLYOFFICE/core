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
#include "../Biff_structures/BiffString.h"

namespace XLS
{
class SXVI: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXVI)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXVI)
public:
	SXVI();
	~SXVI();

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record);

	int serialize(std::wostream & strm);

	static const ElementType type = typeSXVI;

	unsigned short			itmType; //enum
//0x0000	itmtypeData
//0x0001	itmtypeDEFAULT
//0x0002	itmtypeSUM
//0x0003	itmtypeCOUNTA
//0x0004	itmtypeAVERAGE
//0x0005	itmtypeMAX
//0x0006	itmtypeMIN
//0x0007	itmtypePRODUCT
//0x0008	itmtypeCOUNT
//0x0009	itmtypeSTDEV
//0x000A	itmtypeSTDEVP
//0x000B	itmtypeVAR
//0x000C	itmtypeVARP

	bool					fHidden;
	bool					fHideDetail;
	bool					fFormula;
	bool					fMissing;

	short					iCache;
	unsigned short			cchName;
	XLUnicodeStringNoCch	stName;

};

} // namespace XLS

