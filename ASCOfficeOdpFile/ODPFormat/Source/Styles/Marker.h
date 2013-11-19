#pragma once
#ifndef ODP_STYLES_MARKER_INCLUDE_H_
#define ODP_STYLES_MARKER_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include <string>

namespace Odp
{
	namespace Styles
	{
		class Marker : public Odt::WritingElement
		{
		public:
			Marker();
			virtual ~Marker();
			explicit Marker(const XML::XNode& node);
			const Marker& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string> Name;
			nullable_property<std::string> DisplayName;
			property<std::string> ViewBox;
			property<std::string> d;
		};
	} // namespace Styles
} // namespace Odp

#endif // ODP_STYLES_MARKER_INCLUDE_H_