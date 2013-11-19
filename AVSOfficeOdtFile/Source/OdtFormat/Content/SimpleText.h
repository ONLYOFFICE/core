#pragma once
#ifndef ODT_CONTENT_SIMPLE_TEXT_INCLUDE_H_
#define ODT_CONTENT_SIMPLE_TEXT_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "property.h"


namespace Odt
{
	namespace Content
	{
		class SimpleText : public ParagraphItemBase
		{
		public:
			SimpleText();
			virtual ~SimpleText();
			explicit SimpleText(const XML::XNode& node);
			explicit SimpleText(const std::string& text);
			const SimpleText& operator =(const XML::XNode& node);
			const SimpleText& operator =(const std::string& text);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void fromTxt(const std::string& text);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			property<std::string>	Text;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_SIMPLE_TEXT_INCLUDE_H_