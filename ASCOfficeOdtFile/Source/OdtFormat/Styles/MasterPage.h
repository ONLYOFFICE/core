#pragma once
#ifndef ODT_STYLES_MASTER_PAGE_INCLUDE_H_
#define ODT_STYLES_MASTER_PAGE_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"
#include "./../Content/Text.h"


namespace Odt
{
	namespace Styles
	{
		class MasterPage : public WritingElement
		{
		public:
			MasterPage();
			virtual ~MasterPage();
			explicit MasterPage(const XML::XNode& node);
			const MasterPage& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>		Name;
			property<std::string>		Layout;
			property<std::vector<Content::Text> >		Header;
			property<std::vector<Content::Text> >		Footer;
			property<std::vector<Content::Text> >		HeaderLeft;
			property<std::vector<Content::Text> >		FooterLeft;
			nullable_property<std::string> NextStyleName;
	};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_MASTER_PAGE_INCLUDE_H_