
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ListLevel.h"


namespace Odt
{
	namespace Logic
	{
		namespace Property
		{

			ListLevel::ListLevel()
			{
			}


			ListLevel::~ListLevel()
			{
			}


			ListLevel::ListLevel(const XML::XNode& node)
			{
				fromXML(node);
			}


			const ListLevel& ListLevel::operator =(const XML::XNode& node)
			{
				fromXML(node);
				return *this;
			}


			void ListLevel::fromXML(const XML::XNode& node)
			{
				const XML::XElement element(node);
				Mode = element.attribute("list-level-position-and-space-mode").value();
				if(element.attribute("space-before").exist())
				{
					SpaceBefore = element.attribute("space-before").value().ToString();
				}
				else if((element.element("list-level-label-alignment").exist()) &&
					(element.element("list-level-label-alignment").attribute("list-tab-stop-position").exist()))
				{
					SpaceBefore = element.element("list-level-label-alignment").attribute("list-tab-stop-position").value().ToString();
				}

				if(element.attribute("min-label-width").exist())
				{
					//â OpenOffice 2.3
					MinLabelWidth = element.attribute("min-label-width").value().ToString();					
				}
				else if (element.element("list-level-label-alignment").exist())
				{
					//â OpenOffice 3.0
					nullable<UniversalUnit> stopPosition;
					if(element.element("list-level-label-alignment").attribute("text-indent").exist())
						stopPosition = element.element("list-level-label-alignment").attribute("text-indent").value().ToString();

					MinLabelWidth = stopPosition.get_value_or(0) * -1;
				}
			}

			
			const XML::XNode ListLevel::toXML() const
			{
				return
				XML::XElement(ns.style + "list-level-properties",
					XML::XAttribute(ns.text + "list-level-position-and-space-mode", Mode) +
					XML::XAttribute(ns.text + "space-before", SpaceBefore) + 
					XML::XAttribute(ns.text + "min-label-width", MinLabelWidth)
				);
			}


			const bool ListLevel::isSimple() const
			{
				return true;
			}


			const ListLevel ListLevel::merge(const ListLevel& prev, const ListLevel& current)
			{
				ListLevel properties;
				properties.Mode = ::merge(prev.Mode, current.Mode);
				return properties;
			}

		} // namespace Property
	} // namespace Logic
} // namespace Odt