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

#include "../../../../Common/3dParty/pole/pole.h"

#include "BinSmartPointers.h"

#include <map>

namespace XLS
{


class CompoundFile
{
public:
	enum ReadWriteMode
	{
		cf_WriteMode,
		cf_ReadMode
	};

public:
	CompoundFile(const std::wstring & file_path, const ReadWriteMode mode);
	~CompoundFile();

	bool Open(const std::wstring & file_path, const ReadWriteMode mode);
	
	bool isError();

	void copy( int indent, std::wstring path, POLE::Storage * storageOut, bool bWithRoot = true, bool bSortFiles = false);

	CFStreamPtr getWorkbookStream	();
	CFStreamPtr getNamedStream		(const std::wstring& name);

	POLE::Storage *storage_;

	CFStreamPtr createNamedStream	(const std::wstring& name);
	void		closeNamedStream	(const std::wstring& name);
private:
	void copy_stream(std::wstring streamNameOpen, std::wstring streamNameCreate, POLE::Storage * storageOut, bool bWithRoot = true);
	
	POLE::Stream* openStream		(const std::wstring & stream_name); // Opens a stream in the storage (shall be called not more than once per stream)
	POLE::Stream* createStream		(const std::wstring & stream_name); // Creates a new stream in the storage


	std::map<std::wstring, CFStreamPtr>	streams;
	ReadWriteMode						rwMode;
};
typedef boost::shared_ptr<CompoundFile> CompoundFilePtr;


} // namespace XLS

