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
#pragma once

#include "BaseObject.h"
#include "Biff_structures/BiffAttribute.h"
#include "Biff_structures/BiffStructure.h"
#include "Biff_structures/BiffStructure.h"
#include "GlobalWorkbookInfo.h"

namespace XLS
{

class BiffString;
class CFStreamCacheWriter;

// Logical representation of composite objects
//
// Another new function is to buffer objects, that had to be read but was absent in the bin stream.
// The buffered records shall be checked for existence later.
// This feature is specially for files saved in OpenOffice Calc those don't conform to the Microsoft specification
// Call them "wanted objects"
//
class BinProcessor
{
public:
	BinProcessor(BaseObject* parent, GlobalWorkbookInfoPtr global_info);
	BinProcessor(GlobalWorkbookInfoPtr global_info);
	~BinProcessor();

	// This function doesn't 
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

    const int           repeated(BaseObject& object, const int fromN, const int toN);
    virtual const bool  optional(BaseObject& object) = 0;
    virtual const bool  mandatory(BaseObject& object) = 0;

	// Universal check if the we have the specified record in our processing plans (in binary it is not more than 'num_records_to_check' records forward)
	virtual const bool checkNextRecord(const CFRecordType::TypeId desirable_type, const size_t num_records_to_check) = 0;
	// Determine the next substream type
	virtual const CFRecordType::TypeId getNextRecordType() = 0;

	virtual const bool getNextSubstreamType(_UINT16& type) = 0;
	virtual void SeekToEOF() = 0;
	virtual void SkipRecord() = 0;

	BaseObject* getParent() const { return parent_;};
	GlobalWorkbookInfoPtr getGlobalWorkbookInfo() const { return global_info_;};
protected:
	BaseObject*				parent_;
	GlobalWorkbookInfoPtr	global_info_;
};


class BinReaderProcessor : public BinProcessor
{
public:
	BinReaderProcessor(CFStreamCacheReader& reader, BaseObject* parent, const bool is_mandatory);
	//BinReaderProcessor(CFStreamCacheReader& reader, const bool is_mandatory); //root ???

	virtual const bool optional(BaseObject& object);
	virtual const bool mandatory(BaseObject& object);

	// Check if the next read record would be of desired type
	const bool checkNextRecord(const CFRecordType::TypeId desirable_type, const size_t num_records_to_check);
	// Assume that the next record is BOF (if not - return false) and get the type without extracting is from cache
	// In the case of stream end returns false
	const CFRecordType::TypeId getNextRecordType();
	const bool getNextSubstreamType(_UINT16& type);
	void SeekToEOF();
	void SkipRecord();

private:
	const bool readChild(BaseObject& object, const bool is_mandatory);

private:
	CFStreamCacheReader& reader_;
	// objects that are "WANTED", i.e. the objects that weren't read at the right moment and may be read later
	BaseObjectPtrList wanted_objects;
	bool is_mandatory_;
};


} // namespace XLS
