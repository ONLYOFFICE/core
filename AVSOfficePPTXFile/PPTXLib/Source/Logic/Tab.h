#pragma once
#ifndef PPTX_LOGIC_TAB_INCLUDE_H_
#define PPTX_LOGIC_TAB_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "nullable_property.h"
#include "./../Limit/TextTabAlignType.h"

namespace PPTX
{
	namespace Logic
	{
		class Tab : public WrapperWritingElement
		{
		public:
			Tab();
			virtual ~Tab();			
			explicit Tab(const XML::XNode& node);
			const Tab& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<int> pos;
			nullable_property<std::string, Limit::TextTabAlignType> algn;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TAB_INCLUDE_H