#pragma once
#ifndef PPTX_LOGIC_TEXTFONT_INCLUDE_H_
#define PPTX_LOGIC_TEXTFONT_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class TextFont : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TextFont)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name = node.GetName();

				node.ReadAttributeBase(L"charset", charset);
				node.ReadAttributeBase(L"panose", panose);
				node.ReadAttributeBase(L"pitchFamily", pitchFamily);

				typeface = node.GetAttribute(_T("typeface"));
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("typeface"), typeface);
				oAttr.Write(_T("pitchFamily"), pitchFamily);
				oAttr.Write(_T("charset"), charset);
				oAttr.Write(_T("panose"), panose);

				return XmlUtils::CreateNode(m_name, oAttr);
			}

			void Merge(nullable<TextFont>& font)const
			{
				if(!font.is_init())
					font = TextFont();
				if(charset.is_init()) font->charset = *charset;
				if(panose.is_init()) font->panose = *panose;
				if(pitchFamily.is_init()) font->pitchFamily = *pitchFamily;
				font->typeface = typeface;
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{			
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, charset);
				pWriter->WriteString2(1, panose);
				pWriter->WriteString2(2, pitchFamily);
				
				/*
				pWriter->WriteString1(3, typeface);
				*/

				CString sPick = pWriter->m_oCommon.m_pNativePicker->GetTypefacePick(*this);
				pWriter->WriteString1(3, sPick);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
						case 0:
						{
							charset = pReader->GetString2();
							break;
						}
						case 1:
						{
							panose = pReader->GetString2();
							break;
						}
						case 2:
						{
							pitchFamily = pReader->GetString2();
							break;
						}
						case 3:
						{
							typeface = pReader->GetString2();
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
				pWriter->StartNode(m_name);				
				
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("typeface"), typeface);
				pWriter->WriteAttribute(_T("pitchFamily"), pitchFamily);
				pWriter->WriteAttribute(_T("charset"), charset);
				pWriter->WriteAttribute(_T("panose"), panose);
				pWriter->EndAttributes();

				pWriter->EndNode(m_name);
			}

		public:
			nullable_string charset;
			nullable_string panose;
			nullable_string pitchFamily;
			CString typeface;
		//private:
		public:
			CString m_name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TEXTFONT_INCLUDE_H_