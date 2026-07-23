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


#include "PropertyFactory.h"
#include "CodePageOle.h"
#include "../../XlsFile/Format/Binary/CFStream.h"

namespace OLEPS
{

PropertyPtr PropertyFactory::ReadProperty(const unsigned int prop_id, XLS::CFStreamPtr stream, const unsigned int property_offset)
{
	stream->seekFromBegin(property_offset);
	unsigned short value_type;

	if (stream->getStreamPointer() + 2 > stream->getStreamSize()) return PropertyPtr();
	*stream >> value_type;
	stream->seekFromCurForward(2); // Skip 2 reserved unsigned chars

	value_type = value_type & 0x00ff;

	PropertyPtr property_;

	if (prop_id == CODEPAGE || prop_id == CODEPAGEEXT)
	{
		property_ = PropertyPtr(new PropertyCodePage(prop_id, value_type));
		property_->Read(stream);
		
		PropertyCodePagePtr property_CodePage = boost::dynamic_pointer_cast<PropertyCodePage>(property_);
		if(property_CodePage)
		{
			code_page = property_CodePage->code_page;
		}
	}
	else if (prop_id == HEADINGPAIR)
	{
		property_ = PropertyPtr(new PropertyVecHeadingPair(prop_id, value_type, code_page));
	}
	else if (prop_id == DOCPARTS)
	{
		property_ = PropertyPtr(new PropertyVecString(prop_id, value_type, code_page));
	}
	else if (prop_id == DIGSIG)
	{
		property_ = PropertyPtr(new PropertyDigSig(prop_id, value_type));
	}
	else
	{
		if (value_type == Property::VT_LPSTR)
		{
			property_ = PropertyPtr(new PropertyString(prop_id, value_type, code_page));
		}
		else if (value_type == Property::VT_LPWSTR)
		{
			property_ = PropertyPtr(new PropertyWString(prop_id, value_type));
		}
		else if (value_type == Property::VT_I4)
		{
			property_ = PropertyPtr(new PropertyInt(prop_id, value_type));
		}
		else if (value_type == Property::VT_BOOL)
		{
			property_ = PropertyPtr(new PropertyBool(prop_id, value_type));
		}
		else if (value_type == Property::VT_FILETIME)
		{
			property_ = PropertyPtr(new PropertyDTM(prop_id, value_type));
		}
		else
		{
			//skip
		}
	}
	if (property_)
	{
		property_->Read(stream);
	}

	return property_;
}

} // namespace OLEPS
