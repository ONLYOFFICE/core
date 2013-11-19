#pragma once
#ifndef PPTX_LOGIC_SHAPESTYLE_INCLUDE_H_
#define PPTX_LOGIC_SHAPESTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include "StyleRef.h"
#include "FontRef.h"

namespace PPTX
{
	namespace Logic
	{
		class ShapeStyle : public WrapperWritingElement
		{
		public:
			ShapeStyle();
			virtual ~ShapeStyle();			
			explicit ShapeStyle(const XML::XNode& node);
			const ShapeStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<StyleRef> lnRef;
			property<StyleRef> fillRef;
			property<StyleRef> effectRef;
			property<FontRef> fontRef;
		//private:
		public:
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SHAPESTYLE_INCLUDE_H