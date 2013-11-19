#pragma once
#ifndef PPTX_LOGIC_SOLIDFILL_INCLUDE_H_
#define PPTX_LOGIC_SOLIDFILL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class SolidFill : public WrapperWritingElement
		{
		public:
			SolidFill();
			virtual ~SolidFill();
			explicit SolidFill(const XML::XNode& node);
			const SolidFill& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			void Merge(SolidFill& fill)const;
		public:
			UniColor Color;
		private:
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SOLIDFILL_INCLUDE_H_