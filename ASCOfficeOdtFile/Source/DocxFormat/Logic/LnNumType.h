#pragma once

#include <string>

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"

namespace OOX
{
	namespace Logic
	{
		class LnNumType : public WritingElement
		{
		public:
			LnNumType();
			virtual ~LnNumType();
			explicit LnNumType(const XML::XNode& node);
			const LnNumType& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<int>			start;
			nullable_property<int>			countBy;
			nullable_property<int>			distance;
			nullable_property<std::string>	restart;
		};
	}
}