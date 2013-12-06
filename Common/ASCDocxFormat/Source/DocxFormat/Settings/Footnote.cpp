
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Footnote.h"


namespace OOX
{
	namespace Settings
	{

		Footnote::Footnote()
		{
		}


		Footnote::~Footnote()
		{
		}


		Footnote::Footnote(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Footnote& Footnote::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Footnote::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_id = element.attribute("id").value();
		}


		const XML::XNode Footnote::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Settings
} // namespace OOX