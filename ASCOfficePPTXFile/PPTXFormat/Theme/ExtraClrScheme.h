#pragma once
#ifndef PPTX_THEME_EXTRACLRSCHEME_INCLUDE_H_
#define PPTX_THEME_EXTRACLRSCHEME_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "ClrScheme.h"
#include "./../Logic/ClrMap.h"

namespace PPTX
{
	namespace nsTheme
	{
		class ExtraClrScheme : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(ExtraClrScheme)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				clrScheme	= node.ReadNode(_T("a:clrScheme"));
				clrMap		= node.ReadNode(_T("a:clrMap"));

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(clrScheme);
				oValue.WriteNullable(clrMap);

				return XmlUtils::CreateNode(_T("a:extraClrScheme"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:extraClrScheme"));
				pWriter->EndAttributes();

				clrScheme.toXmlWriter(pWriter);
				pWriter->Write(clrMap);

				pWriter->EndNode(_T("a:extraClrScheme"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord1(0, clrScheme);
				pWriter->WriteRecord2(1, clrMap);
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _e = pReader->GetPos() + pReader->GetLong() + 4;

				while (pReader->GetPos() < _e)
				{
					BYTE _rec = pReader->GetUChar();

					switch (_rec)
					{
						case 0:
						{
							clrScheme.fromPPTY(pReader);
							break;
						}
						case 1:
						{
							clrMap = new Logic::ClrMap();
							clrMap->m_name = _T("a:clrMap");
							clrMap->fromPPTY(pReader);
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_e);				
			}

		public:
			ClrScheme				clrScheme;
			nullable<Logic::ClrMap> clrMap;
		protected:
			virtual void FillParentPointersForChilds()
			{
				clrScheme.SetParentPointer(this);
				if(clrMap.is_init())
					clrMap->SetParentPointer(this);
			}
		};
	} // namespace nsTheme
} // namespace PPTX

#endif // PPTX_THEME_EXTRACLRSCHEME_INCLUDE_H