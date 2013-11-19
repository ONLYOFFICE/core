#pragma once
#ifndef PPTX_LOGIC_TMPL_INCLUDE_H_
#define PPTX_LOGIC_TMPL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "TnLst.h"

namespace PPTX
{
	namespace Logic
	{
		class Tmpl : public WrapperWritingElement
		{
		public:
			Tmpl();
			virtual ~Tmpl();
			explicit Tmpl(const XML::XNode& node);
			const Tmpl& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<TnLst> tnLst;
			nullable_property<int, setter::only_positive<int> > lvl;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TMPL_INCLUDE_H_