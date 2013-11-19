#pragma once
#ifndef PPTX_LOGIC_ARCTO_INCLUDE_H_
#define PPTX_LOGIC_ARCTO_INCLUDE_H_

#include "PathBase.h"

namespace PPTX
{
	namespace Logic
	{
		class ArcTo : public PathBase
		{
		public:
			PPTX_LOGIC_BASE(ArcTo)

			ArcTo& operator=(const ArcTo& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				wR		= oSrc.wR;
				hR		= oSrc.hR;
				stAng	= oSrc.stAng;
				swAng	= oSrc.swAng;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				wR		= node.GetAttribute(_T("wR"));
				hR		= node.GetAttribute(_T("hR"));
				stAng	= node.GetAttribute(_T("stAng"));
				swAng	= node.GetAttribute(_T("swAng"));
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("wR"), wR);
				oAttr.Write(_T("hR"), hR);
				oAttr.Write(_T("stAng"), stAng);
				oAttr.Write(_T("swAng"), swAng);

				return XmlUtils::CreateNode(_T("a:arcTo"), oAttr);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:arcTo"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("wR"), wR);
				pWriter->WriteAttribute(_T("hR"), hR);
				pWriter->WriteAttribute(_T("stAng"), stAng);
				pWriter->WriteAttribute(_T("swAng"), swAng);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:arcTo"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(GEOMETRY_TYPE_PATH_ARCTO);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, wR);
				pWriter->WriteString1(1, hR);
				pWriter->WriteString1(2, stAng);
				pWriter->WriteString1(3, swAng);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}

		public:
			CString wR;
			CString hR;
			CString stAng;
			CString swAng;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual CString GetODString()const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("wR"), wR);
				oAttr.Write(_T("hR"), hR);
				oAttr.Write(_T("stAng"), stAng);
				oAttr.Write(_T("swAng"), swAng);

				return XmlUtils::CreateNode(_T("arcTo"), oAttr);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ARCTO_INCLUDE_H_