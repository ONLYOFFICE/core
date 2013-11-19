#pragma once
#ifndef PPTX_LOGIC_SLIDE_FONTREF_INCLUDE_H_
#define PPTX_LOGIC_SLIDE_FONTREF_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/FontStyleIndex.h"
#include "UniColor.h"

namespace PPTX
{
	namespace Logic
	{
		class FontRef : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(FontRef)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name = node.GetName();

				node.ReadAttributeBase(L"idx", idx);
				Color.GetColorFrom(node);

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("idx"), idx);

				return XmlUtils::CreateNode(m_name, oAttr, Color.toXML());
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(m_name);

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("idx"), idx);
				pWriter->EndAttributes();

				Color.toXmlWriter(pWriter);

				pWriter->EndNode(m_name);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, idx);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, Color);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetULong() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					if (0 == _at)
					{
						idx = new Limit::FontStyleIndex();
						idx->SetBYTECode(pReader->GetUChar());
					}
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
							Color.fromPPTY(pReader);
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

		public:
			UniColor Color;
			// Attributes
			nullable_limit<Limit::FontStyleIndex> idx;
		//private:
		public:
			CString m_name;
		protected:
			virtual void FillParentPointersForChilds()
			{
				Color.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SLIDE_FONTREF_INCLUDE_H_