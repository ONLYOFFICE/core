#pragma once
#ifndef OOX_THEME_EXTRA_CLR_SCHEME_LST_INCLUDE_H_
#define OOX_THEME_EXTRA_CLR_SCHEME_LST_INCLUDE_H_

#include "./../WritingElement.h"


namespace OOX
{
	namespace Theme
	{
		class ExtraClrSchemeLst : public WritingElement
		{
		public:
			ExtraClrSchemeLst();
			virtual ~ExtraClrSchemeLst();
			explicit ExtraClrSchemeLst(const XML::XNode& node);
			const ExtraClrSchemeLst& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_EXTRA_CLR_SCHEME_LST_INCLUDE_H_