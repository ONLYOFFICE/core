#pragma once
#ifndef ODT_CONTENT_NUMBER_STYLES_INCLUDE_H_
#define ODT_CONTENT_NUMBER_STYLES_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "Number.h"


namespace Odt
{
	namespace Content
	{
		class NumberStyle : public WritingElement
		{
		public:
			NumberStyle();
			virtual ~NumberStyle();
			explicit NumberStyle(const XML::XNode& node);
			const NumberStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string m_name;
			Number			m_number;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_NUMBER_STYLES_INCLUDE_H_