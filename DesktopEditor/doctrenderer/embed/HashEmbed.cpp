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

#include "HashEmbed.h"

JSSmart<CJSValue> CHashEmbed::hash(JSSmart<CJSValue> data, JSSmart<CJSValue> size, JSSmart<CJSValue> alg)
{
	std::string _data = data->toStringA();
	int _size = size->toInt32();
	int _alg = alg->toInt32();
	unsigned char* pData = m_pHash->hash(reinterpret_cast<const unsigned char*>(_data.c_str()), _size, _alg);
	return CJSContext::createUint8Array(pData, CHash::getDigestLength(static_cast<CHash::HashAlgs>(_alg)), false);
}

JSSmart<CJSValue> CHashEmbed::hash2(JSSmart<CJSValue> password, JSSmart<CJSValue> salt, JSSmart<CJSValue> spinCount, JSSmart<CJSValue> alg)
{
	std::string _password = password->toStringA();
	std::string _salt = salt->toStringA();
	int _spinCount = spinCount->toInt32();
	int _alg = alg->toInt32();
	unsigned char* pData = m_pHash->hash2(reinterpret_cast<const char*>(_password.c_str()), reinterpret_cast<const char*>(_salt.c_str()), _spinCount, _alg);
	return CJSContext::createUint8Array(pData, CHash::getDigestLength(static_cast<CHash::HashAlgs>(_alg)), false);
}
