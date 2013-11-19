#pragma once
#ifndef PPTX_LOGIC_BUAUTONUM_INCLUDE_H_
#define PPTX_LOGIC_BUAUTONUM_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "./../../Limit/TextAutonumberScheme.h"

namespace PPTX
{
	namespace Logic
	{
		class BuAutoNum : public WrapperWritingElement
		{
		public:
			BuAutoNum();
			virtual ~BuAutoNum();			
			explicit BuAutoNum(const XML::XNode& node);
			const BuAutoNum& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string, Limit::TextAutonumberScheme> type;
			nullable_property<int, setter::between<int, 1, 32767> > startAt;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUAUTONUM_INCLUDE_H