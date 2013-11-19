#pragma once
#ifndef PPTX_LOGIC_PATTFILL_INCLUDE_H_
#define PPTX_LOGIC_PATTFILL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../../Limit/PattFillVal.h"
#include "./../UniColor.h"
#include <boost\shared_ptr.hpp>

namespace PPTX
{
	namespace Logic
	{

		class PattFill : public WrapperWritingElement
		{
		public:
			PattFill();
			virtual ~PattFill();
			explicit PattFill(const XML::XNode& node);
			const PattFill& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string, Limit::PattFillVal> prst;

			UniColor fgClr;
			UniColor bgClr;
		private:
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PATTFILL_INCLUDE_H_