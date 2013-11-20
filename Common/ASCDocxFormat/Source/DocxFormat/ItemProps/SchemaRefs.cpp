
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../ItemProps.h"


namespace OOX
{

	ItemProps::SchemaRefs::SchemaRefs()
	{
	}


	ItemProps::SchemaRefs::~SchemaRefs()
	{
	}
	

	ItemProps::SchemaRefs::SchemaRefs(const XML::XNode& node)
	{
		fromXML(node);
	}


	const ItemProps::SchemaRefs& ItemProps::SchemaRefs::operator =(const XML::XNode& node)
	{
		fromXML(node);
		return *this;
	}


	void ItemProps::SchemaRefs::fromXML(const XML::XNode& node)
	{
		const XML::XElement element(node);
		XML::Fill(SchemaRef, element);
	}


	const XML::XNode ItemProps::SchemaRefs::toXML() const
	{
		return XML::XElement(ns.ds + "schemaRefs",	XML::Write(SchemaRef));
	}

} // namespace OOX