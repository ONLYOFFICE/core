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

#include <vector>
#include <sstream>

#include "../XlsElementsType.h"

#include "../../../../Common/Utils/OptPtr.h"
#include "../../Logging/Log.h"

#include "../../Binary/CFRecord.h"

namespace XLS
{

	class BiffStructure;
	typedef boost::shared_ptr<BiffStructure>	BiffStructurePtr;
	typedef std::vector<BiffStructurePtr>		BiffStructurePtrVector;

	class BiffStructure
	{
	public:
		virtual ~BiffStructure() {}
		virtual BiffStructurePtr clone() = 0;

		virtual void load(CFRecord& record) = 0;
		virtual void load(IBinaryReader* reader) {}

		virtual void save(CFRecord& record) {}//= 0;

		virtual ElementType get_type() = 0;

		virtual int serialize(std::wostream & _stream)
		{
			std::stringstream s;
			s << std::string("This element - ") << getClassName() << std::string("- not serialize");
			Log::warning(s.str());
			return 0;
		}

		virtual const std::string & getClassName() const = 0;   // The return value must be a reference to a static variable inside the getter
	};

#define BASE_STRUCTURE_DEFINE_CLASS_NAME(class_name)\
	public: \
	const std::string & getClassName() const { static std::string str(#class_name); return str; }\
	virtual XLS::ElementType get_type() { return type; }

	template<typename T, typename = typename std::enable_if<!std::is_base_of<BiffStructure, T>::value, T>::type>
	CFRecord& operator>>(CFRecord& record, T& val)
	{
		record.loadAnyData(val);
		return record;
	}
	template<typename T, typename = typename std::enable_if<!std::is_base_of<BiffStructure, T>::value, T>::type>
	CFRecord& operator<<(CFRecord& record, T& val)
	{
		record.storeAnyData(val);
		return record;
	}

	CFRecord& operator>>(CFRecord& record, BiffStructure& val);
	CFRecord& operator<<(CFRecord& record, BiffStructure& val);

} // namespace XLS

