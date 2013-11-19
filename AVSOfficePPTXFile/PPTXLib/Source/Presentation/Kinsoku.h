#pragma once
#ifndef PPTX_PRESENTATION_KINSOKU_INCLUDE_H_
#define PPTX_PRESENTATION_KINSOKU_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsPresentation
	{
		class Kinsoku : public WrapperWritingElement
		{
		public:
			Kinsoku();
			virtual ~Kinsoku();
			explicit Kinsoku(const XML::XNode& node);
			const Kinsoku& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string> invalEndChars;
			property<std::string> invalStChars;
			nullable_property<std::string> lang;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsPresentation
} // namespace PPTX

#endif // PPTX_PRESENTATION_KINSOKU_INCLUDE_H_