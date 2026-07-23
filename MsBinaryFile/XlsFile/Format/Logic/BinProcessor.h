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

#include "BaseObject.h"
#include "Biff_structures/BiffAttribute.h"
#include "Biff_structures/BiffStructure.h"
#include "GlobalWorkbookInfo.h"

namespace XLS
{

class BiffString;

class BinProcessor
{
public:
	BinProcessor(BaseObject* parent, GlobalWorkbookInfoPtr global_info);
	BinProcessor(GlobalWorkbookInfoPtr global_info);
	~BinProcessor();

	template<class T>
    const bool optional()
    {
        T obj;
        return optional(obj);
    }

	template<class T>
    const bool mandatory()
    {
        T obj;
        return mandatory(obj);
    }

	// Load the specified child object specified number of times
	// fromN = 0 means the object is optional
	// toN = 0 means no upper limit
	template<class T>
    const int repeated(const int fromN, const int toN)
    {
        T obj;
        return repeated(obj, fromN, toN);
    }

    const int           repeated	(BaseObject& object, const int fromN, const int toN);
    virtual const bool  optional	(BaseObject& object) = 0;
    virtual const bool  mandatory	(BaseObject& object) = 0;

	// Universal check if the we have the specified record in our processing plans (in binary it is not more than 'num_records_to_check' records forward)
	virtual const bool checkNextRecord(const CFRecordType::TypeId desirable_type, const size_t num_records_to_check) = 0;
	virtual const CFRecordType::TypeId getNextRecordType() = 0;

	virtual const bool getNextSubstreamType(_UINT16& type) = 0;
	virtual void SeekToEOF() = 0;
	virtual void SkipRecord(bool log_debug = true) = 0;
    virtual const int GetRecordPosition() = 0;
    virtual void SetRecordPosition(const int position) = 0;

	bool isBOF(CFRecordType::TypeId type);

	BaseObject* getParent() const { return parent_;};
	GlobalWorkbookInfoPtr getGlobalWorkbookInfo() const { return global_info_;};
protected:
	BaseObject*				parent_;
	GlobalWorkbookInfoPtr	global_info_;
};


class BinReaderProcessor : public BinProcessor
{
public:
	BinReaderProcessor(StreamCacheReaderPtr reader, BaseObject* parent, const bool is_mandatory);

	virtual const bool	optional	(BaseObject& object);
	virtual const bool	mandatory	(BaseObject& object);

	const bool checkNextRecord(const CFRecordType::TypeId desirable_type, const size_t num_records_to_check);

	const CFRecordType::TypeId	getNextRecordType	();
	const bool					getNextSubstreamType(_UINT16& type);
	
	void SeekToEOF();
	virtual void SkipRecord(bool log_debug = true);
    const int GetRecordPosition();
    void SetRecordPosition(const int position);

private:
	const bool readChild(BaseObject& object, const bool is_mandatory);

	StreamCacheReaderPtr	reader_;
	BaseObjectPtrList		wanted_objects;
	bool			is_mandatory_;
};

class BinWriterProcessor : public BinProcessor
{
public:
	BinWriterProcessor(StreamCacheWriterPtr writer, BaseObject* parent);

	const bool	optional(BaseObject& object) override;
	const bool	mandatory(BaseObject& object) override;

	virtual const bool checkNextRecord(const CFRecordType::TypeId desirable_type, const size_t num_records_to_check) override { return false; }
	virtual const CFRecordType::TypeId getNextRecordType() override { return CFRecordType::ANY_TYPE; }

	virtual const bool getNextSubstreamType(_UINT16& type) override { return false; }
	virtual void SeekToEOF() override {}
	virtual void SkipRecord(bool log_debug = true) override {}
	virtual const int GetRecordPosition() override;
	virtual void SetRecordPosition(const int position) override {}

private:
	const bool writeChild(BaseObject& object, const bool is_mandatory);

	StreamCacheWriterPtr	writer_;
};


} // namespace XLS
