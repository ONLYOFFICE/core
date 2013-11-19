#pragma once
#ifndef PPTX_LOGIC_CNVSPPR_INCLUDE_H_
#define PPTX_LOGIC_CNVSPPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>


namespace PPTX
{
	namespace Logic
	{
		class CNvSpPr : public WrapperWritingElement
		{
		public:
			CNvSpPr();
			virtual ~CNvSpPr();			
			explicit CNvSpPr(const XML::XNode& node);
			const CNvSpPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<bool> txBox;

			nullable_property<bool> noAdjustHandles;
			nullable_property<bool> noChangeArrowheads;
			nullable_property<bool> noChangeAspect;
			nullable_property<bool> noChangeShapeType;
			nullable_property<bool> noEditPoints;
			nullable_property<bool> noGrp;
			nullable_property<bool> noMove;
			nullable_property<bool> noResize;
			nullable_property<bool> noRot;
			nullable_property<bool> noSelect;
			nullable_property<bool> noTextEdit;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CNVSPPR_INCLUDE_H