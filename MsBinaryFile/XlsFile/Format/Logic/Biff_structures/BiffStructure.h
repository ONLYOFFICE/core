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
#pragma once

#include <vector>
#include <sstream>

#include "../XlsElementsType.h"

#include "../../../../Common/Utils/OptPtr.h"
#include "../../Logging/Log.h"

#include "../../Binary/CFRecord.h"

namespace XLS
{

class BiffStructure_NoVtbl
{
public:
	void load(CFRecord& record); // this function will never be called ( look at operator>>(CFRecord& record, T& val))
	void load(IBinaryReader* reader);
};

class BiffStructure;
typedef boost::shared_ptr<BiffStructure>	BiffStructurePtr;
typedef std::vector<BiffStructurePtr>		BiffStructurePtrVector;

class BiffStructure : protected BiffStructure_NoVtbl
{
public:

	virtual BiffStructurePtr clone() = 0;

	virtual void load(CFRecord& record) = 0;
	virtual void load(IBinaryReader* reader)
	{
	}

	virtual ElementType get_type() = 0;

	virtual int serialize(std::wostream & _stream)
	{
        std::stringstream s;
        s << std::string("This element - ") << getClassName() << std::string("- not serialize");
		Log::warning(s.str());
		return 0;
	}

    virtual const std::string & getClassName() const = 0;   // Must be overridden in every deriver. The return value must be a reference to a static variable inside the getter

};

#define BASE_STRUCTURE_DEFINE_CLASS_NAME(class_name)\
	public: \
        const std::string & getClassName() const { static std::string  str(#class_name); return str;}\
		virtual XLS::ElementType get_type() { return type; }
	


bool DiffBiff(BiffStructure_NoVtbl & val);
bool DiffBiff(BiffStructure & val);


template<class T>
CFRecord& operator>>(CFRecord& record, T& val)
{
	if(DiffBiff(val))
	{
		record.loadAnyData(val);
	}
	else
	{
		val.load(record);
	}
	return record;
}


} // namespace XLS

