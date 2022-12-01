/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			XmlUtils::CXmlNode oNode;
			if (node.GetNode(_T("a:grpSpLocks"), oNode))
			{
                XmlMacroReadAttributeBase(oNode, L"noChangeAspect", noChangeAspect);
                XmlMacroReadAttributeBase(oNode, L"noGrp", noGrp);
                XmlMacroReadAttributeBase(oNode, L"noMove", noMove);
                XmlMacroReadAttributeBase(oNode, L"noResize", noResize);
                XmlMacroReadAttributeBase(oNode, L"noRot", noRot);
                XmlMacroReadAttributeBase(oNode, L"noSelect", noSelect);
                XmlMacroReadAttributeBase(oNode, L"noUngrp", noUngrp);
			}
		}
		void CNvGrpSpPr::ReadAttributesLocks(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("noChangeAspect"),	noChangeAspect)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noGrp"),	noGrp)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noMove"), noMove)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noResize"), noResize)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noRot"), noRot)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noSelect"), noSelect)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("noUngrp"), noUngrp)
			WritingElement_ReadAttributes_End( oReader )
		}
		void CNvGrpSpPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());
			
			if ( oReader.IsEmptyNode() )
				return;
					
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = oReader.GetName();

				if (strName == L"a:grpSpLocks")
				{
					ReadAttributesLocks(oReader);
				}
			}
		}
		std::wstring CNvGrpSpPr::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("noChangeAspect"), noChangeAspect);
			oAttr.Write(_T("noGrp"), noGrp);
			oAttr.Write(_T("noMove"), noMove);
			oAttr.Write(_T("noResize"), noResize);
			oAttr.Write(_T("noRot"), noRot);
			oAttr.Write(_T("noSelect"), noSelect);
			oAttr.Write(_T("noUngrp"), noUngrp);

			return XmlUtils::CreateNode(m_namespace + L":cNvGrpSpPr", XmlUtils::CreateNode(_T("a:grpSpLocks"), oAttr));
		}

	} // namespace Logic
} // namespace PPTX
