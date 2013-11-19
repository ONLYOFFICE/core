#pragma once
#ifndef PPTX_LOGIC_ATTRNAMELST_INCLUDE_H_
#define PPTX_LOGIC_ATTRNAMELST_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include <list>
#include "AttrName.h"

namespace PPTX
{
	namespace Logic
	{
		class AttrNameLst : public WrapperWritingElement
		{
		public:
			AttrNameLst();
			virtual ~AttrNameLst();
			explicit AttrNameLst(const XML::XNode& node);
			const AttrNameLst& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::list<AttrName> > list;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ATTRNAMELST_INCLUDE_H_