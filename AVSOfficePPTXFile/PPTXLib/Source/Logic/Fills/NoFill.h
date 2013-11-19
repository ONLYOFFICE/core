#pragma once
#ifndef PPTX_LOGIC_NOFILL_INCLUDE_H_
#define PPTX_LOGIC_NOFILL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class NoFill : public WrapperWritingElement
		{
		public:
			NoFill();
			virtual ~NoFill();
			explicit NoFill(const XML::XNode& node);
			const NoFill& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		private:
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_NOFILL_INCLUDE_H_