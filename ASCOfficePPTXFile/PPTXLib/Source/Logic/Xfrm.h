#pragma once
#ifndef PPTX_LOGIC_XFRM_INCLUDE_H_
#define PPTX_LOGIC_XFRM_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "nullable_property.h"
#include "property.h"

namespace PPTX
{
	namespace Logic
	{
		class Xfrm : public WrapperWritingElement
		{
		public:
			Xfrm();
			virtual ~Xfrm();
			explicit Xfrm(const XML::XNode& node);
			const Xfrm& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			void Merge(nullable_property<Xfrm>& xfrm)const;

		public:
			nullable_property<int> offX;
			nullable_property<int> offY;
			nullable_property<int, setter::only_positive<int> > extX;
			nullable_property<int, setter::only_positive<int> > extY;
			nullable_property<int> chOffX;
			nullable_property<int> chOffY;
			nullable_property<int, setter::only_positive<int> > chExtX;
			nullable_property<int, setter::only_positive<int> > chExtY;

			nullable_property<bool> flipH;
			nullable_property<bool> flipV;
			nullable_property<int> rot;
		public:
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_XFRM_INCLUDE_H