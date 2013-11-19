#pragma once
#ifndef PPTX_LOGIC_DEFAULTSHAPEDEFINITION_INCLUDE_H_
#define PPTX_LOGIC_DEFAULTSHAPEDEFINITION_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include "SpPr.h"
#include "BodyPr.h"
#include "TextListStyle.h"
#include "ShapeStyle.h"

namespace PPTX
{
	namespace Logic
	{
		class DefaultShapeDefinition : public WrapperWritingElement
		{
		public:
			DefaultShapeDefinition();
			virtual ~DefaultShapeDefinition();			
			explicit DefaultShapeDefinition(const XML::XNode& node);
			const DefaultShapeDefinition& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<SpPr> spPr;
			property<BodyPr> bodyPr;
			property<TextListStyle> lstStyle;
			nullable_property<ShapeStyle> style;
		//private:
		public:
			property<std::string> m_name;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_DEFAULTSHAPEDEFINITION_INCLUDE_H