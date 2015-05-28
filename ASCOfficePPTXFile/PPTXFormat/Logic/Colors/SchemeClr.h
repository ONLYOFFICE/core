#pragma once
#ifndef PPTX_LOGIC_SCHEMECLR_INCLUDE_H_
#define PPTX_LOGIC_SCHEMECLR_INCLUDE_H_

#include "./../../Limit/SchemeClrVal.h"
#include "./../../Theme/ClrScheme.h"
#include "ColorBase.h"

namespace PPTX
{
	namespace Logic
	{
		class SchemeClr : public ColorBase
		{
		public:
			PPTX_LOGIC_BASE(SchemeClr)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual CString toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				CString sNodeNamespace;
				CString sAttrNamespace;
				if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
				{
					sNodeNamespace = _T("w14:");
					sAttrNamespace = sNodeNamespace;
				}
				else
					sNodeNamespace = _T("a:");
				pWriter->StartNode(sNodeNamespace + _T("schemeClr"));
						
				pWriter->StartAttributes();
				pWriter->WriteAttribute(sAttrNamespace + _T("val"), val.get());
				pWriter->EndAttributes();

				size_t nCount = Modifiers.size();
				for (size_t i = 0; i < nCount; ++i)
					Modifiers[i].toXmlWriter(pWriter);
				
				pWriter->EndNode(sNodeNamespace + _T("schemeClr"));
			}

			virtual DWORD GetRGBA(DWORD RGBA) const;
			virtual DWORD GetARGB(DWORD ARGB) const;
			virtual DWORD GetBGRA(DWORD BGRA) const;
			virtual DWORD GetABGR(DWORD ABGR) const;

			virtual DWORD GetRGBColor(NSCommon::smart_ptr<PPTX::WrapperFile>& _oTheme, NSCommon::smart_ptr<PPTX::WrapperWritingElement>& _oClrMap, DWORD ARGB = 0);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(COLOR_TYPE_SCHEME);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit1(0, val);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				ULONG len = (ULONG)Modifiers.size();
				if (len != 0)
				{
					pWriter->StartRecord(0);
					pWriter->WriteULONG(len);

					for (ULONG i = 0; i < len; ++i)
					{
						pWriter->WriteRecord1(1, Modifiers[i]);
					}

					pWriter->EndRecord();
				}

				pWriter->EndRecord();
			}

		public:
			Limit::SchemeClrVal val;
		private:
			void FillRGBFromVal(DWORD rgb);

		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SCHEMECLR_INCLUDE_H