#pragma once
#ifndef ODP_STYLES_PLACEHOLDER_INCLUDE_H_
#define ODP_STYLES_PLACEHOLDER_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include <string>
#include "Common.h"
#include "OdtFormat/Unit.h"

namespace Odp
{
	namespace Styles
	{
		class Placeholder : public Odt::WritingElement
		{
		public:
			Placeholder();
			virtual ~Placeholder();
			explicit Placeholder(const XML::XNode& node);
			const Placeholder& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string> object;//TODO : Здесь можно сделать limit ("handout")
			property<UniversalUnit> x;
			property<UniversalUnit> y;
			property<UniversalUnit> width;
			property<UniversalUnit> height;
		};
	} // namespace Styles
} // namespace Odp

#endif // ODP_STYLES_PLACEHOLDER_INCLUDE_H_