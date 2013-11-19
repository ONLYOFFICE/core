#pragma once
#ifndef PPTX_LOGIC_CNVGRPSPPR_INCLUDE_H_
#define PPTX_LOGIC_CNVGRPSPPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>


namespace PPTX
{
	namespace Logic
	{
		class CNvGrpSpPr : public WrapperWritingElement
		{
		public:
			CNvGrpSpPr();
			virtual ~CNvGrpSpPr();			
			explicit CNvGrpSpPr(const XML::XNode& node);
			const CNvGrpSpPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<bool> noChangeAspect;
			nullable_property<bool> noGrp;
			nullable_property<bool> noMove;
			nullable_property<bool> noResize;
			nullable_property<bool> noRot;
			nullable_property<bool> noSelect;
			nullable_property<bool> noUngrp;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CNVGRPSPPR_INCLUDE_H