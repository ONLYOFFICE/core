#pragma once
#ifndef PPTX_LOGIC_PATTFILL_INCLUDE_H_
#define PPTX_LOGIC_PATTFILL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/PattFillVal.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class PattFill : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(PattFill)

			PattFill& operator=(const PattFill& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				prst = oSrc.prst;

				fgClr = oSrc.fgClr;
				bgClr = oSrc.bgClr;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_namespace = XmlUtils::GetNamespace(node.GetName());

				node.ReadAttributeBase(L"prst", prst);
				
				XmlUtils::CXmlNode oNode;
				if (node.GetNode(_T("a:fgClr"), oNode))
					fgClr.GetColorFrom(oNode);
				if (node.GetNode(_T("a:bgClr"), oNode))
					bgClr.GetColorFrom(oNode);

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("prst"), prst);

				XmlUtils::CNodeValue oValue;
				oValue.Write(_T("a:fgClr"), fgClr);
				oValue.Write(_T("a:bgClr"), bgClr);

				CString strName = (_T("") == m_namespace) ? _T("pattFill") : (m_namespace + _T(":pattFill"));
				return XmlUtils::CreateNode(strName, oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				CString strName = (_T("") == m_namespace) ? _T("pattFill") : (m_namespace + _T(":pattFill"));
				pWriter->StartNode(strName);

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("prst"), prst);
				pWriter->EndAttributes();

				pWriter->StartNode(_T("a:fgClr"));
				pWriter->EndAttributes();
				fgClr.toXmlWriter(pWriter);
				pWriter->EndNode(_T("a:fgClr"));

				pWriter->StartNode(_T("a:bgClr"));
				pWriter->EndAttributes();
				bgClr.toXmlWriter(pWriter);
				pWriter->EndNode(_T("a:bgClr"));

				pWriter->EndNode(strName);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(FILL_TYPE_PATT);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, prst);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, fgClr);
				pWriter->WriteRecord1(1, bgClr);
				
				pWriter->EndRecord();
			}

		public:
			nullable_limit<Limit::PattFillVal> prst;

			UniColor fgClr;
			UniColor bgClr;

			CString m_namespace;
		protected:
			virtual void FillParentPointersForChilds()
			{
				fgClr.SetParentPointer(this);
				bgClr.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PATTFILL_INCLUDE_H_