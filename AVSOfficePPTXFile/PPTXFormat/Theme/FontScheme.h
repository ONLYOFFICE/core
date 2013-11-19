#pragma once
#ifndef PPTX_THEME_FONTSCHEME_INCLUDE_H_
#define PPTX_THEME_FONTSCHEME_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Logic/FontCollection.h"

namespace PPTX
{
	namespace nsTheme
	{
		class FontScheme : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(FontScheme)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				name = node.GetAttribute(_T("name"));

				majorFont = node.ReadNode(_T("a:majorFont"));
				minorFont = node.ReadNode(_T("a:minorFont"));

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("name"), name);

				XmlUtils::CNodeValue oValue;
				oValue.Write(majorFont);
				oValue.Write(minorFont);

				return XmlUtils::CreateNode(_T("a:fontScheme"), oAttr, oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, name);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, majorFont);
				pWriter->WriteRecord1(1, minorFont);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:fontScheme"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("name"), name);
				pWriter->EndAttributes();

				majorFont.toXmlWriter(pWriter);
				minorFont.toXmlWriter(pWriter);				

				pWriter->EndNode(_T("a:fontScheme"));
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

					if (0 == _at)
						name = pReader->GetString2();
					else
						break;
				}

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							majorFont.m_name = _T("a:majorFont");
							majorFont.fromPPTY(pReader);
							break;
						}
						case 1:
						{
							minorFont.m_name = _T("a:minorFont");
							minorFont.fromPPTY(pReader);							
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

		public:
			CString					name;
			Logic::FontCollection	majorFont;
			Logic::FontCollection	minorFont;
		protected:
			virtual void FillParentPointersForChilds()
			{
				majorFont.SetParentPointer(this);
				minorFont.SetParentPointer(this);
			}
		};
	} // namespace nsTheme
} // namespace PPTX

#endif // PPTX_THEME_FONTSCHEME_INCLUDE_H