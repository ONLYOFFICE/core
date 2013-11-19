#pragma once
#ifndef PPTX_LOGIC_CONDLST_INCLUDE_H_
#define PPTX_LOGIC_CONDLST_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include <list>
#include "Cond.h"

namespace PPTX
{
	namespace Logic
	{
		class CondLst : public WrapperWritingElement
		{
		public:
			CondLst();
			virtual ~CondLst();
			explicit CondLst(const XML::XNode& node);
			const CondLst& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::list<Cond> > list;

			property<std::string> name;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CONDLST_INCLUDE_H_