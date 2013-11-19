#pragma once
#ifndef PPTX_LOGIC_LINESTYLE_INCLUDE_H_
#define PPTX_LOGIC_LINESTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "Ln.h"
#include "StyleRef.h"

namespace PPTX
{
	namespace Logic
	{
		class LineStyle : public WrapperWritingElement
		{
		public:
			LineStyle();
			virtual ~LineStyle();			
			explicit LineStyle(const XML::XNode& node);
			const LineStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<Ln> ln;
			nullable_property<StyleRef> lnRef;
		//private:
		public:
			property<std::string> m_name;
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_LINESTYLE_INCLUDE_H