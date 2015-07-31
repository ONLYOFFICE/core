#pragma once


#include "BaseObject.h" 

namespace XLS
{;


// Logical representation of an XML record
class BaseObjectDocument : public BaseObject 
{
public:
	BASE_OBJECT_DEFINE_CLASS_NAME(BaseObjectDocument)
	
	BaseObjectDocument(){}
	~BaseObjectDocument(){}

	static const ElementType	type = typeAnyObject;

	BaseObjectPtr clone() {return BaseObjectPtr(new BaseObjectDocument(*this));}

	virtual const bool read(CFStreamCacheReader& reader, BaseObject* parent, const bool mandatory){return false;}
};

typedef boost::shared_ptr<BaseObjectDocument> BaseObjectDocumentPtr;

} // namespace XLS


