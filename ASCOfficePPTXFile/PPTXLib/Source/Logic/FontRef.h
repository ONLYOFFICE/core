#pragma once
#ifndef PPTX_LOGIC_SLIDE_FONTREF_INCLUDE_H_
#define PPTX_LOGIC_SLIDE_FONTREF_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../Limit/FontStyleIndex.h"
#include "UniColor.h"

namespace PPTX
{
	namespace Logic
	{
		class FontRef : public WrapperWritingElement
		{
		public:
			FontRef();
			virtual ~FontRef();
			explicit FontRef(const XML::XNode& node);
			const FontRef& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			UniColor Color;
			// Attributes
			nullable_property<std::string, Limit::FontStyleIndex > idx;
		//private:
		public:
			property<std::string> m_name;
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SLIDE_FONTREF_INCLUDE_H_