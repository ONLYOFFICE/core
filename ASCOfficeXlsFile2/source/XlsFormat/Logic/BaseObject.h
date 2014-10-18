#pragma once

#include <boost/shared_ptr.hpp>
#include <list>

namespace XLS
{;

class CFStream;
class BinProcessor;
//class BinWriterProcessor;
class CFStreamCacheReader;

class BaseObject;
typedef boost::shared_ptr<BaseObject>	BaseObjectPtr;

// Logical representation of an XML record
class BaseObject
{
public:
	BaseObject(){}
	~BaseObject(){}

	virtual boost::shared_ptr<BaseObject> clone() = 0;

	virtual const bool read(CFStreamCacheReader& reader, BaseObjectPtr & parent, const bool mandatory) = 0; // Read self and children

	virtual void toFrom(BinProcessor& proc) = 0; // This function shall contain only mark functions and is the universal XML serialiser

	virtual const std::wstring & getClassName() const = 0; // Must be overridden in every deriver. The return value must be a reference to a static variable inside the getter

	void add_child (BaseObjectPtr e) {elements_.push_back(e);}
	
	std::list<BaseObjectPtr> elements_;
};

#define BO_ATTRIB_MARKUP_BEGIN void toFrom(BinProcessor& proc) {
#define BO_ATTRIB_MARKUP_ATTRIB(attr) /*(attr).toXML(proc.getParent(), L# attr); */{static std::wstring  attr ## _str(L# attr); proc.markAttribute(attr, attr ## _str);}
#define BO_ATTRIB_MARKUP_ATTRIB_NAME(attr, name) /*(attr).toXML(proc.getParent(), name);*/ proc.markAttribute(attr, name);
#define BO_ATTRIB_MARKUP_COMPLEX(attr) /*(attr).toXML(proc.getParent(), L# attr);*/ proc.markTaggedAttribute(attr);
#define BO_ATTRIB_MARKUP_VECTOR_COMPLEX(vec, class_name)  proc.markVector(vec, class_name());
#define BO_ATTRIB_MARKUP_END }

#define BASE_OBJECT_DEFINE_CLASS_NAME(class_name) public: const std::wstring & getClassName() const { static std::wstring  str(L# class_name); return str; }

typedef enum 
{
	p_OPTIONAL = 0,
	p_MANDATORY = 1
} Presense;

typedef boost::shared_ptr<BaseObject> BaseObjectPtr;
typedef std::list<BaseObjectPtr> BaseObjectPtrList;

} // namespace XLS


