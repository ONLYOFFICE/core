#pragma once
#ifndef ODT_STYLES_FOOTNOTE_SEP_INCLUDE_H_
#define ODT_STYLES_FOOTNOTE_SEP_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "nullable.h"


namespace Odt
{
	namespace Styles
	{
		class FootnoteSep : public WritingElement
		{
		public:
			FootnoteSep();
			virtual ~FootnoteSep();
			explicit FootnoteSep(const XML::XNode& node);
			const FootnoteSep& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			nullable<std::string>		m_width;
			nullable<std::string>		m_distanceBeforeSep;
			nullable<std::string>		m_distanceAfterSep;
			std::string							m_adjustment;
			std::string							m_relWidth;
			std::string							m_color;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_FOOTNOTE_SEP_INCLUDE_H_