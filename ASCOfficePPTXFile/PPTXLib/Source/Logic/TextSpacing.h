#pragma once
#ifndef PPTX_LOGIC_TEXTSPACING_INCLUDE_H_
#define PPTX_LOGIC_TEXTSPACING_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "Unit.h"

namespace PPTX
{
	namespace Logic
	{

		class TextSpacing : public WrapperWritingElement
		{
		public:
			TextSpacing();
			virtual ~TextSpacing();
			explicit TextSpacing(const XML::XNode& node);
			const TextSpacing& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<int> spcPct;
			nullable_property<int, setter::between<int, 0, 158400> > spcPts;

			int GetVal()const;
		//private:
		public:
			property<std::string> m_name;
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TEXTSPACING_INCLUDE_H_