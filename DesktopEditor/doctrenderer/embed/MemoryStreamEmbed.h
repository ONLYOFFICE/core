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
