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
