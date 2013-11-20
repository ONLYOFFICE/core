#pragma once
#ifndef OOX_LOGIC_WRAP_INCLUDE_H_
#define OOX_LOGIC_WRAP_INCLUDE_H_

#include <string>
#include "./../WritingElement.h"

namespace OOX
{
	namespace Logic
	{
		class Wrap : public WritingElement
		{
		public:
			Wrap();
			virtual ~Wrap();			
			explicit Wrap(const XML::XNode& node);
			const Wrap& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			
		public:
			property<std::string>			Type;
			nullable_property<std::string>	AnchorX;
			nullable_property<std::string>	AnchorY;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_WRAP_INCLUDE_H_