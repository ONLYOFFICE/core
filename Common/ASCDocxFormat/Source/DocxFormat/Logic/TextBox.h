#pragma once
#ifndef OOX_LOGIC_TEXT_BOX_INCLUDE_H_
#define OOX_LOGIC_TEXT_BOX_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "TextBoxContent.h"

namespace OOX
{
	namespace Logic
	{
		class TextBox : public WritingElement
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
			property<TextBoxContent>		content;
			nullable_property<std::string>	style;
			nullable_property<std::string>	inset; 
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TEXT_BOX_INCLUDE_H_