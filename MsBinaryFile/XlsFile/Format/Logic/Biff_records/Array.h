﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "../Biff_structures/CellRangeRef.h"
#include "../Biff_structures/ArrayParsedFormula.h"

namespace XLS
{

// Logical representation of Array record in BIFF8 and BrtArrFmla record in biff12
class Array: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Array)
	BASE_OBJECT_DEFINE_CLASS_NAME(Array)
public:
	Array(const CellRef& cell_base_ref_init);
	~Array();

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record) override;
	void writeFields(CFRecord& record) override;
	
	static const ElementType	type = typeArray;

    Ref                 ref_;
    ArrayParsedFormula  formula;
    bool                fAlwaysCalc;

    RFX                 rfx; //biff12
};

typedef boost::shared_ptr<Array> ArrayPtr;

} // namespace XLS

