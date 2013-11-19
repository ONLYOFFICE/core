#pragma once
#ifndef OOX_LOGIC_H_INCLUDE_H_
#define OOX_LOGIC_H_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"
#include <string>


namespace OOX
{
	namespace Logic
	{
		class H : public WritingElement
		{
		public:
			H();
			virtual ~H();
			explicit H(const XML::XNode& node);
			const H& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>			Position;
			nullable_property<std::string>	XRange;
			nullable_property<std::string>	YRange;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_H_INCLUDE_H_