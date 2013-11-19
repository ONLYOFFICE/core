#pragma once
#ifndef PPTX_LOGIC_CLRCHANGE_INCLUDE_H_
#define PPTX_LOGIC_CLRCHANGE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class ClrChange : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(ClrChange)

			ClrChange& operator=(const ClrChange& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				ClrFrom	= oSrc.ClrFrom;
				ClrTo	= oSrc.ClrTo;
				useA	= oSrc.useA;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				ClrFrom.GetColorFrom(node.ReadNode(_T("a:clrFrom")));
				ClrTo.GetColorFrom(node.ReadNode(_T("a:clrTo")));
				node.ReadAttributeBase(L"useA", useA);
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("useA"), useA);

				XmlUtils::CNodeValue oValue;
				oValue.Write(_T("a:clrFrom"), ClrFrom);
				oValue.Write(_T("a:clrTo"), ClrTo);

				return XmlUtils::CreateNode(_T("a:clrChange"), oAttr, oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_CLRCHANGE);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, useA);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, ClrFrom);
				pWriter->WriteRecord1(1, ClrTo);

				pWriter->EndRecord();
			}

		public:
			UniColor		ClrFrom;
			UniColor		ClrTo;
			nullable_bool	useA;
		protected:
			virtual void FillParentPointersForChilds()
			{
				ClrFrom.SetParentPointer(this);
				ClrTo.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CLRCHANGE_INCLUDE_H_