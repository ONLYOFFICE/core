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

#include "Ptg.h"

namespace XLS
{

class OperandPtg : public Ptg
{
public:
	OperandPtg() {};
	OperandPtg(const unsigned short ptg_id_init) : Ptg(ptg_id_init) {};
	enum PtgDataType
	{
		pdt_NO_TYPE_DATA = 0x0,	// Error value or type data is absent
		pdt_REFERENCE = 0x1,	// Specifies a reference to a range.
		pdt_VALUE = 0x2,		// Specifies a single value of a simple type. The type can be a Boolean, a number, a string, or an error code.
		pdt_ARRAY = 0x3			// Specifies an array of values.
	};


protected:

};

typedef boost::shared_ptr<OperandPtg> OperandPtgPtr;

} // namespace XLS

