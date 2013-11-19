#pragma once
#ifndef ODT_CONTENT_DATE_YEAR_INCLUDE_H_
#define ODT_CONTENT_DATE_YEAR_INCLUDE_H_

#include "./../WritingElement.h"


namespace Odt
{
	namespace Content
	{
		class DateYear : public WritingElement
		{
		public:
			DateYear();
			virtual ~DateYear();
			explicit DateYear(const XML::XNode& node);
			const DateYear& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_DATE_YEAR_INCLUDE_H_