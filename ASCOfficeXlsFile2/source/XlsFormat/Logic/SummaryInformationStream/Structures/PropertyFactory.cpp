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


#include <Binary/CFStream.h>
#include "PropertyFactory.h"
#include "CodePageOle.h"


namespace OLEPS
{

PropertyFactory::PropertyFactory()
{
}

PropertyPtr PropertyFactory::ReadProperty(const unsigned int prop_type, XLS::CFStreamPtr stream, const unsigned int property_offset)
{
	stream->seekFromBegin(property_offset);
	unsigned short value_type;

	if (stream->getStreamPointer() +2 > stream->getStreamSize()) return PropertyPtr();
	*stream >> value_type;
	stream->seekFromCurForward(2); // Skip 2 reserved unsigned chars

	switch(prop_type)
	{
        case 0x01://CodePage::Type:
            return PropertyPtr(new PropertyCodePage(value_type, stream));
        case 0x02://TITLE
            return PropertyPtr(new PropertyTitle(value_type, stream));
        case 0x03://SUBJECT
            return PropertyPtr(new PropertySubject(value_type, stream));
        case 0x04://AUTHOR
            return PropertyPtr(new PropertyAuthor(value_type, stream));
        case 0x05://KEYWORDS
            return PropertyPtr(new PropertyKeywords(value_type, stream));
        case 0x06://COMMENTS
            return PropertyPtr(new PropertyComments(value_type, stream));
        case 0x0C://CREATE_DTM
            return PropertyPtr(new PropertyDateCreate(value_type, stream));

		default:
			return PropertyPtr();
	}

}



} // namespace OLEPS
