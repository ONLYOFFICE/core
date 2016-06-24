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


#include "PropertySet.h"
#include <Binary/CFStream.h>
#include "PropertyFactory.h"
#include "CodePageOle.h"


namespace OLEPS
{

PropertySet::PropertySet(XLS::CFStreamPtr stream, const unsigned int property_set_offset)
{
	stream->seekFromBegin(property_set_offset);

	unsigned int Size;
	*stream >> Size;
	unsigned int NumProperties;
	*stream >> NumProperties;
	
	std::vector<PropertyIdentifierAndOffset> prop_offsets;
	for(unsigned int i = 0; i < NumProperties; ++i)
	{
		if (stream->getStreamPointer() + 8 > stream->getStreamSize())
			break;
		PropertyIdentifierAndOffset prop_offset;
		*stream >> prop_offset;

		if (prop_offset.Offset - property_set_offset > Size)
			break;
		prop_offsets.push_back(prop_offset);
	}

    code_page = PropertyCodePage::DefaultCodePage;
	for(unsigned int i = 0; i < prop_offsets.size(); ++i)
	{
		if (stream->getStreamPointer() - property_set_offset > Size)
			break;
		PropertyPtr next_property = PropertyFactory::ReadProperty(prop_offsets[i].PropertyIdentifier, stream, property_set_offset + prop_offsets[i].Offset);
		if(next_property) // Skip the property if the corresponding class is not implemented
		{
			properties.push_back(next_property);

            PropertyCodePagePtr property_CodePage = boost::dynamic_pointer_cast<PropertyCodePage>(next_property);
			if(property_CodePage)
			{
				code_page = property_CodePage->code_page;
			}
		}
	}

}

const unsigned short PropertySet::GetCodePage()
{
	return code_page;
}

} // namespace OLEPS
