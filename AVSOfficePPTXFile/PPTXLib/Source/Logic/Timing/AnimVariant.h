#pragma once
#ifndef PPTX_LOGIC_ANIMVARIANT_INCLUDE_H_
#define PPTX_LOGIC_ANIMVARIANT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{
		class AnimVariant : public WrapperWritingElement
		{
		public:
			AnimVariant();
			virtual ~AnimVariant();
			explicit AnimVariant(const XML::XNode& node);
			const AnimVariant& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string> name;

			nullable_property<bool> boolVal;
			nullable_property<std::string> strVal;
			nullable_property<int> intVal;
			nullable_property<double> fltVal;
			UniColor clrVal;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIMVARIANT_INCLUDE_H_