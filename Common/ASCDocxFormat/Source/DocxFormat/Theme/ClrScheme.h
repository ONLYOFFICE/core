#pragma once
#ifndef OOX_THEME_CLR_SCHEME_INCLUDE_H_
#define OOX_THEME_CLR_SCHEME_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include <vector>
#include "ClrSchemeItem.h"


namespace OOX
{
	namespace Theme
	{
		class ClrScheme : public WritingElement
		{
		public:
			ClrScheme();
			virtual ~ClrScheme();
			explicit ClrScheme(const XML::XNode& node);
			const ClrScheme& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<std::string> name;
			property<std::vector<ClrSchemeItem>> clrSchemeItems;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_CLR_SCHEME_INCLUDE_H_