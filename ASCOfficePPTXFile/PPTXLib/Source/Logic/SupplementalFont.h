#pragma once
#ifndef PPTX_LOGIC_SUPPLEMENTALFONT_INCLUDE_H_
#define PPTX_LOGIC_SUPPLEMENTALFONT_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class SupplementalFont : public WrapperWritingElement
		{
		public:
			SupplementalFont();
			virtual ~SupplementalFont();
			explicit SupplementalFont(const XML::XNode& node);
			const SupplementalFont& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string> script;
			property<std::string> typeface;
		//private:
		public:
			property<std::string> m_name;
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SUPPLEMENTALFONT_INCLUDE_H_