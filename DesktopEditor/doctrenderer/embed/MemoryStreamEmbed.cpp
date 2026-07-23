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

#include "MemoryStreamEmbed.h"

JSSmart<CJSValue> CMemoryStreamEmbed::Copy(JSSmart<CJSValue> stream, JSSmart<CJSValue> pos, JSSmart<CJSValue> len)
{
	JSSmart<CJSObject> _obj = stream->toObject();
	NSMemoryStream::CMemoryStream* pStream = (NSMemoryStream::CMemoryStream*)(_obj->getNative());
	m_pInternal->Copy(pStream, pos->toInt32(), len->toInt32());
	return NULL;
}

JSSmart<CJSValue> CMemoryStreamEmbed::ClearNoAttack()
{
	m_pInternal->ClearNoAttack();
	return NULL;
}

JSSmart<CJSValue> CMemoryStreamEmbed::WriteByte(JSSmart<CJSValue> value)
{
	m_pInternal->WriteBYTE((BYTE)value->toInt32());
	return NULL;
}

JSSmart<CJSValue> CMemoryStreamEmbed::WriteBool(JSSmart<CJSValue> value)
{
	m_pInternal->WriteBYTE(value->toBool() ? 1 : 0);
	return NULL;
}

JSSmart<CJSValue> CMemoryStreamEmbed::WriteLong(JSSmart<CJSValue> value)
{
	m_pInternal->WriteLONG((LONG)value->toInt32());
	return NULL;
}

JSSmart<CJSValue> CMemoryStreamEmbed::WriteDouble(JSSmart<CJSValue> value)
{
	m_pInternal->WriteLONG((LONG)(value->toDouble() * 100000));
	return NULL;
}

JSSmart<CJSValue> CMemoryStreamEmbed::WriteDouble2(JSSmart<CJSValue> value)
{
	m_pInternal->WriteDouble(value->toDouble());
	return NULL;
}

JSSmart<CJSValue> CMemoryStreamEmbed::WriteStringA(JSSmart<CJSValue> value)
{
	std::string param = value->toStringA();
	m_pInternal->WriteStringA2(param.c_str(), (int)param.length());
	return NULL;
}

JSSmart<CJSValue> CMemoryStreamEmbed::WriteString(JSSmart<CJSValue> value)
{
	std::wstring param = value->toStringW();
	m_pInternal->WriteString(param.c_str(), (int)param.length());
	return NULL;
}

JSSmart<CJSValue> CMemoryStreamEmbed::WriteString2(JSSmart<CJSValue> value)
{
	std::wstring param = value->toStringW();
	m_pInternal->WriteString2(param.c_str(), (int)param.length());
	return NULL;
}
