#pragma once
#ifndef PPTX_LOGIC_LIN_INCLUDE_H_
#define PPTX_LOGIC_LIN_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class Lin : public WrapperWritingElement
		{
		public:
			Lin();
			virtual ~Lin();
			explicit Lin(const XML::XNode& node);
			const Lin& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<int, setter::between<int, 0, 21600000> > ang;
			nullable_property<bool> scaled;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_LIN_INCLUDE_H_