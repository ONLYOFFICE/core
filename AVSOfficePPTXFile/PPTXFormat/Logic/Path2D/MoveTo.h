#pragma once
#ifndef PPTX_LOGIC_MOVETO_INCLUDE_H_
#define PPTX_LOGIC_MOVETO_INCLUDE_H_

#include "PathBase.h"

namespace PPTX
{
	namespace Logic
	{

		class MoveTo : public PathBase
		{
		public:
			PPTX_LOGIC_BASE(MoveTo)

			MoveTo& operator=(const MoveTo& oSrc)
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
				str.Format(_T("<a:pt x=\"%s\" y=\"%s\" />"), x, y);
				return _T("<a:moveTo>") + str + _T("</a:moveTo>"); 
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:moveTo"));
				pWriter->EndAttributes();

				pWriter->StartNode(_T("a:pt"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("x"), x);
				pWriter->WriteAttribute(_T("y"), y);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:pt"));
				
				pWriter->EndNode(_T("a:moveTo"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(GEOMETRY_TYPE_PATH_MOVETO);

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
				str.Format(_T("<pt x=\"%s\" y=\"%s\" />"), x, y);
				return _T("<moveTo>") + str + _T("</moveTo>"); 
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_MOVETO_INCLUDE_H_