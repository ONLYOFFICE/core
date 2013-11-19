#pragma once
#ifndef PPTX_SLIDES_CLRMAPOVR_INCLUDE_H_
#define PPTX_SLIDES_CLRMAPOVR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "ClrMap.h"
#include "XML.h"

namespace PPTX
{
	namespace Logic
	{
		class ClrMapOvr : public WrapperWritingElement
		{
		public:
			ClrMapOvr();
			virtual ~ClrMapOvr();
			explicit ClrMapOvr(const XML::XNode& node);
			const ClrMapOvr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<ClrMap> overrideClrMapping;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_SLIDES_CLRMAPOVR_INCLUDE_H_