
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TabsProperty.h"


namespace OOX
{
	namespace Logic
	{

		TabsProperty::TabsProperty()
		{
		}


		TabsProperty::~TabsProperty()
		{
		}
	

		TabsProperty::TabsProperty(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TabsProperty& TabsProperty::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TabsProperty::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Fill(Tabs, element, "tab");
		}


		const XML::XNode TabsProperty::toXML() const
		{
			return 
				XML::XElement(ns.w + "tabs",
					XML::Write(Tabs)
				);
		}

	} // namespace Logic
} // namespace OOX