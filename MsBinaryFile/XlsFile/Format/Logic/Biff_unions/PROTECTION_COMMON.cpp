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

#include "PROTECTION_COMMON.h"

#include "../Biff_records/Protect.h"
#include "../Biff_records/ScenarioProtect.h"
#include "../Biff_records/ObjProtect.h"
#include "../Biff_records/Password.h"

#include "../../../../../OfficeCryptReader/source/CryptTransform.h"

namespace XLS
{

PROTECTION_COMMON::PROTECTION_COMMON()
{
}

PROTECTION_COMMON::~PROTECTION_COMMON()
{
}

BaseObjectPtr PROTECTION_COMMON::clone()
{
	return BaseObjectPtr(new PROTECTION_COMMON(*this));
}


// PROTECTION_COMMON = [Protect] [ScenarioProtect] [ObjProtect] [Password]
const bool PROTECTION_COMMON::loadContent(BinProcessor& proc)
{
	if (proc.optional<Protect>())
	{
		m_Protect = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<ScenarioProtect>())
	{
		m_ScenarioProtect = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<ObjProtect>())
	{
		m_ObjProtect = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<Password>())
	{
		m_Password = elements_.back();
		elements_.pop_back();

		////test
		//Password* passw = dynamic_cast<Password*>(m_Password.get());

		//CRYPT::_ecmaWriteProtectData data;
		//data.hashValue = std::string(passw->wPassword.begin(), passw->wPassword.end());

		//CRYPT::ECMAWriteProtect protect;
		//protect.SetCryptData(data);
		//protect.SetPassword(L"123");

		//bool res = protect.VerifyWrike();
	}

	return m_Protect || m_ScenarioProtect || m_ObjProtect || m_Password;
}

int PROTECTION_COMMON::serialize (std::wostream & _stream)
{
	Protect			*protect	= dynamic_cast<Protect*>		(m_Protect.get());
	Password		*password	= dynamic_cast<Password*>		(m_Password.get());
	ScenarioProtect	*scenario	= dynamic_cast<ScenarioProtect*>(m_ScenarioProtect.get());
	ObjProtect		*object		= dynamic_cast<ObjProtect*>		(m_ObjProtect.get());

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"sheetProtection") 
		{
			if (password)
			{
                CP_XML_ATTR(L"password", password->wPassword);
			}
			if (protect)
			{
                CP_XML_ATTR(L"sheet", (protect->fLock ? 1 : 0));
			}
			if (object)
			{
                CP_XML_ATTR(L"objects", (object->fLockObj ? 1 : 0));
			}
			if (scenario)
			{
                CP_XML_ATTR(L"scenarios", (scenario->fScenProtect ? 1 : 0));
			}
			CP_XML_ATTR(L"formatCells", 0);
			CP_XML_ATTR(L"selectLockedCells", 1);
			CP_XML_ATTR(L"autoFilter", 0);
			//CP_XML_ATTR(L"selectUnlockedCells", 1);
		}
		
	}
	return 0;
}

} // namespace XLS

