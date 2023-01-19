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
#include "PivotCacheDefinitionExt.h"
#include "../../XlsbFormat/Biff12_unions/PCD14.h"
#include "../../XlsbFormat/Biff12_records/BeginPCD14.h"

namespace OOX
{
namespace Spreadsheet
{

void CPivotCacheDefinitionExt::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
    writer.StartNode(sName);
    writer.StartAttributes();
    WritingNullable(m_oPivotCacheId, writer.WriteAttribute(L"pivotCacheId", *m_oPivotCacheId););
    writer.EndAttributes();

    writer.EndNode(sName);
}
void CPivotCacheDefinitionExt::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
    ReadAttributes(oReader);
    if (oReader.IsEmptyNode())
        return;
    oReader.ReadTillEnd();
}
void CPivotCacheDefinitionExt::fromBin(XLS::BaseObjectPtr& obj)
{
    if(obj->get_type() == XLS::typePCD14)
    {
        auto ptr = static_cast<XLSB::PCD14*>(obj.get());
        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginPCD14);
        }
    }
}
void CPivotCacheDefinitionExt::ReadAttributes(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::BeginPCD14*>(obj.get());
    if(ptr != nullptr)
    {
        m_oPivotCacheId = ptr->icacheId;
    }
}
void CPivotCacheDefinitionExt::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
    WritingElement_ReadAttributes_Start( oReader )
        WritingElement_ReadAttributes_Read_if	( oReader, L"pivotCacheId", m_oPivotCacheId )
    WritingElement_ReadAttributes_End( oReader )
}

} //Spreadsheet
} // namespace OOX
