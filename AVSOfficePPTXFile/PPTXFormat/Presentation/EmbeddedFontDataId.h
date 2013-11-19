#pragma once
#ifndef PPTX_PRESENTATION_EMBEDDEDFONTDATAID_INCLUDE_H_
#define PPTX_PRESENTATION_EMBEDDEDFONTDATAID_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsPresentation
	{
		class EmbeddedFontDataId : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(EmbeddedFontDataId)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name = XmlUtils::GetNameNoNS(node.GetName());

				rid = node.GetAttribute(_T("r:id"));
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("r:id"), rid);

				return XmlUtils::CreateNode(_T("p:") + m_name, oAttr);
			}
		public:
			CString rid;
		//private:
		public:
			CString m_name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsPresentation
} // namespace PPTX

#endif // PPTX_PRESENTATION_EMBEDDEDFONTDATAID_INCLUDE_H_