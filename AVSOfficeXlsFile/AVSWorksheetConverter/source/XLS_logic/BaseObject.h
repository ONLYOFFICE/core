#pragma once

namespace XLS
{;

class CFStream;
class BinProcessor;
class BinWriterProcessor;
class CFStreamCacheReader;

// Logical representation of an XML record
class BaseObject
{
public:
	BaseObject(){}
	~BaseObject(){}

	virtual boost::shared_ptr<BaseObject> clone() = 0;

	virtual const bool read(CFStreamCacheReader& reader, MSXML2::IXMLDOMElementPtr parent, const bool mandatory) = 0; // Read self and children
	virtual const bool write(BinWriterProcessor& proc, const bool is_mandatory) = 0; // Write self and children

	virtual void toFromXML(BinProcessor& proc) = 0; // This function shall contain only mark functions and is the universal XML serialiser

	virtual const _bstr_t& getClassName() const = 0; // Must be overridden in every deriver. The return value must be a reference to a static variable inside the getter
};

#define BO_ATTRIB_MARKUP_BEGIN void toFromXML(BinProcessor& proc) {
#define BO_ATTRIB_MARKUP_ATTRIB(attr) /*(attr).toXML(proc.getParent(), L# attr); */{static _bstr_t attr ## _str(L# attr); proc.markAttribute(attr, attr ## _str);}
#define BO_ATTRIB_MARKUP_ATTRIB_NAME(attr, name) /*(attr).toXML(proc.getParent(), name);*/ proc.markAttribute(attr, name);
#define BO_ATTRIB_MARKUP_COMPLEX(attr) /*(attr).toXML(proc.getParent(), L# attr);*/ proc.markTaggedAttribute(attr);
#define BO_ATTRIB_MARKUP_VECTOR_COMPLEX(vec, class_name)  proc.markVector(vec, class_name());
#define BO_ATTRIB_MARKUP_END }

#define BASE_OBJECT_DEFINE_CLASS_NAME(class_name) public: const _bstr_t& getClassName() const { static _bstr_t str(L# class_name); return str; }

typedef enum 
{
	p_OPTIONAL = 0,
	p_MANDATORY = 1
} Presense;

typedef boost::shared_ptr<BaseObject> BaseObjectPtr;
typedef std::list<BaseObjectPtr> BaseObjectPtrList;

} // namespace XLS


