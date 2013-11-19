#pragma once
#ifndef ODT_LOGIC_DROP_CAP_INCLUDE_H_
#define ODT_LOGIC_DROP_CAP_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"
#include "nullable_property.h"
#include "./../Unit.h"


namespace Odt
{
	namespace Logic
	{
		class DropCap : public WritingElement
		{
		public:
			DropCap();
			virtual ~DropCap();
			explicit DropCap(const XML::XNode& node);
			const DropCap& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:	
			property<int>			Lines;
			nullable_property<int>  Lenght;
			nullable_property<UniversalUnit> Distance;
			
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_DROP_CAP_INCLUDE_H_