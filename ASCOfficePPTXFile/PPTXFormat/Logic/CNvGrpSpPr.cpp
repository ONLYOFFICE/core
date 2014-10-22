#include "CNvGrpSpPr.h"

namespace PPTX
{
	namespace Logic
	{

         CNvGrpSpPr& CNvGrpSpPr::operator=(const CNvGrpSpPr& oSrc)
        {
            parentFile		= oSrc.parentFile;
            parentElement	= oSrc.parentElement;

            noChangeAspect	= oSrc.noChangeAspect;
            noGrp			= oSrc.noGrp;
            noMove			= oSrc.noMove;
            noResize		= oSrc.noResize;
            noRot			= oSrc.noRot;
            noSelect		= oSrc.noSelect;
            noUngrp			= oSrc.noUngrp;

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
