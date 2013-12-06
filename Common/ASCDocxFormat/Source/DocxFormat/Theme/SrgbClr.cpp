
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "SrgbClr.h"


namespace OOX
{
	namespace Theme
	{

		SrgbClr::SrgbClr()
		{
		}


		SrgbClr::~SrgbClr()
		{
		}
	

		SrgbClr::SrgbClr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const SrgbClr& SrgbClr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void SrgbClr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_value	=	element.attribute("val").value();
			m_alpha	=	element.element("alpha").attribute("val").value();
		}
		

		const XML::XNode SrgbClr::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Theme
} // namespace OOX