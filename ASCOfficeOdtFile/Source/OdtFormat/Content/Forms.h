#pragma once
#ifndef ODT_CONTENT_FORMS_INCLUDE_H_
#define ODT_CONTENT_FORMS_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"


namespace Odt
{
	namespace Content
	{
		class Forms : public WritingElement
		{
		public:
			Forms();
			virtual ~Forms();
			explicit Forms(const XML::XNode& node);
			const Forms& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<bool>	AutomaticFocus;
			property<bool>	ApplyDesignMode;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_FORMS_INCLUDE_H_