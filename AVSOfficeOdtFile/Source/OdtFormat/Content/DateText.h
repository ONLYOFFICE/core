#pragma once
#ifndef ODT_CONTENT_DATE_TEXT_INCLUDE_H_
#define ODT_CONTENT_DATE_TEXT_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>


namespace Odt
{
	namespace Content
	{
		class DateText : public WritingElement
		{
		public:
			DateText();
			virtual ~DateText();
			explicit DateText(const XML::XNode& node);
			const DateText& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string m_text;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_DATE_TEXT_INCLUDE_H_