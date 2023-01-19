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

#include "LBL.h"
#include "../Biff_records/Lbl.h"
#include "../Biff_records/NameCmt.h"
#include "../Biff_records/NameFnGrp12.h"
#include "../Biff_records/NamePublish.h"
#include "../Biff_structures/PtgRef3d.h"
#include "../Biff_structures/PtgArea3d.h"

#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{
LBL::LBL()
{
	isSerialize = false;
}
LBL::~LBL()
{
}
BaseObjectPtr LBL::clone()
{
	return BaseObjectPtr(new LBL(*this));
}

// LBL = Lbl [NameCmt] [NameFnGrp12] [NamePublish]
const bool LBL::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Lbl>())
	{
		return false;
	}
	
	global_info_ = proc.getGlobalWorkbookInfo();
	
	m_Lbl = elements_.back();
	elements_.pop_back();

	Lbl *lbl = dynamic_cast<Lbl*>(m_Lbl.get());
	if (!lbl) return false;

	std::wstring name;

	if (lbl->fBuiltin)	name = lbl->Name;	
	if (name.empty())	name = lbl->Name_bin.value();
	
    NameCmt namecmt(name);
    if (proc.optional(namecmt))
	{
		m_NameCmt = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<NameFnGrp12>())
	{
		m_NameFnGrp12 = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<NamePublish>())
	{
		m_NamePublish = elements_.back();
		elements_.pop_back();
	}
	return true;
}
int LBL::serialize(std::wostream & stream)
{
	Lbl *lbl = dynamic_cast<Lbl*>(m_Lbl.get());
	if (lbl == NULL) return 0;
		
	std::wstring name;
	if (lbl->fBuiltin)	name = lbl->Name;	
	if (name.empty())	name = lbl->Name_bin.value();
			
	std::wstring value = lbl->rgce.getAssembledFormula(lbl->itab == 0 ? true : false);
	std::wstring description;

	if (value.empty()) return 0;

	int res = 0;

	if ((lbl->itab == 0) && (res = value.find(L"#REF")) >= 0)
	{
		for (size_t i = 0 ; i < lbl->rgce.rgce.sequence.size(); i++)
		{
			PtgRef3d* ptg = dynamic_cast<PtgRef3d*>(lbl->rgce.rgce.sequence[i].get());
			if (ptg) 
			{
				description = value;
				value = std::wstring(L"#REF!");
				break;
			}
		
			PtgArea3d* ptg1 = dynamic_cast<PtgArea3d*>(lbl->rgce.rgce.sequence[i].get());
			if (ptg1) 
			{
				description = value;
				value = std::wstring(L"#REF!");
				break;
			}
		}
	}

	if (value == L"#REF") value = L"#REF!";

	std::wstring name_ = XmlUtils::EncodeXmlString(name);

	if (name_.empty()) name_ = L"_";

	std::wstring name_check = name_ + L"sheet(" + std::to_wstring(lbl->itab) + L")";

	std::map<std::wstring, int>::iterator pFind = global_info_->mapDefineNamesSerialized.find(name_check);
	if (pFind == global_info_->mapDefineNamesSerialized.end())
	{
		global_info_->mapDefineNamesSerialized.insert(std::make_pair(name_check, 1));
	}
	else
	{
		pFind->second++;
		name_ = L"_" + std::to_wstring(pFind->second) + L"_" + name_;
	}

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"definedName")
		{
			CP_XML_ATTR(L"name", name_);

			if (!description.empty())
			{
				std::wstring description_ = XmlUtils::EncodeXmlString(description);
				if (description_.length() > 255)
				{
					description_ = description_.substr(0, 255);
				}
				CP_XML_ATTR(L"comment", description_);
			}

			if (lbl->itab != 0)
			{
				CP_XML_ATTR(L"localSheetId", lbl->itab - 1);
			}
			if (lbl->fHidden)
			{
				CP_XML_ATTR(L"hidden", 1);
			}

			CP_XML_STREAM() << XmlUtils::EncodeXmlString(value);
		}
	}
	return 0;
}
} // namespace XLS

