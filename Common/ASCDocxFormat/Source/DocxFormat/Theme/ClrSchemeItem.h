#pragma once
#ifndef OOX_THEME_CLR_SCHEME_ITEM_INCLUDE_H_
#define OOX_THEME_CLR_SCHEME_ITEM_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include <boost/shared_ptr.hpp>


namespace OOX
{
	namespace Theme
	{
		class ClrSchemeItem : public WritingElement
		{
		public:
			ClrSchemeItem();
			virtual ~ClrSchemeItem();
			explicit ClrSchemeItem(const XML::XNode& node);
			const ClrSchemeItem& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> name;
			property<boost::shared_ptr<WritingElement>> item;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_CLR_SCHEME_ITEM_INCLUDE_H_