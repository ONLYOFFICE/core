#pragma once
#ifndef PPTX_LOGIC_SLIDE_STYLEREF_INCLUDE_H_
#define PPTX_LOGIC_SLIDE_STYLEREF_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class StyleRef : public WrapperWritingElement
		{
		public:
			StyleRef();
			virtual ~StyleRef();
			explicit StyleRef(const XML::XNode& node);
			const StyleRef& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			UniColor Color;
			// Attributes
			nullable_property<int, setter::only_positive<int> > idx;
		//private:
		public:
			property<std::string> m_name;
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SLIDE_STYLEREF_INCLUDE_H_