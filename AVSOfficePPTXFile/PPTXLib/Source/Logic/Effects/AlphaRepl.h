#pragma once
#ifndef PPTX_LOGIC_ALPHAREPL_INCLUDE_H_
#define PPTX_LOGIC_ALPHAREPL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class AlphaRepl : public WrapperWritingElement
		{
		public:
			AlphaRepl();
			virtual ~AlphaRepl();
			explicit AlphaRepl(const XML::XNode& node);
			const AlphaRepl& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<int, setter::only_positive<int> > a;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ALPHAREPL_INCLUDE_H_