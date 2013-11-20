#pragma once
#ifndef OOX_LOGIC_HANDLES_INCLUDE_H_
#define OOX_LOGIC_HANDLES_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include <string>
#include "H.h"
#include <vector>


namespace OOX
{
	namespace Logic
	{
		class Handles : public WritingElement
		{
		public:
			Handles();
			virtual ~Handles();
			explicit Handles(const XML::XNode& node);
			const Handles& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::vector<H> > Hs;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_HANDLES_INCLUDE_H_