#pragma once
#ifndef OOX_LOGIC_F_INCLUDE_H_
#define OOX_LOGIC_F_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"

namespace OOX
{
	namespace Logic
	{
		class F : public WritingElement
		{
		public:
			F();
			virtual ~F();			
			explicit F(const XML::XNode& node);
			const F& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>		Eqn;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_F_INCLUDE_H_