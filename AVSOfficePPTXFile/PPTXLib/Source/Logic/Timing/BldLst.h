#pragma once
#ifndef PPTX_LOGIC_BLDLST_INCLUDE_H_
#define PPTX_LOGIC_BLDLST_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include <list>
#include "BuildNodeBase.h"

namespace PPTX
{
	namespace Logic
	{
		class BldLst : public WrapperWritingElement
		{
		public:
			BldLst();
			virtual ~BldLst();
			explicit BldLst(const XML::XNode& node);
			const BldLst& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::list<BuildNodeBase> > list;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLDLST_INCLUDE_H_