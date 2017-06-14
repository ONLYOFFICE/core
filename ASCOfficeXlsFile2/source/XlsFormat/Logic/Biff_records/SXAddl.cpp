/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "SXAddl.h"

namespace XLS
{

SXAddl::SXAddl()
{
}


SXAddl::~SXAddl()
{
}


BaseObjectPtr SXAddl::clone()
{
	return BaseObjectPtr(new SXAddl(*this));
}

void SXAddl::readFields(CFRecord& record)
{
	m_SXAddlHdr.load(record);

	Log::error("SXAddl record is not implemented.");
	record.skipNunBytes(record.getDataSize() - record.getRdPtr());
}
//-------------------------------------------------------------------------------------------------------
void SXAddl_SXCCache_SXDId::readFields(CFRecord& record)
{
	m_SXAddlHdr.load(record);
	
	short reserved;
	record >> idCache >> reserved;
}

void SXAddl_SXCCache_SXDEnd::readFields(CFRecord& record)
{
	m_SXAddlHdr.load(record);

	_UINT32	reserved1;
	short	reserved2;	
	record >> reserved1 >> reserved2;
}

void SXAddl_SXCCache_SXDVer10Info::readFields(CFRecord& record)
{
	m_SXAddlHdr.load(record);

	_UINT32	reserved1;
	short	reserved2;	
	record >> reserved1 >> reserved2;

	record >> citmGhostMax >> bVerCacheLastRefresh >> bVerCacheRefreshableMin;

	for (int i = 0; i < 8; i++)
		record >> numDateCopy[i];

	record >> reserved2;
}
void SXAddl_SXCCache_SXDInfo12::readFields(CFRecord& record)
{
	m_SXAddlHdr.load(record);

	_UINT32	flags;
	short	reserved;	
	
	record >> flags >> reserved;
	
	fSheetData				= GETBIT(flags, 0);
	fSrvSupportAttribDrill	= GETBIT(flags, 1);
	fSrvSupportSubQuery		= GETBIT(flags, 2);
}
void SXAddl_SXCCache_SXDVerSXMacro::readFields(CFRecord& record)
{
	m_SXAddlHdr.load(record);

	unsigned char	reserved1;
	short			reserved2, reserved3;	
	
	record >> dwVer >> reserved1 >> reserved2 >> reserved3;
}

} // namespace XLS

