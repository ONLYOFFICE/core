#pragma once
#ifndef PPTX_LOGIC_C_NV_PROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_C_NV_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include "Hyperlink.h"


namespace PPTX
{
	namespace Logic
	{
		class CNvPr : public WrapperWritingElement
		{
		public:
			CNvPr();
			virtual ~CNvPr();			
			explicit CNvPr(const XML::XNode& node);
			const CNvPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<int, setter::only_positive<int> > id;
			property<std::string> name;
			nullable_property<std::string> descr;
			nullable_property<bool> hidden;
			nullable_property<std::string> title;
			nullable_property<Hyperlink> hlinkClick;
			nullable_property<Hyperlink> hlinkHover;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_C_NV_PROPERTIES_INCLUDE_H