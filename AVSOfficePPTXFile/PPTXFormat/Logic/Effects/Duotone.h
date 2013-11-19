#pragma once
#ifndef PPTX_LOGIC_DUOTONE_INCLUDE_H_
#define PPTX_LOGIC_DUOTONE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{
		class Duotone : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Duotone)

			Duotone& operator=(const Duotone& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Colors.Copy(oSrc.Colors);
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Colors.RemoveAll();
				node.LoadArray(_T("*"), Colors);
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteArray(Colors);

				return XmlUtils::CreateNode(_T("a:duotone"), oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_DUOTONE);

				ULONG len = (ULONG)Colors.GetCount();
				pWriter->WriteULONG(len);
				
				for (ULONG i = 0; i < len; ++i)
				{
					pWriter->WriteRecord1(0, Colors[i]);
				}

				pWriter->EndRecord();
			}

		public:
			CAtlArray<UniColor> Colors;
		protected:
			virtual void FillParentPointersForChilds()
			{
				size_t count = Colors.GetCount();
				for (size_t i = 0; i < count; ++i)
					Colors[i].SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_DUOTONE_INCLUDE_H_