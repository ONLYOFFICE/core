/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "MetadataStream.h"

#include "Biff12_records/CommonRecords.h"
#include "Biff12_unions/ESMDTINFO.h"
#include "Biff12_unions/ESSTR.h"
#include "Biff12_unions/ESMDX.h"
#include "Biff12_unions/ESFMD.h"
#include "Biff12_unions/ESMDB.h"

#include "Biff12_records/BeginMetadata.h"
#include "Biff12_records/BeginEsmdb.h"
#include "Biff12_records/EndMetadata.h"

using namespace XLS;

namespace XLSB
{

MetadataStream::MetadataStream()
{
}

MetadataStream::~MetadataStream()
{
}


BaseObjectPtr MetadataStream::clone()
{
    return BaseObjectPtr(new MetadataStream(*this));
}

const bool MetadataStream::loadContent(BinProcessor& proc)
{	
    while (true)
    {
        CFRecordType::TypeId type = proc.getNextRecordType();

        if (type == rt_NONE) break;

        switch(type)
        {
        case rt_BeginEsmdtinfo:
        {
            if (proc.optional<ESMDTINFO>())
            {
                m_ESMDTINFO = elements_.back();
                elements_.pop_back();
            }
        }break;
        case rt_BeginEsstr:
        {
            if (proc.optional<ESSTR>())
            {
                m_ESSTR = elements_.back();
                elements_.pop_back();
            }
        }break;
        case rt_BeginEsmdx:
        {
            if (proc.optional<ESMDX>())
            {
                m_ESMDX = elements_.back();
                elements_.pop_back();
            }
        }break;
        case rt_BeginEsfmd:
        {
            int count = proc.repeated<ESFMD>(0, 0);
            while(count > 0)
            {
                m_ESFMDs.insert(m_ESFMDs.begin(), elements_.back());
                elements_.pop_back();
                count--;
            }
        }break;
        case rt_BeginEsmdb:
        {
            if (proc.optional<ESMDB>())
            {
                auto beginPtr = static_cast<BeginEsmdb*>(static_cast<ESMDB*>(elements_.back().get())->m_BrtBeginEsmdb.get());
                if(beginPtr->fCellMeta)
                    m_CellMetadataBlocks = elements_.back();
                else
                    m_ValueMetadataBlocks = elements_.back();
                elements_.pop_back();
            }
        }break;
        case rt_EndMetadata:
        {
            return true;
        }break;
        default://skip
        {
            proc.SkipRecord();
        }break;
        }
    }
	return true;
}

const bool MetadataStream::saveContent(XLS::BinProcessor & proc)
{
    proc.mandatory<XLSB::BeginMetadata>();
    if (m_ESMDTINFO != nullptr)
        proc.mandatory(*m_ESMDTINFO);
    if (m_ESSTR != nullptr)
        proc.mandatory(*m_ESSTR);
    if (m_ESMDX != nullptr)
        proc.mandatory(*m_ESMDX);
    for(auto i:m_ESFMDs)
        proc.mandatory(*i);
    if (m_CellMetadataBlocks != nullptr)
        proc.mandatory(*m_CellMetadataBlocks);
    if (m_ValueMetadataBlocks != nullptr)
        proc.mandatory(*m_ValueMetadataBlocks);   
    proc.mandatory<XLSB::EndMetadata>();
    return true;
}

} // namespace XLSB
