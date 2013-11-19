#pragma once
#ifndef PPTX_LOGIC_BUCLR_INCLUDE_H_
#define PPTX_LOGIC_BUCLR_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{
		class BuClr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BuClr)

			BuClr& operator=(const BuClr& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Color = oSrc.Color;
				return *this;
			}

			virtual DWORD GetRGBA()const{return Color.GetRGBA();};
			virtual DWORD GetARGB()const{return Color.GetARGB();};
			virtual DWORD GetBGRA()const{return Color.GetBGRA();};
			virtual DWORD GetABGR()const{return Color.GetABGR();};

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Color.GetColorFrom(node);
			}
			virtual CString toXML() const
			{
				return XmlUtils::CreateNode(_T("a:buClr"), Color.toXML());
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:buClr"));
				pWriter->EndAttributes();
				Color.toXmlWriter(pWriter);
				pWriter->EndNode(_T("a:buClr"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(BULLET_TYPE_COLOR_CLR);
				pWriter->WriteRecord1(0, Color);
				pWriter->EndRecord();
			}

		public:
			UniColor Color;
		protected:
			virtual void FillParentPointersForChilds()
			{
				Color.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUCLR_INCLUDE_H