#pragma once
#ifndef PPTX_LOGIC_CXN_INCLUDE_H_
#define PPTX_LOGIC_CXN_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class Cxn : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Cxn)
			
			Cxn& operator=(const Cxn& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				x	= oSrc.x;
				y	= oSrc.y;
				ang	= oSrc.ang;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				ang = node.GetAttribute(_T("ang"));

				XmlUtils::CXmlNode oPos;
				if (node.GetNode(_T("a:pos"), oPos))
				{
					x = oPos.GetAttribute(_T("x"));
					y = oPos.GetAttribute(_T("y"));
				}
			}

			virtual CString toXML() const
			{
				return _T("<a:cxn ang=\"") + ang + _T("\"><a:pos x=\"") + x + _T("\" y=\"") + y + _T("\" /></a:cxn>");
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:cxn"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("ang"), ang);
				pWriter->EndAttributes();

				pWriter->StartNode(_T("a:pos"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("x"), x);
				pWriter->WriteAttribute(_T("y"), y);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:pos"));

				pWriter->EndNode(_T("a:cxn"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, x);
				pWriter->WriteString1(1, y);
				pWriter->WriteString1(2, ang);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

		public:
			CString x;
			CString y;
			CString ang;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			CString GetODString()const
			{
				return _T("<cxn ang=\"") + ang + _T("\"><pos x=\"") + x + _T("\" y=\"") + y + _T("\" /></cxn>");
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CXN_INCLUDE_H_