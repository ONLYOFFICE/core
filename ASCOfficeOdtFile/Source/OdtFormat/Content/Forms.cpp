
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Forms.h"


namespace Odt
{
	namespace Content
	{

		Forms::Forms()
		{
		}


		Forms::~Forms()
		{
		}


		Forms::Forms(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Forms& Forms::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Forms::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			AutomaticFocus	= element.attribute("automatic-focus").value();
			ApplyDesignMode = element.attribute("apply-design-mode").value();			
		}


		const XML::XNode Forms::toXML() const
		{
			return 
				XML::XElement(ns.office + "forms", 
					XML::XAttribute(ns.form + "automatic-focus", AutomaticFocus) +
					XML::XAttribute(ns.form + "apply-design-mode", ApplyDesignMode) 
				);
		}	

	} // namespace Content
} // namespace Odt