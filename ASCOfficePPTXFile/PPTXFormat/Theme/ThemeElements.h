#pragma once
#ifndef PPTX_THEME_THEMEELEMENTS_INCLUDE_H_
#define PPTX_THEME_THEMEELEMENTS_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "ClrScheme.h"
#include "FontScheme.h"
#include "FmtScheme.h"

namespace PPTX
{
	namespace nsTheme
	{
		class ThemeElements : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(ThemeElements)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				clrScheme	= node.ReadNode(_T("a:clrScheme"));
				fontScheme	= node.ReadNode(_T("a:fontScheme"));
				fmtScheme	= node.ReadNode(_T("a:fmtScheme"));

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(clrScheme);
				oValue.Write(fontScheme);
				oValue.Write(fmtScheme);

				return XmlUtils::CreateNode(_T("a:themeElements"), oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord1(0, clrScheme);
				pWriter->WriteRecord1(1, fontScheme);
				pWriter->WriteRecord1(2, fmtScheme);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:themeElements"));
				pWriter->EndAttributes();

				clrScheme.toXmlWriter(pWriter);
				fontScheme.toXmlWriter(pWriter);
				fmtScheme.toXmlWriter(pWriter);

				pWriter->EndNode(_T("a:themeElements"));
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							clrScheme.fromPPTY(pReader);
							break;
						}
						case 1:
						{
							fontScheme.fromPPTY(pReader);
							break;
						}
						case 2:
						{
							fmtScheme.fromPPTY(pReader);
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

		public:
			ClrScheme	clrScheme;
			FontScheme	fontScheme;
			FmtScheme	fmtScheme;
		protected:
			virtual void FillParentPointersForChilds()
			{
				clrScheme.SetParentPointer(this);
				fontScheme.SetParentPointer(this);
				fmtScheme.SetParentPointer(this);
			}
		};
	} // namespace nsTheme
} // namespace PPTX

#endif // PPTX_THEME_THEMEELEMENTS_INCLUDE_H