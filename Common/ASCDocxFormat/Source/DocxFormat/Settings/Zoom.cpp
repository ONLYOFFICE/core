
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Zoom.h"


namespace OOX
{
	namespace Settings
	{

		Zoom::Zoom()
		{
		}


		Zoom::~Zoom()
		{
		}
	

		Zoom::Zoom(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Zoom& Zoom::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Zoom::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_value		=	element.attribute("val").value();
			m_percent	=	element.attribute("percent").value();
		}


		const XML::XNode Zoom::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Settings
} // namespace OOX