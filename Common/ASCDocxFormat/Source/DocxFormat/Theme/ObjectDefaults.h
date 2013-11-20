#pragma once
#ifndef OOX_THEME_OBJECT_DEFAULTS_INCLUDE_H_
#define OOX_THEME_OBJECT_DEFAULTS_INCLUDE_H_

#include "./../WritingElement.h"


namespace OOX
{
	namespace Theme
	{
		class ObjectDefaults : public WritingElement
		{
		public:
			ObjectDefaults();
			virtual ~ObjectDefaults();
			explicit ObjectDefaults(const XML::XNode& node);
			const ObjectDefaults& operator =(const XML::XNode& node);

			public:
				virtual void fromXML(const XML::XNode& node);
				virtual const XML::XNode toXML() const;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_OBJECT_DEFAULTS_INCLUDE_H_