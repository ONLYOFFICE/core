#pragma once
#ifndef PPTX_LOGIC_HEADING_VARIANT_INCLUDE_H_
#define PPTX_LOGIC_HEADING_VARIANT_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/VariantType.h"

namespace PPTX
{
	namespace Logic
	{

		class HeadingVariant : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(HeadingVariant)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNode oNode = node.ReadNodeNoNS(_T("i4"));
				if (oNode.IsValid())
				{
					m_type		= L"i4";
					m_iContent	= oNode.GetTextExt();
				}
				else
				{
					m_type		 = L"lpstr";
					m_strContent =	 node.ReadNodeNoNS(_T("lpstr")).GetTextExt();
				}
			}
			virtual CString toXML() const
			{
				if (m_type.IsInit() && (m_type->get() == _T("i4")))
				{
					return _T("<vt:variant><vt:i4>") + XmlUtils::IntToString(*m_iContent) + _T("</vt:i4></vt:variant>");
				}
				return _T("<vt:variant><vt:lpstr>") + *m_strContent + _T("</vt:lpstr></vt:variant>");
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("vt:variant"));
				pWriter->EndAttributes();

				if (m_type.IsInit() && (m_type->get() == _T("i4")))
				{
					pWriter->WriteNodeValue(_T("vt:i4"), *m_iContent);
				}
				else
				{
					pWriter->WriteNodeValue(_T("vt:lpstr"), *m_strContent);
				}

				pWriter->EndNode(_T("vt:variant"));
			}

		public:
			nullable_limit<Limit::VariantType>		m_type;
			nullable_string							m_strContent;
			nullable_int							m_iContent;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_HEADING_VARIANT_INCLUDE_H_