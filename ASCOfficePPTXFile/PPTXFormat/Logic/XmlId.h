#pragma once
#ifndef PPTX_PPTSLIDES_XMLID_INCLUDE_H_
#define PPTX_PPTSLIDES_XMLID_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "../DocxFormat/RId.h"

namespace PPTX
{
	namespace Logic
	{
		class XmlId : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(XmlId)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name		= XmlUtils::GetNameNoNS(node.GetName());
				node.ReadAttributeBase(L"r:id", rid);
				node.ReadAttributeBase(L"id", id);
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("r:id"), rid.ToString());
				oAttr.Write(_T("id"), id);

				return XmlUtils::CreateNode(_T("p:") + m_name, oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, id);
				rid.toPPTY(1, pWriter);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:") + m_name);
				
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("id"), id);
				pWriter->WriteAttribute(_T("r:id"), rid.ToString());
				pWriter->EndAttributes();				

				pWriter->EndNode(_T("p:") + m_name);
			}

		public:
			nullable_string			id;
			PPTX::RId				rid;
		public:
			CString					m_name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_PPTSLIDES_XMLID_INCLUDE_H_