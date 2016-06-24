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

#include "ExternSheet.h"
#include <Logic/Biff_structures/XTI.h>
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{

ExternSheet::ExternSheet()
{
}


ExternSheet::~ExternSheet()
{
}


BaseObjectPtr ExternSheet::clone()
{
	return BaseObjectPtr(new ExternSheet(*this));
}


void ExternSheet::writeFields(CFRecord& record)
{
	cXTI = rgXTI.size();
	record << cXTI;
	for(BiffStructurePtrVector::const_iterator it = rgXTI.begin(), itEnd = rgXTI.end(); it != itEnd; ++it)
	{
		record << **it;
	}
}


void ExternSheet::readFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		unsigned char type;
		//record.skipNunBytes(record.getDataSize() - record.getRdPtr());
		ShortXLAnsiString stName;
		record >> type >> stName;

		std::wstring	name	= stName.value();
		//int				type	= stName.value().substr(0, 1).c_str()[0];
		if (!name.empty())
			record.getGlobalWorkbookInfo()->arExternalNames.push_back(name);

	}
	else
	{
		record >> cXTI;
		for(int i = 0; i < cXTI; ++i)
		{
			XTIPtr xti(new XTI);
			record >> *xti;
			rgXTI.push_back(xti);
		}
	}
}

} // namespace XLS

