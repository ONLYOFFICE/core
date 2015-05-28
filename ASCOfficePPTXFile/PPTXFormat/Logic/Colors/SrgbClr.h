#pragma once
#ifndef PPTX_LOGIC_SRGBCLR_INCLUDE_H_
#define PPTX_LOGIC_SRGBCLR_INCLUDE_H_

#include "ColorBase.h"

namespace PPTX
{
	namespace Logic
	{
		class SrgbClr : public ColorBase
		{
		public:
			PPTX_LOGIC_BASE(SrgbClr)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				CString val = node.GetAttribute(_T("val"));
				red		= HexString2Int(val.Mid(0, 2));
				green	= HexString2Int(val.Mid(2, 2));
				blue	= HexString2Int(val.Mid(4, 2));

				Modifiers.clear();
				node.LoadArray(_T("*"), Modifiers);
			}


			virtual CString toXML() const
			{
				CString str = _T("");
				str.Format(_T("%.02X%.02X%.02X"), red, green, blue);
						
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("val"), str);

				XmlUtils::CNodeValue oValue;
				oValue.WriteArray(Modifiers);

				return XmlUtils::CreateNode(_T("a:srgbClr"), oAttr, oValue);
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

				pWriter->StartNode(sNodeNamespace + _T("srgbClr"));
				
				CString str = _T("");
				str.Format(_T("%.02X%.02X%.02X"), red, green, blue);
						
				pWriter->StartAttributes();
				pWriter->WriteAttribute(sAttrNamespace + _T("val"), str);
				pWriter->EndAttributes();

				size_t nCount = Modifiers.size();
				for (size_t i = 0; i < nCount; ++i)
					Modifiers[i].toXmlWriter(pWriter);
				
				pWriter->EndNode(sNodeNamespace + _T("srgbClr"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(COLOR_TYPE_SRGB);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBYTE(0); pWriter->WriteBYTE(red);
				pWriter->WriteBYTE(1); pWriter->WriteBYTE(green);
				pWriter->WriteBYTE(2); pWriter->WriteBYTE(blue);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				ULONG len = (ULONG)Modifiers.size();
				if (len != 0)
				{
					pWriter->StartRecord(0);
					pWriter->WriteULONG(len);

					for (ULONG i = 0; i < len; ++i)
					{
						pWriter->WriteRecord1(1, Modifiers[i]);
					}

					pWriter->EndRecord();
				}

				pWriter->EndRecord();
			}

		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SRGBCLR_INCLUDE_H
