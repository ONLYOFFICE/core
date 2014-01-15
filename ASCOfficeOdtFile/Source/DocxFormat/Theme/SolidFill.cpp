
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "SolidFill.h"


namespace OOX
{
	namespace Theme
	{

		SolidFill::SolidFill()
		{
		}


		SolidFill::~SolidFill()
		{
		}
	

		SolidFill::SolidFill(const XML::XNode& node)
		{
			fromXML(node);
		}


		const SolidFill& SolidFill::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void SolidFill::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_schemeClr = element.element("schemeClr");
		}


		const XML::XNode SolidFill::toXML() const
		{
			return 
				XML::XElement(ns.a + "solidFill", 
					XML::Write(m_schemeClr)
				);
		}

	} // namespace Theme
} // namespace OOX