#pragma once
#ifndef ODP_CONTENT_TEXTBOX_INCLUDE_H_
#define ODP_CONTENT_TEXTBOX_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include "OdtFormat/Content/Text.h"

namespace Odp
{
	namespace Content
	{
		class TextBox : public Odt::WritingElement
		{
		public:
			TextBox();
			virtual ~TextBox();
			explicit TextBox(const XML::XNode& node);
			const TextBox& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::vector<Odt::Content::Text> > Items;
		};
	} // namespace Content
} // namespace Odp

#endif // ODP_CONTENT_TEXTBOX_INCLUDE_H_