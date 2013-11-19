#pragma once
#ifndef PPTX_VIEWPROPS_SORTER_VIEW_PROPERTIES_INCLUDE_H_
#define PPTX_VIEWPROPS_SORTER_VIEW_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "CViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class SorterViewPr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SorterViewPr)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"showFormatting", attrShowFormatting);
				CViewPr = node.ReadNode(_T("p:cViewPr"));

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("showFormatting"), attrShowFormatting);

				XmlUtils::CNodeValue oValue;
				oValue.Write(CViewPr);

				return XmlUtils::CreateNode(_T("p:sorterViewPr"), oAttr, oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(g_nodeAttributeStart);
				pWriter->WriteBool2(0, attrShowFormatting);
				pWriter->WriteBYTE(g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, CViewPr);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:sorterViewPr"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("showFormatting"), attrShowFormatting);
				pWriter->EndAttributes();

				CViewPr.toXmlWriter(pWriter);

				pWriter->EndNode(_T("p:sorterViewPr"));
			}

		public:
			nsViewProps::CViewPr	CViewPr;
			nullable_bool			attrShowFormatting;
		protected:
			virtual void FillParentPointersForChilds()
			{
				CViewPr.SetParentPointer(this);
			}
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_SORTER_VIEW_PROPERTIES_INCLUDE_H_