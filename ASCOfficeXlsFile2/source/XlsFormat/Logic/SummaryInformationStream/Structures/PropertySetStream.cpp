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

#include "PropertySetStream.h"
#include "Binary/CFStream.h"

#include "Auxiliary/HelpFunc.h"

namespace OLEPS
{

PropertySetStream::PropertySetStream(XLS::CFStreamPtr stream)
{
	unsigned short	ByteOrder=0;
	unsigned short	Version=0;
	_UINT32			SystemIdentifier=0;
	_UINT32			NumPropertySets=0;
	_GUID_			Clsid={};
	
	*stream >> ByteOrder >> Version >> SystemIdentifier;

	if (ByteOrder != 0xFFFE) //file (53).xls
		return;

	if (Version != 0x01 && Version != 0x00) 
		return;

	*stream >> SystemIdentifier >> Clsid;
	
	*stream >> NumPropertySets;

	if (NumPropertySets != 0x01 && NumPropertySets != 0x02) 
	{
		NumPropertySets = 0x01;
		stream->seekFromBegin(28);
	}

	std::vector<_UINT32> property_sets_offsets;
	for(_UINT32 i = 0; i < NumPropertySets; ++i)
	{
		_GUID_ FMTID;
		
		_UINT32 Offset;
		*stream >> FMTID >> Offset;
		
		if (Offset < stream->getStreamSize())
			property_sets_offsets.push_back(Offset);
	}

	for (_UINT32 i = 0; i < NumPropertySets; ++i)
	{
		PropertySetPtr set = PropertySetPtr(new PropertySet(stream, property_sets_offsets[i]));
		
		property_sets.push_back(set);
	}
}

const unsigned short PropertySetStream::GetCodePage()
{
	return 0 != property_sets.size() ? property_sets[0]->GetCodePage() : 0;
}

} // namespace OLEPS
