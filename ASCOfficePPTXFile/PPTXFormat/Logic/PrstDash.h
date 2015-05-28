#pragma once
#ifndef PPTX_LOGIC_PRSTDASH_INCLUDE_H_
#define PPTX_LOGIC_PRSTDASH_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/PrstDashVal.h"

namespace PPTX
{
	namespace Logic
	{

		class PrstDash : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(PrstDash)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"val", val);
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("val"), val);

				return XmlUtils::CreateNode(_T("a:prstDash"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, val);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;
				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
					case 0:
					{
						val = new Limit::PrstDashVal();
						val->SetBYTECode(pReader->GetUChar());
						break;
					}
					default:
						break;
					}
				}

				pReader->Seek(_end_rec);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				CString sNodeNamespace;
				CString sAttrNamespace;
				if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
				{
					sNodeNamespace = _T("w14:");
					sAttrNamespace = sNodeNamespace;
				}
				else
					sNodeNamespace = _T("a:");
				pWriter->StartNode(sNodeNamespace + _T("prstDash"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(sAttrNamespace + _T("val"), val);
				pWriter->EndAttributes();

				pWriter->EndNode(sNodeNamespace + _T("prstDash"));
			}

		public:
			nullable_limit<Limit::PrstDashVal> val;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PRSTDASH_INCLUDE_H_