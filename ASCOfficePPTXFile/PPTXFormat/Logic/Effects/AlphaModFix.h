#pragma once
#ifndef PPTX_LOGIC_ALPHAMODFIX_INCLUDE_H_
#define PPTX_LOGIC_ALPHAMODFIX_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class AlphaModFix : public WrapperWritingElement
		{
		public:

			PPTX_LOGIC_BASE(AlphaModFix)

			AlphaModFix& operator=(const AlphaModFix& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				amt = oSrc.amt;
				return *this;
			}
			
		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"amt", amt);
				Normalize();
			}

			virtual CString toXML() const
			{
				if (amt.IsInit())
				{
					CString strRes = _T("");
					strRes.Format(_T("<a:alphaModFix amt=\"%d\" />"), *amt);
					return strRes;
				}

				return _T("<a:alphaModFix/>");
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (amt.is_init())
				{
					pWriter->StartNode(_T("a:alphaModFix"));
					pWriter->StartAttributes();
					pWriter->WriteAttribute(_T("amt"), amt);
					pWriter->EndAttributes();
					pWriter->EndNode(_T("a:alphaModFix"));
				}
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_ALPHAMODFIX);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, amt);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}
		public:
			nullable_int amt;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				amt.normalize_positive();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ALPHAMODFIX_INCLUDE_H_