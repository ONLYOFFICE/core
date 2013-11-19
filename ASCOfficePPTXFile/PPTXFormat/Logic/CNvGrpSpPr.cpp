#include "CNvGrpSpPr.h"

namespace PPTX
{
	namespace Logic
	{

		CNvGrpSpPr::CNvGrpSpPr()
		{
		}


		CNvGrpSpPr::~CNvGrpSpPr()
		{
		}
	

		CNvGrpSpPr::CNvGrpSpPr(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}


		const CNvGrpSpPr& CNvGrpSpPr::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}


		void CNvGrpSpPr::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNode oNode;
			if (node.GetNode(_T("a:grpSpLocks"), oNode))
			{
				oNode.ReadAttributeBase(L"noChangeAspect", noChangeAspect);
				oNode.ReadAttributeBase(L"noGrp", noGrp);
				oNode.ReadAttributeBase(L"noMove", noMove);
				oNode.ReadAttributeBase(L"noResize", noResize);
				oNode.ReadAttributeBase(L"noRot", noRot);
				oNode.ReadAttributeBase(L"noSelect", noSelect);
				oNode.ReadAttributeBase(L"noUngrp", noUngrp);
			}
		}


		CString CNvGrpSpPr::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("noChangeAspect"), noChangeAspect);
			oAttr.Write(_T("noGrp"), noGrp);
			oAttr.Write(_T("noMove"), noMove);
			oAttr.Write(_T("noResize"), noResize);
			oAttr.Write(_T("noRot"), noRot);
			oAttr.Write(_T("noSelect"), noSelect);
			oAttr.Write(_T("noUngrp"), noUngrp);

			if (_T("") == oAttr.m_strValue)
				return _T("<p:cNvGrpSpPr/>");

			return _T("<p:cNvGrpSpPr>") + XmlUtils::CreateNode(_T("a:grpSpLocks"), oAttr) + _T("</p:cNvGrpSpPr>");
		}

	} // namespace Logic
} // namespace PPTX