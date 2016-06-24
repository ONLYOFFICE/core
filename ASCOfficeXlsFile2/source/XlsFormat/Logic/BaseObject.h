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

#include <boost/shared_ptr.hpp>
#include <list>
#include <sstream>

#include "Logging/Log.h"
#include "XlsElementsType.h" //шоб со строками не работать
#include <simple_xml_writer.h>


namespace XLS
{

class CFStream;
class BinProcessor;
//class BinWriterProcessor;
class CFStreamCacheReader;

class BaseObject;
typedef boost::shared_ptr<BaseObject>	BaseObjectPtr;

// Logical representation of an XML record
class BaseObject //: public boost::enable_shared_from_this<BaseObject>
{
public:
	BaseObject(){}
	~BaseObject(){}

	//BaseObjectPtr get_BaseObjectPtr()  { return shared_from_this();  }

	virtual boost::shared_ptr<BaseObject> clone() = 0;

	virtual const bool read(CFStreamCacheReader& reader, BaseObject* parent, const bool mandatory) = 0; // Read self and children

	//virtual void toFrom(BinProcessor& proc) = 0; // This function shall contain only mark functions and is the universal XML serialiser

	virtual const std::string & getClassName() const = 0; // Must be overridden in every deriver. The return value must be a reference to a static variable inside the getter

	void add_child (BaseObjectPtr e) {elements_.push_back(e);}
	
	std::list<BaseObjectPtr> elements_;

	virtual ElementType get_type() = 0;

	virtual int serialize(std::wostream & _stream)
	{
		std::stringstream s;
		s << std::string("This element - ") << getClassName() << std::string("- not serialize");
		Log::warning(s.str());
		return 0;
	}

	virtual void serialize_attribute(CP_ATTR_NODE){}
};

#define BASE_OBJECT_DEFINE_CLASS_NAME(class_name)\
public: const std::string & getClassName() const { static std::string  str(#class_name); return str; };\
virtual ElementType get_type() { return type; }

#define BASE_OBJECT_DEFINE_CLASS_TYPE(class_name) public: const int & getClassType() const { static std::string  str(#class_name); return str; }

typedef enum 
{
	p_OPTIONAL = 0,
	p_MANDATORY = 1
} Presense;

typedef boost::shared_ptr<BaseObject> BaseObjectPtr;
typedef std::list<BaseObjectPtr> BaseObjectPtrList;

} // namespace XLS


