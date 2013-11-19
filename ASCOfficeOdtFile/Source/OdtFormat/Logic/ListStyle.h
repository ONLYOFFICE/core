#pragma once
#ifndef ODT_LOGIC_LIST_STYLE_INCLUDE_H_
#define ODT_LOGIC_LIST_STYLE_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include <string>
#include <vector>
#include "ListLevelStyle.h"
#include "Properties.h"


namespace Odt
{
	namespace Logic
	{
		class ListStyle : public WritingElement
		{
		public:
			ListStyle();
			virtual ~ListStyle();
			explicit ListStyle(const XML::XNode& node);
			const ListStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const Properties listLevelStyleProperties(const int level) const;

		public:
			property<std::string>										Name;
			property<std::vector<ListLevelStyle> >	Items;
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_LIST_STYLE_INCLUDE_H_