
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../ItemProps.h"


namespace OOX
{

	ItemProps::SchemaRef::SchemaRef()
	{
	}


	ItemProps::SchemaRef::~SchemaRef()
	{
	}
	

	ItemProps::SchemaRef::SchemaRef (const XML::XNode& node)
	{
		fromXML(node);
	}


	const ItemProps::SchemaRef& ItemProps::SchemaRef::operator =(const XML::XNode& node)
	{
		fromXML(node);
		return *this;
	}


	void ItemProps::SchemaRef::fromXML(const XML::XNode& node)
	{
		const XML::XElement element(node);
		Uri = element.attribute("uri").value();		
	}


	const XML::XNode ItemProps::SchemaRef::toXML() const
	{
		return 
			XML::XElement(ns.ds + "schemaRef",
				XML::XAttribute(ns.ds + "uri", Uri)
			);
	}

} // namespace OOX