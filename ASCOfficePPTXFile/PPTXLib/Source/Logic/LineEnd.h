#pragma once
#ifndef PPTX_LOGIC_LINEEND_INCLUDE_H_
#define PPTX_LOGIC_LINEEND_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../Limit/LineEndType.h"
#include "./../Limit/LineEndSize.h"

namespace PPTX
{
	namespace Logic
	{
		class LineEnd : public WrapperWritingElement
		{
		public:
			LineEnd();
			virtual ~LineEnd();
			explicit LineEnd(const XML::XNode& node);
			const LineEnd& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			void Merge(nullable_property<LineEnd>& line)const;

		public:
			nullable_property<std::string, Limit::LineEndType> type;
			nullable_property<std::string, Limit::LineEndSize> w;
			nullable_property<std::string, Limit::LineEndSize> len;
		//private:
		public:
			property<std::string> m_name;
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_LINEEND_INCLUDE_H_