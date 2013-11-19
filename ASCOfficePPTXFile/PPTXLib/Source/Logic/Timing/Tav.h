#pragma once
#ifndef PPTX_LOGIC_Tav_INCLUDE_H_
#define PPTX_LOGIC_Tav_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "AnimVariant.h"

namespace PPTX
{
	namespace Logic
	{
		class Tav : public WrapperWritingElement
		{
		public:
			Tav();
			virtual ~Tav();
			explicit Tav(const XML::XNode& node);
			const Tav& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<AnimVariant> val;
			nullable_property<std::string> tm;
			nullable_property<std::string> fmla;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_Tav_INCLUDE_H_