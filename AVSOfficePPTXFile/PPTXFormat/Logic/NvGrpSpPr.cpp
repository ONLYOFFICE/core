#include "NvGrpSpPr.h"

namespace PPTX
{
	namespace Logic
	{

		NvGrpSpPr::NvGrpSpPr()
		{
		}


		NvGrpSpPr::~NvGrpSpPr()
		{
		}
	

		NvGrpSpPr::NvGrpSpPr(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}


		const NvGrpSpPr& NvGrpSpPr::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}


		void NvGrpSpPr::fromXML(XmlUtils::CXmlNode& node)
		{
			cNvPr		= node.ReadNode(_T("p:cNvPr"));
			cNvGrpSpPr	= node.ReadNode(_T("p:cNvGrpSpPr"));
			nvPr		= node.ReadNode(_T("p:nvPr"));

			FillParentPointersForChilds();
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