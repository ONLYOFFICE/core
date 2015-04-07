#pragma once
#ifndef PPTX_LOGIC_TEXTSPACING_INCLUDE_H_
#define PPTX_LOGIC_TEXTSPACING_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class TextSpacing : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TextSpacing)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name = node.GetName();

				node.ReadNode(_T("a:spcPct")).ReadAttributeBase(L"val", spcPct);
				node.ReadNode(_T("a:spcPts")).ReadAttributeBase(L"val", spcPts);

				Normalize();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				
				if (spcPct.is_init())
				{
					oValue.m_strValue.Format(_T("<a:spcPct val=\"%d\" />"), *spcPct);
				}
				else
				{
					oValue.m_strValue.Format(_T("<a:spcPct val=\"%d\" />"), spcPts.get_value_or(0));
				}

				return XmlUtils::CreateNode(m_name, oValue);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(m_name);				
				pWriter->EndAttributes();

				if (spcPct.is_init())
				{
					pWriter->StartNode(_T("a:spcPct"));
					pWriter->StartAttributes();
					pWriter->WriteAttribute(_T("val"), *spcPct);
					pWriter->EndAttributes();
					pWriter->EndNode(_T("a:spcPct"));					
				}
				else
				{
					pWriter->StartNode(_T("a:spcPts"));
					pWriter->StartAttributes();
					pWriter->WriteAttribute(_T("val"), spcPts.get_value_or(0));
					pWriter->EndAttributes();
					pWriter->EndNode(_T("a:spcPts"));
				}

				pWriter->EndNode(m_name);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, spcPct);
				pWriter->WriteInt2(1, spcPts);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
						case 0:
						{
							spcPct = pReader->GetLong();					
							break;
						}
						case 1:
						{
							spcPts = pReader->GetLong();
							break;
						}						
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

		public:
			nullable_int spcPct;
			nullable_int spcPts;

			int GetVal()const
			{
				if(spcPts.is_init())
					return -spcPts.get();
				else
					return spcPct.get_value_or(100000)/1000;
			}
		//private:
		public:
			CString m_name;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				spcPts.normalize(0, 158400);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TEXTSPACING_INCLUDE_H_