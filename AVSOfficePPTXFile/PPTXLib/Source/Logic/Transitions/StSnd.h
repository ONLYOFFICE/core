#pragma once
#ifndef PPTX_SLIDES_STSND_INCLUDE_H_
#define PPTX_SLIDES_STSND_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "XML.h"
#include "property.h"
#include "nullable_property.h"
#include "DocxFormat/RId.h"

namespace PPTX
{
	namespace Logic
	{

		class StSnd : public WrapperWritingElement
		{
		public:
			StSnd();
			virtual ~StSnd();
			explicit StSnd(const XML::XNode& node);
			const StSnd& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<OOX::RId> embed;
			nullable_property<std::string> name;
			nullable_property<bool> loop;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_SLIDES_STSND_INCLUDE_H_