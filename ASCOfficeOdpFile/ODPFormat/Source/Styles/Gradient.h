#pragma once
#ifndef ODP_STYLES_GRADIENT_INCLUDE_H_
#define ODP_STYLES_GRADIENT_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include <string>
#include "property.h"
#include "OdtFormat/Limit/GradientStyle.h"
#include "OdtFormat/Logic/Color.h"
#include "Utility.h"

namespace Odp
{
	namespace Styles
	{
		class Gradient : public Odt::WritingElement
		{
		public:
			Gradient();
			virtual ~Gradient();
			explicit Gradient(const XML::XNode& node);
			const Gradient& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> Name;
			property<std::string, Odt::Limit::GradientStyle> Style;
			nullable_property<std::string> DisplayName;
			property<Odt::Logic::Color> StartColor;
			property<Odt::Logic::Color> EndColor;
			property<UniversalUnit> StartIntensity;
			property<UniversalUnit> EndIntensity;
			nullable_property<UniversalUnit> cx;
			nullable_property<UniversalUnit> cy;
			nullable_property<int> Angle;
			property<UniversalUnit> Border;
		};
	} // namespace Styles
} // namespace Odp

#endif // ODP_STYLES_GRADIENT_INCLUDE_H_