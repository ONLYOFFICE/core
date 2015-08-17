#pragma once

#include <boost/shared_ptr.hpp>
#include <list>
#include <sstream>

#include "Logging/Log.h"
#include "XlsElementsType.h" //шоб со строками не работать

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


