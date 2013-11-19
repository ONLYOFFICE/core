#pragma once
#ifndef PPTX_LOGIC_CNVGRAPHICFRAMESPPR_INCLUDE_H_
#define PPTX_LOGIC_CNVGRAPHICFRAMESPPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>

namespace PPTX
{
	namespace Logic
	{
		class CNvGraphicFramePr : public WrapperWritingElement
		{
		public:
			CNvGraphicFramePr();
			virtual ~CNvGraphicFramePr();			
			explicit CNvGraphicFramePr(const XML::XNode& node);
			const CNvGraphicFramePr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<bool> noChangeAspect;
			nullable_property<bool> noDrilldown;
			nullable_property<bool> noGrp;
			nullable_property<bool> noMove;
			nullable_property<bool> noResize;
			nullable_property<bool> noSelect;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CNVGRAPHICFRAMESPPR_INCLUDE_H