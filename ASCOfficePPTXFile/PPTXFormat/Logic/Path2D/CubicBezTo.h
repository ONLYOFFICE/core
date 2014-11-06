#pragma once
#ifndef PPTX_LOGIC_CUBICBEZTO_INCLUDE_H_
#define PPTX_LOGIC_CUBICBEZTO_INCLUDE_H_

#include "PathBase.h"

namespace PPTX
{
	namespace Logic
	{

		class CubicBezTo : public PathBase
		{
		public:
			PPTX_LOGIC_BASE(CubicBezTo)

			CubicBezTo& operator=(const CubicBezTo& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				x1 = oSrc.x1;
				y1 = oSrc.y1;
				x2 = oSrc.x2;
				y2 = oSrc.y2;
				x3 = oSrc.x3;
				y3 = oSrc.y3;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNodes oNodes;

				if (node.GetNodes(_T("a:pt"), oNodes))
				{
					int count = oNodes.GetCount();
					XmlUtils::CXmlNode oNode;

					oNodes.GetAt(0, oNode);
					x1 = oNode.GetAttribute(_T("x"));
					y1 = oNode.GetAttribute(_T("y"));
					oNodes.GetAt(1, oNode);
					x2 = oNode.GetAttribute(_T("x"));
					y2 = oNode.GetAttribute(_T("y"));
					oNodes.GetAt(2, oNode);
					x3 = oNode.GetAttribute(_T("x"));
					y3 = oNode.GetAttribute(_T("y"));
				}
			}

			virtual CString toXML() const
			{
				CString str1 = _T("");
				str1.Format(_T("<a:pt x=\"%ls\" y=\"%ls\" />"), x1, y1);
				CString str2 = _T("");
				str2.Format(_T("<a:pt x=\"%ls\" y=\"%ls\" />"), x2, y2);
				CString str3 = _T("");
				str3.Format(_T("<a:pt x=\"%ls\" y=\"%ls\" />"), x3, y3);

				return _T("<a:cubicBezTo>") + str1 + str2 + str3 + _T("</a:cubicBezTo>");
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:cubicBezTo"));
				pWriter->EndAttributes();

				pWriter->StartNode(_T("a:pt"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("x"), x1);
				pWriter->WriteAttribute(_T("y"), y1);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:pt"));

				pWriter->StartNode(_T("a:pt"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("x"), x2);
				pWriter->WriteAttribute(_T("y"), y2);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:pt"));

				pWriter->StartNode(_T("a:pt"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("x"), x3);
				pWriter->WriteAttribute(_T("y"), y3);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:pt"));
				
				pWriter->EndNode(_T("a:cubicBezTo"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(GEOMETRY_TYPE_PATH_CUBICBEZTO);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, x1);
				pWriter->WriteString1(1, y1);
				pWriter->WriteString1(2, x2);
				pWriter->WriteString1(3, y2);
				pWriter->WriteString1(4, x3);
				pWriter->WriteString1(5, y3);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}

		public:
			CString x1;
			CString y1;
			CString x2;
			CString y2;
			CString x3;
			CString y3;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			
			virtual CString GetODString()const
			{
				CString str1 = _T("");
				str1.Format(_T("<pt x=\"%ls\" y=\"%ls\" />"), x1, y1);
				CString str2 = _T("");
				str2.Format(_T("<pt x=\"%ls\" y=\"%ls\" />"), x2, y2);
				CString str3 = _T("");
				str3.Format(_T("<pt x=\"%ls\" y=\"%ls\" />"), x3, y3);

				return _T("<cubicBezTo>") + str1 + str2 + str3 + _T("</cubicBezTo>");
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CUBICBEZTO_INCLUDE_H_