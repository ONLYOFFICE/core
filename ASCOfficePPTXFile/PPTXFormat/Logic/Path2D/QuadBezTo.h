#pragma once
#ifndef PPTX_LOGIC_QUADBEZTO_INCLUDE_H_
#define PPTX_LOGIC_QUADBEZTO_INCLUDE_H_

#include "PathBase.h"

namespace PPTX
{
	namespace Logic
	{
		class QuadBezTo : public PathBase
		{
		public:
			PPTX_LOGIC_BASE(QuadBezTo)

			QuadBezTo& operator=(const QuadBezTo& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				x1 = oSrc.x1;
				y1 = oSrc.y1;
				x2 = oSrc.x2;
				y2 = oSrc.y2;
				*this;
			}
			
		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNodes oNodes;

				if (node.GetNodes(_T("a:pt"), oNodes))
				{
					int count = oNodes.GetCount();
					if(count >= 2)
					{
						XmlUtils::CXmlNode oNode;

						oNodes.GetAt(0, oNode);
						x1 = oNode.GetAttribute(_T("x"));
						y1 = oNode.GetAttribute(_T("y"));
						oNodes.GetAt(1, oNode);
						x2 = oNode.GetAttribute(_T("x"));
						y2 = oNode.GetAttribute(_T("y"));
					}
				}
			}

			virtual CString toXML() const
			{
				CString str1 = _T("");
				str1.Format(_T("<a:pt x=\"%s\" y=\"%s\" />"), x1, y1);
				CString str2 = _T("");
				str2.Format(_T("<a:pt x=\"%s\" y=\"%s\" />"), x2, y2);

				return _T("<a:quadBezTo>") + str1 + str2 + _T("</a:quadBezTo>");
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:quadBezTo"));
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
				
				pWriter->EndNode(_T("a:quadBezTo"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(GEOMETRY_TYPE_PATH_QUADBEZTO);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, x1);
				pWriter->WriteString1(1, y1);
				pWriter->WriteString1(2, x2);
				pWriter->WriteString1(3, y2);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}
		public:
			CString x1;
			CString y1;
			CString x2;
			CString y2;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			
			virtual CString GetODString()const
			{
				CString str1 = _T("");
				str1.Format(_T("<pt x=\"%s\" y=\"%s\" />"), x1, y1);
				CString str2 = _T("");
				str2.Format(_T("<pt x=\"%s\" y=\"%s\" />"), x2, y2);

				return _T("<quadBezTo>") + str1 + str2 + _T("</quadBezTo>");
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_QUADBEZTO_INCLUDE_H_