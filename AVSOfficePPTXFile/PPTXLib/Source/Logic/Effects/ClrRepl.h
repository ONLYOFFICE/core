#pragma once
#ifndef PPTX_LOGIC_CLRREPL_INCLUDE_H_
#define PPTX_LOGIC_CLRREPL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class ClrRepl : public WrapperWritingElement
		{
		public:
			ClrRepl();
			virtual ~ClrRepl();
			explicit ClrRepl(const XML::XNode& node);
			const ClrRepl& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			UniColor Color;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CLRREPL_INCLUDE_H_