/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
