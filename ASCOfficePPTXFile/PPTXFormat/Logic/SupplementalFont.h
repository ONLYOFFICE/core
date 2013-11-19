#pragma once
#ifndef PPTX_LOGIC_SUPPLEMENTALFONT_INCLUDE_H_
#define PPTX_LOGIC_SUPPLEMENTALFONT_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class SupplementalFont : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SupplementalFont)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name = node.GetName();

				node.ReadAttributeBase(L"script", script);
				node.ReadAttributeBase(L"typeface", typeface);
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("script"), script);
				oAttr.Write(_T("typeface"), typeface);

				return XmlUtils::CreateNode(m_name, oAttr);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(m_name);

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("script"), script);
				pWriter->WriteAttribute(_T("typeface"), typeface);
				pWriter->EndAttributes();

				pWriter->EndNode(m_name);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, script);
				pWriter->WriteString1(1, typeface);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attr

				while (true)
				{
					BYTE _at = pReader->GetUChar();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
						case 0:
						{
							script = pReader->GetString2();
							break;
						}
						case 1:
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

		public:
			CString script;
			CString typeface;
		//private:
		public:
			CString m_name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SUPPLEMENTALFONT_INCLUDE_H_