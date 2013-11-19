
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ListStyle.h"
#include "Exception/log_runtime_error.h"


namespace Odt
{
	namespace Logic
	{

		ListStyle::ListStyle()
		{
		}


		ListStyle::~ListStyle()
		{
		}


		ListStyle::ListStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ListStyle& ListStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

	
		void ListStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name = element.attribute("name").value();
			XML::Fill(Items, element, "list-level-style-bullet", "list-level-style-number", "list-level-style-image");
		}


		const XML::XNode ListStyle::toXML() const
		{
			return
				XML::XElement(ns.text + "list-style", 
					XML::XAttribute(ns.style + "name", Name) +
					XML::Write(Items)
				);
		}


		const Properties ListStyle::listLevelStyleProperties(const int level) const
		{
			BOOST_FOREACH(const ListLevelStyle& listLevelStyle, *Items)
			{				
				if (listLevelStyle.getLevel() == level)
				{
					return listLevelStyle.getProperties();
				}
			}
			throw log_runtime_error("bad level name");
		}
	} // namespace Logic
} // namespace Odt