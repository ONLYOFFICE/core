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

	void set_document_uniq(const std::wstring & uniq)	{m_document_uniq = uniq;}
	void set_document_name(const std::wstring & name)	{m_document_name = name;}

	std::wstring m_document_uniq;
	std::wstring m_document_name;
};

typedef boost::shared_ptr<BaseObjectDocument> BaseObjectDocumentPtr;

} // namespace XLS


