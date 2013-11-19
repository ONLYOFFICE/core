#pragma once
#ifndef ODT_STYLES_AUTOMATIC_STYLES_INCLUDE_H_
#define ODT_STYLES_AUTOMATIC_STYLES_INCLUDE_H_

#include "./../WritingElement.h"
#include "PageLayouts.h"
#include "property.h"
#include "Column.h"
#include "./../Logic/NamedStyles.h"


namespace Odt
{
	namespace Styles
	{
		class AutomaticStyles : public WritingElement
		{
		public:
			AutomaticStyles();
			virtual ~AutomaticStyles();
			explicit AutomaticStyles(const XML::XNode& node);
			const AutomaticStyles& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<Logic::NamedStyles>		NamedStyles;
			property<PageLayouts>						PageLayouts;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_AUTOMATIC_STYLES_INCLUDE_H_