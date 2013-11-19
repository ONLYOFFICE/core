#pragma once
#ifndef PPTX_LOGIC_TMPLLST_INCLUDE_H_
#define PPTX_LOGIC_TMPLLST_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include <list>
#include "Tmpl.h"

namespace PPTX
{
	namespace Logic
	{
		class TmplLst : public WrapperWritingElement
		{
		public:
			TmplLst();
			virtual ~TmplLst();
			explicit TmplLst(const XML::XNode& node);
			const TmplLst& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::list<Tmpl> > list;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TMPLLST_INCLUDE_H_