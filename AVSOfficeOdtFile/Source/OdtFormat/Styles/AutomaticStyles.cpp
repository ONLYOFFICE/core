
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "AutomaticStyles.h"
#include "PageLayoutProperties.h"
#include "Exception/log_runtime_error.h"


namespace Odt
{
	namespace Styles
	{

		AutomaticStyles::AutomaticStyles()
		{
		}


		AutomaticStyles::~AutomaticStyles()
		{
		}
	

		AutomaticStyles::AutomaticStyles(const XML::XNode& node)
		{
			fromXML(node);
		}


		const AutomaticStyles& AutomaticStyles::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void AutomaticStyles::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			PageLayouts = element;
			NamedStyles	= element;
		}


		const XML::XNode AutomaticStyles::toXML() const
		{
			return 
				XML::XElement(ns.office + "automatic-styles",
					XML::Write(NamedStyles) +
					XML::Write(PageLayouts)
				);
		}

	} // namespace Styles
} // namespace Odt