#pragma once
#ifndef PPTX_VIEWPROPS_OUTLINE_VIEW_PROPERTIES_INCLUDE_H_
#define PPTX_VIEWPROPS_OUTLINE_VIEW_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "CViewPr.h"
#include "Sld.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class OutlineViewPr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(OutlineViewPr)

			OutlineViewPr& operator=(const OutlineViewPr& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				CViewPr = oSrc.CViewPr;
				SldLst.Copy(oSrc.SldLst);

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				CViewPr = node.ReadNode(_T("p:cViewPr"));
				SldLst.RemoveAll();

				node.ReadNode(_T("p:sldLst")).LoadArray(_T("p:sld"), SldLst);
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(CViewPr);

				if (0 < SldLst.GetCount())
					oValue.WriteArray(_T("p:sldLst"), SldLst);

				return XmlUtils::CreateNode(_T("p:outlineViewPr"), oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord1(0, CViewPr);

				pWriter->StartRecord(1);
				ULONG len = (ULONG)SldLst.GetCount();
				pWriter->WriteULONG(len);
				for (ULONG i = 0; i < len; i++)
				{
					pWriter->WriteRecord1(2, SldLst[i]);
				}
				pWriter->EndRecord();
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:outlineViewPr"));
				pWriter->EndAttributes();

				CViewPr.toXmlWriter(pWriter);
				pWriter->WriteArray(_T("p:sldLst"), SldLst);
				
				pWriter->EndNode(_T("p:outlineViewPr"));
			}

		public:
			nsViewProps::CViewPr			CViewPr;
			CAtlArray<nsViewProps::Sld>		SldLst;
		protected:
			virtual void FillParentPointersForChilds()
			{
				CViewPr.SetParentPointer(this);

				size_t count = SldLst.GetCount();
				for (size_t i = 0; i < count; ++i)
					SldLst[i].SetParentPointer(this);
			}
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_OUTLINE_VIEW_PROPERTIES_INCLUDE_H_