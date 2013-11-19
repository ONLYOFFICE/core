#pragma once
#ifndef ODT_CONTENT_DATE_DAY_INCLUDE_H_
#define ODT_CONTENT_DATE_DAY_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>


namespace Odt
{
	namespace Content
	{
		class DateDay : public WritingElement
		{
		public:
			DateDay();
			virtual ~DateDay();
			explicit DateDay(const XML::XNode& node);
			const DateDay& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string m_style;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_DATE_DAY_INCLUDE_H_