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

#include "DREF.h"
#include "../Biff_records/DConName.h"
#include "../Biff_records/DConBin.h"
#include "../Biff_records/DConRef.h"

namespace XLS
{


DREF::DREF()
{
}

DREF::~DREF()
{
}

BaseObjectPtr DREF::clone()
{
	return BaseObjectPtr(new DREF(*this));
}

// DREF = DConName / DConBin / DConRef
const bool DREF::loadContent(BinProcessor& proc)
{
	if(!proc.optional<DConName>())
	{
		if(!proc.optional<DConBin>())
		{
			if(!proc.optional<DConRef>())
			{
				return false;
			}
		}
	}
	m_DCon = elements_.back();
	elements_.pop_back();
	return true;
}

int DREF::serialize(std::wostream & strm)
{
	if (!m_DCon)return 0;

	DConName*	name	= dynamic_cast<DConName*>(m_DCon.get());
	DConBin*	bin		= dynamic_cast<DConBin*>(m_DCon.get());
	DConRef*	ref		= dynamic_cast<DConRef*>(m_DCon.get());
	
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"cacheSource")
		{
			if (name)
			{
				CP_XML_ATTR(L"type", L"worksheet");
				CP_XML_NODE(L"worksheetSource")
				{
					CP_XML_ATTR(L"name", name->stName.value());
				}
			}
			else if(bin)
			{
				CP_XML_ATTR(L"type", L"worksheet");
				CP_XML_NODE(L"worksheetSource")
				{
					switch(bin->nBuiltin)
					{
						case 0x0000:	CP_XML_ATTR(L"name", L"_xlnm.Consolidate_Area"); break;
						case 0x0001:	CP_XML_ATTR(L"name", L"_xlnm.Auto_Open");		break;
						case 0x0002:	CP_XML_ATTR(L"name", L"_xlnm.Auto_Close");		break;
						case 0x0003:	CP_XML_ATTR(L"name", L"_xlnm.Extract");			break;
						case 0x0004:	CP_XML_ATTR(L"name", L"_xlnm.Database");		break;
						case 0x0005:	CP_XML_ATTR(L"name", L"_xlnm.Criteria");		break;
						case 0x0006:	CP_XML_ATTR(L"name", L"_xlnm.Print_Area");		break;
						case 0x0007:	CP_XML_ATTR(L"name", L"_xlnm.Print_Titles");	break;
						case 0x0008:	CP_XML_ATTR(L"name", L"_xlnm.Recorder");		break;
						case 0x0009:	CP_XML_ATTR(L"name", L"_xlnm.Data_Form");		break;
						case 0x000a:	CP_XML_ATTR(L"name", L"_xlnm.Auto_Activate");	break;
						case 0x000b:	CP_XML_ATTR(L"name", L"_xlnm.Auto_Deactivate");	break;
						case 0x000c:	CP_XML_ATTR(L"name", L"_xlnm.Sheet_Title");		break;
						case 0x000d:	CP_XML_ATTR(L"name", L"_xlnm._FilterDatabase");	break;	//??
					}
				}
			}
			else if(ref)
			{
				ref->check_external();

				CP_XML_ATTR(L"type", L"worksheet");
				CP_XML_NODE(L"worksheetSource")
				{
					CP_XML_ATTR(L"ref", ref->ref.toString());
					CP_XML_ATTR(L"sheet", ref->sheet_name);

					if (ref->index_external >= 0)
					{
						CP_XML_ATTR(L"r:id", L"extId" + std::to_wstring(ref->index_external + 1));
					}
				}
			}
		}
	}
	return 0;
}


} // namespace XLS

