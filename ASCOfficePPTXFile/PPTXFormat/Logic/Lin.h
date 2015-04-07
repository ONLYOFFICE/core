#pragma once
#ifndef PPTX_LOGIC_LIN_INCLUDE_H_
#define PPTX_LOGIC_LIN_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class Lin : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Lin)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"ang", ang);
				node.ReadAttributeBase(L"scaled", scaled);

				Normalize();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("ang"), ang);
				oAttr.Write(_T("scaled"), scaled);

				return XmlUtils::CreateNode(_T("a:lin"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, ang);
				pWriter->WriteBool2(1, scaled);
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

					if (0 == _at)
						ang = pReader->GetLong();
					else if (1 == _at)
						scaled = pReader->GetBool();
				}

				pReader->Seek(_end_rec);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:lin"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("ang"), ang);
				pWriter->WriteAttribute(_T("scaled"), scaled);
				pWriter->EndAttributes();

				pWriter->EndNode(_T("a:lin"));
			}
			
		public:
			nullable_int		ang;
			nullable_bool		scaled;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				ang.normalize(0, 21600000);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_LIN_INCLUDE_H_