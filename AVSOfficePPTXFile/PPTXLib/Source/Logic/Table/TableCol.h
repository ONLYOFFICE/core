#pragma once
#ifndef PPTX_LOGIC_TABLECOL_INCLUDE_H_
#define PPTX_LOGIC_TABLECOL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"

namespace PPTX
{
	namespace Logic
	{
		class TableCol : public WrapperWritingElement
		{
		public:
			TableCol();
			virtual ~TableCol();			
			explicit TableCol(const XML::XNode& node);
			const TableCol& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			
		public:
			property<int> Width;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLECOL_INCLUDE_H_