#pragma once
#ifndef PPTX_LOGIC_INNERSHDW_INCLUDE_H_
#define PPTX_LOGIC_INNERSHDW_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class InnerShdw : public WrapperWritingElement
		{
		public:
			InnerShdw();
			virtual ~InnerShdw();
			explicit InnerShdw(const XML::XNode& node);
			const InnerShdw& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			UniColor Color;

			nullable_property<int, setter::only_positive<int> > blurRad;
			nullable_property<int, setter::between<int, 0, 21600000> > dir;
			nullable_property<int, setter::only_positive<int> > dist;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_INNERSHDW_INCLUDE_H_