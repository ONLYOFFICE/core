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
#include "SimpleTypes_Rtf.h"

namespace SimpleTypes
{
	EOptimizeForBrowserTarget COptimizeForBrowserTarget::FromString(const std::wstring &sValue)
	{
		if       ( _T("W3C XHTML+CSS1") == sValue ) this->m_eValue = optforbrowserXhtmlCSS1;
		else if  ( _T("W3C HTML4+CSS1") == sValue ) this->m_eValue = optforbrowserHtml4CSS1;
		else if  ( _T("W3C XHTML+CSS2") == sValue ) this->m_eValue = optforbrowserXhtmlCSS2;
		else if  ( _T("W3C HTML4+CSS2") == sValue ) this->m_eValue = optforbrowserHtml4CSS2;
		else                                        this->m_eValue = optforbrowserHtml4CSS2;

		return this->m_eValue;
	}

	std::wstring COptimizeForBrowserTarget::ToString() const
	{
		switch(this->m_eValue)
		{
		case optforbrowserXhtmlCSS1 : return _T("W3C XHTML+CSS1");
		case optforbrowserHtml4CSS1 : return _T("W3C HTML4+CSS1 ");
		case optforbrowserXhtmlCSS2 : return _T("W3C XHTML+CSS2 ");
		case optforbrowserHtml4CSS2 : return _T("W3C HTML4+CSS2 ");
		default                     : return _T("W3C HTML4+CSS2 ");
		}
	}

} // SimpleTypes
