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

#include "PROTECTION.h"

#include "../Biff_records/WinProtect.h"
#include "../Biff_records/Protect.h"
#include "../Biff_records/Password.h"
#include "../Biff_records/Prot4Rev.h"
#include "../Biff_records/Prot4RevPass.h"

namespace XLS
{


PROTECTION::PROTECTION()
{
}


PROTECTION::~PROTECTION()
{
}


BaseObjectPtr PROTECTION::clone()
{
	return BaseObjectPtr(new PROTECTION(*this));
}


// PROTECTION = WinProtect Protect Password Prot4Rev Prot4RevPass
const bool PROTECTION::loadContent(BinProcessor& proc)
{
	if (proc.optional<WinProtect>())
	{
		m_WinProtect = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<Protect>())
	{
		m_Protect = elements_.back();
		elements_.pop_back();
	}	
	if (proc.optional<Password>())
	{
		m_Password = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<Prot4Rev>())
	{
		m_Prot4Rev = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<Prot4RevPass>())
	{
		m_Prot4RevPass = elements_.back();
		elements_.pop_back();
	}
	return m_WinProtect || m_Protect || m_Password;
}
int PROTECTION::serialize (std::wostream & _stream)
{
	WinProtect		*win_protect	= dynamic_cast<WinProtect*>		(m_WinProtect.get());
	Protect			*protect		= dynamic_cast<Protect*>		(m_Protect.get());
	Password		*password		= dynamic_cast<Password*>		(m_Password.get());

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"workbookProtection") 
		{
			if (password)
			{
                CP_XML_ATTR(L"workbookPassword", password->wPassword);
			}
			if (win_protect)
			{
                CP_XML_ATTR(L"lockWindows", (win_protect->fLockWn ? 1 : 0));
			}
			if (protect)
			{
                CP_XML_ATTR(L"lockStructure", (protect->fLock ? 1 : 0));
			}
			
		}
		
	}
	return 0;
}


} // namespace XLS

