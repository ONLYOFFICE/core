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
#pragma once

#include "OfficeArtRecord.h"
#include "OfficeArtRecordHeader.h"
#include "OfficeArtBlip.h"


namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{

class OfficeArtBStoreContainerFileBlock : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtBStoreContainerFileBlock)
public:
	XLS::BiffStructurePtr clone();

	OfficeArtBStoreContainerFileBlock()
	{
		pict_size = 0;
		pict_data = NULL;
		pict_type = L".jpeg";

		rgbUid1 = L"";
		rgbUid2 = L"";
		result = false;
	}

	~OfficeArtBStoreContainerFileBlock()
	{
		if (pict_data)
		{
			delete[] pict_data;
			pict_data = NULL;
		}
	}
	
	virtual void load(XLS::CFRecord& record);
	virtual void save(XLS::CFRecord& record);


	static const XLS::ElementType	type = XLS::typeOfficeArtBStoreContainerFileBlock;

	void readCompressedData(XLS::CFRecord& record, OfficeArtMetafileHeader & metafileHeader);

	int				bin_data_id;

	size_t			pict_size;
	std::wstring	pict_type;
	char			*pict_data;

	size_t			recType;	
	std::wstring	rgbUid1;
	std::wstring	rgbUid2;
	std::wstring	nameData;
	bool			result;

	const std::wstring ReadMD4Digest(XLS::CFRecord& record)
	{
		std::wstring result;
		for(int i = 0; i < 16; i++)
		{
			unsigned char hex_data;
			record >> hex_data;
			result += STR::int2hex_wstr(hex_data, sizeof(hex_data));
		}
		return result;
	}

};

typedef boost::shared_ptr<OfficeArtBStoreContainerFileBlock> OfficeArtBStoreContainerFileBlockPtr;


} // namespace XLS
