#pragma once
#ifndef PPTX_LOGIC_TNLST_INCLUDE_H_
#define PPTX_LOGIC_TNLST_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include <list>
#include "TimeNodeBase.h"

namespace PPTX
{
	namespace Logic
	{
		class TnLst : public WrapperWritingElement
		{
		public:
			TnLst();
			virtual ~TnLst();
			explicit TnLst(const XML::XNode& node);
			const TnLst& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::list<TimeNodeBase> > list;

			property<std::string> name;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TNLST_INCLUDE_H_