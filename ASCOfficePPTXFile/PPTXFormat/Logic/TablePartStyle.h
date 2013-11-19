#pragma once
#ifndef PPTX_LOGIC_TABLEPARTSTYLE_INCLUDE_H_
#define PPTX_LOGIC_TABLEPARTSTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "TcStyle.h"
#include "TcTxStyle.h"
#include "UniFill.h"
#include "UniColor.h"
#include "../Theme.h"

namespace PPTX
{
	namespace Logic
	{
		class TablePartStyle : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TablePartStyle)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name = XmlUtils::GetNameNoNS(node.GetName());

				tcTxStyle	= node.ReadNode(_T("a:tcTxStyle"));
				tcStyle		= node.ReadNode(_T("a:tcStyle"));

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(tcTxStyle);
				oValue.WriteNullable(tcStyle);

				return XmlUtils::CreateNode(_T("a:") + m_name, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(m_name);
				pWriter->EndAttributes();

				pWriter->Write(tcTxStyle);
				pWriter->Write(tcStyle);

				pWriter->EndNode(m_name);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord2(0, tcTxStyle);
				pWriter->WriteRecord2(1, tcStyle);
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
							tcTxStyle = new TcTxStyle();
							tcTxStyle->fromPPTY(pReader);							
							break;
						}
						case 1:
						{
							tcStyle = new TcStyle();
							tcStyle->fromPPTY(pReader);
							break;
						}
						default:
							break;
					}
				}				

				pReader->Seek(_end_rec);
			}
		public:
			nullable<TcTxStyle> tcTxStyle;
			nullable<TcStyle>	tcStyle;

			const UniFill GetFillStyle(UniColor& Color)const
			{
				UniFill result;
				result.SetParentFilePointer(parentFile);
				UniColor resColor;
				resColor.SetParentFilePointer(parentFile);
				Color = resColor;
				if(!tcStyle.IsInit())
					return result;
				if(tcStyle->fill.IsInit())
					return tcStyle->fill->Fill;
				if(tcStyle->fillRef.IsInit())
				{
					m_Theme->GetFillStyle(tcStyle->fillRef->idx.get_value_or(0), result);
					Color = tcStyle->fillRef->Color;
					return result;
				}
				return result;
			}

		//private:
		public:
			CString m_name;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(tcTxStyle.IsInit())
					tcTxStyle->SetParentPointer(this);
				if(tcStyle.IsInit())
					tcStyle->SetParentPointer(this);
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

#endif // PPTX_LOGIC_TABLEPARTSTYLE_INCLUDE_H