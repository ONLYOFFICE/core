
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "VariableDecl.h"


namespace Odt
{
	namespace Content
	{

		VariableDecl::VariableDecl()
		{
		}


		VariableDecl::~VariableDecl()
		{
		}


		VariableDecl::VariableDecl(const XML::XNode& node)
		{
			fromXML(node);
		}


		const VariableDecl& VariableDecl::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void VariableDecl::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Type = element.attribute("value-type").value();
			Name = element.attribute("name").value();
		}


		const XML::XNode VariableDecl::toXML() const
		{
			return
				XML::XElement(ns.text + "variable-decl",
					XML::XAttribute(ns.office + "value-type", Type) +
					XML::XAttribute(ns.text + "name", Name)
				);
		}

	} // namespace Content
} // namespace Odt