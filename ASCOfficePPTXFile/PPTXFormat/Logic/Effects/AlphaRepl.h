#pragma once
#ifndef PPTX_LOGIC_ALPHAREPL_INCLUDE_H_
#define PPTX_LOGIC_ALPHAREPL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class AlphaRepl : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AlphaRepl)

			AlphaRepl& operator=(const AlphaRepl& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				a = oSrc.a;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"a", a);
				Normalize();
			}

			virtual CString toXML() const
			{
				if (!a.IsInit())
					return _T("<a:alphaRepl/>");
				
				CString str = _T("");
				str.Format(_T("<a:alphaRepl a=\"%d\" />"), *a);
				return str;
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_ALPHAREPL);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, a);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}

		public:
			nullable_int a;
		protected:
			virtual void FillParentPointersForChilds(){};
			AVSINLINE void Normalize()
			{
				a.normalize_positive();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ALPHAREPL_INCLUDE_H_