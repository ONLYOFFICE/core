#pragma once
#ifndef PPTX_LOGIC_HF_INCLUDE_H_
#define PPTX_LOGIC_HF_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class HF : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(HF)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"dt", dt);
				node.ReadAttributeBase(L"ftr", ftr);
				node.ReadAttributeBase(L"hdr", hdr);
				node.ReadAttributeBase(L"sldNum", sldNum);
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("dt"), dt);
				oAttr.Write(_T("ftr"), ftr);
				oAttr.Write(_T("hdr"), hdr);
				oAttr.Write(_T("sldNum"), sldNum);

				return XmlUtils::CreateNode(_T("p:hf"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, dt);
				pWriter->WriteBool2(1, ftr);
				pWriter->WriteBool2(2, hdr);
				pWriter->WriteBool2(3, sldNum);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:hf"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("dt"), dt.get_value_or(false));
				pWriter->WriteAttribute(_T("ftr"), ftr.get_value_or(false));
				pWriter->WriteAttribute(_T("hdr"), hdr.get_value_or(false));
				pWriter->WriteAttribute(_T("sldNum"), sldNum.get_value_or(false));
				pWriter->EndAttributes();

				pWriter->EndNode(_T("p:hf"));
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _e = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // attribute start
				while (true)
				{
					BYTE _at = pReader->GetUChar();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					if (0 == _at)
						dt = pReader->GetBool();
					else if (1 == _at)
						ftr = pReader->GetBool();
					else if (2 == _at)
						hdr = pReader->GetBool();
					else if (3 == _at)
						sldNum = pReader->GetBool();
				}

				pReader->Seek(_e);
			}

		public:
			nullable_bool		dt;
			nullable_bool		ftr;
			nullable_bool		hdr;
			nullable_bool		sldNum;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_HF_INCLUDE_H_