#pragma once
#ifndef PPTX_LOGIC_TGTEL_INCLUDE_H_
#define PPTX_LOGIC_TGTEL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "DocxFormat/RId.h"
#include "SpTgt.h"

namespace PPTX
{
	namespace Logic
	{
		class TgtEl : public WrapperWritingElement
		{
		public:
			TgtEl();
			virtual ~TgtEl();
			explicit TgtEl(const XML::XNode& node);
			const TgtEl& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string> inkTgt;

			//sndTgt
			nullable_property<std::string> name;
			nullable_property<OOX::RId> embed;
			nullable_property<bool> builtIn;

			nullable_property<SpTgt> spTgt;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TGTEL_INCLUDE_H_