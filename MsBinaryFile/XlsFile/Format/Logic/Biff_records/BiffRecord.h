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

#include "../BaseObject.h"
#include "../Biff_structures/BitMarkedStructs.h"
#include "../BinProcessor.h"

namespace XLS
{

class CFStream;

class BiffRecord: public BaseObject
{
public:
	BiffRecord();
	~BiffRecord();

	virtual const bool read(StreamCacheReaderPtr reader, BaseObject* parent, const bool is_mandatory); // Read self and children

	virtual const bool write(StreamCacheWriterPtr writer, BaseObject* parent); // Write self and children
	
	virtual void readFields(CFRecord& record) {}//= 0;

	virtual void writeFields(CFRecord& record) {}// = 0;

	virtual const CFRecordType::TypeId getTypeId() const = 0;  // made virtual to let derived assign their own names (for example, EOF_T)

	static const ElementType	type = typeBiffRecord;
	virtual ElementType			get_type() { return type; }
//-----------------------------
	virtual void readFollowingContinue(StreamCacheReaderPtr reader);
	virtual const bool storeRecordAndDecideProceeding(CFRecordPtr record); // This function is overridden in BiffRecordSplit

protected:
	BaseObject* parent_;
};
} // namespace XLS

#define BIFF_RECORD_DEFINE_TYPE_INFO(class_name) public: const XLS::CFRecordType::TypeId getTypeId() const { return rt_ ## class_name; }
