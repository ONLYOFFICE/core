#pragma once
#ifndef PPTX_LOGIC_FLD_INCLUDE_H_
#define PPTX_LOGIC_FLD_INCLUDE_H_

#include "RunBase.h"
#include "./../RunProperties.h"
#include "./../TextParagraphPr.h"
//#include "../../../../Common/DocxFormat/Source/Utility/Encoding.h"

namespace PPTX
{
	namespace Logic
	{
		class Fld : public RunBase
		{
		public:
			PPTX_LOGIC_BASE(Fld)

			Fld& operator=(const Fld& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				id		= oSrc.id;
				type	= oSrc.type;
				rPr		= oSrc.rPr;
				pPr		= oSrc.pPr;
				text	= oSrc.text;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				id		= node.GetAttribute(_T("id"));
				node.ReadAttributeBase(L"type", type);

				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int count = oNodes.GetCount();
					for (int i = 0; i < count; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						CString strName = XmlUtils::GetNameNoNS(oNode.GetName());

						if (_T("rPr") == strName)
						{
							if (!rPr.IsInit())
								rPr = oNode;
						}
						else if (_T("pPr") == strName)
						{
							if (!pPr.IsInit())
								pPr = oNode;
						}
						else if (_T("t") == strName)
						{
							if (!text.IsInit())
								text = oNode.GetTextExt();
						}
					}
				}
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("id"), id);
				oAttr.Write(_T("type"), type);

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(rPr);
				oValue.WriteNullable(pPr);
				
				if (text.IsInit())
					oValue.m_strValue += (_T("<a:t>") + *text + _T("</a:t>"));

				return XmlUtils::CreateNode(_T("a:fld"), oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:fld"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("id"), id);
				pWriter->WriteAttribute(_T("type"), type);
				pWriter->EndAttributes();

				pWriter->Write(rPr);
				pWriter->Write(pPr);
				
				if (text.IsInit())
					pWriter->WriteString(_T("<a:t>") + *text + _T("</a:t>"));
				
				pWriter->EndNode(_T("a:fld"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(PARRUN_TYPE_FLD);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, id);
				pWriter->WriteString2(1, type);
				pWriter->WriteString2(2, text);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord2(0, rPr);
				pWriter->WriteRecord2(1, pPr);

				pWriter->EndRecord();

				if (pWriter->m_pCommon->m_pNativePicker->m_bIsEmbeddedFonts)
					pWriter->m_pCommon->m_pNativePicker->m_oEmbeddedFonts.CheckString(text);
			}

			void SetText(const CString& src)
			{
				text = src;
			}

			virtual CString GetText()const{return text.get_value_or(_T(""));};
		public:
			CString						id;
			
			nullable_string				type;

			nullable<RunProperties>		rPr;
			nullable<TextParagraphPr>	pPr;
		private:
			nullable_string				text;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(rPr.IsInit())
					rPr->SetParentPointer(this);
				if(pPr.IsInit())
					pPr->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_FLD_INCLUDE_H