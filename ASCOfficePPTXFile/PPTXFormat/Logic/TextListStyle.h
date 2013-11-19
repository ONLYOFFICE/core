#pragma once
#ifndef PPTX_LOGIC_TEXTSTYLE_INCLUDE_H_
#define PPTX_LOGIC_TEXTSTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "TextParagraphPr.h"

namespace PPTX
{
	namespace Logic
	{

		class TextListStyle : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TextListStyle)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name = node.GetName();

				/*defPPr*/ levels[9] = node.ReadNode(_T("a:defPPr"));
				/*lvl1pPr*/levels[0] = node.ReadNode(_T("a:lvl1pPr"));
				/*lvl2pPr*/levels[1] = node.ReadNode(_T("a:lvl2pPr"));
				/*lvl3pPr*/levels[2] = node.ReadNode(_T("a:lvl3pPr"));
				/*lvl4pPr*/levels[3] = node.ReadNode(_T("a:lvl4pPr"));
				/*lvl5pPr*/levels[4] = node.ReadNode(_T("a:lvl5pPr"));
				/*lvl6pPr*/levels[5] = node.ReadNode(_T("a:lvl6pPr"));
				/*lvl7pPr*/levels[6] = node.ReadNode(_T("a:lvl7pPr"));
				/*lvl8pPr*/levels[7] = node.ReadNode(_T("a:lvl8pPr"));
				/*lvl9pPr*/levels[8] = node.ReadNode(_T("a:lvl9pPr"));

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(/*defPPr*/  levels[9]);
				oValue.WriteNullable(/*lvl1pPr*/ levels[0]);
				oValue.WriteNullable(/*lvl2pPr*/ levels[1]);
				oValue.WriteNullable(/*lvl3pPr*/ levels[2]);
				oValue.WriteNullable(/*lvl4pPr*/ levels[3]);
				oValue.WriteNullable(/*lvl5pPr*/ levels[4]);
				oValue.WriteNullable(/*lvl6pPr*/ levels[5]);
				oValue.WriteNullable(/*lvl7pPr*/ levels[6]);
				oValue.WriteNullable(/*lvl8pPr*/ levels[7]);
				oValue.WriteNullable(/*lvl9pPr*/ levels[8]);

				return XmlUtils::CreateNode(m_name, oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord2(0, levels[0]);
				pWriter->WriteRecord2(1, levels[1]);
				pWriter->WriteRecord2(2, levels[2]);
				pWriter->WriteRecord2(3, levels[3]);
				pWriter->WriteRecord2(4, levels[4]);
				pWriter->WriteRecord2(5, levels[5]);
				pWriter->WriteRecord2(6, levels[6]);
				pWriter->WriteRecord2(7, levels[7]);
				pWriter->WriteRecord2(8, levels[8]);
				pWriter->WriteRecord2(9, levels[9]);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(m_name);				
				pWriter->EndAttributes();

				pWriter->Write(/*defPPr*/  levels[9]);
				pWriter->Write(/*lvl1pPr*/ levels[0]);
				pWriter->Write(/*lvl2pPr*/ levels[1]);
				pWriter->Write(/*lvl3pPr*/ levels[2]);
				pWriter->Write(/*lvl4pPr*/ levels[3]);
				pWriter->Write(/*lvl5pPr*/ levels[4]);
				pWriter->Write(/*lvl6pPr*/ levels[5]);
				pWriter->Write(/*lvl7pPr*/ levels[6]);
				pWriter->Write(/*lvl8pPr*/ levels[7]);
				pWriter->Write(/*lvl9pPr*/ levels[8]);

				pWriter->EndNode(m_name);
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				CString arr_names[10] = {_T("a:lvl1pPr"), _T("a:lvl2pPr"), _T("a:lvl3pPr"), _T("a:lvl4pPr"), _T("a:lvl5pPr"), 
					_T("a:lvl6pPr"), _T("a:lvl7pPr"), _T("a:lvl8pPr"), _T("a:lvl9pPr"), _T("a:defPPr")};

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					levels[_at] = new TextParagraphPr();
					levels[_at]->m_name = arr_names[_at];
					levels[_at]->fromPPTY(pReader);
				}

				pReader->Seek(_end_rec);				
			}

			void Merge(nullable<TextListStyle>& lstStyle)const
			{
				if(!lstStyle.is_init())
					lstStyle = TextListStyle();
				for(int i = 0; i < 10; i++)
					if(levels[i].is_init())
						levels[i]->Merge(lstStyle->levels[i]);
			}
		public:
			nullable<TextParagraphPr> levels[10];
		//private:
		public:
			mutable CString m_name;
		protected:
			virtual void FillParentPointersForChilds()
			{
				for(int i = 0; i < 10; i++)
				{
					if(levels[i].is_init())
						levels[i]->SetParentPointer(this);
				}
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TEXTSTYLE_INCLUDE_H_