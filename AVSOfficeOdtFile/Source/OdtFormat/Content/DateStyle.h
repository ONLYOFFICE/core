#pragma once
#ifndef ODT_CONTENT_DATE_STYLES_INCLUDE_H_
#define ODT_CONTENT_DATE_STYLES_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "DateDay.h"
#include "DateMonth.h"
#include "DateYear.h"
#include <vector>
#include "DateText.h"


namespace Odt
{
	namespace Content
	{
		class DateStyles : public WritingElement
		{
		public:
			DateStyles();
			virtual ~DateStyles();
			explicit DateStyles(const XML::XNode& node);
			const DateStyles& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string m_name;
			bool				m_automaticOrder;
			DateDay			m_day;
			DateMonth		m_month;
			DateYear		m_year;
			std::vector<DateText>	m_separators;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_DATE_STYLES_INCLUDE_H_