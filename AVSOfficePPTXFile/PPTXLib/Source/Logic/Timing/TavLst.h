#pragma once
#ifndef PPTX_LOGIC_TAVLST_INCLUDE_H_
#define PPTX_LOGIC_TAVLST_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include <list>
#include "Tav.h"

namespace PPTX
{
	namespace Logic
	{
		class TavLst : public WrapperWritingElement
		{
		public:
			TavLst();
			virtual ~TavLst();
			explicit TavLst(const XML::XNode& node);
			const TavLst& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::list<Tav> > list;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TAVLST_INCLUDE_H_