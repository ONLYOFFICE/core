#pragma once
#ifndef PPTX_LOGIC_CNVPICPR_INCLUDE_H_
#define PPTX_LOGIC_CNVPICPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>


namespace PPTX
{
	namespace Logic
	{
		class CNvPicPr : public WrapperWritingElement
		{
		public:
			CNvPicPr();
			virtual ~CNvPicPr();			
			explicit CNvPicPr(const XML::XNode& node);
			const CNvPicPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<bool> preferRelativeResize;

			nullable_property<bool> noAdjustHandles;
			nullable_property<bool> noChangeArrowheads;
			nullable_property<bool> noChangeAspect;
			nullable_property<bool> noChangeShapeType;
			nullable_property<bool> noCrop;
			nullable_property<bool> noEditPoints;
			nullable_property<bool> noGrp;
			nullable_property<bool> noMove;
			nullable_property<bool> noResize;
			nullable_property<bool> noRot;
			nullable_property<bool> noSelect;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CNVPICPR_INCLUDE_H