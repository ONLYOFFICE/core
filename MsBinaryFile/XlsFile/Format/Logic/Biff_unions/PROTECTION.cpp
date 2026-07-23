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

const bool PROTECTION::saveContent(BinProcessor& proc)
{
    if(m_WinProtect != nullptr)
        proc.mandatory(*m_WinProtect);
    else
        proc.mandatory<WinProtect>();
    if(m_Protect != nullptr)
        proc.mandatory(*m_Protect);
    else
        proc.mandatory<Protect>();
    if(m_Password != nullptr)
        proc.mandatory(*m_Password);
    else
        proc.mandatory<Password>();
    if(m_Prot4Rev != nullptr)
        proc.mandatory(*m_Prot4Rev);
    else
        proc.mandatory<Prot4Rev>();
    if(m_Prot4RevPass != nullptr)
        proc.mandatory(*m_Prot4RevPass);
    else
        proc.mandatory<Prot4RevPass>();
    return true;
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

