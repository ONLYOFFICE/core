#pragma once

#include <string>

#include "property.h"
#include "nullable_property.h"

#include ".\..\WritingElement.h"

namespace OOX
{
	namespace Logic
	{
		class FillStyle : public WritingElement
		{
		public:
			FillStyle();
			virtual ~FillStyle();			
			explicit FillStyle(const XML::XNode& node);
			const FillStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<std::string>	filled;
			nullable_property<std::string>	fillcolor;
			nullable_property<std::string>	opacity;

			nullable_property<std::string>	Id;		// image id
			nullable_property<std::string>	type;	
			nullable_property<std::string>	recolor;
			nullable_property<std::string>	rotate;
		};
	}
}
