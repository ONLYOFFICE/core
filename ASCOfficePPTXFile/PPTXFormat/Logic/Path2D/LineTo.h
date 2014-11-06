#pragma once
#ifndef PPTX_LOGIC_LINETO_INCLUDE_H_
#define PPTX_LOGIC_LINETO_INCLUDE_H_

#include "PathBase.h"

namespace PPTX
{
	namespace Logic
	{

		class LineTo : public PathBase
		{
		public:
			PPTX_LOGIC_BASE(LineTo)

			LineTo& operator=(const LineTo& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				x = oSrc.x;
				y = oSrc.y;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNode oNode;
				if (node.GetNode(_T("a:pt"), oNode))
				{
					x = oNode.GetAttribute(_T("x"));
					y = oNode.GetAttribute(_T("y"));
				}
			}

			virtual CString toXML() const
			{
				CString str;
				str.Format(_T("<a:pt x=\"%ls\" y=\"%ls\" />"), x, y);
				return _T("<a:lnTo>") + str + _T("</a:lnTo>");
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:lnTo"));
				pWriter->EndAttributes();

				pWriter->StartNode(_T("a:pt"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("x"), x);
				pWriter->WriteAttribute(_T("y"), y);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:pt"));
				
				pWriter->EndNode(_T("a:lnTo"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(GEOMETRY_TYPE_PATH_LINETO);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, x);
				pWriter->WriteString1(1, y);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}

		public:
			CString x;
			CString y;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			
			virtual CString GetODString()const
			{
				CString str;
				str.Format(_T("<pt x=\"%ls\" y=\"%ls\" />"), x, y);
				return _T("<lnTo>") + str + _T("</lnTo>");
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_LINETO_INCLUDE_H_