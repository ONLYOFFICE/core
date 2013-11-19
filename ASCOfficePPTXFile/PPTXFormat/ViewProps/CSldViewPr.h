#pragma once
#ifndef PPTX_VIEWPROPS_COMMON_SLIDE_VIEW_PROPERTIES_INCLUDE_H_
#define PPTX_VIEWPROPS_COMMON_SLIDE_VIEW_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "CViewPr.h"
#include "Guide.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class CSldViewPr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CSldViewPr)

			CSldViewPr& operator=(const CSldViewPr& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				CViewPr				= oSrc.CViewPr;
				attrShowGuides		= oSrc.attrShowGuides;
				attrSnapToGrid		= oSrc.attrSnapToGrid;
				attrSnapToObjects	= oSrc.attrSnapToObjects;
				
				GuideLst.Copy(oSrc.GuideLst);

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				GuideLst.RemoveAll();

				CViewPr = node.ReadNode(_T("p:cViewPr"));

				node.ReadNode(_T("p:guideLst")).LoadArray(_T("p:guide"), GuideLst);

				node.ReadAttributeBase(L"showGuides", attrShowGuides);
				node.ReadAttributeBase(L"snapToGrid", attrSnapToGrid);
				node.ReadAttributeBase(L"snapToObjects", attrSnapToObjects);

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("showGuides"), attrShowGuides);
				oAttr.Write(_T("snapToGrid"), attrSnapToGrid);
				oAttr.Write(_T("snapToObjects"), attrSnapToObjects);

				XmlUtils::CNodeValue oValue;
				oValue.Write(CViewPr);
				oValue.WriteArray(_T("p:guideLst"), GuideLst);

				return XmlUtils::CreateNode(_T("p:cSldViewPr"), oAttr, oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, attrShowGuides);
				pWriter->WriteBool2(1, attrSnapToGrid);
				pWriter->WriteBool2(2, attrSnapToObjects);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, CViewPr);

				pWriter->StartRecord(1);
				ULONG len = (ULONG)GuideLst.GetCount();
				pWriter->WriteULONG(len);
				for (ULONG i = 0; i < len; i++)
				{
					pWriter->WriteRecord1(2, GuideLst[i]);
				}
				pWriter->EndRecord();
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:cSldViewPr"));

				pWriter->StartAttributes();

				pWriter->WriteAttribute(_T("showGuides"), attrShowGuides);
				pWriter->WriteAttribute(_T("snapToGrid"), attrSnapToGrid);
				pWriter->WriteAttribute(_T("snapToObjects"), attrSnapToObjects);

				pWriter->EndAttributes();

				CViewPr.toXmlWriter(pWriter);
				pWriter->WriteArray(_T("p:guideLst"), GuideLst);	

				pWriter->EndNode(_T("p:cSldViewPr"));
			}

		public:
			nsViewProps::CViewPr	CViewPr;
			CAtlArray<Guide>		GuideLst;

			nullable_bool			attrShowGuides;
			nullable_bool			attrSnapToGrid;
			nullable_bool			attrSnapToObjects;
		protected:
			virtual void FillParentPointersForChilds()
			{
				CViewPr.SetParentPointer(this);

				size_t count = GuideLst.GetCount();
				for (size_t i = 0; i < count; ++i)
					GuideLst[i].SetParentPointer(this);
			}
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_COMMON_SLIDE_VIEW_PROPERTIES_INCLUDE_H_