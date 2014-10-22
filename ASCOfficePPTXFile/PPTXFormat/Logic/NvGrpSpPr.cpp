#include "NvGrpSpPr.h"

namespace PPTX
{
	namespace Logic
	{

		void NvGrpSpPr::fromXML(XmlUtils::CXmlNode& node)
		{
			cNvPr		= node.ReadNode(_T("p:cNvPr"));
			cNvGrpSpPr	= node.ReadNode(_T("p:cNvGrpSpPr"));
			nvPr		= node.ReadNode(_T("p:nvPr"));

			FillParentPointersForChilds();
   /*
            XmlUtils::CXmlNodes oNodes;
            if (node.GetNodes(_T("*"), oNodes))
            {
                int nCount = oNodes.GetCount();
                for (int i = 0; i < nCount; ++i)
                {
                    XmlUtils::CXmlNode oNode;
                    oNodes.GetAt(i, oNode);

                    CString strName = XmlUtils::GetNameNoNS(oNode.GetName());

                    if (_T("cNvPr") == strName)
                        cNvPr = oNode;
                    else if (_T("cNvGrpSpPr") == strName)
                        cNvGrpSpPr = oNode;
                    else if (_T("nvPr") == strName)
                        nvPr = oNode;
                }
            }

            FillParentPointersForChilds();
      from header (old)*/
		}


		CString NvGrpSpPr::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.Write(cNvPr);
			oValue.Write(cNvGrpSpPr);
			oValue.Write(nvPr);

			return XmlUtils::CreateNode(_T("p:nvGrpSpPr"), oValue);
		}

		void NvGrpSpPr::FillParentPointersForChilds()
		{
			cNvPr.SetParentPointer(this);
			cNvGrpSpPr.SetParentPointer(this);
			nvPr.SetParentPointer(this);
		}

	} // namespace Logic
} // namespace PPTX
