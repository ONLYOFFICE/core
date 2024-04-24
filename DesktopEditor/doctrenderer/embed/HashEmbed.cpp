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
