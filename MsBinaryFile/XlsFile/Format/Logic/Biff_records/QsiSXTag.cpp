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

#include "QsiSXTag.h"

namespace XLS
{

QsiSXTag::QsiSXTag()
{
}


QsiSXTag::~QsiSXTag()
{
}


BaseObjectPtr QsiSXTag::clone()
{
	return BaseObjectPtr(new QsiSXTag(*this));
}

void QsiSXTag::readFields(CFRecord& record)
{
	unsigned char	reserved1;
	unsigned short	flags, reserved2;

	record >> frtHeaderOld >> fSx >> flags;

	fEnableRefresh	= GETBIT(flags, 0);
	fInvalid		= GETBIT(flags, 1);
	fTensorEx		= GETBIT(flags, 2);
	
	record >> dwQsiFuture >> verSxLastUpdated >> verSxUpdatableMin >> obCchName >> reserved1;
	record >> stName;

	if (fSx)
	{
		//dwQsiFuture is SXView9Save
		fNoStencil			= GETBIT(flags, 0);
		fHideTotAnnotation	= GETBIT(flags, 1);
		//reserved1			= GETBIT(flags, 2);
		fIncludeEmptyRw		= GETBIT(flags, 3);
		fIncludeEmptyCol	= GETBIT(flags, 4);
	}
	else
	{
		//dwQsiFuture is DwQsiFuture
		fPreserveFmt	= GETBIT(flags, 0);
		fAutoFit		= GETBIT(flags, 1);
		//reserved1		= GETBIT(flags, 2);
		//reserved2		= GETBIT(flags, 3);
		fExtDataList	= GETBIT(flags, 4);
		//reserved3		= GETBIT(flags, 5);
		fCreateQTList	= GETBIT(flags, 6);
		fDummyList		= GETBIT(flags, 7);
	}

	record >> reserved2;

	int skip = record.getDataSize() - record.getRdPtr();
	record.skipNunBytes(skip);
}

} // namespace XLS

