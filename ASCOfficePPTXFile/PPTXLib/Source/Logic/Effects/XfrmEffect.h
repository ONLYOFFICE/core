#pragma once
#ifndef PPTX_LOGIC_XFRMEFFECT_INCLUDE_H_
#define PPTX_LOGIC_XFRMEFFECT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class XfrmEffect : public WrapperWritingElement
		{
		public:
			XfrmEffect();
			virtual ~XfrmEffect();
			explicit XfrmEffect(const XML::XNode& node);
			const XfrmEffect& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<int, setter::between<int, -5400000, 5400000> > kx;
			nullable_property<int, setter::between<int, -5400000, 5400000> > ky;
			nullable_property<int> sx;
			nullable_property<int> sy;
			nullable_property<size_t> tx;
			nullable_property<size_t> ty;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_XFRMEFFECT_INCLUDE_H_