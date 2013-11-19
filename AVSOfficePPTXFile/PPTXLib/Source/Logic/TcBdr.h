#pragma once
#ifndef PPTX_LOGIC_TCBDR_INCLUDE_H_
#define PPTX_LOGIC_TCBDR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "LineStyle.h"

namespace PPTX
{
	namespace Logic
	{
		class TcBdr : public WrapperWritingElement
		{
		public:
			TcBdr();
			virtual ~TcBdr();			
			explicit TcBdr(const XML::XNode& node);
			const TcBdr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<LineStyle> left;
			nullable_property<LineStyle> right;
			nullable_property<LineStyle> top;
			nullable_property<LineStyle> bottom;
			nullable_property<LineStyle> insideH;
			nullable_property<LineStyle> insideV;
			nullable_property<LineStyle> tl2br;
			nullable_property<LineStyle> tr2bl;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TCBDR_INCLUDE_H