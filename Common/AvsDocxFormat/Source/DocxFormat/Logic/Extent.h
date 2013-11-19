#pragma once
#ifndef OOX_LOGIC_EXTENT_INCLUDE_H_
#define OOX_LOGIC_EXTENT_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "./../Unit.h"
#include "Common.h"


namespace OOX
{
	namespace Logic
	{
		class Extent : public WritingElement
		{
		public:
			Extent();
			virtual ~Extent();			
			explicit Extent(const XML::XNode& node);
			const Extent& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<Common::Size<UnitSx> >		Size;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_EXTENT_INCLUDE_H