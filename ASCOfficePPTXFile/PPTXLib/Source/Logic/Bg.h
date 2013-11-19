#pragma once
#ifndef PPTX_LOGIC_SLIDE_BACKGROUND_INCLUDE_H_
#define PPTX_LOGIC_SLIDE_BACKGROUND_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../Limit/BWMode.h"
#include "BgPr.h"
#include "StyleRef.h"

namespace PPTX
{
	namespace Logic
	{

		class Bg : public WrapperWritingElement
		{
		public:
			Bg();
			virtual ~Bg();
			explicit Bg(const XML::XNode& node);
			const Bg& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string, Limit::BWMode> bwMode;
			//boost::shared_ptr<WrapperWritingElement> background;
			nullable_property<BgPr> bgPr;
			nullable_property<StyleRef> bgRef;

			virtual void GetBackground(Logic::BgPr& bg, DWORD& ARGB)const;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SLIDE_BACKGROUND_INCLUDE_H_