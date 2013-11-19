#pragma once
#ifndef PPTX_PPTSLIDES_XMLID_INCLUDE_H_
#define PPTX_PPTSLIDES_XMLID_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable_property.h"
#include "DocxFormat/RId.h"

namespace PPTX
{
	namespace Logic
	{
		class XmlId : public WrapperWritingElement
		{
		public:
			XmlId();
			virtual ~XmlId();
			explicit XmlId(const XML::XNode& node);
			const XmlId& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string> id;
			property<OOX::RId> rid;
		public:
			property<std::string> m_name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_PPTSLIDES_XMLID_INCLUDE_H_