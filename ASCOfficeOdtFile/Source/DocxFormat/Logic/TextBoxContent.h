#pragma once
#ifndef OOX_LOGIC_TEXT_BOX_CONTENT_INCLUDE_H_
#define OOX_LOGIC_TEXT_BOX_CONTENT_INCLUDE_H_

#include <vector>
#include "./../WritingElement.h"
#include "property.h"
#include "TextItem.h"

namespace OOX
{
	namespace Logic
	{
		class TextBoxContent : public WritingElement
		{
		public:
			TextBoxContent();
			virtual ~TextBoxContent();			
			explicit TextBoxContent(const XML::XNode& node);
			const TextBoxContent& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:			
			property<std::vector<Logic::TextItem> >	items;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TEXT_BOX_CONTENT_INCLUDE_H_