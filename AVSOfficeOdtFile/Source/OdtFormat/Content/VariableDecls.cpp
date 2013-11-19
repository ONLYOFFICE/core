
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "VariableDecls.h"


namespace Odt
{
	namespace Content
	{

		VariableDecls::VariableDecls()
		{
		}


		VariableDecls::~VariableDecls()
		{
		}
	
	
		VariableDecls::VariableDecls(const XML::XNode& node)
		{
			fromXML(node);
		}
	
	
		const VariableDecls& VariableDecls::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

	
		void VariableDecls::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_list, element);
		}
	
	
		const XML::XNode VariableDecls::toXML() const
		{
			return XML::XElement(ns.text + "variable-decls", XML::Write(m_list));
		}

	} // namespace Content
} // namespace Odt