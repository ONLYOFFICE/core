
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Endnote.h"


namespace OOX
{
	namespace Settings
	{

		Endnote::Endnote()
		{
		}


		Endnote::~Endnote()
		{
		}


		Endnote::Endnote(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Endnote& Endnote::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Endnote::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_id = element.attribute("id").value();
		}


		const XML::XNode Endnote::toXML() const
		{
			return XML::XElement(ns.w + "endnote", XML::XAttribute(ns.w + "id", m_id));
		}

	} // namespace Settings
} // namespace OOX