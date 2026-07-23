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

#ifndef _BUILD_NATIVE_MEMORYSTREAM_EMBED_H_
#define _BUILD_NATIVE_MEMORYSTREAM_EMBED_H_

#include "../../fontengine/MemoryStream.h"
#include "../js_internal/js_base.h"

using namespace NSJSBase;
class JS_DECL CMemoryStreamEmbed : public CJSEmbedObject
{
public:
	NSMemoryStream::CMemoryStream* m_pInternal;

public:
	CMemoryStreamEmbed() : m_pInternal(new NSMemoryStream::CMemoryStream()) {}
	~CMemoryStreamEmbed() { RELEASEOBJECT(m_pInternal); }

	virtual void* getObject() override { return (void*)m_pInternal; }

public:
	JSSmart<CJSValue> Copy(JSSmart<CJSValue> stream, JSSmart<CJSValue> pos, JSSmart<CJSValue> len);
	JSSmart<CJSValue> ClearNoAttack();

	JSSmart<CJSValue> WriteByte(JSSmart<CJSValue> value);
	JSSmart<CJSValue> WriteBool(JSSmart<CJSValue> value);
	JSSmart<CJSValue> WriteLong(JSSmart<CJSValue> value);
	JSSmart<CJSValue> WriteDouble(JSSmart<CJSValue> value);
	JSSmart<CJSValue> WriteDouble2(JSSmart<CJSValue> value);
	JSSmart<CJSValue> WriteStringA(JSSmart<CJSValue> value);
	JSSmart<CJSValue> WriteString(JSSmart<CJSValue> value);
	JSSmart<CJSValue> WriteString2(JSSmart<CJSValue> value);

	DECLARE_EMBED_METHODS
};

#endif // _BUILD_NATIVE_MEMORYSTREAM_EMBED_H_
