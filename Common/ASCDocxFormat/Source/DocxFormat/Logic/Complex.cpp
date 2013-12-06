
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Complex.h"


namespace OOX
{
	namespace Logic
	{
			
		Complex::Complex()
		{
		}


		Complex::~Complex()
		{
		}


		Complex::Complex(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Complex& Complex::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Complex::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Ext = element.attribute("ext").value();
		}

		
		const XML::XNode Complex::toXML() const
		{			
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX