#pragma once
#ifndef PPTX_LOGIC_TABLEBGSTYLE_INCLUDE_H_
#define PPTX_LOGIC_TABLEBGSTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "UniFill.h"
#include "FillStyle.h"
#include "StyleRef.h"
#include "../Theme.h"

namespace PPTX
{
	namespace Logic
	{
		class TableBgStyle : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TableBgStyle)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				fill	= node.ReadNode(_T("a:fill"));
				fillRef = node.ReadNodeNoNS(_T("fillRef"));

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(fill);
				oValue.WriteNullable(fillRef);

				return XmlUtils::CreateNode(_T("a:tblBg"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:tblBg"));
				pWriter->EndAttributes();

				pWriter->Write(fill);
				pWriter->Write(fillRef);

				pWriter->EndNode(_T("a:tblBg"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord2(0, fill);
				pWriter->WriteRecord2(1, fillRef);
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
							fill = new FillStyle();
							fill->fromPPTY(pReader);							
							break;
						}
						case 1:
						{
							fillRef = new StyleRef();
							fillRef->fromPPTY(pReader);
							fillRef->m_name = _T("a:fillRef");
							break;
						}
						default:
							break;
					}
				}				

				pReader->Seek(_end_rec);
			}

		public:
			// Childs
			//effect (Effect)  §20.1.4.2.7 
			//effectRef (Effect Reference)  §20.1.4.2.8 
			nullable<FillStyle>	fill;
			nullable<StyleRef>	fillRef;

			const UniFill GetFillStyle(UniColor& Color)const
			{
				UniFill result;
				result.SetParentFilePointer(parentFile);
				UniColor resColor;
				resColor.SetParentFilePointer(parentFile);
				Color = resColor;

				if(fill.IsInit())
					return fill->Fill;
				if(fillRef.IsInit())
				{
					m_Theme->GetFillStyle(fillRef->idx.get_value_or(0), result);
					Color = fillRef->Color;
					return result;
				}
				return result;
			}
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(fill.IsInit())
					fill->SetParentPointer(this);
				if(fillRef.IsInit())
					fillRef->SetParentPointer(this);
			}

		public:
			void SetTheme(const smart_ptr<PPTX::Theme> theme)
			{
				m_Theme = theme;
			}
		private:
			smart_ptr<PPTX::Theme> m_Theme;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLEBGSTYLE_INCLUDE_H