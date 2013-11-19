
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "SequenceDecl.h"


namespace Odt
{
	namespace Content
	{

		SequenceDecl::SequenceDecl()
		{
		}


		SequenceDecl::~SequenceDecl()
		{
		}


		SequenceDecl::SequenceDecl(const XML::XNode& node)
		{
			fromXML(node);
		}


		const SequenceDecl& SequenceDecl::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void SequenceDecl::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_level				= element.attribute("display-outline-level").value();
			m_separation	= element.attribute("separation-character").value();
			m_name				= element.attribute("name").value();
		}


		const XML::XNode SequenceDecl::toXML() const
		{
			return 
				XML::XElement(ns.text + "sequence-decl",
					XML::XAttribute(ns.text + "display-outline-level", m_level) +
					XML::XAttribute(ns.text + "separation-character", m_separation) +
					XML::XAttribute(ns.text + "name", m_name)
				);
		}

	} // namespace Content
} // namespace Odt