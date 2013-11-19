#pragma once
#ifndef OOX_LOGIC_TABS_PROPERTY_INCLUDE_H_
#define OOX_LOGIC_TABS_PROPERTY_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "TabProperty.h"
#include <vector>


namespace OOX
{
	namespace Logic
	{
		class TabsProperty : public WritingElement
		{
		public:
			TabsProperty();
			virtual ~TabsProperty();
			explicit TabsProperty(const XML::XNode& node);
			const TabsProperty& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::vector<TabProperty> >	Tabs;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABS_PROPERTY_INCLUDE_H_