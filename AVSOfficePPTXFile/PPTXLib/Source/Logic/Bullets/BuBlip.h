#pragma once
#ifndef PPTX_LOGIC_BUBLIP_INCLUDE_H_
#define PPTX_LOGIC_BUBLIP_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "./../Fills/Blip.h"

namespace PPTX
{
	namespace Logic
	{
		class BuBlip : public WrapperWritingElement
		{
		public:
			BuBlip();
			virtual ~BuBlip();			
			explicit BuBlip(const XML::XNode& node);
			const BuBlip& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<Blip> blip;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUBLIP_INCLUDE_H