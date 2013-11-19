#pragma once
#ifndef PPTX_LOGIC_BILEVEL_INCLUDE_H_
#define PPTX_LOGIC_BILEVEL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class BiLevel : public WrapperWritingElement
		{
		public:
			BiLevel();
			virtual ~BiLevel();
			explicit BiLevel(const XML::XNode& node);
			const BiLevel& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<int, setter::only_positive<int> > thresh;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BILEVEL_INCLUDE_H_