#pragma once
#ifndef PPTX_LOGIC_TCSTYLE_INCLUDE_H_
#define PPTX_LOGIC_TCSTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "TcBdr.h"
#include "Cell3D.h"
#include "FillStyle.h"
#include "StyleRef.h"

namespace PPTX
{
	namespace Logic
	{
		class TcStyle : public WrapperWritingElement
		{
		public:
			TcStyle();
			virtual ~TcStyle();			
			explicit TcStyle(const XML::XNode& node);
			const TcStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<TcBdr> tcBdr;
			//boost::shared_ptr<WrapperWritingElement> Fill;
			nullable_property<StyleRef> fillRef;
			nullable_property<FillStyle>  fill;
			nullable_property<Cell3D> cell3D;
		protected:
			virtual void FillParentPointersForChilds();
		};

	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TCSTYLE_INCLUDE_H