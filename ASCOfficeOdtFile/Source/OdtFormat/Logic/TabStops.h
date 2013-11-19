#pragma once
#ifndef ODT_LOGIC_TAB_STOPS_INCLUDE_H_
#define ODT_LOGIC_TAB_STOPS_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "TabStop.h"
#include <vector>


namespace Odt
{
	namespace Logic
	{
		class TabStops : public WritingElement
		{
		public:
			TabStops();
			virtual ~TabStops();
			explicit TabStops(const XML::XNode& node);
			const TabStops& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::vector<TabStop> > TabStop;
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_TAB_STOPS_INCLUDE_H_