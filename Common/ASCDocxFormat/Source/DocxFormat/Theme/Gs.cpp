
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Gs.h"


namespace OOX
{
	namespace Theme
	{

		Gs::Gs()
		{
		}


		Gs::~Gs()
		{
		}


		Gs::Gs(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Gs& Gs::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}
	

		void Gs::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_pos				= element.attribute("pos").value();
			m_schemeClr = element.element("schemeClr");
		}

	
		const XML::XNode Gs::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Theme
} // namespace OOX