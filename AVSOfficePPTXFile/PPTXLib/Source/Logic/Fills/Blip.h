#pragma once
#ifndef PPTX_LOGIC_BLIP_INCLUDE_H_
#define PPTX_LOGIC_BLIP_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../../Limit/BlipCompression.h"
#include "./../UniEffect.h"
#include "DocxFormat/RId.h"
#include <list>

namespace PPTX
{
	namespace Logic
	{

		class Blip : public WrapperWritingElement
		{
		public:
			Blip();
			virtual ~Blip();
			explicit Blip(const XML::XNode& node);
			const Blip& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			virtual std::wstring GetFullPicName()const;
		public:
			property<std::list<UniEffect> > Effects;

			nullable_property<std::string, Limit::BlipCompression> cstate;
			nullable_property<OOX::RId> embed;
			nullable_property<OOX::RId> link;
		//private:
		public:
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLIP_INCLUDE_H_