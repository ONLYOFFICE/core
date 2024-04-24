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
