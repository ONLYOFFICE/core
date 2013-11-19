#pragma once
#ifndef ODT_CONTENT_NUMBER_INCLUDE_H_
#define ODT_CONTENT_NUMBER_INCLUDE_H_

#include "./../WritingElement.h"


namespace Odt
{
	namespace Content
	{
		class Number : public WritingElement
		{
		public:
			Number();
			virtual ~Number();
			explicit Number(const XML::XNode& node);
			const Number& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			int m_minIntegerDigits;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_NUMBER_INCLUDE_H_