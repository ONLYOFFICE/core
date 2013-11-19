#pragma once
#ifndef ODP_STYLES_FILL_IMAGE_INCLUDE_H_
#define ODP_STYLES_FILL_IMAGE_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include <string>
#include "property.h"
#include "OdtFormat/Limit/HrefType.h"
#include "OdtFormat/Limit/Show.h"
#include "OdtFormat/Limit/Actuate.h"

namespace Odp
{
	namespace Styles
	{
		class FillImage : public Odt::WritingElement
		{
		public:
			FillImage();
			virtual ~FillImage();
			explicit FillImage(const XML::XNode& node);
			const FillImage& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> Name;
			property<std::string> Href;
			nullable_property<std::string> DisplayName;
			nullable_property<std::string, Odt::Limit::HrefType> Type;
			nullable_property<std::string, Odt::Limit::Show> Show;
			nullable_property<std::string, Odt::Limit::Actuate> Actuate;
		};
	} // namespace Styles
} // namespace Odp

#endif // ODP_STYLES_FILL_IMAGE_INCLUDE_H_