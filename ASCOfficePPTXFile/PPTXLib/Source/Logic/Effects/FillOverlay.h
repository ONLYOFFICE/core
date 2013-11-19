#pragma once
#ifndef PPTX_LOGIC_FILLOVERLAY_INCLUDE_H_
#define PPTX_LOGIC_FILLOVERLAY_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../UniFill.h"
#include "./../../Limit/BlendMode.h"

namespace PPTX
{
	namespace Logic
	{

		class FillOverlay : public WrapperWritingElement
		{
		public:
			FillOverlay();
			virtual ~FillOverlay();
			explicit FillOverlay(const XML::XNode& node);
			const FillOverlay& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			UniFill Fill;
			property<std::string, Limit::BlendMode> blend;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_FILLOVERLAY_INCLUDE_H_