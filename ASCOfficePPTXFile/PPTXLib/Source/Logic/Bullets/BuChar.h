#pragma once
#ifndef PPTX_LOGIC_BUCHAR_INCLUDE_H_
#define PPTX_LOGIC_BUCHAR_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"

namespace PPTX
{
	namespace Logic
	{
		class BuChar : public WrapperWritingElement
		{
		public:
			BuChar();
			virtual ~BuChar();			
			explicit BuChar(const XML::XNode& node);
			const BuChar& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string> Char;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUCHAR_INCLUDE_H