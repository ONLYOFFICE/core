#pragma once
#ifndef PPTX_VIEWPROPS_NOTES_TEXT_VIEW_PROPERTIES_INCLUDE_H_
#define PPTX_VIEWPROPS_NOTES_TEXT_VIEW_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "CViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class NotesTextViewPr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(NotesTextViewPr)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				CViewPr = node.ReadNode(_T("p:cViewPr"));

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(CViewPr);

				return XmlUtils::CreateNode(_T("p:notesTextViewPr"), oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord1(0, CViewPr);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:notesTextViewPr"));
				pWriter->EndAttributes();

				CViewPr.toXmlWriter(pWriter);

				pWriter->EndNode(_T("p:notesTextViewPr"));
			}

		public:
			nsViewProps::CViewPr CViewPr;
		protected:
			virtual void FillParentPointersForChilds()
			{
				CViewPr.SetParentPointer(this);
			}
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_NOTES_TEXT_VIEW_PROPERTIES_INCLUDE_H_