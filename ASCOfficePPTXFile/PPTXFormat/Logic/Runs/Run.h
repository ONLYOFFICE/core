#pragma once
#ifndef PPTX_LOGIC_RUN_INCLUDE_H_
#define PPTX_LOGIC_RUN_INCLUDE_H_

#include "RunBase.h"
#include "./../RunProperties.h"
//#include "../../../../Common/DocxFormat/Source/Utility/Encoding.h"

namespace PPTX
{
	namespace Logic
	{
		class Run : public RunBase
		{
		public:
			PPTX_LOGIC_BASE(Run)

			Run& operator=(const Run& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				rPr = oSrc.rPr;
				text = oSrc.text;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
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
							rPr = oNode;
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
				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(rPr);
				
				if (text.IsInit())
					oValue.m_strValue += (_T("<a:t>") + *text + _T("</a:t>"));

				return XmlUtils::CreateNode(_T("a:r"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:r"));
				pWriter->EndAttributes();

				if (rPr.is_init())
					rPr->m_name = _T("a:rPr");
				pWriter->Write(rPr);

				if (text.is_init())
				{
					pWriter->WriteString(_T("<a:t>"));
					pWriter->WriteStringXML(*text);
					pWriter->WriteString(_T("</a:t>"));
				}
				
				pWriter->EndNode(_T("a:r"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(PARRUN_TYPE_RUN);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, text);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord2(0, rPr);

				pWriter->EndRecord();

#ifdef _WIN32
                if (pWriter->m_pCommon->m_pNativePicker->m_bIsEmbeddedFonts)
					pWriter->m_pCommon->m_pNativePicker->m_oEmbeddedFonts.CheckString(text);
#endif
			}

			virtual CString GetText()const{return text.get_value_or(_T(""));};
			void SetText(const CString& srcText)
			{
				text = srcText;
			}
		public:
			nullable<RunProperties> rPr;
		private:
			nullable_string			text;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(rPr.IsInit())
					rPr->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_RUN_INCLUDE_H
