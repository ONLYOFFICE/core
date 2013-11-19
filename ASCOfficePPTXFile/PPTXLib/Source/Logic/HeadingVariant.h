#pragma once
#ifndef PPTX_LOGIC_HEADING_VARIANT_INCLUDE_H_
#define PPTX_LOGIC_HEADING_VARIANT_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/VariantType.h"

namespace PPTX
{
	namespace Logic
	{

		class HeadingVariant : public WrapperWritingElement
		{
		public:
			HeadingVariant();
			virtual ~HeadingVariant();
			explicit HeadingVariant(const XML::XNode& node);
			const HeadingVariant& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<std::string, Limit::VariantType > m_type;
			nullable_property<std::string> m_strContent;
			nullable_property<int> m_iContent;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_HEADING_VARIANT_INCLUDE_H_