#pragma once
#ifndef ODP_CONTENT_RECT_INCLUDE_H_
#define ODP_CONTENT_RECT_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include "Utility.h"
#include <string>
#include <windows.h>
#include "OdtFormat/Content/Text.h"

namespace Odp
{
	namespace Content
	{
		class Rect : public Odt::WritingElement
		{
		public:
			Rect();
			virtual ~Rect();
			explicit Rect(const XML::XNode& node);
			const Rect& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string> PresentationStyleName;
			nullable_property<std::string> DrawStyleName;
			nullable_property<std::string> TextStyleName;
			nullable_property<std::string> Id;
			property<std::string> layer;
			property<UniversalUnit> height;
			property<UniversalUnit> width;
			property<UniversalUnit> x;
			property<UniversalUnit> y;

			property<std::vector<Odt::Content::Text> > Text;
		public:
			void GetMmRect(RECT& rect)const;
		};
	} // namespace Content
} // namespace Odp

#endif // ODP_CONTENT_RECT_INCLUDE_H_