#pragma once
#ifndef PPTX_LOGIC_BUCHAR_INCLUDE_H_
#define PPTX_LOGIC_BUCHAR_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class BuChar : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BuChar)

			BuChar& operator=(const BuChar& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Char = oSrc.Char;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Char = node.GetAttribute(_T("char"));
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("char"), Char);
				return XmlUtils::CreateNode(_T("a:buChar"), oAttr);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:buChar"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("char"), Char);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:buChar"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(BULLET_TYPE_BULLET_CHAR);
				
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);				
				pWriter->WriteString1(0, Char);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();


                if (pWriter->m_pCommon->m_pNativePicker->m_bIsEmbeddedFonts)
					pWriter->m_pCommon->m_pNativePicker->m_oEmbeddedFonts.CheckString(Char);

			}

		public:
			CString Char;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUCHAR_INCLUDE_H
