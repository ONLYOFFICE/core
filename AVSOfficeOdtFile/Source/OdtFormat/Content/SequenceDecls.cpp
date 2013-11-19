
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "SequenceDecls.h"


namespace Odt
{
	namespace Content
	{

		SequenceDecls::SequenceDecls()
		{
		}


		SequenceDecls::~SequenceDecls()
		{
		}


		SequenceDecls::SequenceDecls(const XML::XNode& node)
		{
			fromXML(node);
		}


		const SequenceDecls& SequenceDecls::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void SequenceDecls::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_list, element);
		}


		const XML::XNode SequenceDecls::toXML() const
		{
			return XML::XElement(ns.text + "sequence-decls", XML::Write(m_list));
		}

	} // namespace Content
} // namespace Odt