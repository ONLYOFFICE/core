﻿/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "DrawingExt.h"
#include "../../XlsxFormat/Worksheets/Sparkline.h"
#include "../../XlsxFormat/Table/Table.h"
#include "../Diagram/DiagramData.h"
#include "../../XlsxFormat/Worksheets/ConditionalFormatting.h"

namespace OOX
{
	namespace Drawing
	{
		CCompatExt::~CCompatExt()
		{
		}

		COfficeArtExtension::~COfficeArtExtension()
		{
			m_oSparklineGroups.reset();
			m_oAltTextTable.reset();
			m_oDataModelExt.reset();
			m_oCompatExt.reset();

			// delete Conditional Formatting
			for (size_t nIndex = 0, nLength = m_arrConditionalFormatting.size(); nIndex < nLength; ++nIndex)
			{
				delete m_arrConditionalFormatting[nIndex];
			}
			m_arrConditionalFormatting.clear();
		}
		void COfficeArtExtension::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

            if ((m_sUri.IsInit()) && (*m_sUri == L"{63B3BB69-23CF-44E3-9099-C40C66FF867C}"	||
                                      *m_sUri == L"{05C60535-1F16-4fd2-B633-F4F36F0B64E0}"	|| 
									  *m_sUri == L"{504A1905-F514-4f6f-8877-14C23A59335A}"	|| 
									  *m_sUri == L"{78C0D931-6437-407d-A8EE-F0AAD7539E65}"	||
									  *m_sUri == L"http://schemas.microsoft.com/office/drawing/2008/diagram"))   
			{
				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if (sName == L"compatExt")//2.3.1.2 compatExt
					{	//attributes spid -https://msdn.microsoft.com/en-us/library/hh657207(v=office.12).aspx
						m_oCompatExt = oReader;
					}
					else if (sName == L"sparklineGroups")
					{
						m_oSparklineGroups = oReader;
					}
					else if (sName == L"dataModelExt")
					{
						m_oDataModelExt = oReader;
					}
					else if (sName == _T("table"))
					{
						m_oAltTextTable = oReader;
					}
					else if (sName == L"conditionalFormattings")
					{
						if ( oReader.IsEmptyNode() )
							continue;

						int nCurDepth1 = oReader.GetDepth();
						while( oReader.ReadNextSiblingNode( nCurDepth1 ) )
						{						
							m_arrConditionalFormatting.push_back(new OOX::Spreadsheet::CConditionalFormatting(oReader));
						}
					}
				}
			}
			else
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
		}
        std::wstring COfficeArtExtension::toXML() const
        {
            return toXMLWithNS(_T("a:"));
        }
        std::wstring COfficeArtExtension::toXMLWithNS(const std::wstring& sNamespace) const
		{
            std::wstring sResult = _T("<");
            sResult += sNamespace;
            sResult += _T("ext");

            if ( m_sUri.IsInit() )
			{
                sResult += L" uri=\"" + m_sUri.get2() + L"\"";
			}

			if (!m_sAdditionalNamespace.empty())
			{
				sResult += L" " + m_sAdditionalNamespace;
			}
			
			sResult += _T(">");

			if(m_oCompatExt.IsInit())
			{
				sResult += m_oCompatExt->toXML();
			}
			if(m_oSparklineGroups.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSparklineGroups->toXML(writer);
                sResult += writer.GetData().c_str();
			}
			if(m_oAltTextTable.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oAltTextTable->toXML(writer);
				sResult += writer.GetData().c_str();
			}

            sResult += _T("</");
            sResult += sNamespace;
            sResult += _T("ext>");

			return sResult;
		}
	}
}
